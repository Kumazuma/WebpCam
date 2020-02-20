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
	CaptureFrame* frame = new CaptureFrame();
	this->SetTopWindow(frame);
	return true;
}

int AWebpApp::OnExit()
{
	return wxApp::OnExit();
}

