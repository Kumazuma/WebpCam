#pragma once
#include <wx/wx.h>
#include <optional>
#include "presenter.h"
enum class SizingState
{
	None = 0,
	North = 4,
	West = 8,
	East = 2,
	South = 1,
	NorthEast = North | East,
	NorthWest = North | West,
	SouthEast = South | East,
	SouthWest = South | West,
	All = North|South|East|West
};
SizingState& operator |= (SizingState& obj, SizingState state);

bool operator && (SizingState& obj, SizingState state);
class CaptureFrame : public wxFrame
{
	const int BOTTOM_PANEL_HEIGHT = 36;
	const int TOP_PANEL_HEIGHT = 36;
	const int BORDER_THICKNESS = 10;
private:
	SizingState m_sizingState;
	std::optional<wxPoint> m_prevPosition;
	AppPresenter m_presenter;
public:
	CaptureFrame();
	void SetCapturedRect(wxRect rc);
private:
	wxRect CvtCaptureRegionToWindowRect(const wxRect& rc);
	wxRect CvtWindowRectToCaptureRegion(const wxRect& size);
	void SetRect(const wxRect& rc);
	bool GetHitTests(const wxPoint& pt, wxWindow* window = nullptr);
protected:
	void CalcSizingDirection(const wxPoint& mousePosition);
	wxCursor GetSizingCursor();
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
private:
	
	wxDECLARE_EVENT_TABLE();
};