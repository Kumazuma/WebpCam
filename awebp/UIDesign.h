///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/panel.h>
#include <wx/ribbon/toolbar.h>
#include <wx/ribbon/page.h>
#include <wx/ribbon/control.h>
#include <wx/ribbon/art.h>
#include <wx/ribbon/bar.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include "editframelistwidget.h"
#include "editrenderwidget.h"
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/htmllbox.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/aui/aui.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/dialog.h>
#include <wx/choice.h>
#include <wx/gauge.h>

///////////////////////////////////////////////////////////////////////////

#define ID_NEW_CAPTURE 1000
#define ID_SAVE_FILE 1001
#define ID_RESIZE_FRAME 1002
#define ID_CROP_TOOL 1003
#define ID_STORE_WINDOW 1004
#define ID_MAIN_PANE 1005
#define ID_DRAW_WIDGET 1006
#define ID_PLAY_BUTTON 1007
#define ID_HISTORY_LIST 1008
#define ID_PROPERTY 1009
#define ID_SPIN_X 1010
#define ID_SPIN_Y 1011
#define ID_SPIN_WIDTH 1012
#define ID_SPIN_HEIGHT 1013
#define ID_CHOICE_FPS 1014
#define ID_BTN_START 1015
#define ID_BTN_STOP 1016
#define ID_BTN_CLOSE 1017
#define ID_FILE_TYPE 1018
#define ID_QUALITY 1019
#define ID_PROGRESS_LABEL 1020
#define ID_PROGRESS_GAUGE 1021
#define ID_SAVE 1022
#define ID_CANCEL 1023

///////////////////////////////////////////////////////////////////////////////
/// Class UIEditFrame
///////////////////////////////////////////////////////////////////////////////
class UIEditFrame : public wxFrame
{
	private:

	protected:
		wxRibbonBar* m_ribbonBar1;
		wxRibbonPage* m_ribbonPage1;
		wxRibbonPanel* m_ribbonPanel1;
		wxRibbonButtonBar* m_ribbonButtonBar1;
		wxRibbonPanel* m_ribbonPanel2;
		wxRibbonButtonBar* m_ribbonButtonBar2;
		wxRibbonPanel* m_ribbonPanel3;
		wxRibbonToolBar* m_ribbonToolBar1;
		wxRibbonPanel* m_ribbonPanel7;
		wxRibbonButtonBar* m_ribbonButtonBar4;
		wxRibbonPage* m_ribbonPage3;

	public:

		UIEditFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxWANTS_CHARS );

		~UIEditFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class UIEditForm
///////////////////////////////////////////////////////////////////////////////
class UIEditForm : public wxPanel
{
	private:

	protected:
		FrameListWidget* ui_frameList;
		wxPanel* m_panel1;
		Edit::EditRenderWidget* ui_drawWidget;
		wxButton* ui_btnButton;
		wxSimpleHtmlListBox* m_customControl3;
		wxPropertyGrid* ui_propertyGrid;

	public:

		UIEditForm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString ); wxAuiManager m_mgr;

		~UIEditForm();

};

///////////////////////////////////////////////////////////////////////////////
/// Class UIResizeDialog
///////////////////////////////////////////////////////////////////////////////
class UIResizeDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText9;
		wxSpinCtrl* m_spinCtrl4;
		wxStaticText* m_staticText10;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;

	public:

		UIResizeDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Resize Image"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~UIResizeDialog();

};

///////////////////////////////////////////////////////////////////////////////
/// Class TopPannel
///////////////////////////////////////////////////////////////////////////////
class TopPannel : public wxPanel
{
	private:

	protected:
		wxStaticText* m_staticText18;
		wxSpinCtrl* ui_spinX;
		wxStaticText* m_staticText19;
		wxSpinCtrl* ui_spinY;
		wxStaticText* m_staticText1;
		wxSpinCtrl* ui_spinWidth;
		wxStaticText* m_staticText2;
		wxSpinCtrl* ui_spinHeight;

	public:

		TopPannel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~TopPannel();

};

///////////////////////////////////////////////////////////////////////////////
/// Class BottomPanel
///////////////////////////////////////////////////////////////////////////////
class BottomPanel : public wxPanel
{
	private:

	protected:
		wxStaticText* m_staticText3;
		wxChoice* ui_choiceFps;
		wxButton* ui_btnStart;
		wxButton* ui_btnStop;
		wxButton* m_button11;

	public:

		BottomPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~BottomPanel();

};

///////////////////////////////////////////////////////////////////////////////
/// Class UIEncodingDialog
///////////////////////////////////////////////////////////////////////////////
class UIEncodingDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText13;
		wxChoice* m_choice5;
		wxStaticText* m_staticText14;
		wxSpinCtrl* m_spinCtrl8;
		wxStaticText* m_staticText15;
		wxGauge* m_gauge1;
		wxButton* m_button9;
		wxButton* m_button10;

	public:

		UIEncodingDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~UIEncodingDialog();

};

