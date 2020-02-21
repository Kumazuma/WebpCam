#include "wx/wxprec.h"
#include "editcropwidget.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
void Edit::EditCropToolWidget::ScrollWindow(int dx, int dy, const wxRect* rect)
{
	m_centerPoint.x -= dx;
	m_centerPoint.y -= dy;
	wxClientDC dc(this);

	wxBufferedDC bdc(&dc, GetSize());
	
	bdc.SetBackground(wxBrush(0xCCCCCC));
	bdc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(bdc);
	OverDraw(gc);
	delete gc;
}
Edit::EditCropToolWidget::EditCropToolWidget():
	m_presenter(nullptr)
{
}
Edit::EditCropToolWidget::EditCropToolWidget(EditFramePresenter& presenter, wxWindow* parent, wxWindowID id):
	wxScrolledCanvas(parent, id, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxHSCROLL),
	m_presenter(&presenter), m_scale(1), m_centerPoint(0,0)
{
	SetScrollRate(1, 1);
	wxImage image;
	uint32_t du;
	m_presenter->GetImage(0, image, du);
	SetVirtualSize(image.GetSize());
	AlwaysShowScrollbars();
	
	m_workArea.Create(m_presenter->GetImageSize());
	m_memDC =new wxMemoryDC(m_workArea);
	m_memDC->Clear();
	m_memDC->DrawBitmap(image, 0, 0);
	
}
void Edit::EditCropToolWidget::DoPaint()
{
	wxImage image;
	uint32_t du;
	m_presenter->GetImage(0, image, du);
	m_memDC->DrawBitmap(image,0,0);
}
void Edit::EditCropToolWidget::OverDraw(wxGraphicsContext* gc)
{
	if (!m_workArea.IsOk())
		return;
	wxSize rc = this->GetClientSize();
	wxSize imageSize = m_workArea.GetSize();
	wxPoint viewStart = m_centerPoint;
	if (imageSize.x * m_scale  <= rc.x)
	{
		viewStart.x = ( imageSize.x * m_scale  - rc.x)/2;
	}
	if (imageSize.y * m_scale <= rc.y)
	{
		viewStart.y = (imageSize.y * m_scale  - rc.y)/ 2;
	}
	
	gc->Translate(-viewStart.x , -viewStart.y);
	gc->Scale(m_scale , m_scale );
	gc->DrawBitmap(m_workArea, 0, 0 , imageSize.x,  imageSize.y);
	gc->Flush();
}
Edit::EditCropToolWidget::~EditCropToolWidget()
{
	delete m_memDC;
}

void Edit::EditCropToolWidget::OverDraw()
{
	wxClientDC dc(this);
	wxBufferedDC bdc(&dc, GetSize());
	bdc.SetBackground(wxBrush(0xCCCCCC));
	bdc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(bdc);
	OverDraw(gc);
	delete gc;
	auto rc = GetClientSize() / 2;
	bdc.DrawRectangle(rc.x - 10, rc.y - 10, 20, 20);

}

void Edit::EditCropToolWidget::OnScrollWinEvent(wxScrollWinEvent& event)
{
	event.Skip();
	this->GetScrollHelper()->HandleOnScroll(event);
}
void Edit::EditCropToolWidget::OnPaint(wxPaintEvent& event)
{
	//여기서는 단순히 작업 영역을 다시 출력만 한다.
	wxPaintDC dc(this);
	dc.SetBackground(wxBrush(0xCCCCCC));
	dc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
	OverDraw(gc);
	delete gc;
	
}
void Edit::EditCropToolWidget::OnSize(wxSizeEvent& event)
{
	event.Skip();
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
		auto style = GetWindowStyle();
		if (event.GetWheelRotation() == 0)
			return;
		auto sign = event.GetWheelRotation() / (abs(event.GetWheelRotation()));
		auto prevScale = m_scale;
		m_scale += 0.01f * sign* (event.GetWheelDelta() / 120) * 10;
		if (m_scale <= 0)
		{
			m_scale = 0.1f;
		}
		SetVirtualSize(m_workArea.GetSize() * m_scale );
		auto s = this->GetViewStart();
		auto t = m_workArea.GetSize();
		t.x =(m_scale * s.x) / prevScale;
		t.y =(m_scale * s.y) / prevScale;
		
		AdjustScrollbars();
		Scroll(t.x, t.y);
		DoPaint();
		OverDraw();

	}
}
namespace Edit
{
	wxIMPLEMENT_DYNAMIC_CLASS(EditCropToolWidget, wxScrolledCanvas);
	wxBEGIN_EVENT_TABLE(EditCropToolWidget, wxScrolledCanvas)
		EVT_LEFT_DOWN(EditCropToolWidget::OnMouseLeftDown)
		EVT_LEFT_UP(EditCropToolWidget::OnMouseLeftUp)
		EVT_MOTION(EditCropToolWidget::OnMouseMotion)
		EVT_MOUSEWHEEL(EditCropToolWidget::OnMouseWheel)
		EVT_PAINT(EditCropToolWidget::OnPaint)
		EVT_SIZE(EditCropToolWidget::OnSize)
		EVT_SCROLLWIN(EditCropToolWidget::OnScrollWinEvent)
	wxEND_EVENT_TABLE()
}

