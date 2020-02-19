#include "wx/wxprec.h"
#include "editframelistwidget.h"
#include <optional>
#include <wx/dcbuffer.h>
const int IMAGE_RESIZE_SiZE = 128;
const int WIDGET_BORDER = 5;
wxBitmap& FrameListItemWidget::GetBitmap()
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_virtualScreen;
}
inline void FrameListItemWidget::ItemSelect()
{ 
	m_isSelected = true; 
}

inline void FrameListItemWidget::ItemUnselect() 
{ 
	m_isSelected = false; 
}

FrameListItemWidget::FrameListItemWidget(wxWindow* parent,
	EditFramePresenter* presenter, size_t index):
	m_parent(parent),
	m_presenter(presenter), m_index(index), m_isSelected(false)
{
	Init();
}

FrameListItemWidget::~FrameListItemWidget()
{
}

void FrameListItemWidget::LoadData()
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
		m_virtualScreen.Create(m_size);
		DoPaint();
	}
}

void FrameListItemWidget::UnloadData()
{
	if (m_bitmap.IsOk())
	{
		m_bitmap.UnRef();
	}
	if (m_virtualScreen.IsOk())
	{
		m_virtualScreen.UnRef();
	}
}


inline void FrameListItemWidget::Init()
{
}

inline wxSize FrameListItemWidget::GetBestSize() const {
	// we need to calculate and return the best size of the widget...
	return wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
}

void FrameListItemWidget::OnPaint(const wxPoint& viewport, wxDC& dc)
{
	if (m_virtualScreen.IsOk())
	{
		dc.DrawBitmap(m_virtualScreen, m_position - viewport);
	}
}
void FrameListItemWidget::DoPaint()
{
	wxMemoryDC dc(m_virtualScreen);
	wxColour bgColor = wxColour(255, 255, 255);
	if (m_isSelected)
	{
		bgColor = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
	}
	dc.SetBrush(wxBrush(bgColor));
	dc.DrawRectangle(m_size);
	//dc.DrawRectangle(5, 5, 256, 256);
	if (m_bitmap.IsOk())
	{
		int x = (IMAGE_RESIZE_SiZE - m_bitmap.GetWidth()) / 2;
		int y = (IMAGE_RESIZE_SiZE - m_bitmap.GetHeight()) / 2;
		dc.DrawBitmap(m_bitmap, wxPoint(5 + x, 5 + y));
	}
}

FrameListWidget::FrameListWidget():
	m_dragStartPosition()
{ 
	Init();
}

FrameListWidget::FrameListWidget(
	wxWindow* parent, wxWindowID winid,
	const wxPoint& pos, const wxSize& size,
	long style, const wxValidator& val, const wxString& name):
	wxScrolledCanvas(parent,winid,pos,size,style | wxVSCROLL, name),
	m_dragStartPosition()
{
	Init();
}

FrameListWidget::~FrameListWidget()
{
	ClearChildren();
}

int FrameListWidget::AddFrameImage(FrameListItemWidget* item)
{
	
	int index = m_items.size();
	item->ItemUnselect();
	m_items.push_back(item);
	this->AlignItems();
	return index;
}

std::vector<size_t> FrameListWidget::GetSelections()
{
	std::vector<size_t> res;
	for (int i = 0; i < m_items.size(); i++)
	{
		if (m_items[i]->IsSelected())
		{
			res.push_back(i);
		}
	}
	return res;
}

std::optional<size_t> FrameListWidget::GetSelection()
{
	for (int i = 0; i < m_items.size(); i++)
	{
		if (m_items[i]->IsSelected())
		{
			return i;
		}
	}
	return std::optional<size_t>();
}

void FrameListWidget::SetSelection(size_t index)
{
	for (int i = 0; i < m_items.size(); i++)
	{
		if (index == i)
		{
			m_items[i]->ItemSelect();
			Scroll(m_items[i]->GetPosition()/ m_items[i]->GetSize().y);
		}
		else
		{
			m_items[i]->ItemUnselect();
		}
	}
	UpdateItemsImageLoad();
	DoPaint();
}

void FrameListWidget::ClearChildren()
{
	//this->DestroyChildren();
	for (auto it : m_items)
	{
		delete it;
	}
	m_items.clear();
	this->Refresh(true);
}




void FrameListWidget::Init() {
	long style = this->GetWindowStyle();
	SetMinSize(wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2));
	SetSize(wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2));
	this->SetWindowStyle(style | wxVSCROLL);
	this->EnableScrolling(false, true);
	SetScrollRate(0, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
}

