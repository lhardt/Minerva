#include "gui.hpp"

ListSubjectGroupsPane::ListSubjectGroupsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * body_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * butn_sz = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Lista de Grupos de Disciplinas"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxArrayString group_names;
	for(i = 0; i < school->n_subject_groups; ++i){
		group_names.push_back(wxString::FromUTF8(school->subject_group_names[i]));
	}

	m_groups_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300), group_names);

	m_name_text = new wxStaticText(this, wxID_ANY, wxT("Nome:"), wxDefaultPosition, wxSize(300,30));
	m_members_text = new wxStaticText(this, wxID_ANY, wxT("Disciplinas no Grupo:"), wxDefaultPosition, wxSize(300,-1));

	wxButton * edit_btn = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,wxT("Remover"), wxDefaultPosition, wxSize(200,30));

	butn_sz->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	butn_sz->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	desc_sz->Add(m_name_text, 0, wxBOTTOM, 5);
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

	edit_btn->Bind(wxEVT_BUTTON, &ListSubjectGroupsPane::OnEditButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListSubjectGroupsPane::OnDeleteButtonClicked, this);
	m_groups_list->Bind(wxEVT_LISTBOX, &ListSubjectGroupsPane::OnSelectionChanged, this);

}

void ListSubjectGroupsPane::OnEditButtonClicked(wxCommandEvent & evt){

}

void ListSubjectGroupsPane::OnDeleteButtonClicked(wxCommandEvent & evt){

}

void ListSubjectGroupsPane::OnSelectionChanged(wxCommandEvent & evt){
	int i = 0, i_select = m_groups_list->GetSelection();
	School * school = m_owner->m_school;
	if(i_select != wxNOT_FOUND){
		m_name_text->SetLabel(wxString::Format(wxT("Nome: %s"), wxString::FromUTF8(school->subject_group_names[i_select])));
		m_members_text->SetLabel(wxT("Disciplinas no Grupo:"));
		for(i = 0; i < school->n_subjects; ++i){
			if(school->subjects[i].in_groups[i_select] > 0){
				m_members_text->SetLabel(m_members_text->GetLabel() + wxString::Format(wxT("\n%s"),
						wxString::FromUTF8(school->subjects[i].name)));

			}
		}
	}
}

ListSubjectGroupsPane::~ListSubjectGroupsPane(){

}
