#include "wx/wxprec.h"
#include "webpencoder.h"
#include "webp/encode.h"
#include <wx/wfstream.h>
#include <wx/time.h>

#pragma comment(lib,"libwebp.lib")
#pragma comment(lib,"libwebpdemux.lib")
#pragma comment(lib,"libwebpmux.lib")

class WebPProcessingThread : public wxThread
{
public:
	WebPAnimEncoder* m_encoder;
	WebPAnimEncoderOptions m_encoderOption;
	WebPConfig m_config;
	WebPPicture m_frame;
	uint32_t m_timestamp;
	wxMessageQueue<std::pair<wxImage,uint32_t>>* m_queue;
	wxEvtHandler* m_evtHandler;
protected:
	void* Entry() override
	{
		std::pair<wxImage, uint32_t> data;
		while (true)
		{
			auto err = m_queue->ReceiveTimeout(1000, data);
			if (err == wxMSGQUEUE_TIMEOUT)
			{
				break;
			}
			auto rgbData = data.first.GetData();
			WebPPictureImportRGB(&m_frame, rgbData, 3 * m_frame.width);
			WebPAnimEncoderAdd(m_encoder, &m_frame, m_timestamp, &m_config);
			m_timestamp += data.second;
		}

		WebPData webpData;
		WebPDataInit(&webpData);
		//끝나면 NULL프레임을 집어넣는다.
		WebPAnimEncoderAdd(m_encoder, nullptr, m_timestamp, nullptr);
		WebPAnimEncoderAssemble(m_encoder, &webpData);

		auto webpMux = WebPMuxCreate(&webpData, 1);
		WebPDataClear(&webpData);
		WebPMuxAssemble(webpMux, &webpData);

		wxFileOutputStream* wfos = new wxFileOutputStream(wxT("temp.webp"));
		wfos->WriteAll(webpData.bytes, webpData.size);
		wfos->Close();
		delete wfos;
		WebPPictureFree(&m_frame);
		WebPDataClear(&webpData);
		WebPMuxDelete(webpMux);
		WebPAnimEncoderDelete(m_encoder);

		wxCommandEvent* event = new wxCommandEvent(EVT_CompleteRecord);
		m_evtHandler->QueueEvent(event);
		return nullptr;
	}
};


bool WebpEncoder::BeginEncode(wxEvtHandler* handler, uint32_t width, uint32_t height)
{
	m_timestamp = 0;
	WebPConfigInit(&m_config);
	WebPAnimEncoderOptionsInit(&m_encoderOption);
	
	
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

	auto thread = new WebPProcessingThread();
	thread->m_timestamp = 0;
	thread->m_config = m_config;
	thread->m_encoder = m_encoder;
	thread->m_encoderOption = m_encoderOption;
	thread->m_frame = m_frame;
	thread->m_queue = &m_queue;
	thread->m_evtHandler = handler;
	thread->Run();
	return true;
}

bool WebpEncoder::AddFrame(const wxImage& frame, uint32_t duration)
{
	m_queue.Post(std::pair<wxImage, uint32_t>{frame, duration});
	return true;
}

bool WebpEncoder::EndEncode()
{
	return true;
}

