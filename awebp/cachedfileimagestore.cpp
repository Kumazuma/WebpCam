#include "wx/wxprec.h"
#include "cfimagestore.h"
#include <wx/log.h>
#ifndef __WXMSW__
#include <unistd.h>
#endif
wxAtomicInt g_p = 0;
wxString GetNUMBER()
{
	auto r = wxString::Format(wxT("./%04d"), g_p);
	wxAtomicInc(g_p);
	return r;
}
class MappedFile
{
public:

};
class CachedImageStorage : public IImageStore
{
private:
	wxSize m_imageSize;
	wxString m_fileName;
	std::vector<uint32_t> m_durations;
	std::vector<std::array<size_t, 2>> m_pages;
	uint8_t* m_mappedFile;
#ifdef __WXMSW__
	HANDLE m_hFile;
	HANDLE m_hMap;
#else
	wxFile m_file;
	size_t m_fileSize;
#endif
public:
	CachedImageStorage(
		wxString fileName,
		const wxSize& imageSize,
		const std::vector<size_t>& bytesCounts,
		const std::vector<uint32_t>& durations);
	~CachedImageStorage();
	virtual IImageStoreBuilder* CreateBuilder(const wxSize& imageSize) override;
	virtual  std::pair<wxImage, uint32_t> Get(size_t index);
	virtual size_t GetCount() const;
	virtual wxSize GetImageSize() const;
	virtual void Clear() override{  }
	virtual bool IsSupportedEdit()override { return true; }
	virtual IImageStore* RemoveImages(size_t from, size_t to);
};
CISSaveThread::CISSaveThread(
	wxFileOutputStream& fOStream,
	std::vector<size_t>& bytesCounts,
	wxMessageQueue<std::pair<wxImage*, uint32_t>>& mqueue):
	m_fOStream(fOStream),
	m_bytesCounts(bytesCounts),
	m_mqueue(mqueue)
{
}

void* CISSaveThread::Entry()
{
	int i = 0;
	wxFileOffset preOffset = 0;
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
		wxZlibOutputStream stream(m_fOStream, 1);
		
		auto s = data.first->GetSize();
		auto bits = data.first->GetData();
		stream.WriteAll(bits, s.x * s.y * 3l);
		stream.Close();
		auto ts = m_fOStream.TellO();
		m_bytesCounts.push_back(ts - preOffset);
		preOffset = ts;
		delete data.first;
	}
	
	m_fOStream.Close();
	return nullptr;
}

CachedImageStorageBuilder::CachedImageStorageBuilder(const wxSize& imageSize):
	m_fileName(wxFileName::CreateTempFileName(wxT("webpcam"))),
	m_imageSize(imageSize),
	m_fileOStream(m_fileName),
	m_mqueue(),
	m_backgroundThread(m_fileOStream, m_bytesCounts, m_mqueue)
{
	m_backgroundThread.CreateThread();
	m_backgroundThread.GetThread()->Run();
}

CachedImageStorageBuilder::~CachedImageStorageBuilder()
{
	auto thread = m_backgroundThread.GetThread();
	if (thread != nullptr && thread->IsRunning())
	{
		m_mqueue.Post(std::pair<wxImage*, uint32_t>(nullptr, 0));
		thread->Wait();
	}
	m_fileOStream.GetFile()->Detach();
	m_fileOStream.Close();
	m_fileOStream.UnRef();
}

void CachedImageStorageBuilder::PushBack(const wxImage& image, uint32_t duration)
{
	m_durations.push_back(duration);
	m_mqueue.Post(std::pair<wxImage*, uint32_t>(new wxImage(image), duration));
}

size_t CachedImageStorageBuilder::GetSize() const
{
	return m_durations.size();
}

IImageStore* CachedImageStorageBuilder::BuildStore()
{
	auto thread = m_backgroundThread.GetThread();
	if (thread != nullptr)
	{
		m_mqueue.Post(std::pair<wxImage*, uint32_t>(nullptr, 0));
		thread->Wait();
	}
	m_fileOStream.GetFile()->Detach();
	m_fileOStream.Close();
	m_fileOStream.UnRef();
	return new CachedImageStorage(m_fileName, m_imageSize, m_bytesCounts, m_durations);
}

