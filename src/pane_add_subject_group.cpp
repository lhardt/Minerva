#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

AddSubjectGroupPane::AddSubjectGroupPane(Application * owner, wxWindow * parent, wxPoint pos)  : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_subject_group, wxPoint(30,30), wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_name, wxDefaultPosition, wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_subjects_to_the_group, wxDefaultPosition, wxSize(400,15));
	subjects_label->SetFont(*m_owner->m_small_font);

	wxArrayString subj_names;
	for(i = 0; i < school->n_subjects; ++i){
		added_subjects[i] = false;
		subj_names.Add(wxString::FromUTF8(school->subjects[i].name));
	}

	m_subjects_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(310,30), subj_names);
	wxButton * add_subject = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_subject, wxDefaultPosition, wxSize(180,30));

	wxButton * remove_subject = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(180,30));
	wxButton * remove_all_subjects = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove_all, wxDefaultPosition, wxSize(180,30));
	m_subjects_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));

	wxButton * add_group = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_group, wxDefaultPosition, wxSize(180,30));

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	add_sizer->Add(m_subjects_choice,0,wxRIGHT,10);
	add_sizer->Add(add_subject,0,wxRIGHT,10);

	wxSizer * buttons_sizer = new wxBoxSizer(wxVERTICAL);
	buttons_sizer->Add(remove_subject, 0, wxBOTTOM, 10);
	buttons_sizer->Add(remove_all_subjects, 0, wxBOTTOM, 10);

	wxSizer * subjects_sizer = new wxBoxSizer(wxHORIZONTAL);
	subjects_sizer->Add(m_subjects_list, 0, wxRIGHT, 10);
	subjects_sizer->Add(buttons_sizer, 0, 0);


	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0, wxLEFT ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_label, 0, wxLEFT, 15);
	sizer->Add(add_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_group, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	add_subject->Bind(wxEVT_BUTTON, &AddSubjectGroupPane::OnAddSubjectButtonClicked, this);
	remove_all_subjects->Bind(wxEVT_BUTTON, &AddSubjectGroupPane::OnRemoveAllButtonClicked, this);
	add_group->Bind(wxEVT_BUTTON, &AddSubjectGroupPane::OnCreateButtonClicked, this);
}

void AddSubjectGroupPane::OnAddSubjectButtonClicked(wxCommandEvent & evt){
	int add_i = m_subjects_choice->GetSelection();
	if(add_i != wxNOT_FOUND && added_subjects[add_i] == false){
		added_subjects[add_i] = true;
		wxString name = wxString::FromUTF8(m_owner->m_school->subjects[add_i].name);
		m_subjects_list->InsertItems(1, &name, m_subjects_list->GetCount());
	}
}

void AddSubjectGroupPane::OnRemoveAllButtonClicked(wxCommandEvent & evt){
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		added_subjects[i] = false;
	}
	m_subjects_list->Clear();
}

void AddSubjectGroupPane::OnCreateButtonClicked(wxCommandEvent & evt){
	int i, id, sing_id ;
	School * school = m_owner->m_school;
	if(!m_name_text->IsEmpty() && !m_subjects_list->IsEmpty()){
		char * name = copy_wx_string(m_name_text->GetValue());
		id = insert_subject_group(stdout, m_owner->m_database, school, name);
		if(id != -1){
			if(school->n_subject_groups == 0 || school->subject_group_names == NULL){
				school->subject_group_names = (char **) calloc(11, sizeof(char *));
				school->subject_group_ids = (int *) calloc(11, sizeof(int));
				school->n_subject_groups = 0;
			} else if(school->n_subject_groups % 10 == 0) {
				school->subject_group_names = (char **) realloc(school->subject_group_names, (school->n_subject_groups + 11) * sizeof(char*));
				school->subject_group_ids = (int *) realloc(school->subject_group_ids, (school->n_subject_groups + 11) * sizeof(int*));
			}

			school->subject_group_names[school->n_subject_groups] = name;
			school->subject_group_ids[school->n_subject_groups] = id;
			++school->n_subject_groups;

			for(i = 0; i < school->n_subjects; ++i){
				if(added_subjects[i]){
					sing_id = insert_subject_in_group(stdout, m_owner->m_database, school->subjects[i].id, id);
					if(sing_id >= 0){
						school->subjects[i].in_groups[school->n_subject_groups -1] = 1;
					} else {
						m_err_msg->SetLabel(m_owner->m_lang->str_error);
					}
				}
			}
			m_err_msg->SetLabel(m_owner->m_lang->str_success);
			ClearInsertedData();
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}

void AddSubjectGroupPane::ClearInsertedData(){
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		added_subjects[i] = false;
	}
	m_name_text->Clear();
	m_subjects_list->Clear();
}


AddSubjectGroupPane::~AddSubjectGroupPane(){

}
