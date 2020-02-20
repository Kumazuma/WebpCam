#pragma once
#include <wx/wx.h>
#include <optional>
#include "presenter.h"
enum class SizingState
{
	SSNone = 0,
	SSN = 4,
	SSW = 8,
	SSE = 2,
	SSS = 1,
	SSNE = SSN | SSE,
	SSNW = SSN | SSW,
	SSSE = SSS | SSE,
	SSSW = SSS | SSW,
	SSNSEW = SSN|SSS|SSE|SSW
};
SizingState& operator |= (SizingState& obj, SizingState state);

bool operator && (SizingState& obj, SizingState state);
class CaptureFrame : public wxFrame
{
	const int BOTTOM_PANEL_HEIGHT = 36;
	const int TOP_PANEL_HEIGHT = 36;
	const int BORDER_THICKNESS = 5;
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
protected:
	void CalcSizingDirection(const wxPoint& mousePosition);
	wxCursor GetSizingCursor();
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseLost(wxMouseCaptureLostEvent& event);
	void OnSized(wxSizeEvent& event);
	void OnMoved(wxMoveEvent& event);

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