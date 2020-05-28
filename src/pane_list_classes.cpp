#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

ListClassesPane::ListClassesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Lista de Turmas"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	m_classes_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));

	wxSizer * body_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * description_sizer = new wxBoxSizer(wxVERTICAL);

	body_sizer->Add(m_classes_list, 0, wxEXPAND|wxALL, 15);
	body_sizer->Add(description_sizer, 1, wxEXPAND|wxALL, 15);

	m_name_text = new wxStaticText(this, wxID_ANY, wxT("Nome:"), wxDefaultPosition, wxSize(300,30));
	m_size_text = new wxStaticText(this, wxID_ANY, wxT("Tamanho: "), wxDefaultPosition, wxSize(300,30));
	m_free_periods_text = new wxStaticText(this, wxID_ANY, wxString::FromUTF8("Pode ter Períodos Livres: "), wxDefaultPosition, wxSize(300,30));
	m_entry_period_text = new wxStaticText(this, wxID_ANY, wxString::FromUTF8("Período de Entrada: "), wxDefaultPosition, wxSize(300,30));
	m_exit_period_text = new wxStaticText(this, wxID_ANY, wxString::FromUTF8("Período de Saída: "), wxDefaultPosition, wxSize(300,30));
	m_subjects_text = new wxStaticText(this, wxID_ANY, wxString::FromUTF8("Disciplinas Requeridas: "), wxDefaultPosition, wxSize(300,-1));

	wxStaticText * periods_text = new wxStaticText(this, wxID_ANY, wxT("Períodos em que ele está disponível"), wxDefaultPosition,wxSize(300,20));
	m_periods_grid = new ChoiceGrid(this, wxID_ANY, wxDefaultPosition,wxSize(300,200));

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(wxT("Disponível"));
	grid_values.push_back(wxT("Indisponível"));
	m_periods_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200));
	m_periods_grid->SetBackgroundColors(grid_colors);

	m_periods_grid->m_basic_col_name = wxT("Dia");
	m_periods_grid->m_basic_row_name = wxT("Período");

	wxSizer * btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton * edit_btn = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,wxT("Remover"), wxDefaultPosition, wxSize(200,30));

	edit_btn->Disable();
	btn_sizer->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	btn_sizer->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	description_sizer->Add(m_name_text, 0, wxBOTTOM, 5);
	description_sizer->Add(m_size_text, 0, wxBOTTOM, 5);
	description_sizer->Add(m_free_periods_text, 0, wxBOTTOM, 5);
	description_sizer->Add(m_entry_period_text, 0, wxBOTTOM, 5);
	description_sizer->Add(m_exit_period_text, 0, wxBOTTOM, 5);
	description_sizer->Add(m_subjects_text, 1, wxBOTTOM, 5);
	description_sizer->Add(periods_text, 0, wxBOTTOM, 1);
	description_sizer->Add(m_periods_grid, 0, wxBOTTOM, 5);
	description_sizer->AddStretchSpacer();
	description_sizer->Add(btn_sizer, 0, 0);


	if(school->n_classes > 0){
		wxArrayString list;
		for(i = 0; i < school->n_classes; ++i){
			list.Add(wxString::FromUTF8(school->classes[i].name));
		}
		m_classes_list->InsertItems(list,0);
	}

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(body_sizer, 1, wxEXPAND | wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	edit_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnEditButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnRemoveButtonClicked, this);
	m_classes_list->Bind(wxEVT_LISTBOX, &ListClassesPane::OnSelectionChanged, this);
}

void ListClassesPane::OnSelectionChanged(wxCommandEvent & ev){
	int i;
	School * school = m_owner->m_school;
	if(m_classes_list->GetSelection() != wxNOT_FOUND){
		Class * c = &school->classes[m_classes_list->GetSelection()];
		m_name_text->SetLabel(wxT("Nome: ") + wxString::FromUTF8(c->name));
		m_size_text->SetLabel(wxT("Tamanho: ") + wxString::Format("%d",c->size));
		m_free_periods_text->SetLabel(wxString::FromUTF8("Pode ter Períodos Livres: ") + wxString(c->can_have_free_periods_flag?wxT("Sim"):wxString::FromUTF8("Não")));
		m_entry_period_text->SetLabel(wxString::FromUTF8("Período de Entrada: ") + wxString::Format("%d",c->maximal_entry_period));
		m_exit_period_text->SetLabel(wxString::FromUTF8("Período de Saída: ") + wxString::Format("%d",c->minimal_exit_period));
		m_subjects_text->SetLabel(wxString::FromUTF8("Disciplinas Requeridas: "));
		if(c->needs != NULL){
			for(i = 0; c->needs[i].subject != NULL; ++i){
				m_subjects_text->SetLabel(m_subjects_text->GetLabel() + wxString::FromUTF8(c->needs[i].subject->name) +
						wxString::Format(wxString::FromUTF8(": %d períodos;\n"), c->needs[i].quantity));
			}

		}
		m_subjects_text->Wrap(300);

		m_periods_grid->GridRemake(school->n_days,school->n_periods_per_day);

		for(i = 0; i < school->n_periods; ++i){
			if(school->periods[i] == false){
				m_periods_grid->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
			} else {
				m_periods_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
						wxString::Format("%s" , (c->periods[i] > 0?wxT("Aberta"):wxT("Fechada")) ));
				m_periods_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
						(c->periods[i] > 0?wxColor(200,200,255):wxColor(255,200,200)));
			}
			m_periods_grid->SetReadOnly(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), true);
		}
		FitInside();
	}
}

void ListClassesPane::OnEditButtonClicked(wxCommandEvent & ev){
	printf("Edit clicked\n");
}

void ListClassesPane::OnRemoveButtonClicked(wxCommandEvent & ev){
	int i, j, del_i;
	bool success = false;
	School * school = m_owner->m_school;
	Class * c;

	del_i = m_classes_list->GetSelection();
	if(del_i != wxNOT_FOUND){
		c = &school->classes[del_i];
		success = remove_class(stdout, m_owner->m_database, c->id);

		if(success) {
			/* TODO: Check for subordinates too. */
			Meeting * m_list;
			for(i = 0; i < school->n_solutions; ++i){
				m_list=  school->solutions[i].meetings;
				for(j = 0; m_list[j].m_class != NULL; ++j){
					if(m_list[i].m_class->id == c->id){
						for(j = i; m_list[j].m_class != NULL; ++j){
							m_list[j] = m_list[j+1];
						}
					}
				}
			}
			for(i = del_i; i < school->n_classes; ++i){
				school->classes[i] = school->classes[i+1];
			}

			m_classes_list->Delete(del_i);
		} else {
			printf("Não foi possível deletar.\n");
		}

	}
}

ListClassesPane::~ListClassesPane(){

}
