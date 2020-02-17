#pragma once
#include <wx/wx.h>
#include "EncorderPresenter.h"
#include "interface.h"
class EncodingPregressDialog : public wxDialog
{
private:
	EncoderPresenter m_presenter;
	wxGauge* ui_gauge;
public:
	EncodingPregressDialog(wxWindow* parent, IImageStore& store);
	~EncodingPregressDialog();
protected:
	void OnRefreshView(wxCommandEvent& event);
};