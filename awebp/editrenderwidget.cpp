#include "wx/wxprec.h"
#include "editrenderwidget.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/rawbmp.h>
#include <math.h>
#include "util.h"
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
	return D2D1::SizeU(obj.GetWidth(), obj.GetHeight());
}
using D2SizeU = decltype(D2D1::SizeU());

bool operator !=(const D2SizeU& obj, const D2SizeU& obj2)
{
	return !(obj == obj2);
}
RECT ToRect(const wxRect& obj)
{
	return RECT{ obj.x, obj.y,obj.width - obj.x, obj.height- obj.y };
}
#endif
void Edit::EditRenderWidget::InitDirect2D()
{
#ifdef __WXMSW__
	if (m_factory == nullptr)
	{
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
		InitResource();
	}
#endif
}

void Edit::EditRenderWidget::InitResource()
{
#ifdef __WXMSW__
	float dpi = GetDpiForWindow(this->GetHWND());
	float a = dpi / 96;
	auto d2SizeU = ToSizeU(GetClientSize());
	if (!m_factory)
	{
		InitDirect2D();
	}
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
	
	if (m_selectedImage.IsOk())
	{
		int strip = m_selectedImage.GetWidth() * 4;
		size_t wxh = m_selectedImage.GetHeight() * m_selectedImage.GetWidth();
		auto imgData = m_selectedImage.GetData();
		std::vector<uint32_t> data;
		data.assign(wxh, 0);
		D2D1_RECT_U d2Size = D2D1::RectU(0, 0, m_selectedImage.GetWidth(), m_selectedImage.GetHeight());
		for (size_t i = 0; i < wxh; i++)
		{
			uint32_t pixel =
				uint32_t(imgData[i * 3 + 2]) << 16 |
				uint32_t(imgData[i * 3 + 1]) << 8 |
				uint32_t(imgData[i * 3 + 0]) << 0;

			data[i] = pixel;
		}
		
		
		if (m_bitmap)
		{
			if (ToSizeU(m_presenter->GetImageSize()) != m_bitmap->GetPixelSize())
			{
				m_bitmap->Release();
				D2D_SIZE_U d2Size = ToSizeU(m_selectedImage.GetSize());
				auto d2BitmapProperty = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
				m_renderTarget->CreateBitmap(d2Size, data.data(), strip, d2BitmapProperty, &m_bitmap);
			}
			else
			{
				m_bitmap->CopyFromMemory(&d2Size, data.data(), strip);
			}
		}
		else
		{
			D2D_SIZE_U d2Size = ToSizeU(m_presenter->GetImageSize());
			auto d2BitmapProperty = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
			m_renderTarget->CreateBitmap(d2Size, data.data(), strip, d2BitmapProperty, &m_bitmap);
		}
		SafeRelease(m_cropAreaRegion);
		auto imgSize = m_selectedImage.GetSize();
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(0, 0),
			D2D1::Point2F(0, imgSize.y),
			D2D1::Point2F(imgSize.x, imgSize.y),
			D2D1::Point2F(imgSize.x, 0),
			D2D1::Point2F(0, 0)
		};
		auto cropArea = m_presenter->GetCropRect();
		D2D1_POINT_2F points2[] = {
			D2D1::Point2F(cropArea.x, cropArea.y),
			D2D1::Point2F(cropArea.x + cropArea.width, cropArea.y),
			D2D1::Point2F(cropArea.x + cropArea.width, cropArea.y + cropArea.height),
			D2D1::Point2F(cropArea.x, cropArea.y + cropArea.height),
			D2D1::Point2F(cropArea.x, cropArea.y)
		};
		ID2D1GeometrySink* pSink = NULL;
		m_factory->CreatePathGeometry(&m_cropAreaRegion);
		m_cropAreaRegion->Open(&pSink);
		pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
		pSink->BeginFigure(D2D1::Point2F(cropArea.x, cropArea.y), D2D1_FIGURE_BEGIN_FILLED);
		pSink->AddLines(points2, ARRAYSIZE(points2));
		pSink->AddLines(points, ARRAYSIZE(points));
		pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		pSink->Close();
		SafeRelease(pSink);
		SafeRelease(m_cropAreaDotLines);
		m_factory->CreatePathGeometry(&m_cropAreaDotLines);
		m_cropAreaDotLines->Open(&pSink);
		pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
		pSink->BeginFigure(D2D1::Point2F(cropArea.x, cropArea.y), D2D1_FIGURE_BEGIN_FILLED);
		pSink->AddLines(points2, ARRAYSIZE(points2));
		pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		pSink->Close();
		SafeRelease(pSink);
	}
