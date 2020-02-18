#include "wx/wxprec.h"
#include "memoryimagestore.h"

MemorySaveThread::MemorySaveThread(wxZipOutputStream& store, wxMessageQueue<std::pair<wxImage*, uint32_t>>& mqueue):
	m_store(store), m_mqueue(mqueue)
{
	
}

void* MemorySaveThread::Entry()
{
	int i = 0;

	while (true)
	{
		std::pair<wxImage*, uint32_t> data;
		m_mqueue.Receive(data);
		if (!data.first)
		{
			break;
		}
		i++;
		wxString tempName = wxString::Format(wxT("%d.tmp"), i);

		m_store.PutNextEntry(tempName);
		auto s = data.first->GetSize();
		auto bits = data.first->GetData();
		m_store.WriteAll(bits, s.x * s.y * 3l);
		m_store.CloseEntry();
		delete data.first;
	}
	return nullptr;
}

MemoryImageStoreBuilder::MemoryImageStoreBuilder(const wxSize& imageSize):
	m_mqueue(), m_imageSize(imageSize), m_memOStream(new wxMemoryOutputStream()), m_zipOutputStream(*m_memOStream),
	m_backgroundThread(m_zipOutputStream, m_mqueue)
{

}

MemoryImageStoreBuilder::~MemoryImageStoreBuilder()
{
	auto thread = m_backgroundThread.GetThread();
	if (thread != nullptr)
	{
		m_mqueue.Clear();
		m_mqueue.Post(std::pair<wxImage*, uint32_t>(nullptr, 0));
	}
	if (m_memOStream != nullptr)
	{
		delete m_memOStream;
	}
}

void MemoryImageStoreBuilder::PushBack(const wxImage& image, uint32_t duration)
{
	m_durations.push_back(duration);
	m_mqueue.Post(std::pair<wxImage*, uint32_t>(new wxImage(image), duration));
}

size_t MemoryImageStoreBuilder::GetSize() const
{
	return m_durations.size();
}

IImageStore* MemoryImageStoreBuilder::BuildStore()
{
	auto thread = m_backgroundThread.GetThread();
	if (thread != nullptr)
	{
		m_mqueue.Post(std::pair<wxImage*, uint32_t>(nullptr, 0));
		thread->Wait();
	}
	m_zipOutputStream.Close();
	m_zipOutputStream.UnRef();
	auto temp = m_memOStream;
	m_memOStream = nullptr;
	return new MemoryImageStore(temp, m_imageSize.GetWidth(), m_imageSize.GetHeight(), m_durations);
}

MemoryImageStore::MemoryImageStore(
	wxMemoryOutputStream* memoryOStream, decltype(m_imageWidth) width, decltype(m_imageHeight) height, const std::vector<uint32_t>& durations):
	m_memoryOStream(memoryOStream),
	m_zipInputStream(memoryOStream->GetOutputStreamBuffer()->GetInputStream()),
	m_imageHeight(height),
	m_imageWidth(width)
{
	auto& tar = m_zipInputStream;
	auto it = durations.begin();
	if (tar.Eof() || tar.CanRead() == false || durations.size() == 0)
	{
		return;
	}

	for (auto entry = tar.GetNextEntry(); entry != nullptr; entry = tar.GetNextEntry())
	{
		m_store.push_back(std::pair<wxZipEntry*, uint32_t>(entry, *it));
		it++;
	}
}

MemoryImageStore::~MemoryImageStore()
{
	Clear();
}

IImageStoreBuilder* MemoryImageStore::CreateBuilder(const wxSize& imageSize)
{
	return new MemoryImageStoreBuilder(imageSize);
}

std::pair<wxImage, uint32_t> MemoryImageStore::Get(size_t index)
{
	if (index < m_store.size())
	{
		auto entry = m_store[index].first;
		m_zipInputStream.OpenEntry(*entry);
		uint8_t* buffer = (uint8_t*)malloc(entry->GetSize());
		wxMemoryOutputStream memStream(buffer, entry->GetSize());
		m_zipInputStream.Read(memStream);
		memStream.Close();
		m_zipInputStream.CloseEntry();

		auto image = wxImage(m_imageWidth, m_imageHeight);
		image.SetData(buffer);

		return std::pair<wxImage, uint32_t>(std::move(image), m_store[index].second);
	}
	return std::pair<wxImage, uint32_t>();
}

wxSize MemoryImageStore::GetImageSize() const
{
	return wxSize(m_imageWidth, m_imageHeight);
}

void MemoryImageStore::Clear()
{
	for (auto it : m_store)
	{
		delete it.first;
	}
	m_store.clear();
	m_zipInputStream.UnRef();
}
