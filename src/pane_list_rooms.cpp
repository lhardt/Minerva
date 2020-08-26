#include "gui.hpp"

extern "C" {
	#include "loader.h"
};
#include <wx/spinctrl.h>

ListRoomsPane::ListRoomsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	m_rooms_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(230,300));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * size_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_size);
	wxStaticText * active_label = new wxStaticText(this, wxID_ANY, wxT("Ativo"));
	wxNotebook   * notebook = new wxNotebook(this, wxID_ANY);
	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove);
	m_active_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_periods 	= new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_err_msg 	= new wxStaticText(this, wxID_ANY, wxT(""));
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_size_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));

	notebook->AddPage(m_periods, m_owner->m_lang->str_periods);

	ChoiceGrid * periods_grid = m_periods->GetGrid();
	periods_grid->AddState(m_owner->m_lang->str_adj__open, wxColor(200,200,255));
	periods_grid->AddState(m_owner->m_lang->str_adj__closed, wxColor(255,200,200));
	periods_grid->m_basic_col_name = m_owner->m_lang->str_day;
	periods_grid->m_basic_row_name = m_owner->m_lang->str_period;

	if(school->n_rooms > 0){
		wxArrayString list;
		for(i = 0; i < school->n_rooms; ++i){
			list.Add(wxString::FromUTF8(school->rooms[i].name));
		}
		m_rooms_list->InsertItems(list,0);
	}
	periods_grid->GridRemake(school->n_days,school->n_periods_per_day);

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * fields_sz = new wxFlexGridSizer(4,5,5);
	wxSizer * fields_wrap = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Dados Básicos"));
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);

	fields_sz->Add(name_label, 0, wxRIGHT | wxEXPAND, 10);
	fields_sz->Add(m_name_text, 0, wxEXPAND);
	fields_sz->Add(size_label, 0, wxRIGHT |wxLEFT |wxEXPAND, 10);
	fields_sz->Add(m_size_text, 0, wxEXPAND);
	fields_sz->Add(active_label, 0, wxRIGHT | wxEXPAND, 10);
	fields_sz->Add(m_active_text, 0, wxEXPAND);
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->Add(m_cancel_btn, 0, wxEXPAND);
	fields_sz->Add(m_edit_btn, 0, wxEXPAND);

	fields_wrap->Add(fields_sz, 0, wxALL | wxEXPAND, 10);
	desc_sz->Add(fields_wrap, 0, wxBOTTOM, 5);
	desc_sz->Add(notebook, 0, wxEXPAND | wxBOTTOM, 5);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(delete_btn, 0, wxEXPAND |wxBOTTOM, 5);
	desc_sz->Add(m_err_msg, 0, 0);

	sizer->Add(m_rooms_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(desc_sz, 0, wxEXPAND|wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_edit_btn->Bind(wxEVT_BUTTON, &ListRoomsPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &ListRoomsPane::OnCancelButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListRoomsPane::OnDeleteButtonClicked, this);
	m_rooms_list->Bind(wxEVT_LISTBOX, &ListRoomsPane::OnSelectionChanged, this);

	m_cancel_btn->Hide();
	m_cancel_btn->Hide();
	m_name_text->Disable();
	m_size_text->Disable();
	m_active_text->Disable();
	m_size_text->Enable();
}

void ListRoomsPane::OnEditButtonClicked(wxCommandEvent &){
	if(m_name_text->IsEnabled()){
		m_cancel_btn->Hide();
		m_name_text->Disable();
		m_size_text->Disable();
		m_active_text->Disable();
		m_size_text->Enable();
		m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	} else {
		m_cancel_btn->Show();
		m_name_text->Enable();
		m_size_text->Enable();
		m_active_text->Enable();
		m_size_text->Enable();
		m_edit_btn->SetLabel(m_owner->m_lang->str_save);
	}
}


void ListRoomsPane::OnCancelButtonClicked(wxCommandEvent &){
	m_cancel_btn->Hide();
	m_name_text->Disable();
	m_size_text->Disable();
	m_active_text->Disable();
	m_size_text->Enable();
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
}

void ListRoomsPane::OnDeleteButtonClicked(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i,j;
	if(m_rooms_list->GetSelection() != wxNOT_FOUND){
		int del_i = m_rooms_list->GetSelection();
		bool success = remove_room(stdout, m_owner->m_database, school->rooms[del_i].id);

		if(success){
			if(school->solutions != nullptr){
				for(i = 0; i < school->n_solutions; ++i){
					Meeting * m_list = school->solutions[i].meetings;
					for(j = 0; m_list[j].m_class != nullptr; ++j){
						if(m_list[j].room->id == school->rooms[del_i].id){
							m_list[j].room = nullptr;
						}
					}
				}
			}
			for(i = del_i; i < school->n_rooms; ++i){
				school->rooms[i] = school->rooms[i+ 1];
			}
			m_rooms_list->Delete(m_selected_index);
			m_owner->NotifyNewUnsavedData();
		} else {
			printf("Não foi possível apagar.");
		}
	}
}

void ListRoomsPane::OnSelectionChanged(wxCommandEvent &){
	int i;
	School * school = m_owner->m_school;
	if(m_rooms_list->GetSelection() != wxNOT_FOUND){
		Room * room = & school->rooms[m_rooms_list->GetSelection()];
		m_selected_index = m_rooms_list->GetSelection();
		m_name_text->SetValue(wxString::FromUTF8( room->name ));
		m_size_text->SetValue(room->size);
		m_active_text->SetValue(room->active);

		ChoiceGrid * periods_grid = m_periods->GetGrid();
		periods_grid->GridRemake(school->n_days,school->n_periods_per_day);
		for(i = 0; i < school->n_periods; ++i){
			if(school->periods[i] == false){
				periods_grid->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
			} else {
				periods_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
						wxString::Format("%s" , (room->availability[i] > 0?m_owner->m_lang->str_adj__open:m_owner->m_lang->str_adj__closed) ));
				periods_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
						(room->availability[i] > 0?wxColor(200,200,255):wxColor(255,200,200)));
			}
			periods_grid->SetReadOnly(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), true);
		}

	}
	Layout();
}

ListRoomsPane::~ListRoomsPane(){

}
