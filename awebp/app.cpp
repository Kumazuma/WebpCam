#include "wx/wxprec.h"
#include "app.h"
#include "frame.h"
wxIMPLEMENT_APP(AWebpApp);

bool AWebpApp::OnInit()
{
	wxInitAllImageHandlers();
	m_representer = new AppPresenter();
	ui_mainFrame = new CommandFrame(wxT("TEST2"));
	ui_mainFrame->Show();
	this->SetTopWindow(ui_mainFrame);
	

	this->Bind(EVT_RefreshView, &AWebpApp::OnRefleshView,this);

	return true;
}

int AWebpApp::OnExit()
{
	delete m_representer;
	return wxApp::OnExit();
}

void AWebpApp::OnRefleshView(wxCommandEvent& event)
{
	if (m_representer->IsRecording())
	{
		ui_mainFrame->ui_btnStart->Disable();
		ui_mainFrame->ui_btnStop->Enable();
		ui_mainFrame->ui_regionFrame->Disable();
	}
	else
	{
		ui_mainFrame->ui_btnStart->Enable();
		ui_mainFrame->ui_btnStop->Disable();
		ui_mainFrame->ui_regionFrame->Enable();
	}
	ui_mainFrame->ui_spinHeight->SetValue(m_representer->GetRecordedRect().GetHeight());
	ui_mainFrame->ui_spinWidth->SetValue(m_representer->GetRecordedRect().GetWidth());
}
