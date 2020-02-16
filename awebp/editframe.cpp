#include <wx/wxprec.h>
#include "editframe.h"
#include "encoderview.h"

EditFrame::EditFrame(IImageStore* imageStore):
	UIEditFrame(nullptr, wxID_ANY, wxT("edit form")),
	m_presenter(imageStore)
{
	ui_editForm = new EditForm(this, m_presenter);
	this->GetSizer()->Add(ui_editForm,1,wxEXPAND);
	

	this->Layout();
}


EditFrame::~EditFrame()
{
}

void EditFrame::OnRbarBtnSaveFileClick(wxRibbonButtonBarEvent& event)
{
	wxFrame* frame = new EncoderFrame(m_presenter.MoveImageStore());
	frame->Show();
	this->Close();
}

EditForm::EditForm(wxWindow* parent, EditFramePresenter& presenter):
	UIEditForm(parent),
	m_presenter(presenter)
{
	for (int i = 0; i < m_presenter.GetImagesCount(); i++)
	{
		auto* temp = new FrameListItemWidgets(ui_frameList,  wxID_ANY, &m_presenter, i);
		ui_frameList->AddFrameImage(temp);
	}
}

wxBEGIN_EVENT_TABLE(EditFrame, wxFrame)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SAVE_FILE, EditFrame::OnRbarBtnSaveFileClick)
wxEND_EVENT_TABLE()