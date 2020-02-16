#pragma once
#include "interface.h"
class HistoryItemDeleteFrame : public IHistoryItem
{
public:
	HistoryItemDeleteFrame(IImageStore* imageStore, size_t start, size_t end);
	~HistoryItemDeleteFrame();
	virtual void Undo(IImageStore*& imageStore);
	virtual void Redo(IImageStore*& imageStore);
private:
	IImageStore* m_imageStore;
	size_t m_start;
	size_t m_end;
};
class EditDeleteFrame : public IEditTool
{
public:
	void Execute(IImageStore*& OUT imageStore, size_t start, size_t end, IHistoryItem*& OUT historyItem) override;
};

class EditSetDurationFrame : public IEditTool
{
public:
};
