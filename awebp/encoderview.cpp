#include "wx/wxprec.h"
#include "encoderview.h"
#include<wx/filedlg.h>
#include "event.h"
EncodingPregressDialog::EncodingPregressDialog(wxWindow* parent, IImageStore& store):
	wxDialog(parent, wxID_ANY, wxT("encode form")),
	m_presenter(this, store)
{
	Center();
	ui_gauge = new wxGauge(this, wxID_ANY, m_presenter.GetImagesCount());
	ui_gauge->Show();
	m_presenter.Bind(EVT_RefreshView, & EncodingPregressDialog::OnRefreshView, this);
	wxFileDialog saveFileDialog(this, wxT("Save Anim File"), "", "",
		m_presenter.GetFileFilter(), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...
	m_presenter.SaveAnimImage(saveFileDialog.GetPath());
}

EncodingPregressDialog::~EncodingPregressDialog()
{
}

void EncodingPregressDialog::OnRefreshView(wxCommandEvent& event)
{
	ui_gauge->SetValue(m_presenter.GetEncodeProgress());
	if (m_presenter.GetEncodeProgress() == m_presenter.GetImagesCount())
	{
		wxMessageBox(wxT("완료되었습니다"), wxT("완료"), wxOK | wxCENTRE | wxICON_INFORMATION, this);
		Close();
	}
}
