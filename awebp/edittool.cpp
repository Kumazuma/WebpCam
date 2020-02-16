#include "wx/wxprec.h"
#include "edittool.h"

void EditDeleteFrame::Execute(IImageStore*& OUT imageStore, size_t start, size_t end, IHistoryItem*& OUT historyItem)
{
	auto builder = imageStore->CreateBuilder();
	auto tempSaveBuilder = imageStore->CreateBuilder();
	for (int i = 0;i < imageStore->GetSize(); i++)
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
	
	historyItem = new HistoryItemDeleteFrame(tempSaveBuilder->BuildStore(), start, end);
	imageStore = builder->BuildStore();

	delete tempSaveBuilder;
	delete builder;
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
	auto builder = imageStore->CreateBuilder();
	for (int i = 0; i < imageStore->GetSize(); i++)
	{
		if (i == m_start)
		{
			for (int j = 0; j < m_imageStore->GetSize(); j++)
			{
				auto item = m_imageStore->Get(i);
				builder->PushBack(item.first, item.second);
			}
		}
		auto item = m_imageStore->Get(i);
		builder->PushBack(item.first, item.second);
	}
	delete imageStore;
	imageStore = builder->BuildStore();
	delete builder;
}

void HistoryItemDeleteFrame::Redo(IImageStore*& imageStore)
{
	auto builder = imageStore->CreateBuilder();
	for (int i = 0; i < imageStore->GetSize(); i++)
	{
		auto item = imageStore->Get(i);
		if (i < m_start || m_end <= i)
		{
			builder->PushBack(item.first, item.second);
		}
	}
	delete imageStore;
	imageStore = builder->BuildStore();

	delete builder;
}
