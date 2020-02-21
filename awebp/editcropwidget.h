#pragma once
#include<wx/wx.h>
#include"editpresenter.h"
#include<d2d1.h>
#include<d2d1helper.h>

namespace Edit
{
	class EditCropToolWidget :public wxScrolledCanvas
	{
	private:
		EditFramePresenter* m_presenter;
		wxBitmap m_workArea;
		float m_scale;
		wxPoint m_centerPoint;
		wxSharedPtr<wxMemoryDC> m_memDC;
		std::optional<wxPoint> m_prevMousePosition;
		wxRect m_cropArea;
	public:
		virtual void ScrollWindow(int dx, int dy, const wxRect* rect = NULL) override;
		EditCropToolWidget();
		~EditCropToolWidget();
		EditCropToolWidget(EditFramePresenter& presenter, wxWindow* parent, wxWindowID id);
	protected:
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
	private:
		wxDECLARE_DYNAMIC_CLASS(EditCropToolWidget);
		wxDECLARE_EVENT_TABLE();

	};
}
