#pragma once
#include "interface.h"
#include <vector>
#include <wx/file.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include <wx/msgqueue.h>
class FileSaveThread : public wxThreadHelper
{
	wxZipOutputStream& m_store;
	wxMessageQueue<std::pair<wxImage*, uint32_t>>& m_mqueue;
public:
	FileSaveThread(
		wxZipOutputStream& store,
		wxMessageQueue<std::pair<wxImage*, uint32_t>>& mqueue);
protected:
	void* Entry() override;
};
class FileImageStoreBuilder : public IImageStoreBuilder
{
public:
	FileImageStoreBuilder(const wxSize& imageSize);
	~FileImageStoreBuilder();
	virtual void PushBack(const wxImage& image, uint32_t duration) override;
	virtual size_t GetSize() const override;
protected:
	virtual IImageStore* BuildStore() override;
	
private:
	wxSize m_imageSize;
	std::vector<uint32_t> m_durations;
	wxMessageQueue<std::pair<wxImage*, uint32_t>> m_mqueue;
	FileSaveThread m_backgroundThread;
	wxString m_fileName;
	wxFileOutputStream m_fileOStream;
	wxZipOutputStream m_zipOutputStream;
};
class FileImageStore : public IImageStore
{
private:
	std::vector<std::pair<wxZipEntry*, uint32_t>> m_store;
	wxZipInputStream m_zipInputStream;
	wxFileInputStream m_fileIStream;
	uint32_t m_imageHeight;
	uint32_t m_imageWidth;
	wxString m_fileName;
public:
	FileImageStore(wxString fileName,
		decltype(m_imageWidth)width ,
		decltype(m_imageHeight) height,
		const std::vector<uint32_t> & durations);
	virtual ~FileImageStore();
	
	virtual IImageStoreBuilder* CreateBuilder(const wxSize& imageSize) override;
	virtual std::pair<wxImage, uint32_t> Get(size_t index) ;
	virtual size_t GetCount() const override
	{
		return m_store.size();
	}
	virtual wxSize GetImageSize() const override;
	virtual void Clear() override;
}; 