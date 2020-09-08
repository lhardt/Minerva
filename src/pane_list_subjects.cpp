#include "gui.hpp"

extern "C" {
	#include "util.h"
	#include "loader.h"
	#include "preprocess.h"
};

ListSubjectsPane::ListSubjectsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	m_subjects_list = new SearchableListPane(m_owner, this /*, wxID_ANY, wxDefaultPosition, wxSize(230,300) */);
	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""));
	for(i = 0; i < school->n_subjects; ++i){
		m_subjects_list->AddItem(school->subjects[i].id, wxString::FromUTF8(school->subjects[i].name));
	}
	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_cancel_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_cancel);
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove);

	m_name_text->Disable();
	m_cancel_btn->Hide();

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * fields_sz= new wxGridSizer(4,5,5);
	wxSizer * fields_wrap= new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_basic_data);
	fields_sz->Add(name_label);
	fields_sz->Add(m_name_text, 1, wxEXPAND);
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->Add(m_cancel_btn, 1, wxEXPAND);
	fields_sz->Add(m_edit_btn, 1, wxEXPAND);
	fields_wrap->Add(fields_sz, 1, wxALL | wxEXPAND, 5);
	desc_sz->Add(fields_wrap, 0, wxBOTTOM , 5);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(delete_btn, 0, 0);

	sizer->Add(m_subjects_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(desc_sz, 1, wxEXPAND|wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_edit_btn->Bind(wxEVT_BUTTON, &ListSubjectsPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &ListSubjectsPane::OnEditButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListSubjectsPane::OnDeleteButtonClicked, this);
	m_subjects_list->GetList()->Bind(wxEVT_LISTBOX, &ListSubjectsPane::OnSelectionChanged, this);
}

ListSubjectsPane::~ListSubjectsPane(){

}

void ListSubjectsPane::OnEditButtonClicked(wxCommandEvent & ev){
	if(m_cancel_btn->IsShown()){
		m_name_text->Disable();
		m_cancel_btn->Hide();
		m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	} else {
		m_name_text->Enable();
		m_cancel_btn->Show();
		m_edit_btn->SetLabel(m_owner->m_lang->str_save);
	}
}

void ListSubjectsPane::OnCancelButtonClicked(wxCommandEvent & ev){
	m_name_text->Enable();
	m_cancel_btn->Show();
	m_edit_btn->SetLabel(m_owner->m_lang->str_save);
}

void ListSubjectsPane::OnDeleteButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	int del_i;
	bool success;
	int i_select = m_subjects_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int subject_id = ((IntClientData*) m_subjects_list->GetList()->GetClientObject(i_select))->m_value;
		success = remove_subject(stdout, m_owner->m_database, subject_id);
		if(success){
			school_subject_remove(school, get_subject_index_by_id(school, subject_id));
			m_subjects_list->RemoveItem(subject_id);
			m_owner->NotifyNewUnsavedData();
		} else {
			printf("Não foi possível.\n");
		}
	}
}

void ListSubjectsPane::OnSelectionChanged(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	Subject * subject = nullptr;
	int i_select = m_subjects_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int subject_id = ((IntClientData*) m_subjects_list->GetList()->GetClientObject(i_select))->m_value;
		subject = find_subject_by_id(school, subject_id);
		// subject = &(m_owner->m_school->subjects[m_subjects_list->GetSelection()]);
		m_name_text->SetValue(wxString::FromUTF8(subject->name));
	} else {
		m_name_text->SetLabel(wxT(""));
	}
}
