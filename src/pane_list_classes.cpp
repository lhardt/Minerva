#include "gui.hpp"

#include <wx/spinctrl.h>

extern "C" {
	#include "loader.h"
	#include "preprocess.h"
	#include "util.h"
};

ListClassesPane::ListClassesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));
	int i = 0;

	/* Layout */
	wxNotebook  * notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	/* Declaration of members */
	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * size_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_size);
	wxStaticText * free_periods_label = new wxStaticText(this, wxID_ANY, wxT("Períodos Livres: "));
	wxStaticText * entry_period_label = new wxStaticText(this, wxID_ANY, wxT("Período de Entrada: "));
	wxStaticText * exit_period_label = new wxStaticText(this, wxID_ANY, wxT("Período de Saída: "));
	wxStaticText * active_label = new wxStaticText(this, wxID_ANY, wxT("Ativo:"));
	wxStaticText * m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));
	m_classes_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(230,300));
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150,30));
	m_active_text = new wxCheckBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
	m_size_text = new wxSpinCtrl(this, wxID_ANY);
	m_free_periods_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_entry_period_text = new wxChoice(this, wxID_ANY);
	m_exit_period_text = new wxChoice(this, wxID_ANY);
	m_subjects_text = new wxStaticText(this, wxID_ANY, wxT(""));
	// m_periods_grid = new ChoiceGrid(notebook, wxID_ANY, wxDefaultPosition,wxSize(250,200));
	wxButton * delete_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_delete);
	m_basic_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	m_basic_edit_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_edit);

	m_periods = new ScoreGridPane(m_owner,notebook, wxID_ANY);
	// m_superclasses = new ScoreGridPane(m_owner,notebook, wxID_ANY);
	m_rooms = new ScoreGridPane(m_owner,notebook, wxID_ANY);

	wxVector<wxString> all_subject_names = wxVector<wxString>();
	wxString subject_col_name = m_owner->m_lang->str_periods;
	for(i = 0; i < school->n_subjects; ++i){
		all_subject_names.push_back(wxString::FromUTF8(school->subjects[i].name));
	}
	m_assignments = new PosIntGridPane(m_owner, notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, subject_col_name, all_subject_names);

	wxVector<wxString> all_sgroup_names = wxVector<wxString>();
	wxString sgroup_col_name = m_owner->m_lang->str_max_number_of_periods_per_day;
	for(i = 0; i < school->n_subject_groups; ++i){
		all_sgroup_names.push_back(wxString::FromUTF8(school->subject_group_names[i]));
	}
	m_groups = new PosIntGridPane(m_owner, notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, sgroup_col_name, all_sgroup_names);

	m_err_msg->SetFont(*m_owner->m_small_font);

	/* Sizers */
	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * description_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * box_field_sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Dados básicos"));
	wxSizer * fields_sizer = new wxFlexGridSizer(4,5,5);

	sizer->Add(m_classes_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(description_sizer, 1, wxEXPAND|wxALL, 15);

	fields_sizer->Add(name_label, 0, wxEXPAND);
	fields_sizer->Add(m_name_text, 0, wxEXPAND);
	fields_sizer->Add(size_label, 0, wxEXPAND);
	fields_sizer->Add(m_size_text, 0, wxEXPAND);
	fields_sizer->Add(entry_period_label, 0, wxEXPAND);
	fields_sizer->Add(m_entry_period_text, 0, wxEXPAND);
	fields_sizer->Add(exit_period_label, 0, wxEXPAND);
	fields_sizer->Add(m_exit_period_text, 0, wxEXPAND);
	fields_sizer->Add(free_periods_label, 0, wxEXPAND);
	fields_sizer->Add(m_free_periods_text, 0, wxEXPAND);
	fields_sizer->Add(active_label, 0, wxEXPAND);
	fields_sizer->Add(m_active_text, 0, wxEXPAND);
	fields_sizer->AddStretchSpacer();
	fields_sizer->AddStretchSpacer();
	fields_sizer->Add(m_basic_cancel_btn, 0, wxEXPAND);
	fields_sizer->Add(m_basic_edit_btn, 0, wxEXPAND);
	box_field_sizer->Add(fields_sizer, 0, wxALL, 5);
	description_sizer->Add(box_field_sizer, 0, wxEXPAND | wxBOTTOM, 15);
	description_sizer->Add(notebook, 1, wxEXPAND | wxBOTTOM, 15);
	description_sizer->Add(btn_sizer, 0, wxEXPAND);
	description_sizer->Add(delete_btn, 0, wxEXPAND);
	description_sizer->Add(m_err_msg, 0, wxEXPAND);

	notebook->InsertPage(0, m_periods, wxT("Disponibilidade"));
	notebook->InsertPage(1, m_assignments, wxT("Disciplinas"));
	notebook->InsertPage(2, m_rooms, wxT("Salas"));
	notebook->InsertPage(3, m_groups, wxT("Grupos Disc."));

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	/* Functionality and Information */
	m_basic_edit_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnEditButtonClicked, this);
	m_basic_cancel_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnCancelButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListClassesPane::OnRemoveButtonClicked, this);
	m_classes_list->Bind(wxEVT_LISTBOX, &ListClassesPane::OnSelectionChanged, this);


	// AVAILABILITY PANE CODE
	ChoiceGrid * periods_grid = m_periods->GetGrid();
	periods_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	periods_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	periods_grid->m_basic_col_name = m_owner->m_lang->str_day;
	periods_grid->m_basic_row_name = m_owner->m_lang->str_period;
	periods_grid->GridRemake(school->n_days, school->n_periods_per_day);

	// ROOM PANE CODE
	ChoiceGrid * rooms_grid = m_rooms->GetGrid();
	rooms_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	rooms_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	rooms_grid->SetColName(0,m_owner->m_lang->str_name);
	for(i = 0; i < school->n_rooms; ++i){
		rooms_grid->SetRowName(i, wxString::FromUTF8(school->rooms[i].name));
	}
	rooms_grid->GridRemake(1, school->n_rooms);

	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i]){
			periods_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
					wxT(""));
			periods_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
					wxColor(255,255,255));
		}
	}

	if(school->n_classes > 0){
		wxArrayString list;
		for(i = 0; i < school->n_classes; ++i){
			list.Add(wxString::FromUTF8(school->classes[i].name));
		}
		m_classes_list->InsertItems(list,0);
	}
	for(i = 0; i < school->n_periods_per_day; ++i){
		m_entry_period_text->Insert(wxString::FromUTF8(school->daily_period_names[i]), i, new IntClientData(i));
		m_exit_period_text->Insert(wxString::FromUTF8(school->daily_period_names[i]), i, new IntClientData(i));
	}

	m_classes_list->Disable();
	m_name_text->Disable();
	m_active_text->Disable();
	m_size_text->Disable();
	m_entry_period_text->Disable();
	m_exit_period_text->Disable();
	m_free_periods_text->Disable();
	m_classes_list->Enable();
	m_basic_cancel_btn->Hide();

}

