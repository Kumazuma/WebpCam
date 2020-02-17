#pragma once
#include<wx/wx.h>
#include<wx/vector.h>
#include<optional>
#include"interface.h"
#include"editpresenter.h"
class FrameListItemWidget;
class FrameListWidget : public wxScrolledCanvas
{
private:
    wxVector<FrameListItemWidget*> m_items;
    //마우스 처리를 위한 변수들...
    std::optional<wxPoint> m_dragStartPosition;
public:
    FrameListWidget();
    FrameListWidget(wxWindow* parent,
        wxWindowID winid,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0L,
        const wxValidator& val = wxDefaultValidator,
        const wxString& name = "FrameListWidgets");
    ~FrameListWidget();
    int AddFrameImage(FrameListItemWidget* item);
    std::vector<size_t> GetSelections();
    std::optional<size_t> GetSelection();
    void ClearChildren();
protected:
    void DoPaint(wxDC& dc);
    void DoPaint() { wxClientDC dc(this); DoPaint(dc); }
    void Init();
    void UpdateItemsImageLoad();
    virtual wxSize DoGetBestSize() const;
    void AlignItems();
    void OnScrolledEvent(wxScrollWinEvent& event);
    void OnSized(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);
    /*
    아래부터는 클릭 및 선택에 대한 이벤트 핸들러들.
    */
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnMouseLeftUp(wxMouseEvent& event);
    void OnMouseMotion(wxMouseEvent& event);
    void OnKeyDown(wxKeyEvent& event);
private:
    wxDECLARE_DYNAMIC_CLASS(FrameListWidget);
    wxDECLARE_EVENT_TABLE();
};
class FrameListItemWidget
{
    friend class FrameListWidget;
private:
    wxBitmap m_bitmap;
    size_t m_duration;
    bool m_isSelected;
    EditFramePresenter* m_presenter;
    wxWindow* m_parent;
    size_t m_index;
    wxPoint m_position;
    wxSize m_size;
    wxBitmap m_virtualScreen;
public:
    wxBitmap& GetBitmap();
    bool IsSelected() { return m_isSelected; }
    void ItemSelect();
    void ItemUnselect();
    FrameListItemWidget(wxWindow* parent,
        EditFramePresenter* presenter, size_t index);
    ~FrameListItemWidget();
    void LoadData();
    void UnloadData();
    wxSize GetBestSize() const;
    wxSize GetSize() { return m_size; }
    wxPoint GetPosition() { return m_position; }
    wxRect GetRect() { return wxRect(m_position, m_size); }
    void SetPosition(const wxPoint& point) { m_position = point; }
    void OnPaint(const wxPoint& viewport, wxDC& dc);
    void SetSize(const wxSize& size) { m_size = size; }
protected:
    const size_t HEIGHT_PER_DPI = 256 / 96;
    void Init();
    void DoPaint();
private:
};