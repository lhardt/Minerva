#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "util.h"
};

AddSubjectGroupPane::AddSubjectGroupPane(Application * owner, wxWindow * parent, wxPoint pos)  : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(250,250,250));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_name, wxDefaultPosition, wxSize(200,15));
	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_subjects_to_the_group, wxDefaultPosition, wxSize(400,15));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1));

	name_label->SetFont(*m_owner->m_small_font);
	subjects_label->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1));
	m_subjects_grid = new ChoiceGrid(m_owner, this, wxID_ANY);
	wxButton * add_group = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_group, wxDefaultPosition, wxSize(180,-1));

	m_subjects_grid->AddState(m_owner->m_lang->str_no, wxColor(255,200,200));
	m_subjects_grid->AddState(m_owner->m_lang->str_yes, wxColor(200,200,255));
	wxString col_name = m_owner->m_lang->str_belongs;
	m_subjects_grid->SetColLabel(0, col_name);

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
	Bind(DATA_CHANGE_EVENT, &AddSubjectGroupPane::OnDataChange, this);

	ShowData();
}

void AddSubjectGroupPane::ShowData(){
	School * school = m_owner->m_school;
	for(int i = 0; i < school->n_subjects; ++i){
		wxString row_name = wxString::FromUTF8(school->subjects[i].name);
		m_subjects_grid->SetRowLabel(i, row_name);
	}
	m_subjects_grid->GridRemake(1, school->n_subjects);
}

void AddSubjectGroupPane::OnDataChange(wxNotifyEvent & ev){
	ShowData();
}

void AddSubjectGroupPane::OnCreateButtonClicked(wxCommandEvent & evt){
	int i, n_subjects=0;
	School * school = m_owner->m_school;

	for(i = 0; i < school->n_subjects; ++i){
		n_subjects += (m_subjects_grid->GetCellState(i, 0) > 0)?1:0;
	}

	if(!m_name_text->IsEmpty() && n_subjects > 0){
		char * name = copy_wx_string(m_name_text->GetValue());
		int * members = (int*) calloc(school->n_subjects + 1, sizeof(int));

		members[school->n_subjects] = -1;
		for(i = 0; i < school->n_subjects; ++i){
			if(m_subjects_grid->GetCellState(i,0) > 0){
				members[i] = 1;
			}
		}
		SubjectGroupInsertAction * act = new SubjectGroupInsertAction(m_owner, name, members);
		bool success = m_owner->Do(act);
		if(success){
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
	int i; School * school = m_owner->m_school;
	m_name_text->Clear();
	for(i = 0; i < school->n_subjects; ++i){
		m_subjects_grid->SetCellState(i,0, 0);
	}
}

AddSubjectGroupPane::~AddSubjectGroupPane(){

}
