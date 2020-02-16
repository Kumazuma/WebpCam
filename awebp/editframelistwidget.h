#pragma once
#include<wx/wx.h>
#include<wx/vector.h>
#include<optional>
#include"interface.h"
#include"editpresenter.h"
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
    void Init();
    virtual wxSize DoGetBestSize() const;
    void AlignItems();
    void OnScrolledEvent(wxScrollWinEvent& event);
    void OnSized(wxSizeEvent& event);
    void UpdateItemsImageLoad();
    /*
    아래부터는 클릭 및 선택에 대한 이벤트 핸들러들.
    */
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnMouseLeftUp(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
    void OnMouseEnter(wxMouseEvent& event);
private:
    wxDECLARE_DYNAMIC_CLASS(FrameListWidgets);
    wxDECLARE_EVENT_TABLE();
};
class FrameListItemWidgets : public wxControl
{
private:
    wxBitmap m_bitmap;
    size_t m_duration;
    bool m_isSelected;
    EditFramePresenter* m_presenter;
    size_t m_index;
public:
    FrameListItemWidgets() { Init(); }
    bool IsSelected() { return m_isSelected; }
    void ItemSelect();
    void ItemUnselect();
    FrameListItemWidgets(wxWindow* parent,
        wxWindowID winid,
        EditFramePresenter* presenter, size_t index,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0L,
        const wxValidator& val = wxDefaultValidator,
        const wxString& name = "FrameListItemWidgets");
    void LoadData();
    void UnloadData();
protected:
    const size_t HEIGHT_PER_DPI = 256 / 96;
    void Init();
    virtual wxSize DoGetBestSize() const;
    void OnPaint(wxPaintEvent& event);
    void DoPaint(wxDC& dc);
private:
    wxDECLARE_DYNAMIC_CLASS(FrameListItemWidgets);
    wxDECLARE_EVENT_TABLE();
};