#include "gui.hpp"
#include <wx/wx.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/gallery.h>
#include <wx/ribbon/toolbar.h>
#include <wx/artprov.h>

#include "gui_language.hpp"
#include "art_metro.h"

extern "C" {
	#include "util.h"
};

MainMenuForm::MainMenuForm(Application * owner)  : wxFrame(nullptr, wxID_ANY, wxT(""), wxPoint(30,30), wxSize(800,600)){
	m_owner = owner;

	SetFont(*m_owner->m_text_font);

	#ifdef __WXMSW__
		SetIcon(wxICON(aaaaaaaa));
	#endif

	wxString title;
	title << owner->m_lang->str_minerva_school_timetables << " - " << wxString::FromUTF8(m_owner->m_school->name);
	SetTitle(title);
	SetMinSize(wxSize(800,600));
	SetBackgroundColour(wxColor(0x29, 0x80, 0xb9));

	wxRibbonBar * m_ribbon = new wxRibbonBar(this,-1,wxDefaultPosition, wxSize(800,150), wxRIBBON_BAR_FLOW_HORIZONTAL | wxRIBBON_BAR_SHOW_PAGE_LABELS);
	m_ribbon->SetArtProvider(new wxRibbonMetroArtProvider(true, m_owner->m_small_font));
	wxRibbonButtonBar * m_rib_bbars[7][5];

	const wchar_t * const menu_names[7] = {m_owner->m_lang->str_school, m_owner->m_lang->str_rooms, m_owner->m_lang->str_subjects, m_owner->m_lang->str_teachers, m_owner->m_lang->str_classes, m_owner->m_lang->str_lectures, m_owner->m_lang->str_timetable};
	const wchar_t * const smenu_names[7][7] = {
		{m_owner->m_lang->str_data, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_help, NULL}
	};

	for(int i = 0; i < 7; ++i){
		m_ribbon_pages[i] = new wxRibbonPage(m_ribbon, wxID_ANY, (menu_names[i]));
		for(int j = 0; smenu_names[i][j] != NULL; ++j){
			m_rib_bbars[i][j] = new wxRibbonButtonBar(new wxRibbonPanel(m_ribbon_pages[i], wxID_ANY, smenu_names[i][j], wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE));
		}
	}
	m_ribbon->Bind(wxEVT_RIBBONBUTTONBAR_CLICKED, &MainMenuForm::OnMenuItemClicked, this);

	auto image	      = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, wxSize(32,32));
	auto image_add    = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, wxSize(32,32));
	auto image_help   = wxArtProvider::GetBitmap(wxART_HELP, wxART_TOOLBAR, wxSize(32,32));
	// auto image_saveas = wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR, wxSize(1,32));
	auto image_list   = wxArtProvider::GetBitmap(wxART_LIST_VIEW, wxART_TOOLBAR, wxSize(32,32));
	auto image_detail = wxArtProvider::GetBitmap(wxART_CLOSE, wxART_TOOLBAR, wxSize(32,32));
	auto image_helpsm = wxArtProvider::GetBitmap(wxART_HELP, wxART_TOOLBAR, wxSize(16,16));
	auto image_undo   = wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR, wxSize(16,16));
	auto image_redo   = wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR, wxSize(16,16));
	auto image_save   = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, wxSize(16,16));
	auto image_close  = wxArtProvider::GetBitmap(wxART_CLOSE, wxART_TOOLBAR, wxSize(16,16));

	m_toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_TEXT);
	m_toolbar->AddTool(LHID_OF(LHN_SAVE), m_owner->m_lang->str_save, image_save);
	m_toolbar->AddTool(LHID_OF(LHN_UNDO), m_owner->m_lang->str_undo, image_undo);
	m_toolbar->AddTool(LHID_OF(LHN_REDO), m_owner->m_lang->str_redo, image_redo);
	m_toolbar->AddTool(LHID_OF(LHN_CLOSE), m_owner->m_lang->str_close, image_close);
	m_toolbar->AddTool(LHID_OF(LHN_HELP), m_owner->m_lang->str_help, image_helpsm);
	m_toolbar->Bind(wxEVT_MENU, &MainMenuForm::OnToolbarEvent, this);
	m_toolbar->EnableTool(LHID_OF(LHN_SAVE), false);
	m_toolbar->EnableTool(LHID_OF(LHN_UNDO), false);
	m_toolbar->EnableTool(LHID_OF(LHN_REDO), false);
	m_toolbar->Realize();
	/* ESCOLA */
	m_rib_bbars[0][0]->AddButton(LHID_OF(LHN_SCHOOL_DATA),m_owner->m_lang->str_school_details, image_list);
	m_rib_bbars[0][1]->AddButton(LHID_OF(LHN_OPEN_SCHOOL_MANUAL),m_owner->m_lang->str_open_manual, image_help);
	/* SALAS E CARACTERISTICAS DE SALAS */
	m_rib_bbars[1][0]->AddButton(LHID_OF(LHN_SEE_ROOMS), m_owner->m_lang->str_list_rooms, image_list);
	m_rib_bbars[1][1]->AddButton(LHID_OF(LHN_ADD_ROOM), m_owner->m_lang->str_add_room, image_add);
	m_rib_bbars[1][2]->AddButton(LHID_OF(LHN_CHECK_ALL_ROOMS), m_owner->m_lang->str_check_all, image_detail);
	m_rib_bbars[1][3]->AddButton(LHID_OF(LHN_OPEN_ROOMS_MANUAL), m_owner->m_lang->str_open_manual, image_detail);
	/* DISCIPLINAS */
	m_rib_bbars[2][0]->AddButton(LHID_OF(LHN_SEE_SUBJECTS), m_owner->m_lang->str_list_subjects, image_list);
	m_rib_bbars[2][0]->AddButton(LHID_OF(LHN_SEE_SUBJECT_GROUPS), m_owner->m_lang->str_list_subject_groups, image_detail);
	m_rib_bbars[2][1]->AddButton(LHID_OF(LHN_ADD_SUBJECT), m_owner->m_lang->str_add_subject, image_add);
	m_rib_bbars[2][1]->AddButton(LHID_OF(LHN_ADD_SUBJECT_GROUP), m_owner->m_lang->str_add_subject_group, image_add);
	m_rib_bbars[2][2]->AddButton(LHID_OF(LHN_CHECK_ALL_SUBJECTS), m_owner->m_lang->str_check_all, image_detail);
	m_rib_bbars[2][3]->AddButton(LHID_OF(LHN_OPEN_SUBJECTS_MANUAL), m_owner->m_lang->str_open_manual, image_help);
	/* PROFESSORES */
	m_rib_bbars[3][0]->AddButton(LHID_OF(LHN_SEE_TEACHERS), m_owner->m_lang->str_list_teachers, image_list);
	m_rib_bbars[3][0]->AddButton(LHID_OF(LHN_SEE_TEACHER_GROUPS), m_owner->m_lang->str_list_teacher_groups, image_detail);
	m_rib_bbars[3][1]->AddButton(LHID_OF(LHN_ADD_TEACHER), m_owner->m_lang->str_add_teacher, image_add);
	m_rib_bbars[3][1]->AddButton(LHID_OF(LHN_ADD_TEACHER_GROUP), m_owner->m_lang->str_add_teacher_group, image_detail);
	m_rib_bbars[3][2]->AddButton(LHID_OF(LHN_CHECK_ALL_TEACHERS), m_owner->m_lang->str_check_all, image_detail);
	m_rib_bbars[3][3]->AddButton(LHID_OF(LHN_OPEN_TEACHERS_MANUAL), m_owner->m_lang->str_open_manual, image_help);
	/* TURMAS */
	m_rib_bbars[4][0]->AddButton(LHID_OF(LHN_SEE_CLASSES), m_owner->m_lang->str_list_classes, image_list);
	m_rib_bbars[4][0]->AddButton(LHID_OF(LHN_SEE_CLASS_GROUPS), m_owner->m_lang->str_list_class_groups, image_detail);
	m_rib_bbars[4][1]->AddButton(LHID_OF(LHN_ADD_CLASS), m_owner->m_lang->str_add_class, image_add);
	m_rib_bbars[4][1]->AddButton(LHID_OF(LHN_ADD_CLASS_GROUP), m_owner->m_lang->str_add_class_group, image_detail);
	m_rib_bbars[4][2]->AddButton(LHID_OF(LHN_CHECK_ALL_CLASSES), m_owner->m_lang->str_check_all, image_detail);
	m_rib_bbars[4][3]->AddButton(LHID_OF(LHN_OPEN_CLASSES_MANUAL), m_owner->m_lang->str_open_manual, image_help);
	/* AULAS */
	m_rib_bbars[5][0]->AddButton(LHID_OF(LHN_SEE_LECTURES), m_owner->m_lang->str_list_lectures_by_class, image_list);
	m_rib_bbars[5][1]->AddButton(LHID_OF(LHN_ADD_LECTURES), m_owner->m_lang->str_add_lectures_by_class, image_add);
	m_rib_bbars[5][2]->AddButton(LHID_OF(LHN_CHECK_ALL_LECTURES), m_owner->m_lang->str_check_all, image_detail);
	m_rib_bbars[5][3]->AddButton(LHID_OF(LHN_OPEN_LECTURES_MAUAL), m_owner->m_lang->str_open_manual, image_help);
	/* HORÁRIO */
	m_rib_bbars[6][0]->AddButton(LHID_OF(LHN_SEE_TIMETABLE), m_owner->m_lang->str_see_timetable, image_list);
	m_rib_bbars[6][0]->AddButton(LHID_OF(LHN_EXPORT_TIMETABLE), m_owner->m_lang->str_export_timetable, image_detail);
	m_rib_bbars[6][1]->AddButton(LHID_OF(LHN_GENERATE_TIMETABLE), m_owner->m_lang->str_generate_timetable, image_add);
	m_rib_bbars[6][1]->AddButton(LHID_OF(LHN_CREATE_TIMETABLE), m_owner->m_lang->str_create_timetable_manually, image_detail);
	m_rib_bbars[6][2]->AddButton(LHID_OF(LHN_OPEN_TIMETABLE_MANUAL), m_owner->m_lang->str_open_manual, image_help);

	m_ribbon->AddPageHighlight(m_ribbon->GetPageCount() - 1); /* TODO is it working?*/
	m_ribbon->Realize();

	m_center_pane = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_ribbon, 0, wxEXPAND);
	m_sizer->Add(m_center_pane, 1, wxEXPAND);
	m_sizer->Add(m_toolbar,0,wxEXPAND);

	m_sizer->SetMinSize(wxSize(800,600));
	SetSizerAndFit(m_sizer);
	Bind(wxEVT_CLOSE_WINDOW, &MainMenuForm::OnCloseClose, this);
	Bind(wxEVT_BUTTON, &MainMenuForm::OnSave, this,LHID_OF(LHN_SAVE));
	Bind(wxEVT_BUTTON, &MainMenuForm::OnCloseCommand, this,LHID_OF(LHN_CLOSE));
	Bind(wxEVT_BUTTON, &MainMenuForm::OnUndo, this,LHID_OF(LHN_UNDO));
	Bind(wxEVT_BUTTON, &MainMenuForm::OnRedo, this,LHID_OF(LHN_REDO));
	wxAcceleratorEntry keyboard_shortcuts[4];
	keyboard_shortcuts[0].Set(wxACCEL_CTRL, (int)'S', LHID_OF(LHN_SAVE));
	keyboard_shortcuts[1].Set(wxACCEL_CTRL, (int)'Q', LHID_OF(LHN_CLOSE));
	keyboard_shortcuts[2].Set(wxACCEL_CTRL, (int)'Z', LHID_OF(LHN_UNDO));
	keyboard_shortcuts[3].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'Z', LHID_OF(LHN_REDO));
	wxAcceleratorTable shortcuts_table(4,keyboard_shortcuts);
	SetAcceleratorTable(shortcuts_table);
}