#endif
}
void Edit::EditRenderWidget::ReleaseResource()
{
#ifdef __WXMSW__
	SafeRelease(m_cropAreaDotLines);
	SafeRelease(m_cropAreaRegion);
	SafeRelease(m_renderTarget);
	SafeRelease(m_bitmap);
#endif
}
void Edit::EditRenderWidget::ReleaseDirect2D()
{
#ifdef __WXMSW__
	ReleaseResource();
	SafeRelease(m_factory);
#endif
}
void Edit::EditRenderWidget::ScrollOnWhell(const wxPoint& pt)
{
}
void Edit::EditRenderWidget::ScrollWindow(int dx, int dy, const wxRect* rect)
{
	m_viewStart.x += dx;
	m_viewStart.y += dy;
	Paint();
	
}
Edit::EditRenderWidget::EditRenderWidget():
	m_presenter(nullptr)
{

}
void Edit::EditRenderWidget::Init()
{
	InitDirect2D();
	
	SetFocus();
	if (!m_presenter)
	{
		return;
	}
	if (m_timer == nullptr)
	{
		m_timer = new wxTimer(this);
	}
	m_isPlaying = false;
	m_scale = 1.f;
	auto imgSize = m_presenter->GetImageSize();
	SetVirtualSize(imgSize);
	AdjustScrollbars();

	if (m_selectedImage.IsOk())
	{
		SafeRelease(m_cropAreaRegion);
		m_factory->CreatePathGeometry(&m_cropAreaRegion);
		ID2D1GeometrySink* pSink = NULL;
		m_cropAreaRegion->Open(&pSink);
		pSink->SetFillMode(D2D1_FILL_MODE_WINDING);
		D2D1_POINT_2F points[] = {
			D2D1::Point2F(0, 0),
			D2D1::Point2F(0, imgSize.y),
			D2D1::Point2F(imgSize.x, imgSize.y),
			D2D1::Point2F(imgSize.x, 0),
			D2D1::Point2F(0, 0)
		};
		auto cropArea = m_presenter->GetCropRect();
		D2D1_POINT_2F points2[] = {
			D2D1::Point2F(cropArea.x, cropArea.y),
			D2D1::Point2F(cropArea.x + cropArea.width, cropArea.y),
			D2D1::Point2F(cropArea.x + cropArea.width, cropArea.y + cropArea.height),
			D2D1::Point2F(cropArea.x, cropArea.y + cropArea.height),
			D2D1::Point2F(cropArea.x, cropArea.y)
		};
		pSink->BeginFigure(D2D1::Point2F(cropArea.x, cropArea.y), D2D1_FIGURE_BEGIN_FILLED);
		pSink->AddLines(points2, ARRAYSIZE(points2));
		pSink->AddLines(points, ARRAYSIZE(points));
		pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		pSink->Close();
		SafeRelease(pSink);
		auto clientSize = GetClientSize();
		auto imageSize = m_selectedImage.GetSize();
		if (imageSize.x * m_scale <= clientSize.x)
		{
			m_viewStart.x = (clientSize.x  - imageSize.x * m_scale) / 2;
		}
		if (imageSize.y * m_scale <= clientSize.y)
		{
			m_viewStart.y = (clientSize.y - imageSize.y * m_scale) / 2;
		}

		SetScrollbar(wxVERTICAL, 0, clientSize.y , imageSize.y * m_scale);
		SetScrollbar(wxHORIZONTAL, 0, clientSize.x, imageSize.x * m_scale);

	}
	
//
	ReloadPaintResource();
	Paint();
}
void Edit::EditRenderWidget::ReloadPaintResource()
{
	if (!m_presenter)
		return;
	InitResource();
}

