#pragma once
#include<wx/wx.h>
#include<wx/vector.h>
#include<optional>
#include"interface.h"
class FrameListItemWidgets;
class FrameListWidgets : public wxScrolledCanvas
{
private:
    wxVector<FrameListItemWidgets*> m_items;
    //마우스 처리를 위한 변수들...
    std::optional<wxPoint> m_dragStartPosition;
    
public:
    FrameListWidgets();
    FrameListWidgets(wxWindow* parent,
        wxWindowID winid,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0L,
        const wxString& name = "FrameListWidgets");
    int AddFrameImage(FrameListItemWidgets* item);
protected:
    void Init() {
        long style = this->GetWindowStyle();
        this->SetWindowStyle(style | wxVSCROLL);
        SetScrollbar(wxVERTICAL, 0, 266, 0);
        SetScrollRate(0, 266);
    }
    virtual wxSize DoGetBestSize() const;
    void AlignItems();
    void OnScrolledEvent(wxScrollEvent& event);
    void OnSized(wxSizeEvent& event);
    /*
    아래부터는 클릭 및 선택에 대한 이벤트 핸들러들.
    */
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnMouseLeftUp(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
    void OnMouseEnter(wxMouseEvent& event);
    void OnMouseMotion(wxMouseEvent& event);
private:
    wxDECLARE_DYNAMIC_CLASS(FrameListWidgets);
    wxDECLARE_EVENT_TABLE();
};
class FrameListItemWidgets : public wxControl
{
private:
    wxBitmap m_bitmap;
    wxImage m_image;
    size_t m_duration;
    bool m_isSelected;
    
public:
    FrameListItemWidgets() { Init(); }
    void SetImage(const wxImage& image);
    void SetDuration(size_t duration);
    bool IsSelected() { return m_isSelected; }
    void ItemSelect();
    void ItemUnselect();
    FrameListItemWidgets(wxWindow* parent,
        wxWindowID winid,
        const wxImage& frame,
        size_t duration,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0L,
        const wxValidator& val = wxDefaultValidator,
        const wxString& name = "FrameListItemWidgets");
protected:
    const size_t HEIGHT_PER_DPI = 256 / 96;
    void Init() {
        
        // init widget's internals...
        if (m_image.IsOk())
        {
            auto size = m_image.GetSize();
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
            float radio = 256.f / *max;
            *max = 256;
            *min = *min * radio;
            auto image = m_image.Scale(size.x, size.y,wxIMAGE_QUALITY_NEAREST);
            m_bitmap = image;
            m_image.Clear();
        }
        
    }
    virtual wxSize DoGetBestSize() const {
        // we need to calculate and return the best size of the widget...
        return wxSize(266, 266);
    }
    void OnPaint(wxPaintEvent& event);
    
private:
    wxDECLARE_DYNAMIC_CLASS(FrameListItemWidgets);
    wxDECLARE_EVENT_TABLE();
};