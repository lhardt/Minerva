#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "preprocess.h"
};

ListClassesPane::ListClassesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetFont(*m_owner->m_text_font);
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_list_of_classes);
	title->SetFont(*m_owner->m_page_title_font);

	m_classes_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(230,300));

	wxSizer * body_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * description_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * fields_sizer = new wxFlexGridSizer(2,10,10);

	body_sizer->Add(m_classes_list, 0, wxEXPAND|wxALL, 15);
	body_sizer->Add(description_sizer, 1, wxEXPAND|wxALL, 15);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * size_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_size);
	wxStaticText * free_periods_label = new wxStaticText(this, wxID_ANY, wxT("Períodos Livres: "));
	wxStaticText * entry_period_label = new wxStaticText(this, wxID_ANY, wxT("Período de Entrada: "));
	wxStaticText * exit_period_label = new wxStaticText(this, wxID_ANY, wxT("Período de Saída: "));
	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, wxT("Disciplinas Requeridas: "));
	wxStaticText * periods_label = new wxStaticText(this, wxID_ANY, wxT("Disponibilidade:"));
	wxStaticText * m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));

	name_label->SetFont(*m_owner->m_bold_text_font);
	size_label->SetFont(*m_owner->m_bold_text_font);
	free_periods_label->SetFont(*m_owner->m_bold_text_font);
	entry_period_label->SetFont(*m_owner->m_bold_text_font);
	exit_period_label->SetFont(*m_owner->m_bold_text_font);
	subjects_label->SetFont(*m_owner->m_bold_text_font);
	periods_label->SetFont(*m_owner->m_bold_text_font);
	m_err_msg->SetFont(*m_owner->m_small_font);

	m_name_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_size_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_free_periods_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_entry_period_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_exit_period_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_subjects_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_periods_grid = new ChoiceGrid(this, wxID_ANY, wxDefaultPosition,wxSize(250,200));
	wxButton * edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove);

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(m_owner->m_lang->str_class_availible);
	grid_values.push_back(m_owner->m_lang->str_class_unavailible);
	m_periods_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200));
	m_periods_grid->SetBackgroundColors(grid_colors);

	m_periods_grid->m_basic_col_name = m_owner->m_lang->str_day;
	m_periods_grid->m_basic_row_name = m_owner->m_lang->str_period;

	m_periods_grid->GridRemake(school->n_days,school->n_periods_per_day);
	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i]){
			m_periods_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
					wxT(""));
			m_periods_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
					wxColor(255,255,255));
		}
	}

	edit_btn->Disable();

	wxSizer * btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	btn_sizer->Add(edit_btn, 1, wxEXPAND | wxRIGHT, 10);
	btn_sizer->Add(delete_btn, 1, wxEXPAND);

	fields_sizer->Add(name_label, 1, wxEXPAND);
	fields_sizer->Add(m_name_text, 1, wxEXPAND);
	fields_sizer->Add(size_label, 1, wxEXPAND);
	fields_sizer->Add(m_size_text, 1, wxEXPAND);
	fields_sizer->Add(free_periods_label, 1, wxEXPAND);
	fields_sizer->Add(m_free_periods_text, 1, wxEXPAND);
	fields_sizer->Add(entry_period_label, 1, wxEXPAND);
	fields_sizer->Add(m_entry_period_text, 1, wxEXPAND);
	fields_sizer->Add(exit_period_label, 1, wxEXPAND);
	fields_sizer->Add(m_exit_period_text, 1, wxEXPAND);
	fields_sizer->Add(subjects_label, 1, wxEXPAND);
	fields_sizer->Add(m_subjects_text, 1, wxEXPAND);
	description_sizer->Add(fields_sizer, 0, wxEXPAND | wxBOTTOM, 15);
	description_sizer->Add(periods_label, 0, wxEXPAND);
	description_sizer->Add(m_periods_grid, 0, wxEXPAND | wxBOTTOM, 15);
	description_sizer->AddStretchSpacer();
	description_sizer->Add(btn_sizer, 0, wxEXPAND);
	description_sizer->Add(m_err_msg, 0, wxEXPAND);

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
		m_name_text->SetLabel(wxString::FromUTF8(c->name));
		m_size_text->SetLabel(wxString::Format("%d",c->size));
		m_free_periods_text->SetLabel(c->can_have_free_periods_flag? m_owner->m_lang->str_yes : m_owner->m_lang->str_no );
		m_entry_period_text->SetLabel(wxString::Format("%s",school->period_names[c->maximal_entry_period]));
		m_exit_period_text->SetLabel(wxString::Format("%s",school->period_names[c->minimal_exit_period]));
		m_subjects_text->SetLabel(wxString::FromUTF8(""));
		if(c->needs != NULL){
			for(i = 0; c->needs[i].subject != NULL; ++i){
				if(i == 0){
					m_subjects_text->SetLabel(m_subjects_text->GetLabel() + wxString::FromUTF8(c->needs[i].subject->name) +
						wxString::Format(wxString::FromUTF8(": %d períodos;"), c->needs[i].quantity));
				} else {
					m_subjects_text->SetLabel(m_subjects_text->GetLabel() + wxT("\n") + wxString::FromUTF8(c->needs[i].subject->name) +
						wxString::Format(wxString::FromUTF8(": %d períodos;"), c->needs[i].quantity));
				}
			}
		}

		for(i = 0; i < school->n_periods; ++i){
			if(school->periods[i]){
				m_periods_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
						c->periods[i] > 0?m_owner->m_lang->str_class_availible:m_owner->m_lang->str_class_unavailible);
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
	int i, del_i;
	bool success = false;
	School * school = m_owner->m_school;
	Class * c;

	del_i = m_classes_list->GetSelection();
	if(del_i != wxNOT_FOUND){
		c = &school->classes[del_i];
		success = remove_class(stdout, m_owner->m_database, c->id);

		if(success) {
			school_class_remove(school, del_i);
			m_classes_list->Delete(del_i);

			m_name_text->SetLabel(wxT(""));
			m_size_text->SetLabel(wxT(""));
			m_free_periods_text->SetLabel(wxT(""));
			m_entry_period_text->SetLabel(wxT(""));
			m_exit_period_text->SetLabel(wxT(""));
			m_subjects_text->SetLabel(wxT(""));

			for(i = 0; i < school->n_periods; ++i){
				if(school->periods[i]){
					m_periods_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
							wxT(""));
					m_periods_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
							wxColor(255,255,255));
				}
			}
			m_owner->NotifyNewUnsavedData();
		} else {
			printf("Não foi possível deletar.\n");
		}

	}
}

ListClassesPane::~ListClassesPane(){

}
