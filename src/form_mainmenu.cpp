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

MainMenuForm::MainMenuForm(Application * owner)  : wxFrame(nullptr, wxID_ANY, owner->m_lang->str_minerva_school_timetables, wxPoint(30,30), wxSize(800,600)){
	m_owner = owner;

	SetFont(*m_owner->m_text_font);

	#ifdef __WXMSW__
		SetIcon(wxICON(aaaaaaaa));
	#endif

	SetMinSize(wxSize(800,600));
	SetBackgroundColour(wxColor(0x29, 0x80, 0xb9));


	wxRibbonBar * m_ribbon = new wxRibbonBar(this,-1,wxDefaultPosition, wxSize(800,150), wxRIBBON_BAR_FLOW_HORIZONTAL | wxRIBBON_BAR_SHOW_PAGE_LABELS);
	m_ribbon->SetArtProvider(new wxRibbonMetroArtProvider(true, m_owner->m_small_font));
	wxRibbonButtonBar * m_rib_bbars[7][5];

	const wchar_t * const menu_names[7] = {m_owner->m_lang->str_school, m_owner->m_lang->str_rooms, m_owner->m_lang->str_subjects, m_owner->m_lang->str_teachers, m_owner->m_lang->str_classes, m_owner->m_lang->str_lectures, m_owner->m_lang->str_timetable};
	const wchar_t * const smenu_names[10][10] = {
		{m_owner->m_lang->str_data, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_preferences, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_preferences, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_preferences, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_preferences, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_preferences, m_owner->m_lang->str_check, m_owner->m_lang->str_help, NULL},
		{m_owner->m_lang->str_view, m_owner->m_lang->str_add, m_owner->m_lang->str_help, NULL}
	};

	for(int i = 0; i < 7; ++i){
		m_ribbon_pages[i] = new wxRibbonPage(m_ribbon, wxID_ANY, wxString(menu_names[i]));
		for(int j = 0; smenu_names[i][j] != NULL; ++j){
			m_rib_bbars[i][j] = new wxRibbonButtonBar(new wxRibbonPanel(m_ribbon_pages[i], wxID_ANY, wxString(smenu_names[i][j])));
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
	m_rib_bbars[1][0]->AddButton(LHID_OF(LHN_SEE_FEATURES), m_owner->m_lang->str_list_features, image_list);
	m_rib_bbars[1][0]->AddButton(LHID_OF(LHN_SEE_ROOMS), m_owner->m_lang->str_list_rooms, image_list);
	m_rib_bbars[1][1]->AddButton(LHID_OF(LHN_ADD_FEATURE), m_owner->m_lang->str_add_feature, image_add);
	m_rib_bbars[1][1]->AddButton(LHID_OF(LHN_ADD_ROOM), m_owner->m_lang->str_add_room, image_add);
	m_rib_bbars[1][2]->AddButton(LHID_OF(LHN_ROOM_PERIOD_PREF), m_owner->m_lang->str_period_preference, image_detail);
	m_rib_bbars[1][3]->AddButton(LHID_OF(LHN_CHECK_ALL_ROOMS), m_owner->m_lang->str_check_all, image_detail);
	m_rib_bbars[1][4]->AddButton(LHID_OF(LHN_OPEN_ROOMS_MANUAL), m_owner->m_lang->str_open_manual, image_detail);
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
	m_rib_bbars[3][2]->AddButton(LHID_OF(LHN_TEACHER_SUBJECT_PREF), m_owner->m_lang->str_subject_preference, image_detail);
	m_rib_bbars[3][2]->AddButton(LHID_OF(LHN_TEACHER_PERIOD_PREF), m_owner->m_lang->str_period_preference, image_detail);
	m_rib_bbars[3][2]->AddButton(LHID_OF(LHN_TEACHER_TWIN_PREF), m_owner->m_lang->str_twin_preference, image_detail);
	m_rib_bbars[3][2]->AddButton(LHID_OF(LHN_TEACHER_ROOM_PREF), m_owner->m_lang->str_room_preference, image_detail);
	m_rib_bbars[3][3]->AddButton(LHID_OF(LHN_CHECK_ALL_TEACHERS), m_owner->m_lang->str_check_all, image_detail);
	m_rib_bbars[3][4]->AddButton(LHID_OF(LHN_OPEN_TEACHERS_MANUAL), m_owner->m_lang->str_open_manual, image_help);
	/* TURMAS */
	m_rib_bbars[4][0]->AddButton(LHID_OF(LHN_SEE_CLASSES), m_owner->m_lang->str_list_classes, image_list);
	m_rib_bbars[4][0]->AddButton(LHID_OF(LHN_SEE_CLASS_GROUPS), m_owner->m_lang->str_list_class_groups, image_detail);
	m_rib_bbars[4][1]->AddButton(LHID_OF(LHN_ADD_CLASS), m_owner->m_lang->str_add_class, image_add);
	m_rib_bbars[4][1]->AddButton(LHID_OF(LHN_ADD_CLASS_GROUP), m_owner->m_lang->str_add_class_group, image_detail);
	m_rib_bbars[4][2]->AddButton(LHID_OF(LHN_CLASS_PERIOD_PREF), m_owner->m_lang->str_period_preference, image_detail);
	m_rib_bbars[4][2]->AddButton(LHID_OF(LHN_CLASS_TEACHER_PREF), m_owner->m_lang->str_teacher_preference, image_detail);
	m_rib_bbars[4][2]->AddButton(LHID_OF(LHN_CLASS_ROOM_PREF), m_owner->m_lang->str_room_preference, image_detail);
	m_rib_bbars[4][3]->AddButton(LHID_OF(LHN_CHECK_ALL_CLASSES), m_owner->m_lang->str_check_all, image_detail);
	m_rib_bbars[4][4]->AddButton(LHID_OF(LHN_OPEN_CLASSES_MANUAL), m_owner->m_lang->str_open_manual, image_help);
	/* AULAS */
	m_rib_bbars[5][0]->AddButton(LHID_OF(LHN_SEE_LECTURES), m_owner->m_lang->str_list_lectures_by_class, image_list);
	m_rib_bbars[5][1]->AddButton(LHID_OF(LHN_ADD_LECTURES), m_owner->m_lang->str_add_lectures_by_class, image_add);
	m_rib_bbars[5][2]->AddButton(LHID_OF(LHN_LECTURE_PERIOD_PREF), m_owner->m_lang->str_period_preference, image_detail);
	m_rib_bbars[5][2]->AddButton(LHID_OF(LHN_LECTURE_TEACHER_PREF), m_owner->m_lang->str_teacher_preference, image_detail);
	m_rib_bbars[5][2]->AddButton(LHID_OF(LHN_LECTURE_TWIN_PREF), m_owner->m_lang->str_twin_preference, image_detail);
	m_rib_bbars[5][2]->AddButton(LHID_OF(LHN_LECTURE_ROOM_PREF), m_owner->m_lang->str_room_preference, image_detail);
	m_rib_bbars[5][3]->AddButton(LHID_OF(LHN_CHECK_ALL_LECTURES), m_owner->m_lang->str_check_all, image_detail);
	m_rib_bbars[5][4]->AddButton(LHID_OF(LHN_OPEN_LECTURES_MAUAL), m_owner->m_lang->str_open_manual, image_help);
	/* HORÁRIO */
	m_rib_bbars[6][0]->AddButton(LHID_OF(LHN_SEE_TIMETABLE), m_owner->m_lang->str_see_timetable, image_list);
	m_rib_bbars[6][0]->AddButton(LHID_OF(LHN_EXPORT_TIMETABLE), m_owner->m_lang->str_export_timetable, image_detail);
	m_rib_bbars[6][1]->AddButton(LHID_OF(LHN_GENERATE_TIMETABLE), m_owner->m_lang->str_generate_timetable, image_add);
	m_rib_bbars[6][1]->AddButton(LHID_OF(LHN_CREATE_TIMETABLE), m_owner->m_lang->str_create_timetable_manually, image_detail);
	m_rib_bbars[6][2]->AddButton(LHID_OF(LHN_OPEN_TIMETABLE_MANUAL), m_owner->m_lang->str_open_manual, image_help);

	m_ribbon->AddPageHighlight(m_ribbon->GetPageCount() - 1); /* TODO is it working?*/
	m_ribbon->Realize();

	m_center_pane = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_ribbon, 0, wxEXPAND);
	sizer->Add(m_center_pane, 1, wxEXPAND);
	sizer->Add(m_toolbar,0,wxEXPAND);

	sizer->SetMinSize(wxSize(800,600));
	SetSizerAndFit(sizer);
	//m_footer->SetBackgroundColour(wxColor(0x25,0x75,0xb0));
}

void MainMenuForm::NotifyNewUnsavedData(){
	m_toolbar->EnableTool(LHID_OF(LHN_SAVE), true);
}

void MainMenuForm::OnToolbarEvent(wxCommandEvent & evt){
	switch(evt.GetId()){
		case LHID_OF(LHN_SAVE):{
			m_owner->SaveDatabase();
			m_toolbar->EnableTool(LHID_OF(LHN_SAVE), false);
			break;
		}
		case LHID_OF(LHN_UNDO):{
			printf("Undo!\n");
			break;
		}
		case LHID_OF(LHN_REDO):{
			printf("Redo!\n");
			break;
		}
		case LHID_OF(LHN_CLOSE):{
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

						m_owner->SwitchForm(FORM_WELCOME);
						Destroy();
						break;
					}
					case wxID_CANCEL: {
						/* Empty on purpouse */
						break;
					}
				}
			} else {
				free_school(m_owner->m_school);
				m_owner->m_school = nullptr;

				m_owner->SwitchForm(FORM_WELCOME);
				Destroy();
				break;
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
		case LHID_OF(LHN_ADD_FEATURE): {
			CloseOpenedPane();
			m_open_pane = new AddFeaturePane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_ROOM): {
			CloseOpenedPane();
			m_open_pane = new AddRoomPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_FEATURES):{
			CloseOpenedPane();
			m_open_pane = new ListFeaturesPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_ROOMS):{
			CloseOpenedPane();
			m_open_pane = new ListRoomsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ROOM_PERIOD_PREF):{
			CloseOpenedPane();
			m_open_pane = new RoomPeriodPrefPane(m_owner, m_center_pane, wxPoint(100,15));
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
		case LHID_OF(LHN_SUBJECT_PERIOD_PREF):{
			CloseOpenedPane();
			m_open_pane = new SubjectPeriodPrefPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SUBJECT_ROOM_PREF):{
			CloseOpenedPane();
			m_open_pane = new SubjectRoomPrefPane(m_owner, m_center_pane, wxPoint(100,15));
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
		case LHID_OF(LHN_TEACHER_SUBJECT_PREF):{
			CloseOpenedPane();
			m_open_pane = new TeacherSubjPrefPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_TEACHER_PERIOD_PREF):{
			CloseOpenedPane();
			m_open_pane = new TeacherPeriodPrefPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_TEACHER_TWIN_PREF):{
			CloseOpenedPane();
			m_open_pane = new TeacherTwinPrefPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_TEACHER_ROOM_PREF):{
			CloseOpenedPane();
			m_open_pane = new TeacherRoomPrefPane(m_owner, m_center_pane, wxPoint(100,15));
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
		case LHID_OF(LHN_CLASS_PERIOD_PREF):{
			CloseOpenedPane();
			m_open_pane = new ClassPeriodPrefPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_CLASS_TEACHER_PREF):{
			CloseOpenedPane();
			m_open_pane = new ClassTeacherPrefPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_CLASS_ROOM_PREF):{
			CloseOpenedPane();
			m_open_pane = new ClassRoomPrefPane(m_owner, m_center_pane, wxPoint(100,15));
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
		case LHID_OF(LHN_LECTURE_PERIOD_PREF): {
			CloseOpenedPane();
			m_open_pane = new AddLecturePane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_LECTURE_TEACHER_PREF): {
			CloseOpenedPane();
			m_open_pane = new LectureTeacherPrefPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_LECTURE_TWIN_PREF): {
			CloseOpenedPane();
			m_open_pane = new LectureTwinPrefPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_LECTURE_ROOM_PREF): {
			CloseOpenedPane();
			m_open_pane = new LectureRoomPrefPane(m_owner, m_center_pane, wxPoint(100,15));
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
