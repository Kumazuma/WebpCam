#pragma once
#include<wx/wx.h>
#include"UIDesign.h"
#include "interface.h"
class EditForm;
class EditFrame : public UIEditFrame
{
	wxDECLARE_EVENT_TABLE();
private:
	IImageStore* m_imageStore;
	EditForm* ui_editForm;
public:
	EditFrame(IImageStore* imageStore);
	virtual ~EditFrame();
protected:
	void OnRbarBtnSaveFileClick(wxRibbonButtonBarEvent& event);
};
class EditForm : public UIEditForm
{
private:
public:
	EditForm(wxWindow* parent);
};