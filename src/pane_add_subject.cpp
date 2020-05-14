#include "gui.hpp"

AddSubjectPane::AddSubjectPane(Application * owner, wxWindow * parent, wxPoint pos)  : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Adicionar Disciplina"), wxDefaultPosition, wxSize(200,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * subject_name_label = new wxStaticText(this,wxID_ANY,wxT("Nome da Disciplina"), wxDefaultPosition, wxSize(200,15));
	subject_name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxButton * button = new wxButton(this,wxID_ANY, wxT("Adicionar"), wxDefaultPosition, wxSize(200,30));
	button->Bind(wxEVT_BUTTON, &AddSubjectPane::OnCreateButtonClicked, this);

	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(title,0, wxFIXED_MINSIZE | wxALL, 15);
	sizer->Add(subject_name_label,0, wxFIXED_MINSIZE | wxTOP | wxLEFT, 15);
	sizer->Add(m_name_text,0, wxFIXED_MINSIZE | wxLEFT, 15);
	sizer->Add(button,0, wxFIXED_MINSIZE | wxTOP | wxLEFT, 15);
	SetSizerAndFit(sizer);
}

AddSubjectPane::~AddSubjectPane(){

}

void AddSubjectPane::OnCreateButtonClicked(wxCommandEvent & ev){
	printf("Button Clicked.\n");
}
