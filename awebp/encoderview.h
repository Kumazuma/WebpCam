#pragma once
#include <wx/wx.h>
#include "EncorderPresenter.h"
#include "interface.h"
class EncoderFrame : public wxFrame
{
public:
	EncoderFrame(IImageStore * store);
	~EncoderFrame();
private:
	EncoderPresenter* m_presenter;
};