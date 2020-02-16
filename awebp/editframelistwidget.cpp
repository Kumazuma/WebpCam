#include "wx/wxprec.h"
#include "editframelistwidget.h"
#include <optional>
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

inline void FrameListItemWidgets::ItemSelect()
{ 
	m_isSelected = true; 

	
	this->Refresh();
}

inline void FrameListItemWidgets::ItemUnselect() 
{ 
	m_isSelected = false; 

	this->Refresh();
}

FrameListItemWidgets::FrameListItemWidgets(
	wxWindow* parent, wxWindowID winid,
	const wxImage& frame,size_t duration, const wxPoint& pos,
	const wxSize& size, long style,
	const wxValidator& val, const wxString& name):
	wxControl(parent,winid, pos, size, style, val, name ),
	m_image(frame), m_duration(duration), m_isSelected(false)
{
	Init();

}

void FrameListItemWidgets::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	wxColour bgColor = wxColour(255, 255, 255);
	if (m_isSelected)
	{
		bgColor = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
	}
	dc.SetBrush(wxBrush(bgColor));
	dc.DrawRectangle(wxRect(GetSize()));
	//dc.DrawRectangle(5, 5, 256, 256);
	if (m_bitmap.IsOk())
	{
		int x =(256 - m_bitmap.GetWidth()) / 2;
		int y =(256 - m_bitmap.GetHeight()) / 2;
		dc.DrawBitmap(m_bitmap, wxPoint(5 + x, 5 + y));
	}
}

FrameListWidgets::FrameListWidgets():
	m_dragStartPosition()
{ 
	Init();
}

FrameListWidgets::FrameListWidgets(
	wxWindow* parent, wxWindowID winid,
	const wxPoint& pos, const wxSize& size,
	long style, const wxString& name):
	wxScrolledCanvas(parent,winid,pos,size,style | wxVSCROLL, name),
	m_dragStartPosition()
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
	item->Bind(wxEVT_LEFT_DOWN, &FrameListWidgets::OnMouseLeftDown, this);
	item->Bind(wxEVT_LEFT_UP, &FrameListWidgets::OnMouseLeftUp, this);
	item->Bind(wxEVT_LEAVE_WINDOW, &FrameListWidgets::OnMouseLeave, this);
	item->Bind(wxEVT_ENTER_WINDOW, &FrameListWidgets::OnMouseEnter, this);
	item->Bind(wxEVT_MOTION, &FrameListWidgets::OnMouseEnter, this);
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
void FrameListWidgets::OnMouseLeftDown(wxMouseEvent& event)
{
	auto pos = event.GetPosition();
	auto obj = wxDynamicCast(event.GetEventObject(), wxWindow);
	pos = obj->ClientToScreen(pos);
	pos = this->ScreenToClient(pos);
	m_dragStartPosition = pos;
	auto ctrl = wxDynamicCast(event.GetEventObject(), FrameListItemWidgets);
	
	for (auto item : m_items)
	{
		
		item->ItemUnselect();
	}
	if (ctrl != nullptr)
	{
		ctrl->ItemSelect();
	}
}
void FrameListWidgets::OnMouseLeftUp(wxMouseEvent& event) 
{
	if (m_dragStartPosition.has_value())
	{
		auto item = wxDynamicCast(event.GetEventObject(), FrameListItemWidgets);
		if (item != nullptr)
		{
			item->ItemSelect();
		}
		//TODO: 아이템 선택
	}
	m_dragStartPosition.reset();
}
void FrameListWidgets::OnMouseLeave(wxMouseEvent& event) 
{
	if (m_dragStartPosition.has_value())
	{
		auto ctrl = wxDynamicCast(event.GetEventObject(), FrameListItemWidgets);
		auto pos = event.GetPosition();
		pos = ctrl->ClientToScreen(pos);
		pos = this->ScreenToClient(pos);
		const wxRect rc(*m_dragStartPosition, pos);
		for (auto item : m_items)
		{
			
			auto itemRect = item->GetRect();
			auto res = rc.Intersect(itemRect);
			//겹치는 부분이 없다면
			if (res.width * res.height == 0)
			{
				//아이템은 선택 해제한다.
				item->ItemUnselect();
			}
			else
			{
				item->ItemSelect();
			}
		}
	}
}
void FrameListWidgets::OnMouseEnter(wxMouseEvent& event) 
{
	if (m_dragStartPosition.has_value())
	{
		auto ctrl = wxDynamicCast(event.GetEventObject(), FrameListItemWidgets);
		auto pos = event.GetPosition();
		pos = ctrl->ClientToScreen(pos);
		pos = this->ScreenToClient(pos);
		const wxRect rc(*m_dragStartPosition, pos);
		for (auto item : m_items)
		{
			auto itemRect = item->GetRect();
			auto res = rc.Intersect(itemRect);
			//겹치는 부분이 없다면
			if (res.width * res.height == 0)
			{
				//아이템은 선택 해제한다.
				item->ItemUnselect();
			}
			else
			{
				item->ItemSelect();
			}
		}
	}
}


wxBEGIN_EVENT_TABLE(FrameListWidgets, wxScrolledCanvas)
EVT_LEFT_DOWN(FrameListWidgets::OnMouseLeftDown)
EVT_LEFT_UP(FrameListWidgets::OnMouseLeftUp)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(FrameListItemWidgets, wxControl)
EVT_PAINT(FrameListItemWidgets::OnPaint)
wxEND_EVENT_TABLE()

wxIMPLEMENT_DYNAMIC_CLASS(FrameListWidgets, wxScrolledCanvas)
wxIMPLEMENT_DYNAMIC_CLASS(FrameListItemWidgets, wxControl)

