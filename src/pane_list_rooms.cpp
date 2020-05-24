#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

ListRoomsPane::ListRoomsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * body_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * butn_sz = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Lista de Salas"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	m_rooms_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));

	m_name_text = new wxStaticText(this, wxID_ANY, wxT("Nome:"), wxDefaultPosition, wxSize(300,30));
	m_size_text = new wxStaticText(this, wxID_ANY, wxT("Tamanho:"), wxDefaultPosition, wxSize(300,30));
	m_features_text = new wxStaticText(this, wxID_ANY, wxT("Características:"), wxDefaultPosition, wxSize(300,30));

	wxButton * edit_btn = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,wxT("Remover"), wxDefaultPosition, wxSize(200,30));

	wxStaticText * periods_text = new wxStaticText(this, wxID_ANY, wxT("Períodos em que a sala está aberta:"), wxDefaultPosition, wxSize(300,15));
	m_periods_grid = new ChoiceGrid(this, wxID_ANY, wxDefaultPosition, wxSize(300,200));

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(wxT("Aberta"));
	grid_values.push_back(wxT("Fechada"));
	m_periods_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200));
	m_periods_grid->SetBackgroundColors(grid_colors);

	m_periods_grid->m_basic_col_name = wxT("Dia");
	m_periods_grid->m_basic_row_name = wxT("Período");

	edit_btn->Disable();

	if(school->n_rooms > 0){
		wxArrayString list;
		for(i = 0; i < school->n_rooms; ++i){
			list.Add(wxString::FromUTF8(school->rooms[i].name));
		}
		m_rooms_list->InsertItems(list,0);
	}

	butn_sz->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	butn_sz->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	desc_sz->Add(m_name_text, 0, wxBOTTOM, 5);
	desc_sz->Add(m_size_text, 0, wxBOTTOM, 5);
	desc_sz->Add(m_features_text, 0, wxBOTTOM, 5);
	desc_sz->Add(periods_text, 0, wxTOP | wxLEFT, 15);
	desc_sz->Add(m_periods_grid, 0, wxBOTTOM|wxLEFT, 15);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(butn_sz, 0, 0);

	body_sz->Add(m_rooms_list, 0, wxEXPAND|wxALL, 15);
	body_sz->Add(desc_sz, 1, wxEXPAND|wxALL, 15);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(body_sz, 1, wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	edit_btn->Bind(wxEVT_BUTTON, &ListRoomsPane::OnEditButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListRoomsPane::OnDeleteButtonClicked, this);
	m_rooms_list->Bind(wxEVT_LISTBOX, &ListRoomsPane::OnSelectionChanged, this);
}

void ListRoomsPane::OnEditButtonClicked(wxCommandEvent &){
	printf("Edit clicked\n");
}

void ListRoomsPane::OnDeleteButtonClicked(wxCommandEvent &){
	printf("Delete clicked\n");
	School * school = m_owner->m_school;
	int i;
	if(m_rooms_list->GetSelection() != wxNOT_FOUND){
		int del_i = m_rooms_list->GetSelection();
		bool success = remove_room(stdout, m_owner->m_database, school->rooms[del_i].id);

		if(success){
			if(school->all_meetings != nullptr){
				for(i = 0; school->all_meetings[i].m_class != nullptr; ++i){
					if(school->all_meetings[i].room->id == school->rooms[del_i].id){
						school->all_meetings[i].room = nullptr;
					}
				}
			}
			for(i = del_i; i < school->n_rooms; ++i){
				school->rooms[i] = school->rooms[i+ 1];
			}
			m_rooms_list->Delete(m_selected_index);
		} else {
			printf("Não foi possível apagar.");
		}
	}
}

void ListRoomsPane::OnSelectionChanged(wxCommandEvent &){
	printf("Selection chagned");
	int i;
	School * school = m_owner->m_school;
	if(m_rooms_list->GetSelection() != wxNOT_FOUND){
		Room * room = & school->rooms[m_rooms_list->GetSelection()];
		m_selected_index = m_rooms_list->GetSelection();
		m_name_text->SetLabel(wxString::Format(wxT("Nome: %s"), wxString::FromUTF8( room->name )));
		m_size_text->SetLabel(wxString::Format(wxT("Tamanho: %d"), ( room->size )));
		m_features_text->SetLabel(wxT("Características: "));
		for(i = 0; i < school->n_features && room->room_features[i] >= 0; ++i){
			if(room->room_features[i] > 0){
				m_features_text->SetLabel(m_features_text->GetLabel().Append(wxString::Format("\n\t%s: %d", school->feature_names[i], room->room_features[i])));
			}
		}
		m_periods_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);
		for(i = 0; i < school->n_periods; ++i){
			if(school->periods[i] == false){
				m_periods_grid->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
			} else {
				m_periods_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
						wxString::Format("%s" , (room->disponibility[i] > 0?wxT("Aberta"):wxT("Fechada")) ));
				m_periods_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
						(room->disponibility[i] > 0?wxColor(200,200,255):wxColor(255,200,200)));
			}
			m_periods_grid->SetReadOnly(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), true);
		}
	}
}

ListRoomsPane::~ListRoomsPane(){

}
