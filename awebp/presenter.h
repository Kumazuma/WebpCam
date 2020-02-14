#pragma once
#include<wx/wx.h>
#include"model.h"
#include "interface.h"
wxDECLARE_EVENT(EVT_RefreshView, wxCommandEvent);
class AppPresenter : public wxEvtHandler
{
	wxDECLARE_DYNAMIC_CLASS(AppPresenter);
private:
	Model m_model;
public:
	AppPresenter();
	~AppPresenter();
	bool IsRecording() { return m_model.IsResording(); }
	
	void StartRecording();
	void StopRecording();
	wxRect GetRecordedRect() { return m_model.GetRecordedRect(); }
	void SetRecordedRect(const wxRect& rect) { m_model.SetRecordedRect(rect); }
	void EnableTemporalFile() { m_model.SetUsingTemporalFile(true); }
	void DisableTemporalFile() { m_model.SetUsingTemporalFile(false); }
	bool IsUsingTemporalFile() { return m_model.IsUsingTemporalFile(); }
	const IImageStore& GetImageStore() const;
	FPS  GetFPS() { return m_model.GetFPS(); }
	void SetFPS(FPS fps) { m_model.SetFPS(fps); }
protected:
	wxTimer* m_timer;
	ICapturer* m_capturer;
	//IEncoder* m_recorder;
	IImageStore* m_imageStore;
	void OnModelPropertyChanged(wxCommandEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnCompleteRecord(wxCommandEvent& event);
};
