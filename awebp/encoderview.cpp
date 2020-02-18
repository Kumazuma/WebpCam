#include "wx/wxprec.h"
#include "encoderview.h"
#include<wx/filedlg.h>
#include "event.h"
EncodingProgressDialog::EncodingProgressDialog(wxWindow* parent, IImageStore& store):
	wxDialog(parent, wxID_ANY, wxT("encode form")),
	m_presenter(this, store)
{
	Center();
	auto sizer = new wxBoxSizer(wxVERTICAL);
	ui_staticText = new wxStaticText(this, wxID_ANY, wxT("0/0(0%)"));
	ui_gauge = new wxGauge(this, wxID_ANY, m_presenter.GetImagesCount());
	
	sizer->Add(ui_staticText, 0, wxALL | wxEXPAND);
	sizer->Add(ui_gauge, 0, wxALL|wxEXPAND);
	SetSizer(sizer);
	Layout();
	FitInside();
	SetSize(GetBestSize());
	m_presenter.Bind(EVT_RefreshView, & EncodingProgressDialog::OnRefreshView, this);
	wxFileDialog saveFileDialog(this, wxT("Save Anim File"), "", "",
		m_presenter.GetFileFilter(), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		Close(true);
		return;
	}
		
	m_presenter.SaveAnimImage(saveFileDialog.GetPath());
}

EncodingProgressDialog::~EncodingProgressDialog()
{
}

void EncodingProgressDialog::OnRefreshView(wxCommandEvent& event)
{
	ui_gauge->SetValue(m_presenter.GetEncodeProgress());
	int progress = m_presenter.GetEncodeProgress();
	int total = m_presenter.GetImagesCount();
	wxString label;
	label << progress << "/" << total
		<< "(" << progress * 100 / total << "%)";
	ui_staticText->SetLabel(label);
	
	if (progress == total)
	{
		wxMessageBox(wxT("완료되었습니다"), wxT("완료"), wxOK | wxCENTRE | wxICON_INFORMATION, this);
		Close();
	}
}

void EncodingProgressDialog::OnClosing(wxCloseEvent& event)
{
	if (event.CanVeto() && m_presenter.GetEncodeProgress() != m_presenter.GetImagesCount())
	{
		auto res = (wxStandardID)
			wxMessageBox (wxT("정말로 종료하시겠습니까?"), wxT("인코딩"),
				wxYES | wxNO | wxCENTER |wxICON_QUESTION, this);
		if (res == wxNO)
		{
			event.Veto();
			return;
		}
		else
		{
			m_presenter.StopEncode();
			
		}
	}
	event.Skip();
	
}

wxBEGIN_EVENT_TABLE(EncodingProgressDialog, wxDialog)
EVT_COMMAND(wxID_ANY, EVT_RefreshView, EncodingProgressDialog::OnRefreshView)
EVT_CLOSE(EncodingProgressDialog::OnClosing)
wxEND_EVENT_TABLE()