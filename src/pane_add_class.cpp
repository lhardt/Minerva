#include "gui.hpp"

#include <wx/spinctrl.h>

extern "C" {
	#include "loader.h"
	#include "util.h"
};

AddClassPane::AddClassPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(250,250,250));
	int i;
	School * school = m_owner->m_school;

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_name);
	wxStaticText * size_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_size);
	wxStaticText * entry_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_entry_period);
	wxStaticText * exit_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_exit_period);
	wxStaticText * periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class_availability);
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
	m_periods = new ChoiceGrid(m_owner,this, wxID_ANY, wxDefaultPosition, wxSize(400,-1));
	wxButton * add_class = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_class, wxDefaultPosition, wxSize(220,-1));
	m_subjects_grid = new wxGrid(this, wxID_ANY);
	PosIntGridTable * subjects_grid_table = new PosIntGridTable(school->n_subjects,1);

	wxString col_name = m_owner->m_lang->str_amount;
	subjects_grid_table->SetColLabelValue(0, col_name);
	for(i = 0; i < school->n_subjects; ++i){
		wxString name = wxString::FromUTF8(school->subjects[i].name);
		subjects_grid_table->SetRowLabelValue(i, name);
	}
	m_subjects_grid->SetTable(subjects_grid_table, true);
	m_subjects_grid->AutoSizeColumn(0, true);
	m_subjects_grid->SetLabelBackgroundColour( wxColor(255,255,255) );


	for(i = 0; i < school->n_periods_per_day; ++i){
		m_entry_text->Append(wxString::FromUTF8(school->daily_period_names[i]), new IntClientData(i));
		m_exit_text->Append(wxString::FromUTF8(school->daily_period_names[i]), new IntClientData(i));
	}
	m_free_periods_checkbox->SetFont(*m_owner->m_small_font);

	m_periods->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	m_periods->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	m_periods->SetDefaultColumnLabel(m_owner->m_lang->str_day);
	m_periods->SetDefaultRowLabel(m_owner->m_lang->str_period);
	m_periods->GridRemake(school->n_days,school->n_periods_per_day);

	last_entry = 0;
	last_exit = school->n_periods_per_day;

	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i] == false){
			m_periods->SetCellLocked(i % school->n_periods_per_day, i / school->n_periods_per_day);
		}
	}

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(name_label, 0, wxLEFT | wxTOP ,15);
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
	sizer->Add(m_subjects_grid, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_class, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_entry_text->Bind(wxEVT_CHOICE, &AddClassPane::OnPeriodChoice, this);
	m_exit_text->Bind(wxEVT_CHOICE, &AddClassPane::OnPeriodChoice, this);
	add_class->Bind(wxEVT_BUTTON, &AddClassPane::OnAddClassButtonClicked, this);
	Bind(DATA_CHANGE_EVENT, &AddClassPane::OnDataChange, this);
}

void AddClassPane::OnDataChange(wxNotifyEvent & evt) {
	printf("Data change!\n");
}

void AddClassPane::OnAddClassButtonClicked(wxCommandEvent & ev){
	int i, i_subject, i_need, n_needs;
	Assignment * alist = NULL;
	School * school = m_owner->m_school;
	if((!m_name_text->GetValue().IsEmpty()) && (m_size_text->GetValue() > 0)  && (m_entry_text->GetSelection() != wxNOT_FOUND)
			&& (m_exit_text->GetSelection() != wxNOT_FOUND)){
		Class c;
		c.name = copy_wx_string(m_name_text->GetValue());
		c.short_name = copy_wx_string(m_name_text->GetValue());
		c.size = m_size_text->GetValue();
		c.period_scores = (int *) calloc(school->n_periods + 1, sizeof(int));
		for(i = 0; i < school->n_periods; ++i){
			int state = m_periods->GetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day);
			c.period_scores[i] = state >= 0? state:0; /* State -1 is for blocked cells */
		}
		c.period_scores[school->n_periods] = -1;
		c.room_scores = nullptr;
		c.can_have_free_periods_flag = m_free_periods_checkbox->GetValue();
		c.maximal_entry_period = m_entry_text->GetSelection();
		c.minimal_exit_period = m_exit_text->GetSelection();
		c.max_per_day_subject_group = NULL;
		c.active = true;

		n_needs = 0;
		for(i = 0; i < school->n_subjects; ++i){
			long long_val;
			if((m_subjects_grid->GetCellValue(i,0).ToLong(&long_val)) && long_val > 0){
				++n_needs;
			}
		}
		if(n_needs > 0){
			i_need = 0;
			c.assignments = (Assignment**) calloc(n_needs + 1, sizeof(Assignment*));
			alist = (Assignment *) calloc(n_needs+1, sizeof(Assignment));

			for(i_subject = 0; i_subject < school->n_subjects; ++i_subject){
				long n_per;
				if(m_subjects_grid->GetCellValue(i_subject,0).ToLong(&n_per) && n_per > 0){
					c.assignments[i_need] = &alist[i_need];
					alist[i_need].subject = &school->subjects[i_subject];
					alist[i_need].amount = (int) n_per;
					alist[i_need].m_class = &c;
					++i_need;
				}
			}
			alist[i_need].subject = NULL;
			alist[i_need].amount = -1;
		} else {
			c.assignments = nullptr;
		}
		c.subordinates = nullptr;

		bool success = false;
		Action * act = new ClassInsertAction(m_owner, c);
		if(can_insert_class(school, &c)){
			bool success =  m_owner->Do(act);
			if(success){
				m_err_msg->SetLabel(m_owner->m_lang->str_success);
				ClearInsertedData();
				// TODO Does the action take ownership over c.assignments?
			} else {
				m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
			}
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_repeated_name_error);
		}
		// FREEING
		if(n_needs > 0){
			free(alist);
		}
		if(! success){
			free(c.name);
			free(c.short_name);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}

void AddClassPane::ClearInsertedData(){
	School * school = m_owner->m_school;
	int i = 0;
	m_name_text->Clear();
	m_periods->SetAllCellsState(0);
	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i] == false){
			m_periods->SetCellLocked(i % school->n_periods_per_day, i / school->n_periods_per_day);
		}
	}
	m_size_text->SetValue(0);
	m_free_periods_checkbox->SetValue(false);
	last_entry = -1;
	last_exit = -1;

	m_entry_text->SetSelection(wxNOT_FOUND);
	m_exit_text->SetSelection(wxNOT_FOUND);
}

void AddClassPane::OnPeriodChoice(wxCommandEvent& ev){
	School * school = m_owner->m_school;
	if(m_entry_text->GetSelection() != wxNOT_FOUND){
		int new_entry = m_entry_text->GetSelection();

		if(new_entry > last_entry){
			for(int i = last_entry; i < new_entry; ++i){
				for(int j = 0; j < school->n_days; ++j){
					m_periods->SetCellLocked(i,j);
				}
			}
		} else {
			for(int i = new_entry; i < last_entry; ++i){
				for(int j = 0; j < school->n_days; ++j){
					if(school->periods[j * school->n_periods_per_day + i]){
						m_periods->SetCellState(i,j, 1);
					} /* Else the cell is locked anyway */
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
						m_periods->SetCellState(i,j, 1);
					}
				}
			}
		} else {
			for(int i = new_exit; i < last_exit; ++i){
				for(int j = 0; j < school->n_days; ++j){
					m_periods->SetCellLocked(i,j);
				}
			}
		}
		last_exit = new_exit;
	}
}

AddClassPane::~AddClassPane(){

}
