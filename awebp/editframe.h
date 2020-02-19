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
	wxAuiManager* m_manager;
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
	void OnRbarBtnRestoreWindow(wxRibbonButtonBarEvent& event);
	void OnRBarBtnNewCap(wxRibbonButtonBarEvent& event);
	void OnBtnClickPlay(wxCommandEvent& event);
	void OnAnimProcessAFrame(wxCommandEvent& event);
	void OnPropertyValueChanged(wxPropertyGridEvent& event);
};
class EditForm : public UIEditForm
{
private:
	EditFramePresenter& m_presenter;
public:
	EditForm(wxWindow* parent, EditFramePresenter& imageStore);
	inline wxAuiManager& GetAuiManger() { return m_mgr; }
};