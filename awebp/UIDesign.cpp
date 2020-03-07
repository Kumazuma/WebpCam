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

UIEncodingDialog::UIEncodingDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("파일 종류"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer2->Add( m_staticText13, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_choice5Choices;
	m_choice5 = new wxChoice( this, ID_FILE_TYPE, wxDefaultPosition, wxDefaultSize, m_choice5Choices, 0 );
	m_choice5->SetSelection( 0 );
	fgSizer2->Add( m_choice5, 0, wxALL|wxEXPAND, 5 );

	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("퀄리티"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer2->Add( m_staticText14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spinCtrl8 = new wxSpinCtrl( this, ID_QUALITY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS|wxTE_PROCESS_ENTER, 0, 100, 1 );
	fgSizer2->Add( m_spinCtrl8, 0, wxALL, 5 );


	bSizer19->Add( fgSizer2, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText15 = new wxStaticText( this, ID_PROGRESS_LABEL, wxT("0/0(0%)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer22->Add( m_staticText15, 0, wxALL, 5 );

	m_gauge1 = new wxGauge( this, ID_PROGRESS_GAUGE, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	m_gauge1->SetValue( 0 );
	bSizer22->Add( m_gauge1, 0, wxALL|wxEXPAND, 5 );


	bSizer19->Add( bSizer22, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );

	m_button9 = new wxButton( this, ID_SAVE, wxT("저장"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_button9, 1, wxALL|wxEXPAND, 5 );

	m_button10 = new wxButton( this, ID_CANCEL, wxT("취소"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_button10, 1, wxALL|wxEXPAND, 5 );


	bSizer19->Add( bSizer21, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );


	this->SetSizer( bSizer19 );
	this->Layout();
	bSizer19->Fit( this );

	this->Centre( wxBOTH );
}

UIEncodingDialog::~UIEncodingDialog()
{
}
