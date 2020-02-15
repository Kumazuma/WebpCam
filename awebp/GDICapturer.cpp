#include "wx/wxprec.h"
#include "GDICapturer.h"


GDICapturer::GDICapturer():
	m_memDC(nullptr)
{

}

GDICapturer::~GDICapturer()
{
	if (m_memDC != nullptr)
	{
		m_memDC = nullptr;
	}
}

void GDICapturer::BeginCapture(wxEvtHandler* handler, const wxRect& rect , uint32_t duration)
{
	m_rect = rect;
	m_duration = duration;
	m_handler = handler;
	m_bitmap.Create(rect.GetSize());
	m_memDC = new wxMemoryDC(m_bitmap);
}

wxImage GDICapturer::CaptureFrame()
{
	wxScreenDC dc;
	m_memDC->Blit(wxPoint(0, 0), m_rect.GetSize(), &dc, m_rect.GetPosition());
	return m_bitmap.ConvertToImage();
}

void GDICapturer::EndCapture()
{
	delete m_memDC;
	m_memDC = nullptr;
}
