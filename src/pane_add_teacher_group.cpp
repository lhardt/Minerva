#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "preprocess.h"
};

AddTeacherGroupPane::AddTeacherGroupPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i = 0;
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));
	School * school = m_owner->m_school;

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_name);
	wxStaticText * teachers_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_teachers_to_the_group);
	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teachers_teach);
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));

	name_label->SetFont(*m_owner->m_small_font);
	teachers_label->SetFont(*m_owner->m_small_font);
	subjects_label->SetFont(*m_owner->m_small_font);
	m_err_msg->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));
	m_teachers_grid = new ChoiceGrid(m_owner, this, wxID_ANY);
	m_subjects_grid = new ChoiceGrid(m_owner, this, wxID_ANY);
	wxButton * add_group = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_group, wxDefaultPosition, wxSize(180,30));

	m_teachers_grid->AddState(m_owner->m_lang->str_no, wxColor(255,200,200));
	m_teachers_grid->AddState(m_owner->m_lang->str_yes, wxColor(200,200,255));

	m_subjects_grid->AddState(m_owner->m_lang->str_no, wxColor(255,200,200));
	m_subjects_grid->AddState(m_owner->m_lang->str_yes, wxColor(200,200,255));

	wxString col_name = wxT("");
	for(i = 0; i < school->n_teachers; ++i){
		wxString row_name = wxString::FromUTF8(school->teachers[i].name);
		m_teachers_grid->SetRowName(i, row_name);
	}
	m_teachers_grid->SetColName(0, col_name);
	m_teachers_grid->GridRemake(1, school->n_teachers);

	col_name = wxT("");
	for(i = 0; i < school->n_subjects; ++i){
		wxString row_name = wxString::FromUTF8(school->subjects[i].name);
		m_subjects_grid->SetRowName(i, row_name);
	}
	m_subjects_grid->SetColName(0, col_name);
	m_subjects_grid->GridRemake(1, school->n_subjects);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(name_label, 0, wxLEFT | wxTOP ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(teachers_label, 0, wxLEFT, 15);
	sizer->Add(m_teachers_grid, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_label, 0, wxLEFT, 15);
	sizer->Add(m_subjects_grid, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(add_group, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	add_group->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnAddGroupButtonClicked, this);
}

void AddTeacherGroupPane::OnAddGroupButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	int n_members = 0;
	int n_subjects = 0;
	for(int i = 0; i < school->n_teachers; ++i){
		if(m_teachers_grid->GetCellState(i,0) > 0){
			++n_members;
		}
	}
	for(int i = 0; i < school->n_subjects; ++i){
		if(m_subjects_grid->GetCellState(i,0) > 0){
			++n_subjects;
		}
	}
	if(!m_name_text->GetValue().IsEmpty() && n_members > 0 && n_subjects > 0){
		int i = 0, j = 0, added_ctr = 0;
		Teacher group = (Teacher){
			.name =  copy_wx_string(m_name_text->GetValue()),
			.short_name = copy_wx_string(m_name_text->GetValue()),
			.max_days = 0,
			.max_meetings_per_day = 0,
			.max_meetings_per_class_per_day = 0,
			.max_meetings = 0,
			.planning_needs_room = false,
			.num_planning_periods = 0,
			.active = true,
			.teaches = (Teaches**)calloc(n_subjects + 1, sizeof(Teaches*)),
			.subordinates = (int *)calloc(n_members + 1, sizeof(int)),
			.planning_room_scores = (int*)calloc(1+school->n_rooms, sizeof(int)),
			.lecture_room_scores = (int*)calloc(1+school->n_rooms, sizeof(int)),
			.day_max_meetings = (int*)calloc(school->n_days+1, sizeof(int)),
			.day_scores = (int*)calloc(school->n_days+1, sizeof(int)),
			.lecture_period_scores = (int*)calloc(school->n_periods+1, sizeof(int)),
			.planning_period_scores = (int*)calloc(school->n_periods+1, sizeof(int)),
			.planning_twin_scores = NULL
		};
		group.lecture_room_scores[school->n_rooms] = -1;
		group.planning_room_scores[school->n_rooms] = -1;
		Teaches * teaches_vals = (Teaches*)calloc(n_subjects + 1, sizeof(Teaches));
		/* group.max_days will be the least of .max_days of all teachers. Similarly */
		for(i = 0; i < school->n_teachers; ++i){
			if(m_teachers_grid->GetCellState(i,0) > 0){
				Teacher * teacher = &school->teachers[i];
				group.subordinates[i] = 1;
				if(added_ctr == 0){
					group.max_meetings_per_class_per_day = teacher->max_meetings_per_class_per_day;
					group.max_meetings_per_day = teacher->max_meetings_per_day;
					group.max_meetings = teacher->max_meetings;
					group.max_days = teacher->max_days;
					for(j = 0; j < school->n_days; ++j){
						if(teacher->day_max_meetings != NULL){
							group.day_max_meetings[j] = teacher->day_max_meetings[j];
						} else {
							group.day_max_meetings[j] = 0;
						}
						if(teacher->day_scores != NULL){
							group.day_scores[j] = teacher->day_scores[j];
						} else {
							group.day_scores[j] = 0;
						}
					}
					for(j = 0; j < school->n_periods;++j){
						group.lecture_period_scores[j] = teacher->lecture_period_scores[j];
						group.planning_period_scores[j] = teacher->planning_period_scores[j];
					}
				} else {
					if(group.max_meetings_per_class_per_day > teacher->max_meetings_per_class_per_day){
						group.max_meetings_per_class_per_day = teacher->max_meetings_per_class_per_day;
					}
					if(group.max_meetings_per_day > teacher->max_meetings_per_day){
						group.max_meetings_per_day = teacher->max_meetings_per_day;
					}
					if(group.max_meetings > teacher->max_meetings){
						group.max_meetings = teacher->max_meetings;
					}
					if(group.max_days > teacher->max_days){
						group.max_days = teacher->max_days;
					}
					for(j = 0; j < school->n_days; ++j){
						// TODO  see a better scoring system;
						if(teacher->day_max_meetings && group.day_max_meetings[j] > teacher->day_max_meetings[j]){
							group.day_max_meetings[j] = teacher->day_max_meetings[j];
						}
						if(teacher->day_scores && group.day_scores[j] > teacher->day_scores[j]){
							group.day_scores[j] = teacher->day_scores[j];
						}
					}
					for(j = 0; j < school->n_periods; ++j){
						if(group.lecture_period_scores[j] > teacher->lecture_period_scores[j]){
							group.lecture_period_scores[j] = teacher->lecture_period_scores[j];
						}
						if(group.planning_period_scores[j] > teacher->planning_period_scores[j]){
							group.planning_period_scores[j] = teacher->planning_period_scores[j];
						}
					}
				}
				++added_ctr;
			}
		}
		int i_teaches = 0;
		for(i = 0; i < school->n_subjects; ++i){
			if(m_subjects_grid->GetCellState(i,0)){
				teaches_vals[i_teaches].teacher = &group;
				teaches_vals[i_teaches].subject = &(school->subjects[i]);
				teaches_vals[i_teaches].score = 1;
				group.teaches[i_teaches] = &(teaches_vals[i_teaches]);
				++i_teaches;
			}
		}
		bool success = insert_teacher(stdout, m_owner->m_database, &group, school, -1);
		if(success){
			school_teacher_add(school, &group);

			AddTeacherGroupPane::ClearInsertedData();
			m_err_msg->SetLabel(m_owner->m_lang->str_success);
			m_owner->NotifyNewUnsavedData();
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}

void AddTeacherGroupPane::ClearInsertedData(){
	m_name_text->Clear();
	m_teachers_grid->SetAllCellsState(0);
	m_subjects_grid->SetAllCellsState(0);
}

AddTeacherGroupPane::~AddTeacherGroupPane(){
}
