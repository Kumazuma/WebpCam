#pragma once
#include "interface.h"
#include "webp/encode.h"
#include "webp/mux.h"
#include <wx/msgqueue.h>
class WebpEncoder : public IEncoder
{
private:
	bool m_requestedStop = false;
	WebPAnimEncoder* m_encoder = nullptr;
	WebPAnimEncoderOptions m_encoderOption;
	WebPConfig m_config;
	WebPPicture m_frame;
	wxMessageQueue<std::pair<wxImage, uint32_t>> m_queue;
	int m_quality;
public:
	~WebpEncoder();
	void Encode(wxEvtHandler* handler, const wxString filePath, IImageStore& imageStore) override;
	void StopEncode() override;
	wxString GetFileFilter() override;
	wxString GetFileExtension() override;
	void SetQuality(int) override;

};