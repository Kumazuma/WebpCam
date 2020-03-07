#pragma once
#include<wx/wx.h>
#include"interface.h"
#include<memory>
class EncoderPresenter :public wxEvtHandler
{
	wxDECLARE_DYNAMIC_CLASS(EncoderPresenter);
private:
	int m_progress = 0;
	int m_quality = 0;
	wxEvtHandler* m_view = nullptr;
	IImageStore& m_imageStore;
	std::shared_ptr<IEncoder> m_encoder;
	wxThreadHelper* m_threadHelper = nullptr;
public:
	EncoderPresenter():m_imageStore(*(IImageStore*)nullptr){}
	EncoderPresenter(wxEvtHandler* view, IImageStore& imageStore);
	
	~EncoderPresenter();
	int GetImagesCount() { return m_imageStore.GetCount(); }
	int GetEncodeProgress() { return m_progress; }

	wxString GetFileFilter();
	void SetEncoderToWebp();
	void SetEncoderToWebm();
	void SetQuality(int value);
	void SaveAnimImage(const wxString& filePath);
	void StopEncode();
	bool Encoding();
protected:
	void OnAddedAFrame(wxCommandEvent& event);
	void OnFinishEncode(wxCommandEvent& event);
};