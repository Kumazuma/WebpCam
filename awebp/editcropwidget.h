#pragma once
#include<wx/wx.h>
#include"editpresenter.h"
namespace Edit
{
	class EditCropToolWidget :public wxScrolledCanvas
	{
	private:
		EditFramePresenter* m_presenter;
		wxBitmap m_workArea;
		float m_scale;
		wxPoint m_centerPoint;
		wxMemoryDC* m_memDC;
	public:
		virtual void ScrollWindow(int dx, int dy, const wxRect* rect = NULL) override;
		EditCropToolWidget();
		~EditCropToolWidget();
		EditCropToolWidget(EditFramePresenter& presenter, wxWindow* parent, wxWindowID id);
	protected:
		void DoPaint();
		void OverDraw();
		void OverDraw(wxGraphicsContext* gc);
	protected:
		void OnScrollWinEvent(wxScrollWinEvent& event);
		void OnPaint(wxPaintEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnMouseLeftDown(wxMouseEvent& event);
		void OnMouseLeftUp(wxMouseEvent& event);
		void OnMouseMotion(wxMouseEvent& event);
		void OnMouseWheel(wxMouseEvent& event);
	private:
		wxDECLARE_DYNAMIC_CLASS(EditCropToolWidget);
		wxDECLARE_EVENT_TABLE();

	};
}
