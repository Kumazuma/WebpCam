#pragma once
#include<wx/wx.h>
#include"wx/wxprec.h"
#include"presenter.h"
#include "frame.h"
class AWebpApp : public wxApp
{
private:
	CommandFrame* ui_mainFrame;
public:
	
	bool OnInit() override;
	int OnExit() override;
protected:
	
};
wxDECLARE_APP(AWebpApp);

