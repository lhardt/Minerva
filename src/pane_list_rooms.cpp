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

	wxButton * edit_btn = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,wxT("Remover"), wxDefaultPosition, wxSize(200,30));

	delete_btn->Disable();

	wxArrayString list;
	for(i = 0; i < school->n_rooms; ++i){
		list.Add(wxString::FromUTF8(school->rooms[i].name));
	}
	if(school->n_rooms > 0){
		m_rooms_list->InsertItems(list,0);
	}

	butn_sz->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	butn_sz->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	desc_sz->Add(m_name_text, 0, wxBOTTOM, 15);
	desc_sz->Add(m_size_text, 0, wxBOTTOM, 15);
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
	if(m_rooms_list->GetSelection() != wxNOT_FOUND){
		int del_i = m_rooms_list->GetSelection();
		bool success = remove_room(stdout, m_owner->m_database, school->rooms[del_i].id);

	}
}

void ListRoomsPane::OnSelectionChanged(wxCommandEvent &){
	printf("Selection chagned");
	if(m_rooms_list->GetSelection() != wxNOT_FOUND){
		Room * room = & m_owner->m_school->rooms[m_rooms_list->GetSelection()];
		m_selected_index = m_rooms_list->GetSelection();
		m_name_text->SetLabel(wxString::Format(wxT("Nome: %s"), wxString::FromUTF8( room->name )));
		m_size_text->SetLabel(wxString::Format(wxT("Tamanho: %d"), ( room->size )));
	}
}

ListRoomsPane::~ListRoomsPane(){

}
