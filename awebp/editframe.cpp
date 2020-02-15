#include <wx/wxprec.h>
#include "editframe.h"
#include "encoderview.h"
wxBEGIN_EVENT_TABLE(EditFrame, wxFrame)	
	EVT_RIBBONBUTTONBAR_CLICKED(ID_SAVE_FILE,EditFrame::OnRbarBtnSaveFileClick)
wxEND_EVENT_TABLE()
EditFrame::EditFrame(IImageStore* imageStore):
	UIEditFrame(nullptr, wxID_ANY, wxT("ÆíÁýÃ¢")),
	m_imageStore(imageStore)
{

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

