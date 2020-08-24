#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

ListClassGroupsPane::ListClassGroupsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxArrayString group_names;
	for(i = 0; i < school->n_classes; ++i){
		if(school->classes[i].subordinates != NULL){
			group_names.push_back(wxString::FromUTF8(school->classes[i].name));
		}
	}

	m_groups_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(230,300), group_names);
	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_name);
	wxStaticText * members_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_members);
	wxStaticText * periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_availibility);

	m_name_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_members_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_periods_grid = new ChoiceGrid(m_owner,this, wxID_ANY, wxDefaultPosition, wxSize(400,300));
	wxButton * edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit, wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(200,30));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,30));

	m_periods_grid->AddState(m_owner->m_lang->str_group_available, wxColor(200,200,255));
	m_periods_grid->AddState(m_owner->m_lang->str_group_unavailable, wxColor(255,200,200));

	m_periods_grid->m_basic_col_name = m_owner->m_lang->str_day;
	m_periods_grid->m_basic_row_name = m_owner->m_lang->str_period;

	m_periods_grid->GridRemake(school->n_days,school->n_periods_per_day);

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * fields_sz = new wxFlexGridSizer(2,10,10);
	wxSizer * butn_sz = new wxBoxSizer(wxHORIZONTAL);

	butn_sz->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	butn_sz->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	fields_sz->Add(name_label);
	fields_sz->Add(m_name_text);
	fields_sz->Add(members_label);
	fields_sz->Add(m_members_text);
	desc_sz->Add(fields_sz, 0, wxBOTTOM, 5);
	desc_sz->Add(periods_label, 0, wxBOTTOM, 5);
	desc_sz->Add(m_periods_grid, 0, wxBOTTOM, 5);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(butn_sz, 0, wxBOTTOM, 5);
	desc_sz->Add(m_err_msg, 0, 15);

	sizer->Add(m_groups_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(desc_sz, 1, wxEXPAND|wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_groups_list->Bind(wxEVT_LISTBOX, &ListClassGroupsPane::OnSelectionChanged, this);
	edit_btn->Bind(wxEVT_BUTTON, &ListClassGroupsPane::OnEditButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListClassGroupsPane::OnDeleteButtonClicked, this);
}

void ListClassGroupsPane::OnSelectionChanged(wxCommandEvent & evt){
	int i_select, i_class, i_group = 0, i;
	School * school = m_owner->m_school;
	i_select = m_groups_list->GetSelection();
	if(i_select != wxNOT_FOUND){
		for(i_class = 0; i_class < school->n_classes && i_group <= i_select; ++i_class){
			if(school->classes[i_class].subordinates != NULL){
				++i_group;
			}
		}
		if(i_group == i_select +1){
			--i_class;
			m_name_text->SetLabel(wxString::FromUTF8(school->classes[i_class].name));

			for(i = 0; i < school->n_classes; ++i){
				if(school->classes[i_class].subordinates[i] > 0){
					if(i == 0){
						m_members_text->SetLabel(wxString::FromUTF8(school->classes[i].name));
					} else {
						m_members_text->SetLabel(m_members_text->GetLabel() + "\n" + wxString::FromUTF8(school->classes[i].name));

					}
				}
			}
		}
	}
}

void ListClassGroupsPane::OnEditButtonClicked(wxCommandEvent & evt){

}

void ListClassGroupsPane::OnDeleteButtonClicked(wxCommandEvent & evt){
	int i_class, i_group = 0, i_select = m_groups_list->GetSelection();
	School * school = m_owner->m_school;
	if(i_select != wxNOT_FOUND){
		for(i_class = 0; i_class < school->n_classes && i_group <= i_select; ++i_class){
			// TODO
		}
		// TODO
		m_owner->NotifyNewUnsavedData();
	}
}


ListClassGroupsPane::~ListClassGroupsPane(){

}
