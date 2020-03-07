#pragma once
#include "interface.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
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
	AVPacket* m_pkt;
	AVStream* m_stream;
	wxFile m_file;
	bool m_requestedStop = false;
	int m_quality;
	int m_lastI = -1;
public:
	WebmEncoder();
	~WebmEncoder();
	void Encode(wxEvtHandler* handler, const wxString filePath, IImageStore& imageStore) override;
	void SetQuality(int) override;

	void StopEncode() override;
	wxString GetFileFilter() override;
	wxString GetFileExtension() override;
private:
	int receivePacket(IImageStore& imageStore, wxEvtHandler* handler);
};