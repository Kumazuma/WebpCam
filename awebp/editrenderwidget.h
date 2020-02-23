﻿#pragma once
#include<wx/wx.h>
#include"editpresenter.h"
#include<d2d1.h>
#include<d2d1helper.h>
#include "event.h"
#include "util.h"
namespace Edit
{
	class EditRenderWidget :public wxControl
	{
	private:
		EditFramePresenter* m_presenter;
		std::optional<wxPoint> m_prevMousePosition;
		std::optional<size_t> m_index;
		wxImage m_selectedImage;
		wxTimer* m_timer;
		bool m_isPlaying;
		uint32_t m_duration;
		int64_t m_lastTick;
		
		float m_scale;
		wxPoint m_viewStart;

		int64_t m_lastRenderTime = 0;
		DirectionState m_direction = DirectionState::None;
	public:
		//스크롤 처리를 위한 메소드들
		void AdjustScrollbars();
		int AdjectScroll(const int value, const  float factorInImage, const  float factorInScreen);
		void SetScrollRate(int x, int y);
		wxPoint GetViewStart();
		void Scroll(int x, int y);
		void Scroll(const wxPoint& pt) { Scroll(pt.x, pt.y); }
		void ScrollOnWhell(const wxPoint& pt);
		virtual void ScrollWindow(int dx, int dy, const wxRect* rect = NULL) override;
		EditRenderWidget();
		~EditRenderWidget();
		EditRenderWidget(
			wxWindow* parent, wxWindowID winid,
			const wxPoint& pos, const wxSize& size,
			long style, const wxValidator& val, const wxString& name) :
			wxControl(parent, winid, pos, size, style |wxVSCROLL |wxHSCROLL, val, name)
		{
			Init();
		}
		void RefreshView();
		void MoveView(wxPoint delta);
		void UpdateCropRect(wxPoint delta);
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
		ID2D1PathGeometry* m_cropAreaRegion;
		ID2D1PathGeometry* m_cropAreaDotLines;
		wxClientDC* m_clientDC;
		void InitDirect2D();
		void ReleaseDirect2D();
		void InitResource();
		void ReleaseResource();
#endif
	protected:
		void OnScrollThubTrack(wxScrollWinEvent& event);
		//void OnScrollWinEvent(wxScrollWinEvent& event);
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
