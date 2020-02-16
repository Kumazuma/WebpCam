#include "wx/wxprec.h"
#include "editpresenter.h"

EditFramePresenter::EditFramePresenter(IImageStore* imageStore):
	m_model(imageStore)
{
	
}

bool EditFramePresenter::GetImage(size_t index, wxImage& OUT image, uint32_t& OUT duration)
{
	auto imageStore = m_model.GetImageStore();
	if (index < imageStore->GetSize())
	{
		auto item = imageStore->Get(index);
		image = item.first;
		duration = item.second;
		return true;
	}
	return false;
}

IImageStore* EditFramePresenter::MoveImageStore()
{
	auto& imageStore = m_model.GetImageStore();
	auto* temp = imageStore;
	imageStore = nullptr;
	return temp;
}

size_t EditFramePresenter::GetImagesCount()
{
	auto& imageStore = m_model.GetImageStore();
	return imageStore->GetSize();
}

wxIMPLEMENT_DYNAMIC_CLASS(EditFramePresenter, wxEvtHandler)