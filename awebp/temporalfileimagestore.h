#pragma once
#include "interface.h"
#include <vector>
#include <wx/file.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/tarstrm.h>
#include <wx/msgqueue.h>
class FileSaveThread : public wxThreadHelper
{
	wxTarOutputStream& m_store;
	wxMessageQueue<std::pair<wxImage*, uint32_t>>& m_mqueue;
public:
	FileSaveThread(
		wxTarOutputStream& store,
		wxMessageQueue<std::pair<wxImage*, uint32_t>>& mqueue);
protected:
	void* Entry() override;
};
class FileImageStoreBuilder : public IImageStoreBuilder
{
public:
	FileImageStoreBuilder();
	~FileImageStoreBuilder();
	virtual void PushBack(const wxImage& image, uint32_t duration) override;
	virtual size_t GetSize() const override;
	virtual IImageStore* BuildStore() override;
private:
	uint32_t m_imageHeight;
	uint32_t m_imageWidth;
	std::vector<uint32_t> m_durations;
	wxMessageQueue<std::pair<wxImage*, uint32_t>> m_mqueue;
	FileSaveThread m_backgroundThread;
	wxString m_fileName;
	wxFileOutputStream m_fileOStream;
	wxTarOutputStream m_taroutputStream;
};
class FileImageStore : public IImageStore
{
private:
	std::vector<std::pair<wxTarEntry*, uint32_t>> m_store;
	wxTarInputStream m_tarInputStream;
	wxFileInputStream m_fileIStream;
	uint32_t m_imageHeight;
	uint32_t m_imageWidth;
public:
	FileImageStore(wxString fileName,
		decltype(m_imageHeight) height,
		decltype(m_imageWidth) width, 
		std::vector<uint32_t> && durations);

	virtual ~FileImageStore()
	{
		Clear();
	}
	virtual std::pair<wxImage, uint32_t> Get(size_t index) ;
	virtual size_t GetSize() const override
	{
		return m_store.size();
	}
	virtual void Clear() override;
}; 