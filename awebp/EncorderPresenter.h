#pragma once
#include<wx/wx.h>
#include"interface.h"
enum class EncodeType {
	Webp
};
class EncoderPresenter :public wxEvtHandler
{
	wxDECLARE_DYNAMIC_CLASS(EncoderPresenter);
private:
	int m_progress = 0;
	wxEvtHandler* m_view = nullptr;
	IImageStore& m_imageStore;
	IEncoder* m_encoder = nullptr;
	wxThreadHelper* m_threadHelper = nullptr;
public:
	EncoderPresenter():m_imageStore(*(IImageStore*)nullptr){}
	EncoderPresenter(wxEvtHandler* view, IImageStore& imageStore);
	
	~EncoderPresenter();
	int GetImagesCount() { return m_imageStore.GetCount(); }
	int GetEncodeProgress() { return m_progress; }
	void SetFileFormat(EncodeType type);
	wxString GetFileFilter();
	
	void SaveAnimImage(const wxString& filePath);
	void StopEncode();
protected:
	void OnAddedAFrame(wxCommandEvent& event);
	void OnFinishEncode(wxCommandEvent& event);
};