#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "preprocess.h"
};

ListFeaturesPane::ListFeaturesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	m_selected_index = -1;

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_list_of_features, wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	m_features_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));
	m_features_list->Bind(wxEVT_LISTBOX,&ListFeaturesPane::OnSelectionChange, this);
	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	name_label->SetFont(*m_owner->m_bold_text_font);
	m_name_text = new wxStaticText(this, wxID_ANY, wxT(""));

	wxButton * edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit, wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(200,30));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer* body_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* description_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * field_sizer = new wxBoxSizer(wxHORIZONTAL);

	body_sizer->Add(m_features_list, 0, wxEXPAND|wxALL, 15);
	body_sizer->Add(description_sizer, 1, wxEXPAND|wxALL, 15);

	btn_sizer->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	btn_sizer->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	description_sizer->Add(field_sizer, 0, 0);
	description_sizer->AddStretchSpacer();
	description_sizer->Add(btn_sizer, 0, 0);

	field_sizer->Add(name_label, 0, wxRIGHT, 10);
	field_sizer->Add(m_name_text, 0, wxRIGHT, 10);

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
	if(m_features_list->GetSelection() != wxNOT_FOUND){
		m_selected_index = m_features_list->GetSelection();
		m_name_text->SetLabel(wxString::FromUTF8( m_owner->m_school->feature_names[m_selected_index]));
	}
}

void ListFeaturesPane::OnRemoveButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	if(m_features_list->GetSelection() != wxNOT_FOUND){
		bool success = remove_feature(stdout, m_owner->m_database, school->feature_ids[m_selected_index]);
		if(success){
			school_feature_remove(school, m_selected_index);

			m_name_text->SetLabel(wxT(""));
			m_features_list->Delete(m_selected_index);
			m_owner->NotifyNewUnsavedData();
		}
	}
}


ListFeaturesPane::~ListFeaturesPane(){

}
