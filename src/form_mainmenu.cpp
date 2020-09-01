#include "gui.hpp"
#include <wx/wx.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/gallery.h>
#include <wx/ribbon/toolbar.h>
#include <wx/artprov.h>
#include <wx/bitmap.h>

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

	wxString wnd_title;
	wnd_title << owner->m_lang->str_minerva_school_timetables << " - " << wxString::FromUTF8(m_owner->m_school->name);
	SetTitle(wnd_title);
	SetMinSize(wxSize(800,600));
	SetBackgroundColour(wxColor(0x29, 0x80, 0xb9));

	wxRibbonBar * m_ribbon = new wxRibbonBar(this,-1,wxDefaultPosition, wxSize(800,150), wxRIBBON_BAR_FLOW_HORIZONTAL | wxRIBBON_BAR_SHOW_PAGE_LABELS);
	m_ribbon->SetArtProvider(new wxRibbonMetroArtProvider(true, m_owner->m_small_font));
	wxRibbonButtonBar * m_rib_bbars[7][5];

	const wchar_t * const menu_names[7] = {
		m_owner->m_lang->str_school,
		m_owner->m_lang->str_rooms,
		m_owner->m_lang->str_subjects,
		m_owner->m_lang->str_teachers,
		m_owner->m_lang->str_classes,
		m_owner->m_lang->str_events,
		m_owner->m_lang->str_timetable
	};
	const wchar_t * const smenu_names[7][7] = {
		{m_owner->m_lang->str_data, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_help, NULL}
	};

	for(int i = 0; i < 7; ++i){
		m_ribbon_pages[i] = new wxRibbonPage(m_ribbon, wxID_ANY, (menu_names[i]));
		for(int j = 0; smenu_names[i][j] != NULL; ++j){
			m_rib_bbars[i][j] = new wxRibbonButtonBar(new wxRibbonPanel(m_ribbon_pages[i], wxID_ANY, smenu_names[i][j], wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE));
		}
	}
	m_ribbon->Bind(wxEVT_RIBBONBUTTONBAR_CLICKED, &MainMenuForm::OnMenuItemClicked, this);

	wxBitmap image_add("res/twotone_create_black_18dp.png", wxBITMAP_TYPE_PNG);
	wxBitmap image_build("res/twotone_build_black_18dp.png", wxBITMAP_TYPE_PNG);
	wxBitmap image_config("res/twotone_miscellaneous_services_black_18dp.png", wxBITMAP_TYPE_PNG);
	wxBitmap image_export("res/twotone_photo_library_black_18dp.png", wxBITMAP_TYPE_PNG);
	wxBitmap image_list("res/twotone_list_black_18dp.png", wxBITMAP_TYPE_PNG);
	wxBitmap image_check("res/twotone_check_box_black_18dp.png", wxBITMAP_TYPE_PNG);
	wxBitmap image_undo("res/undo.png", wxBITMAP_TYPE_PNG);
	wxBitmap image_redo("res/redo.png", wxBITMAP_TYPE_PNG);
	wxBitmap image_save("res/save.png", wxBITMAP_TYPE_PNG);
	wxBitmap image_help("res/manual.png", wxBITMAP_TYPE_PNG);
	wxBitmap image_close("res/cancel.png", wxBITMAP_TYPE_PNG);

	m_toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT);

	m_toolbar->AddTool(ID_SAVE, m_owner->m_lang->str_save, image_save);
	m_toolbar->AddTool(ID_UNDO, m_owner->m_lang->str_undo, image_undo);
	m_toolbar->AddTool(ID_REDO, m_owner->m_lang->str_redo, image_redo);
	m_toolbar->AddTool(ID_CLOSE, m_owner->m_lang->str_close, image_close);

	m_toolbar->SetToolDisabledBitmap(ID_SAVE, wxNullBitmap);
	m_toolbar->SetToolDisabledBitmap(ID_UNDO, wxNullBitmap);
	m_toolbar->SetToolDisabledBitmap(ID_REDO, wxNullBitmap);
	m_toolbar->SetToolDisabledBitmap(ID_CLOSE, wxNullBitmap);
	m_toolbar->Bind(wxEVT_MENU, &MainMenuForm::OnToolbarEvent, this);
	m_toolbar->EnableTool(ID_SAVE, false);
	m_toolbar->EnableTool(ID_UNDO, false);
	m_toolbar->EnableTool(ID_REDO, false);
	m_toolbar->Realize();
	/* ESCOLA */
	m_rib_bbars[0][0]->AddButton(ID_SCHOOL_DATA,m_owner->m_lang->str_school_details, image_config);
	m_rib_bbars[0][1]->AddButton(ID_OPEN_SCHOOL_MANUAL,m_owner->m_lang->str_open_manual, image_help);
	/* SALAS E CARACTERISTICAS DE SALAS */
	m_rib_bbars[1][0]->AddButton(ID_VIEW_ROOMS, m_owner->m_lang->str_list_rooms, image_list);
	m_rib_bbars[1][1]->AddButton(ID_ADD_ROOM, m_owner->m_lang->str_add_room, image_add);
	m_rib_bbars[1][2]->AddButton(ID_CHECK_ALL_ROOMS, m_owner->m_lang->str_check_all, image_check);
	m_rib_bbars[1][3]->AddButton(ID_OPEN_ROOMS_MANUAL, m_owner->m_lang->str_open_manual, image_help);
	/* DISCIPLINAS */
	m_rib_bbars[2][0]->AddButton(ID_VIEW_SUBJECTS, m_owner->m_lang->str_list_subjects, image_list);
	m_rib_bbars[2][0]->AddButton(ID_VIEW_SUBJECT_GROUPS, m_owner->m_lang->str_list_subject_groups, image_list);
	m_rib_bbars[2][1]->AddButton(ID_ADD_SUBJECT, m_owner->m_lang->str_add_subject, image_add);
	m_rib_bbars[2][1]->AddButton(ID_ADD_SUBJECT_GROUP, m_owner->m_lang->str_add_subject_group, image_add);
	m_rib_bbars[2][2]->AddButton(ID_CHECK_ALL_SUBJECTS, m_owner->m_lang->str_check_all, image_check);
	m_rib_bbars[2][3]->AddButton(ID_OPEN_SUBJECTS_MANUAL, m_owner->m_lang->str_open_manual, image_help);
	/* PROFESSORES */
	m_rib_bbars[3][0]->AddButton(ID_VIEW_TEACHERS, m_owner->m_lang->str_list_teachers_and_their_groups, image_list);
	m_rib_bbars[3][1]->AddButton(ID_ADD_TEACHER, m_owner->m_lang->str_add_teacher, image_add);
	m_rib_bbars[3][1]->AddButton(ID_ADD_TEACHER_GROUP, m_owner->m_lang->str_add_teacher_group, image_add);
	m_rib_bbars[3][2]->AddButton(ID_CHECK_ALL_TEACHERS, m_owner->m_lang->str_check_all, image_check);
	m_rib_bbars[3][3]->AddButton(ID_OPEN_TEACHERS_MANUAL, m_owner->m_lang->str_open_manual, image_help);
	/* TURMAS */
	m_rib_bbars[4][0]->AddButton(ID_VIEW_CLASSES, m_owner->m_lang->str_list_classes_and_their_groups, image_list);
	m_rib_bbars[4][1]->AddButton(ID_ADD_CLASS, m_owner->m_lang->str_add_class, image_add);
	m_rib_bbars[4][1]->AddButton(ID_ADD_CLASS_GROUP, m_owner->m_lang->str_add_class_group, image_add);
	m_rib_bbars[4][2]->AddButton(ID_CHECK_ALL_CLASSES, m_owner->m_lang->str_check_all, image_check);
	m_rib_bbars[4][3]->AddButton(ID_OPEN_CLASSES_MANUAL, m_owner->m_lang->str_open_manual, image_help);
	/* AULAS */
	m_rib_bbars[5][0]->AddButton(ID_VIEW_LECTURES, m_owner->m_lang->str_list_lectures_by_class, image_list);
	m_rib_bbars[5][0]->AddButton(ID_VIEW_PLANNING_TIMES, m_owner->m_lang->str_list_planning_times, image_list);
	m_rib_bbars[5][1]->AddButton(ID_CHECK_ALL_EVENTS, m_owner->m_lang->str_check_all, image_check);
	m_rib_bbars[5][2]->AddButton(ID_OPEN_EVENTS_MANUAL, m_owner->m_lang->str_open_manual, image_help);
	/* HORÁRIO */
	m_rib_bbars[6][0]->AddButton(ID_VIEW_TIMETABLE, m_owner->m_lang->str_see_timetable, image_list);
	m_rib_bbars[6][0]->AddButton(ID_EXPORT_TIMETABLE, m_owner->m_lang->str_export_timetable, image_export);
	m_rib_bbars[6][1]->AddButton(ID_GENERATE_TIMETABLE, m_owner->m_lang->str_generate_timetable, image_build);
	m_rib_bbars[6][1]->AddButton(ID_CREATE_TIMETABLE, m_owner->m_lang->str_create_timetable_manually, image_build);
	m_rib_bbars[6][2]->AddButton(ID_OPEN_TIMETABLE_MANUAL, m_owner->m_lang->str_open_manual, image_help);

	m_ribbon->AddPageHighlight(m_ribbon->GetPageCount() - 1); /* TODO is it working?*/
	m_ribbon->Realize();

	m_center_pane = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	m_page_title = new wxStaticText(m_center_pane, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,30));
	m_page_detail = new wxStaticText(m_center_pane, wxID_ANY, wxT(""));

	m_page_title->SetFont(*m_owner->m_page_title_font);
	m_page_title->SetForegroundColour(wxColour(255,255,255));
	m_page_title->SetLabel(m_page_title->GetLabel()); // maybe it recalculates size.
	m_page_detail->SetFont(*m_owner->m_small_font);
	m_page_detail->SetForegroundColour(wxColour(255,255,255));

	wxSizer * center_sizer = new wxBoxSizer(wxVERTICAL);
	center_sizer->Add(m_page_title,0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 15);
	center_sizer->Add(m_page_detail,0, wxEXPAND | wxLEFT |  wxRIGHT, 15);
	m_center_pane->SetSizerAndFit(center_sizer);

	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_ribbon, 0, wxEXPAND);
	m_sizer->Add(m_center_pane, 1, wxEXPAND);
	m_sizer->Add(m_toolbar,0,wxEXPAND);

	m_sizer->SetMinSize(wxSize(800,600));
	SetSizerAndFit(m_sizer);
	Bind(wxEVT_CLOSE_WINDOW, &MainMenuForm::OnCloseClose, this);
	Bind(wxEVT_BUTTON, &MainMenuForm::OnSave, this, ID_SAVE);
	Bind(wxEVT_BUTTON, &MainMenuForm::OnCloseCommand, this, ID_CLOSE);
	Bind(wxEVT_BUTTON, &MainMenuForm::OnUndo, this, ID_UNDO);
	Bind(wxEVT_BUTTON, &MainMenuForm::OnRedo, this, ID_REDO);
	wxAcceleratorEntry keyboard_shortcuts[4];
	keyboard_shortcuts[0].Set(wxACCEL_CTRL, (int)'S', ID_SAVE);
	keyboard_shortcuts[1].Set(wxACCEL_CTRL, (int)'Q', ID_CLOSE);
	keyboard_shortcuts[2].Set(wxACCEL_CTRL, (int)'Z', ID_UNDO);
	keyboard_shortcuts[3].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'Z', ID_REDO);
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
	bool needs_to_save = m_toolbar->GetToolEnabled(ID_SAVE);
	bool to_close = false;
	if(needs_to_save){
		wxMessageDialog * dialog = new wxMessageDialog(nullptr, m_owner->m_lang->str_confirm_close_without_saving, m_owner->m_lang->str_are_you_sure, wxCANCEL | wxYES_NO);
		dialog->SetYesNoCancelLabels(m_owner->m_lang->str_close_and_save, m_owner->m_lang->str_close_without_saving, m_owner->m_lang->str_cancel);
		int confirmation = dialog->ShowModal();
		switch(confirmation){
			case wxID_YES: {
				m_owner->SaveDatabase();
				to_close = true;
			}
			case wxID_NO: {
				to_close = true;
			}
			case wxID_CANCEL: {
				/* Empty on purpouse */
				break;
			}
		}
	}
	if(to_close || !needs_to_save){
		free_school(m_owner->m_school);
		m_owner->m_school = nullptr;
		Destroy();
		return true;
	}
	return false;
}

