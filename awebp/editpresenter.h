#pragma once
#include<wx/wx.h>
#include"editframemodel.h"
class EditFramePresenter: public wxEvtHandler
{
	
	wxDECLARE_DYNAMIC_CLASS(EditFramePresenter);
public:
	EditFramePresenter():m_model(nullptr) {}
	EditFramePresenter(wxWindow* parent, IImageStore* imageStore);
	bool GetImage(size_t index, wxImage& OUT image, uint32_t& OUT duration);
	IImageStore& GetImageStore();
	size_t GetImagesCount();
	bool DeleteFrams(size_t start, size_t end);
	void Undo();
	void Redo();
	const wxVector<const IHistoryItem*>& GetHistory() const;
private:
	EditFrameModel m_model;
};