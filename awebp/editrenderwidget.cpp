#include "wx/wxprec.h"
#include "editrenderwidget.h"
#include "event.h"
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
	DoPaint(dc);
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
	DoPaint(dc);
}

void EditFrameRenderWidget::Init()
{
	m_timer = new wxTimer(this);
	m_isPlaying = false;
#ifdef wxUSE_GRAPHICS_DIRECT2D
	m_renderer = wxGraphicsRenderer::GetDirect2DRenderer();
#else
	m_renderer = wxGraphicsRenderer::GetDefaultRenderer();
#endif
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
		wxCommandEvent* e = new wxCommandEvent(EVT_ANIM_PROCESS_A_FRAME);
		e->SetInt(*m_index);
		this->QueueEvent(e);
		if (img.IsOk())
		{
			m_bitmap = img;
		}
		m_duration = due - cap;
		wxClientDC dc(this);
		DoPaint(dc);
	}
}

void EditFrameRenderWidget::DoPaint(wxClientDC& dc)
{
	auto context = m_renderer->CreateContext(dc);
	if (m_bitmap.IsOk())
	{
		wxSize rc = this->GetClientSize();
		wxSize imageSize = m_bitmap.GetSize();
		wxPoint viewStart(0,0);
		if (imageSize.x  <= rc.x)
		{
			viewStart.x = (imageSize.x  - rc.x) / 2;
		}
		if (imageSize.y  <= rc.y)
		{
			viewStart.y = (imageSize.y  - rc.y) / 2;
		}
		context->Translate(-viewStart.x, -viewStart.y);
		context->DrawBitmap(m_bitmap, 
			0, 0,
			imageSize.x, imageSize.y);
	}
	delete context;
}

void EditFrameRenderWidget::DoPaint()
{

}

void EditFrameRenderWidget::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
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
