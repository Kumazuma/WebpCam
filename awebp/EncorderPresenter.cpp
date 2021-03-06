﻿#include "wx/wxprec.h"
#include "EncorderPresenter.h"
#include "webmencoder.h"
#include "webpencoder.h"
#include "event.h"
wxIMPLEMENT_DYNAMIC_CLASS(EncoderPresenter, wxEvtHandler);
EncoderPresenter::EncoderPresenter(wxEvtHandler* view, IImageStore& imageStore):
	wxEvtHandler(), m_view(view), m_imageStore(imageStore), m_encoder(new WebpEncoder()), m_threadHelper(nullptr)
{
	this->SetNextHandler(view);
	this->Bind(EVT_ADDED_A_FRAME, &EncoderPresenter::OnAddedAFrame, this);
	this->Bind(EVT_FINISH_ENCODE, &EncoderPresenter::OnFinishEncode, this);
}

EncoderPresenter::~EncoderPresenter()
{
	if (m_threadHelper != nullptr)
	{
		auto thread = m_threadHelper->GetThread();
		thread->Wait();
		delete m_threadHelper;
		m_threadHelper = nullptr;
	}
}

wxString EncoderPresenter::GetFileFilter()
{
	return m_encoder->GetFileFilter();
}

void EncoderPresenter::SetEncoderToWebp()
{
	
	m_encoder.reset(new WebpEncoder());

}

void EncoderPresenter::SetEncoderToWebm()
{
	m_encoder.reset(new WebmEncoder());
}

void EncoderPresenter::SetQuality(int value)
{
	m_quality = value;
}

class EncodeThread :public wxThreadHelper
{
public:
	EncodeThread(wxEvtHandler* handler,
		const wxString& filePath,
		std::shared_ptr<IEncoder>& encoder,
		IImageStore& store):
		m_encoder(encoder),
		m_store(store),
		m_filePath(filePath),
		m_handler(handler)
	{

	}
protected:
	void* Entry() override;
private:
	std::shared_ptr<IEncoder> m_encoder;
	IImageStore& m_store;
	wxString m_filePath;
	wxEvtHandler* m_handler;
};

void EncoderPresenter::SaveAnimImage(const wxString& filePath)
{
	m_encoder->SetQuality(m_quality);
	m_threadHelper = new EncodeThread(this, filePath, m_encoder, m_imageStore);
	m_threadHelper->CreateThread();
	m_threadHelper->GetThread()->Run();
}

void EncoderPresenter::StopEncode()
{
	if (m_encoder != nullptr)
	{
		m_encoder->StopEncode();
	}
	if (m_threadHelper != nullptr)
	{
		auto thread = m_threadHelper->GetThread();
		thread->Wait();
		delete m_threadHelper;
		m_threadHelper = nullptr;
	}
}

bool EncoderPresenter::Encoding()
{
	if (!m_threadHelper)
		return false;
	auto thread = m_threadHelper->GetThread();
	if (!thread)
		return false;
	if (!thread->IsRunning())
		return false;
	return true;
}

void EncoderPresenter::OnAddedAFrame(wxCommandEvent& event)
{
	m_progress = event.GetInt();
	wxEvent* e =new wxCommandEvent(EVT_RefreshView);
	this->QueueEvent(e);
}

void EncoderPresenter::OnFinishEncode(wxCommandEvent& event)
{
	m_progress = m_imageStore.GetCount();
	wxCommandEvent* e = new wxCommandEvent(EVT_FINISH_JOB);
	this->QueueEvent(e);
}

void* EncodeThread::Entry()
{
	m_encoder->Encode(m_handler, m_filePath, m_store);
	return nullptr;
}
