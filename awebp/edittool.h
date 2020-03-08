#pragma once
#include "interface.h"
class HistoryItemDeleteFrame : public IHistoryItem
{
public:
	HistoryItemDeleteFrame(IImageStore* imageStore, size_t start, size_t end);
	~HistoryItemDeleteFrame();
	virtual void Undo(IImageStore*& imageStore) override;
	virtual void Redo(IImageStore*& imageStore) override;
	virtual wxString GetDescription() const override;
private:
	IImageStore* m_imageStore;
	size_t m_start;
	size_t m_end;
};
class EditDeleteFrameTool : public IEditTool
{
public:
	void Execute(IImageStore*& imageStore, size_t start, size_t end, IHistoryItem*& OUT historyItem) override;
};
class EditResizeTool : public IEditTool
{
public:
	EditResizeTool(const wxSize& to);
	void Execute(IImageStore*& imageStore, size_t start, size_t end, IHistoryItem*& OUT historyItem) override;
private:
	wxSize m_sizeResizeTo;
};
class HistoryItemResize : public IHistoryItem
{
private:
	IImageStore* m_imageStore;
	wxSize m_sizeResizeTo;
public:
	HistoryItemResize(IImageStore* imageStore, const wxSize& resizeTo);
	~HistoryItemResize();
	virtual void Undo(IImageStore*& imageStore) override;
	virtual void Redo(IImageStore*& imageStore) override;
	virtual wxString GetDescription() const override;
};
class HistoryCropImage : public IHistoryItem
{
private:
	IImageStore* m_imageStore;
	wxRect m_crop;
public:
	HistoryCropImage(IImageStore* imageStore, const wxRect& crop);
	~HistoryCropImage();
	virtual void Undo(IImageStore*& imageStore) override;
	virtual void Redo(IImageStore*& imageStore) override;
	virtual wxString GetDescription() const override;
};
class EditCropImageTool : public IEditTool
{
private:
	wxRect m_crop;
public:
	EditCropImageTool(const wxRect& cropRect) : m_crop(cropRect) {}
	void Execute(IImageStore*& OUT imageStore, size_t start, size_t end, IHistoryItem*& OUT historyItem) override;
};
class EditSetDurationFrame : public IEditTool
{
public:
};
