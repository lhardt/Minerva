#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

AddClassGroupPane::AddClassGroupPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	School * school = m_owner->m_school;
	int i;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_class_group);
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_name);
	wxStaticText * classes_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_classes_to_the_group);
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));

	name_label->SetFont(*m_owner->m_small_font);
	classes_label->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1));
	m_all_classes_list = new wxChoice(this, wxID_ANY,  wxDefaultPosition, wxSize(310,30));
	wxButton * add_class = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_class, wxDefaultPosition, wxSize(180,-1));
	m_selected_classes_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	wxButton * remove_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(180,-1));
	wxButton * remove_all = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove_all, wxDefaultPosition, wxSize(180,-1));
	wxButton * add_group = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_group, wxDefaultPosition, wxSize(180,-1));

	for(i = 0; i < school->n_classes; ++i){
		m_all_classes_list->Insert(wxString::FromUTF8(school->classes[i].name), i, new IntClientData(i));
	}

	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	add_sizer->Add(m_all_classes_list,0,wxRIGHT,10);
	add_sizer->Add(add_class,0,wxRIGHT,10);

	wxSizer * buttons_sizer = new wxBoxSizer(wxVERTICAL);
	buttons_sizer->Add(remove_button, 0, wxBOTTOM, 10);
	buttons_sizer->Add(remove_all, 0, wxBOTTOM, 10);

	wxSizer * classes_sizer = new wxBoxSizer(wxHORIZONTAL);
	classes_sizer->Add(m_selected_classes_list, 0, wxRIGHT, 10);
	classes_sizer->Add(buttons_sizer, 0, 0);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0, wxLEFT ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(classes_label, 0, wxLEFT, 15);
	sizer->Add(add_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(classes_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_group, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);
	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	add_group->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnAddGroupButtonClicked, this);
	add_class->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnAddClassButtonClicked, this);
	remove_all->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnRemoveAllButtonClicked, this);
}

void AddClassGroupPane::OnAddGroupButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	int i, i_member = 0, j, n_subordinates = m_selected_classes_list->GetCount();
	if(!m_name_text->GetValue().IsEmpty() && n_subordinates > 0){
		Class c;
		c.name = copy_wx_string(m_name_text->GetValue());
		c.short_name = copy_wx_string(m_name_text->GetValue());
		c.size = 0;
		c.maximal_entry_period = school->n_periods_per_day -1;
		c.minimal_exit_period = 0;
		c.abstract = false;
		c.needs = NULL;
		c.rooms = NULL; /* TODO */
		c.subordinates = (int *) calloc(n_subordinates+1, sizeof(int));
		for(i = 0; i < m_selected_classes_list->GetCount(); ++i){
			int i_class = ((IntClientData*)m_selected_classes_list->GetClientObject(i))->m_value;

			c.size += school->classes[i_class].size;

			if(c.maximal_entry_period > school->classes[i_class].maximal_entry_period){
				c.maximal_entry_period = school->classes[i_class].maximal_entry_period;
			}
			if(c.minimal_exit_period < school->classes[i_class].minimal_exit_period){
				c.minimal_exit_period = school->classes[i_class].minimal_exit_period;
			}
			if(c.can_have_free_periods_flag && !school->classes[i_class].can_have_free_periods_flag){
				c.can_have_free_periods_flag = false;
			}

			for(j = 0; j < school->n_subject_groups; ++j){
				if(i_member == 0 ||  c.max_per_day_subject_group[j] > school->classes[i_class].max_per_day_subject_group[j]){
					c.max_per_day_subject_group[j] = school->classes[i_class].max_per_day_subject_group[j];
				}
			}
			for(j = 0; j < school->n_periods; ++j){
				if(school->classes[i_class].periods[j] == 0 ){
					c.periods[j] = 0;
					break;
				} else {
					c.periods[j] += school->classes[i_class].periods[j];
				}
			}
			c.subordinates[i_class] = 1;
			++i_member;
		}
		for(i = 0; i < school->n_periods; ++i){
			if(c.periods[i] > 0){
				/* Garanteed above zero since each is >1 */
				c.periods[i] = c.periods[i]/m_selected_classes_list->GetCount();
			}
		}
		/* TODO  populate * rooms; */
		bool success = insert_class(stdout, m_owner->m_database, &c, school);
		if(success){
			school->classes[i] = c;
			++school->n_classes;

			ClearInsertedData();
			m_err_msg->SetLabel(m_owner->m_lang->str_success);
			m_owner->NotifyNewUnsavedData();
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	}
}

void AddClassGroupPane::OnAddClassButtonClicked(wxCommandEvent & ev){
	int i_select = m_all_classes_list->GetSelection();
	if(i_select != wxNOT_FOUND){
		wxString name = wxString::FromUTF8(m_owner->m_school->classes[i_select].name);
		m_selected_classes_list->Insert(name, m_selected_classes_list->GetCount(), new IntClientData(i_select));
		m_all_classes_list->Delete(i_select);
	}
}

void AddClassGroupPane::OnRemoveAllButtonClicked(wxCommandEvent & ev){
	m_selected_classes_list->Clear();
	m_all_classes_list->Clear();
	for(int i = 0; i < m_owner->m_school->n_classes; ++i){
		m_all_classes_list->Insert(wxString::FromUTF8(m_owner->m_school->classes[i].name), i, new IntClientData(i));
	}
}

void AddClassGroupPane::ClearInsertedData(){
	m_name_text->Clear();
	m_all_classes_list->Clear();
	m_selected_classes_list->Clear();
	for(int i = 0; i < m_owner->m_school->n_classes; ++i){
		m_all_classes_list->Insert(wxString::FromUTF8(m_owner->m_school->classes[i].name), i, new IntClientData(i));
	}
}

AddClassGroupPane::~AddClassGroupPane(){

}
