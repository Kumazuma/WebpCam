#pragma once
#include <wx/wx.h>
#include "EncorderPresenter.h"
#include "interface.h"
class EncodingProgressDialog : public wxDialog
{
	wxDECLARE_EVENT_TABLE();
private:
	EncoderPresenter m_presenter;
	wxGauge* ui_gauge;
	wxStaticText* ui_staticText;

public:
	EncodingProgressDialog(wxWindow* parent, IImageStore& store);
	~EncodingProgressDialog();
protected:
	void OnRefreshView(wxCommandEvent& event);
	void OnClosing(wxCloseEvent& event);
	
};