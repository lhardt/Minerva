#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

ListTeacherGroupsPane::ListTeacherGroupsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * butn_sz = new wxBoxSizer(wxHORIZONTAL);

	wxArrayString group_names;
	for(i = 0; i < school->n_teachers; ++i){
		if(school->teachers[i].subordinates != NULL){
			group_names.push_back(school->teachers[i].name);
		}
	}
	m_groups_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(230,300), group_names);
	m_name_text = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name, wxDefaultPosition, wxSize(400,25));
	m_members_text = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_members, wxDefaultPosition, wxSize(400,-1));

	wxButton * edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit, wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(200,30));

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,30));

	butn_sz->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	butn_sz->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	desc_sz->Add(m_name_text, 0, wxBOTTOM, 5);
	desc_sz->Add(m_members_text, 0, wxBOTTOM, 5);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(m_err_msg,0, wxBOTTOM, 5);
	desc_sz->Add(butn_sz);

	sizer->Add(m_groups_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(desc_sz, 1, wxEXPAND|wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_groups_list->Bind(wxEVT_LISTBOX, &ListTeacherGroupsPane::OnSelectionChanged, this);
	edit_btn->Bind(wxEVT_BUTTON, &ListTeacherGroupsPane::OnEditButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListTeacherGroupsPane::OnDeleteButtonClicked, this);
}


void ListTeacherGroupsPane::OnEditButtonClicked(wxCommandEvent &) {
	m_err_msg->SetLabel(wxT("Not Implemented."));
}

void ListTeacherGroupsPane::OnDeleteButtonClicked(wxCommandEvent &) {
	int i_sel, i_teacher, i_group = 0;
	School * school = m_owner->m_school;
	i_sel = m_groups_list->GetSelection();
	if(i_sel != wxNOT_FOUND){
		for(i_teacher = 0; i_teacher < school->n_teachers && i_group <= i_sel; ++i_teacher){
			if(school->teachers[i_teacher].subordinates != NULL){
				++i_group;
			}
		}
		if(i_group == i_sel){
			--i_teacher;
			bool success = remove_teacher(stdout, m_owner->m_database, school->teachers[i_teacher].id);
			if(success){
				m_groups_list->Delete(i_sel);
				m_err_msg->SetLabel(m_owner->m_lang->str_success);
				m_owner->NotifyNewUnsavedData();
			} else {
				m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
			}
		} else {
			printf("ERROR IN THE CODE! F: %s, L: %d\n", __FILE__, __LINE__);
		}
	}
}

void ListTeacherGroupsPane::OnSelectionChanged(wxCommandEvent &) {
	int i, i_teacher, i_group = 0, i_sel = m_groups_list->GetSelection();
	Teacher * t;
	School * school = m_owner->m_school;
	if(i_sel != wxNOT_FOUND){
		for(i_teacher = 0; i_teacher < school->n_teachers && i_group <= i_sel; ++i_teacher){
			if(school->teachers[i_teacher].subordinates != NULL){
				++i_group;
			}
		}
		if(i_group == i_sel+1){
			--i_teacher;
			t = &school->teachers[i_teacher];
			m_name_text->SetLabel(m_owner->m_lang->str_name + wxString::FromUTF8(t->name));
			m_members_text->SetLabel(m_owner->m_lang->str_group_members);
			for(i = 0; i < school->n_teachers; ++i){
				if(t->subordinates[i] > 0){
					m_members_text->SetLabel(m_members_text->GetLabel() + wxT("\n\t") + wxString::FromUTF8(school->teachers[i].name));
				}
			}
		} else {
			printf("Not found.\n");
		}
	}
}

ListTeacherGroupsPane::~ListTeacherGroupsPane(){

}
