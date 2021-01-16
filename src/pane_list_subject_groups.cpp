#include "gui.hpp"

extern "C" {
	#include "util.h"
	#include "loader.h"
};

ListSubjectGroupsPane::ListSubjectGroupsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(250,250,250));

	m_groups_list = new SearchableListPane(m_owner, this, wxID_ANY, wxDefaultPosition, wxSize(230,300));
	wxNotebook * notebook = new wxNotebook(this, wxID_ANY);
	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_members = new ScoreGridPane(m_owner, notebook, wxID_ANY);

	notebook->AddPage(m_members, m_owner->m_lang->str_members);

	ChoiceGrid * members_grid = m_members->GetGrid();
	members_grid->AddState(m_owner->m_lang->str_no, wxColor(255,200,200));
	members_grid->AddState(m_owner->m_lang->str_yes, wxColor(200,200,255));

	wxString col_name = m_owner->m_lang->str_member;
	members_grid->SetColLabel(0,col_name);

	wxSizer * fields_wrap = new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_basic_data);
	wxSizer * fields_sz = new wxGridSizer(4,5,5);
	fields_sz->Add(name_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
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
	m_members->GetSaveButton()->Bind(wxEVT_BUTTON, &ListSubjectGroupsPane::OnMembersSaveButtonClicked, this);
	m_members->GetCancelButton()->Bind(wxEVT_BUTTON, &ListSubjectGroupsPane::OnMembersCancelButtonClicked, this);

	Bind(DATA_CHANGE_EVENT, &ListSubjectGroupsPane::OnDataChange, this);

	ShowData();
}

void ListSubjectGroupsPane::OnMembersCancelButtonClicked(wxCommandEvent &){
	School * school = m_owner->m_school;
	ChoiceGrid * members_grid = m_members->GetGrid();
	int i_select = m_groups_list->GetList()->GetSelection();
	int group_id = ((IntClientData*)m_groups_list->GetList()->GetClientObject(i_select))->m_value;
	int group_i = get_subject_group_index_by_id(m_owner->m_school, group_id);
	for(int i = 0; i < school->n_subjects; ++i){
		if(school->subjects[i].in_groups){
			members_grid->SetCellState(i, 0, school->subjects[i].in_groups[group_i] > 0 ? 1:0);
		} else {
			members_grid->SetCellState(i, 0, 0);
		}
	}
}

void ListSubjectGroupsPane::OnMembersSaveButtonClicked(wxCommandEvent &){
	School * school = m_owner->m_school;
	ChoiceGrid * members_grid = m_members->GetGrid();
	int * members = (int *) calloc(school->n_subjects+1, sizeof(int));
	members[school->n_subjects] = -1;

	for(int i = 0; i < school->n_subjects; ++i){
		members[i] = members_grid->GetCellState(i,0);
	}
	int i_select = m_groups_list->GetList()->GetSelection();
	int group_id = ((IntClientData*)m_groups_list->GetList()->GetClientObject(i_select))->m_value;

	SubjectGroupMembersUpdateAction * act = new SubjectGroupMembersUpdateAction(m_owner, group_id, members);
	if(m_owner->Do(act)){
		ShowData();
	} else {
		printf("Did not do.\n");
	}
}

void ListSubjectGroupsPane::ShowData(){
	School * school = m_owner->m_school;

	m_cancel_btn->Hide();
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	m_name_text->Enable(false);
	m_name_text->SetValue("");
	m_members->SetEditing(false);

	m_groups_list->Clear();
	for(int i = 0; i < school->n_subject_groups; ++i){
		m_groups_list->AddItem(school->subject_group_ids[i], wxString::FromUTF8(school->subject_group_names[i]));
	}

	ChoiceGrid * members_grid = m_members->GetGrid();
	for(int i = 0; i < school->n_subjects; ++i){
		wxString row_name = wxString::FromUTF8(school->subjects[i].name);
		members_grid->SetRowLabel(i, row_name);
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
		int i_select = m_groups_list->GetList()->GetSelection();
		int group_id = ((IntClientData*)m_groups_list->GetList()->GetClientObject(i_select))->m_value;
		int group_i = get_subject_group_index_by_id(m_owner->m_school, group_id);
		char * sgr_name = copy_wx_string(m_name_text->GetValue());
		SubjectGroupNameUpdateAction * act = new SubjectGroupNameUpdateAction(m_owner, group_i, sgr_name);
		bool success = m_owner->Do(act);
		if(success){
			m_cancel_btn->Hide();
			m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
			m_name_text->Enable(false);
			m_groups_list->EditItem(group_id, m_name_text->GetValue());
		}
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
		SubjectGroupDeleteAction * act = new SubjectGroupDeleteAction(m_owner, get_subject_group_index_by_id(m_owner->m_school, group_id));
		bool success = m_owner->Do(act);
		if(success){
			m_groups_list->RemoveItem(group_id);
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
		m_name_text->SetValue(wxString::FromUTF8(school->subject_group_names[group_i]));
		ChoiceGrid * members_grid = m_members->GetGrid();
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
