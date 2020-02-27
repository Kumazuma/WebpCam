﻿///////////////////////////////////////////////////////////////////////////
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
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include "editframelistwidget.h"
#include "editrenderwidget.h"
#include <wx/panel.h>
#include <wx/htmllbox.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/aui/aui.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/scrolwin.h>
#include <wx/treectrl.h>
#include <wx/treelist.h>
#include <wx/dialog.h>
#include <wx/dirctrl.h>

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
/// Class MyFrame5
///////////////////////////////////////////////////////////////////////////////
class MyFrame5 : public wxFrame
{
	private:

	protected:
		wxBitmapButton* m_bpButton1;

	public:

		MyFrame5( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame5();

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
/// Class UIRecordFrame
///////////////////////////////////////////////////////////////////////////////
class UIRecordFrame : public wxFrame
{
	private:

	protected:
		wxPanel* m_panel30;
		wxStaticText* m_staticText1;
		wxSpinCtrl* ui_spinWidth;
		wxStaticText* m_staticText2;
		wxSpinCtrl* ui_spinHeight;
		wxStaticText* m_staticText3;
		wxChoice* ui_choiceFps;
		wxStaticText* m_staticText5;
		wxChoice* m_choice2;
		wxCheckBox* ui_chkUsingTempFile;
		wxButton* ui_btnStart;
		wxButton* ui_btnStop;
		wxStaticText* ui_lblLog;

	public:

		UIRecordFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~UIRecordFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame3
///////////////////////////////////////////////////////////////////////////////
class MyFrame3 : public wxFrame
{
	private:

	protected:
		wxScrolledWindow* m_scrolledWindow1;
		wxPanel* m_panel4;
		wxPanel* m_panel45;
		wxPanel* m_panel44;
		wxPanel* m_panel43;
		wxPanel* m_panel42;
		wxPanel* m_panel41;
		wxTreeCtrl* m_treeCtrl1;
		wxTreeListCtrl* m_treeListCtrl1;

	public:

		MyFrame3( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame3();

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
/// Class MyFrame6
///////////////////////////////////////////////////////////////////////////////
class MyFrame6 : public wxFrame
{
	private:

	protected:
		wxPanel* m_panel11;
		wxButton* m_button10;
		wxPanel* m_panel12;
		wxButton* m_button9;

	public:

		MyFrame6( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame6();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame4
///////////////////////////////////////////////////////////////////////////////
class MyFrame4 : public wxFrame
{
	private:

	protected:
		wxPanel* m_panel10;
		wxRibbonBar* m_ribbonBar3;
		wxRibbonPage* m_ribbonPage5;
		wxRibbonPanel* m_ribbonPanel5;
		wxRibbonPanel* m_ribbonPanel6;
		wxPanel* m_panel11;
		wxGenericDirCtrl* m_genericDirCtrl1;

	public:

		MyFrame4( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;

		~MyFrame4();

};