void MainMenuForm::NotifyNewAction(Action * action){
	Notification * notification = new Notification(m_owner, this, wxID_ANY, action, wxDefaultPosition, wxDefaultSize);
	notification->GetTimer()->Bind(wxEVT_TIMER, &MainMenuForm::OnNotificationTimer, this);
	notification->GetButton()->Bind(wxEVT_HYPERLINK, &MainMenuForm::OnNotificationAction, this);

	AddNotification(notification);
	m_toolbar->EnableTool(ID_SAVE, true);
	m_toolbar->EnableTool(ID_REDO, false);
	m_toolbar->EnableTool(ID_UNDO, true);
}

void MainMenuForm::NotifyNewUnsavedData(){
	Notification * notification = new Notification(m_owner, this, wxID_ANY, m_owner->m_lang->str_success, m_owner->m_lang->str_undo, wxDefaultPosition, wxDefaultSize);
	notification->GetTimer()->Bind(wxEVT_TIMER, &MainMenuForm::OnNotificationTimer, this);
	notification->GetButton()->Bind(wxEVT_HYPERLINK, &MainMenuForm::OnNotificationAction, this);

	AddNotification(notification);
	m_toolbar->EnableTool(ID_SAVE, true);
}

void MainMenuForm::OnSave(wxCommandEvent & evt){
	if(m_toolbar->GetToolEnabled(ID_SAVE)){
		m_owner->SaveDatabase();
		m_toolbar->EnableTool(ID_SAVE, false);
	}
}

