#pragma once
#include <wx/wx.h>
#include "UIDesign.h"
#include "presenter.h"
enum WidgetID {
	BTN_START = 1000,
	BTN_STOP
};
class CaptureRegionFrame : public wxFrame
{
	wxDECLARE_EVENT_TABLE();
public:
	CaptureRegionFrame(wxWindow* parent,AppPresenter& presenter,  const wxString& title);
	virtual ~CaptureRegionFrame();
protected:
	void OnMoved(wxMoveEvent& event);
	void OnSized(wxSizeEvent& event);
private:
	AppPresenter& m_presenter;
};
class CommandFrame : public UIRecordFrame
{
public:
	CommandFrame(const wxString& title);
	virtual ~CommandFrame();
protected:
	CaptureRegionFrame* ui_regionFrame;
	void OnClickBtnStart(wxCommandEvent& event);
	void OnClickBtnStop(wxCommandEvent& event);
	void OnSpinWidth(wxSpinEvent& event);
	void OnSpinHeigt(wxSpinEvent& event);
	void OnChoiceFPS(wxCommandEvent& event);
	void OnChangeChkUsingTemoFile(wxCommandEvent& event);
	void OnClickSave(wxCommandEvent& event);
	void OnRefleshView(wxCommandEvent& event);
	
private:
	AppPresenter m_presenter;
};
