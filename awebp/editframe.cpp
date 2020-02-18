#include <wx/wxprec.h>
#include "editframe.h"
#include "encoderview.h"
#include "event.h"
#include <wx/dcbuffer.h>

EditFrame::EditFrame(IImageStore* imageStore, const wxSize& imageSize):
	UIEditFrame(nullptr, wxID_ANY, wxT("edit form")),
	m_presenter(this, imageStore, imageSize)
{
	SetIcon(wxICON(AAAAA));
	m_presenter.Bind(EVT_RefreshView, &EditFrame::OnRefreshView, this);
	ui_editForm = new EditForm(this, m_presenter);
	this->FindWindow(wxT("ui_draw"))->Bind(wxEVT_PAINT, &EditFrame::OnSelectFramePaint, this);
	this->FindWindow(wxT("ui_frameList"))->Bind(wxEVT_LISTBOX, &EditFrame::OnListItemSelected, this);
	this->GetSizer()->Add(ui_editForm,1,wxEXPAND);
	this->Layout();
}


EditFrame::~EditFrame()
{
}

void EditFrame::DoPaint(wxDC& dc)
{
	if (m_lastSelectedIndex.has_value())
	{
		wxImage image;
		uint32_t duration;
		m_presenter.GetImage(*m_lastSelectedIndex, image, duration);
		if (image.IsOk())
		{
			dc.DrawBitmap(image, 0, 0);
		}
	}
}

void EditFrame::OnRbarBtnSaveFile(wxRibbonButtonBarEvent& event)
{
	wxDialog* frame = new EncodingProgressDialog(this, m_presenter.GetImageStore());
	frame->ShowModal();
}

void EditFrame::OnRefreshView(wxCommandEvent& event)
{
	auto widgets = wxDynamicCast(this->FindWindow(wxT("ui_frameList")), FrameListWidget) ;
	widgets->ClearChildren();
	widgets->Hide();
	for (int i = 0; i < m_presenter.GetImagesCount(); i++)
	{
		auto* temp = new FrameListItemWidget(widgets, &m_presenter, i);
		widgets->AddFrameImage(temp);
	}
	widgets->Show();
	auto lbHistory = wxDynamicCast(this->FindWindow(wxT("ui_historyBox")), wxListBox);
	if (lbHistory != nullptr)
	{
		lbHistory->Clear();
		auto history = m_presenter.GetHistory();
		for (auto item : history)
		{
			lbHistory->Append(item->GetDescription(), (void*)item);
		}
	}
}

void EditFrame::OnRbarBtnDeleteFrames(wxRibbonToolBarEvent& event)
{
	auto widgets = wxDynamicCast(this->FindWindow(wxT("ui_frameList")), FrameListWidget);
	if (widgets != nullptr)
	{
		auto selectedItemIndexes = widgets->GetSelections();
		if (selectedItemIndexes.size() == 0)return;
		size_t start = selectedItemIndexes.front();
		size_t end = *selectedItemIndexes.rbegin();
		m_presenter.DeleteFrame(start, end + 1);
	}
}

void EditFrame::OnRbarBtnResizeFrames(wxRibbonToolBarEvent& event)
{
	UIResizeDialog* dialog = new UIResizeDialog(this);
	auto ui_width = wxDynamicCast(dialog->FindWindowByName(wxT("ui_width")), wxSpinCtrl);
	auto ui_height = wxDynamicCast(dialog->FindWindowByName(wxT("ui_height")), wxSpinCtrl);
	auto imageSize = m_presenter.GetImageSize();
	ui_width->SetValue(imageSize.GetWidth());
	ui_height->SetValue(imageSize.GetHeight());
	if (dialog->ShowModal() == wxID_OK)
	{
		wxSize resize(
			ui_width->GetValue(),
			ui_height->GetValue()
		);
		//같으면 리사이즈 할 필요가 없다.
		if (resize == imageSize)
		{
			return;
		}
		m_presenter.ResizeImage(resize);
	}
	delete dialog;
}

void EditFrame::OnSelectFramePaint(wxPaintEvent& event)
{
	auto widget = wxDynamicCast(this->FindWindow(wxT("ui_frameList")), FrameListWidget);
	wxBufferedPaintDC dc(wxDynamicCast(event.GetEventObject(), wxWindow));
	DoPaint(dc);
}

void EditFrame::OnListItemSelected(wxCommandEvent& event)
{
	m_lastSelectedIndex = event.GetSelection();
	wxClientDC dc(this->FindWindow(wxT("ui_draw")));
	wxBufferedDC bufferedDC(&dc);
	DoPaint(bufferedDC);
	//this->FindWindow(wxT("ui_draw"))->Refresh();
}

void EditFrame::OnRbarBtnUndo(wxRibbonButtonBarEvent& event)
{
	m_presenter.Undo();
}

void EditFrame::OnRbarBtnRedo(wxRibbonButtonBarEvent& event)
{
	m_presenter.Redo();
}

EditForm::EditForm(wxWindow* parent, EditFramePresenter& presenter) :
	UIEditForm(parent),
	m_presenter(presenter)
{
	ui_frameList->Hide();
	for (int i = 0; i < m_presenter.GetImagesCount(); i++)
	{
		auto* temp = new FrameListItemWidget(ui_frameList, &m_presenter, i);
		ui_frameList->AddFrameImage(temp);
	}
	ui_frameList->Show();
}

wxBEGIN_EVENT_TABLE(EditFrame, wxFrame)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SAVE_FILE, EditFrame::OnRbarBtnSaveFile)
EVT_RIBBONBUTTONBAR_CLICKED(wxID_UNDO, EditFrame::OnRbarBtnUndo)
EVT_RIBBONBUTTONBAR_CLICKED(wxID_REDO, EditFrame::OnRbarBtnRedo)
EVT_RIBBONTOOLBAR_CLICKED(ID_RESIZE_FRAME, EditFrame::OnRbarBtnResizeFrames)
EVT_RIBBONTOOLBAR_CLICKED(wxID_DELETE, EditFrame::OnRbarBtnDeleteFrames)
wxEND_EVENT_TABLE()