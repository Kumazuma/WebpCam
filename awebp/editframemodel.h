#pragma once
#include<wx/wx.h>
#include "interface.h"
#include <optional>
class EditFrameModel
{
private:
	IImageStore* m_imageStore;
	wxVector<IHistoryItem*> m_editHistory;
	wxRect m_cropRect;
	std::optional<size_t> m_selectFrameIndex;
	int m_editHistoryCursor;
public:
	EditFrameModel(IImageStore* imageStore);
	~EditFrameModel();
	IImageStore*& GetImageStore() { return m_imageStore; }
	wxVector<IHistoryItem*>& GetEditHistory() {
		return m_editHistory;
	};
	const wxVector<IHistoryItem*>& GetEditHistory() const {
		return m_editHistory;
	};
	int GetEditHistoryCursor() { return m_editHistoryCursor; }
	void SetEditHistoryCursor(int val) { m_editHistoryCursor = val; }
	void SetCropRect(const wxRect& rc) { m_cropRect = rc; }
	wxRect GetCropRect() { return m_cropRect; }
	std::optional<size_t> GetSelectFrameIndex()const { return m_selectFrameIndex; }
	void SetSelectFrameIndex(const std::optional<size_t> value) { m_selectFrameIndex = value; }
};