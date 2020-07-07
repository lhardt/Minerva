#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

AddSubjectPane::AddSubjectPane(Application * owner, wxWindow * parent, wxPoint pos)  : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_subject, wxDefaultPosition, wxSize(200,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * subject_name_label = new wxStaticText(this,wxID_ANY,m_owner->m_lang->str_name, wxDefaultPosition, wxSize(200,15));
	subject_name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxButton * button = new wxButton(this,wxID_ANY, m_owner->m_lang->str_add, wxDefaultPosition, wxSize(200,30));
	button->Bind(wxEVT_BUTTON, &AddSubjectPane::OnCreateButtonClicked, this);

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxPoint(30, 180), wxSize(300,30));
	m_err_msg->SetFont(*m_owner->m_small_font);


	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(title,0, wxFIXED_MINSIZE | wxALL, 15);
	sizer->Add(subject_name_label,0, wxFIXED_MINSIZE | wxTOP | wxLEFT, 15);
	sizer->Add(m_name_text,0, wxFIXED_MINSIZE | wxLEFT, 15);
	sizer->Add(button,0, wxFIXED_MINSIZE | wxTOP | wxLEFT, 15);
	sizer->Add(m_err_msg,0, wxFIXED_MINSIZE | wxTOP | wxLEFT, 15);
	SetSizerAndFit(sizer);
}

AddSubjectPane::~AddSubjectPane(){

}

void AddSubjectPane::ClearInsertedData(){
	m_name_text->Clear();
}

void AddSubjectPane::OnCreateButtonClicked(wxCommandEvent & ev){
	int i;
	School * school = m_owner->m_school;
	if(!m_name_text->GetValue().IsEmpty()){
		Subject subject;

		subject.name = copy_wx_string(m_name_text->GetValue());
		subject.short_name = copy_wx_string(m_name_text->GetValue());

		for(i = 0; i < MAX_GROUPS; ++i){
			subject.in_groups[i] = 0;
		}
		subject.in_groups[ MAX_GROUPS ] = -1;

		int id = insert_subject(stdout, m_owner->m_database, &subject, school);
		if(id != -1){
			if(school->subjects == nullptr || school->n_subjects == 0){
				school->subjects = (Subject*)calloc(10, sizeof(Subject));
				school->n_subjects = 0;
			} else if(school->n_subjects % 10 == 0) {
				school->subjects = (Subject*)realloc(school->subjects,(school->n_subjects +11)*sizeof(Subject));
			}
			school->subjects[ school->n_subjects ] = subject;
			school->n_subjects++;

			m_err_msg->SetLabel(m_owner->m_lang->str_success);
			m_owner->NotifyNewUnsavedData();
			ClearInsertedData();
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}
