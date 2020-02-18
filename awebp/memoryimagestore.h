#pragma once
#include "interface.h"
#include <vector>
#include <wx/mstream.h>
#include <wx/zipstrm.h>
#include <wx/msgqueue.h>
class MemorySaveThread : public wxThreadHelper
{
	wxZipOutputStream& m_store;
	wxMessageQueue<std::pair<wxImage*, uint32_t>>& m_mqueue;
public:
	MemorySaveThread(
		wxZipOutputStream& store,
		wxMessageQueue<std::pair<wxImage*, uint32_t>>& mqueue);
protected:
	void* Entry() override;
};
class MemoryImageStoreBuilder : public IImageStoreBuilder
{
public:
	MemoryImageStoreBuilder(const wxSize& imageSize);
	~MemoryImageStoreBuilder();
	virtual void PushBack(const wxImage& image, uint32_t duration) override;
	virtual size_t GetSize() const override;
protected:
	virtual IImageStore* BuildStore() override;

private:
	wxSize m_imageSize;
	std::vector<uint32_t> m_durations;
	wxMessageQueue<std::pair<wxImage*, uint32_t>> m_mqueue;
	MemorySaveThread m_backgroundThread;
	wxMemoryOutputStream* m_memOStream;
	wxMemoryInputStream m_memIStream;
	wxZipOutputStream m_zipOutputStream;
};
class MemoryImageStore : public IImageStore
{
private:
	std::vector<std::pair<wxZipEntry*, uint32_t>> m_store;
	wxZipInputStream m_zipInputStream;
	
	wxMemoryInputStream m_memoryIStream;
	uint32_t m_imageHeight;
	uint32_t m_imageWidth;
public:
	MemoryImageStore(
		wxMemoryOutputStream* memoryOStream,
		decltype(m_imageWidth)width,
		decltype(m_imageHeight) height,
		const std::vector<uint32_t>& durations);
	virtual ~MemoryImageStore();

	virtual IImageStoreBuilder* CreateBuilder(const wxSize& imageSize) override;
	virtual std::pair<wxImage, uint32_t> Get(size_t index);
	virtual size_t GetCount() const override
	{
		return m_store.size();
	}
	virtual wxSize GetImageSize() const override;
	virtual void Clear() override;
};