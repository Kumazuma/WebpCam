#include "wx/wxprec.h"
#include "app.h"
#include "captureframe.h"

wxIMPLEMENT_APP(AWebpApp);

bool AWebpApp::OnInit()
{
#ifdef __WXMSW__
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
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

