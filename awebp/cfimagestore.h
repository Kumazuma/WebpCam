#pragma once
#include "interface.h"
#include <wx/zstream.h>
#include <wx/wfstream.h>
#include <wx/msgqueue.h>
#include <vector>
#include <array>
#include <wx/filename.h>
#include <wx/mstream.h>
class CISSaveThread : public wxThreadHelper
{
	wxMutex& m_mutex;
	HFILE& m_hFile;
	std::vector<size_t>& m_bytesCounts;
	wxMessageQueue<std::pair<wxImage*, uint32_t>>& m_mqueue;
public:
	CISSaveThread(
		wxMutex& m_mutex,
		HFILE& hFile,
		std::vector<size_t>& offsets,
		wxMessageQueue<std::pair<wxImage*, uint32_t>>& mqueue);
protected:
	void* Entry() override;
};
class CachedImageStorageBuilder : public IImageStoreBuilder
{
public:
public:
	CachedImageStorageBuilder(const wxSize& imageSize);
	~CachedImageStorageBuilder();
	virtual void PushBack(const wxImage& image, uint32_t duration) override;
	virtual size_t GetSize() const override;
protected:
	virtual IImageStore* BuildStore() override;

private:
	wxMutex m_mutex;
	wxSize m_imageSize;
	HANDLE m_hFile;
	wxString m_fileName;
	wxMessageQueue<std::pair<wxImage*, uint32_t>> m_mqueue;
	std::vector<uint32_t> m_durations;
	std::vector<size_t> m_bytesCounts;
	CISSaveThread m_backgroundThread;
	
	
};