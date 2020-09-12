#include "gui.hpp"

extern "C" {
	#include "util.h"
	#include "loader.h"
};

ListSubjectGroupsPane::ListSubjectGroupsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	m_groups_list = new SearchableListPane(m_owner, this, wxID_ANY, wxDefaultPosition, wxSize(230,300));
	wxNotebook * notebook = new wxNotebook(this, wxID_ANY);
	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_members = new ScoreGridPane(m_owner, notebook, wxID_ANY);

	for(i = 0; i < school->n_subject_groups; ++i){
		m_groups_list->AddItem(school->subject_group_ids[i], wxString::FromUTF8(school->subject_group_names[i]));
	}

	notebook->AddPage(m_members, m_owner->m_lang->str_member);

	ChoiceGrid * members_grid = m_members->GetGrid();
	members_grid->AddState(m_owner->m_lang->str_no, wxColor(255,200,200));
	members_grid->AddState(m_owner->m_lang->str_yes, wxColor(200,200,255));

	wxString col_name = m_owner->m_lang->str_member;
	members_grid->SetColName(0,col_name);

	wxSizer * fields_wrap = new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_basic_data);
	wxSizer * fields_sz = new wxGridSizer(4,5,5);
	fields_sz->Add(name_label);
	fields_sz->Add(m_name_text);
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->Add(m_cancel_btn, 0, wxEXPAND);
	fields_sz->Add(m_edit_btn, 0, wxEXPAND);
	fields_wrap->Add(fields_sz, 1, wxEXPAND | wxALL, 5);

	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	desc_sz->Add(fields_wrap, 0, wxEXPAND | wxBOTTOM, 5);
	desc_sz->Add(notebook, 1, wxEXPAND | wxBOTTOM, 5);
	desc_sz->Add(delete_btn, 0, wxEXPAND);

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_groups_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(desc_sz, 1, wxEXPAND|wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();


	m_edit_btn->Bind(wxEVT_BUTTON, &ListSubjectGroupsPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &ListSubjectGroupsPane::OnCancelButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListSubjectGroupsPane::OnDeleteButtonClicked, this);
	m_groups_list->GetList()->Bind(wxEVT_LISTBOX, &ListSubjectGroupsPane::OnSelectionChanged, this);
	Bind(DATA_CHANGE_EVENT, &ListSubjectGroupsPane::OnDataChange, this);

	ShowData();
}

void ListSubjectGroupsPane::ShowData(){
	m_cancel_btn->Hide();
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	m_name_text->Enable(false);
	m_name_text->SetValue("");
	m_members->SetEditing(false);

	ChoiceGrid * members_grid = m_members->GetGrid();
	School * school = m_owner->m_school;
	for(int i = 0; i < school->n_subjects; ++i){
		wxString row_name = wxString::FromUTF8(school->subjects[i].name);
		members_grid->SetRowName(i, row_name);
	}
	members_grid->GridRemake(1, school->n_subjects);
	members_grid->SetAllActiveCellsState(0);
	members_grid->Fit();
	m_members->Fit();

}

void ListSubjectGroupsPane::OnDataChange(wxNotifyEvent & evt) {
	ShowData();
}

void ListSubjectGroupsPane::OnEditButtonClicked(wxCommandEvent & evt){
	if(m_cancel_btn->IsShown()){
		m_cancel_btn->Hide();
		m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
		m_name_text->Enable(false);
	} else {
		m_cancel_btn->Show();
		m_edit_btn->SetLabel(m_owner->m_lang->str_save);
		m_name_text->Enable(true);
	}
}

void ListSubjectGroupsPane::OnCancelButtonClicked(wxCommandEvent & evt){
	m_cancel_btn->Hide();
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	m_name_text->Enable(false);
}

void ListSubjectGroupsPane::OnDeleteButtonClicked(wxCommandEvent & evt){
	int sel = m_groups_list->GetList()->GetSelection();
	if(sel != wxNOT_FOUND){
		int group_id = ((IntClientData*)m_groups_list->GetList()->GetClientObject(sel))->m_value;
		if(remove_subject_group(stdout, m_owner->m_database, m_owner->m_school->subject_group_ids[group_id])){
			m_groups_list->RemoveItem(group_id);
			m_owner->NotifyNewUnsavedData();
		} else {
			printf("Error! Could not delete\n");
		}
	}
}

void ListSubjectGroupsPane::OnSelectionChanged(wxCommandEvent & evt){
	int i = 0, i_select = m_groups_list->GetList()->GetSelection();
	School * school = m_owner->m_school;
	if(i_select != wxNOT_FOUND){
		int group_id = ((IntClientData*)m_groups_list->GetList()->GetClientObject(i_select))->m_value;
		int group_i = get_subject_group_index_by_id(school, group_id);
		ChoiceGrid * members_grid = m_members->GetGrid();
		m_name_text->SetValue(wxString::FromUTF8(school->subject_group_names[group_i]));
		for(i = 0; i < school->n_subjects; ++i){
			if(school->subjects[i].in_groups){
				members_grid->SetCellState(i, 0, school->subjects[i].in_groups[group_i] > 0 ? 1:0);
			} else {
				members_grid->SetCellState(i, 0, 0);
			}
		}
	}
	Layout();
}

ListSubjectGroupsPane::~ListSubjectGroupsPane(){

}