void MainMenuForm::AddNotification(Notification * notification){
	m_sizer->Insert(2, notification, 0, wxEXPAND);
	Layout();
	notification->Start();
}

void MainMenuForm::OnCloseCommand(wxCommandEvent &evt){
	if(OnClose()){
		evt.Skip();
	}
}

void MainMenuForm::OnCloseClose(wxCloseEvent &evt){
	if(OnClose()){
		evt.Skip();
	}
}

bool MainMenuForm::OnClose(){
	if(m_toolbar->GetToolEnabled(LHID_OF(LHN_SAVE))){
		wxMessageDialog * dialog = new wxMessageDialog(nullptr, m_owner->m_lang->str_confirm_close_without_saving, m_owner->m_lang->str_are_you_sure, wxCANCEL | wxYES_NO);
		dialog->SetYesNoCancelLabels(m_owner->m_lang->str_close_and_save, m_owner->m_lang->str_close_without_saving, m_owner->m_lang->str_cancel);
		int confirmation = dialog->ShowModal();
		switch(confirmation){
			case wxID_YES: {
				m_owner->SaveDatabase();
			} /* Fallthrough */
			case wxID_NO: {
				free_school(m_owner->m_school);
				m_owner->m_school = nullptr;
				Destroy();
				return true;
				// break;
			}
			case wxID_CANCEL: {
				/* Empty on purpouse */
				break;
			}
		}
	} else {
		free_school(m_owner->m_school);
		m_owner->m_school = nullptr;
		Destroy();
		return true;
	}
}

