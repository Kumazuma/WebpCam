#pragma once
#include "interface.h"
#include "webp/encode.h"
#include "webp/mux.h"
#include <wx/msgqueue.h>
class WebpEncoder : public IEncoder
{
public:
	bool BeginEncode(wxEvtHandler* handler, uint32_t width, uint32_t height) override;
	bool AddFrame(const wxImage& frame, uint32_t duration) override;
	bool EndEncode() override;
private:
	WebPAnimEncoder* m_encoder;
	WebPAnimEncoderOptions m_encoderOption;
	WebPConfig m_config;
	WebPPicture m_frame;
	uint32_t m_timestamp;
	uint32_t m_duration;
	wxLongLong m_prevTick;
	wxMessageQueue<std::pair<wxImage, uint32_t>> m_queue;
	wxThread* m_loopThread;
};