Edit::EditRenderWidget::~EditRenderWidget()
{
	m_timer->Stop();
	delete m_timer;
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
void Edit::EditRenderWidget::Paint()
{
	//this->Refresh(true, &GetClientRect());
	//this->
	auto now = wxGetLocalTimeMillis().GetValue();
	if (now - m_lastRenderTime < 1)
	{
		return;
	}
	m_lastRenderTime = now;
	if (!m_renderTarget)
	{
		InitResource();
	}
	if (!m_selectedImage.IsOk())
	{
		return;
	}
	float dpi = GetDpiForWindow(this->GetHWND());
	float a = dpi / 96;
	wxSize rc = this->GetClientSize();
	wxSize imageSize = m_selectedImage.GetSize();
	
	if (m_bitmap == nullptr)
	{
		return;
	}
	
	m_renderTarget->BeginDraw();
	m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	auto translation = D2D1::Matrix3x2F::Translation(m_viewStart.x , m_viewStart.y );
	auto scale = D2D1::Matrix3x2F::Scale(m_scale , m_scale );
	m_renderTarget->SetTransform(scale * translation);
	m_renderTarget->DrawBitmap(m_bitmap, D2D1::RectF(0, 0, m_bitmap->GetSize().width, m_bitmap->GetSize().height));
	if (m_cropAreaRegion)
	{
		float dashes[] = { 2.f,2.f };
		ID2D1StrokeStyle* strokeStyle;
		m_factory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_SQUARE,
				D2D1_LINE_JOIN_MITER,
				10.0f,
				D2D1_DASH_STYLE_CUSTOM,
				0.5f),
			dashes,
			ARRAYSIZE(dashes),
			&strokeStyle
		);
		ID2D1SolidColorBrush* solidBrush = nullptr;
		m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(0x000000), &solidBrush);
		
		solidBrush->SetOpacity(0.5f);
		m_renderTarget->FillGeometry(m_cropAreaRegion, solidBrush);
		solidBrush->SetOpacity(1);
		solidBrush->SetColor(D2D1::ColorF(1, 1, 1));
		m_renderTarget->DrawGeometry(m_cropAreaDotLines, solidBrush, 1, strokeStyle);
		SafeRelease(strokeStyle);
		SafeRelease(solidBrush);
	}
	

	auto res = m_renderTarget->EndDraw();
	if (res == D2DERR_RECREATE_TARGET)
	{
		ReleaseResource();
	}
	
}
void Edit::EditRenderWidget::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.UnRef();
	Paint();
}
#endif
void Edit::EditRenderWidget::OnScrollThubTrack(wxScrollWinEvent& event)
{
	int range = this->GetScrollRange(event.GetOrientation());
	int pos = event.GetPosition();//event.GetPosition();
	int thumbs = this->GetScrollThumb(event.GetOrientation());
	wxSize imgSize = m_presenter->GetImageSize();
	float imgWidth = imgSize.x * m_scale;
	float imgHeight = imgSize.y * m_scale;
	auto viewStart = GetViewStart();
	int* factorViewStart = nullptr;
	int factorScreen = 0;
	float factorImage = 0;
	switch (event.GetOrientation())
	{
	case wxVERTICAL:
		factorScreen = GetClientSize().y;
		factorViewStart = &viewStart.y;
		factorImage = imgHeight;
		break;
	case wxHORIZONTAL:
		factorScreen = GetClientSize().x;
		factorViewStart = &viewStart.x;
		factorImage = imgWidth;
		break;
	}
	//페이지의 크기가 윈도우 축 크기보다 작다면
	if (thumbs < factorScreen)
	{
		//좌상단이다, 따라서 뷰 시작 좌표를 더한다.
		//*factorViewStart = *factorViewStart - pos;
	}

	if (-pos != *factorViewStart)
	{
		*factorViewStart = -pos;
		Scroll(viewStart);
	}
	else
	{

	}
	
	//스크롤 범위가 이미지 보다 크다면
	if(range > factorImage)
	{
		//우하단이다, 따라서... 할 것은 없다.
	}
	Paint();
}

