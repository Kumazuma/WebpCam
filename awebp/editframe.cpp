#include <wx/wxprec.h>
#include "editframe.h"
#include "encoderview.h"
wxBEGIN_EVENT_TABLE(EditFrame, wxFrame)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SAVE_FILE, EditFrame::OnRbarBtnSaveFileClick)
wxEND_EVENT_TABLE();
EditFrame::EditFrame(IImageStore* imageStore):
	UIEditFrame(nullptr, wxID_ANY, wxT("edit form")),
	m_imageStore(imageStore)
{
	ui_editForm = new EditForm(this, *m_imageStore);
	this->GetSizer()->Add(ui_editForm,1,wxEXPAND);
	

	this->Layout();
}


EditFrame::~EditFrame()
{
	if (m_imageStore != nullptr)
	{
		delete m_imageStore;
		m_imageStore = nullptr;
	}
}

void EditFrame::OnRbarBtnSaveFileClick(wxRibbonButtonBarEvent& event)
{
	wxFrame* frame = new EncoderFrame(m_imageStore);
	m_imageStore = nullptr;
	frame->Show();
	this->Close();
}

EditForm::EditForm(wxWindow* parent, IImageStore& imageStore):
	UIEditForm(parent),
	m_imageStore(imageStore)
{
	for (int i = 0; i < imageStore.GetSize(); i++)
	{
		auto it = imageStore.Get(i);
		auto* temp = new FrameListItemWidgets(ui_frameList, wxID_ANY, it.first, it.second);
		ui_frameList->AddFrameImage(temp);
	}
}
