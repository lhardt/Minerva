#include "gui.hpp"

ListTeacherGroupsPane::ListTeacherGroupsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * body_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * butn_sz = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Grupos de Professores"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	m_groups_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));
	m_members_text = new wxStaticText(this, wxID_ANY, wxT("Membros do Grupo"), wxDefaultPosition, wxSize(400,25));

	wxButton * edit_btn = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,wxT("Remover"), wxDefaultPosition, wxSize(200,30));

	butn_sz->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	butn_sz->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	desc_sz->Add(m_members_text, 0, wxBOTTOM, 5);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(butn_sz, 0, 0);

	body_sz->Add(m_groups_list, 0, wxEXPAND|wxALL, 15);
	body_sz->Add(desc_sz, 1, wxEXPAND|wxALL, 15);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(body_sz, 1, wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

}


void ListTeacherGroupsPane::OnEditButtonClicked(wxCommandEvent &) {

}

void ListTeacherGroupsPane::OnDeleteButtonClicked(wxCommandEvent &) {

}

void ListTeacherGroupsPane::OnSelectionChanged(wxCommandEvent &) {

}

ListTeacherGroupsPane::~ListTeacherGroupsPane(){

}