wxSize FrameListWidget::DoGetBestSize() const {
	// we need to calculate and return the best size of the widget...
	auto vscrollWidth = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
	size_t height = 0;
	for (auto it : m_items)
	{
		height += it->GetBestSize().GetHeight();
	}
	return wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2 + vscrollWidth, height);
}

void FrameListWidget::AlignItems()
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
	//UpdateItemsImageLoad();
	//this->PostSizeEventToParent();
}
void FrameListWidget::OnScrolledEvent(wxScrollWinEvent& event)
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

void FrameListWidget::OnSized(wxSizeEvent& event)
{
	event.Skip();
	UpdateItemsImageLoad();
}
void FrameListWidget::DoPaint(wxDC& dc)
{
	dc.Clear();
	auto viewStart = GetViewStart() * (IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
	auto viewRect = wxRect(viewStart, GetSize());
	for (auto it : m_items)
	{
		//실제로 화면에 보일 때만 갱신한다.
		if (viewRect.Contains(it->GetPosition()))
		{
			it->OnPaint(viewStart, dc);
		}
	}
}
void FrameListWidget::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	DoPaint(dc);
}
void FrameListWidget::UpdateItemsImageLoad()
{
	auto viewViewStart = GetViewStart();
	int viewStartY = viewViewStart.y;
	
	int viewEndY = viewStartY + GetSize().GetHeight() / IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2;
	if (viewEndY <= 0)
		return;
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
		m_items[i]->DoPaint();
	}
	DoPaint();
}
void FrameListWidget::OnMouseLeftDown(wxMouseEvent& event)
{
	auto pos = event.GetPosition();
	auto obj = wxDynamicCast(event.GetEventObject(), wxWindow);
	auto viewStart = GetViewStart() * (IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
	pos = obj->ClientToScreen(pos);
	pos = this->ScreenToClient(pos);
	//가상 영역 포지션으로 바꾼다.
	pos = wxPoint(pos.x + viewStart.x, pos.y + viewStart.y);
	m_dragStartPosition = pos;
	
	for (auto item : m_items)
	{
		wxRect rc(item->GetPosition(), item->GetSize());
		if (rc.Contains(pos))
		{
			item->ItemSelect();
		}
		else
		{
			item->ItemUnselect();
		}
		item->DoPaint();
	}
	DoPaint();
}
void FrameListWidget::OnMouseLeftUp(wxMouseEvent& event) 
{
	if (m_dragStartPosition.has_value())
	{
		auto ctrl = wxDynamicCast(event.GetEventObject(), wxWindow);
		auto pos = event.GetPosition();
		pos = ctrl->ClientToScreen(pos);
		pos = this->ScreenToClient(pos);
		auto viewStart = GetViewStart() * (IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
		pos = wxPoint(pos.x + viewStart.x, pos.y + viewStart.y);

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
			item->DoPaint();
		}
		if (auto selection =this->GetSelection())
		{
			wxCommandEvent* event = new wxCommandEvent(wxEVT_LISTBOX);
			event->SetInt(*selection);
			this->QueueEvent(event);
		}
		DoPaint();
	}
	m_dragStartPosition.reset();
}
void FrameListWidget::OnMouseMotion(wxMouseEvent& event) 
{
	if (m_dragStartPosition.has_value())
	{
		auto ctrl = wxDynamicCast(event.GetEventObject(), wxWindow);
		auto pos = event.GetPosition();
		pos = ctrl->ClientToScreen(pos);
		pos = this->ScreenToClient(pos);
		auto viewStart = GetViewStart() * (IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
		pos = wxPoint(pos.x + viewStart.x, pos.y + viewStart.y);

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
			item->DoPaint();
		}
		DoPaint();
	}
}

void FrameListWidget::OnKeyDown(wxKeyEvent& event)
{
//TODO:
}


wxBEGIN_EVENT_TABLE(FrameListWidget, wxScrolledCanvas)
EVT_LEFT_DOWN(FrameListWidget::OnMouseLeftDown)
EVT_LEFT_UP(FrameListWidget::OnMouseLeftUp)
EVT_MOTION(FrameListWidget::OnMouseMotion)
EVT_SCROLLWIN(FrameListWidget::OnScrolledEvent)
EVT_SIZE(FrameListWidget::OnSized)
EVT_PAINT(FrameListWidget::OnPaint)
EVT_KEY_DOWN(FrameListWidget::OnKeyDown)
wxEND_EVENT_TABLE()

wxIMPLEMENT_DYNAMIC_CLASS(FrameListWidget, wxScrolledCanvas)
