#pragma once
#include "interface.h"

struct GDICapturer : public ICapturer
{
public:
	GDICapturer();
	virtual ~GDICapturer();
	virtual void BeginCapture(wxEvtHandler* handler, const wxRect& rect, uint32_t duration) override;
	virtual wxImage CaptureFrame()  override;
	virtual void EndCapture()  override;
private:
	wxRect m_rect;
	wxEvtHandler* m_handler;
	uint32_t m_duration;
	wxBitmap m_bitmap;
	wxMemoryDC* m_memDC;
};