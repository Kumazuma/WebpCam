#pragma once
#include<wx/wx.h>
#include"wx/wxprec.h"
#include"presenter.h"
class AWebpApp : public wxApp
{
private:
public:
	
	bool OnInit() override;
	int OnExit() override;
	void KeyDown(wxKeyEvent* event);
	void KeyUp(wxKeyEvent* event);
protected:
	
};
wxDECLARE_APP(AWebpApp);

