#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "preprocess.h"
};

AddSubjectPane::AddSubjectPane(Application * owner, wxWindow * parent, wxPoint pos)  : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * subject_name_label = new wxStaticText(this,wxID_ANY,m_owner->m_lang->str_name);
	subject_name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxButton * button = new wxButton(this,wxID_ANY, m_owner->m_lang->str_add, wxDefaultPosition, wxSize(200,30));
	button->Bind(wxEVT_BUTTON, &AddSubjectPane::OnCreateButtonClicked, this);

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));
	m_err_msg->SetFont(*m_owner->m_small_font);

	wxSizer * wrapper = new wxBoxSizer(wxVERTICAL);
	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(subject_name_label, 0, wxEXPAND | wxBOTTOM, 5);
	sizer->Add(m_name_text, 0, wxBOTTOM, 15);
	sizer->Add(button, 0,  wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxEXPAND | wxBOTTOM, 15);
	wrapper->Add(sizer, 1, wxEXPAND | wxALL, 15);
	SetSizerAndFit(wrapper);
}

AddSubjectPane::~AddSubjectPane(){

}

void AddSubjectPane::ClearInsertedData(){
	m_name_text->Clear();
}

void AddSubjectPane::OnCreateButtonClicked(wxCommandEvent & ev){
	if(!m_name_text->GetValue().IsEmpty()){
		Subject subject;
		subject.name = copy_wx_string(m_name_text->GetValue());
		subject.short_name = copy_wx_string(m_name_text->GetValue());
		subject.in_groups = NULL;

		int id = insert_subject(stdout, m_owner->m_database, &subject, m_owner->m_school);
		if(id != -1){
			school_subject_add(m_owner->m_school, &subject);

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
