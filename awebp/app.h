#pragma once
#include<wx/wx.h>
#include"wx/wxprec.h"
#include"presenter.h"
#include "frame.h"
class AWebpApp : public wxApp
{
private:
	
	CaptureRegionFrame* m_frame;
	CommandFrame* ui_mainFrame;
public:
	AppPresenter* m_representer;
	bool OnInit() override;
	int OnExit() override;
protected:
	
	void OnRefleshView(wxCommandEvent& event);
};
wxDECLARE_APP(AWebpApp);

