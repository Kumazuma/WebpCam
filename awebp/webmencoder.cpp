#include "wx/wxprec.h"
#include "webmencoder.h"
extern "C" {
#include <libavutil/imgutils.h>
}
#include "event.h"

#include<wx/log.h>
#define BEGIN_ERROR_HANDLE(func, condi) {auto __ret = func; if(__ret condi){
#define END_ERROR_HANDLE() return;}}
#define IsNULL ==nullptr
#define IsNotNull !=nullptr
#define RETURN() __ret
#define SCALE_FLAGS SWS_BICUBIC
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
WebmEncoder::WebmEncoder():
	m_codecEncode(nullptr),
	m_context(nullptr),
	m_pictureEncoded(nullptr),
	m_pkt(nullptr),
	m_opt(nullptr)
{
	
	
}

WebmEncoder::~WebmEncoder()
{
	if (m_pictureEncoded)
	{
		av_free(m_pictureEncoded);
	}
	if (m_pkt)
	{
		av_packet_free(&m_pkt);
	}
	if (m_codecEncode)
	{
		av_free(m_codecEncode);
	}
	if (m_context)
	{
		avcodec_free_context(&m_context);
	}
}

void WebmEncoder::Encode(wxEvtHandler* handler, const wxString filePath, IImageStore& imageStore)
{
	avformat_alloc_output_context2(&m_formatContext, nullptr, "webm", filePath.ToUTF8().data());
	BEGIN_ERROR_HANDLE(m_formatContext, IsNULL);
	wxLogGeneric(wxLogLevelValues::wxLOG_Error, wxT("webm파일 포맷을 만들 수 없습니다."));
	END_ERROR_HANDLE();
	m_codecEncode = avcodec_find_encoder(AVCodecID::AV_CODEC_ID_VP9);
	m_context = avcodec_alloc_context3(m_codecEncode);
	
	m_formatContext->oformat->video_codec = AV_CODEC_ID_VP9;
	m_formatContext->oformat->audio_codec = AV_CODEC_ID_NONE;
	m_stream = avformat_new_stream(m_formatContext, nullptr);
	BEGIN_ERROR_HANDLE(m_stream, IsNULL);
	wxLogGeneric(wxLogLevelValues::wxLOG_Error, wxT("비디오 인코드 스트림을 만들 수 없습니다."));
	END_ERROR_HANDLE();
	m_stream->id = m_formatContext->nb_streams - 1;
	int ret = 0;
	auto imgSize = imageStore.GetImageSize();
	m_context->codec_id = AV_CODEC_ID_VP9;
	m_context->pix_fmt = AV_PIX_FMT_YUV420P;

	

	m_context->width = imgSize.GetWidth();
	m_context->height = imgSize.GetHeight();
	m_context->bit_rate = (1024 * 3000) * (m_quality / 100.f);
	m_context->time_base.den = 20;
	m_context->time_base.num = 1;
	
	

	m_context->time_base = { 1, 20 };;
	m_context->framerate.den = 1;
	m_context->framerate.num = 20;

	m_stream->time_base = { 1, 20 };
	m_stream->avg_frame_rate = { 20,1 };
	m_stream->r_frame_rate = m_stream->avg_frame_rate;
	m_stream->duration = imageStore.GetCount() * 50;
	

	m_context->qmin = 0;
	m_context->qmax = 60;
	m_context->max_b_frames = 1;
	m_context->gop_size = imageStore.GetCount();
	m_context->thread_count = wxThread::GetCPUCount();
	
	/* Some formats want stream headers to be separate. */
	if (m_formatContext->oformat->flags & AVFMT_GLOBALHEADER)
		m_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	avcodec_open2(m_context, m_codecEncode, nullptr);
	avcodec_parameters_from_context(m_stream->codecpar, m_context);
	m_formatContext->video_codec = m_codecEncode;
	m_formatContext->duration = imageStore.GetCount() * 50;
	BEGIN_ERROR_HANDLE(avio_open(&m_formatContext->pb, filePath.ToUTF8().data(), AVIO_FLAG_READ_WRITE),< 0);
	wxLogGeneric(wxLogLevelValues::wxLOG_Error, wxT("파일 열기에 실패했습니다."));
	END_ERROR_HANDLE();
	BEGIN_ERROR_HANDLE(avformat_write_header(m_formatContext, &m_opt), < 0);
	char bff[AV_ERROR_MAX_STRING_SIZE];
	wxString msg = av_make_error_string(bff, AV_ERROR_MAX_STRING_SIZE, RETURN());
	wxLogGeneric(wxLogLevelValues::wxLOG_Error, msg);
	END_ERROR_HANDLE();

	m_pictureEncoded = av_frame_alloc();
	m_pictureEncoded->format = m_context->pix_fmt;
	m_pictureEncoded->width = m_context->width;
	m_pictureEncoded->height = m_context->height;
	av_frame_get_buffer(m_pictureEncoded, 0);
	
	
//RGB24에서 YUV420으로 바꾸기 위한 준비
	AVFrame* rgbFrame = av_frame_alloc();
	rgbFrame->format = AV_PIX_FMT_RGB24;
	rgbFrame->width = m_context->width;
	rgbFrame->height = m_context->height;
	av_frame_get_buffer(rgbFrame, 0);

	SwsContext* swsContext = sws_getContext(
		m_context->width, m_context->height,
		AV_PIX_FMT_RGB24,
		m_context->width, m_context->height,
		 AV_PIX_FMT_YUV420P,
		SCALE_FLAGS, NULL, NULL, NULL);
	BEGIN_ERROR_HANDLE(swsContext,IsNULL);
	wxLogGeneric(wxLogLevelValues::wxLOG_Error, wxT("SwsContext초기화에 실패했습니다"));
	END_ERROR_HANDLE();
	m_pkt = av_packet_alloc();
	av_frame_make_writable(m_pictureEncoded);
	av_frame_make_writable(rgbFrame);
	
	for (int i = 0; i < imageStore.GetCount(); i++)
	{
		
		wxImage image = imageStore.Get(i).first;
		if (!image.IsOk())
		{
			wxLogGeneric(wxLogLevelValues::wxLOG_Error, wxT("image is not ok"));
		}
		auto bits = image.GetData();
		auto* frame = rgbFrame;
		for (int h = 0; h < imgSize.y; h++)
		{
			memcpy(frame->data[0] + h * frame->linesize[0], bits + h * imgSize.x * 3, imgSize.x * 3);
		}
		sws_scale(swsContext, (const uint8_t* const*)frame->data,
			frame->linesize, 0, frame->height, m_pictureEncoded->data,
			m_pictureEncoded->linesize);
		m_pictureEncoded->pts = i;
		//코덱에 프레임을 넣는데,
		while (auto ret = avcodec_send_frame(m_context, m_pictureEncoded) != 0)
		{
			//큐가 꽉차서 못 넣는 것이면 패킷을 처리한 후에 다시 시도한다.
			if (ret == AVERROR(EAGAIN))
			{
				receivePacket(handler);
				continue;
			}
			else if (ret == AVERROR_EOF)
			{
				break;
			}
			else
			{
				//그 외에는 처리할 수 없는 에러가 실패했다는 이벤트를 보내고 죽어버리자.
				avcodec_free_context(&m_context);
				av_frame_free(&rgbFrame);
				av_frame_free(&m_pictureEncoded);
				sws_freeContext(swsContext);
				avio_closep(&m_formatContext->pb);

				/* free the stream */
				avformat_free_context(m_formatContext);
				auto* event = new wxCommandEvent(EVT_FAILED_ENCODE);
				handler->QueueEvent(event);
				return;
			}
		}
		receivePacket(handler);
	}
	
	receivePacket(handler);

	int t = avcodec_send_frame(m_context, NULL);
	while (t >= 0) {
		av_init_packet(m_pkt);
		t = avcodec_receive_packet(m_context, m_pkt);
		if (t == AVERROR(EAGAIN))
		{
			t = 0;
			continue;
		}
		else if (ret == AVERROR_EOF)
		{
			break;
		}
		else if (t < 0) {
			break;
		}
		m_lastI++;
		auto* event = new wxCommandEvent(EVT_ADDED_A_FRAME);
		event->SetInt(m_lastI);
		handler->QueueEvent(event);
		m_pkt->duration = 1;
		av_packet_rescale_ts(m_pkt, m_context->time_base, m_stream->time_base); 
		m_pkt->stream_index = m_stream->index;
		av_interleaved_write_frame(m_formatContext, m_pkt);
		av_packet_unref(m_pkt);
	}

	avcodec_flush_buffers(m_context);
	avformat_flush(m_formatContext);
	av_write_trailer(m_formatContext);
	avio_closep(&m_formatContext->pb);
	avcodec_free_context(&m_context);
	av_frame_free(&rgbFrame);
	av_frame_free(&m_pictureEncoded);
	sws_freeContext(swsContext);
	

	/* free the stream */
	avformat_free_context(m_formatContext);
	auto* event = new wxCommandEvent(EVT_FINISH_ENCODE);
	handler->QueueEvent(event);
}

