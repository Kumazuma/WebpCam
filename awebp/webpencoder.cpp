#include "wx/wxprec.h"
#include "webpencoder.h"
#include "webp/encode.h"
#include <wx/wfstream.h>
#include <wx/time.h>
#include "event.h"

#ifdef _DEBUG
	#pragma comment(lib,"./debug/webp.lib")
	#pragma comment(lib,"./debug/webpdemux.lib")
	#pragma comment(lib,"./debug/webpmux.lib")
#else
	#pragma comment(lib,"./release/webp.lib")
	#pragma comment(lib,"./release/webpdemux.lib")
	#pragma comment(lib,"./release/webpmux.lib")
#endif

const int etypes[] = {
	VP8_ENC_OK,
	VP8_ENC_ERROR_OUT_OF_MEMORY,            // memory error allocating objects
	VP8_ENC_ERROR_BITSTREAM_OUT_OF_MEMORY,  // memory error while flushing bits
	VP8_ENC_ERROR_NULL_PARAMETER,           // a pointer parameter is NULL
	VP8_ENC_ERROR_INVALID_CONFIGURATION,    // configuration is invalid
	VP8_ENC_ERROR_BAD_DIMENSION,            // picture has invalid width/height
	VP8_ENC_ERROR_PARTITION0_OVERFLOW,      // partition is bigger than 512k
	VP8_ENC_ERROR_PARTITION_OVERFLOW,       // partition is bigger than 16M
	VP8_ENC_ERROR_BAD_WRITE,                // error while flushing bytes
	VP8_ENC_ERROR_FILE_TOO_BIG,             // file is bigger than 4G
	VP8_ENC_ERROR_USER_ABORT,               // abort request by user
	VP8_ENC_ERROR_LAST
};
const char* const  typestr[] = {
	"VP8_ENC_OK",
	"VP8_ENC_ERROR_OUT_OF_MEMORY",            // memory error allocating objects
	"VP8_ENC_ERROR_BITSTREAM_OUT_OF_MEMORY",  // memory error while flushing bits
	"VP8_ENC_ERROR_NULL_PARAMETER",           // a pointer parameter is NULL
	"VP8_ENC_ERROR_INVALID_CONFIGURATION",    // configuration is invalid
	"VP8_ENC_ERROR_BAD_DIMENSION",            // picture has invalid width/height
	"VP8_ENC_ERROR_PARTITION0_OVERFLOW",      // partition is bigger than 512k
	"VP8_ENC_ERROR_PARTITION_OVERFLOW",       // partition is bigger than 16M
	"VP8_ENC_ERROR_BAD_WRITE",                // error while flushing bytes
	"VP8_ENC_ERROR_FILE_TOO_BIG",             // file is bigger than 4G
	"VP8_ENC_ERROR_USER_ABORT",               // abort request by user
	"VP8_ENC_ERROR_LAST"
};

WebpEncoder::~WebpEncoder()
{

}

void WebpEncoder::Encode(wxEvtHandler* handler, const wxString filePath, IImageStore& imageStore)
{
	WebPConfigInit(&m_config);
	WebPAnimEncoderOptionsInit(&m_encoderOption);
	auto image = imageStore.Get(0).first;
	uint32_t width = image.GetWidth();
	uint32_t height = image.GetHeight();
	uint32_t timestamp = 0;
	//WebP Config세팅
	m_config.quality = 80.f;
	m_config.thread_level = 1;//멀티 스레드 사용
	m_config.lossless = 0;//손실포맷
	m_config.low_memory = 0;
	m_config.filter_strength = 0;//필터 잘 모르겠습 
	m_config.method = 6;//압축률 최대
	//키프레임 최소와 최대;
	m_encoderOption.kmin = 1;
	m_encoderOption.kmax = 40;
	//인코더를 초기화
	m_encoder = WebPAnimEncoderNew(width, height,&m_encoderOption);
	//프레임을 초기화한다
	WebPPictureInit(&m_frame);
	m_frame.width = width;
	m_frame.height = height;
	m_frame.use_argb = 1;
	WebPPictureAlloc(&m_frame);
	for(int i = 0 ; i < imageStore.GetCount(); i++)
	{
		if (m_requestedStop)
		{
			WebPAnimEncoderDelete(m_encoder);
			WebPPictureFree(&m_frame);
			auto* event = new wxCommandEvent(EVT_FAILED_ENCODE);
			handler->QueueEvent(event);
			return;
		}
		auto data = imageStore.Get(i);
		auto rgbData = data.first.GetData();
		WebPPictureImportRGB(&m_frame, rgbData, 3 * m_frame.width);
		if(!WebPAnimEncoderAdd(m_encoder, &m_frame, timestamp, &m_config))
		{
			for (int j = 0; j < sizeof(etypes) / sizeof(etypes[0]); j++)
			{
				if (m_frame.error_code == etypes[j])
				{

					wxLogGeneric(wxLogLevelValues::wxLOG_Message, typestr[j]);
					break;
				}
			}
			auto* event = new wxCommandEvent(EVT_FAILED_ENCODE);
			handler->QueueEvent(event);
			return;
		}
		timestamp += data.second;

		auto* event = new wxCommandEvent(EVT_ADDED_A_FRAME);
		event->SetInt(i);
		handler->QueueEvent(event);
	}

	WebPData webpData;
	WebPDataInit(&webpData);
	//끝나면 NULL프레임을 집어넣는다.
	WebPAnimEncoderAdd(m_encoder, nullptr, timestamp, nullptr);
	WebPAnimEncoderAssemble(m_encoder, &webpData);

	auto webpMux = WebPMuxCreate(&webpData, 1);
	WebPDataClear(&webpData);
	WebPMuxAssemble(webpMux, &webpData);

	wxFileOutputStream* wfos = new wxFileOutputStream(filePath);
	wfos->WriteAll(webpData.bytes, webpData.size);
	wfos->Close();
	delete wfos;
	WebPPictureFree(&m_frame);
	WebPDataClear(&webpData);
	
	WebPMuxDelete(webpMux);
	WebPAnimEncoderDelete(m_encoder);
	
	auto* event = new wxCommandEvent(EVT_FINISH_ENCODE);
	handler->QueueEvent(event);
}

void WebpEncoder::StopEncode()
{
	m_requestedStop = true;
}

wxString WebpEncoder::GetFileFilter()
{
	//TODO:
	return wxT("webp 이미지 파일 (*.webp)|*.webp");
}

wxString WebpEncoder::GetFileExtension()
{
	return wxT("webp");
}

