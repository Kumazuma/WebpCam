#pragma once
#include "interface.h"
#include "webp/encode.h"
#include "webp/mux.h"
#include <wx/msgqueue.h>
class WebpEncoder : public IEncoder
{
public:
	~WebpEncoder();
	void Encode(wxEvtHandler* handler, const wxString filePath, IImageStore& imageStore) override;
	wxString GetFileFilter() override;
	wxString GetFileExtension() override;
private:
	WebPAnimEncoder* m_encoder;
	WebPAnimEncoderOptions m_encoderOption;
	WebPConfig m_config;
	WebPPicture m_frame;
	wxMessageQueue<std::pair<wxImage, uint32_t>> m_queue;
	wxThread* m_loopThread;
};