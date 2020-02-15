#include "wx/wxprec.h"
#include "model.h"

wxDEFINE_EVENT(EVT_PropertyChanged, wxCommandEvent);
Model::Model()
{
	m_isRecording = false;
	m_fps = FPS::_10;
	m_isUsingTemporalFile = true;
	m_recordedRect = wxRect();
}

Model::~Model()
{
	m_representers.clear();
}
void Model::LinkPresenter(wxEvtHandler* handler)
{
	auto& representers = m_representers;
	auto it = representers.find(handler);
	if (representers.end() == it)
	{
		representers.insert(handler);
	}
}

void Model::UnlinkPresenter(wxEvtHandler* handler)
{
	auto& representers = m_representers;
	auto it = representers.find(handler);
	if (representers.end() != it)
	{
		representers.erase(it);
	}
}

void Model::SetFPS(FPS fps)
{ 
	m_fps = fps;
}

void Model::UpdateNotify(ModelPropertyId id)
{
	wxCommandEvent event(EVT_PropertyChanged);
	event.SetId(id);
	for (auto* handler : m_representers)
	{
		wxQueueEvent(handler, new wxCommandEvent(event));
	}
}
