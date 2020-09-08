#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "preprocess.h"
};

AddSubjectGroupPane::AddSubjectGroupPane(Application * owner, wxWindow * parent, wxPoint pos)  : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_name, wxDefaultPosition, wxSize(200,15));
	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_subjects_to_the_group, wxDefaultPosition, wxSize(400,15));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	name_label->SetFont(*m_owner->m_small_font);
	subjects_label->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));
	m_subjects_grid = new ChoiceGrid(m_owner, this, wxID_ANY);
	wxButton * add_group = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_group, wxDefaultPosition, wxSize(180,30));

	m_subjects_grid->AddState(m_owner->m_lang->str_no, wxColor(255,200,200));
	m_subjects_grid->AddState(m_owner->m_lang->str_yes, wxColor(200,200,255));
	wxString col_name = m_owner->m_lang->str_belongs;
	m_subjects_grid->SetColName(0, col_name);
	for(i = 0; i < school->n_subjects; ++i){
		wxString row_name = wxString::FromUTF8(school->subjects[i].name);
		m_subjects_grid->SetRowName(i, row_name);
	}
	m_subjects_grid->GridRemake(1, school->n_subjects);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(name_label, 0, wxLEFT | wxTOP ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_label, 0, wxLEFT, 15);
	sizer->Add(m_subjects_grid, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(add_group, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	add_group->Bind(wxEVT_BUTTON, &AddSubjectGroupPane::OnCreateButtonClicked, this);
}

void AddSubjectGroupPane::OnCreateButtonClicked(wxCommandEvent & evt){
	int i, id, sing_id, n_subjects=0;
	School * school = m_owner->m_school;

	for(i = 0; i < school->n_subjects; ++i){
		n_subjects += (m_subjects_grid->GetCellState(i, 0) > 0)?1:0;
	}

	if(!m_name_text->IsEmpty() && n_subjects > 0){
		char * name = copy_wx_string(m_name_text->GetValue());
		id = insert_subject_group(stdout, m_owner->m_database, school, name, -1);
		if(id != -1){
			school_subjectgroup_add(school, name,id);
			for(i = 0; i < school->n_subjects; ++i){
				if(m_subjects_grid->GetCellState(i,0) > 0){
					sing_id = insert_subject_in_group(stdout, m_owner->m_database, school->subjects[i].id, id);
					if(sing_id >= 0){
						if(school->subjects[i].in_groups == NULL){
							school->subjects[i].in_groups = (int *)calloc(1 + school->n_subject_groups, sizeof(int));
							school->subjects[i].in_groups[school->n_subject_groups] = -1;
						}
						school->subjects[i].in_groups[school->n_subject_groups -1] = 1;
					} else {
						m_err_msg->SetLabel(m_owner->m_lang->str_error);
						break;
					}
				}
			}
			m_err_msg->SetLabel(m_owner->m_lang->str_success);
			ClearInsertedData();
			m_owner->NotifyNewUnsavedData();
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}

void AddSubjectGroupPane::ClearInsertedData(){
	int i; School * school = m_owner->m_school;
	m_name_text->Clear();
	for(i = 0; i < school->n_subjects; ++i){
		m_subjects_grid->SetCellState(i,0, 0);
	}
}

AddSubjectGroupPane::~AddSubjectGroupPane(){

}
