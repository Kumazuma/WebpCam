#include "wx/wxprec.h"
#include "frame.h"
#include "app.h"
#include "encoderview.h"
const auto CAPTURE_REGION_FRAME_STYPE =
wxCAPTION | wxRESIZE_BORDER | wxSTAY_ON_TOP |
wxFRAME_SHAPED | wxFRAME_TOOL_WINDOW;

wxBEGIN_EVENT_TABLE(CaptureRegionFrame, wxFrame)
EVT_SIZE(CaptureRegionFrame::OnSized)
EVT_MOVE(CaptureRegionFrame::OnMoved)
wxEND_EVENT_TABLE();

int TOP_HEIGHT = 0;
int BOTTOM_HEIGHT = 0;
int BORDER_THICKNESS = 0;

CaptureRegionFrame::CaptureRegionFrame(wxWindow* parent, AppPresenter* presenter, const wxString& title):
	wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, CAPTURE_REGION_FRAME_STYPE)
{
	m_presenter = presenter;
	CenterOnScreen();
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	wxRect windowRect = GetSize();
	wxRegion region(windowRect);
	this->SetShape(region);
	auto rect = this->GetRect();
	auto clientRect = this->GetClientRect();
	TOP_HEIGHT = wxSystemSettings::GetMetric(wxSYS_CAPTION_Y, this);
	BOTTOM_HEIGHT = (rect.height - clientRect.height - TOP_HEIGHT) / 2;
	BORDER_THICKNESS = (rect.width - clientRect.width) / 2;
	
	windowRect.x += BORDER_THICKNESS;
	windowRect.width -= BORDER_THICKNESS * 2;
	windowRect.y += TOP_HEIGHT + BOTTOM_HEIGHT;
	windowRect.height -= BOTTOM_HEIGHT * 2 + TOP_HEIGHT;
	//windowRect.Deflate(16, 16);
	region.Xor(windowRect);
	this->SetShape(region);
	rect.x += BORDER_THICKNESS;
	rect.width -= BORDER_THICKNESS * 2;
	rect.y += TOP_HEIGHT + BOTTOM_HEIGHT;
	rect.height -= BOTTOM_HEIGHT * 2 + TOP_HEIGHT;
	
	
	m_presenter->SetRecordedRect(rect);
	this->SetShape(region);
}

CaptureRegionFrame::~CaptureRegionFrame()
{
	
}

void CaptureRegionFrame::OnMoved(wxMoveEvent& event)
{
	auto rect = this->GetRect();
	rect.x += BORDER_THICKNESS;
	rect.width -= BORDER_THICKNESS * 2;
	rect.y += TOP_HEIGHT + BOTTOM_HEIGHT;
	rect.height -= BOTTOM_HEIGHT * 2 + TOP_HEIGHT;
	m_presenter->SetRecordedRect(rect);
}

void CaptureRegionFrame::OnSized(wxSizeEvent& event)
{
	wxRect windowRect = GetSize();
	wxRegion region(windowRect);
	windowRect.x += BORDER_THICKNESS;
	windowRect.width -= BORDER_THICKNESS * 2;
	windowRect.y += TOP_HEIGHT + BOTTOM_HEIGHT;
	windowRect.height -= BOTTOM_HEIGHT * 2 + TOP_HEIGHT;
	//windowRect.Deflate(16, 16);
	region.Xor(windowRect);
	this->SetShape(region);
	auto rect = this->GetRect();
	rect.x += BORDER_THICKNESS;
	rect.width -= BORDER_THICKNESS * 2;
	rect.y += TOP_HEIGHT + BOTTOM_HEIGHT;
	rect.height -= BOTTOM_HEIGHT * 2 + TOP_HEIGHT;
	m_presenter->SetRecordedRect(rect);
}

