#include <wx/wxprec.h>
#include "editframe.h"
#include "encoderview.h"
#include "event.h"
#include <wx/dcbuffer.h>
#include "frame.h"
EditFrame::EditFrame(IImageStore* imageStore, const wxSize& imageSize):
	UIEditFrame(nullptr, wxID_ANY, wxT("edit form")),
	m_presenter(this, imageStore, imageSize)
{
	SetIcon(wxICON(AAAAA));
	m_presenter.Bind(EVT_RefreshView, &EditFrame::OnRefreshView, this);
	ui_editForm = new EditForm(this, m_presenter);
	
	this->FindWindow(wxT("ui_frameList"))->Bind(wxEVT_LISTBOX, &EditFrame::OnListItemSelected, this);
	this->FindWindowById(ID_DRAW_WIDGET)->Bind(EVT_ANIM_PROCESS_A_FRAME, &EditFrame::OnAnimProcessAFrame, this);
	this->FindWindowById(ID_PROPERTY)->Bind(wxEVT_PG_CHANGED, &EditFrame::OnPropertyValueChanged, this);
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
	widgets->PostSizeEvent();
	auto lbHistory = wxDynamicCast(this->FindWindowById(ID_HISTORY_LIST), wxSimpleHtmlListBox);
	if (lbHistory != nullptr)
	{
		lbHistory->Clear();
		auto history = m_presenter.GetHistory();
		auto cursor = m_presenter.GetHistoryCursor();
		for (auto i = 0; i < history.size(); i++)
		{
			auto item = history[i];
			wxString html;
			wxString format;
			if (i < cursor)
			{
				//TO DO Nothing;
				format = wxT("%s");
			}
			else if (i == cursor)
			{
				format = wxT("<strong>%s</strong>");
			}
			else if (i > cursor)
			{
				format = wxT("<span style='color:#CCC;'>%s</span>");
			}
			html = wxString::Format(format, item->GetDescription());
			lbHistory->Append(html, (void*)item);
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
	auto ui_scale = wxDynamicCast(dialog->FindWindowByName(wxT("ui_scale")), wxSpinCtrl);
	auto imageSize = m_presenter.GetImageSize();
	if (dialog->ShowModal() == wxID_OK)
	{
		auto scale = ui_scale->GetValue() / 100.f;
		if (scale == 100)
		{
			return;
		}
		wxSize resize(
			imageSize.GetWidth()  * scale,
			imageSize.GetHeight() * scale);
		//같으면 리사이즈 할 필요가 없다.
		
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
	wxDynamicCast(FindWindowById(ID_DRAW_WIDGET), EditFrameRenderWidget)->SetSelectImage(m_lastSelectedIndex);
	auto progrid = wxDynamicCast(FindWindowById(ID_PROPERTY), wxPropertyGrid);
	auto t = wxDynamicCast(event.GetEventObject(), FrameListWidget);
	auto selections = t->GetSelections();
	auto item = progrid->GetProperty(wxT("PROPERTY_FRAME_DURATION"));
	item->ChangeFlag(wxPG_PROP_READONLY, true);
	if(selections.size() == 1)
	{
		item->ChangeFlag(wxPG_PROP_READONLY, false);
		item->SetValue(wxVariant((long)*m_presenter.GetFrameDuration(selections.front())));
	}
	
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

void EditFrame::OnRbarBtnRestoreWindow(wxRibbonButtonBarEvent& event)
{
	auto& mgr = ui_editForm->GetAuiManger();
	auto win = FindWindowById(ID_HISTORY_LIST);
	auto pane = mgr.GetPane(win);
	//pane.Right();
	pane.Dock().Right();
	mgr.RestorePane(pane);
	mgr.Update();
}

void EditFrame::OnRBarBtnNewCap(wxRibbonButtonBarEvent& event)
{
	auto frame = new CommandFrame(wxT("설정"));
	frame->Show();
	Close();
}

void EditFrame::OnBtnClickPlay(wxCommandEvent& event)
{
	auto widget = wxDynamicCast(FindWindowById(ID_DRAW_WIDGET), EditFrameRenderWidget);
	if (widget != nullptr)
	{
		auto index = wxDynamicCast(this->FindWindow(wxT("ui_frameList")), FrameListWidget)->GetSelection();
		if (index.has_value())
		{
			index = 0;
		}
		//widget->SetSelectImage(index);
		widget->PlayAnimImage();
	}
}

void EditFrame::OnAnimProcessAFrame(wxCommandEvent& event)
{
	wxDynamicCast(this->FindWindow(wxT("ui_frameList")), FrameListWidget)->SetSelection(event.GetSelection());
}

void EditFrame::OnPropertyValueChanged(wxPropertyGridEvent& event)
{
	auto res = wxMessageBox(wxT("해당 프레임의 시간을 바꾸시겠습니까?"), wxT("Question"), wxYES_NO | wxICON_QUESTION, this);
	if (res == wxYES)
	{
		event.GetProperty()->GetValue();
	}
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
	wxPGProperty* PROPERTY_IMAGE_SIZE;
	
	ui_propertyGrid->Append(new wxPropertyCategory(wxT("Project Information")));
	ui_propertyGrid->Append(new wxUIntProperty(wxT("프레임 수"), wxT("PROPERTY_FRAMES_COUNT"), presenter.GetImagesCount()))
		->ChangeFlag(wxPG_PROP_READONLY, true);
	auto imageSize = presenter.GetImageSize();
	auto s = wxString::Format(wxT("(%d, %d)"), imageSize.GetWidth(), imageSize.GetHeight());
	PROPERTY_IMAGE_SIZE = ui_propertyGrid->Append(new wxStringProperty(wxT("이미지 사이즈"), wxT("PROPERTY_IMAGE_SIZE"), s));
	PROPERTY_IMAGE_SIZE->ChangeFlag(wxPG_PROP_READONLY, true);
	ui_propertyGrid->AppendIn(PROPERTY_IMAGE_SIZE, new wxUIntProperty(wxT("폭"),wxT("PROPERTY_IMAGE_SIZE_WIDTH"), imageSize.GetWidth()))
		->ChangeFlag(wxPG_PROP_READONLY, true);
	ui_propertyGrid->AppendIn(PROPERTY_IMAGE_SIZE, new wxUIntProperty(wxT("높이"), wxT("PROPERTY_IMAGE_SIZE_HEIGHT"), imageSize.GetHeight()))
		->ChangeFlag(wxPG_PROP_READONLY, true);
	ui_propertyGrid->Append(new wxPropertyCategory(wxT("Frame Information")));
	ui_propertyGrid->Append(new wxUIntProperty(wxT("시간(ms)"), wxT("PROPERTY_FRAME_DURATION")));

	ui_frameList->Show();
	ui_drawWidget->SetPresenter(&presenter);
}

wxBEGIN_EVENT_TABLE(EditFrame, wxFrame)
EVT_BUTTON(ID_PLAY_BUTTON, EditFrame::OnBtnClickPlay)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SAVE_FILE, EditFrame::OnRbarBtnSaveFile)
EVT_RIBBONBUTTONBAR_CLICKED(wxID_UNDO, EditFrame::OnRbarBtnUndo)
EVT_RIBBONBUTTONBAR_CLICKED(wxID_REDO, EditFrame::OnRbarBtnRedo)
EVT_RIBBONBUTTONBAR_CLICKED(ID_NEW_CAPTURE, EditFrame::OnRBarBtnNewCap)
EVT_RIBBONBUTTONBAR_CLICKED(ID_STORE_WINDOW, EditFrame::OnRbarBtnRestoreWindow)
EVT_RIBBONTOOLBAR_CLICKED(ID_RESIZE_FRAME, EditFrame::OnRbarBtnResizeFrames)
EVT_RIBBONTOOLBAR_CLICKED(wxID_DELETE, EditFrame::OnRbarBtnDeleteFrames)
//EVT_PG_CHANGED(ID_PROPERTY, )
wxEND_EVENT_TABLE()