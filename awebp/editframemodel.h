#pragma once
#include<wx/wx.h>
#include "interface.h"
class EditFrameModel
{
public:
	EditFrameModel(IImageStore* imageStore);
	~EditFrameModel();
	IImageStore*& GetImageStore() { return m_imageStore; }
private:
	IImageStore* m_imageStore;
};