#pragma once
#include "interface.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include<wx/file.h>
class WebmEncoder : public IEncoder
{
private:
	AVFormatContext* m_formatContext;
	AVOutputFormat* fmt;
	AVCodec* m_codecEncode;
	AVCodecContext* m_context;
	AVDictionary* m_opt;
	AVFrame* m_pictureEncoded;
	AVFrame* m_pictureDecoded;
	AVPacket* m_pkt;
	AVStream* m_stream;
	wxFile m_file;
	bool m_requestedStop = false;
public:
	WebmEncoder();
	~WebmEncoder();
	void Encode(wxEvtHandler* handler, const wxString filePath, IImageStore& imageStore) override;
	void StopEncode() override;
	wxString GetFileFilter() override;
	wxString GetFileExtension() override;
private:
    static void encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt,
		wxFile& file);
};