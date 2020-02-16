#include "wx/wxprec.h"
#include "editframelistwidget.h"
#include <optional>
#include <wx/dcbuffer.h>
const int IMAGE_RESIZE_SiZE = 128;
const int WIDGET_BORDER = 5;
inline void FrameListItemWidgets::ItemSelect()
{ 
	m_isSelected = true; 
	wxClientDC dc(this);
	DoPaint(dc);
}

inline void FrameListItemWidgets::ItemUnselect() 
{ 
	m_isSelected = false; 
	wxClientDC dc(this);
	DoPaint(dc);
}

FrameListItemWidgets::FrameListItemWidgets(
	wxWindow* parent, wxWindowID winid,
	EditFramePresenter* presenter, size_t index, const wxPoint& pos,
	const wxSize& size, long style,
	const wxValidator& val, const wxString& name):
	wxControl(parent,winid, pos, size, style, val, name ),
	m_presenter(presenter), m_index(index), m_isSelected(false)
{
	Init();
}

void FrameListItemWidgets::LoadData()
{
	if (m_presenter == nullptr)
	{
		return;
	}
	if (m_bitmap.IsOk())
	{
		return;
	}
	auto& presenter = *m_presenter;
	wxImage image;
	uint32_t duration;
	if (presenter.GetImage(m_index, image, duration))
	{
		auto size = image.GetSize();
		int* max = nullptr;
		int* min = nullptr;

		if (size.x >= size.y)
		{
			max = &size.x;
			min = &size.y;
		}
		else
		{
			max = &size.y;
			min = &size.x;
		}
		float radio = float(IMAGE_RESIZE_SiZE) / *max;
		*max = IMAGE_RESIZE_SiZE;
		*min = *min * radio;
		image.Rescale(size.x, size.y, wxIMAGE_QUALITY_NEAREST);
		m_bitmap = image;
		
		wxClientDC dc(this);
		DoPaint(dc);
	}
}

void FrameListItemWidgets::UnloadData()
{
	if (m_bitmap.IsOk())
	{
		m_bitmap.UnRef();
	}
}

inline void FrameListItemWidgets::Init()
{
}

inline wxSize FrameListItemWidgets::DoGetBestSize() const {
	// we need to calculate and return the best size of the widget...
	return wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
}

void FrameListItemWidgets::OnPaint(wxPaintEvent& event)
{
	
	wxBufferedPaintDC dc(this);
	DoPaint(dc);
}

void FrameListItemWidgets::DoPaint(wxDC& dc)
{
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
		int x = (IMAGE_RESIZE_SiZE - m_bitmap.GetWidth()) / 2;
		int y = (IMAGE_RESIZE_SiZE - m_bitmap.GetHeight()) / 2;
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

void FrameListWidgets::Init() {
	long style = this->GetWindowStyle();
	SetMinSize(wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2));
	SetSize(wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2));
	this->SetWindowStyle(style | wxVSCROLL);
	this->EnableScrolling(false, true);
	SetScrollRate(0, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
}

wxSize FrameListWidgets::DoGetBestSize() const {
	// we need to calculate and return the best size of the widget...
	auto vscrollWidth = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
	size_t height = 0;
	for (auto it : m_items)
	{
		height += it->GetBestSize().GetHeight();
	}
	return wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2 + vscrollWidth, height);
}

void FrameListWidgets::AlignItems()
{
	size_t height = 0;
	for (auto it : m_items)
	{
		auto size = it->GetBestSize();
		it->SetPosition(wxPoint(0, height));
		it->SetSize(size);
		height += size.GetHeight();
	}
	SetVirtualSize(DoGetBestSize());
	UpdateItemsImageLoad();
}
void FrameListWidgets::OnScrolledEvent(wxScrollWinEvent& event)
{
	event.Skip();
	int etypes[] = {
		wxEVT_SCROLLWIN_BOTTOM,
		wxEVT_SCROLLWIN_LINEDOWN,
		wxEVT_SCROLLWIN_LINEUP,
		wxEVT_SCROLLWIN_PAGEDOWN,
		wxEVT_SCROLLWIN_PAGEUP,
		wxEVT_SCROLLWIN_THUMBRELEASE,
		wxEVT_SCROLLWIN_THUMBTRACK,
		wxEVT_SCROLLWIN_TOP
	};
	wxString typestr[] = {
		"wxEVT_SCROLLWIN_BOTTOM",
		"wxEVT_SCROLLWIN_LINEDOWN",
		"wxEVT_SCROLLWIN_LINEUP",
		"wxEVT_SCROLLWIN_PAGEDOWN",
		"wxEVT_SCROLLWIN_PAGEUP",
		"wxEVT_SCROLLWIN_THUMBRELEASE",
		"wxEVT_SCROLLWIN_THUMBTRACK",
		"wxEVT_SCROLLWIN_TOP"
	};
	/*auto t = event.GetEventType();
	for (int i = 0; i < sizeof(etypes) / sizeof(etypes[0]); i++)
	{
		if (t == etypes[i])
		{
			wxLogGeneric(wxLogLevelValues::wxLOG_Message, typestr[i]);
			break;
		}
	}*/

		 this->CallAfter([this]()
		{
			UpdateItemsImageLoad();
		});
}

void FrameListWidgets::OnSized(wxSizeEvent& event)
{
	event.Skip();
	UpdateItemsImageLoad();
}
void FrameListWidgets::UpdateItemsImageLoad()
{
	auto viewViewStart = GetViewStart();
	int viewStartY = viewViewStart.y;
	
	int viewEndY = viewStartY + GetSize().GetHeight() / IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2;
	for (int i = 0 ; i <m_items.size() ;i++)
	{
		if (viewStartY <= i && i <= viewEndY)
		{
			m_items[i]->LoadData();
		}
		else
		{
			m_items[i]->UnloadData();
		}
	}
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
EVT_SCROLLWIN(FrameListWidgets::OnScrolledEvent)
EVT_SIZE(FrameListWidgets::OnSized)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(FrameListItemWidgets, wxControl)
EVT_PAINT(FrameListItemWidgets::OnPaint)
wxEND_EVENT_TABLE()

wxIMPLEMENT_DYNAMIC_CLASS(FrameListWidgets, wxScrolledCanvas)
wxIMPLEMENT_DYNAMIC_CLASS(FrameListItemWidgets, wxControl)

