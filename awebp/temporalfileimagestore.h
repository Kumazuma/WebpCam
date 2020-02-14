#pragma once
#include "interface.h"
#include <vector>
#include <wx/file.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
class FileImageStore : public IImageStore
{
private:
	std::vector<std::pair<wxString, uint32_t>> m_store;
public:
	virtual ~FileImageStore()
	{
		Clear();
	}
	virtual size_t Add(const wxImage& image, uint32_t duration) override
	{
		auto it = m_store.size();
		auto file = new wxFile();
		auto path = wxFileName::CreateTempFileName(wxT("awebmaker"), file);
		wxFileOutputStream stream{*file };
		image.SaveFile(stream, wxBITMAP_TYPE_PNG);
		stream.Close();
		m_store.push_back(std::pair<wxString, uint32_t>(path, duration));
		return it;
	}
	virtual const std::pair<wxImage, uint32_t> Get(size_t index) const  override
	{
		auto path = m_store[index].first;
		
		wxFileInputStream stream(path);
		auto image = wxImage(stream, wxBITMAP_TYPE_PNG);
		
		return std::pair<wxImage, uint32_t>(image, m_store[index].second);
	}
	virtual size_t GetSize() const override
	{
		return m_store.size();
	}
	virtual void Clear() override
	{
		m_store.clear();
	}
}; 