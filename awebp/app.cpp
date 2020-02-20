#include "wx/wxprec.h"
#include "app.h"
#include "frame.h"
#include "captureframe.h"

wxIMPLEMENT_APP(AWebpApp);

bool AWebpApp::OnInit()
{
#ifdef __WXMSW__
	SetProcessDPIAware();
#endif
	wxInitAllImageHandlers();
	ui_mainFrame = new CommandFrame(wxT("TEST2"));
	ui_mainFrame->Show();
	this->SetTopWindow(ui_mainFrame);
	return true;
}

int AWebpApp::OnExit()
{
	return wxApp::OnExit();
}

