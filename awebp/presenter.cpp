#include "wx/wxprec.h"
#include "presenter.h"
#include <wx/dc.h>
#include "webpencoder.h"
#include "GDICapturer.h"
#include "temporalfileimagestore.h"
#include "memoryimagestore.h"
wxDEFINE_EVENT(EVT_RefreshView, wxCommandEvent);
wxIMPLEMENT_DYNAMIC_CLASS(AppPresenter, wxEvtHandler);
AppPresenter::AppPresenter(wxEvtHandler * view):
	wxEvtHandler(),
	m_timer(new wxTimer(this)),
	m_capturer(nullptr),
	m_imageStore(nullptr),
	m_view(view)
{
	m_model.LinkPresenter(this);
	this->Bind(EVT_PropertyChanged, &AppPresenter::OnModelPropertyChanged, this,ModelPropertyId::IsRecording);
	this->Bind(EVT_PropertyChanged, &AppPresenter::OnModelPropertyChanged, this, ModelPropertyId::RecordedRect);
	this->Bind(wxEVT_TIMER, &AppPresenter::OnTimer, this);
	this->Bind(EVT_CompleteRecord, &AppPresenter::OnCompleteRecord, this);
	//m_timer->Bind(wxEVT_TIMER, &AppPresenter::OnTimer, this);
}

AppPresenter::~AppPresenter()
{
	m_model.UnlinkPresenter(this);
	if (m_timer != nullptr)
	{
		m_timer->Stop();
		delete m_timer;
	}
	if (m_capturer != nullptr)
	{
		delete m_capturer;
		m_capturer = nullptr;
	}
	if (m_imageStore != nullptr)
	{
		delete m_imageStore;
		m_imageStore = nullptr;
	}
	
}

void AppPresenter::StartRecording()
{
	if (m_model.IsResording())
	{
		return;
	}
	m_model.SetRecording(true);
	auto recordedRect = m_model.GetRecordedRect();
	auto size = recordedRect.GetSize();
	m_capturer = new GDICapturer();
	if (m_model.IsUsingTemporalFile())
		m_imageStore = new FileImageStore();
	else
		m_imageStore = new MemoryImageStore();
	m_capturer->BeginCapture(this, recordedRect, (int)m_model.GetFPS());
	m_timer->Start((int)m_model.GetFPS()); 
}	

void AppPresenter::StopRecording()
{
	m_timer->Stop();
	m_capturer->EndCapture();
	m_model.SetRecording(false);
	delete m_capturer;
	m_capturer = nullptr;
}

const IImageStore& AppPresenter::GetImageStore() const
{
	return *m_imageStore;
}

void AppPresenter::StartEncodeAndSave()
{

}

void AppPresenter::OnModelPropertyChanged(wxCommandEvent& event)
{
	m_view->QueueEvent(new wxCommandEvent(EVT_RefreshView));
}

void AppPresenter::OnTimer(wxTimerEvent& event)
{
	auto frame = m_capturer->CaptureFrame();
	m_imageStore->Add(frame.first, frame.second);
}

void AppPresenter::OnCompleteRecord(wxCommandEvent& event)
{
	
}
