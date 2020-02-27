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
	m_ribbonToolBar1->AddTool( ID_CROP_TOOL, wxArtProvider::GetBitmap( wxART_CUT, wxART_TOOLBAR ), wxEmptyString);
	m_ribbonPanel7 = new wxRibbonPanel( m_ribbonPage1, wxID_ANY, wxT("기타") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonButtonBar4 = new wxRibbonButtonBar( m_ribbonPanel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ribbonButtonBar4->AddButton( ID_STORE_WINDOW, wxT("창복구"), wxBitmap( wxT("restore.png"), wxBITMAP_TYPE_ANY ), wxEmptyString);
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

MyFrame5::MyFrame5( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	m_bpButton1 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );

	m_bpButton1->SetBitmap( wxBitmap( wxT("C:\\Users\\qwead\\Downloads\\record.png"), wxBITMAP_TYPE_ANY ) );
	m_bpButton1->SetBitmapDisabled( wxBitmap( wxT("C:\\Users\\qwead\\Downloads\\record.png"), wxBITMAP_TYPE_ANY ) );
	m_bpButton1->SetBitmapPressed( wxBitmap( wxT("C:\\Users\\qwead\\Downloads\\record_press.png"), wxBITMAP_TYPE_ANY ) );
	m_bpButton1->SetBitmapFocus( wxBitmap( wxT("C:\\Users\\qwead\\Downloads\\record.png"), wxBITMAP_TYPE_ANY ) );
	m_bpButton1->SetBitmapCurrent( wxBitmap( wxT("C:\\Users\\qwead\\Downloads\\record_over.png"), wxBITMAP_TYPE_ANY ) );
	m_bpButton1->SetBitmapPosition( wxRIGHT );
	m_bpButton1->SetToolTip( wxT("Record") );

	bSizer13->Add( m_bpButton1, 0, wxALL, 5 );


	this->SetSizer( bSizer13 );
	this->Layout();

	this->Centre( wxBOTH );
}

MyFrame5::~MyFrame5()
{
}

UIEditForm::UIEditForm( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_ALLOW_FLOATING|wxAUI_MGR_LIVE_RESIZE|wxAUI_MGR_RECTANGLE_HINT|wxAUI_MGR_TRANSPARENT_HINT);

	ui_frameList = new FrameListWidget( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("ui_frameList") );
	m_mgr.AddPane( ui_frameList, wxAuiPaneInfo() .Left() .CaptionVisible( false ).CloseButton( false ).Movable( false ).Dock().Resizable().FloatingSize( wxDefaultSize ) );

	m_panel1 = new wxPanel( this, ID_MAIN_PANE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel1, wxAuiPaneInfo() .Center() .Caption( wxT("테스트") ).CaptionVisible( false ).CloseButton( false ).Movable( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( true ).BottomDockable( false ).TopDockable( false ).LeftDockable( false ).RightDockable( false ).Floatable( false ).CentrePane() );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	ui_drawWidget = new Edit::EditRenderWidget( m_panel1, ID_DRAW_WIDGET, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("ui_drawWidget") );
	bSizer2->Add( ui_drawWidget, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	ui_btnButton = new wxButton( m_panel1, ID_PLAY_BUTTON, wxT("재생"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( ui_btnButton, 0, wxALL, 5 );


	bSizer2->Add( bSizer11, 0, wxEXPAND, 5 );


	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_customControl3 = new wxSimpleHtmlListBox(this,ID_HISTORY_LIST);
	m_mgr.AddPane( m_customControl3, wxAuiPaneInfo() .Right() .Caption( wxT("변경 이력") ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ) );

	ui_propertyGrid = new wxPropertyGrid(this, ID_PROPERTY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	m_mgr.AddPane( ui_propertyGrid, wxAuiPaneInfo() .Right() .PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 50,63 ) ).MinSize( wxSize( 200,-1 ) ) );


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
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

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

TopPannel::TopPannel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText18 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer3->Add( m_staticText18, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	ui_spinX = new wxSpinCtrl( this, ID_SPIN_X, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 9999, 0 );
	bSizer3->Add( ui_spinX, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText19 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer3->Add( m_staticText19, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	ui_spinY = new wxSpinCtrl( this, ID_SPIN_Y, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 9999, 0 );
	bSizer3->Add( ui_spinY, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Width"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText1->Wrap( -1 );
	bSizer3->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	ui_spinWidth = new wxSpinCtrl( this, ID_SPIN_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS|wxTE_PROCESS_ENTER|wxTAB_TRAVERSAL, 0, 9999, 0 );
	bSizer3->Add( ui_spinWidth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	ui_spinHeight = new wxSpinCtrl( this, ID_SPIN_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS|wxTE_PROCESS_ENTER|wxTAB_TRAVERSAL, 0, 9999, 0 );
	bSizer3->Add( ui_spinHeight, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );
}

TopPannel::~TopPannel()
{
}

BottomPanel::BottomPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("fps"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer15->Add( m_staticText3, 0, wxALL|wxALIGN_BOTTOM, 5 );

	wxArrayString ui_choiceFpsChoices;
	ui_choiceFps = new wxChoice( this, ID_CHOICE_FPS, wxDefaultPosition, wxDefaultSize, ui_choiceFpsChoices, 0|wxTAB_TRAVERSAL );
	ui_choiceFps->SetSelection( 0 );
	bSizer15->Add( ui_choiceFps, 0, wxRIGHT|wxLEFT|wxALIGN_BOTTOM, 5 );

	ui_btnStart = new wxButton( this, ID_BTN_START, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0|wxTAB_TRAVERSAL );
	bSizer15->Add( ui_btnStart, 0, wxRIGHT|wxLEFT|wxALIGN_BOTTOM, 5 );

	ui_btnStop = new wxButton( this, ID_BTN_STOP, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0|wxTAB_TRAVERSAL );
	bSizer15->Add( ui_btnStop, 0, wxRIGHT|wxLEFT|wxALIGN_BOTTOM, 5 );

	m_button11 = new wxButton( this, ID_BTN_CLOSE, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_button11, 0, wxALIGN_BOTTOM|wxRIGHT|wxLEFT, 5 );


	this->SetSizer( bSizer15 );
	this->Layout();
	bSizer15->Fit( this );
}

BottomPanel::~BottomPanel()
{
}

MyFrame6::MyFrame6( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );

	m_panel11 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel11->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );

	m_button10 = new wxButton( m_panel11, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_button10, 0, wxALL, 5 );


	m_panel11->SetSizer( bSizer18 );
	m_panel11->Layout();
	bSizer18->Fit( m_panel11 );
	bSizer16->Add( m_panel11, 0, wxEXPAND | wxALL, 5 );


	bSizer16->Add( 0, 0, 1, wxEXPAND, 5 );

	m_panel12 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );

	m_button9 = new wxButton( m_panel12, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_button9, 0, wxALL, 5 );


	m_panel12->SetSizer( bSizer17 );
	m_panel12->Layout();
	bSizer17->Fit( m_panel12 );
	bSizer16->Add( m_panel12, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer16 );
	this->Layout();

	this->Centre( wxBOTH );
}

MyFrame6::~MyFrame6()
{
}

MyFrame4::MyFrame4( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);

	m_panel10 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel10, wxAuiPaneInfo() .Top() .CaptionVisible( false ).CloseButton( false ).PaneBorder( false ).Movable( false ).Dock().Resizable().FloatingSize( wxDefaultSize ) );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_ribbonBar3 = new wxRibbonBar( m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRIBBON_BAR_DEFAULT_STYLE );
	m_ribbonBar3->SetArtProvider(new wxRibbonDefaultArtProvider);
	m_ribbonPage5 = new wxRibbonPage( m_ribbonBar3, wxID_ANY, wxT("MyRibbonPage") , wxNullBitmap , 0 );
	m_ribbonPanel5 = new wxRibbonPanel( m_ribbonPage5, wxID_ANY, wxT("MyRibbonPanel") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonPanel6 = new wxRibbonPanel( m_ribbonPage5, wxID_ANY, wxT("MyRibbonPanel") , wxNullBitmap , wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE );
	m_ribbonBar3->Realize();

	bSizer12->Add( m_ribbonBar3, 1, wxEXPAND, 5 );


	m_panel10->SetSizer( bSizer12 );
	m_panel10->Layout();
	bSizer12->Fit( m_panel10 );
	m_panel11 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel11, wxAuiPaneInfo() .Left() .PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).CentrePane() );

	m_genericDirCtrl1 = new wxGenericDirCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_3D_INTERNAL|wxSUNKEN_BORDER, wxEmptyString, 0 );

	m_genericDirCtrl1->ShowHidden( false );
	m_mgr.AddPane( m_genericDirCtrl1, wxAuiPaneInfo() .Right() .PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ) );


	m_mgr.Update();
	this->Centre( wxBOTH );
}

MyFrame4::~MyFrame4()
{
	m_mgr.UnInit();

}
