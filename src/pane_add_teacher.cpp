#include "gui.hpp"

extern "C" {
	#include "loader.h"
};
AddTeacherPane::AddTeacherPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Adicionar Professor"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, wxT("Nome do Professor"), wxDefaultPosition, wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxStaticText * grid_label = new wxStaticText(this, wxID_ANY, wxT("O professor está disponível em quais períodos?"), wxPoint(30,195), wxSize(250,15));
	grid_label->SetFont(*m_owner->m_small_font);
	m_grid = new ChoiceGrid(this, wxID_ANY, wxPoint(30,210), wxSize(500,200));

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(wxT("Disponível"));
	grid_values.push_back(wxT("Ocupado"));
	m_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200));
	m_grid->SetBackgroundColors(grid_colors);

	m_grid->m_basic_col_name = wxT("Dia");
	m_grid->m_basic_row_name = wxT("Período");

	m_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);
	for(i = 0; i < m_owner->m_school->n_periods; ++i){
		if(m_owner->m_school->periods[i] == false){
			m_grid->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
		}
	}

	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, wxT("Adicione Disciplinas que o Professor leciona."), wxDefaultPosition, wxSize(400,15));
	subjects_label->SetFont(*m_owner->m_small_font);

	wxArrayString arr;
	for(i = 0; i < m_owner->m_school->n_subjects; ++i){
		arr.push_back(wxString::FromUTF8(m_owner->m_school->subjects[i].name));
	}

	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	m_all_subjects_list = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(310,30),arr);
	wxButton * add_subject = new wxButton(this, wxID_ANY, wxT("Adicionar Disciplina"), wxDefaultPosition, wxSize(180,30));
	add_subject->Bind(wxEVT_BUTTON, &AddTeacherPane::OnAddSubjectButtonClicked, this);

	add_sizer->Add(m_all_subjects_list,0,wxRIGHT,10);
	add_sizer->Add(add_subject,0,wxRIGHT,10);

	for(i = 0; i < MAX_SUBJECTS; ++i){
		m_teaches_subj[i] = false;
	}

	wxSizer * subjects_sizer  = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * buttons_sizer = new wxBoxSizer(wxVERTICAL);

	wxButton * remove_subject = new wxButton(this, wxID_ANY, wxT("Remover"), wxDefaultPosition, wxSize(180,30));
	wxButton * remove_all     = new wxButton(this, wxID_ANY, wxT("Remover Todas"), wxDefaultPosition, wxSize(180,30));
	m_teaches_subjects_list   = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	remove_subject->Disable();
	subjects_sizer->Add(m_teaches_subjects_list, 0, wxRIGHT, 10);
	subjects_sizer->Add(buttons_sizer, 0, 0);
	buttons_sizer->Add(remove_subject, 0, wxALL, 10);
	buttons_sizer->Add(remove_all, 0, wxLEFT, 10);

	remove_all->Bind(wxEVT_BUTTON, &AddTeacherPane::OnRemoveAllButtonClicked, this);

	wxButton * add_teacher = new wxButton(this, wxID_ANY, wxT("Adicionar Professor"), wxDefaultPosition, wxSize(180,30));
	add_teacher->Bind(wxEVT_BUTTON, &AddTeacherPane::OnAddTeacherButtonClicked, this);

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxPoint(30, 180), wxSize(300,30));
	m_err_msg->SetFont(*m_owner->m_small_font);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0, wxLEFT ,15);
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
	if(m_all_subjects_list->GetSelection() != wxNOT_FOUND && m_teaches_subj[ m_all_subjects_list->GetSelection() ] == false ){
		wxString text = wxString::FromUTF8(m_owner->m_school->subjects[ m_all_subjects_list->GetSelection() ].name);
		m_teaches_subj[ m_all_subjects_list->GetSelection() ] = true;
		m_teaches_subjects_list->InsertItems(1, &text, m_teaches_subjects_list->GetCount());
	}
}

void AddTeacherPane::OnRemoveAllButtonClicked(wxCommandEvent & ev){
	int i;
	for(i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_teaches_subj[i] = false;
	}
	m_teaches_subjects_list->Clear();
}

void AddTeacherPane::ClearInsertedData(){
	School * school = m_owner->m_school;
	int i;
	m_name_text->Clear();
	m_teaches_subjects_list->Clear();

	for(i = 0; i < MAX_SUBJECTS; ++i){
		m_teaches_subj[i] = false;
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
	int i;
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


		if(m_teaches_subjects_list->GetCount() > 0){
			t.teaches = (Teaches**)calloc(m_teaches_subjects_list->GetCount() + 1, sizeof(Teaches*));
			int i_teaches = 0;
			/* The number of elements in the listbox is equal
			 * to the number of true elements in m_teaches_subj */
			for(i = 0; i < school->n_subjects; ++i){
				if(m_teaches_subj[i] == true){
					t.teaches[i_teaches] = (Teaches*) calloc(1,sizeof(Teaches));
					t.teaches[i_teaches]->teacher = &t;
					t.teaches[i_teaches]->subject = &(school->subjects[i]);
					t.teaches[i_teaches]->score = 1;
					/* Anull features */
					for(int i_feature = 0; i_feature < MAX_FEATURES; ++i_feature){
						t.teaches[i_teaches]->features[i_feature] = 0;
						t.teaches[i_teaches]->min_features[i_feature] = 0;
					}
					++i_teaches;
				}
			}
		} else {
			t.teaches = NULL;
		}

		t.periods = (int*)calloc(1 + school->n_periods, sizeof(int));
		for(i = 0; i < school->n_periods; ++i){
			t.periods[i] =
					(m_grid->GetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day))==wxT("Disponível") ? 1:0);
		}

		int result = insert_teacher(stdout, m_owner->m_database, &t, school);

		if(result != -1){

			if(school->teachers == nullptr || school->n_teachers == 0){
				school->teachers = (Teacher *)calloc(11, sizeof(Teacher));
				school->n_teachers = 0;
			} else if(school->n_teachers % 10 == 0){
				school->teachers = (Teacher *)realloc(school->teachers, (school->n_teachers + 11)*sizeof(Teacher));
			}
			school->teachers[ school->n_teachers ] = t;
			++school->n_teachers;

			/* Correcting teacher addresses and mallocs. */
			if(school->teaches == NULL || school->n_teaches == 0){
				// Will take the next ten
				school->teaches = (Teaches *)calloc(11 +
							(m_teaches_subjects_list->GetCount() - m_teaches_subjects_list->GetCount() % 10),
							 sizeof(Teaches));
				school->n_teaches = 0;
			} else if(school->n_teaches %10 == 0){
				school->teaches = (Teaches *)realloc(school->teaches,
							(school->n_teaches + 11 + (m_teaches_subjects_list->GetCount() - m_teaches_subjects_list->GetCount() % 10))
							* sizeof(Teaches));
			}

			int i_subj = 0;
			i = 0;
			for(i_subj = 0; i_subj < school->n_subjects; ++i_subj){
				if(m_teaches_subj[i_subj] == true){
					school->teaches[school->n_teaches] = * t.teaches[i];
					free(t.teaches[i]);
					t.teaches[i] = & school->teaches[school->n_teaches];
					school->n_teaches++;
					++i;
				}
			}

			m_err_msg->SetLabel(wxT("Adicionado com sucesso."));
			ClearInsertedData();
		} else {
			m_err_msg->SetLabel(wxT("Não foi possível adicionar. Erro no banco de dados."));
		}
	} else {
		m_err_msg->SetLabel(wxT("Preencha ao menos o nome do professor."));
	}
}

AddTeacherPane::~AddTeacherPane(){

}
