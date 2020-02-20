#include "wx/wxprec.h"
#include "editcropwidget.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
Edit::EditCropToolWidget::EditCropToolWidget():
	m_presenter(nullptr)
{
}
Edit::EditCropToolWidget::EditCropToolWidget(EditFramePresenter& presenter, wxWindow* parent, wxWindowID id):
	wxControl(parent, id, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxHSCROLL),
	m_presenter(&presenter), m_scale(100)
{
	
	SetScrollbar(wxVSCROLL, 0, 1, 0, false);
	SetScrollbar(wxHSCROLL, 0, 1, 0,false);
	m_workArea.Create(m_presenter->GetImageSize());
	m_memDC =new wxMemoryDC(m_workArea);
	m_memDC->Clear();
}
void Edit::EditCropToolWidget::DoPaint()
{

}
Edit::EditCropToolWidget::~EditCropToolWidget()
{
	delete m_memDC;
}

void Edit::EditCropToolWidget::OverDraw()
{
	wxClientDC dc(this);
	wxBufferedDC bdc(&dc, GetSize());
	bdc.Clear();
	wxSize rc = this->GetSize();
	wxSize imageSize = m_workArea.GetSize() / 2;
	wxGraphicsContext* gc = wxGraphicsContext::Create(bdc);

	gc->Translate(rc.x / 2, rc.y / 2);
	gc->Scale(m_scale / 100.f, m_scale / 100.f);
	gc->DrawBitmap(m_workArea, -imageSize.x / 2, -imageSize.y / 2, imageSize.x, imageSize.y);
	gc->DrawRectangle(-1, -1, 2, 2);
	gc->Flush();
	delete gc;

}

void Edit::EditCropToolWidget::OnScrollWinEvent(wxScrollWinEvent& event)
{
	SetScrollPos(event.GetOrientation(), event.GetPosition(), false);
}
void Edit::EditCropToolWidget::OnPaint(wxPaintEvent& event)
{
	//여기서는 단순히 작업 영역을 다시 출력만 한다.
	wxBufferedPaintDC dc(this);
	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
	float size[2];
	wxRect rc = this->GetRect();
	auto s = m_workArea.GetSize();
	size[0] = s.x * (m_scale / 100.f);
	size[1] = s.y * (m_scale / 100.f);
	float x = (rc.width - size[0]) / 2;
	float y = (rc.height - size[1]) / 2;
	dc.SetBackground(wxColor(0xCC, 0xCC, 0xCC));
	dc.Clear();
	gc->DrawBitmap(m_workArea, x, y, size[0], size[1]);
	gc->Flush();
	delete gc;
	
}
void Edit::EditCropToolWidget::OnMouseLeftDown(wxMouseEvent& event)
{
}
void Edit::EditCropToolWidget::OnMouseLeftUp(wxMouseEvent& event)
{

}
void Edit::EditCropToolWidget::OnMouseMotion(wxMouseEvent& event)
{

}
void Edit::EditCropToolWidget::OnMouseWheel(wxMouseEvent& event)
{
	if (event.ControlDown())
	{
		auto sign = event.GetWheelRotation() / (abs(event.GetWheelRotation()));
		m_scale += sign* (event.GetWheelDelta() / 120) * 10;
		if (m_scale <= 0)
		{
			m_scale = 10;
		}
		DoPaint();
		OverDraw();
	}
}
namespace Edit
{
	wxIMPLEMENT_DYNAMIC_CLASS(EditCropToolWidget, wxControl);
	wxBEGIN_EVENT_TABLE(EditCropToolWidget, wxControl)
		EVT_LEFT_DOWN(EditCropToolWidget::OnMouseLeftDown)
		EVT_LEFT_UP(EditCropToolWidget::OnMouseLeftUp)
		EVT_MOTION(EditCropToolWidget::OnMouseMotion)
		EVT_MOUSEWHEEL(EditCropToolWidget::OnMouseWheel)
		EVT_PAINT(EditCropToolWidget::OnPaint)
		EVT_SCROLLWIN(EditCropToolWidget::OnScrollWinEvent)
	wxEND_EVENT_TABLE()
}

