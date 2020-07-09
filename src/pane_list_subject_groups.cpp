#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

ListSubjectGroupsPane::ListSubjectGroupsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * body_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * fields_sz = new wxFlexGridSizer(2,5,5);
	wxSizer * butn_sz = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_list_of_subject_groups, wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxArrayString group_names;
	for(i = 0; i < school->n_subject_groups; ++i){
		group_names.push_back(wxString::FromUTF8(school->subject_group_names[i]));
	}

	m_groups_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300), group_names);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * members_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_subjects_in_the_group);

	name_label->SetFont(*m_owner->m_bold_text_font);
	members_label->SetFont(*m_owner->m_bold_text_font);

	m_name_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_members_text = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,-1));
	wxButton * edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit, wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(200,30));

	butn_sz->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	butn_sz->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	fields_sz->Add(name_label);
	fields_sz->Add(m_name_text);
	fields_sz->Add(members_label);
	fields_sz->Add(m_members_text);
	desc_sz->Add(fields_sz, 0, wxBOTTOM, 5);
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
	/* TODO */
	int sel = m_groups_list->GetSelection();
	int id =((IntClientData*) m_groups_list->GetClientData(sel))->m_value;
	if(sel != wxNOT_FOUND){
		remove_subject_group(stdout, m_owner->m_database, m_owner->m_school->subject_group_ids[id]);
		m_groups_list->Delete(sel);
		m_owner->NotifyNewUnsavedData();
	}
}

void ListSubjectGroupsPane::OnSelectionChanged(wxCommandEvent & evt){
	int i = 0, i_select = m_groups_list->GetSelection();
	bool more_than_zero = false;
	School * school = m_owner->m_school;
	if(i_select != wxNOT_FOUND){
		m_name_text->SetLabel(wxString::FromUTF8(school->subject_group_names[i_select]));
		m_members_text->SetLabel("");
		for(i = 0; i < school->n_subjects; ++i){
			if(school->subjects[i].in_groups[i_select] > 0){
				if(more_than_zero){
					m_members_text->SetLabel(m_members_text->GetLabel() + wxString::Format(wxT("\n%s"),
						wxString::FromUTF8(school->subjects[i].name)));
				} else {
					m_members_text->SetLabel(m_members_text->GetLabel() + wxString::Format(wxT("%s"),
						wxString::FromUTF8(school->subjects[i].name)));
					more_than_zero = true;
				}
			}
		}
	}
	Layout();
}

ListSubjectGroupsPane::~ListSubjectGroupsPane(){

}
