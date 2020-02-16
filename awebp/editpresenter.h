#pragma once
#include<wx/wx.h>
#include"editframemodel.h"
class EditFramePresenter: public wxEvtHandler
{
	
	wxDECLARE_DYNAMIC_CLASS(EditFramePresenter);
public:
	EditFramePresenter():m_model(nullptr) {}
	EditFramePresenter(IImageStore* imageStore);
private:
	EditFrameModel m_model;
};