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
#include <wx/ribbon/page.h>
#include <wx/ribbon/control.h>
#include <wx/ribbon/art.h>
#include <wx/ribbon/bar.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/slider.h>
#include <wx/aui/aui.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/scrolwin.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////
class MyFrame1 : public wxFrame
{
	private:

	protected:
		wxRibbonBar* m_ribbonBar1;
		wxRibbonPage* m_ribbonPage1;
		wxRibbonPanel* m_ribbonPanel1;
		wxRibbonButtonBar* m_ribbonButtonBar1;

	public:

		MyFrame1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame1();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MyPanel1
///////////////////////////////////////////////////////////////////////////////
class MyPanel1 : public wxPanel
{
	private:

	protected:
		wxListBox* m_listBox1;
		wxPanel* m_panel1;
		wxPanel* m_panel2;
		wxSlider* m_slider1;

	public:

		MyPanel1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString ); wxAuiManager m_mgr;

		~MyPanel1();

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

	public:

		MyFrame3( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame3();

};