CachedImageStorage::CachedImageStorage(
	wxString fileName, const wxSize& imageSize, 
	const std::vector<size_t>& bytesCounts, const std::vector<uint32_t>& durations):
	m_fileName(fileName),m_imageSize(imageSize), m_durations(durations) , m_mappedFile(nullptr)
{
	size_t offset = 0;
	for (auto bytesCount: bytesCounts)
	{
		m_pages.push_back({ offset, bytesCount });
		offset += bytesCount;
	}
	//TODO:이미지 매핑
#ifdef __WXMSW__
	m_hFile = CreateFileW(m_fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD high = 0;
	DWORD low = GetFileSize(m_hFile, &high);
	if (high == 0 && low == 0)
		high = 0x1;
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		wchar_t* text = nullptr;
		auto err = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&text, 0, nullptr);
		wxMessageBox(text);
	}
	m_hMap = CreateFileMappingW(m_hFile, NULL, PAGE_READWRITE, high, low, NULL);
	if (m_hMap == NULL)
	{
		wchar_t* text = nullptr;
		auto err = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&text, 0, nullptr);
		wxMessageBox(text);
		if (err == 1006)
		{

		}
		else
		{
			
		}

	}
	m_mappedFile = (uint8_t*)MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS,0,0,0);
	if (!m_mappedFile)
	{
		wchar_t* text = nullptr;
		auto err = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&text, 0, nullptr);
		wxMessageBox(text);

	}
	
#else
	wxStructStat strucStat;
	wxStat(m_fileName, &strucStat);
	m_fileSize = strucStat.st_size;
	mmap(m_file.fd(), 
#endif
}

CachedImageStorage::~CachedImageStorage()
{
#ifdef __WXMSW__
	UnmapViewOfFile(m_mappedFile);
	CloseHandle(m_hMap);
	CloseHandle(m_hFile);
#endif
	wxRemoveFile(m_fileName);
}

IImageStoreBuilder* CachedImageStorage::CreateBuilder(const wxSize& imageSize)
{
	return new CachedImageStorageBuilder(imageSize);
}

std::pair<wxImage, uint32_t> CachedImageStorage::Get(size_t index)
{
	auto s = m_pages[index];
	wxMemoryInputStream memIStream(m_mappedFile + s[0], s[1]);
	wxZlibInputStream zIStream(memIStream);

	uint8_t* mem = (uint8_t*)malloc(3 * m_imageSize.x * m_imageSize.y);

	wxMemoryOutputStream memOStream(mem, 3 * m_imageSize.x * m_imageSize.y);
	memOStream.Write(zIStream);
	wxImage image(m_imageSize, mem);

	return std::pair<wxImage, uint32_t>(image,m_durations[index]);
}

size_t CachedImageStorage::GetCount() const
{
	return m_pages.size();
}

wxSize CachedImageStorage::GetImageSize() const
{
	return m_imageSize;
}

IImageStore* CachedImageStorage::RemoveImages(size_t from, size_t to)
{
	wxString fileName = wxFileName::CreateTempFileName(wxT("webpcam"));
	auto storage = new CachedImageStorage(fileName, m_imageSize, std::vector<size_t>(), std::vector<uint32_t>());
	size_t offset = 0;
	for (size_t i = from; i < to; i++)
	{
		auto& page = m_pages[i];
		storage->m_durations.push_back(m_durations[0]);
		storage->m_pages.push_back({ offset, page[1] });
		memcpy(storage->m_mappedFile + offset, m_mappedFile + page[0], page[1]);
		offset += page[1];
	}
	size_t source = m_pages[from][0];
	auto lastPage = *m_pages.rbegin();
	size_t dec = m_pages[to][0] - m_pages[from][0];
	
	for (size_t i = to; i < m_pages.size(); i++)
	{

		auto& page = m_pages[i];
		size_t oldOffset = page[0];
		page[0] = page[0] - dec;
		memcpy(m_mappedFile + page[0], m_mappedFile + oldOffset, page[1]);
	}
	m_pages.erase(m_pages.begin() + from , m_pages.begin() + to);

	return storage;
}
