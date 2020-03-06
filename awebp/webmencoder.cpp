#include "wx/wxprec.h"
#include "webmencoder.h"
#include <libavutil/imgutils.h>
#include "event.h"

#include<wx/log.h>
#define ERROR_HANDLER(func, condi, handle) {auto __ret = func; if(__ret condi){ handle; return;}}
#define IsNULL ==nullptr
#define IsNotNull !=nullptr
#define RETURN() __ret
#define SCALE_FLAGS SWS_BICUBIC
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "avformat.lib")

WebmEncoder::WebmEncoder():
	m_codecEncode(nullptr),
	m_context(nullptr),
	m_pictureEncoded(nullptr),
	m_pictureDecoded(nullptr),
	m_pkt(nullptr),
	m_opt(nullptr)
{
	m_codecEncode = avcodec_find_encoder(AVCodecID::AV_CODEC_ID_VP9);
	m_context = avcodec_alloc_context3(m_codecEncode);
	ERROR_HANDLER(
		avformat_alloc_output_context2(&m_formatContext, nullptr, "webm", nullptr),
		!= 0,
		wxLogGeneric(wxLogLevelValues::wxLOG_Error, wxT("webm파일 포맷을 만들 수 없습니다."))
	);
	
	
	m_formatContext->oformat->video_codec = AV_CODEC_ID_VP9;
	m_formatContext->oformat->audio_codec = AV_CODEC_ID_NONE;
	
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
	m_stream = avformat_new_stream(m_formatContext, m_codecEncode);
	ERROR_HANDLER(
		m_stream,
		IsNULL,
		wxLogGeneric(wxLogLevelValues::wxLOG_Error, wxT("비디오 인코드 스트림을 만들 수 없습니다."))
	);
	m_stream->id = m_formatContext->nb_streams - 1;

	auto imgSize = imageStore.GetImageSize();
	m_context->pix_fmt = AV_PIX_FMT_YUV420P;
	m_context->width = imgSize.GetWidth();
	m_context->height = imgSize.GetHeight();
	m_context->bit_rate = 1024 * 3000;
	m_context->time_base.den = 10;
	m_context->time_base.num = 1;
	m_context->time_base = m_stream->time_base;
	m_context->framerate.den = 1;
	m_context->framerate.num = 10;
	m_context->qmin = 0;
	m_context->qmax = 60;
	m_context->gop_size = imageStore.GetCount();
	/* Some formats want stream headers to be separate. */
	if (m_formatContext->oformat->flags & AVFMT_GLOBALHEADER)
		m_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	ERROR_HANDLER(
		avio_open(&m_formatContext->pb, filePath.ToUTF8().data(), AVIO_FLAG_WRITE),
		< 0, 
		wxLogGeneric(wxLogLevelValues::wxLOG_Error, wxT("파일 열기에 실패했습니다."))
	);
	ERROR_HANDLER(
		avformat_write_header(m_formatContext, &m_opt),
		< 0,
		{
			wxString msg = av_err2str(RETURN());
			wxLogGeneric(wxLogLevelValues::wxLOG_Error, msg);
		}
	);
	avcodec_open2(m_context, m_codecEncode, nullptr);
	avcodec_parameters_from_context(m_stream->codecpar, m_context);
	
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
		AV_PIX_FMT_YUV420P,
		m_context->width, m_context->height,
		AV_PIX_FMT_RGB24,
		SCALE_FLAGS, NULL, NULL, NULL);
	ERROR_HANDLER(
		swsContext,
		IsNULL,
		{
			wxLogGeneric(wxLogLevelValues::wxLOG_Error, wxT("SwsContext초기화에 실패했습니다"));
		}
	);

	for (int i = 0; i < imageStore.GetCount(); i++)
	{
		av_init_packet(m_pkt);
		wxImage image = imageStore.Get(i).first;
		auto bits = image.GetData();
		auto* frame = rgbFrame;
		int got_packet = 0;
		av_frame_make_writable(frame);
		for (int h = 0; h < imgSize.GetHeight(); h++)
		{
			for (int w = 0; w < imgSize.GetHeight(); w++)
			{
				frame->data[0][h * frame->linesize[0] + w] = bits[h * (image.GetWidth() * 3) + w];
				frame->data[1][h * frame->linesize[1] + w] = bits[h * (image.GetWidth() * 3) + w + 1];
				frame->data[2][h * frame->linesize[2] + w] = bits[h * (image.GetWidth() * 3) + w + 2];
			}
		}
		sws_scale(swsContext, (const uint8_t* const*)frame->data,
			frame->linesize, 0, frame->height, m_pictureEncoded->data,
			m_pictureEncoded->linesize);
		m_pictureEncoded->pts = i;
		avcodec_encode_video2(m_context, m_pkt, m_pictureEncoded, &got_packet);
		if (!got_packet)
			continue;
		/* rescale output packet timestamp values from codec to stream timebase */
		av_packet_rescale_ts(m_pkt, m_context->time_base, m_stream->time_base);
		m_pkt->stream_index = m_stream->index;
		{
			auto* event = new wxCommandEvent(EVT_ADDED_A_FRAME);
			event->SetInt(m_pkt->pts);
			handler->QueueEvent(event);
		}
		/* Write the compressed frame to the media file. */
		//log_packet(fmt_ctx, pkt);
		av_interleaved_write_frame(m_formatContext, m_pkt);

	}
	av_write_trailer(m_formatContext);
	avcodec_free_context(&m_context);
	av_frame_free(&rgbFrame);
	av_frame_free(&m_pictureEncoded);
	sws_freeContext(swsContext);
	avio_closep(&m_formatContext->pb);

	/* free the stream */
	avformat_free_context(m_formatContext);
	auto* event = new wxCommandEvent(EVT_FINISH_ENCODE);
	handler->QueueEvent(event);
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

 void WebmEncoder::encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt, wxFile& file)
{
	int ret;

	/* send the frame to the encoder */
	if (frame)
		//printf("Send frame %3"PRId64"\n", frame->pts);

	ret = avcodec_send_frame(enc_ctx, frame);
	if (ret < 0) {
		fprintf(stderr, "Error sending a frame for encoding\n");
		exit(1);
	}

	while (ret >= 0) {
		ret = avcodec_receive_packet(enc_ctx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0) {
			fprintf(stderr, "Error during encoding\n");
			exit(1);
		}

		//printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
		file.Write(pkt->data, pkt->size);
		av_packet_unref(pkt);
	}
}
