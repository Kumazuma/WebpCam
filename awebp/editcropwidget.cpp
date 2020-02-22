#include "wx/wxprec.h"
#include "editcropwidget.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/rawbmp.h>
#include <math.h>
#ifdef __WXMSW__
#pragma comment(lib, "D2D1.lib") 
template<class Interface>
inline void SafeRelease(
	Interface*& pInterfaceToRelease
)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();

		pInterfaceToRelease = NULL;
	}
}
template<typename T>
decltype(D2D1::SizeU()) ToSizeU(const T& obj)
{
	return D2D1::SizeU(obj.x, obj.y);
}

RECT ToRect(const wxRect& obj)
{
	return RECT{ obj.x, obj.y,obj.width - obj.x, obj.height- obj.y };
}
#endif
void Edit::EditCropToolWidget::InitDirect2D()
{
#ifdef __WXMSW__
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
	InitResource();
#endif
}

void Edit::EditCropToolWidget::InitResource()
{
#ifdef __WXMSW__
	float dpi = GetDpiForWindow(this->GetHWND());
	float a = dpi / 96;
	auto d2SizeU = ToSizeU(GetClientSize());
	if (!m_renderTarget)
	{
		D2D1_RENDER_TARGET_PROPERTIES dxProperties =
			D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE,
				D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
		auto propertyWndRenderTarget = D2D1::HwndRenderTargetProperties(
			GetHWND(), ToSizeU(GetClientSize()));
		
		FLOAT dpiX, dpiY;
		
		m_factory->CreateHwndRenderTarget(&dxProperties,&propertyWndRenderTarget, &m_renderTarget);
		m_renderTarget->SetDpi(96, 96);
		
	}
	if (!m_bitmap &&
		m_backbuffer.IsOk())
	{
		

	}
	if (m_backbuffer.IsOk())
	{
		int strip = m_backbuffer.GetWidth() * 4;
		size_t wxh = m_backbuffer.GetHeight() * m_backbuffer.GetWidth();
		auto imgData = m_backbuffer.GetData();
		std::vector<uint8_t> data;
		data.assign(wxh * 4, 0);
		D2D1_RECT_U d2Size = D2D1::RectU(0, 0, m_backbuffer.GetWidth(), m_backbuffer.GetHeight());
		for (size_t i = 0; i < wxh; i++)
		{
			data[i * 4] = imgData[i * 3];
			data[i * 4 + 1] = imgData[i * 3 + 1];
			data[i * 4 + 2] = imgData[i * 3 + 2];
			data[i * 4 + 3] = 0x00;
		}
		if (m_bitmap)
		{
			m_bitmap->CopyFromMemory(&d2Size, data.data(), strip);
		}
		else
		{
			D2D_SIZE_U d2Size = D2D1::SizeU(m_backbuffer.GetWidth(), m_backbuffer.GetHeight());
			auto d2BitmapProperty = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
			m_renderTarget->CreateBitmap(d2Size, data.data(), strip, d2BitmapProperty, &m_bitmap);
		}
		
	}
#endif
}
void Edit::EditCropToolWidget::ReleaseResource()
{
#ifdef __WXMSW__
	SafeRelease(m_renderTarget);
	SafeRelease(m_bitmap);
#endif
}
void Edit::EditCropToolWidget::ReleaseDirect2D()
{
#ifdef __WXMSW__
	ReleaseResource();
	SafeRelease(m_factory);
#endif
}
void Edit::EditCropToolWidget::ScrollWindow(int dx, int dy, const wxRect* rect)
{
	m_centerPoint.x -= dx;
	m_centerPoint.y -= dy;
	OverDraw();
	
}
Edit::EditCropToolWidget::EditCropToolWidget():
	m_presenter(nullptr)
{

}
Edit::EditCropToolWidget::EditCropToolWidget(EditFramePresenter& presenter, wxWindow* parent, wxWindowID id):
	wxScrolledCanvas(parent, id, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS|wxVSCROLL|wxHSCROLL),
	m_presenter(&presenter), m_scale(1), m_centerPoint(0,0)
{
	InitDirect2D();
	SetScrollRate(1, 1);
	SetFocus();
	auto imgSize = m_presenter->GetImageSize();
	SetVirtualSize(imgSize);
	m_cropArea = m_presenter->GetImageSize();
	//TEST
	auto min =std::min(m_cropArea.width, m_cropArea.height);
	m_cropArea.x = (m_cropArea.width - min  ) / 2;
	m_cropArea.y = (m_cropArea.height - min) / 2;
	m_cropArea.width = min;
	m_cropArea.height = min;
	
	
	AlwaysShowScrollbars();
	
	m_workArea.Create(m_presenter->GetImageSize());
	m_memDC =new wxMemoryDC(m_workArea);
	m_memDC->Clear();
	DoPaint();
	OverDraw();
}
void Edit::EditCropToolWidget::DoPaint()
{
	wxImage image;
	uint32_t du;
	m_presenter->GetImage(0, image, du);
	//m_memDC->DrawBitmap(image,0,0);
	//auto * gc = wxGraphicsContext::Create(*m_memDC);
	//auto s = image.GetSize();
	//gc->DrawBitmap(image,0,0,s.GetWidth(), s.GetHeight() );
	//wxRegion rc(wxRect(0,0,s.x,s.y));
	//rc.Subtract(wxRegion(m_cropArea));
	//wxGraphicsPath path = gc->CreatePath();
	//wxBrush brush(wxColor(0,0,0,128), wxBrushStyle::wxBRUSHSTYLE_SOLID);
	//path.AddRectangle(0, 0, s.GetWidth(), s.GetHeight());
	//path.AddRectangle(m_cropArea.x, m_cropArea.y, m_cropArea.width, m_cropArea.height);
	//
	//gc->SetBrush(brush);
	//gc->FillPath(path);
	//delete gc;
	//
	m_backbuffer = m_workArea.ConvertToImage();
	InitResource();
}

