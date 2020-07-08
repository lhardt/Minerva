#include "gui.hpp"
#include <wx/richtooltip.h>

extern "C" {
	#include "loader.h"
};


AddFeaturePane::AddFeaturePane(Application * owner, wxWindow * parent, wxPoint pos)  : wxPanel(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * wrapper = new wxBoxSizer(wxVERTICAL);
	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_feature, wxDefaultPosition, wxSize(300,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name, wxDefaultPosition, wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxButton * button_add = new wxButton(this,wxID_ANY, m_owner->m_lang->str_add, wxDefaultPosition, wxSize(200,30));

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,30));
	m_err_msg->SetFont(*m_owner->m_small_font);

	sizer->Add(title, 0, wxEXPAND , 15);
	sizer->Add(name_label, 0, wxEXPAND | wxTOP, 15);
	sizer->Add(m_name_text, 0, wxBOTTOM, 15);
	sizer->Add(button_add, 0, wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxEXPAND | wxBOTTOM, 15);

	wrapper->Add(sizer, 0, wxALL, 30);
	SetSizerAndFit(wrapper);

	button_add->Bind(wxEVT_BUTTON, &AddFeaturePane::OnCreateButtonClicked, this);
}

AddFeaturePane::~AddFeaturePane(){

}

void AddFeaturePane::OnCreateButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	if(!m_name_text->GetValue().IsEmpty()){
		int id = insert_feature(stdout, m_owner->m_database, m_name_text->GetValue().ToUTF8().data(), school);
		if(id != -1){
			if(school->n_features == 0){
				school->feature_names = (char**) calloc(10, sizeof(char *));
				school->feature_ids = (int*) calloc(10, sizeof(int));
			} else if(school->n_features % 10 == 0){
				school->feature_names = (char**) realloc(school->feature_names, school->n_features + 10);
				school->feature_ids = (int*) realloc(school->feature_ids, school->n_features + 10);
			}
			school->feature_names[school->n_features] = copy_wx_string(m_name_text->GetValue());
			school->feature_ids[school->n_features] = id;

			school->n_features++;

			ClearInsertedData();
			m_err_msg->SetLabel(m_owner->m_lang->str_success);
			m_owner->NotifyNewUnsavedData();
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}

void AddFeaturePane::ClearInsertedData(){
	m_name_text->SetValue("");
}
