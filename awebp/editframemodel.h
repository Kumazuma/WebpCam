#pragma once
#include<wx/wx.h>
#include "interface.h"
class EditFrameModel
{
public:
	EditFrameModel(IImageStore* imageStore);
private:
	IImageStore* m_imageStore;
};