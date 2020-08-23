#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "preprocess.h"
};
AddTeacherPane::AddTeacherPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * grid_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teacher_availibility);
	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teacher_teaches);
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));

	name_label->SetFont(*m_owner->m_small_font);
	grid_label->SetFont(*m_owner->m_small_font);
	subjects_label->SetFont(*m_owner->m_small_font);
	m_err_msg->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1));
	m_grid = new ChoiceGrid(m_owner,this, wxID_ANY, wxPoint(30,210), wxSize(500,200));
	m_all_subjects_list = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(310,30));
	wxButton * add_subject = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_subject, wxDefaultPosition, wxSize(180,-1));
	wxButton * remove_subject = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(180,30));
	wxButton * remove_all     = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove_all, wxDefaultPosition, wxSize(180,30));
	wxButton * add_teacher = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_teacher, wxDefaultPosition, wxSize(180,30));
	m_teaches_subjects_list   = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(m_owner->m_lang->str_teacher_availible);
	grid_values.push_back(m_owner->m_lang->str_teacher_unavailible);
	m_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200));
	m_grid->SetBackgroundColors(grid_colors);

	m_grid->m_basic_col_name = m_owner->m_lang->str_day;
	m_grid->m_basic_row_name = m_owner->m_lang->str_period;

	m_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);
	for(i = 0; i < m_owner->m_school->n_periods; ++i){
		if(m_owner->m_school->periods[i] == false){
			m_grid->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
		}
	}

	add_subject->Bind(wxEVT_BUTTON, &AddTeacherPane::OnAddSubjectButtonClicked, this);
	remove_all->Bind(wxEVT_BUTTON, &AddTeacherPane::OnRemoveAllButtonClicked, this);
	remove_subject->Bind(wxEVT_BUTTON, &AddTeacherPane::OnRemoveSubjectButtonClicked, this);
	add_teacher->Bind(wxEVT_BUTTON, &AddTeacherPane::OnAddTeacherButtonClicked, this);

	for(i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_all_subjects_list->Insert(wxString::FromUTF8(m_owner->m_school->subjects[i].name), i, new IntClientData(i));
	}

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * subjects_sizer  = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * buttons_sizer = new wxBoxSizer(wxVERTICAL);
	add_sizer->Add(m_all_subjects_list,0,wxRIGHT,10);
	add_sizer->Add(add_subject,0,wxRIGHT,10);

	subjects_sizer->Add(m_teaches_subjects_list, 0, wxRIGHT, 10);
	subjects_sizer->Add(buttons_sizer, 0, 0);
	buttons_sizer->Add(remove_subject, 0, wxALL, 10);
	buttons_sizer->Add(remove_all, 0, wxLEFT, 10);

	sizer->Add(name_label, 0, wxLEFT | wxTOP ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(grid_label, 0, wxLEFT, 15);
	sizer->Add(m_grid, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_label, 0, wxLEFT, 15);
	sizer->Add(add_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_teacher, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();
}

void AddTeacherPane::OnAddSubjectButtonClicked(wxCommandEvent & ev){
	int i_sel = m_all_subjects_list->GetSelection();
	if(i_sel != wxNOT_FOUND ){
		int i_subj = ((IntClientData*)m_all_subjects_list->GetClientObject(i_sel))->m_value;
		wxString text = wxString::FromUTF8(m_owner->m_school->subjects[ i_subj ].name);
		m_teaches_subjects_list->Insert(text, m_teaches_subjects_list->GetCount(), new IntClientData(i_subj));
		m_all_subjects_list->Delete(i_sel);
	}
}

void AddTeacherPane::OnRemoveSubjectButtonClicked(wxCommandEvent & ev){
	int i_sel = m_teaches_subjects_list->GetSelection();
	if(i_sel != wxNOT_FOUND ){
		int i_subj = ((IntClientData*)m_teaches_subjects_list->GetClientObject(i_sel))->m_value;
		wxString text = wxString::FromUTF8(m_owner->m_school->subjects[ i_subj ].name);
		m_all_subjects_list->Insert(text, m_all_subjects_list->GetCount(), new IntClientData(i_subj));
		m_teaches_subjects_list->Delete(i_sel);
	}
}

void AddTeacherPane::OnRemoveAllButtonClicked(wxCommandEvent & ev){
	m_all_subjects_list->Clear();
	m_teaches_subjects_list->Clear();
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_all_subjects_list->Insert(wxString::FromUTF8(m_owner->m_school->subjects[i].name), i, new IntClientData(i));
	}
}

void AddTeacherPane::ClearInsertedData(){
	School * school = m_owner->m_school;
	int i;
	m_name_text->Clear();
	m_teaches_subjects_list->Clear();
	m_all_subjects_list->Clear();

	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_all_subjects_list->Insert(wxString::FromUTF8(m_owner->m_school->subjects[i].name), i, new IntClientData(i));
	}
	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i] == false){
			m_grid->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
		} else {
			m_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), wxT("Disponível"));
			m_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), wxColor(200,200,255));
		}
	}

}

void AddTeacherPane::OnAddTeacherButtonClicked(wxCommandEvent & ev){
	int i_teaches = 0;
	Teaches * teaches_vals;
	School * school = m_owner->m_school;
	if(!m_name_text->GetValue().IsEmpty()){
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
		t.room_scores = NULL;
		t.day_max_meetings = NULL;
		t.day_scores = NULL;
		t.planning_twin_scores = NULL;

		if(m_teaches_subjects_list->GetCount() > 0){
			int n_teaches = m_teaches_subjects_list->GetCount();
			t.teaches = (Teaches**)calloc(n_teaches + 1, sizeof(Teaches*));
			teaches_vals = (Teaches*)calloc(n_teaches + 1, sizeof(Teaches));
			for(i_teaches = 0; i_teaches < n_teaches; ++i_teaches){
				IntClientData * data = (IntClientData* )m_teaches_subjects_list->GetClientObject(i_teaches);
				t.teaches[i_teaches] = &(teaches_vals[i_teaches]);
				t.teaches[i_teaches]->teacher = &t;
				t.teaches[i_teaches]->subject = &(school->subjects[data->m_value]);
				t.teaches[i_teaches]->score = 1;
			}
			t.teaches[i_teaches] = NULL;
		} else {
			t.teaches = NULL;
		}
		t.lecture_period_scores = (int*)calloc(1 + school->n_periods, sizeof(int));
		t.planning_period_scores = (int*)calloc(1 + school->n_periods, sizeof(int));
		for(int i = 0; i < school->n_periods; ++i){
			t.lecture_period_scores[i] =
					(m_grid->GetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day))==m_owner->m_lang->str_teacher_availible ? 1:0);
			t.planning_period_scores[i] =
					(m_grid->GetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day))==m_owner->m_lang->str_teacher_availible ? 1:0);
		}
		int result = insert_teacher(stdout, m_owner->m_database, &t, school);

		if(result != -1){
			school_teacher_add(school, &t);
			m_err_msg->SetLabel(m_owner->m_lang->str_success);
			if(m_teaches_subjects_list->GetCount() > 0){
				free(teaches_vals);
			}
			ClearInsertedData();
			m_owner->NotifyNewUnsavedData();
		} else {
			if(m_teaches_subjects_list->GetCount() > 0){
				free(teaches_vals);
			}
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}

AddTeacherPane::~AddTeacherPane(){

}