void MainMenuForm::OnNotificationAction(wxHyperlinkEvent & ev){
	ev.Skip();
	Layout();
	if(!m_owner->m_actions.m_undo_list.empty()){
		m_toolbar->EnableTool(ID_UNDO, true);
	}
	if(!m_owner->m_actions.m_redo_list.empty()){
		m_toolbar->EnableTool(ID_REDO, true);
	}
	m_toolbar->EnableTool(ID_SAVE, true);
}

void MainMenuForm::OnNotificationTimer(wxTimerEvent & ev){
	ev.Skip();
	Layout();
}

void MainMenuForm::OnUndo(wxCommandEvent &){
	if(!m_owner->m_actions.m_undo_list.empty()){
		printf("Undo!\n");
		m_owner->m_actions.Undo();
		m_toolbar->EnableTool(ID_REDO, true);
		if(m_owner->m_actions.m_undo_list.empty()){
			m_toolbar->EnableTool(ID_UNDO, false);
		}
		m_toolbar->EnableTool(ID_SAVE, true);
	}
}

void MainMenuForm::OnRedo(wxCommandEvent &){
	if(!m_owner->m_actions.m_redo_list.empty()){
		printf("Redo!\n");
		m_owner->m_actions.Redo();
		m_toolbar->EnableTool(ID_UNDO, true);
		if(m_owner->m_actions.m_redo_list.empty()){
			m_toolbar->EnableTool(ID_REDO, false);
		}
		m_toolbar->EnableTool(ID_SAVE, true);
	}
}

