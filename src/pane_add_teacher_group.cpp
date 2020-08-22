#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "preprocess.h"
};

AddTeacherGroupPane::AddTeacherGroupPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i = 0;
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_name);
	wxStaticText * teachers_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_teachers_to_the_group);
	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teachers_teach);
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));

	name_label->SetFont(*m_owner->m_small_font);
	teachers_label->SetFont(*m_owner->m_small_font);
	subjects_label->SetFont(*m_owner->m_small_font);
	m_err_msg->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));
	m_all_teachers_list = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(310,30));
	wxButton * add_teacher = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_teacher, wxDefaultPosition, wxSize(180,30));
	wxButton * remove_teacher = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(180,30));
	wxButton * remove_all_teachers = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove_all, wxDefaultPosition, wxSize(180,30));
	m_selected_teachers_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));

	m_all_subjects_list = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(310,30));
	wxButton * add_subject = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_subject, wxDefaultPosition, wxSize(180,30));
	wxButton * remove_subject = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(180,30));
	wxButton * remove_all_subjects = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove_all, wxDefaultPosition, wxSize(180,30));
	m_selected_subjects_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));

	wxButton * add_group = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_group, wxDefaultPosition, wxSize(180,30));

	for(i = 0; i < m_owner->m_school->n_teachers; ++i){
		m_all_teachers_list->Insert(wxString::FromUTF8(m_owner->m_school->teachers[i].name), i, new IntClientData(i));
	}

	for(i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_all_subjects_list->Insert(wxString::FromUTF8(m_owner->m_school->subjects[i].name), i, new IntClientData(i));
	}

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * addt_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * adds_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * tbuttons_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * sbuttons_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * teachers_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * subjects_sizer = new wxBoxSizer(wxHORIZONTAL);

	addt_sizer->Add(m_all_teachers_list,0,wxRIGHT,10);
	addt_sizer->Add(add_teacher,0,wxRIGHT,10);

	adds_sizer->Add(m_all_subjects_list,0,wxRIGHT,10);
	adds_sizer->Add(add_subject,0,wxRIGHT,10);

	tbuttons_sizer->Add(remove_teacher,0,wxBOTTOM, 10);
	tbuttons_sizer->Add(remove_all_teachers, 0, wxBOTTOM, 10);

	sbuttons_sizer->Add(remove_subject,0,wxBOTTOM, 10);
	sbuttons_sizer->Add(remove_all_subjects, 0, wxBOTTOM, 10);

	teachers_sizer->Add(m_selected_teachers_list, 0, wxRIGHT, 10);
	teachers_sizer->Add(tbuttons_sizer, 0, 0);

	subjects_sizer->Add(m_selected_subjects_list, 0, wxRIGHT, 10);
	subjects_sizer->Add(sbuttons_sizer, 0, 0);

	sizer->Add(name_label, 0, wxLEFT | wxTOP ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(teachers_label, 0, wxLEFT, 15);
	sizer->Add(addt_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(teachers_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(subjects_label, 0, wxLEFT, 15);
	sizer->Add(adds_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_group, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	add_group->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnAddGroupButtonClicked, this);
	remove_teacher->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnRemoveTeacherButtonClicked, this);
	remove_subject->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnRemoveSubjectButtonClicked, this);
	remove_all_teachers->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnRemoveAllTeachersButtonClicked, this);
	remove_all_subjects->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnRemoveAllSubjectsButtonClicked, this);
	add_teacher->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnAddTeacherButtonClicked, this);
	add_subject->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnAddSubjectButtonClicked, this);
}

