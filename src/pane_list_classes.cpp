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

	wxSizer * btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton * edit_btn = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,wxT("Remover"), wxDefaultPosition, wxSize(200,30));

	delete_btn->Disable();
	btn_sizer->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	btn_sizer->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	description_sizer->Add(m_name_text, 0, 0);
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
	if(m_classes_list->GetSelection() != wxNOT_FOUND){
		Class * c = &m_owner->m_school->classes[m_classes_list->GetSelection()];
		m_name_text->SetLabel(wxT("Nome: " + wxString::FromUTF8(c->name)));
	}
}

void ListClassesPane::OnEditButtonClicked(wxCommandEvent & ev){
	printf("Edit clicked\n");
}

void ListClassesPane::OnRemoveButtonClicked(wxCommandEvent & ev){
	printf("Remove clicked.\n");
}

ListClassesPane::~ListClassesPane(){

}
