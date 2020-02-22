#pragma once
#include<wx/wx.h>
#include<optional>
#include"editpresenter.h"
#include <Wx/graphics.h>
class EditFrameRenderWidget :public wxControl
{
private:
	EditFramePresenter* m_presenter;
	std::optional<size_t> m_index;
	wxTimer* m_timer;
	bool m_isPlaying;
	wxBitmap m_bitmap;
	uint32_t m_duration;
	int64_t m_lastTick;
	wxGraphicsRenderer* m_renderer;
public:
    EditFrameRenderWidget();
	EditFrameRenderWidget(
		wxWindow* parent, wxWindowID winid,
		const wxPoint& pos, const wxSize& size,
		long style, const wxValidator& val, const wxString& name) :
		wxControl(parent, winid, pos, size, style, val, name)
	{
		Init();
	}
	~EditFrameRenderWidget();
	void SetPresenter(EditFramePresenter* presenter);
	void SetSelectImage(std::optional<size_t> index);
	void PlayAnimImage();
protected:
	void Init();
	void ProcessAnim();
	void DoPaint(wxClientDC& dc);
	void DoPaint();
protected:
	void OnPaint(wxPaintEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnIdle(wxIdleEvent& event);
private:
    wxDECLARE_DYNAMIC_CLASS(EditFrameRenderWidget);
    wxDECLARE_EVENT_TABLE();
};