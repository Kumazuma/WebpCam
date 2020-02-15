#pragma once
#include<wx/wx.h>
#include"UIDesign.h"
#include "interface.h"

class EditFrame : public UIEditFrame
{
	wxDECLARE_EVENT_TABLE();
private:
	IImageStore* m_imageStore;
public:
	EditFrame(IImageStore* imageStore);
	virtual ~EditFrame();
protected:
	void OnRbarBtnSaveFileClick(wxRibbonButtonBarEvent& event);
};