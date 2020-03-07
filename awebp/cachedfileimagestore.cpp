#include "wx/wxprec.h"
#include "cfimagestore.h"
#include <wx/log.h>
#include <optional>
#include <cmath>
#ifndef __WXMSW__
#include <unistd.h>
#endif
wxAtomicInt g_p = 0;
template<typename T>
constexpr uint8_t bitcountof() {
	return sizeof(T) * 8;
}
template<typename T>
constexpr uint8_t bitcountof(T m) {
	return bitcountof<T>();
}
template<typename T>
constexpr T hiword(T m) {
	return m >> (bitcountof(m) / 2);
}
template<typename T>
constexpr T loword(T m) {
	return m ^ (hiword(m) << (bitcountof(m) / 2));
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
	CachedImageStorage(size_t fileSize, const wxSize& imgSize);
	~CachedImageStorage();
	virtual IImageStoreBuilder* CreateBuilder(const wxSize& imageSize) override;
	virtual  std::pair<wxImage, uint32_t> Get(size_t index);
	virtual std::pair<wxInputStream*, uint32_t> GetRawData(size_t index);
	virtual size_t GetCount() const;
	virtual wxSize GetImageSize() const;
	virtual void Clear() override{  }
	virtual bool IsSupportedEdit()override { return false; }
	virtual IImageStore* RemoveImages(size_t from, size_t to);
	virtual bool InsertImages(IImageStore*& image, size_t to) override;
	virtual std::optional<uint32_t> GetFrameDuration(size_t index) override;
	virtual void SetFrameDuration(size_t index, uint32_t duration) override;
	virtual uint32_t GetTotalDuration()const override;
};
CISSaveThread::CISSaveThread(
	wxMutex& mutex,
	wxFileOutputStream& fOStream,
	std::vector<size_t>& bytesCounts,
	wxMessageQueue<std::pair<wxImage*, uint32_t>>& mqueue):
	m_mutex(mutex),
	m_fOStream(fOStream),
	m_bytesCounts(bytesCounts),
	m_mqueue(mqueue)
{
}

void* CISSaveThread::Entry()
{
	m_mutex.Lock();
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

		auto s = data.first->GetSize();
		auto bits = data.first->GetData();
		m_fOStream.WriteAll(bits, s.x * s.y * 3l);

		auto ts = m_fOStream.TellO();
		m_bytesCounts.push_back(ts - preOffset);
		preOffset = ts;
		delete data.first;
	}
	m_mutex.Unlock();
	return nullptr;
}

CachedImageStorageBuilder::CachedImageStorageBuilder(const wxSize& imageSize):
	m_mutex(),
	m_fileName(wxFileName::CreateTempFileName(wxT("webpcam"),&m_file)),
	m_imageSize(imageSize),
	m_fileOStream(m_file),
	m_mqueue(),
	m_backgroundThread(m_mutex, m_fileOStream, m_bytesCounts, m_mqueue)
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
	m_mutex.Lock();
	m_fileOStream.Close();
	m_fileOStream.UnRef();
	if (m_fileName.empty() == false)
	{
		m_file.Close();
		m_file.Detach();
		wxRemoveFile(m_fileName);
	}
	m_mutex.Unlock();
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
	m_mutex.Lock();
	m_fileOStream.Close();
	m_fileOStream.UnRef();
	if (m_fileName.empty() == false)
	{
		m_file.Close();
		m_file.Detach();
	}
	m_mutex.Unlock();
	wxString temp = m_fileName;
	m_fileName.Clear();
	return new CachedImageStorage(temp, m_imageSize, m_bytesCounts, m_durations);
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
	//if (high == 0 && low == 0)
		low |= 0x20000000;
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


