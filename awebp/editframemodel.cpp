#include "wx/wxprec.h"
#include "editframemodel.h"

EditFrameModel::EditFrameModel(IImageStore* imageStore):
	m_imageStore(imageStore),
	m_editHistoryCursor(-1)
{

}

EditFrameModel::~EditFrameModel()
{
	if (m_imageStore != nullptr)
	{
		delete m_imageStore;
	}
	for (auto item : m_editHistory)
	{
		delete item;
	}
	m_editHistory.clear();
}
