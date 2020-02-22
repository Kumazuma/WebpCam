#include "wx/wxprec.h"
#include "captureframe.h"
#include <wx/region.h>
#include <wx/spinctrl.h>
#include "UIDesign.h"
#include "editframe.h"
#include "event.h"
#include "util.h"

CaptureFrame::CaptureFrame() :
	wxFrame(nullptr, wxID_ANY, wxT("캡처"), wxDefaultPosition, wxDefaultSize,wxSYSTEM_MENU| wxSTAY_ON_TOP |wxFRAME_SHAPED),
	m_presenter(this),
	m_sizingState(DirectionState::None)
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
	res.y -= TOP_PANEL_HEIGHT + BORDER_THICKNESS + BORDER_THICKNESS / 2;
	res.height += BORDER_THICKNESS * 2 + TOP_PANEL_HEIGHT + BOTTOM_PANEL_HEIGHT + BORDER_THICKNESS / 2;
	return res;
}

wxRect CaptureFrame::CvtWindowRectToCaptureRegion(const wxRect& rc)
{
	wxRect res = rc;
	res.x += BORDER_THICKNESS;
	res.width -= BORDER_THICKNESS * 2;
	res.y += TOP_PANEL_HEIGHT + BORDER_THICKNESS + BORDER_THICKNESS/2;
	res.height -= BORDER_THICKNESS * 2 + TOP_PANEL_HEIGHT + BOTTOM_PANEL_HEIGHT + BORDER_THICKNESS / 2;
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
void CaptureFrame::CalcSizingDirection(const wxPoint& mousePosition)
{
	wxSize size = this->GetClientSize();
	auto direction = GetDirection(size, mousePosition);
	wxRegion region{ wxRect(size) };
	region.Subtract(wxRect(size).Deflate(BORDER_THICKNESS));
	if (region.Contains(mousePosition) == wxRegionContain::wxInRegion)
	{
		direction |= wxRect(wxPoint(0, 0), wxPoint(BORDER_THICKNESS, BORDER_THICKNESS)).Contains(mousePosition)? DirectionState::NorthWest : direction;
		direction |= wxRect(wxPoint(size.x - BORDER_THICKNESS, size.y - BORDER_THICKNESS), wxPoint(size.x, size.y)).Contains(mousePosition) ? DirectionState::SouthEast : direction;
		direction |= wxRect(wxPoint(size.x - BORDER_THICKNESS, 0), wxPoint(size.x, BORDER_THICKNESS)).Contains(mousePosition) ? DirectionState::NorthEast : direction;
		direction |= wxRect(wxPoint(0, size.y - BORDER_THICKNESS), wxPoint(BORDER_THICKNESS, size.y)).Contains(mousePosition) ? DirectionState::SouthWest : direction;
	}
	else
	{
		direction = DirectionState::None;
	}
	if (direction == DirectionState::None && mousePosition.y < TOP_PANEL_HEIGHT + BORDER_THICKNESS)
	{
		direction = DirectionState::All;
	}

	m_sizingState = direction;
}
void CaptureFrame::OnMouseLeftDown(wxMouseEvent& event)
{
	CalcSizingDirection(event.GetPosition());
	SetCursor(GetSizingCursor(m_sizingState));
	if (m_sizingState != DirectionState::None)
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
		SetCursor(GetSizingCursor(m_sizingState));
	}
	else if(!m_presenter.IsRecording())
	{
		
		auto nowPosition = this->ClientToScreen(event.GetPosition());
		auto deltaPos = *m_prevPosition - nowPosition;
		m_prevPosition = nowPosition;
		auto rect = this->GetRect();
		if (m_sizingState && DirectionState::West)
		{
			rect.x -= deltaPos.x;
			rect.width += deltaPos.x;
		}
		if (m_sizingState && DirectionState::East)
		{
			rect.width -= deltaPos.x;
		}
		if (m_sizingState && DirectionState::North)
		{
			rect.y -= deltaPos.y;
			rect.height += deltaPos.y;
		}
		if (m_sizingState && DirectionState::South)
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