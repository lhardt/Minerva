#include "gui.hpp"


AddCharacteristicPane::AddCharacteristicPane(Application * owner, wxWindow * parent, wxPoint pos)  : wxPanel(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Adicionar Característica"), wxPoint(30,30), wxSize(200,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, wxT("Nome da Característica"), wxPoint(30,75), wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,90), wxSize(200,30));

	wxButton * button_go = new wxButton(this,wxID_ANY, wxT("Adicionar"), wxPoint(30,135), wxSize(200,30));

	button_go->Bind(wxEVT_BUTTON, &AddCharacteristicPane::OnCreateButtonClicked, this);
}

AddCharacteristicPane::~AddCharacteristicPane(){

}

void AddCharacteristicPane::OnCreateButtonClicked(wxCommandEvent & ev){
	printf("Button clicked!\n");
}
