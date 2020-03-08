#include "wx/wxprec.h"
#include "cfimagestore.h"
#include <wx/log.h>
#include <optional>
#include <cmath>
#include <random>
#include<wx/stdpaths.h>
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
wxString GetTempFile(const wxString& prefix, wxFile* file = nullptr)
{
	std::random_device rn;
	std::mt19937_64 rnd(rn());

	//< 2단계. 분포 설정 ( 정수 )
	std::uniform_int_distribution<long> range(0, LONG_MAX);

	auto& path = wxStandardPaths::Get();
	wxFileName name;
	do
	{
		name.SetName(wxString::Format(wxT("%s%08X.%08X"), prefix, range(rn), (long)time(nullptr)));
		name.SetExt(wxT("tmp"));
		name.SetPath(path.GetTempDir());
	} while (name.Exists());
	
	if (file != nullptr)
	{
		if (file->Open(name.GetFullPath(), wxFile::write_excl)== false)
		{
			file->Seek(0);
			wxLogGeneric(wxLOG_Error, wxString() << file->GetLastError());
			return wxT("");
		}
	}
	return name.GetFullPath();
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
	virtual std::pair<wxInputStream*, uint32_t> GetRawData(size_t index);
	virtual size_t GetCount() const;
	virtual wxSize GetImageSize() const;
	virtual void Clear() override{  }
	virtual void LoadToMemory() override;
	virtual void UnloadFromMemory() override;
	virtual std::optional<uint32_t> GetFrameDuration(size_t index) override;
	virtual void SetFrameDuration(size_t index, uint32_t duration) override;
	virtual uint32_t GetTotalDuration()const override;
	virtual size_t GetSize();
};
CISSaveThread::CISSaveThread(
	wxMutex& mutex,
	HFILE& hFile,
	std::vector<size_t>& bytesCounts,
	wxMessageQueue<std::pair<wxImage*, uint32_t>>& mqueue):
	m_mutex(mutex),
	m_hFile(hFile),
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
		DWORD size = s.x * s.y * 3;
		DWORD totalWriteCount = 0;
		
		do {
			DWORD writeCount = 0;
			WriteFile((HANDLE)m_hFile,
				bits + totalWriteCount,
				size - totalWriteCount,
				&writeCount,
				nullptr);
			totalWriteCount += writeCount;

		} while (size != totalWriteCount);
		
		m_bytesCounts.push_back(size);
		delete data.first;
	}
	m_mutex.Unlock();
	return nullptr;
}

CachedImageStorageBuilder::CachedImageStorageBuilder(const wxSize& imageSize):
	m_mutex(),
	m_fileName(GetTempFile(wxT("webpcam"))),
	m_imageSize(imageSize),
	m_mqueue(),
	m_backgroundThread(m_mutex, (HFILE&)m_hFile, m_bytesCounts, m_mqueue)
{
	m_hFile = CreateFileW(m_fileName.wc_str(), 
		GENERIC_READ | GENERIC_WRITE,0,
		nullptr, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
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
	if (m_hFile)
	{
		CloseHandle(m_hFile);
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
	if (m_hFile)
		CloseHandle(m_hFile);
	m_hFile = nullptr;
	m_mutex.Unlock();
	wxString temp = m_fileName;
	m_fileName.Clear();
	return new CachedImageStorage(temp, m_imageSize, m_bytesCounts, m_durations);
}

CachedImageStorage::CachedImageStorage(
	wxString fileName, const wxSize& imageSize, 
	const std::vector<size_t>& bytesCounts, const std::vector<uint32_t>& durations):
	m_fileName(fileName),
	m_imageSize(imageSize),
	m_durations(durations),
	m_hFile(nullptr),
	m_hMap(nullptr),
	m_mappedFile(nullptr)
{
	size_t offset = 0;
	for (auto bytesCount: bytesCounts)
	{
		m_pages.push_back({ offset, bytesCount });
		offset += bytesCount;
	}
	//TODO:이미지 매핑
#ifdef __WXMSW__
	try
	{
		m_hFile = CreateFileW(m_fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE)
			throw GetLastError();
		DWORD high = 0;
		DWORD low = GetFileSize(m_hFile, &high);
		m_hMap = CreateFileMappingW(m_hFile, NULL, PAGE_READWRITE, high, low, NULL);
		if (m_hMap == NULL)
			throw GetLastError();
		m_mappedFile = (uint8_t*)MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (!m_mappedFile)
			throw GetLastError();
	}
	catch (DWORD code)
	{
		wchar_t* text = nullptr;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&text, 0, nullptr);
		wxLogGeneric(wxLOG_Error, text);
		if(m_hMap)
			CloseHandle(m_hMap);
		if (m_hFile)
			CloseHandle(m_hFile);
		m_mappedFile = nullptr;
		m_hFile = nullptr;
		m_hMap = nullptr;
		m_durations.clear();
		m_pages.clear();
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
	if(m_mappedFile)
		UnmapViewOfFile(m_mappedFile);
	if(m_hMap)
		CloseHandle(m_hMap);
	if(m_hFile)
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
	bool isUnloadedData = m_mappedFile == nullptr;
	
	if (isUnloadedData)
		LoadToMemory();
	unsigned char* ptr = m_mappedFile + s[0];
	wxImage image(m_imageSize, ptr, true);
	
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

void CachedImageStorage::LoadToMemory()
{
	if (m_mappedFile)
		return;
	if (!m_hFile)
		return;
	try
	{
		DWORD high = 0;
		DWORD low = GetFileSize(m_hFile, &high);
		m_hMap = CreateFileMappingW(m_hFile, NULL, PAGE_READWRITE, high, low, NULL);
		if (m_hMap == NULL)
			throw GetLastError();
		m_mappedFile = (uint8_t*)MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (!m_mappedFile)
			throw GetLastError();
	}
	catch (DWORD code)
	{
		wchar_t* text = nullptr;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&text, 0, nullptr);
		wxLogGeneric(wxLOG_Error, text);
		if (m_hMap)
			CloseHandle(m_hMap);
		if (m_hFile)
			CloseHandle(m_hFile);
		m_mappedFile = nullptr;
		m_hMap = nullptr;
	}
}

void CachedImageStorage::UnloadFromMemory()
{
	if (m_mappedFile)
		UnmapViewOfFile(m_mappedFile);
	if (m_hMap)
		CloseHandle(m_hMap);
	m_mappedFile = nullptr;
	m_hMap = nullptr;
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

size_t CachedImageStorage::GetSize()
{
	auto it = m_pages.rbegin();
	if (it == m_pages.rend())
		return 0;
	return it->at(0) + it->at(1);
}
