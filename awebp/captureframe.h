#pragma once
#include <wx/wx.h>
#include <optional>
#include "presenter.h"
#include "util.h"
class CaptureFrame : public wxFrame
{
	const int BOTTOM_PANEL_HEIGHT = 36;
	const int TOP_PANEL_HEIGHT = 36;
	const int BORDER_THICKNESS = 10;
private:
	DirectionState m_sizingState;
	std::optional<wxPoint> m_prevPosition;
	AppPresenter m_presenter;
	int m_recoredRectScale;
public:
	CaptureFrame();
	void SetCapturedRect(wxRect rc);
private:
	wxRect CvtCaptureRegionToWindowRect(const wxRect& rc);
	wxRect CvtWindowRectToCaptureRegion(const wxRect& size);
	void SetRect(const wxRect& rc);
protected:
	void CalcSizingDirection(const wxPoint& mousePosition);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseLost(wxMouseCaptureLostEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnSized(wxSizeEvent& event);
	void OnMoved(wxMoveEvent& event);
	void OnChildMouseMotion(wxMouseEvent& event);
	void OnClickBtnStart(wxCommandEvent& event);
	void OnClickBtnStop(wxCommandEvent& event);
	void OnSpinWidth(wxSpinEvent& event);
	void OnSpinHeigt(wxSpinEvent& event);
	void OnSpinPosX(wxSpinEvent& event);
	void OnSpinPosY(wxSpinEvent& event);
	void OnChoiceFPS(wxCommandEvent& event);
	void OnChangeChkUsingTemoFile(wxCommandEvent& event);
	void OnClickSave(wxCommandEvent& event);
	void OnRefleshView(wxCommandEvent& event);
	void OnClickClose(wxCommandEvent& event);
	void OnDPIChageEvent(wxDPIChangedEvent& event);
private:
	
	wxDECLARE_EVENT_TABLE();
};