void MainMenuForm::OnToolbarEvent(wxCommandEvent & evt){
	switch(evt.GetId()){
		case ID_SAVE:{
			OnSave(evt);
			break;
		}
		case ID_UNDO:{
			OnUndo(evt);
			break;
		}
		case ID_REDO:{
			OnRedo(evt);
			break;
		}
		case ID_CLOSE:{
			OnClose();
			break;
		}
		case ID_HELP:{
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
	wxString title, detail;
	wxPanel * pane_to_open = nullptr;
	switch(ev.GetId()){
		/* School */
		case ID_SCHOOL_DATA: {
			pane_to_open = new DescSchoolPane(m_owner, m_center_pane, wxPoint(100,15));
			title = m_owner->m_lang->str_school_details;
			detail = m_owner->m_lang->str_lorem;
			break;
		}
		/* Room */
		case ID_ADD_ROOM: {
			pane_to_open = new AddRoomPane(m_owner, m_center_pane, wxPoint(100,15));
			title = m_owner->m_lang->str_add_room;
			detail = m_owner->m_lang->str_lorem;
			break;
		}
		case ID_VIEW_ROOMS:{
			pane_to_open = new ListRoomsPane(m_owner, m_center_pane, wxPoint(100,15));
			title = m_owner->m_lang->str_list_of_rooms;
			detail = m_owner->m_lang->str_lorem;
			break;
		}
		case ID_CHECK_ALL_ROOMS:{
			break;
		}
		/* Subject */
		case ID_ADD_SUBJECT:{
			title = m_owner->m_lang->str_add_subject;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new AddSubjectPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_ADD_SUBJECT_GROUP:{
			title = m_owner->m_lang->str_add_subject_group;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new AddSubjectGroupPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_VIEW_SUBJECTS:{
			title = m_owner->m_lang->str_list_of_subjects;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new ListSubjectsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_VIEW_SUBJECT_GROUPS:{
			title = m_owner->m_lang->str_list_of_subject_groups;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open =new ListSubjectGroupsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_CHECK_ALL_SUBJECTS:{
			printf("Not there yet.\n");
			break;
		}
		/* Teacher */
		case ID_ADD_TEACHER:{
			title = m_owner->m_lang->str_add_teacher;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new AddTeacherPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_ADD_TEACHER_GROUP:{
			title = m_owner->m_lang->str_add_teacher_group;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new AddTeacherGroupPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_VIEW_TEACHERS:{
			title = m_owner->m_lang->str_list_of_teachers_and_their_groups;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new ListTeachersPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_CHECK_ALL_TEACHERS:{
			printf("Not there yet.\n");
			break;
		}
		/* Class */
		case ID_ADD_CLASS:{
			title = m_owner->m_lang->str_add_class;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new AddClassPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_ADD_CLASS_GROUP:{
			title = m_owner->m_lang->str_add_class_group;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new AddClassGroupPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_VIEW_CLASSES:{
			title = m_owner->m_lang->str_list_classes_and_their_groups;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new ListClassesPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_CHECK_ALL_CLASSES:{
			printf("Not there yet.\n");
			break;
		}
		/* Lecture */
		case ID_VIEW_LECTURES: {
			title = m_owner->m_lang->str_list_of_lectures;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new ListLecturesPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_VIEW_PLANNING_TIMES: {
			title = m_owner->m_lang->str_list_of_planning_times;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new ListPlanningTimesPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_CHECK_ALL_EVENTS: {
			printf("Not there yet.\n");
			break;
		}
		/* Timetable */
		case ID_GENERATE_TIMETABLE:{
			title = m_owner->m_lang->str_generate_timetable;
			detail = m_owner->m_lang->str_lorem;
			pane_to_open = new GenerateTimetablePane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case ID_VIEW_TIMETABLE:{
			title = m_owner->m_lang->str_see_timetable;
			pane_to_open = new DescTimetablePane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		/* Manual */
		case ID_OPEN_SCHOOL_MANUAL:
		case ID_OPEN_ROOMS_MANUAL:
		case ID_OPEN_SUBJECTS_MANUAL:
		case ID_OPEN_TEACHERS_MANUAL:
		case ID_OPEN_CLASSES_MANUAL:
		case ID_OPEN_EVENTS_MANUAL:
		case ID_OPEN_TIMETABLE_MANUAL:{
			title = m_page_title->GetLabel();
			detail = m_page_detail->GetLabel();
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
	if(pane_to_open != nullptr){
		CloseOpenedPane();
		m_open_pane = pane_to_open;
		m_page_title->SetLabel(title);
		m_page_detail->SetLabel(detail);
		m_center_pane->GetSizer()->Add(m_open_pane,1, wxEXPAND | wxALL, 15);
	}
	Layout();
}
