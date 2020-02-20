#pragma once
#include <wx/wx.h>
#include <optional>
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
public:
	CaptureFrame();
private:
	wxSize CvtCaptureRegionToWindowSize(const wxRect& rc);
	wxRect CvtWindowSizeToCaptureRegion(const wxSize& size);
protected:
	void CalcSizingRect(const wxPoint& mousePosition);
	wxCursor GetSizingCursor();
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseLost(wxMouseCaptureLostEvent& event);
private:
	
	wxDECLARE_EVENT_TABLE();
};