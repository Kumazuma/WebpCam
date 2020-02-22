#pragma once
#include<wx/wx.h>
#include"editpresenter.h"
#include<d2d1.h>
#include<d2d1helper.h>
#include "event.h"
namespace Edit
{
	class EditRenderWidget :public wxScrolledCanvas
	{
	private:
		std::optional<size_t> m_index;
		wxTimer* m_timer;
		bool m_isPlaying;
		wxImage m_selectedImage;
		uint32_t m_duration;
		int64_t m_lastTick;
		EditFramePresenter* m_presenter;
		wxBitmap m_workArea;
		float m_scale;
		wxPoint m_centerPoint;
		
		std::optional<wxPoint> m_prevMousePosition;
		wxRect m_cropArea;
		int64_t m_lastRenderTime = 0;
	public:
		virtual void ScrollWindow(int dx, int dy, const wxRect* rect = NULL) override;
		EditRenderWidget();
		~EditRenderWidget();
		EditRenderWidget(
			wxWindow* parent, wxWindowID winid,
			const wxPoint& pos, const wxSize& size,
			long style, const wxValidator& val, const wxString& name) :
			wxScrolledCanvas(parent, winid, pos, size, style,name)
		{
			Init();
		}
		void SetPresenter(EditFramePresenter* presenter);
		void SetSelectImage(std::optional<size_t> index);
		void PlayAnimImage();
	protected:
		void Init();
		void DoPaint();
		void OverDraw();
#ifndef __WXMSW__
		void OverDraw(wxGraphicsContext* gc);
#else
		
		ID2D1Factory* m_factory;
		ID2D1HwndRenderTarget* m_renderTarget;
		ID2D1Bitmap* m_bitmap;
		wxImage m_backbuffer;
		wxClientDC* m_clientDC;
		void InitDirect2D();
		void ReleaseDirect2D();
		void InitResource();
		void ReleaseResource();
#endif
	protected:
		void OnScrollWinEvent(wxScrollWinEvent& event);
		void OnPaint(wxPaintEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnMouseLeftDown(wxMouseEvent& event);
		void OnMouseLeftUp(wxMouseEvent& event);
		void OnMouseMotion(wxMouseEvent& event);
		void OnMouseWheel(wxMouseEvent& event);
		void OnKeyDown(wxKeyEvent& event);
		void OnKeyUp(wxKeyEvent& event);
	protected:
		void OnTimer(wxTimerEvent& event);
		void OnIdle(wxIdleEvent& event);
		
		void ProcessAnim();
	private:
		wxDECLARE_DYNAMIC_CLASS(EditRenderWidget);
		wxDECLARE_EVENT_TABLE();

	};
}
