#include "wx/wxprec.h"
#include "presenter.h"
#include <wx/dc.h>
#include "webpencoder.h"
#include "GDICapturer.h"
#include "temporalfileimagestore.h"
#include "memoryimagestore.h"
wxDEFINE_EVENT(EVT_RefreshView, wxCommandEvent);
wxIMPLEMENT_DYNAMIC_CLASS(AppPresenter, wxEvtHandler);

bool isStop = false;
class CaptureThread : public wxThread
{
public:
	ICapturer* m_capturer = nullptr;
	IImageStoreBuilder* m_imageStoreBuilder = nullptr;
	FPS m_fps;
protected:
	void* Entry() override
	{
		isStop = false;
		auto pre = wxGetLocalTimeMillis();
		do
		{
			auto now = wxGetLocalTimeMillis();
			auto delta = (now - pre).GetValue();
			if (delta >= (int)m_fps)
			{
				if (isStop == false)
				{
					auto frame = m_capturer->CaptureFrame();
					m_imageStoreBuilder->PushBack(frame, delta);
					pre = now;
				}
			}
		} while (isStop == false);
		
		delete m_capturer;
		return nullptr;
	}
};
AppPresenter::AppPresenter(wxEvtHandler * view):
	wxEvtHandler(),
	m_timer(new wxTimer(this)),
	m_capturer(nullptr),
	m_imageStoreBuilder(nullptr),
	m_view(view)
{
	m_model.LinkPresenter(this);
	this->Bind(EVT_PropertyChanged, &AppPresenter::OnModelPropertyChanged, this,ModelPropertyId::IsRecording);
	this->Bind(EVT_PropertyChanged, &AppPresenter::OnModelPropertyChanged, this, ModelPropertyId::RecordedRect);
	this->Bind(wxEVT_TIMER, &AppPresenter::OnTimer, this);
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
	if (m_imageStoreBuilder != nullptr)
	{
		delete m_imageStoreBuilder;
		m_imageStoreBuilder = nullptr;
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
	{
		m_imageStoreBuilder = new FileImageStoreBuilder();
	}	
	else
	{
		//TODO: Not yet implement;
		//m_imageStore = new MemoryImageStore();
	}
	m_capturer->BeginCapture(this, recordedRect, (int)m_model.GetFPS());
	//m_timer->Start((int)m_model.GetFPS()); 
	auto* thread = new CaptureThread();
	thread->m_capturer = m_capturer;
	thread->m_fps = m_model.GetFPS();
	thread->m_imageStoreBuilder = m_imageStoreBuilder;
	thread->Run();
}	

void AppPresenter::StopRecording()
{
	isStop = true;
	m_capturer->EndCapture();
	m_model.SetRecording(false);
	m_capturer = nullptr;
}

IImageStore* AppPresenter::BuildImageStore()
{
	auto store = m_imageStoreBuilder->BuildStore();
	delete m_imageStoreBuilder;
	return store;
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
	
}

void AppPresenter::OnCompleteRecord(wxCommandEvent& event)
{
	
}
