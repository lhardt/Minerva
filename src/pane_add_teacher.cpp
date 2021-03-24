#include "gui.hpp"

extern "C" {
	#include "assert.h"
	#include "loader.h"
	#include "util.h"
};
AddTeacherPane::AddTeacherPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(250,250,250));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * grid_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teacher_availability);
	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teacher_teaches);
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));

	name_label->SetFont(*m_owner->m_small_font);
	grid_label->SetFont(*m_owner->m_small_font);
	subjects_label->SetFont(*m_owner->m_small_font);
	m_err_msg->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1));
	m_periods_grid = new ChoiceGrid(m_owner, this, wxID_ANY);
	m_subjects_grid = new ChoiceGrid(m_owner, this, wxID_ANY);
	wxButton * button_add = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_teacher, wxDefaultPosition, wxSize(180,30));

	wxString col_name = m_owner->m_lang->str_teaches;
	m_subjects_grid->SetColLabel(0, col_name);
	m_subjects_grid->AddState(m_owner->m_lang->str_no, wxColor(255,200,200));
	m_subjects_grid->AddState(m_owner->m_lang->str_yes, wxColor(200,200,255));
	for(i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_subjects_grid->SetRowLabel(i, wxString::FromUTF8(m_owner->m_school->subjects[i].name));
	}
	m_subjects_grid->GridRemake(1,m_owner->m_school->n_subjects);

	m_periods_grid->AddState(m_owner->m_lang->str_teacher_unavailable, wxColor(255,200,200));
	m_periods_grid->AddState(m_owner->m_lang->str_teacher_available, wxColor(200,200,255));

	m_periods_grid->SetDefaultColumnLabel(m_owner->m_lang->str_day);
	m_periods_grid->SetDefaultRowLabel(m_owner->m_lang->str_period);

	m_periods_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);
	for(i = 0; i < m_owner->m_school->n_periods; ++i){
		m_periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, school->periods[i] ? 1 : ChoiceGrid::CELL_STATE_LOCKED);
	}
	button_add->Bind(wxEVT_BUTTON, &AddTeacherPane::OnAddTeacherButtonClicked, this);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(name_label, 0, wxLEFT | wxTOP ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(grid_label, 0, wxLEFT, 15);
	sizer->Add(m_periods_grid, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_label, 0, wxLEFT, 15);
	sizer->Add(m_subjects_grid, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(button_add, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();
}

void AddTeacherPane::ClearInsertedData(){
	School * school = m_owner->m_school;
	int i;
	m_name_text->Clear();
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_subjects_grid->SetCellState(i,0,0);
	}
	for(i = 0; i < school->n_periods; ++i){
		m_periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, school->periods[i] ? 1 : ChoiceGrid::CELL_STATE_LOCKED);
	}

}

void AddTeacherPane::OnAddTeacherButtonClicked(wxCommandEvent & ev){
	int i_subject = 0, n_subjects = 0;
	Teaches * teaches_vals;
	School * school = m_owner->m_school;

	for(i_subject = 0; i_subject < school->n_subjects; ++i_subject){
		if(m_subjects_grid->GetCellState(i_subject,0) > 0){
			++n_subjects;
		}
	}
	if(!m_name_text->GetValue().IsEmpty()){
		// This structure will outlive the function.
		Teacher t;

		t.name = copy_wx_string(m_name_text->GetValue());
		t.short_name = copy_wx_string(m_name_text->GetValue());
		t.max_days = school->n_days;
		t.max_meetings = school->n_periods;
		t.max_meetings_per_day = school->n_periods_per_day;
		t.max_meetings_per_class_per_day = school->n_periods_per_day;
		t.num_planning_periods = 0;
		t.planning_needs_room = false;
		t.active = true;
		t.subordinates = NULL;
		t.lecture_room_scores = (int*) calloc(school->n_rooms + 1, sizeof(int));
		t.planning_room_scores = (int*) calloc(school->n_rooms + 1, sizeof(int));
		t.day_max_meetings = (int*) calloc(school->n_days + 1, sizeof(int));
		t.day_scores = (int*) calloc(school->n_days + 1, sizeof(int));
		t.planning_twin_scores = NULL;

		for(int i = 0; i < school->n_rooms; ++i){
			t.lecture_room_scores[i] = 1;
			t.planning_room_scores[i] = 1;
		}

		for(int i = 0; i < school->n_days; ++i){
			t.day_max_meetings[i] = school->n_periods_per_day;
			t.day_scores[i] = 1;
		}
		t.day_scores[school->n_days] = -1;
		t.day_max_meetings[school->n_days] = -1;
		t.lecture_room_scores[school->n_rooms] = -1;
		t.planning_room_scores[school->n_rooms] = -1;

		t.teaches = (Teaches**)calloc(n_subjects + 1, sizeof(Teaches*));
		if(n_subjects > 0){
			teaches_vals = (Teaches*)calloc(n_subjects + 1, sizeof(Teaches));
			int i_teaches = 0;
			for(i_subject = 0; i_subject < school->n_subjects; ++i_subject){
				if(m_subjects_grid->GetCellState(i_subject,0) > 0){
					t.teaches[i_teaches] = &(teaches_vals[i_teaches]);
					// the var 't' is temporary and the info will be corrected in the school_teacher_add anyway;
					// t.teaches[i_teaches]->teacher = &t;
					t.teaches[i_teaches]->subject = &(school->subjects[i_subject]);
					int state = m_subjects_grid->GetCellState(i_subject,0);
					t.teaches[i_teaches]->score = state >= 0? state:0;
					++i_teaches;
				}
			}
			t.teaches[i_teaches] = NULL;
		}
		t.lecture_period_scores = (int*)calloc(1 + school->n_periods, sizeof(int));
		t.planning_period_scores = (int*)calloc(1 + school->n_periods, sizeof(int));
		for(int i = 0; i < school->n_periods; ++i){
			int state = m_periods_grid->GetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day);
			t.lecture_period_scores[i] = state >= 0? state:0;
			t.planning_period_scores[i] = t.lecture_period_scores[i];
		}
		t.lecture_period_scores[school->n_periods] = -1;
		t.planning_period_scores[school->n_periods] = -1;

		if(can_insert_teacher(school, &t)){
			TeacherInsertAction * act = new TeacherInsertAction(m_owner, t);
			bool inserted = m_owner->Do(act);
			if(inserted){
				m_err_msg->SetLabel(m_owner->m_lang->str_success);
				ClearInsertedData();
			} else {
				m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
			}
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_repeated_name_error);
		}
		if(n_subjects > 0){
			free(teaches_vals);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}

AddTeacherPane::~AddTeacherPane(){

}
