#include "gui.hpp"

#include <wx/spinctrl.h>

extern "C" {
	#include "loader.h"
	#include "preprocess.h"
};

AddClassPane::AddClassPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));
	int i;
	School * school = m_owner->m_school;

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_class);
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_name);
	wxStaticText * size_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_size);
	wxStaticText * entry_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_entry_period);
	wxStaticText * exit_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_exit_period);
	wxStaticText * periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_availibility);
	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_subjects);
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));

	name_label->SetFont(*m_owner->m_small_font);
	size_label->SetFont(*m_owner->m_small_font);
	entry_label->SetFont(*m_owner->m_small_font);
	exit_label->SetFont(*m_owner->m_small_font);
	periods_label->SetFont(*m_owner->m_small_font);
	subjects_label->SetFont(*m_owner->m_small_font);
	m_err_msg->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(180,-1));
	m_size_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(180,-1));
	m_entry_text = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(300,30));
	m_exit_text = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(300,30));
	m_free_periods_checkbox = new wxCheckBox(this, wxID_ANY, m_owner->m_lang->str_class_can_have_free_periods, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	m_periods = new ChoiceGrid(this, wxID_ANY, wxDefaultPosition, wxSize(400,300));
	wxButton * remove_subject = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(220,-1));
	wxButton * remove_all = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove_all, wxDefaultPosition, wxSize(220,-1));
	m_selected_subjects_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	wxButton * add_class = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_class, wxDefaultPosition, wxSize(220,-1));
	m_all_subjects_list = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(180,30));
	m_score_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(120,30));
	wxButton * add_subject = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_subject, wxDefaultPosition, wxSize(220,30));

	for(i = 0; i < school->n_periods_per_day; ++i){
		m_entry_text->Append(wxString::FromUTF8(school->daily_period_names[i]), new IntClientData(i));
		m_exit_text->Append(wxString::FromUTF8(school->daily_period_names[i]), new IntClientData(i));
	}

	for(i = 0; i < school->n_subjects; ++i){
		m_all_subjects_list->Append(wxString::FromUTF8(school->subjects[i].name), new IntClientData(i));
	}

	m_free_periods_checkbox->SetFont(*m_owner->m_small_font);

	wxVector<wxString> possible_values;
	possible_values.push_back(m_owner->m_lang->str_class_availible);
	possible_values.push_back(m_owner->m_lang->str_class_unavailible);

	wxVector<wxColor> possible_backgrounds;
	possible_backgrounds.push_back(wxColor(200,200,255));
	possible_backgrounds.push_back(wxColor(255,200,200));

	m_periods->SetPossibleValues(possible_values);
	m_periods->SetBackgroundColors(possible_backgrounds);

	m_periods->m_basic_col_name = m_owner->m_lang->str_day;
	m_periods->m_basic_row_name = m_owner->m_lang->str_period;

	m_periods->GridRemake(school->n_days,school->n_periods_per_day);

	last_entry = 0;
	last_exit = school->n_periods_per_day;

	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i] == false){
			m_periods->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
		}
	}

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * subjects_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * buttons_sizer = new wxBoxSizer(wxVERTICAL);

	add_sizer->Add(m_all_subjects_list,0,wxRIGHT,10);
	add_sizer->Add(m_score_text,0,wxRIGHT,10);
	add_sizer->Add(add_subject,0,wxRIGHT,10);

	buttons_sizer->Add(remove_subject, 0, wxBOTTOM, 15);
	buttons_sizer->Add(remove_all, 0, wxBOTTOM, 15);
	subjects_sizer->Add(m_selected_subjects_list, 0, wxRIGHT, 10);
	subjects_sizer->Add(buttons_sizer, 0, 0);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0, wxLEFT ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(size_label, 0, wxLEFT ,15);
	sizer->Add(m_size_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(m_free_periods_checkbox, 0, wxLEFT | wxBOTTOM ,15);
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

	m_entry_text->Bind(wxEVT_CHOICE, &AddClassPane::OnPeriodChoice, this);
	m_exit_text->Bind(wxEVT_CHOICE, &AddClassPane::OnPeriodChoice, this);
	remove_all->Bind(wxEVT_BUTTON, &AddClassPane::OnRemoveAllButtonClicked, this);
	add_subject->Bind(wxEVT_BUTTON, &AddClassPane::OnAddSubjectButtonClicked, this);
	add_class->Bind(wxEVT_BUTTON, &AddClassPane::OnAddClassButtonClicked, this);
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
		c.periods[school->n_periods] = -1;
		c.rooms = nullptr;
		c.can_have_free_periods_flag = m_free_periods_checkbox->GetValue();
		c.maximal_entry_period = m_entry_text->GetSelection();
		c.minimal_exit_period = m_exit_text->GetSelection();
		c.abstract = false;
		if(m_selected_subjects_list->GetCount() > 0){
			int n_needs = m_selected_subjects_list->GetCount();
			c.needs = (SubjectQuantity*) calloc(n_needs + 1, sizeof(SubjectQuantity));
			int i_need = 0;

			for(i_need = 0; i < n_needs; ++i_need){
				IntPairClientData* item_data =(IntPairClientData*) m_selected_subjects_list->GetClientObject(i_need);
				c.needs[i_need].subject = &school->subjects[ item_data->m_v1 ];
				c.needs[i_need].quantity = item_data->m_v2;
			}

			c.needs[i_need].subject = NULL;
			c.needs[i_need].quantity = -1;
		} else {
			c.needs = nullptr;
		}
		c.subordinates = nullptr;

		int id = insert_class(stdout, m_owner->m_database, &c, school);
		if(id != -1){
			school_class_add(school, &c);
			m_err_msg->SetLabel(m_owner->m_lang->str_success);

			ClearInsertedData();
			m_owner->NotifyNewUnsavedData();
		} else {
			free(c.name);
			free(c.short_name);
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}

void AddClassPane::OnAddSubjectButtonClicked(wxCommandEvent & ev){
	if(m_all_subjects_list->GetSelection() != wxNOT_FOUND && m_score_text->GetValue() > 0){
		int subject_i = ((IntClientData*)(m_all_subjects_list->GetClientObject( m_all_subjects_list->GetSelection())))->m_value;
		IntPairClientData * data = new IntPairClientData(subject_i, m_score_text->GetValue());
		wxString text = m_all_subjects_list->GetStringSelection() + wxString::Format(": %d", m_score_text->GetValue());
		m_selected_subjects_list->Insert(text, m_selected_subjects_list->GetCount(),data);
		m_score_text->SetValue(0);
	}
}

void AddClassPane::ClearInsertedData(){
	School * school = m_owner->m_school;
	int i = 0;
	m_name_text->Clear();
	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i] == false){
			m_periods->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
		} else {
			m_periods->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), m_owner->m_lang->str_class_availible);
			m_periods->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), wxColor(200,200,255));
		}
	}
	m_selected_subjects_list->Clear();
	m_score_text->SetValue(0);
	m_size_text->SetValue(0);
	m_free_periods_checkbox->SetValue(false);
	last_entry = -1;
	last_exit = -1;

	m_entry_text->Clear();
	m_exit_text->Clear();
}

