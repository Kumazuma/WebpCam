#include "wx/wxprec.h"
#include "temporalfileimagestore.h"
#include <wx/mstream.h>
FileImageStore::FileImageStore(wxString fileName,
	decltype(m_imageWidth) width,
	decltype(m_imageHeight) height, const std::vector<uint32_t> & durations) :
	m_imageHeight(height),
	m_imageWidth(width),
	m_fileName(fileName),
	m_fileIStream(fileName),
	m_tarInputStream(m_fileIStream)
{
	auto& tar = m_tarInputStream;
	auto it = durations.begin();
	for (auto entry = tar.GetNextEntry(); entry != nullptr; entry = tar.GetNextEntry())
	{
		m_store.push_back(std::pair<wxTarEntry*, uint32_t>(entry, *it));
		it++;
	}
}


FileImageStore::~FileImageStore()
{
	Clear();
	
}

IImageStoreBuilder* FileImageStore::CreateBuilder(const wxSize& imageSize)
{
	return new FileImageStoreBuilder(imageSize);
}

std::pair<wxImage, uint32_t> FileImageStore::Get(size_t index)
{
	auto entry = m_store[index].first;
	m_tarInputStream.OpenEntry(*entry);
	uint8_t* buffer = (uint8_t*)malloc(entry->GetSize());
	wxMemoryOutputStream memStream(buffer, entry->GetSize());
	m_tarInputStream.Read(memStream);
	memStream.Close();
	m_tarInputStream.CloseEntry();

	auto image = wxImage(m_imageWidth, m_imageHeight);
	image.SetData(buffer);
	return std::pair<wxImage, uint32_t>(std::move(image), m_store[index].second);
}

wxSize FileImageStore::GetImageSize() const
{
	return wxSize(m_imageWidth, m_imageHeight);
}

void FileImageStore::Clear()
{
	for (auto it : m_store)
	{
		delete it.first;
	}
	m_store.clear();

	m_tarInputStream.UnRef();
	m_fileIStream.GetFile()->Detach();
	m_fileIStream.UnRef();
	wxFile file(m_fileName, wxFile::write);
	file.Flush();
}

FileSaveThread::FileSaveThread(
	wxTarOutputStream& store,
	wxMessageQueue<std::pair<wxImage*, uint32_t>>& mqueue):
	m_store(store),
	m_mqueue(mqueue)
{

}

void* FileSaveThread::Entry()
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
		//wxTarEntry* entry = new wxTarEntry(tempName);
		m_store.PutNextEntry(tempName);
		auto s = data.first->GetSize();
		//data.first.SaveFile(m_store, wxBITMAP_TYPE_PNG);
		auto bits = data.first->GetData();
		m_store.WriteAll(bits, s.x * s.y * 3);
		m_store.CloseEntry();
		delete data.first;
	}
	return nullptr;
}

FileImageStoreBuilder::FileImageStoreBuilder(const wxSize& imageSize):
	m_fileName(wxFileName::CreateTempFileName(wxT("awebpcap"))),
	m_imageSize(imageSize),
	m_fileOStream(m_fileName),
	m_taroutputStream(m_fileOStream),
	m_mqueue(),
	m_backgroundThread(m_taroutputStream, m_mqueue)
{
	m_backgroundThread.CreateThread();
	m_backgroundThread.GetThread()->Run();
}

FileImageStoreBuilder::~FileImageStoreBuilder()
{
	auto thread = m_backgroundThread.GetThread();
	if (thread != nullptr)
	{
		m_mqueue.Clear();
		m_mqueue.Post(std::pair<wxImage*, uint32_t>(nullptr, 0));
	}
}

void FileImageStoreBuilder::PushBack(const wxImage& image, uint32_t duration)
{
	m_durations.push_back(duration);
	m_mqueue.Post(std::pair<wxImage*, uint32_t>(new wxImage(image), duration));
}

size_t FileImageStoreBuilder::GetSize() const
{
	return m_durations.size();
}

IImageStore* FileImageStoreBuilder::BuildStore()
{
	auto thread = m_backgroundThread.GetThread();
	if (thread != nullptr)
	{
		m_mqueue.Post(std::pair<wxImage*, uint32_t>(nullptr, 0));
		thread->Wait();
	}
	m_taroutputStream.Close();
	m_fileOStream.Close();
	m_taroutputStream.UnRef();
	m_fileOStream.UnRef();
	return new FileImageStore(m_fileName,m_imageSize.GetWidth(),m_imageSize.GetHeight(), m_durations);
}