CommandFrame::CommandFrame(const wxString& title):
	UIRecordFrame(nullptr, wxID_ANY, title)
{
	m_presenter = new AppPresenter(this);
	ui_regionFrame = new CaptureRegionFrame(this, m_presenter, wxT("캡처영역"));
	ui_regionFrame->Show();
	ui_btnStart->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CommandFrame::OnClickBtnStart, this);
	ui_btnStop->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CommandFrame::OnClickBtnStop, this);
	ui_spinHeight->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED, &CommandFrame::OnSpinHeigt, this);
	ui_spinWidth->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED, &CommandFrame::OnSpinWidth, this);
	ui_choiceFps->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &CommandFrame::OnChoiceFPS, this);
	ui_choiceFps->Append(wxT("10"),(void*)FPS::_10);
	ui_choiceFps->Append(wxT("12"), (void*)FPS::_12);
	ui_choiceFps->Append(wxT("15"), (void*)FPS::_15);
	ui_choiceFps->Append(wxT("20"), (void*)FPS::_20);
	ui_chkUsingTempFile->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &CommandFrame::OnChangeChkUsingTemoFile, this);
	ui_btnStop->Disable();
	this->Bind(EVT_RefreshView, &CommandFrame::OnRefleshView, this);
}
CommandFrame::~CommandFrame()
{
	ui_regionFrame->Close(true);
	delete m_presenter;
}

void CommandFrame::OnClickBtnStart(wxCommandEvent& event)
{
	m_presenter->StartRecording();
}

void CommandFrame::OnClickBtnStop(wxCommandEvent& event)
{
	m_presenter->StopRecording();
	wxFrame* tempFrame = new wxFrame(this,wxID_ANY, wxT("테스트"));
	wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
	
	wxScrolledWindow* panel = new wxScrolledWindow(tempFrame,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL|wxALWAYS_SHOW_SB );
	panel->SetScrollRate(5, 5);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	panel->SetSizer(sizer);
	panel->Layout();
	sizer->Fit(panel);
	sizer1->Add(panel, 1, wxEXPAND);
	auto btnSave =new wxButton(tempFrame, wxID_ANY, wxT("저장"));
	btnSave->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CommandFrame::OnClickSave, this);
	sizer1->Add(btnSave, 0, wxEXPAND);
	tempFrame->SetSizer(sizer1);
	tempFrame->Layout();
	tempFrame->Show();
}

void CommandFrame::OnSpinWidth(wxSpinEvent& event)
{
	auto size = ui_regionFrame->GetSize();
	size.x = event.GetValue();
	size.x += BORDER_THICKNESS * 2;
	ui_regionFrame->SetSize(size);
}

void CommandFrame::OnSpinHeigt(wxSpinEvent& event)
{
	auto size = ui_regionFrame->GetSize();
	size.y = event.GetValue();
	size.y += BOTTOM_HEIGHT * 2 + TOP_HEIGHT;
	ui_regionFrame->SetSize(size);
}

void CommandFrame::OnChoiceFPS(wxCommandEvent& event)
{
	FPS fps = (FPS)(int)event.GetClientData();
	m_presenter->SetFPS(fps);
}

void CommandFrame::OnChangeChkUsingTemoFile(wxCommandEvent& event)
{
	if (ui_chkUsingTempFile->GetValue())
	{
		m_presenter->EnableTemporalFile();
	}
	else
	{
		m_presenter->DisableTemporalFile();
	}
}

void CommandFrame::OnClickSave(wxCommandEvent& event)
{
	EncoderFrame* frame = new EncoderFrame(m_presenter->BuildImageStore());
	frame->Show();
	Close();
}
void CommandFrame::OnRefleshView(wxCommandEvent& event)
{
	if (m_presenter->IsRecording())
	{
		ui_btnStart->Disable();
		ui_btnStop->Enable();
		ui_regionFrame->Disable();
	}
	else
	{
		ui_btnStart->Enable();
		ui_btnStop->Disable();
		ui_regionFrame->Enable();
	}
	ui_spinHeight->SetValue(m_presenter->GetRecordedRect().GetHeight());
	ui_spinWidth->SetValue(m_presenter->GetRecordedRect().GetWidth());
}
