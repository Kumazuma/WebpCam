#include "wx/wxprec.h"
#include "editpresenter.h"
#include "edittool.h"
#include "event.h"

EditFramePresenter::EditFramePresenter(wxWindow* parent, IImageStore* imageStore, const wxSize& imageSize):
	m_model(imageStore)
{
	this->SetNextHandler(parent);
	m_model.SetCropRect(m_model.GetImageStore()->GetImageSize());
}

bool EditFramePresenter::GetImage(size_t index, wxImage& OUT image, uint32_t& OUT duration)
{
	auto imageStore = m_model.GetImageStore();
	if (index < imageStore->GetCount())
	{
		auto item = imageStore->Get(index);
		image = item.first;
		duration = item.second;
		return true;
	}
	return false;
}

std::optional<uint32_t> EditFramePresenter::GetFrameDuration(size_t index)
{
	return m_model.GetImageStore()->GetFrameDuration(index);
}

IImageStore& EditFramePresenter::GetImageStore()
{
	return *m_model.GetImageStore();
}

size_t EditFramePresenter::GetImagesCount()
{
	auto& imageStore = m_model.GetImageStore();
	return imageStore->GetCount();
}
size_t EditFramePresenter::GetHistoryCursor()
{
	return m_model.GetEditHistoryCursor();
}
void EditFramePresenter::AddHistoryItem(IHistoryItem* historyItem)
{
	//커서가 현재 히스토리의 전체 사이즈보다 작다면, 현재 커서 이후의 로그는 전부 날려야 한다.
	int cursor = m_model.GetEditHistoryCursor() + 1;
	size_t count = m_model.GetEditHistory().size();
	auto& history = m_model.GetEditHistory();
	if (cursor < count)
	{
		auto start = history.begin() + cursor;
		for (auto it = start; it != history.end(); it++)
		{
			delete* it;
		}
		history.erase(start, history.end());
	}
	m_model.SetEditHistoryCursor(cursor);
	history.push_back(historyItem);
}
bool EditFramePresenter::DeleteFrame(size_t start, size_t end)
{
	EditDeleteFrameTool deleteTool;
	IHistoryItem* historyItem = nullptr;
	deleteTool.Execute(m_model.GetImageStore(), start, end, historyItem);
	bool res = historyItem != nullptr;
	if (res)
	{
		AddHistoryItem(historyItem);
	}

	this->QueueEvent(new wxCommandEvent(EVT_RefreshView));
	return res;
}

bool EditFramePresenter::ResizeImage(const wxSize& reImageSize)
{
	EditResizeTool resizeTool(reImageSize);
	IHistoryItem* historyItem = nullptr;
	resizeTool.Execute(m_model.GetImageStore(), 0, 0, historyItem);
	bool res = historyItem != nullptr;
	if (res)
	{
		AddHistoryItem(historyItem);
	}

	this->QueueEvent(new wxCommandEvent(EVT_RefreshView));
	return res;
}

bool EditFramePresenter::CropImage()
{
	wxRect cropArea = m_model.GetCropRect();
	if (cropArea.GetSize() == m_model.GetImageStore()->GetImageSize())
	{
		return false;
	}
	EditCropImageTool resizeTool(cropArea);
	IHistoryItem* historyItem = nullptr;
	resizeTool.Execute(m_model.GetImageStore(), 0, 0, historyItem);
	bool res = historyItem != nullptr;
	if (res)
	{
		AddHistoryItem(historyItem);
	}
	m_model.SetCropRect(m_model.GetImageStore()->GetImageSize());
	this->QueueEvent(new wxCommandEvent(EVT_RefreshView));
	return false;
}

wxRect EditFramePresenter::GetCropRect()
{
	auto rect = m_model.GetCropRect();
	auto imageSize = m_model.GetImageStore()->GetImageSize();
	if (rect.x < 0)
	{
		rect.width -= rect.x;
		rect.x = 0;
	}
	if (rect.y < 0)
	{
		rect.height -= rect.y;
		rect.y = 0;
	}
	if (rect.GetWidth() > imageSize.GetWidth())
	{
		rect.SetWidth(imageSize.GetWidth());
	}
	if (rect.GetHeight() > imageSize.GetHeight())
	{
		rect.SetHeight(imageSize.GetHeight());
	}
	m_model.SetCropRect(rect);
	return rect;
}

void EditFramePresenter::SetCropRect(const wxRect& temp)
{
	auto imgSize = m_model.GetImageStore()->GetImageSize();
	if (temp.x >= 0 &&
		temp.y >= 0 &&
		temp.width >= 10 &&
		temp.height >= 10 &&
		(temp.width + temp.x) <= imgSize.GetWidth() &&
		(temp.height + temp.y) <= imgSize.GetHeight())
	{
		m_model.SetCropRect(temp);
	}
	else
	{
		m_model.SetCropRect(m_model.GetCropRect());
	}
	
}

void EditFramePresenter::SetSelectFrameIndex( size_t index)
{
	auto store = m_model.GetImageStore();
	
	if (store->GetCount() == 0)
	{
		m_model.SetSelectFrameIndex(std::nullopt);
	}
	else if (index >= store->GetCount())
	{
		index = store->GetCount() - 1;
	}
	m_model.SetSelectFrameIndex(index);
	wxCommandEvent* e = new wxCommandEvent(EVT_RefreshView);
	e->SetEventObject(this);
	this->QueueEvent(e);
}

std::optional<size_t> EditFramePresenter::GetSelectFrameIndex()
{
	auto index = m_model.GetSelectFrameIndex();
	if (!index)
	{
		return index;
	}
	if (*index >= m_model.GetImageStore()->GetCount())
	{
		m_model.SetSelectFrameIndex(std::nullopt);
		index = std::nullopt;
	}
	return index;
}

void EditFramePresenter::Undo()
{
	auto cursor =  m_model.GetEditHistoryCursor();
	if (cursor != -1)
	{
		auto& history = m_model.GetEditHistory();
		auto item = history[cursor];
		item->Undo(m_model.GetImageStore());
		m_model.SetEditHistoryCursor(cursor - 1);
		this->QueueEvent(new wxCommandEvent(EVT_RefreshView));
	}
}

void EditFramePresenter::Redo()
{
	auto& history = m_model.GetEditHistory();
	auto cursor = m_model.GetEditHistoryCursor() + 1;
	if (cursor < history.size())
	{
		auto item = history[cursor];
		item->Redo(m_model.GetImageStore());
		m_model.SetEditHistoryCursor(cursor);
		this->QueueEvent(new wxCommandEvent(EVT_RefreshView));
	}
}

const wxVector<const IHistoryItem*>& EditFramePresenter::GetHistory() const
{
	return (const wxVector<const IHistoryItem*>&) m_model.GetEditHistory();
}

wxIMPLEMENT_DYNAMIC_CLASS(EditFramePresenter, wxEvtHandler)