void AddClassPane::OnRemoveAllButtonClicked(wxCommandEvent & ev){
	m_selected_subjects_list->Clear();
}

void AddClassPane::OnPeriodChoice(wxCommandEvent& ev){
	School * school = m_owner->m_school;
	if(m_entry_text->GetSelection() != wxNOT_FOUND){
		int new_entry = m_entry_text->GetSelection();

		if(new_entry > last_entry){
			for(int i = last_entry; i < new_entry; ++i){
				for(int j = 0; j < school->n_days; ++j){
					m_periods->SetCellImmutable(1 + i, 1 + j);
				}
			}
		} else {
			for(int i = new_entry; i < last_entry; ++i){
				for(int j = 0; j < school->n_days; ++j){
					if(school->periods[j * school->n_periods_per_day + i]){
						m_periods->SetCellValue(1 + i, 1 + j, m_owner->m_lang->str_class_availible);
						m_periods->SetCellBackgroundColour(1 + i, 1 + j, wxColor(200,200,255));
					} /* Else the cell is immutable anyway */
				}
			}
		}
		last_entry = new_entry;
	}
	if(m_exit_text->GetSelection() != wxNOT_FOUND){
		int new_exit = m_exit_text->GetSelection() + 1;
		if(new_exit > last_exit){
			for(int i = last_exit; i < new_exit; ++i){
				for(int j = 0; j < school->n_days; ++j){
					if(school->periods[j * school->n_periods_per_day + i]){
						m_periods->SetCellValue(1 + i, 1 + j, m_owner->m_lang->str_class_availible);
						m_periods->SetCellBackgroundColour(1 + i, 1 + j, wxColor(200,200,255));
					}
				}
			}
		} else {
			for(int i = new_exit; i < last_exit; ++i){
				for(int j = 0; j < school->n_days; ++j){
					m_periods->SetCellImmutable(1 + i, 1 + j);
				}
			}
		}
		last_exit = new_exit;
	}
}

AddClassPane::~AddClassPane(){

}
