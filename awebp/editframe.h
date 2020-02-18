#pragma once
#include<wx/wx.h>
#include"UIDesign.h"
#include "interface.h"
#include "editframelistwidget.h"
class EditForm;
class EditFrame : public UIEditFrame
{
	wxDECLARE_EVENT_TABLE();
private:
	EditFramePresenter m_presenter;
	EditForm* ui_editForm;
	std::optional<size_t> m_lastSelectedIndex;
public:
	EditFrame(IImageStore* imageStore,const wxSize& imageSize);
	virtual ~EditFrame();
protected:
	void DoPaint(wxDC& dc);
	void OnRbarBtnSaveFile(wxRibbonButtonBarEvent& event);
	void OnRefreshView(wxCommandEvent& event);
	void OnRbarBtnDeleteFrames(wxRibbonToolBarEvent& event);
	void OnRbarBtnResizeFrames(wxRibbonToolBarEvent& event);
	void OnSelectFramePaint(wxPaintEvent& event);
	void OnListItemSelected(wxCommandEvent& event);
	void OnRbarBtnUndo(wxRibbonButtonBarEvent& event);
	void OnRbarBtnRedo(wxRibbonButtonBarEvent& event);
};
class EditForm : public UIEditForm
{
private:
	EditFramePresenter& m_presenter;
public:
	EditForm(wxWindow* parent, EditFramePresenter& imageStore);
};