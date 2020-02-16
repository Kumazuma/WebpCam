#include "wx/wxprec.h"
#include "editpresenter.h"

EditFramePresenter::EditFramePresenter(IImageStore* imageStore):
	m_model(imageStore)
{
	
}

wxIMPLEMENT_DYNAMIC_CLASS(EditFramePresenter, wxEvtHandler)