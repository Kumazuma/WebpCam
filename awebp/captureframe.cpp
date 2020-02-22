#include "wx/wxprec.h"
#include "captureframe.h"
#include <wx/region.h>
#include <wx/spinctrl.h>
#include "UIDesign.h"
#include "editframe.h"
#include "event.h"
SizingState& operator |= (SizingState& obj, SizingState state)
{
	obj = (SizingState)((uint8_t)obj | (uint8_t)state);
	return obj;
}
bool operator && (SizingState& obj, SizingState state)
{
	return ((uint8_t)obj & (uint8_t)state) != 0;
}
CaptureFrame::CaptureFrame() :
	wxFrame(nullptr, wxID_ANY, wxT("캡처"), wxDefaultPosition, wxDefaultSize,wxSYSTEM_MENU| wxSTAY_ON_TOP |wxFRAME_SHAPED),
	m_presenter(this),
	m_sizingState(SizingState::None)
{
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	auto sizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* panel = new TopPannel(this);
	panel->Bind(wxEVT_MOTION, &CaptureFrame::OnChildMouseMotion, this);
	panel->SetMaxClientSize(wxSize(99999, TOP_PANEL_HEIGHT));
	panel->SetWindowStyle(panel->GetWindowStyle() | wxSIMPLE_BORDER);
	sizer->Add(panel, 0, wxALL  , BORDER_THICKNESS);
	sizer->Add(0,0, 1, wxEXPAND, BORDER_THICKNESS);

	panel = new BottomPanel(this);
	panel->SetMaxClientSize(wxSize(99999, BOTTOM_PANEL_HEIGHT));
	panel->Bind(wxEVT_MOTION, &CaptureFrame::OnChildMouseMotion, this);
	sizer->Add(panel, 0, wxALL | wxEXPAND, BORDER_THICKNESS);
	SetSizer(sizer);
	Layout();
	Centre();
	Show();
	auto rc = GetRect(); 
	rc = CvtWindowRectToCaptureRegion(rc);
	m_presenter.SetRecordedRect(rc);
	wxRegion rg{ wxRect(GetSize()) };
	rc = wxRect(GetSize());
	rc = CvtWindowRectToCaptureRegion(rc);
	
	rg.Subtract(rc);
	SetShape(rg);

	FindWindowById(ID_BTN_START)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CaptureFrame::OnClickBtnStart, this);
	FindWindowById(ID_BTN_STOP)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CaptureFrame::OnClickBtnStop, this);
	FindWindowById(ID_BTN_CLOSE)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CaptureFrame::OnClickClose, this);
	FindWindowById(ID_SPIN_HEIGHT)->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED, &CaptureFrame::OnSpinHeigt, this);
	FindWindowById(ID_SPIN_WIDTH)->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED, &CaptureFrame::OnSpinWidth, this);
	FindWindowById(ID_SPIN_X)->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED, &CaptureFrame::OnSpinPosX, this);
	FindWindowById(ID_SPIN_Y)->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED, &CaptureFrame::OnSpinPosY, this);
	auto* choice = wxDynamicCast(FindWindowById(ID_CHOICE_FPS), wxChoice);
	choice->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &CaptureFrame::OnChoiceFPS, this);
	choice->Append(wxT("10"), (void*)FPS::_10);
	choice->Append(wxT("12"), (void*)FPS::_12);
	choice->Append(wxT("15"), (void*)FPS::_15);
	choice->Append(wxT("20"), (void*)FPS::_20);
	FindWindowById(ID_BTN_STOP)->Disable();
	this->Bind(EVT_RefreshView, &CaptureFrame::OnRefleshView, this);
}
void CaptureFrame::OnChildMouseMotion(wxMouseEvent& event)
{
	event.Skip();
	SetCursor(wxCursor(wxStockCursor::wxCURSOR_ARROW));
}
wxRect CaptureFrame::CvtCaptureRegionToWindowRect(const wxRect& rc)
{
	wxRect res = rc;
	res.x -= BORDER_THICKNESS;
	res.width += BORDER_THICKNESS * 2;
	res.y -= TOP_PANEL_HEIGHT + BORDER_THICKNESS;
	res.height += BORDER_THICKNESS * 2 + TOP_PANEL_HEIGHT + BOTTOM_PANEL_HEIGHT;
	return res;
}

wxRect CaptureFrame::CvtWindowRectToCaptureRegion(const wxRect& rc)
{
	wxRect res = rc;
	res.x += BORDER_THICKNESS;
	res.width -= BORDER_THICKNESS * 2;
	res.y += TOP_PANEL_HEIGHT + BORDER_THICKNESS;
	res.height -= BORDER_THICKNESS * 2 + TOP_PANEL_HEIGHT + BOTTOM_PANEL_HEIGHT;
	return res;
}
void CaptureFrame::SetRect(const wxRect& rc)
{
	
	this->SetPosition(rc.GetPosition());
	this->SetSize(rc.GetSize());
	wxRect rect = CvtWindowRectToCaptureRegion(rc);
	m_presenter.SetRecordedRect(rect);
}

