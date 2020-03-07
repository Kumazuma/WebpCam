#include "wx/wxprec.h"
#include "encoderview.h"
#include<wx/filedlg.h>
#include "event.h"
EncodingDialog::EncodingDialog(wxWindow* parent, IImageStore& store):
	UIEncodingDialog(parent, wxID_ANY, wxT("encode form")),
	m_presenter(this, store)
{
	Center();
	m_presenter.Bind(EVT_RefreshView, &EncodingDialog::OnRefreshView, this);
	m_presenter.Bind(EVT_FINISH_JOB, &EncodingDialog::OnFinishEncode, this);
	FindWindowById(ID_FILE_TYPE)->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &EncodingDialog::OnUpdateEncoder, this);
	FindWindowById(ID_QUALITY)->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED, &EncodingDialog::OnUpdateQuality, this);
	try
	{
		auto fileType = wxDynamicCast(FindWindowById(ID_FILE_TYPE), wxChoice);
		if (!fileType)
			throw wxString(wxT("FILE TYPE "));
		fileType->Append(wxT("webp"));
		fileType->Append(wxT("webm"));
		fileType->Select(0);
	}
	catch (wxString err)
	{
		wxLogGeneric(wxLogLevelValues::wxLOG_Error, err);
		Close(true);
	}
	this->Bind(wxEVT_BUTTON, 
		[this](wxCommandEvent&)
		{
			
			wxFileDialog saveFileDialog(this, wxT("Save Anim File"), "", "",
				m_presenter.GetFileFilter(), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			if (saveFileDialog.ShowModal() == wxID_CANCEL)
				return;
			FindWindowById(ID_SAVE)->Disable();
			m_presenter.SaveAnimImage(saveFileDialog.GetPath());
		}, ID_SAVE);
	this->Bind(wxEVT_BUTTON,
		[this](wxCommandEvent&)
		{
			Close();
		}, wxID_CANCEL);
}

EncodingDialog::~EncodingDialog()
{
}

void EncodingDialog::OnRefreshView(wxCommandEvent& event)
{
	auto gauge = wxDynamicCast(FindWindowById(ID_PROGRESS_GAUGE), wxGauge);
	auto label = wxDynamicCast(FindWindowById(ID_PROGRESS_LABEL), wxStaticText);
	int progress = m_presenter.GetEncodeProgress();
	int total = m_presenter.GetImagesCount();
	wxString labelText;
	gauge->SetValue(progress);
	gauge->SetRange(total);
	labelText << progress << "/" << total
		<< "(" << progress * 100 / total << "%)";
	label->SetLabel(labelText);
	this->SetSize(this->GetBestSize());
	this->Layout();
	if (progress == total)
	{
		
	}
}

void EncodingDialog::OnFinishEncode(wxCommandEvent&)
{
	auto gauge = wxDynamicCast(FindWindowById(ID_PROGRESS_GAUGE), wxGauge);
	auto label = wxDynamicCast(FindWindowById(ID_PROGRESS_LABEL), wxStaticText);
	int progress = m_presenter.GetEncodeProgress();
	int total = m_presenter.GetImagesCount();
	wxString labelText;
	gauge->SetValue(progress);
	gauge->SetRange(total);
	labelText << progress << "/" << total
		<< "(" << progress * 100 / total << "%)";
	label->SetLabel(labelText);
	this->SetSize(this->GetBestSize());
	this->Layout();

	wxMessageBox(wxT("완료되었습니다"), wxT("완료"), wxOK | wxCENTRE | wxICON_INFORMATION, this);
	Close();
}

void EncodingDialog::OnClosing(wxCloseEvent& event)
{
	
	if (event.CanVeto() && //닫는 것을 거부할 수 있어야 하고
		m_presenter.Encoding() && //인코딩 중이여야 하고
		m_presenter.GetEncodeProgress() != m_presenter.GetImagesCount())//완료가 안 되어 있어야 한다.
	{
		auto res =
			wxMessageBox (wxT("정말로 종료하시겠습니까?"), wxT("인코딩"),
				wxYES | wxNO | wxCENTER |wxICON_QUESTION, this);
		if (res == wxNO)
		{
			event.Skip();
			event.Veto();
			return;
		}
		else
		{
			m_presenter.StopEncode();
		}
	}
	else
	{

	}
	Destroy();
	
}

void EncodingDialog::OnUpdateQuality(wxSpinEvent& event)
{
	
	m_presenter.SetQuality(event.GetValue());
}

void EncodingDialog::OnUpdateEncoder(wxCommandEvent& event)
{
	switch (event.GetSelection())
	{
	case 0:
		m_presenter.SetEncoderToWebp();
		break;
	case 1:
		m_presenter.SetEncoderToWebm();
		break;
	}
}

wxBEGIN_EVENT_TABLE(EncodingDialog, wxDialog)
EVT_COMMAND(wxID_ANY, EVT_RefreshView, EncodingDialog::OnRefreshView)
EVT_CLOSE(EncodingDialog::OnClosing)
wxEND_EVENT_TABLE()