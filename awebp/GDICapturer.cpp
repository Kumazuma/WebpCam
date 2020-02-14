#include "wx/wxprec.h"
#include "GDICapturer.h"

void GDICapturer::BeginCapture(wxEvtHandler* handler, const wxRect& rect , uint32_t duration)
{
	m_rect = rect;
	m_duration = duration;
	m_handler = handler;
}

std::pair<wxImage, uint32_t> GDICapturer::CaptureFrame()
{
	
	wxScreenDC dc;
	wxBitmap bitmap(m_rect.GetSize());
	auto* memDC = new wxMemoryDC(bitmap);
	memDC->Blit(wxPoint(0, 0), m_rect.GetSize(), &dc, m_rect.GetPosition());
	delete memDC;
	return std::pair<wxImage, uint32_t>(bitmap.ConvertToImage(), m_duration);
}

void GDICapturer::EndCapture()
{

}