void ListClassesPane::OnCancelButtonClicked(wxCommandEvent & ev){
	m_classes_list->Disable();
	m_name_text->Disable();
	m_active_text->Disable();
	m_size_text->Disable();
	m_entry_period_text->Disable();
	m_exit_period_text->Disable();
	m_free_periods_text->Disable();
	m_classes_list->Enable();
	m_basic_cancel_btn->Hide();
	m_basic_edit_btn->SetLabel(m_owner->m_lang->str_edit);
}

void ListClassesPane::OnEditButtonClicked(wxCommandEvent & ev){
	if(m_basic_cancel_btn->IsShown()){
		m_classes_list->Disable();
		m_name_text->Disable();
		m_active_text->Disable();
		m_size_text->Disable();
		m_entry_period_text->Disable();
		m_exit_period_text->Disable();
		m_free_periods_text->Disable();
		m_classes_list->Enable();
		m_basic_cancel_btn->Hide();
		m_basic_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	} else {
		m_classes_list->Enable();
		m_name_text->Enable();
		m_active_text->Enable();
		m_size_text->Enable();
		m_entry_period_text->Enable();
		m_exit_period_text->Enable();
		m_free_periods_text->Enable();
		m_basic_cancel_btn->Show();
		m_basic_edit_btn->SetLabel(m_owner->m_lang->str_save);
	}
}

void ListClassesPane::OnSelectionChanged(wxCommandEvent & ev){
	int i;
	School * school = m_owner->m_school;
	if(m_classes_list->GetSelection() != wxNOT_FOUND){
		Class * c = &school->classes[m_classes_list->GetSelection()];
		m_name_text->SetValue(wxString::FromUTF8(c->name));
		m_size_text->SetValue(c->size);
		m_free_periods_text->SetValue(c->can_have_free_periods_flag);
		m_active_text->SetValue(c->active);
		m_free_periods_text->SetLabel(c->can_have_free_periods_flag? m_owner->m_lang->str_yes : m_owner->m_lang->str_no );
		m_active_text->SetLabel(c->active? m_owner->m_lang->str_yes : m_owner->m_lang->str_no );
		m_entry_period_text->SetSelection(c->maximal_entry_period);
		m_exit_period_text->SetSelection(c->minimal_exit_period);
		m_subjects_text->SetLabel(wxString::FromUTF8(""));
		if(c->assignments != NULL){
			for(i = 0; i < school->n_assignments && c->assignments[i] != NULL; ++i){
				int i_subject = get_subject_index_by_id(school, c->assignments[i]->subject->id);
				m_assignments->GetGrid()->SetCellValue(i_subject,0,wxString::Format("%d",c->assignments[i]->amount));
			}
		}
		m_periods->SetValues(c->period_scores);
		m_groups->SetValues(c->max_per_day_subject_group);

	}
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

			// for(i = 0; i < school->n_periods; ++i){
			// 	if(school->periods[i]){
			// 		m_periods_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
			// 				wxT(""));
			// 		m_periods_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
			// 				wxColor(255,255,255));
			// 	}
			// }
			m_owner->NotifyNewUnsavedData();
		} else {
			printf("Não foi possível deletar.\n");
		}

	}
}

ListClassesPane::~ListClassesPane(){

}
