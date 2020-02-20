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
	m_sizingState(SizingState::SSNone)
{
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	auto sizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* panel = new TopPannel(this);
	
	panel->SetMaxClientSize(wxSize(99999, 36));
	panel->SetWindowStyle(panel->GetWindowStyle() | wxSIMPLE_BORDER);
	sizer->Add(panel, 0, wxALL  , 5);
	sizer->Add(0,0, 1, wxEXPAND, 5);

	panel = new BottomPanel(this);
	panel->SetMaxClientSize(wxSize(99999, 36));
	sizer->Add(panel, 0, wxALL | wxEXPAND, 5);

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
	region.Subtract(wxRect(size).Deflate(5));
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
	if (m_sizingState == SizingState::SSNone && mousePosition.y < TOP_PANEL_HEIGHT)
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
	else if(!m_presenter.IsRecording())
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