Edit::EditCropToolWidget::~EditCropToolWidget()
{
	ReleaseDirect2D();
}
#ifndef __WXMSW__
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
void Edit::EditCropToolWidget::OverDraw()
{
	wxClientDC dc(this);
	wxBufferedDC bdc(&dc, GetSize());
	bdc.SetBackground(wxBrush(0xCCCCCC));
	bdc.Clear();
	wxGraphicsContext* gc = wxGraphicsContext::Create(bdc);
	OverDraw(gc);
	delete gc;

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
#else
void Edit::EditCropToolWidget::OverDraw()
{
	//this->Refresh(true, &GetClientRect());
	//this->
	auto now = wxGetLocalTimeMillis().GetValue();
	if (now - m_lastRenderTime < 5)
	{
		return;
	}
	m_lastRenderTime = now;
	if (!m_renderTarget)
	{
		InitResource();
	}
	float dpi = GetDpiForWindow(this->GetHWND());
	float a = dpi / 96;
	wxSize rc = this->GetClientSize();
	wxSize imageSize = m_workArea.GetSize();
	wxPoint viewStart = m_centerPoint;
	if (imageSize.x * m_scale <= rc.x)
	{
		viewStart.x = (imageSize.x * m_scale - rc.x ) / 2;
	}
	if (imageSize.y * m_scale <= rc.y)
	{
		viewStart.y = (imageSize.y * m_scale - rc.y ) / 2;
	}
	if (m_bitmap == nullptr)
	{
		return;
	}
	
	m_renderTarget->BeginDraw();
	m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	auto translation = D2D1::Matrix3x2F::Translation(-viewStart.x , -viewStart.y );
	auto scale = D2D1::Matrix3x2F::Scale(m_scale , m_scale );
	m_renderTarget->SetTransform(translation  * scale);
	m_renderTarget->DrawBitmap(m_bitmap, D2D1::RectF(0, 0, m_bitmap->GetSize().width, m_bitmap->GetSize().height));

	auto res = m_renderTarget->EndDraw();
	if (res == D2DERR_RECREATE_TARGET)
	{
		ReleaseResource();
	}
	
}
void Edit::EditCropToolWidget::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.UnRef();
	OverDraw();
}
#endif
void Edit::EditCropToolWidget::OnScrollWinEvent(wxScrollWinEvent& event)
{
	event.Skip();
	OverDraw();
}

void Edit::EditCropToolWidget::OnSize(wxSizeEvent& event)
{
#ifdef __WXMSW__
	if (m_renderTarget)
	{
		float dpi = GetDpiForWindow(this->GetHWND());
		float a = dpi / 96;
		auto d2SizeU = ToSizeU(GetClientSize());
		m_renderTarget->Resize(d2SizeU);
	}
#endif
	OverDraw();
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
	SetFocus();
	if (m_prevMousePosition)
	{
		wxPoint delta = event.GetPosition() - *m_prevMousePosition;
		//float dx = delta.x / 5.f;
		//float dy = delta.y / 5.f;
		delta = delta / 2;
		AdjustScrollbars();
		m_prevMousePosition = event.GetPosition();
		auto s = m_centerPoint - delta;
		
		Scroll(s);
		OverDraw();

	}
}
void Edit::EditCropToolWidget::OnMouseWheel(wxMouseEvent& event)
{
	if (event.GetWheelRotation() == 0)
		return;
	auto sign = event.GetWheelRotation() / (abs(event.GetWheelRotation()));
	if (event.ControlDown())
	{
		auto style = GetWindowStyle();
		
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
		

	}
	else 
	{
		auto s = this->GetViewStart();
		auto t = m_workArea.GetSize();
		if (event.ShiftDown())
		{
			s.x -= sign*(m_scale * t.x) / 20;
		}
		else
		{
			s.y -= sign * (m_scale * t.y) / 20;
			
		}
		Scroll(s.x, s.y);
	}
	OverDraw();
}
void Edit::EditCropToolWidget::OnKeyDown(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_SPACE && !m_prevMousePosition)
	{
		m_prevMousePosition = event.GetPosition();
		SetCursor(wxCursor(wxStockCursor::wxCURSOR_SIZING));
	}
	else
	{
	}
}
void Edit::EditCropToolWidget::OnKeyUp(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_SPACE)
	{
		m_prevMousePosition.reset();
		SetCursor(wxCursor(wxStockCursor::wxCURSOR_ARROW));
	}
	else
	{
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
		EVT_KEY_DOWN(EditCropToolWidget::OnKeyDown)
		EVT_KEY_UP(EditCropToolWidget::OnKeyUp)
		EVT_PAINT(EditCropToolWidget::OnPaint)
		EVT_SIZE(EditCropToolWidget::OnSize)
		EVT_SCROLLWIN(EditCropToolWidget::OnScrollWinEvent)
	wxEND_EVENT_TABLE()
}