void Edit::EditRenderWidget::OnSize(wxSizeEvent& event)
{
#ifdef __WXMSW__
	if (m_renderTarget)
	{
		float dpi = GetDpiForWindow(this->GetHWND());
		float a = dpi / 96;
		auto d2SizeU = ToSizeU(GetClientSize());
		m_renderTarget->Resize(d2SizeU);
		ReloadPaintResource();
	}
#endif
	
	Paint();
	event.Skip();
}
void Edit::EditRenderWidget::OnMouseLeftDown(wxMouseEvent& event)
{
	//마우스로 아무 작업도 안할 때
	if (!m_prevMousePosition && m_selectedImage.IsOk())
	{
		wxPoint mPosition = event.GetPosition();
		wxRect rc = m_presenter->GetCropRect();
		rc.x *= m_scale;
		rc.y *= m_scale;
		rc.width *= m_scale;
		rc.height *= m_scale;
		wxPoint s = m_viewStart;
		wxSize imgSize = m_selectedImage.GetSize();
		rc.x += s.x;
		rc.y += s.y;
		rc.Deflate(-16);
		auto res = GetDirection(rc, mPosition);

		if (res != DirectionState::None )
		{
			rc.Deflate(32);
			if (!rc.Contains(mPosition))
			{
				//마우스로 크롭 영역을 조작한다
				m_direction = res;
				m_prevMousePosition = event.GetPosition();
			}
		}
		else
		{
			res = DirectionState::None;
		}
		SetCursor(GetSizingCursor(m_direction));
	}
}
void Edit::EditRenderWidget::OnMouseLeftUp(wxMouseEvent& event)
{
	if (m_direction != DirectionState::None)
	{
		wxPoint delta = event.GetPosition() - *m_prevMousePosition;
		UpdateCropRect(delta);
		m_direction = DirectionState::None;
		if (!wxGetKeyState(WXK_SPACE))
		{
			SetCursor(wxStockCursor::wxCURSOR_ARROW);
			m_prevMousePosition.reset();
		}
		//화면 갱신
		ReloadPaintResource();
		Paint();

	}
}
void Edit::EditRenderWidget::OnMouseMotion(wxMouseEvent& event)
{
	SetFocus();
	if (!m_prevMousePosition)
	{
		if (!m_selectedImage.IsOk())
		{
			return;
		}
		wxPoint mPosition = event.GetPosition();
		wxRect rc = m_presenter->GetCropRect();
		rc.x *= m_scale;
		rc.y *= m_scale;
		rc.width *= m_scale;
		rc.height *= m_scale;
		wxPoint s = m_viewStart;
		wxSize imgSize = m_selectedImage.GetSize();
		rc.x += s.x;
		rc.y += s.y;
		rc.Deflate(-16);
		auto res = GetDirection(rc, mPosition);
		rc.Deflate(32);
		if (rc.Contains(mPosition))
		{
			//마우스로 크롭 영역을 조작한다
			res = DirectionState::None;
		}
		SetCursor(GetSizingCursor(res));
		return;
	}
	
	
	auto keyState = wxGetKeyState(WXK_SPACE);
	//스페이스바가 눌려져 있고 크롭영역 조작을 안하면 이미지 이동이다
	if (keyState && m_direction == DirectionState::None)
	{
		wxPoint delta = event.GetPosition() + GetViewStart() - *m_prevMousePosition;
		MoveView(delta);
		m_prevMousePosition = event.GetPosition() + GetViewStart();
	}
	else if(m_selectedImage.IsOk() && m_direction != DirectionState::None)
	{
		wxPoint delta = event.GetPosition()  - *m_prevMousePosition;
		UpdateCropRect(delta);
		//화면 갱신
		ReloadPaintResource();
		Paint();
		m_prevMousePosition = event.GetPosition();
	}
	else
	{
		m_prevMousePosition.reset();
	}
}
void Edit::EditRenderWidget::OnMouseWheel(wxMouseEvent& event)
{
	if (event.GetWheelRotation() == 0)
		return;
	if (!m_selectedImage.IsOk())
		return;
	auto sign = event.GetWheelRotation() / (abs(event.GetWheelRotation()));
	if (event.ControlDown())
	{
		auto style = GetWindowStyle();
		//이동전 이미지 중심의 XY
		wxPoint ptPrevCenter = GetViewStart();
		auto g = m_selectedImage.GetSize() * m_scale;
		ptPrevCenter += g / 2;

		auto prevScale = m_scale;
		m_scale += 0.01f * sign* (event.GetWheelDelta() / 120) * 10;
		if (m_scale <= 0)
		{
			m_scale = 0.1f;
		}
		AdjustScrollbars();
		wxPoint ptNowCenter = GetViewStart();
		 g = m_selectedImage.GetSize() * m_scale;
		ptNowCenter += g / 2;
		wxPoint delta = ptPrevCenter - ptNowCenter;
		ptPrevCenter -= g / 2;
		Scroll(ptPrevCenter);
	}
	else if(m_selectedImage.IsOk())
	{
		auto s = this->GetViewStart();
		auto t = m_selectedImage.GetSize();
		if (event.ShiftDown())
		{
			s.x += sign*(m_scale * t.x) / 20;
		}
		else
		{
			s.y += sign * (m_scale * t.y) / 20;
			
		}
		Scroll(s.x, s.y);
	}
	Paint();
}

