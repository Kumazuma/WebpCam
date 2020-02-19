#include <wx/wxprec.h>

const wxBitmapType wxBITMAP_TYPE_WEBP = (wxBitmapType)(wxBitmapType::wxBITMAP_TYPE_ANY + 1);
class wxWebPHandler :public wxImageHandler
{
public:
    wxWebPHandler()
    {
        m_name = wxT("WebP file");
        m_extension = wxT("webp");
        m_mime = wxT("image/webp");
    }
    virtual bool LoadFile(wxImage* image, wxInputStream& stream, bool verbose = true, int index = -1) wxOVERRIDE;
    virtual bool SaveFile(wxImage* image, wxOutputStream& stream, bool verbose = true) wxOVERRIDE;
protected:
    virtual bool DoCanRead(wxInputStream& stream) wxOVERRIDE;
private:
	wxDECLARE_DYNAMIC_CLASS(wxWebPHandler);
};