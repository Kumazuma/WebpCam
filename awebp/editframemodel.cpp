#include "wx/wxprec.h"
#include "editframemodel.h"

EditFrameModel::EditFrameModel(IImageStore* imageStore):
	m_imageStore(imageStore)
{

}

EditFrameModel::~EditFrameModel()
{
	if (m_imageStore != nullptr)
	{
		delete m_imageStore;
	}
}