void Edit::EditRenderWidget::OnKeyDown(wxKeyEvent& event)
{
	auto mouseState = wxGetMouseState();
	if (
		mouseState.LeftIsDown() || //마우스 버튼이 눌려져 있거나
		event.GetKeyCode() != WXK_SPACE ||//스페이스바를 안 누르고 있거나
		m_direction != DirectionState::None//크롭영역을 자르고 있으면
		)
	{
		return;//뷰영역 이동이 아니다.
	}
	m_prevMousePosition = event.GetPosition() + GetViewStart();
	SetCursor(wxCursor(wxStockCursor::wxCURSOR_SIZING));
}
void Edit::EditRenderWidget::OnKeyUp(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_SPACE && m_direction == DirectionState::None)
	{
		wxPoint delta = event.GetPosition() + GetViewStart() - *m_prevMousePosition;
		
		MoveView(delta);
		Paint();
		m_prevMousePosition.reset();
		SetCursor(wxCursor(wxStockCursor::wxCURSOR_ARROW));
	}
}
void Edit::EditRenderWidget::OnMouseCaptureLostEvent(wxMouseCaptureLostEvent& event)
{

}
void Edit::EditRenderWidget::RefreshView()
{
	auto it = m_presenter->GetSelectFrameIndex();
	if (m_isPlaying)
	{
		m_timer->Stop();
		m_isPlaying = false;
	}
	m_index = it;
	wxImage img;
	m_presenter->GetImage(*m_index, img, m_duration);
	bool needInit = false;
	if (!m_selectedImage.IsOk())
	{
		needInit = true;
	}
	else
	{
		needInit = m_selectedImage.GetSize() != img.GetSize();
	}
	m_selectedImage = img;
	if (needInit)
	{
		m_scale = 1.0;
		Init();
	}
	ReloadPaintResource();
	
	Paint();
}
void Edit::EditRenderWidget::MoveView(wxPoint delta)
{
	if (delta.x || delta.y)
	{
		auto s = GetViewStart() + delta;
		
		auto b = GetBestSize();
		auto c = GetClientSize();
		int unitX;
		int unitY;
		//this->GetScro;
		Scroll(s);
		Paint();
  		//this->GetScrollHelper()->SetScrollbars(unitX, unitY, b.x, b.y, s.x, s.y,true);
		//this->GetScrollHelper()->AdjustScrollbars();
	}
}
void Edit::EditRenderWidget::UpdateCropRect(wxPoint delta)
{
	if (m_selectedImage.IsOk())
	{
		delta.x /= m_scale;
		delta.y /= m_scale;
		auto temp = m_presenter->GetCropRect();
		switch (m_direction)
		{
		case DirectionState::North:
			temp.y += delta.y;
			delta.y *= -1;
		case DirectionState::South:
			temp.height += delta.y;
			break;
		case DirectionState::West:
			temp.x += delta.x;
			delta.x *= -1;
		case DirectionState::East:
			temp.width += delta.x;
			break;
		default:
			m_direction = DirectionState::None;
			m_prevMousePosition.reset();
		}
		m_presenter->SetCropRect(temp);
	}
}
void Edit::EditRenderWidget::SetPresenter(EditFramePresenter* presenter)
{
	auto old = m_presenter;
	m_presenter = presenter;
	if (old != nullptr)
	{
		
	}
	Init();
}
void Edit::EditRenderWidget::OnTimer(wxTimerEvent& event)
{
	ProcessAnim();
}

