#include "gui.hpp"

extern "C" {
	#include "util.h"
	#include "loader.h"
};

ListSubjectsPane::ListSubjectsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(250,250,250));

	m_subjects_list = new SearchableListPane(m_owner, this);
	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_cancel_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_cancel);
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove);

	m_name_text->Disable();
	m_cancel_btn->Hide();

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * fields_sz= new wxGridSizer(4,5,5);
	wxSizer * fields_wrap= new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_basic_data);
	fields_sz->Add(name_label, 0, wxALIGN_BOTTOM);
	fields_sz->Add(m_name_text, 1, wxEXPAND);
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->Add(m_cancel_btn, 1, wxEXPAND);
	fields_sz->Add(m_edit_btn, 1, wxEXPAND);
	fields_wrap->Add(fields_sz, 1, wxALL | wxEXPAND, 5);
	desc_sz->Add(fields_wrap, 0, wxEXPAND | wxBOTTOM , 5);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(delete_btn, 0,  wxEXPAND);

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
	Bind(DATA_CHANGE_EVENT, &ListSubjectsPane::OnDataChange, this);

	ShowData();
}

void ListSubjectsPane::ShowData(){
	int i;
	School * school = m_owner->m_school;
	m_subjects_list->Clear();
	for(i = 0; i < school->n_subjects; ++i){
		m_subjects_list->AddItem(school->subjects[i].id, wxString::FromUTF8(school->subjects[i].name));
	}
	m_name_text->SetValue(wxT(""));
	m_name_text->Disable();
	m_cancel_btn->Hide();
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	m_edit_btn->Show();
}

void ListSubjectsPane::OnDataChange(wxNotifyEvent & evt) {
	ShowData();
}

ListSubjectsPane::~ListSubjectsPane(){

}

void ListSubjectsPane::OnEditButtonClicked(wxCommandEvent & ev){
	int i_select = m_subjects_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		if(m_cancel_btn->IsShown()){
			int subj_id = ((IntClientData*)m_subjects_list->GetList()->GetClientObject(i_select))->m_value;

			Subject update = (Subject){
				.id = subj_id,
				.name = copy_wx_string(m_name_text->GetValue()),
				/* They need to be alloc'd separately */
				.short_name = copy_wx_string(m_name_text->GetValue()),
			};
			SubjectBasicDataUpdateAction * act = new SubjectBasicDataUpdateAction(m_owner, update);
			if(m_owner->Do(act)){
				m_name_text->Disable();
				m_cancel_btn->Hide();
				m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
				m_subjects_list->GetList()->SetString(i_select, m_name_text->GetValue());
			} else {
				printf("No Success");
			}
		} else {
			m_name_text->Enable();
			m_cancel_btn->Show();
			m_edit_btn->SetLabel(m_owner->m_lang->str_save);
		}
	}
}

void ListSubjectsPane::OnCancelButtonClicked(wxCommandEvent & ev){
	m_name_text->Enable();
	m_cancel_btn->Show();
	m_edit_btn->SetLabel(m_owner->m_lang->str_save);
}

void ListSubjectsPane::OnDeleteButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	int i_select = m_subjects_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int subject_id = ((IntClientData*) m_subjects_list->GetList()->GetClientObject(i_select))->m_value;
		if(can_remove_subject(school, subject_id)){
			SubjectDeleteAction * act = new SubjectDeleteAction(m_owner, subject_id);
			bool success = m_owner->Do(act);
			if(success){
				/* TODO: substitute for Action */
				m_subjects_list->RemoveItem(subject_id);
				m_owner->NotifyNewUnsavedData();
				m_name_text->SetValue("");
				m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
				m_cancel_btn->Hide();
			} else {
				printf("Não foi possível.\n");
			}
		} else {
			wxMessageDialog * dialog = new wxMessageDialog(nullptr, wxT("(TODO lang) Error! Could not remove, because it is in a timetable already"), m_owner->m_lang->str_error, wxOK);
			dialog->ShowModal();
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