void MainMenuForm::NotifyNewUnsavedData(){
	Notification * notification = new Notification(m_owner, this, wxID_ANY, wxT("Ação taltaltal Realizada"), wxT("Desfazer"), wxDefaultPosition, wxDefaultSize);
	notification->GetTimer()->Bind(wxEVT_TIMER, &MainMenuForm::OnNotificationTimer, this);
	notification->GetAction()->Bind(wxEVT_HYPERLINK, &MainMenuForm::OnNotificationAction, this);

	AddNotification(notification);
	m_toolbar->EnableTool(LHID_OF(LHN_SAVE), true);
}

void MainMenuForm::OnSave(wxCommandEvent & evt){
	if(m_toolbar->GetToolEnabled(LHID_OF(LHN_SAVE))){
		m_owner->SaveDatabase();
		m_toolbar->EnableTool(LHID_OF(LHN_SAVE), false);
	}
}

void MainMenuForm::OnNotificationAction(wxHyperlinkEvent & ev){
	Layout();
	ev.Skip();
}

void MainMenuForm::OnNotificationTimer(wxTimerEvent & ev){
	Layout();
	ev.Skip();
}


void MainMenuForm::OnUndo(wxCommandEvent &){
	printf("Undo!\n");
}

void MainMenuForm::OnRedo(wxCommandEvent &){
	printf("Redo!\n");
}

