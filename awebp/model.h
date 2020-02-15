#pragma once
#include<wx/wx.h>
#include<wx/vector.h>
#include<wx/anidecod.h>
#include<set>
enum ModelPropertyId {
	IsRecording,
	RecordedRect,
	Fps,
	UsingTemporalFIle,
};
enum class FPS {
	_10 = 1000/10,
	_12 = 1000/12,
	_15 = 1000/15,
	_20 = 1000/20
};
wxDECLARE_EVENT(EVT_PropertyChanged, wxCommandEvent);
class Model
{
public:
	
	Model();
	~Model();
	void LinkPresenter(wxEvtHandler* handler);
	void UnlinkPresenter(wxEvtHandler* handler);
	bool IsResording() { return m_isRecording; }
	void SetRecording(bool isRecording) { m_isRecording = isRecording; UpdateNotify(ModelPropertyId::IsRecording); }
	void SetRecordedRect(const wxRect& rect){ m_recordedRect = rect; UpdateNotify(ModelPropertyId::RecordedRect); }
	wxRect GetRecordedRect() { return m_recordedRect; }
	void SetUsingTemporalFile(bool isUsingTemporalFile) { m_isUsingTemporalFile = isUsingTemporalFile; }
	bool IsUsingTemporalFile() { return m_isUsingTemporalFile; }
	FPS  GetFPS() { return m_fps; }
	void SetFPS(FPS fps);
private:
	void UpdateNotify(ModelPropertyId id);
	std::set<wxEvtHandler*> m_representers;
	FPS m_fps;
	bool m_isRecording;
	bool m_isUsingTemporalFile;
	wxRect m_recordedRect;
};
