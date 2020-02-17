#pragma once
#include<wx/wx.h>
#include "interface.h"
class EditFrameModel
{
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

private:
	IImageStore* m_imageStore;
	wxVector<IHistoryItem*> m_editHistory;
	int m_editHistoryCursor;
};