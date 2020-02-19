#include "wx/wxprec.h"
#include "editrenderwidget.h"
#include <wx/dcbuffer.h>

EditFrameRenderWidget::EditFrameRenderWidget():
	m_timer(nullptr)
{
	Init();
}

EditFrameRenderWidget::~EditFrameRenderWidget()
{
	m_timer->Stop();
	delete m_timer;
}

void EditFrameRenderWidget::SetPresenter(EditFramePresenter* presenter)
{
	m_presenter = presenter;
}

void EditFrameRenderWidget::SetSelectImage(std::optional<size_t> index)
{
	if (m_isPlaying)
	{
		m_timer->Stop();
		m_isPlaying = false;
	}
	m_index = index;
	
	
	wxImage img;
	m_presenter->GetImage(*m_index, img, m_duration);
	if (img.IsOk())
	{
		m_bitmap = img;
	}
	wxClientDC dc(this);
	wxBufferedDC bufferdDc(&dc, GetClientSize());
	DoPaint(bufferdDc);
}

void EditFrameRenderWidget::PlayAnimImage()
{
	if (m_isPlaying)
	{
		m_isPlaying = false;
		m_timer->Stop();
		return;
	}
	if (m_presenter == nullptr)
	{
		return;
	}
	if (!m_index.has_value())
	{
		m_index = 0;
	}
	m_isPlaying = true;
	if (!m_timer->IsRunning())
	{
		m_timer->Start(10);
	}
	wxImage img;
	uint32_t duration;
	m_presenter->GetImage(*m_index, img, duration);
	if (img.IsOk())
	{
		m_bitmap = img;
	}
	m_duration = duration;
	m_lastTick = wxGetLocalTimeMillis().GetValue();
	wxClientDC dc(this);
	wxBufferedDC bufferdDc(&dc, GetClientSize());
	DoPaint(bufferdDc);
}

void EditFrameRenderWidget::Init()
{
	m_timer = new wxTimer(this);
	m_isPlaying = false;
}

void EditFrameRenderWidget::ProcessAnim()
{
	auto now = wxGetLocalTimeMillis().GetValue();
	auto cap = now - m_lastTick;
	if (cap >= m_duration)
	{
		
		if (!m_index.has_value())
			m_index = 0;
		m_lastTick = now;
		std::optional<uint32_t> duration = m_duration;
		while(cap >= duration)
		{
			cap -= *duration;
			m_index = *m_index + 1;
			duration = m_presenter->GetFrameDuration(*m_index);
			if (!duration.has_value())
			{
				m_index = 0;
				continue;
			}
		}
		uint32_t due;
		wxImage img;
		m_presenter->GetImage(*m_index,img, due);

		if (img.IsOk())
		{
			m_bitmap = img;
		}
		m_duration = due - cap;
		wxClientDC dc(this);
		wxBufferedDC bufferdDc(&dc, GetClientSize());
		DoPaint(bufferdDc);
	}
}

void EditFrameRenderWidget::DoPaint(wxDC& dc)
{
	if(m_bitmap.IsOk())
		dc.DrawBitmap(m_bitmap, wxPoint(0, 0));
}

void EditFrameRenderWidget::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	DoPaint(dc);
}

void EditFrameRenderWidget::OnTimer(wxTimerEvent& event)
{
	ProcessAnim();
}

void EditFrameRenderWidget::OnIdle(wxIdleEvent& event)
{
	if (m_isPlaying)
	{
		ProcessAnim();
	}
}


wxBEGIN_EVENT_TABLE(EditFrameRenderWidget, wxControl)
EVT_PAINT(EditFrameRenderWidget::OnPaint)
EVT_TIMER(wxID_ANY,EditFrameRenderWidget::OnTimer)
EVT_IDLE(EditFrameRenderWidget::OnIdle)
wxEND_EVENT_TABLE()
wxIMPLEMENT_DYNAMIC_CLASS(EditFrameRenderWidget, wxControl)
