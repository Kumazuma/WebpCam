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
	m_presenter(&presenter), m_scale(100), m_centerPoint(0,0)
{
	AlwaysShowScrollbars(false, false);
	
	m_workArea.Create(m_presenter->GetImageSize());
	m_memDC =new wxMemoryDC(m_workArea);
	m_memDC->Clear();
	wxImage image;
	uint32_t du;
	m_presenter->GetImage(0, image, du);
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
	wxSize rc = this->GetClientSize();
	wxSize imageSize = m_workArea.GetSize();
	wxDouble dpix, dpiy;
	gc->GetDPI(&dpix, &dpiy);
	
	auto s = (dpix / 96.f);
	
	gc->Translate(rc.x / 2.f - m_centerPoint.x, rc.y / 2.f - m_centerPoint.y);
	gc->Scale(m_scale / 100, m_scale / 100);
	
	gc->DrawBitmap(m_workArea,  -1 * imageSize.x / 2.f, -1 * imageSize.y / 2.f, imageSize.x,  imageSize.y);
	
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
	bdc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(bdc);
	OverDraw(gc);
	delete gc;
	auto rc = GetClientSize() / 2;
	bdc.DrawRectangle(rc.x - 10, rc.y - 10, 20, 20);
}

void Edit::EditCropToolWidget::OnScrollWinEvent(wxScrollWinEvent& event)
{
	auto pos = event.GetPosition();
	auto areaSize = m_workArea.GetSize();
	auto clientSize = GetClientSize();
	auto gapx = (areaSize.x * m_scale / 100 - clientSize.x)/50;
	auto gapy = (areaSize.y * m_scale / 100 - clientSize.y)/50;
	switch (event.GetOrientation())
	{
	case wxVERTICAL:
		if (event.GetEventType() == wxEVT_SCROLLWIN_THUMBRELEASE)
		{
			printf("start\n");
		}
		m_centerPoint.y = gapy * (pos - 50);
		//m_centerPoint.y /= 2;
		break;
	case wxHORIZONTAL:
		if (event.GetEventType() == wxEVT_SCROLLWIN_THUMBRELEASE)
		{
			printf("start\n");
		}
		m_centerPoint.x = gapx *(pos - 50);
		//m_centerPoint.x /= 2;
		break;
	}
	SetScrollPos(event.GetOrientation(), pos, true);
	DoPaint();
	OverDraw();
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
	auto areaSize = m_workArea.GetSize();
	auto clientSize = GetClientSize();
	auto gapx = (areaSize.x * m_scale / 100 - clientSize.x) / 50;
	auto gapy = (areaSize.y * m_scale / 100 - clientSize.y) / 50;


	if (areaSize.x * m_scale / 100 > clientSize.x)
	{
		SetWindowStyle(GetWindowStyle()| wxHSCROLL);
		SetScrollbar(wxHSCROLL,
			m_centerPoint.x / gapx + 50,
			1,
			100, true);
	}
	else
	{
		m_centerPoint.x = 0;
		SetWindowStyle(GetWindowStyle() & ~(wxHSCROLL));
	}
	if (areaSize.y * m_scale / 100 > clientSize.y)
	{
		SetWindowStyle(GetWindowStyle() | wxVSCROLL);
		SetScrollbar(wxVSCROLL,
			m_centerPoint.y / gapy + 50,//버튼의 위치
			1,//한 화면에 보여줄 양
			100, true);//전체 크기
	}
	else
	{
		m_centerPoint.y = 0;
		SetWindowStyle(GetWindowStyle() & (~wxVSCROLL));
	}
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
		auto areaSize = m_workArea.GetSize();
		auto clientSize = GetClientSize();
		if (areaSize.x * m_scale / 100 > clientSize.x)
		{
			SetScrollbar(wxHSCROLL,
				0,
				1,
				100, true);
		}
		else
		{
			m_centerPoint.x = 0;
			SetScrollbar(wxHSCROLL, 0, 0, 0, true);
		}
		if (areaSize.y * m_scale / 100 > clientSize.y)
		{
			SetScrollbar(wxVSCROLL,
				0,//버튼의 위치
				1,//한 화면에 보여줄 양
				100, true);//전체 크기
		}
		else
		{
			
			m_centerPoint.y = 0;
			SetScrollbar(wxVSCROLL, 0, 0, 0, false);
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
		EVT_SIZE(EditCropToolWidget::OnSize)
		EVT_SCROLLWIN(EditCropToolWidget::OnScrollWinEvent)
	wxEND_EVENT_TABLE()
}