void AddTeacherGroupPane::OnAddGroupButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	int n_members = m_selected_teachers_list->GetCount();
	int n_subjects = m_selected_subjects_list->GetCount();
	if(!m_name_text->GetValue().IsEmpty() && n_members > 0 && n_subjects > 0){
		Teacher group;
		int i = 0, j = 0, added_ctr = 0;
		group.name = copy_wx_string(m_name_text->GetValue());
		group.short_name = copy_wx_string(m_name_text->GetValue());
		group.num_planning_periods = 0;
		group.lecture_period_scores = (int*)calloc(school->n_periods+1, sizeof(int));
		group.planning_period_scores = (int*)calloc(school->n_periods+1, sizeof(int));
		group.room_scores = (int*)calloc(school->n_rooms, sizeof(int));
		group.subordinates = (int *)calloc(n_members + 1, sizeof(int));
		Teaches * teaches_vals = (Teaches*)calloc(n_subjects + 1, sizeof(Teaches));
		group.teaches = (Teaches**)calloc(n_subjects + 1, sizeof(Teaches*));
		/* group.max_days will be the least of .max_days of all teachers. Similarly */
		for(i = 0; i < n_members; ++i){
			int i_teacher = ((IntClientData*)m_selected_teachers_list->GetClientObject(i))->m_value;
			Teacher * teacher = &school->teachers[i_teacher];
			group.subordinates[i_teacher] = 1;
			if(added_ctr == 0){
				group.max_meetings_per_class_per_day = teacher->max_meetings_per_class_per_day;
				group.max_meetings_per_day = teacher->max_meetings_per_day;
				group.max_meetings = teacher->max_meetings;
				group.max_days = teacher->max_days;
				for(j = 0; j < school->n_days; ++j){
					group.day_max_meetings[j] = teacher->day_max_meetings[j];
					group.day_scores[j] = teacher->day_max_meetings[j];
				}
				for(j = 0; j < school->n_periods;++j){
					group.lecture_period_scores[j] = teacher->lecture_period_scores[j];
					group.planning_period_scores[j] = teacher->planning_period_scores[j];
				}
				if(teacher->room_scores){
					for(j  = 0; j < school->n_rooms; ++j){
						group.room_scores[j] = teacher->room_scores[j];
					}
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
					if(group.day_max_meetings[j] > teacher->day_max_meetings[j]){
						group.day_max_meetings[j] = teacher->day_max_meetings[j];
					}
					if(group.day_scores[j] > teacher->day_max_meetings[j]){
						group.day_scores[j] = teacher->day_max_meetings[j];
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
				for(j = 0; j < school->n_rooms; ++j){
					if(teacher->room_scores && teacher->room_scores[j] < group.room_scores[j]){
						group.room_scores[j] = teacher->room_scores[j];
					}
				}
			}
			++added_ctr;
		}
		for(i = 0; i < n_subjects; ++i){
			teaches_vals[i].teacher = &group;
			teaches_vals[i].subject = &(school->subjects[ ((IntClientData*)m_selected_subjects_list->GetClientObject(i))->m_value ]);
			teaches_vals[i].score = 1;
			group.teaches[i] = &(teaches_vals[i]);
		}
		bool success = insert_teacher(stdout, m_owner->m_database, &group, school);
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

void AddTeacherGroupPane::OnAddSubjectButtonClicked(wxCommandEvent & evt){
	int i_select = m_all_subjects_list->GetSelection();
	if(i_select != wxNOT_FOUND){
		int i_subj = ((IntClientData*)m_all_subjects_list->GetClientObject(i_select))->m_value;
		wxString sname = wxString::FromUTF8(m_owner->m_school->subjects[i_subj].name);
		m_selected_subjects_list->Insert(sname, m_selected_subjects_list->GetCount(), new IntClientData(i_subj));
		m_all_subjects_list->Delete(i_select);
	}
}


void AddTeacherGroupPane::OnAddTeacherButtonClicked(wxCommandEvent & evt){
	int i_select = m_all_teachers_list->GetSelection();
	if(i_select != wxNOT_FOUND){
		int i_teacher = ((IntClientData*)m_all_teachers_list->GetClientObject(i_select))->m_value;
		wxString t_name = wxString::FromUTF8(m_owner->m_school->teachers[i_teacher].name);
		m_selected_teachers_list->Insert(t_name, m_selected_teachers_list->GetCount(), new IntClientData(i_teacher));
		m_all_teachers_list->Delete(i_select);
	}
}

void AddTeacherGroupPane::OnRemoveSubjectButtonClicked(wxCommandEvent & evt){
	int i_select = m_selected_subjects_list->GetSelection();
	if(i_select != wxNOT_FOUND){
		int i_subj = ((IntClientData*)m_selected_subjects_list->GetClientObject(i_select))->m_value;
		wxString sname = wxString::FromUTF8(m_owner->m_school->subjects[i_subj].name);
		m_all_subjects_list->Insert(sname, m_all_subjects_list->GetCount(), new IntClientData(i_subj));
		m_selected_subjects_list->Delete(i_select);
	}
}

void AddTeacherGroupPane::OnRemoveTeacherButtonClicked(wxCommandEvent & evt){
	int i_select = m_selected_teachers_list->GetSelection();
	if(i_select != wxNOT_FOUND){
		int i_teacher = ((IntClientData*)m_selected_teachers_list->GetClientObject(i_select))->m_value;
		wxString t_name = wxString::FromUTF8(m_owner->m_school->teachers[i_teacher].name);
		m_all_teachers_list->Insert(t_name, m_all_teachers_list->GetCount(), new IntClientData(i_teacher));
		m_selected_teachers_list->Delete(i_select);
	}
}

void AddTeacherGroupPane::OnRemoveAllSubjectsButtonClicked(wxCommandEvent & evt){
	m_selected_subjects_list->Clear();
	m_all_subjects_list->Clear();
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_all_subjects_list->Insert(wxString::FromUTF8(m_owner->m_school->subjects[i].name), i, new IntClientData(i));
	}
}

void AddTeacherGroupPane::OnRemoveAllTeachersButtonClicked(wxCommandEvent & evt){
	m_selected_teachers_list->Clear();
	m_all_teachers_list->Clear();
	for(int i = 0; i < m_owner->m_school->n_teachers; ++i){
		m_all_teachers_list->Insert(wxString::FromUTF8(m_owner->m_school->teachers[i].name), i, new IntClientData(i));
	}
}

void AddTeacherGroupPane::ClearInsertedData(){
	m_selected_teachers_list->Clear();
	m_name_text->Clear();
	m_all_teachers_list->Clear();
	for(int i = 0; i < m_owner->m_school->n_teachers; ++i){
		m_all_teachers_list->Insert(wxString::FromUTF8(m_owner->m_school->teachers[i].name), i, new IntClientData(i));
	}
}

AddTeacherGroupPane::~AddTeacherGroupPane(){
}
