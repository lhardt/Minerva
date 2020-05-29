#include "gui.hpp"

extern "C" {
	#include "loader.h"
};


AddFeaturePane::AddFeaturePane(Application * owner, wxWindow * parent, wxPoint pos)  : wxPanel(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Adicionar Característica"), wxPoint(30,30), wxSize(200,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, wxT("Nome da Característica"), wxPoint(30,75), wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,90), wxSize(200,30));

	wxButton * button_go = new wxButton(this,wxID_ANY, wxT("Adicionar"), wxPoint(30,135), wxSize(200,30));

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxPoint(30, 180), wxSize(300,30));
	m_err_msg->SetFont(*m_owner->m_small_font);

	button_go->Bind(wxEVT_BUTTON, &AddFeaturePane::OnCreateButtonClicked, this);
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

			school->n_features++;

			ClearInsertedData();
			m_err_msg->SetLabel(wxT("Inserido com sucesso."));
		} else {
			m_err_msg->SetLabel(wxT("Não foi possível inserir.\n Possível erro no banco de dados"));
		}
	} else {
		m_err_msg->SetLabel(wxT("Não é possível adicionar uma característica sem nome."));
	}
}

void AddFeaturePane::ClearInsertedData(){
	m_name_text->SetValue("");
}
