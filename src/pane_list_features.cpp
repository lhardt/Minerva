#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

ListFeaturesPane::ListFeaturesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	m_selected_index = -1;

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Lista de Características"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	m_features_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));
	m_features_list->Bind(wxEVT_LISTBOX,&ListFeaturesPane::OnSelectionChange, this);

	wxSizer* body_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* description_sizer = new wxBoxSizer(wxVERTICAL);

	body_sizer->Add(m_features_list, 0, wxEXPAND|wxALL, 15);
	body_sizer->Add(description_sizer, 1, wxEXPAND|wxALL, 15);


	m_name_text = new wxStaticText(this, wxID_ANY, wxT("Nome:"), wxDefaultPosition, wxSize(300,30));

	wxSizer * btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton * edit_btn = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,wxT("Remover"), wxDefaultPosition, wxSize(200,30));
	btn_sizer->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	btn_sizer->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	description_sizer->Add(m_name_text, 0, 0);
	description_sizer->AddStretchSpacer();
	description_sizer->Add(btn_sizer, 0, 0);

	edit_btn->Disable();
	delete_btn->Bind(wxEVT_BUTTON, &ListFeaturesPane::OnRemoveButtonClicked, this);

	wxArrayString list;
	for(i = 0; i < school->n_features; ++i){
		list.Add(wxString::FromUTF8(school->feature_names[i]));
	}
	if(school->n_features > 0){
		m_features_list->InsertItems(list,0);
	}

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(body_sizer, 1, wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();
}

void ListFeaturesPane::OnSelectionChange(wxCommandEvent & ev){
	printf("Selection changed;\n");
	if(m_features_list->GetSelection() != wxNOT_FOUND){
		m_selected_index = m_features_list->GetSelection();
		m_name_text->SetLabel(wxString::Format(wxT("Nome: %s"), wxString::FromUTF8( m_owner->m_school->feature_names[m_selected_index] )));
	}
}

void ListFeaturesPane::OnRemoveButtonClicked(wxCommandEvent & ev){
	printf("remove clicked\n");
	School * school = m_owner->m_school;
	if(m_selected_index != -1){
		bool success = remove_feature(stdout, m_owner->m_database, school->feature_ids[m_selected_index]);
		if(success){
			free(school->feature_names[m_selected_index]);

			for(int i = m_selected_index; i < school->n_features && school->feature_ids[i] >= 0; ++i ){
				school->feature_names[i] = school->feature_names[i+1];
				school->feature_ids[i] = school->feature_ids[i+1];
			}

			/* If there are features right of the deleted, push them back, in all structures. */
			if(m_selected_index == MAX_FEATURES -1 || school->feature_ids[m_selected_index+1] != -1){
				if(school->rooms != NULL && school->n_rooms > 0){
					for(int i = 0;  i < school->n_rooms; ++i ){
						for(int j = m_selected_index + 1; (j < MAX_FEATURES -1) && (school->rooms[i].room_features[j] >= 0); ++j){
							/* copies the -1 terminator too. */
							school->rooms[i].room_features[j] = school->rooms[i].room_features[j+1];
						}
					}
				}

				if(school->teaches != NULL && school->n_teaches > 0){
					for(int i = 0; i < school->n_teaches; ++i){
						for(int j = m_selected_index + 1; (j < MAX_FEATURES -1) && (school->teaches[i].features[j] >= 0); ++j){
							/* copies the -1 terminator too. */
							school->teaches[i].features[j] = school->teaches[i].features[j+1];
						}
						/* May have a different terminator. */
						for(int j = m_selected_index + 1; (j < MAX_FEATURES -1) && (school->teaches[i].features[j] >= 0); ++j){
							school->teaches[i].min_features[j] = school->teaches[i].min_features[j+1];
						}
					}
				}
			}
			m_features_list->Delete(m_selected_index);
		}
	}
}


ListFeaturesPane::~ListFeaturesPane(){

}
