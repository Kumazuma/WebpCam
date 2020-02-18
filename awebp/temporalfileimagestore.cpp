#include "wx/wxprec.h"
#include "temporalfileimagestore.h"
#include <wx/mstream.h>
#include "webp/encode.h"
#include "webp/decode.h"
#include "webp/mux.h"
#include "webp/demux.h"
class LibWebp
{
public:
	LibWebp()
	{

	}
	bool Encode(const wxImage& image, wxOutputStream* stream)
	{
		WebPPicture picture;
		WebPConfig config;
		if (!WebPConfigPreset(&config, WEBP_PRESET_PHOTO, 100)) return false;
		config.lossless = true;
		config.thread_level = 1;
		config.method = 2;
		int config_error = WebPValidateConfig(&config);

		WebPPictureInit(&picture);
		picture.width = image.GetWidth();
		picture.height = image.GetHeight();
		picture.use_argb = 1;
		WebPPictureAlloc(&picture);
		auto rgbData = image.GetData();
		WebPPictureImportRGB(&picture, rgbData, 3 * picture.width);
		picture.writer = LibWebp::WebPWriterFunction;
		picture.custom_ptr = stream;
		int ok = WebPEncode(&config, &picture);
		
		WebPPictureFree(&picture);
		return !ok;
	}
	wxImage Decode(const uint8_t* data,size_t len)
	{
		int height;
		int width;
		if (!WebPGetInfo(data, len, &width, &height))
		{
			return wxImage();
		}
		auto decodedBytes = WebPDecodeRGB(data, len,&width, &height);
		auto imageBytes = (uint8_t*)malloc( width * height * 3l);
		memcpy_s(imageBytes, width * height * 3l, decodedBytes, width * height * 3l);
		wxImage image(width, height, imageBytes);
		WebPFree(decodedBytes);
		return image;
		//정보를 얻어야 한다.
		WebPData webpData;
		WebPDataInit(&webpData);
		webpData.size = len;
		webpData.bytes = data;
		WebPDemuxer* demux = WebPDemux(&webpData);
		//uint32_t width = WebPDemuxGetI(demux, WEBP_FF_CANVAS_WIDTH);
		//uint32_t height = WebPDemuxGetI(demux, WEBP_FF_CANVAS_HEIGHT);

		WebPDecoderConfig config;
		WebPInitDecoderConfig(&config);
		config.options.use_threads = true;
		config.output.colorspace = MODE_RGB;
		config.output.height = height;
		config.output.height = width;
		config.input.width = width;
		config.input.height= width;
		config.input.format = 2;
		//
	}
private:
	static int WebPWriterFunction(const uint8_t* data, size_t data_size,
		const WebPPicture* picture)
	{
		wxOutputStream* stream = (wxOutputStream*)picture->custom_ptr;
		int writeCount = 0;
		stream->WriteAll(data, data_size);
		return true;
	}
};
FileImageStore::FileImageStore(wxString fileName,
	decltype(m_imageWidth) width,
	decltype(m_imageHeight) height, const std::vector<uint32_t> & durations) :
	m_imageHeight(height),
	m_imageWidth(width),
	m_fileName(fileName),
	m_fileIStream(fileName),
	m_zipInputStream(m_fileIStream)
{
	auto& tar = m_zipInputStream;
	auto it = durations.begin();
	if (tar.Eof() || tar.CanRead() == false || durations.size() ==0)
	{
		return;
	}
	
	for (auto entry = tar.GetNextEntry(); entry != nullptr; entry = tar.GetNextEntry())
	{
		m_store.push_back(std::pair<wxZipEntry*, uint32_t>(entry, *it));
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

	m_zipInputStream.UnRef();
	m_fileIStream.GetFile()->Detach();
	m_fileIStream.UnRef();
	wxFile file(m_fileName, wxFile::write);
	file.Flush();
}

FileSaveThread::FileSaveThread(
	wxZipOutputStream& store,
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
		
		m_store.PutNextEntry(tempName);
		auto s = data.first->GetSize();
		auto bits = data.first->GetData();
		m_store.WriteAll(bits, s.x * s.y * 3l);
		m_store.CloseEntry();
		delete data.first;
	}
	return nullptr;
}

FileImageStoreBuilder::FileImageStoreBuilder(const wxSize& imageSize):
	m_fileName(wxFileName::CreateTempFileName(wxT("awebpcap"))),
	m_imageSize(imageSize),
	m_fileOStream(m_fileName),
	m_zipOutputStream(m_fileOStream),
	m_mqueue(),
	m_backgroundThread(m_zipOutputStream, m_mqueue)
{
	m_zipOutputStream.SetLevel(2);
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
	m_zipOutputStream.Close();
	m_fileOStream.Close();
	m_zipOutputStream.UnRef();
	m_fileOStream.UnRef();
	return new FileImageStore(m_fileName,m_imageSize.GetWidth(),m_imageSize.GetHeight(), m_durations);
}
