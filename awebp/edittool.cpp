#include "wx/wxprec.h"
#include "edittool.h"
#include "webp/decode.h"
void EditDeleteFrameTool::Execute(IImageStore*&  imageStore, size_t start, size_t end, IHistoryItem*& OUT historyItem)
{
	auto tempSaveBuilder = imageStore->CreateBuilder(imageStore->GetImageSize());
	for (int i = 0; i < imageStore->GetCount(); i++)
	{
		if (i < start || end <= i)
		{
			auto item = imageStore->Get(i);
			tempSaveBuilder->PushBack(item.first, item.second);
		}
	}
	
	historyItem = new HistoryItemDeleteFrame(imageStore, start, end);
	imageStore = IImageStoreBuilder::BuildStore(tempSaveBuilder);
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
	auto* store = imageStore;
	imageStore = m_imageStore;
	m_imageStore = store;
}

void HistoryItemDeleteFrame::Redo(IImageStore*& imageStore)
{
	auto* store = imageStore;
	imageStore = m_imageStore;
	m_imageStore = store;
}

wxString HistoryItemDeleteFrame::GetDescription() const
{
	wxString description;
	description << wxT("프레임 삭제(갯수:") << (m_end - m_start) << wxT(")");
	return description;
}

EditResizeTool::EditResizeTool(const wxSize& to):
	m_sizeResizeTo(to)
{

}
#include<future>
void EditResizeTool::Execute(
	IImageStore*& OUT imageStore, 
	size_t start, size_t end, 
	IHistoryItem*& OUT historyItem)
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
	description << wxT("이미지 리사이즈(") << m_sizeResizeTo.GetWidth()<<wxT(", ")<< m_sizeResizeTo.GetHeight() << wxT(")");
	return description;
}

void EditCropImageTool::Execute(IImageStore*& OUT imageStore, size_t start, size_t end, IHistoryItem*& OUT historyItem)
{
	auto builder = imageStore->CreateBuilder(m_crop.GetSize());
	for (int i = 0; i < imageStore->GetCount(); i++)
	{

		auto item = imageStore->Get(i);
		auto croped = item.first.GetSubImage(m_crop);
		builder->PushBack(croped, item.second);
	}
	historyItem = new HistoryCropImage(imageStore, m_crop);
	imageStore = IImageStoreBuilder::BuildStore(builder);

}

HistoryCropImage::HistoryCropImage(IImageStore* imageStore, const wxRect& crop):
	m_imageStore(imageStore), m_crop(crop)
{

}

HistoryCropImage::~HistoryCropImage()
{
	delete m_imageStore;
}

void HistoryCropImage::Undo(IImageStore*& imageStore)
{
	std::swap(imageStore, m_imageStore);
}

void HistoryCropImage::Redo(IImageStore*& imageStore)
{
	std::swap(m_imageStore, imageStore);
}

wxString HistoryCropImage::GetDescription() const
{
	wxString description;
	description << wxT("잘라내기(") <<
		m_crop.x << wxT(", ") << m_crop.y << wxT(", ")<<
		m_crop.width<< wxT(", ") << m_crop.height <<
		wxT(")");
	return description;
}
