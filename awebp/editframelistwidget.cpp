#include "wx/wxprec.h"
#include "editframelistwidget.h"
#include <optional>
#include <wx/dcbuffer.h>
const int IMAGE_RESIZE_SiZE = 128;
const int WIDGET_BORDER = 5;

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
}

void FrameListItemWidget::UnloadData()
{

}


inline void FrameListItemWidget::Init()
{
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
	 	m_thumnail =  image.Rescale(size.x, size.y, wxIMAGE_QUALITY_NEAREST);

		DoPaint();
	}
}

inline wxSize FrameListItemWidget::GetBestSize() const {
	return wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
}

void FrameListItemWidget::OnPaint(const wxPoint& viewport, wxDC& dc)
{
	if (m_thumnail.IsOk())
	{
		wxColour bgColor = wxColour(255, 255, 255);
		if (m_isSelected)
		{
			bgColor = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
		}
		dc.SetBrush(wxBrush(bgColor));
		dc.DrawRectangle(m_position - viewport, m_size);
		wxPoint gap = (wxPoint(m_size.x, m_size.y) - m_thumnail.GetSize()) / 2;
		dc.DrawBitmap(m_thumnail, m_position - viewport + gap);
	}
}
void FrameListItemWidget::OnPaint(wxGraphicsContext& gc)
{
	if (m_thumnail.IsOk())
	{
		wxColour bgColor = wxColour(255, 255, 255);
		if (m_isSelected)
		{
			bgColor = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
		}
		gc.SetBrush(wxBrush(bgColor));
		auto pos = m_position ;
		wxPoint gap = (wxPoint(m_size.x, m_size.y) - m_thumnail.GetSize()) / 2;
		gc.DrawRectangle(pos.x,pos.y, m_size.x,m_size.y);
		pos = m_position + gap;
		auto thSize = m_thumnail.GetSize();
		gc.DrawBitmap(m_thumnail, pos.x, pos.y,thSize.x, thSize.y);
	}
}
void FrameListItemWidget::DoPaint()
{
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
#ifdef wxUSE_GRAPHICS_DIRECT2D
	m_renderer = wxGraphicsRenderer::GetDirect2DRenderer();
#else
	m_renderer = wxGraphicsRenderer::GetDefaultRenderer();
	
#endif
	long style = this->GetWindowStyle();
	SetMinSize(wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2));
	SetSize(wxSize(IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2));
	this->SetWindowStyle(style | wxVSCROLL);
	this->EnableScrolling(false, true);
	SetScrollRate(0, IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
}

wxSize FrameListWidget::DoGetBestSize() const {
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
}
void FrameListWidget::OnScrolledEvent(wxScrollWinEvent& event)
{
	event.Skip();
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
template<class DC>
void FrameListWidget::DoPaint(DC& dc)
{
	auto gc = m_renderer->CreateContext(dc);
	
	auto viewStart = GetViewStart() * (IMAGE_RESIZE_SiZE + WIDGET_BORDER * 2);
	auto viewRect = wxRect(viewStart, GetClientSize());
	gc->Translate(-viewStart.x, -viewStart.y);
	gc->SetBrush(wxBrush(wxColour(0xFF, 0xFF, 0xFF), wxBRUSHSTYLE_SOLID));
	gc->DrawRectangle(viewStart.x, viewStart.y, viewRect.width, viewRect.height);
	for (auto it : m_items)
	{
		//실제로 화면에 보일 때만 갱신한다.
		if (viewRect.Contains(it->GetPosition()))
		{
			it->OnPaint(*gc);
		}
	}
	delete gc;
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
			event->SetEventObject(this);
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
