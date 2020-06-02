#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

AddTeacherGroupPane::AddTeacherGroupPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	teachers_inside = new bool[m_owner->m_school->n_teachers];
	for(i = 0; i < m_owner->m_school->n_teachers; ++i){
		teachers_inside[i] = false;
	}

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Adicionar Grupo de Professores"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, wxT("Nome do Grupo"), wxDefaultPosition, wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxStaticText * teachers_label = new wxStaticText(this, wxID_ANY, wxT("Adicione Professores ao Grupo."), wxDefaultPosition, wxSize(400,15));
	teachers_label->SetFont(*m_owner->m_small_font);

	wxArrayString teacher_names;
	for(i = 0; i < m_owner->m_school->n_teachers; ++i){
		teacher_names.push_back(wxString::FromUTF8(m_owner->m_school->teachers[i].name));
	}

	m_all_teachers_list = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(310,30), teacher_names);
	wxButton * add_teacher = new wxButton(this, wxID_ANY, wxT("Adicionar Professor"), wxDefaultPosition, wxSize(180,30));

	wxButton * remove_teacher = new wxButton(this, wxID_ANY, wxT("Remover"), wxDefaultPosition, wxSize(180,30));
	wxButton * remove_all_teachers = new wxButton(this, wxID_ANY, wxT("Remover Todos"), wxDefaultPosition, wxSize(180,30));
	m_selected_teachers_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));

	wxButton * add_group = new wxButton(this, wxID_ANY, wxT("Adicionar Grupo"), wxDefaultPosition, wxSize(180,30));

	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	add_sizer->Add(m_all_teachers_list,0,wxRIGHT,10);
	add_sizer->Add(add_teacher,0,wxRIGHT,10);

	wxSizer * buttons_sizer = new wxBoxSizer(wxVERTICAL);
	buttons_sizer->Add(remove_teacher,0,wxBOTTOM, 10);
	buttons_sizer->Add(remove_all_teachers, 0, wxBOTTOM, 10);

	wxSizer * teachers_sizer = new wxBoxSizer(wxHORIZONTAL);
	teachers_sizer->Add(m_selected_teachers_list, 0, wxRIGHT, 10);
	teachers_sizer->Add(buttons_sizer, 0, 0);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0, wxLEFT ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(teachers_label, 0, wxLEFT, 15);
	sizer->Add(add_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(teachers_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_group, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	add_group->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnAddGroupButtonClicked, this);
	remove_teacher->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnRemoveTeacherButtonClicked, this);
	remove_all_teachers->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnRemoveAllButtonClicked, this);
	add_teacher->Bind(wxEVT_BUTTON, &AddTeacherGroupPane::OnAddTeacherButtonClicked, this);
}

void AddTeacherGroupPane::OnAddGroupButtonClicked(wxCommandEvent & ev){
	printf("add group\n");
	School * school = m_owner->m_school;
	if(!m_name_text->GetValue().IsEmpty() && m_selected_teachers_list->GetCount() > 0){
		Teacher group;
		int i = 0, j = 0, added_ctr = 0;
		group.name = copy_wx_string(m_name_text->GetValue());
		group.short_name = copy_wx_string(m_name_text->GetValue());
		group.num_planning_periods = 0;
		group.periods = (int*)calloc(school->n_periods, sizeof(int));
		group.periods = (int*)calloc(school->n_rooms, sizeof(int));
		group.subordinates = (int *)calloc(m_selected_teachers_list->GetCount() + 1, sizeof(int));
		/* group.max_days will be the least of .max_days of all teachers. Similarly */
		for(i = 0; i < school->n_teachers; ++i){
			if(teachers_inside[i]){
				Teacher * teacher = &school->teachers[i];
				group.subordinates[i] = 1;
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
						group.periods[j] = teacher->periods[j];
					}
					if(teacher->rooms){
						for(j  = 0; j < school->n_rooms; ++j){
							group.rooms[j] = teacher->rooms[j];
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
						if(group.periods[j] > teacher->periods[j]){
							group.periods[j] = teacher->periods[j];
						}
					}
					for(j = 0; j < school->n_rooms; ++j){
						if(teacher->rooms && teacher->rooms[j] < group.rooms[j]){
							group.rooms[j] = teacher->rooms[j];
						}
					}
				}
				++added_ctr;
			}
		}


		bool success = insert_teacher(stdout, m_owner->m_database, &group, school);
		if(success){
			if(school->teachers == NULL || school->n_teachers == 0){
				school->teachers = (Teacher *) calloc(11, sizeof(Teacher));
				school->n_teachers = 0;
			} else if(school->n_teachers % 10 == 0) {
				school->teachers = (Teacher *) realloc(school->teachers, (i + 11) * sizeof(Teacher));
			}
			school->teachers[school->n_teachers] = group;
			++school->n_teachers;
		}
	}
}

void AddTeacherGroupPane::OnAddTeacherButtonClicked(wxCommandEvent & evt){
	int select_i = m_all_teachers_list->GetSelection();
	if(select_i != wxNOT_FOUND && teachers_inside[select_i] == false){
		wxString t_name = wxString::FromUTF8(m_owner->m_school->teachers[select_i].name);
		teachers_inside[select_i] = true;
		m_selected_teachers_list->InsertItems(1, &t_name, m_selected_teachers_list->GetCount());
	}
	printf("add teacher\n");
}

void AddTeacherGroupPane::OnRemoveTeacherButtonClicked(wxCommandEvent & evt){
	printf("remove teacher\n");
}

void AddTeacherGroupPane::OnRemoveAllButtonClicked(wxCommandEvent & evt){
	printf("remove all\n");
	int i = 0, n = m_owner->m_school->n_teachers;
	for(i = 0; i < n; ++i){
		teachers_inside[i] = false;
	}
	m_selected_teachers_list->Clear();
}

AddTeacherGroupPane::~AddTeacherGroupPane(){
	delete[] teachers_inside;
}
