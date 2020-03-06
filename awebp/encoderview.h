#pragma once
#include <wx/wx.h>
#include "UIDesign.h"
#include "EncorderPresenter.h"
#include "interface.h"
class EncodingDialog : public UIEncodingDialog
{
	wxDECLARE_EVENT_TABLE();
private:
	EncoderPresenter m_presenter;
public:
	EncodingDialog(wxWindow* parent, IImageStore& store);
	~EncodingDialog();
protected:
	void OnRefreshView(wxCommandEvent& event);
	void OnClosing(wxCloseEvent& event);
	void OnUpdateQuality(wxSpinEvent&);
	void OnUpdateEncoder(wxCommandEvent&);
};