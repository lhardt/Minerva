#include "gui.hpp"

DescSchoolPane::DescSchoolPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Detalhes da Escola"), wxPoint(30,30), wxSize(200,25));
	title->SetFont(*m_owner->m_page_title_font);

}

DescSchoolPane::~DescSchoolPane(){

}
