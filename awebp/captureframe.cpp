#include "wx/wxprec.h"
#include "captureframe.h"
#include <wx/region.h>
SizingState& operator |= (SizingState& obj, SizingState state)
{
	obj = (SizingState)((uint8_t)obj | (uint8_t)state);
	return obj;
}
bool operator && (SizingState& obj, SizingState state)
{
	return ((uint8_t)obj & (uint8_t)state) != 0;
}
CaptureFrame::CaptureFrame(wxWindow* parent, AppPresenter& presenter) :
	wxFrame(parent, wxID_ANY, wxT("캡처"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP |wxFRAME_SHAPED),
	m_presenter(presenter),
	m_sizingState(SizingState::SSNone)
{
	Centre();
	Show();
	auto rc = GetRect();
	rc.x += 5;
	rc.width -= 10;
	rc.y += 36;
	rc.height -= 41;
	m_presenter.SetRecordedRect(rc);
	wxRegion rg{ wxRect(GetSize()) };
	auto r = wxRect(GetSize());
	r.x += 5;
	r.width -= 10;
	r.y += 36;
	r.height -= 41;
	rg.Subtract(r);
	SetShape(rg);
}

wxSize CaptureFrame::CvtCaptureRegionToWindowSize(const wxRect& rc)
{
	return wxSize();
}

wxRect CaptureFrame::CvtWindowSizeToCaptureRegion(const wxSize& size)
{
	return wxRect();
}
void CaptureFrame::SetRect(const wxRect& rc)
{
	
	this->SetPosition(rc.GetPosition());
	this->SetSize(rc.GetSize());
	wxRect temp = rc;
	temp.x += 5;
	temp.width -= 10;
	temp.y += 36;
	temp.height -= 41;
	m_presenter.SetRecordedRect(temp);
}
void CaptureFrame::SetCapturedRect(wxRect rc)
{
	rc.x -= 5;
	rc.width += 10;
	rc.y -= 36;
	rc.height += 41;
	SetRect(rc);
	
}
void CaptureFrame::CalcSizingDirection(const wxPoint& mousePosition)
{
	//기울기 계산. \방향
	wxSize size = this->GetClientSize();
	float degree;
	degree = float(size.y) / float(size.x);
	float calc1 = degree * mousePosition.x;
	float calc2 = size.y - degree * mousePosition.x;
	char state = (calc2 < mousePosition.y) << 1 | (calc1 < mousePosition.y);
	wxClientDC dc(this);
	wxRegion region{ wxRect(size) };
	region.Xor(wxRect(size).Deflate(5));
	m_sizingState = SizingState::SSNone;
	if (region.Contains(mousePosition) == wxRegionContain::wxInRegion)
	{
		m_sizingState |= wxRect(wxPoint(0, 0), wxPoint(5, 5)).Contains(mousePosition)? SizingState::SSNW : m_sizingState;
		m_sizingState |= wxRect(wxPoint(size.x - 5, size.y - 5), wxPoint(size.x, size.y)).Contains(mousePosition) ? SizingState::SSSE : m_sizingState;
		m_sizingState |= wxRect(wxPoint(size.x - 5, 0), wxPoint(size.x, 5)).Contains(mousePosition) ? SizingState::SSNE : m_sizingState;
		m_sizingState |= wxRect(wxPoint(0, size.y - 5), wxPoint(5, size.y)).Contains(mousePosition) ? SizingState::SSSW : m_sizingState;
		switch (state)
		{
		case 0:
			m_sizingState |= SizingState::SSN;
			break;
		case 1:
			m_sizingState |= SizingState::SSW;
			break;
		case 2:
			m_sizingState |= SizingState::SSE;
			break;
		case 3:
			m_sizingState |= SizingState::SSS;
			break;
		}
	}
	if (m_sizingState == SizingState::SSNone && mousePosition.y < 36)
	{
		m_sizingState = SizingState::SSNSEW;
	}
}
wxCursor CaptureFrame::GetSizingCursor()
{
	switch (m_sizingState)
	{
	case SizingState::SSNW:
	case SizingState::SSSE:
		return wxCursor(wxCURSOR_SIZENWSE);
	case SizingState::SSNE:
	case SizingState::SSSW:
		return wxCursor(wxCURSOR_SIZENESW);
	case SizingState::SSW:
	case SizingState::SSE:
		return wxCursor(wxCURSOR_SIZEWE);
	case SizingState::SSN:
	case SizingState::SSS:
		return wxCursor(wxCURSOR_SIZENS);
	case SizingState::SSNSEW:
		return wxCursor(wxCURSOR_SIZING);
	default:
		return wxCursor(wxCURSOR_ARROW);
	}
}
void CaptureFrame::OnMouseLeftDown(wxMouseEvent& event)
{
	CalcSizingDirection(event.GetPosition());
	SetCursor(GetSizingCursor());
	if (m_sizingState != SizingState::SSNone)
	{
		this->CaptureMouse();
		m_prevPosition = this->ClientToScreen(event.GetPosition());
	}
}
void CaptureFrame::OnMouseLeftUp(wxMouseEvent& event)
{
	SetCursor(wxStockCursor::wxCURSOR_ARROW);
	if (m_prevPosition.has_value())
	{
		this->ReleaseMouse();
		m_prevPosition.reset();
	}
	
}
void CaptureFrame::OnMouseMotion(wxMouseEvent& event)
{
	//CalcSizingRect(event.GetPosition());
	if (!m_prevPosition.has_value())
	{
		CalcSizingDirection(event.GetPosition());
		SetCursor(GetSizingCursor());
	}
	else
	{
		auto nowPosition = this->ClientToScreen(event.GetPosition());
		auto deltaPos = *m_prevPosition - nowPosition;
		m_prevPosition = nowPosition;
		auto rect = this->GetRect();
		if (m_sizingState && SizingState::SSW)
		{
			rect.x -= deltaPos.x;
			rect.width += deltaPos.x;
		}
		if (m_sizingState && SizingState::SSE)
		{
			rect.width -= deltaPos.x;
		}
		if (m_sizingState && SizingState::SSN)
		{
			rect.y -= deltaPos.y;
			rect.height += deltaPos.y;
		}
		if (m_sizingState && SizingState::SSS)
		{
			rect.height -= deltaPos.y;
		}
		this->SetPosition(rect.GetPosition());
		this->SetSize(rect.GetSize());
	}
}

void CaptureFrame::OnMouseLost(wxMouseCaptureLostEvent& event)
{
	SetCursor(wxStockCursor::wxCURSOR_ARROW);
	if (m_prevPosition.has_value())
	{
		m_prevPosition.reset();
	}
}

void CaptureFrame::OnSized(wxSizeEvent& event)
{
	auto rc = GetRect();
	rc.x += 5;
	rc.width -= 10;
	rc.y += 36;
	rc.height -= 41;
	m_presenter.SetRecordedRect(rc);
	wxRegion rg{ wxRect(GetSize()) };
	auto r = wxRect(GetSize());
	r.x += 5;
	r.width -= 10;
	r.y += 36;
	r.height -= 41;
	rg.Subtract(r);
	SetShape(rg);
}

void CaptureFrame::OnMoved(wxMoveEvent& event)
{
	auto rc = GetRect();
	rc.x += 5;
	rc.width -= 10;
	rc.y += 36;
	rc.height -= 41;
	m_presenter.SetRecordedRect(rc);
}

wxBEGIN_EVENT_TABLE(CaptureFrame, wxFrame)
EVT_LEFT_DOWN(CaptureFrame::OnMouseLeftDown)
EVT_LEFT_UP(CaptureFrame::OnMouseLeftUp)
EVT_MOTION(CaptureFrame::OnMouseMotion)
EVT_MOUSE_CAPTURE_LOST(CaptureFrame::OnMouseLost)
EVT_SIZE(CaptureFrame::OnSized)
EVT_MOVE(CaptureFrame::OnMoved)
wxEND_EVENT_TABLE()