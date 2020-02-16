#pragma once
#include<wx/wx.h>
#include"editframemodel.h"
class EditFramePresenter: public wxEvtHandler
{
	
	wxDECLARE_DYNAMIC_CLASS(EditFramePresenter);
public:
	EditFramePresenter():m_model(nullptr) {}
	EditFramePresenter(IImageStore* imageStore);
	bool GetImage(size_t index, wxImage& OUT image, uint32_t& OUT duration);
	IImageStore* MoveImageStore();
	size_t GetImagesCount();
private:
	EditFrameModel m_model;
};