CachedImageStorage::CachedImageStorage(size_t fileSize, const wxSize& imgSize):
	m_fileName(wxFileName::CreateTempFileName(wxT("webpcam"))), m_imageSize(imgSize), m_durations(), m_mappedFile(nullptr)
{
	//TODO:이미지 매핑
#ifdef __WXMSW__
	m_hFile = CreateFileW(m_fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	for (int i = 0; i < fileSize / sizeof(uint64_t); i++)
	{
		uint64_t b = 0;
		if (!WriteFile(m_hFile, &b, sizeof(uint64_t), nullptr, nullptr))
		{
			wchar_t* text = nullptr;
			auto err = GetLastError();
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&text, 0, nullptr);
			wxMessageBox(text);
		}
	}
	FlushFileBuffers(m_hFile);
	DWORD high = fileSize >> (bitcountof(fileSize)/2);
	DWORD low = fileSize;
	//if (high == 0 && low == 0)
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
	m_mappedFile = (uint8_t*)MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (!m_mappedFile)
	{
		wchar_t* text = nullptr;
		auto err = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&text, 0, nullptr);
		wxMessageBox(text);

	}
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
	wxImage image(m_imageSize, m_mappedFile + s[0], true);

	return std::pair<wxImage, uint32_t>(image,m_durations[index]);
}

std::pair<wxInputStream*, uint32_t> CachedImageStorage::GetRawData(size_t index)
{
	auto s = m_pages[index];
	wxMemoryInputStream* memIStream = new wxMemoryInputStream(m_mappedFile + s[0], s[1]);
	return std::pair<wxInputStream*, uint32_t>(memIStream, m_durations[index]);
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
	return nullptr;
	size_t fileSize = 0;
	{
		auto page = m_pages[to - 1];
		fileSize = page[0] + page[1];
		page = m_pages[from];
		fileSize -= page[0];
	}
	auto storage = new CachedImageStorage(fileSize, m_imageSize);
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
	size_t dec = m_pages[to -1][0] - m_pages[from][0];
	
	for (size_t i = to; i < m_pages.size(); i++)
	{

		auto& page = m_pages[i];
		size_t oldOffset = page[0];
		page[0] = page[0] - dec;
		memcpy(m_mappedFile + page[0], m_mappedFile + oldOffset, page[1]);
	}
	m_pages.erase(m_pages.begin() + from , m_pages.begin() + to);
	auto base = m_durations.begin();
	m_durations.erase(base + from, base + to);
	return storage;
}

bool CachedImageStorage::InsertImages(IImageStore*& image, size_t to)
{
	return false;
	CachedImageStorage* obj = dynamic_cast<CachedImageStorage*>(image);
	if (obj != nullptr)
	{
		auto lastPage = *obj->m_pages.rbegin();
		size_t objMemSize = lastPage[0] + lastPage[1];
		size_t endCount = (m_pages.size() - 1) - to;
		uint8_t* ptr = nullptr;
		size_t to_offset = 0;
		if(m_pages.size() != to)
			to_offset = m_pages[to][0];
		else if(m_pages.size() != 0)
			to_offset = m_pages[to- 1][0] + m_pages[to - 1][1];
		for (auto it = m_pages.rbegin(); it != m_pages.rend() - to; it++)
		{
			auto& page = *it;
			ptr = m_mappedFile + page[0];
			memcpy(ptr + objMemSize , ptr, page[1]);
			page[0] += objMemSize;
		}
		for (auto& it : obj->m_pages)
		{
			it[0] += to_offset;
		}
		memcpy(m_mappedFile + to_offset, obj->m_mappedFile, objMemSize);
		size_t offset = 0;
		size_t i = 0;
		m_pages.insert(m_pages.begin() + to, obj->m_pages.begin(), obj->m_pages.end());
		m_durations.insert(m_durations.begin() + to, obj->m_durations.begin(), obj->m_durations.end());
		return true;
	}
	else
	{

	}
	return false;
}

std::optional<uint32_t> CachedImageStorage::GetFrameDuration(size_t index)
{
	if(m_durations.size() <= index)
		return std::optional<uint32_t>();
	return m_durations[index];
}

void CachedImageStorage::SetFrameDuration(size_t index, uint32_t duration)
{
	m_durations[index] = duration;
}
#include<algorithm>
uint32_t CachedImageStorage::GetTotalDuration() const
{
	uint32_t s = 0;
	for (auto it : m_durations)
	{
		s += it;
	}
	return s;
}