void Edit::EditRenderWidget::OnIdle(wxIdleEvent& event)
{
	if (m_isPlaying)
	{
		ProcessAnim();
	}
}
void Edit::EditRenderWidget::ProcessAnim()
{
	auto now = wxGetLocalTimeMillis().GetValue();
	auto cap = now - m_lastTick;
	if (cap >= m_duration)
	{

		if (!m_index.has_value())
			m_index = 0;
		m_lastTick = now;
		std::optional<uint32_t> duration = m_duration;
		while (cap >= duration)
		{
			cap -= *duration;
			m_index = *m_index + 1;
			duration = m_presenter->GetFrameDuration(*m_index);
			if (!duration.has_value())
			{
				m_index = 0;
				duration = m_presenter->GetFrameDuration(*m_index);
				continue;
			}
		}
		uint32_t due;
		wxImage img;
		m_presenter->GetImage(*m_index, img, due);
		wxCommandEvent* e = new wxCommandEvent(EVT_ANIM_PROCESS_A_FRAME);
		e->SetInt(*m_index);
		this->QueueEvent(e);
		if (img.IsOk())
		{
			m_selectedImage = img;
		}
		m_duration = due - cap;
		ReloadPaintResource();
		Paint();
	}
}
void Edit::EditRenderWidget::PlayAnimImage()
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
		m_selectedImage = img;
	}
	m_duration = duration;
	m_lastTick = wxGetLocalTimeMillis().GetValue();
	ReloadPaintResource();
	Paint();
}

namespace Edit
{
	wxIMPLEMENT_DYNAMIC_CLASS(EditRenderWidget, wxControl);
	wxBEGIN_EVENT_TABLE(EditRenderWidget, wxControl)
		EVT_LEFT_DOWN(EditRenderWidget::OnMouseLeftDown)
		EVT_LEFT_UP(EditRenderWidget::OnMouseLeftUp)
		EVT_MOTION(EditRenderWidget::OnMouseMotion)
		EVT_MOUSEWHEEL(EditRenderWidget::OnMouseWheel)
		EVT_KEY_DOWN(EditRenderWidget::OnKeyDown)
		EVT_KEY_UP(EditRenderWidget::OnKeyUp)
		EVT_PAINT(EditRenderWidget::OnPaint)
		EVT_SIZE(EditRenderWidget::OnSize)
		EVT_TIMER(wxID_ANY, EditRenderWidget::OnTimer)
		EVT_IDLE(EditRenderWidget::OnIdle)
		EVT_SCROLLWIN_THUMBTRACK(EditRenderWidget::OnScrollThubTrack)
	wxEND_EVENT_TABLE()
}

