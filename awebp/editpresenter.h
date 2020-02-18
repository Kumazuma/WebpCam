#pragma once
#include<wx/wx.h>
#include"editframemodel.h"
class EditFramePresenter: public wxEvtHandler
{
	
	wxDECLARE_DYNAMIC_CLASS(EditFramePresenter);
private:
	EditFrameModel m_model;
protected:
	void AddHistoryItem(IHistoryItem* historyItem);
public:
	EditFramePresenter():m_model(nullptr) {}
	EditFramePresenter(wxWindow* parent, IImageStore* imageStore, const wxSize& imageSize);
	bool GetImage(size_t index, wxImage& OUT image, uint32_t& OUT duration);
	IImageStore& GetImageStore();
	size_t GetImagesCount();
	bool DeleteFrame(size_t start, size_t end);
	bool ResizeImage(const wxSize& reImageSize);
	void Undo();
	void Redo();
	const wxVector<const IHistoryItem*>& GetHistory() const;
	wxSize GetImageSize() {
		return m_model.GetImageStore()->GetImageSize();
	}
};