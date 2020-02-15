#pragma once
#include<wx/wx.h>
#include"interface.h"
enum class EncodeType {
	Webp
};
class EncoderPresenter :public wxEvtHandler
{
	wxDECLARE_DYNAMIC_CLASS(EncoderPresenter);
public:
	EncoderPresenter() {}
	EncoderPresenter(wxEvtHandler* view, IImageStore* imageStore);
	~EncoderPresenter();
	void SetFileFormat(EncodeType type);
	wxString GetFileFilter();
	void SaveAnimImage(const wxString& filePath);
private:
	wxEvtHandler* m_view = nullptr;
	IImageStore* m_imageStore = nullptr;
	IEncoder* m_encoder = nullptr;
};