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
	
protected:
	wxFont m_defaultFont;
};
wxDECLARE_APP(AWebpApp);

