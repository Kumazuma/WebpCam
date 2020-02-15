#include "wx/wxprec.h"
#include "temporalfileimagestore.h"

FileImageStore::FileImageStore(wxString fileName, std::vector<uint32_t>&& durations):
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

std::pair<wxImage, uint32_t> FileImageStore::Get(size_t index)
{
	auto entry = m_store[index].first;
	m_tarInputStream.OpenEntry(*entry);
	auto image = wxImage(m_tarInputStream, wxBITMAP_TYPE_PNG);
	m_tarInputStream.CloseEntry();
	return std::pair<wxImage, uint32_t>(image, m_store[index].second);
}

void FileImageStore::Clear()
{
	for (auto it : m_store)
	{
		delete it.first;
	}
	m_store.clear();
}

FileSaveThread::FileSaveThread(
	wxTarOutputStream& store,
	wxMessageQueue<std::pair<wxImage, uint32_t>>& mqueue):
	m_store(store),
	m_mqueue(mqueue)
{

}

void* FileSaveThread::Entry()
{
	int i = 0;
	while (true)
	{
		std::pair<wxImage, uint32_t> data;
		m_mqueue.Receive(data);
		if (!data.first.IsOk())
		{
			break;
		}
		i++;
		wxString tempName = wxString::Format(wxT("%d.tmp"), i);
		//wxTarEntry* entry = new wxTarEntry(tempName);
		m_store.PutNextEntry(tempName);
		data.first.SaveFile(m_store, wxBITMAP_TYPE_PNG);
		m_store.CloseEntry();
	}
	return nullptr;
}

FileImageStoreBuilder::FileImageStoreBuilder():
	m_fileName(wxFileName::CreateTempFileName(wxT("awebpcap"))),
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
		m_mqueue.Post(std::pair<wxImage, uint32_t>(wxImage(), 0));
	}
}

void FileImageStoreBuilder::PushBack(const wxImage& image, uint32_t duration)
{
	m_durations.push_back(duration);
	m_mqueue.Post(std::pair<wxImage, uint32_t>(image, duration));
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
		m_mqueue.Post(std::pair<wxImage, uint32_t>(wxImage(), 0));
		thread->Wait();
	}
	m_taroutputStream.Close();
	m_fileOStream.Close();
	
	return new FileImageStore(m_fileName,std::move(m_durations));
}