void WebmEncoder::SetQuality(int val)
{
	m_quality = val;
}

void WebmEncoder::StopEncode()
{
	m_requestedStop = true;
}

wxString WebmEncoder::GetFileFilter()
{
	return wxT("webm 동영상파일 파일 (*.webm)|*.webm");
}

wxString WebmEncoder::GetFileExtension()
{
	return wxString();
}

int WebmEncoder::receivePacket(wxEvtHandler* handler)
{
	int ret = 0;
	while (ret >= 0)
	{
		
		av_init_packet(m_pkt);
		ret = avcodec_receive_packet(m_context, m_pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
		{
			break;
		}
		if (ret < 0)
		{
			wxLogGeneric(wxLogLevelValues::wxLOG_Error, wxT("Error during encoding"));
			break;
		}
		m_lastI++;
		auto* event = new wxCommandEvent(EVT_ADDED_A_FRAME);
		event->SetInt(m_lastI);
		handler->QueueEvent(event);
		m_pkt->duration = 1;
		/* rescale output packet timestamp values from codec to stream timebase */
		av_packet_rescale_ts(m_pkt, m_context->time_base, m_stream->time_base);
		
		m_pkt->stream_index = m_stream->index;
		if (auto err = av_interleaved_write_frame(m_formatContext, m_pkt) != 0)
		{
			char bff[AV_ERROR_MAX_STRING_SIZE];
			wxString msg = av_make_error_string(bff, AV_ERROR_MAX_STRING_SIZE, err);
			wxLogGeneric(wxLogLevelValues::wxLOG_Error, msg);
		}
		else
		{
			
		}
		av_packet_unref(m_pkt);
	}
	return ret;
}

