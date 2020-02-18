#include "wx/wxprec.h"
#include "edittool.h"
#include "webp/decode.h"
void EditDeleteFrameTool::Execute(IImageStore*& OUT imageStore, size_t start, size_t end, IHistoryItem*& OUT historyItem)
{
	if (imageStore->IsSupportedRemoveImages())
	{
		auto newStore = imageStore->RemoveImages(start, end);
		historyItem = new HistoryItemDeleteFrame(newStore, start, end);
		return;
	}
	auto builder = imageStore->CreateBuilder(imageStore->GetImageSize());
	auto tempSaveBuilder = imageStore->CreateBuilder(imageStore->GetImageSize());
	for (int i = 0;i < imageStore->GetCount(); i++)
	{
		auto item = imageStore->Get(i);
		auto store = tempSaveBuilder;
		if (i < start || end <= i)
		{
			store = builder;
			//builder->PushBack(item.first, item.second);
		}
		store->PushBack(item.first, item.second);
	}
	delete imageStore;
	
	historyItem = new HistoryItemDeleteFrame(IImageStoreBuilder::BuildStore(tempSaveBuilder), start, end);
	imageStore = IImageStoreBuilder::BuildStore(builder);
}

HistoryItemDeleteFrame::HistoryItemDeleteFrame(IImageStore* imageStore, size_t start, size_t end):
	m_imageStore(imageStore), m_start(start), m_end(end)

{

}

HistoryItemDeleteFrame::~HistoryItemDeleteFrame()
{
	delete m_imageStore;
}

void HistoryItemDeleteFrame::Undo(IImageStore*& imageStore)
{
	auto builder = imageStore->CreateBuilder(imageStore->GetImageSize());
	int j = 0;
	if (imageStore->GetCount() == 0)
	{
		for (j = 0; j < m_imageStore->GetCount(); j++)
		{
			auto item = m_imageStore->Get(j);
			builder->PushBack(item.first, item.second);
		}
	}
	for (int i = 0; i < imageStore->GetCount(); i++)
	{
		if (i == m_start)
		{
			for (j = 0; j < m_imageStore->GetCount(); j++)
			{
				auto item = m_imageStore->Get(j);
				builder->PushBack(item.first, item.second);
			}
		}
		auto item = imageStore->Get(i);
		builder->PushBack(item.first, item.second);
	}
	
	delete imageStore;
	imageStore =IImageStoreBuilder::BuildStore(builder);
}

void HistoryItemDeleteFrame::Redo(IImageStore*& imageStore)
{
	auto builder = imageStore->CreateBuilder(imageStore->GetImageSize());
	for (int i = 0; i < imageStore->GetCount(); i++)
	{
		auto item = imageStore->Get(i);
		if (i < m_start || m_end <= i)
		{
			builder->PushBack(item.first, item.second);
		}
	}
	delete imageStore;
	imageStore =IImageStoreBuilder::BuildStore(builder);
}

wxString HistoryItemDeleteFrame::GetDescription() const
{
	wxString description;
	description << wxT("프레임 삭제(갯수:") << m_imageStore->GetCount() << wxT(")");
	return description;
}

EditResizeTool::EditResizeTool(const wxSize& to):
	m_sizeResizeTo(to)
{

}
void EditResizeTool::Execute(IImageStore*& OUT imageStore, size_t start, size_t end, IHistoryItem*& OUT historyItem)
{
	auto builder = imageStore->CreateBuilder(m_sizeResizeTo);
	for (int i = 0; i < imageStore->GetCount(); i++)
	{
		
		auto item = imageStore->Get(i);
		item.first.Rescale(m_sizeResizeTo.GetWidth(), m_sizeResizeTo.GetHeight());
		builder->PushBack(item.first, item.second);
	}
	historyItem = new HistoryItemResize(imageStore, m_sizeResizeTo);
	imageStore = IImageStoreBuilder::BuildStore(builder);
}

HistoryItemResize::HistoryItemResize(IImageStore* imageStore, const wxSize& resizeTo):
	m_imageStore(imageStore), m_sizeResizeTo(resizeTo)
{

}

HistoryItemResize::~HistoryItemResize()
{
	delete m_imageStore;
}

void HistoryItemResize::Undo(IImageStore*& imageStore)
{
	auto temp = imageStore;
	imageStore = m_imageStore;
	m_imageStore = temp;
}

void HistoryItemResize::Redo(IImageStore*& imageStore)
{
	auto temp = imageStore;
	imageStore = m_imageStore;
	m_imageStore = temp;
}

wxString HistoryItemResize::GetDescription() const
{
	wxString description;
	wxImage image = m_imageStore->Get(0).first;
	description << wxT("이미지 리사이즈(") << m_sizeResizeTo.GetWidth()<<wxT(", ")<< m_sizeResizeTo.GetHeight() << wxT(")");
	return description;
}