void CaptureFrame::SetCapturedRect(wxRect rc)
{
	wxRect rect = CvtCaptureRegionToWindowRect(rc);
	SetRect(rect);
}
bool CaptureFrame::GetHitTests(const wxPoint& pt, wxWindow* window )
{
	if (window == nullptr)
	{
		return GetHitTests(pt, this);
	}
	else
	{
		auto& t = window->GetChildren();
		for (auto child : window->GetChildren())
		{
			if (GetHitTests(pt, child))
			{
				return true;
			}
			wxPoint point = window->ScreenToClient(pt);
			if (window->HitTest(point) == wxHT_WINDOW_INSIDE)
			{
				return true;
			}

		}
	}
	
	return false;
}
void CaptureFrame::CalcSizingDirection(const wxPoint& mousePosition)
{
	wxPoint screenPosition = ClientToScreen(mousePosition);

	
	
	wxSize size = this->GetClientSize();
	float degree;
	degree = float(size.y) / float(size.x);
	//기울기 계산. \방향
	float calc1 = degree * mousePosition.x;
	//기울기 계산. /방향
	float calc2 = size.y - degree * mousePosition.x;
	char state = (calc2 < mousePosition.y) << 1 | (calc1 < mousePosition.y);
	wxClientDC dc(this);
	wxRegion region{ wxRect(size) };
	region.Subtract(wxRect(size).Deflate(BORDER_THICKNESS));
	m_sizingState = SizingState::None;
	if (region.Contains(mousePosition) == wxRegionContain::wxInRegion)
	{
		m_sizingState |= wxRect(wxPoint(0, 0), wxPoint(BORDER_THICKNESS, BORDER_THICKNESS)).Contains(mousePosition)? SizingState::NorthWest : m_sizingState;
		m_sizingState |= wxRect(wxPoint(size.x - BORDER_THICKNESS, size.y - BORDER_THICKNESS), wxPoint(size.x, size.y)).Contains(mousePosition) ? SizingState::SouthEast : m_sizingState;
		m_sizingState |= wxRect(wxPoint(size.x - BORDER_THICKNESS, 0), wxPoint(size.x, BORDER_THICKNESS)).Contains(mousePosition) ? SizingState::NorthEast : m_sizingState;
		m_sizingState |= wxRect(wxPoint(0, size.y - BORDER_THICKNESS), wxPoint(BORDER_THICKNESS, size.y)).Contains(mousePosition) ? SizingState::SouthWest : m_sizingState;
		switch (state)
		{
		case 0:
			m_sizingState |= SizingState::North;
			break;
		case 1:
			m_sizingState |= SizingState::West;
			break;
		case 2:
			m_sizingState |= SizingState::East;
			break;
		case 3:
			m_sizingState |= SizingState::South;
			break;
		}
	}
	else
	{

	}
	if (m_sizingState == SizingState::None && mousePosition.y < TOP_PANEL_HEIGHT + BORDER_THICKNESS)
	{
		m_sizingState = SizingState::All;
	}
}
wxCursor CaptureFrame::GetSizingCursor()
{
	switch (m_sizingState)
	{
	case SizingState::NorthWest:
	case SizingState::SouthEast:
		return wxCursor(wxCURSOR_SIZENWSE);
	case SizingState::NorthEast:
	case SizingState::SouthWest:
		return wxCursor(wxCURSOR_SIZENESW);
	case SizingState::West:
	case SizingState::East:
		return wxCursor(wxCURSOR_SIZEWE);
	case SizingState::North:
	case SizingState::South:
		return wxCursor(wxCURSOR_SIZENS);
	case SizingState::All:
		return wxCursor(wxCURSOR_SIZING);
	default:
		return wxCursor(wxCURSOR_ARROW);
	}
}
void CaptureFrame::OnMouseLeftDown(wxMouseEvent& event)
{
	CalcSizingDirection(event.GetPosition());
	SetCursor(GetSizingCursor());
	if (m_sizingState != SizingState::None)
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
	else if(!m_presenter.IsRecording())
	{
		
		auto nowPosition = this->ClientToScreen(event.GetPosition());
		auto deltaPos = *m_prevPosition - nowPosition;
		m_prevPosition = nowPosition;
		auto rect = this->GetRect();
		if (m_sizingState && SizingState::West)
		{
			rect.x -= deltaPos.x;
			rect.width += deltaPos.x;
		}
		if (m_sizingState && SizingState::East)
		{
			rect.width -= deltaPos.x;
		}
		if (m_sizingState && SizingState::North)
		{
			rect.y -= deltaPos.y;
			rect.height += deltaPos.y;
		}
		if (m_sizingState && SizingState::South)
		{
			rect.height -= deltaPos.y;
		}
		this->SetPosition(rect.GetPosition());
		this->SetSize(rect.GetSize());
		this->Layout();
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

void CaptureFrame::OnMouseLeave(wxMouseEvent& event)
{
	SetCursor(wxStockCursor::wxCURSOR_ARROW);
}

void CaptureFrame::OnSized(wxSizeEvent& event)
{
	auto rc = GetRect();
	rc = CvtWindowRectToCaptureRegion(rc);
	m_presenter.SetRecordedRect(rc);
	wxRegion rg{ wxRect(GetSize()) };
	rc = wxRect(GetSize());
	rc = CvtWindowRectToCaptureRegion(rc);
	rg.Subtract(rc);
	SetShape(rg);
}

void CaptureFrame::OnMoved(wxMoveEvent& event)
{
	auto rc = GetRect();
	rc= CvtWindowRectToCaptureRegion(rc);
	m_presenter.SetRecordedRect(rc);
}

void CaptureFrame::OnClickBtnStart(wxCommandEvent& event)
{
	m_presenter.StartRecording();
}

void CaptureFrame::OnClickBtnStop(wxCommandEvent& event)
{
	m_presenter.StopRecording();
	EditFrame* frame = new EditFrame(m_presenter.BuildImageStore(), m_presenter.GetRecordedRect().GetSize());
	frame->Show();
	Close();
}

void CaptureFrame::OnSpinWidth(wxSpinEvent& event)
{
	auto size = m_presenter.GetRecordedRect();
	size.width = event.GetValue();
	SetCapturedRect(size);
}

void CaptureFrame::OnSpinHeigt(wxSpinEvent& event)
{
	auto size = m_presenter.GetRecordedRect();
	size.height = event.GetValue();
	SetCapturedRect(size);
}
void CaptureFrame::OnSpinPosX(wxSpinEvent& event)
{
	auto size = m_presenter.GetRecordedRect();
	size.x = event.GetValue();
	SetCapturedRect(size);
}

void CaptureFrame::OnSpinPosY(wxSpinEvent& event)
{
	auto size = m_presenter.GetRecordedRect();
	size.y = event.GetValue();
	SetCapturedRect(size);
}
void CaptureFrame::OnChoiceFPS(wxCommandEvent& event)
{
	int data = (long long)event.GetClientData();
	if (data != 0)
	{
		FPS fps = (FPS)data;
		m_presenter.SetFPS(fps);
	}
}

void CaptureFrame::OnChangeChkUsingTemoFile(wxCommandEvent& event)
{
	//if (ui_chkUsingTempFile->GetValue())
	//{
	//	m_presenter.EnableTemporalFile();
	//}
	//else
	//{
	//	m_presenter.DisableTemporalFile();
	//}
}

void CaptureFrame::OnClickSave(wxCommandEvent& event)
{

}
void CaptureFrame::OnRefleshView(wxCommandEvent& event)
{
	if (m_presenter.IsRecording())
	{
		FindWindowById(ID_BTN_START)->Disable();
		FindWindowById(ID_BTN_STOP)->Enable();
	}
	else
	{
		FindWindowById(ID_BTN_START)->Enable();
		FindWindowById(ID_BTN_STOP)->Disable();
	}
	wxDynamicCast(FindWindowById(ID_SPIN_HEIGHT),wxSpinCtrl)->SetValue(m_presenter.GetRecordedRect().GetHeight());
	wxDynamicCast(FindWindowById(ID_SPIN_WIDTH), wxSpinCtrl)->SetValue(m_presenter.GetRecordedRect().GetWidth());
	wxDynamicCast(FindWindowById(ID_SPIN_X), wxSpinCtrl)->SetValue(m_presenter.GetRecordedRect().GetX());
	wxDynamicCast(FindWindowById(ID_SPIN_Y), wxSpinCtrl)->SetValue(m_presenter.GetRecordedRect().GetY());
}
void CaptureFrame::OnClickClose(wxCommandEvent& event)
{
	Close();
}
wxBEGIN_EVENT_TABLE(CaptureFrame, wxFrame)
EVT_LEFT_DOWN(CaptureFrame::OnMouseLeftDown)
EVT_LEFT_UP(CaptureFrame::OnMouseLeftUp)
EVT_MOTION(CaptureFrame::OnMouseMotion)
EVT_MOUSE_CAPTURE_LOST(CaptureFrame::OnMouseLost)
EVT_SIZE(CaptureFrame::OnSized)
EVT_MOVE(CaptureFrame::OnMoved)
wxEND_EVENT_TABLE()