#include "wx/wxprec.h"
#include "encoderview.h"
#include<wx/filedlg.h>
EncoderFrame::EncoderFrame(IImageStore* store):
	wxFrame(nullptr, wxID_ANY, wxT("ÀÎÄÚµù")),
	m_presenter(new EncoderPresenter( this, store))
{
	Center();
	wxFileDialog saveFileDialog(this, wxT("Save Anim File"), "", "",
		m_presenter->GetFileFilter(), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...
	m_presenter->SaveAnimImage(saveFileDialog.GetPath());
}

EncoderFrame::~EncoderFrame()
{
	delete m_presenter;
}
