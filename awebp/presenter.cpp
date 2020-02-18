#include "wx/wxprec.h"
#include "presenter.h"
#include <wx/dc.h>
#include "webpencoder.h"
#include "GDICapturer.h"
#include "temporalfileimagestore.h"
#include "memoryimagestore.h"
#include "event.h"
wxIMPLEMENT_DYNAMIC_CLASS(AppPresenter, wxEvtHandler);

bool isStop = false;
class CaptureThread : public wxThreadHelper
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
			if (delta >= (long long)m_fps)
			{
				if (isStop == false)
				{
					auto frame = m_capturer->CaptureFrame();
					m_imageStoreBuilder->PushBack(frame, delta);
					pre = now;
				}
				else
				{
					break;
				}
			}
			else if(((int64_t)m_fps - delta) >= 10)
			{
				wxThread::Sleep(10);
			}
		} while (isStop == false);
		
		
		return nullptr;
	}
};
AppPresenter::AppPresenter(wxEvtHandler * view):
	wxEvtHandler(),
	m_capturer(nullptr),
	m_imageStoreBuilder(nullptr),
	m_captureThreadHelper(nullptr),
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
	if (m_captureThreadHelper != nullptr)
	{
		if (m_captureThreadHelper->GetThread() != nullptr)
		{
			m_captureThreadHelper->GetThread()->Wait();
		}
		delete m_captureThreadHelper;
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
		m_imageStoreBuilder = new FileImageStoreBuilder(size);
	}	
	else
	{
		m_imageStoreBuilder = new FileImageStoreBuilder(size);
	}
	m_capturer->BeginCapture(this, recordedRect, (int)m_model.GetFPS());
	//m_timer->Start((int)m_model.GetFPS()); 
	auto* thread = new CaptureThread();
	thread->m_capturer = m_capturer;
	thread->m_fps = m_model.GetFPS();
	thread->m_imageStoreBuilder = m_imageStoreBuilder;
	m_captureThreadHelper = thread;
	thread->CreateThread();
	thread->GetThread()->Run();
}	

void AppPresenter::StopRecording()
{
	isStop = true;
	if (m_captureThreadHelper->GetThread() != nullptr)
	{
		m_captureThreadHelper->GetThread()->Wait();
	}
	
	m_model.SetRecording(false);
	m_capturer->EndCapture();
	delete m_capturer;
	delete m_captureThreadHelper;
	m_capturer = nullptr;
	m_captureThreadHelper = nullptr;
}

IImageStore* AppPresenter::BuildImageStore()
{
	return IImageStoreBuilder::BuildStore(m_imageStoreBuilder);
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
