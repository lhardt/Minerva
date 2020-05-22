#include "gui.hpp"

#include <wx/spinctrl.h>

extern "C" {
	#include "loader.h"
};

AddClassPane::AddClassPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));
	int i;
	School * school = m_owner->m_school;

	for(i = 0; i < MAX_SUBJECTS; ++i){
		selected_subjects[i] = 0;
	}

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Adicionar Turma"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, wxT("Nome da Turma"), wxDefaultPosition, wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxStaticText * size_label = new wxStaticText(this, wxID_ANY, wxT("Qual o tamanho da turma?"), wxDefaultPosition, wxSize(200,15));
	m_size_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxArrayString per_arr;
	for(i = 0; i < school->n_periods_per_day; ++i){
		per_arr.push_back(wxString::FromUTF8(school->daily_period_names[i]));
	}

	wxStaticText * entry_label = new wxStaticText(this, wxID_ANY, wxT("Qual é o período de entrada da turma?"), wxDefaultPosition, wxSize(200,15));
	m_entry_text = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(300,30), per_arr);

	wxStaticText * exit_label = new wxStaticText(this, wxID_ANY, wxT("Qual é o período de saída da turma?"), wxDefaultPosition, wxSize(200,15));
	m_exit_text = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(300,30), per_arr);

	m_free_periods_checkbox = new wxCheckBox(this, wxID_ANY, wxT("A turma pode ter períodos livres?"), wxDefaultPosition, wxSize(300, 30));

	wxStaticText * periods_label = new wxStaticText(this, wxID_ANY, wxT("Em Quais Períodos a Turma está Disponível?"), wxDefaultPosition, wxSize(350,15));

	m_periods = new ChoiceGrid(this, wxID_ANY, wxDefaultPosition, wxSize(400,300));

	wxVector<wxString> possible_values;
	possible_values.push_back(wxT("Disponível"));
	possible_values.push_back(wxT("Indisponível"));

	wxVector<wxColor> possible_backgrounds;
	possible_backgrounds.push_back(wxColor(200,200,255));
	possible_backgrounds.push_back(wxColor(255,200,200));

	m_periods->SetPossibleValues(possible_values);
	m_periods->SetBackgroundColors(possible_backgrounds);

	m_periods->GridRemake(school->n_days,school->n_periods_per_day);

	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i] == false){
			m_periods->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
		}
	}

	wxArrayString subj_arr;
	for(i = 0; i < school->n_subjects; ++i){
		subj_arr.push_back(wxString::FromUTF8(school->subjects[i].name));
	}

	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, wxT("Quantos Períodos de Cada Disciplina a Turma Assiste?."), wxDefaultPosition, wxSize(400,15));
	subjects_label->SetFont(*m_owner->m_small_font);

	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	m_all_subjects_list = new wxComboBox(this, wxID_ANY, wxT("Disciplinas"), wxDefaultPosition, wxSize(180,30), subj_arr);
	m_score_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(120,30));
	wxButton * add_subject = new wxButton(this, wxID_ANY, wxT("Adicionar Disciplina"), wxDefaultPosition, wxSize(180,30));
	add_sizer->Add(m_all_subjects_list,0,wxRIGHT,10);
	add_sizer->Add(m_score_text,0,wxRIGHT,10);
	add_sizer->Add(add_subject,0,wxRIGHT,10);


	add_subject->Bind(wxEVT_BUTTON, &AddClassPane::OnAddSubjectButtonClicked, this);


	wxSizer * subjects_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * buttons_sizer = new wxBoxSizer(wxVERTICAL);

	wxButton * remove_subject = new wxButton(this, wxID_ANY, wxT("Remover"), wxDefaultPosition, wxSize(180,30));
	wxButton * remove_all = new wxButton(this, wxID_ANY, wxT("Remover Todos"), wxDefaultPosition, wxSize(180,30));
	m_selected_subjects_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	buttons_sizer->Add(remove_subject, 0, wxBOTTOM, 15);
	buttons_sizer->Add(remove_all, 0, wxBOTTOM, 15);
	subjects_sizer->Add(m_selected_subjects_list, 0, wxRIGHT, 10);
	subjects_sizer->Add(buttons_sizer, 0, 0);

	remove_all->Bind(wxEVT_BUTTON, &AddClassPane::OnRemoveAllButtonClicked, this);


	wxButton * add_class = new wxButton(this, wxID_ANY, wxT("Adicionar Turma"), wxDefaultPosition, wxSize(180,30));
	add_class->Bind(wxEVT_BUTTON, &AddClassPane::OnAddClassButtonClicked, this);

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,30));

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0, wxLEFT ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(size_label, 0, wxLEFT ,15);
	sizer->Add(m_size_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(m_free_periods_checkbox, 0, wxLEFT ,15);
	sizer->Add(entry_label, 0, wxLEFT ,15);
	sizer->Add(m_entry_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(exit_label, 0, wxLEFT ,15);
	sizer->Add(m_exit_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(periods_label, 0, wxLEFT ,15);
	sizer->Add(m_periods, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_label, 0, wxLEFT, 15);
	sizer->Add(add_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_class, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();
}

void AddClassPane::OnAddClassButtonClicked(wxCommandEvent & ev){
	int i;
	School * school = m_owner->m_school;
	if((!m_name_text->GetValue().IsEmpty()) && (m_size_text->GetValue() > 0)  && (m_entry_text->GetSelection() != wxNOT_FOUND)
			&& (m_exit_text->GetSelection() != wxNOT_FOUND)){
		Class c;
		c.name = copy_wx_string(m_name_text->GetValue());
		c.short_name = copy_wx_string(m_name_text->GetValue());
		c.size = m_size_text->GetValue();
		for(i = 0; i < school->n_periods; ++i){
			c.periods[i] =
					(m_periods->GetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day))==wxT("Disponível") ? 1:0);
		}
		c.rooms = nullptr;
		c.can_have_free_periods_flag = m_free_periods_checkbox->GetValue();
		c.maximal_entry_period = m_entry_text->GetSelection();
		c.minimal_exit_period = m_exit_text->GetSelection();

		if(m_selected_subjects_list->GetCount() > 0){
			c.needs = (SubjectQuantity*) calloc(m_selected_subjects_list->GetCount() + 1, sizeof(SubjectQuantity));
			int i_need = 0;
			for(i = 0; i < school->n_subjects; ++i){
				if(selected_subjects[i] > 0){
					c.needs[i_need].subject = &school->subjects[i];
					c.needs[i_need].quantity = selected_subjects[i];
					++i_need;
				}

			}
		} else {
			c.needs = nullptr;
		}

		int id = insert_class(stdout, m_owner->m_database, &c, school);
		if(id != -1){
			if(school->classes == NULL || school->n_classes == 0){
				school->classes = (Class*)calloc(11, sizeof(Class));
			} else if(school->n_classes % 10 == 0) {
				school->classes = (Class*)realloc(school->classes,(school->n_classes + 11) * sizeof(Class));
			}
			school->classes[ school->n_classes ] = c;
		} else {
			m_err_msg->SetLabel(wxString::FromUTF8("Erro no banco. Não foi possível inserir."));
		}
	} else {
		printf("invalid creation parameters");
	}
}

void AddClassPane::OnAddSubjectButtonClicked(wxCommandEvent & ev){
	if(m_all_subjects_list->GetSelection() != wxNOT_FOUND && m_score_text->GetValue() > 0){
		wxString text = m_all_subjects_list->GetStringSelection() + wxString::Format(": %d", m_score_text->GetValue());
		m_selected_subjects_list->InsertItems(1, &text, m_selected_subjects_list->GetCount());

		selected_subjects[m_all_subjects_list->GetSelection()] = m_score_text->GetValue();
	} else {
		printf("No can do.\n");
	}
	m_err_msg->SetLabel(wxString::FromUTF8("Button clicked\n"));
}

void AddClassPane::OnRemoveAllButtonClicked(wxCommandEvent & ev){
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		selected_subjects[i] = 0;
	}
	m_selected_subjects_list->Clear();
}

AddClassPane::~AddClassPane(){

}
