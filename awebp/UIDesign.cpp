///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include<wx/wxprec.h>

#include "UIDesign.h"

///////////////////////////////////////////////////////////////////////////

UIEditFrame::UIEditFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_ribbonBar1 = new wxRibbonBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRIBBON_BAR_DEFAULT_STYLE );
	m_ribbonBar1->SetArtProvider(new wxRibbonMSWArtProvider);
	m_ribbonPage1 = new wxRibbonPage( m_ribbonBar1, wxID_ANY, wxT("편집") , wxNullBitmap , 0 );
	m_ribbonPanel1 = new wxRibbonPanel( m_ribbonPage1, wxID_ANY, wxT("파일") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBar1 = new wxRibbonButtonBar( m_ribbonPanel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBar1->AddButton( ID_NEW_CAPTURE, wxT("새로 촬영하기"), wxArtProvider::GetBitmap( wxART_NEW, wxART_TOOLBAR ), wxEmptyString);
	m_ribbonButtonBar1->AddButton( ID_SAVE_FILE, wxT("저장하기"), wxArtProvider::GetBitmap( wxART_FILE_SAVE, wxART_TOOLBAR ), wxEmptyString);
	m_ribbonPanel2 = new wxRibbonPanel( m_ribbonPage1, wxID_ANY, wxT("작업내역") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBar2 = new wxRibbonButtonBar( m_ribbonPanel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBar2->AddButton( wxID_UNDO, wxT("되돌리기"), wxArtProvider::GetBitmap( wxART_UNDO, wxART_TOOLBAR ), wxEmptyString);
	m_ribbonButtonBar2->AddButton( wxID_REDO, wxT("실행복귀"), wxArtProvider::GetBitmap( wxART_REDO, wxART_TOOLBAR ), wxEmptyString);
	m_ribbonPanel3 = new wxRibbonPanel( m_ribbonPage1, wxID_ANY, wxT("도구") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonToolBar1 = new wxRibbonToolBar( m_ribbonPanel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonToolBar1->AddTool( wxID_DELETE, wxArtProvider::GetBitmap( wxART_DELETE, wxART_TOOLBAR ), wxT("선택된 프레임을 제거합니다"));
	m_ribbonToolBar1->AddTool( ID_RESIZE_FRAME, wxBitmap( wxT("expand(2).png"), wxBITMAP_TYPE_ANY ), wxT("이미지의 크기를 조절합니다"));
	m_ribbonPage3 = new wxRibbonPage( m_ribbonBar1, wxID_ANY, wxT("테스트") , wxNullBitmap , 0 );
	m_ribbonBar1->Realize();

	bSizer1->Add( m_ribbonBar1, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

UIEditFrame::~UIEditFrame()
{
}

UIEditForm::UIEditForm( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_LIVE_RESIZE|wxAUI_MGR_RECTANGLE_HINT|wxAUI_MGR_TRANSPARENT_HINT);

	ui_frameList = new FrameListWidget( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("ui_frameList") );
	m_mgr.AddPane( ui_frameList, wxAuiPaneInfo() .Left() .CaptionVisible( false ).CloseButton( false ).Movable( false ).Dock().Resizable().FloatingSize( wxDefaultSize ) );

	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel1, wxAuiPaneInfo() .Center() .Caption( wxT("테스트") ).CaptionVisible( false ).CloseButton( false ).Movable( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( true ).BottomDockable( false ).TopDockable( false ).LeftDockable( false ).RightDockable( false ).Floatable( false ).CentrePane() );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_panel2 = new wxPanel( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("ui_draw") );
	bSizer2->Add( m_panel2, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	m_button3 = new wxButton( m_panel1, wxID_ANY, wxT("재생"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_button3, 0, wxALL, 5 );

	m_slider1 = new wxSlider( m_panel1, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer11->Add( m_slider1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer2->Add( bSizer11, 0, wxEXPAND, 5 );


	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_listBox1 = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0, wxDefaultValidator, wxT("ui_historyBox") );
	m_mgr.AddPane( m_listBox1, wxAuiPaneInfo() .Right() .CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 163,107 ) ) );


	m_mgr.Update();
}

UIEditForm::~UIEditForm()
{
	m_mgr.UnInit();

}

UIRecordFrame::UIRecordFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	m_panel30 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( m_panel30, wxID_ANY, wxT("Width"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	ui_spinWidth = new wxSpinCtrl( m_panel30, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS|wxTE_PROCESS_ENTER|wxTAB_TRAVERSAL, 0, 9999, 0 );
	fgSizer1->Add( ui_spinWidth, 0, wxALL|wxEXPAND, 5 );

	m_staticText2 = new wxStaticText( m_panel30, wxID_ANY, wxT("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	ui_spinHeight = new wxSpinCtrl( m_panel30, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS|wxTE_PROCESS_ENTER|wxTAB_TRAVERSAL, 0, 9999, 0 );
	fgSizer1->Add( ui_spinHeight, 0, wxALL|wxEXPAND, 5 );

	m_staticText3 = new wxStaticText( m_panel30, wxID_ANY, wxT("fps"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	wxArrayString ui_choiceFpsChoices;
	ui_choiceFps = new wxChoice( m_panel30, wxID_ANY, wxDefaultPosition, wxDefaultSize, ui_choiceFpsChoices, 0|wxTAB_TRAVERSAL );
	ui_choiceFps->SetSelection( 0 );
	fgSizer1->Add( ui_choiceFps, 0, wxALL|wxEXPAND, 5 );

	m_staticText5 = new wxStaticText( m_panel30, wxID_ANY, wxT("method"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( 1 );
	fgSizer1->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxArrayString m_choice2Choices;
	m_choice2 = new wxChoice( m_panel30, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice2Choices, 0|wxTAB_TRAVERSAL );
	m_choice2->SetSelection( 0 );
	fgSizer1->Add( m_choice2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer3->Add( fgSizer1, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	ui_chkUsingTempFile = new wxCheckBox( m_panel30, wxID_ANY, wxT("Store image as file temporally (it makes using memory less)"), wxDefaultPosition, wxDefaultSize, 0|wxTAB_TRAVERSAL );
	ui_chkUsingTempFile->SetValue(true);
	bSizer5->Add( ui_chkUsingTempFile, 0, wxALL, 5 );


	bSizer3->Add( bSizer5, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	ui_btnStart = new wxButton( m_panel30, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0|wxTAB_TRAVERSAL );
	bSizer4->Add( ui_btnStart, 1, wxALL|wxEXPAND, 5 );

	ui_btnStop = new wxButton( m_panel30, wxID_ANY, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0|wxTAB_TRAVERSAL );
	bSizer4->Add( ui_btnStop, 1, wxALL|wxEXPAND, 5 );


	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );

	ui_lblLog = new wxStaticText( m_panel30, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	ui_lblLog->Wrap( -1 );
	bSizer3->Add( ui_lblLog, 0, wxALL|wxEXPAND, 5 );


	m_panel30->SetSizer( bSizer3 );
	m_panel30->Layout();
	bSizer3->Fit( m_panel30 );
	bSizer8->Add( m_panel30, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer8 );
	this->Layout();

	this->Centre( wxBOTH );
}

UIRecordFrame::~UIRecordFrame()
{
}

MyFrame3::MyFrame3( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_scrolledWindow1 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxALWAYS_SHOW_SB|wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	m_panel4 = new wxPanel( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxSize( 500,500 ), wxTAB_TRAVERSAL );
	m_panel4->SetBackgroundColour( wxColour( 255, 0, 0 ) );

	bSizer7->Add( m_panel4, 1, wxEXPAND | wxALL, 5 );

	m_panel45 = new wxPanel( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxSize( 500,500 ), wxTAB_TRAVERSAL );
	bSizer7->Add( m_panel45, 1, wxEXPAND | wxALL, 5 );

	m_panel44 = new wxPanel( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxSize( 500,500 ), wxTAB_TRAVERSAL );
	bSizer7->Add( m_panel44, 1, wxEXPAND | wxALL, 5 );

	m_panel43 = new wxPanel( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxSize( 500,500 ), wxTAB_TRAVERSAL );
	bSizer7->Add( m_panel43, 1, wxEXPAND | wxALL, 5 );

	m_panel42 = new wxPanel( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxSize( 500,500 ), wxTAB_TRAVERSAL );
	bSizer7->Add( m_panel42, 1, wxEXPAND | wxALL, 5 );

	m_panel41 = new wxPanel( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxSize( 500,500 ), wxTAB_TRAVERSAL );
	bSizer7->Add( m_panel41, 1, wxEXPAND | wxALL, 5 );


	m_scrolledWindow1->SetSizer( bSizer7 );
	m_scrolledWindow1->Layout();
	bSizer7->Fit( m_scrolledWindow1 );
	bSizer6->Add( m_scrolledWindow1, 1, wxEXPAND | wxALL, 5 );

	m_treeCtrl1 = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer6->Add( m_treeCtrl1, 0, wxALL, 5 );

	m_treeListCtrl1 = new wxTreeListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_DEFAULT_STYLE );

	bSizer6->Add( m_treeListCtrl1, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer6 );
	this->Layout();

	this->Centre( wxBOTH );
}

MyFrame3::~MyFrame3()
{
}

UIResizeDialog::UIResizeDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Scale To"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer13->Add( m_staticText9, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl4 = new wxSpinCtrl( this, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 50, 300, 100, wxT("ui_scale") );
	bSizer13->Add( m_spinCtrl4, 0, wxALL, 5 );

	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("%"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer13->Add( m_staticText10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer12->Add( bSizer13, 1, wxEXPAND, 5 );

	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();

	bSizer12->Add( m_sdbSizer2, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer12 );
	this->Layout();
	bSizer12->Fit( this );

	this->Centre( wxBOTH );
}

UIResizeDialog::~UIResizeDialog()
{
}
