#pragma once
#include<wx/wx.h>
#include"editframemodel.h"
#include <optional>
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
	std::optional<uint32_t> GetFrameDuration(size_t index);
	IImageStore& GetImageStore();
	size_t GetImagesCount();
	size_t GetHistoryCursor();
	bool DeleteFrame(size_t start, size_t end);
	bool ResizeImage(const wxSize& reImageSize);
	bool CropImage();
	wxRect GetCropRect();
	void SetCropRect(const wxRect& rc);
	void Undo();
	void Redo();
	const wxVector<const IHistoryItem*>& GetHistory() const;
	wxSize GetImageSize() {
		return m_model.GetImageStore()->GetImageSize();
	}
};