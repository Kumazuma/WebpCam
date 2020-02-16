#include "wx/wxprec.h"
#include "editframelistwidget.h"

void FrameListItemWidgets::SetImage(const wxImage& image)
{
	m_image = image;
	Init();
}

void FrameListItemWidgets::SetDuration(size_t duration)
{
	m_duration = duration;
	Init();
}

FrameListItemWidgets::FrameListItemWidgets(
	wxWindow* parent, wxWindowID winid,
	const wxImage& frame,size_t duration, const wxPoint& pos,
	const wxSize& size, long style,
	const wxValidator& val, const wxString& name):
	wxControl(parent,winid, pos, size, style, val, name ),
	m_image(frame), m_duration(duration)
{
	Init();

}

void FrameListItemWidgets::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.DrawRectangle(5, 5, 256, 256);
	if (m_bitmap.IsOk())
	{
		int x =(256 - m_bitmap.GetWidth()) / 2;
		int y =(256 - m_bitmap.GetHeight()) / 2;
		dc.DrawBitmap(m_bitmap, wxPoint(5 + x, 5 + y));
	}
}
FrameListWidgets::FrameListWidgets(
	wxWindow* parent, wxWindowID winid,
	const wxPoint& pos, const wxSize& size,
	long style, const wxString& name):
	wxScrolledCanvas(parent,winid,pos,size,style | wxVSCROLL, name)
{
	Init();
}

int FrameListWidgets::AddFrameImage(FrameListItemWidgets* item)
{
	
	int index = m_items.size();
	m_items.push_back(item);
	this->AlignItems();
	item->Show();
	this->Layout();
	return index;
}

wxSize FrameListWidgets::DoGetBestSize() const {
	// we need to calculate and return the best size of the widget...
	auto vscrollWidth = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
	size_t height = 0;
	for (auto it : m_items)
	{
		height += it->GetBestSize().GetHeight();
	}
	return wxSize(266 + vscrollWidth, height);
}

void FrameListWidgets::AlignItems()
{
	size_t height = 0;
	for (auto it : m_items)
	{
		it->SetPosition(wxPoint(0, height));
		it->SetSize(it->GetBestSize());
		height += it->GetBestSize().GetHeight();
	}
	auto size = DoGetBestSize();
	SetVirtualSize(size);
}
void FrameListWidgets::OnScrolledEvent(wxScrollEvent& event)
{
}

void FrameListWidgets::OnSized(wxSizeEvent& event)
{
	SetScrollbar(wxVERTICAL, 0, GetBestSize().GetHeight() / 266, m_items.size());
}


wxBEGIN_EVENT_TABLE(FrameListWidgets, wxScrolledCanvas)
EVT_SCROLL(FrameListWidgets::OnScrolledEvent)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(FrameListItemWidgets, wxControl)
EVT_PAINT(FrameListItemWidgets::OnPaint)
wxEND_EVENT_TABLE()

wxIMPLEMENT_DYNAMIC_CLASS(FrameListWidgets, wxScrolledCanvas)
wxIMPLEMENT_DYNAMIC_CLASS(FrameListItemWidgets, wxControl)