void MainMenuForm::OnToolbarEvent(wxCommandEvent & evt){
	switch(evt.GetId()){
		case LHID_OF(LHN_SAVE):{
			OnSave(evt);
			break;
		}
		case LHID_OF(LHN_UNDO):{
			OnUndo(evt);
			break;
		}
		case LHID_OF(LHN_REDO):{
			OnRedo(evt);
			break;
		}
		case LHID_OF(LHN_CLOSE):{
			OnClose();
			break;
		}
		case LHID_OF(LHN_HELP):{
			if(m_owner->m_window_manual == nullptr){
				m_owner->m_window_manual = new ManualWindow(m_owner);
				m_owner->m_window_manual->Show();
			}
			break;
		}
	}
}

MainMenuForm::~MainMenuForm(){

}

void MainMenuForm::CloseOpenedPane(){
	if(m_open_pane != nullptr){
		m_open_pane->Destroy();
		m_open_pane = nullptr;
	}
}


void MainMenuForm::OnMenuItemClicked(wxCommandEvent & ev){
	switch(ev.GetId()){
		/* School */
		case LHID_OF(LHN_SCHOOL_DATA): {
			CloseOpenedPane();
			m_open_pane = new DescSchoolPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		/* Room */
		case LHID_OF(LHN_ADD_ROOM): {
			CloseOpenedPane();
			m_open_pane = new AddRoomPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_ROOMS):{
			CloseOpenedPane();
			m_open_pane = new ListRoomsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_CHECK_ALL_ROOMS):{
			break;
		}
		/* Subject */
		case LHID_OF(LHN_ADD_SUBJECT):{
			CloseOpenedPane();
			m_open_pane = new AddSubjectPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_SUBJECT_GROUP):{
			CloseOpenedPane();
			m_open_pane = new AddSubjectGroupPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_SUBJECTS):{
			CloseOpenedPane();
			m_open_pane = new ListSubjectsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_SUBJECT_GROUPS):{
			CloseOpenedPane();
			m_open_pane =new ListSubjectGroupsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_CHECK_ALL_SUBJECTS):{
			printf("Not there yet.\n");
			break;
		}
		/* Teacher */
		case LHID_OF(LHN_ADD_TEACHER):{
			CloseOpenedPane();
			m_open_pane = new AddTeacherPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_TEACHER_GROUP):{
			CloseOpenedPane();
			m_open_pane = new AddTeacherGroupPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_TEACHERS):{
			CloseOpenedPane();
			m_open_pane = new ListTeachersPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_TEACHER_GROUPS):{
			CloseOpenedPane();
			m_open_pane = new ListTeacherGroupsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_CHECK_ALL_TEACHERS):{
			printf("Not there yet.\n");
			break;
		}
		/* Class */
		case LHID_OF(LHN_ADD_CLASS):{
			CloseOpenedPane();
			m_open_pane = new AddClassPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_CLASS_GROUP):{
			CloseOpenedPane();
			m_open_pane = new AddClassGroupPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_CLASSES):{
			CloseOpenedPane();
			m_open_pane = new ListClassesPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_CLASS_GROUPS):{
			CloseOpenedPane();
			m_open_pane = new ListClassGroupsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_CHECK_ALL_CLASSES):{
			printf("Not there yet.\n");
			break;
		}
		/* Lecture */
		case LHID_OF(LHN_SEE_LECTURES): {
			CloseOpenedPane();
			m_open_pane = new ListLecturesPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_LECTURES): {
			CloseOpenedPane();
			m_open_pane = new AddLecturePane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_CHECK_ALL_LECTURES): {
			printf("Not there yet.\n");
			break;
		}
		/* Timetable */
		case LHID_OF(LHN_GENERATE_TIMETABLE):{
			CloseOpenedPane();
			m_open_pane = new GenerateTimetablePane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_TIMETABLE):{
			CloseOpenedPane();
			m_open_pane = new DescTimetablePane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		/* Manual */
		case LHID_OF(LHN_OPEN_SCHOOL_MANUAL):
		case LHID_OF(LHN_OPEN_ROOMS_MANUAL):
		case LHID_OF(LHN_OPEN_SUBJECTS_MANUAL):
		case LHID_OF(LHN_OPEN_TEACHERS_MANUAL):
		case LHID_OF(LHN_OPEN_CLASSES_MANUAL):
		case LHID_OF(LHN_OPEN_LECTURES_MAUAL):
		case LHID_OF(LHN_OPEN_TIMETABLE_MANUAL):{
			if(m_owner->m_window_manual == nullptr){
				m_owner->m_window_manual = new ManualWindow(m_owner);
				m_owner->m_window_manual->Show();
			}
			break;
		}
		default:{
			printf("Event not implemented.\n");
			break;
		}
	}
	if(m_open_pane != nullptr){
		wxSizer * center_sizer = new wxBoxSizer(wxVERTICAL);
		center_sizer->Add(m_open_pane,1, wxEXPAND | wxALL, 15);
		m_center_pane->SetSizerAndFit(center_sizer);
	}
	Layout();
}
