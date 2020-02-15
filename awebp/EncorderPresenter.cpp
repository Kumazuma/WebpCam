#include "wx/wxprec.h"
#include "EncorderPresenter.h"
#include "webpencoder.h"
wxIMPLEMENT_DYNAMIC_CLASS(EncoderPresenter, wxEvtHandler);
EncoderPresenter::EncoderPresenter(wxEvtHandler* view, IImageStore * imageStore):
	wxEvtHandler(), m_view(view), m_imageStore(imageStore), m_encoder(new WebpEncoder())
{

}

EncoderPresenter::~EncoderPresenter()
{
	if (m_imageStore != nullptr)
	{
		delete m_imageStore;
	}
	if (m_encoder != nullptr)
	{
		delete m_encoder;
	}
}

void EncoderPresenter::SetFileFormat(EncodeType type)
{
	//TODO: Not Implement
}

wxString EncoderPresenter::GetFileFilter()
{
	return m_encoder->GetFileFilter();
}

class EncodeThread :public wxThread
{
public:
	EncodeThread(wxEvtHandler* handler,const wxString& filePath, IEncoder* encoder, IImageStore* store):
		m_encoder(encoder),
		m_store(store),
		m_filePath(filePath),
		m_handler(handler)
	{

	}
protected:
	void* Entry() override;
private:
	IEncoder* m_encoder;
	IImageStore* m_store;
	wxString m_filePath;
	wxEvtHandler* m_handler;
};

void EncoderPresenter::SaveAnimImage(const wxString& filePath)
{
	auto thread = new EncodeThread(this, filePath, m_encoder, m_imageStore);
	thread->Run();
}

void* EncodeThread::Entry()
{
	m_encoder->Encode(m_handler, m_filePath,*m_store);
	return nullptr;
}
