﻿#include "wx/wxprec.h"
#include "editpresenter.h"
#include "edittool.h"
#include "event.h"

EditFramePresenter::EditFramePresenter(wxWindow* parent, IImageStore* imageStore, const wxSize& imageSize):
	m_model(imageStore)
{
	this->SetNextHandler(parent);
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