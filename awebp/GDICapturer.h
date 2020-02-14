#pragma once
#include "interface.h"

struct GDICapturer : public ICapturer
{
public:
	virtual void BeginCapture(wxEvtHandler* handler, const wxRect& rect, uint32_t duration) override;
	virtual std::pair<wxImage, uint32_t> CaptureFrame()  override;
	virtual void EndCapture()  override;
private:
	wxRect m_rect;
	wxEvtHandler* m_handler;
	uint32_t m_duration;
};