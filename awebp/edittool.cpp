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
	auto* temp = imageStore;
	imageStore = IImageStoreBuilder::BuildStore(tempSaveBuilder);
	historyItem = new HistoryItemDeleteFrame(temp, start, end);
}

HistoryItemDeleteFrame::HistoryItemDeleteFrame(IImageStore* imageStore, size_t start, size_t end):
	m_imageStore(imageStore), m_start(start), m_end(end)

{
	m_imageStore->UnloadFromMemory();
}

HistoryItemDeleteFrame::~HistoryItemDeleteFrame()
{
	delete m_imageStore;
}

void HistoryItemDeleteFrame::Undo(IImageStore*& imageStore)
{
	imageStore->UnloadFromMemory();
	m_imageStore->LoadToMemory();
	auto* store = imageStore;
	imageStore = m_imageStore;
	m_imageStore = store;
}

void HistoryItemDeleteFrame::Redo(IImageStore*& imageStore)
{
	imageStore->UnloadFromMemory();
	m_imageStore->LoadToMemory();
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
	auto* temp = imageStore;
	imageStore = IImageStoreBuilder::BuildStore(builder);
	historyItem = new HistoryItemResize(temp, m_sizeResizeTo);
}

HistoryItemResize::HistoryItemResize(IImageStore* imageStore, const wxSize& resizeTo):
	m_imageStore(imageStore), m_sizeResizeTo(resizeTo)
{
	m_imageStore->UnloadFromMemory();
}

HistoryItemResize::~HistoryItemResize()
{
	delete m_imageStore;
}

void HistoryItemResize::Undo(IImageStore*& imageStore)
{
	imageStore->UnloadFromMemory();
	m_imageStore->LoadToMemory();
	auto temp = imageStore;
	imageStore = m_imageStore;
	m_imageStore = temp;
}

void HistoryItemResize::Redo(IImageStore*& imageStore)
{
	imageStore->UnloadFromMemory();
	m_imageStore->LoadToMemory();
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
	auto* temp = imageStore;
	imageStore = IImageStoreBuilder::BuildStore(builder);
	historyItem = new HistoryCropImage(temp, m_crop);
	

}

HistoryCropImage::HistoryCropImage(IImageStore* imageStore, const wxRect& crop):
	m_imageStore(imageStore), m_crop(crop)
{
	m_imageStore->UnloadFromMemory();
}

HistoryCropImage::~HistoryCropImage()
{
	delete m_imageStore;
}

void HistoryCropImage::Undo(IImageStore*& imageStore)
{
	imageStore->UnloadFromMemory();
	m_imageStore->LoadToMemory();
	std::swap(imageStore, m_imageStore);
}

void HistoryCropImage::Redo(IImageStore*& imageStore)
{
	imageStore->UnloadFromMemory();
	m_imageStore->LoadToMemory();
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
