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
private:
	SizingState m_sizingState;
	std::optional<wxPoint> m_prevPosition;
	AppPresenter& m_presenter;
public:
	CaptureFrame(wxWindow* parent, AppPresenter& presenter);
	void SetCapturedRect(wxRect rc);
private:
	wxSize CvtCaptureRegionToWindowSize(const wxRect& rc);
	wxRect CvtWindowSizeToCaptureRegion(const wxSize& size);
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
private:
	
	wxDECLARE_EVENT_TABLE();
};