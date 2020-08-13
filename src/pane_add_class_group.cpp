#include "gui.hpp"

#include <wx/spinctrl.h>
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
	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_subjects);
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));

	name_label->SetFont(*m_owner->m_small_font);
	classes_label->SetFont(*m_owner->m_small_font);
	subjects_label->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1));
	m_all_classes_list = new wxChoice(this, wxID_ANY,  wxDefaultPosition, wxSize(310,30));
	wxButton * add_class = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_class, wxDefaultPosition, wxSize(180,-1));
	m_selected_classes_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	wxButton * remove_class_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(180,-1));
	wxButton * remove_all_classes = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove_all, wxDefaultPosition, wxSize(180,-1));
	m_all_subjects_list = new wxChoice(this, wxID_ANY,  wxDefaultPosition, wxSize(180,30));
	m_nper_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(120,30));
	wxButton * add_subject = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_subject, wxDefaultPosition, wxSize(180,-1));
	m_selected_subjects_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	wxButton * remove_subject_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(180,-1));
	wxButton * remove_all_subjects = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove_all, wxDefaultPosition, wxSize(180,-1));
	wxButton * add_group = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_group, wxDefaultPosition, wxSize(180,-1));

	for(i = 0; i < school->n_classes; ++i){
		m_all_classes_list->Insert(wxString::FromUTF8(school->classes[i].name), i, new IntClientData(i));
	}
	for(i = 0; i < school->n_subjects; ++i){
		m_all_subjects_list->Insert(wxString::FromUTF8(school->subjects[i].name), i, new IntClientData(i));
	}

	wxSizer * add_class_sizer = new wxBoxSizer(wxHORIZONTAL);
	add_class_sizer->Add(m_all_classes_list,0,wxRIGHT,10);
	add_class_sizer->Add(add_class,0,wxRIGHT,10);

	wxSizer * add_subject_sizer = new wxBoxSizer(wxHORIZONTAL);
	add_subject_sizer->Add(m_all_subjects_list,0,wxRIGHT,10);
	add_subject_sizer->Add(m_nper_text,0,wxRIGHT,10);
	add_subject_sizer->Add(add_subject,0,wxRIGHT,10);

	wxSizer * class_buttons_sizer = new wxBoxSizer(wxVERTICAL);
	class_buttons_sizer->Add(remove_class_button, 0, wxBOTTOM, 10);
	class_buttons_sizer->Add(remove_all_classes, 0, wxBOTTOM, 10);

	wxSizer * subject_buttons_sizer = new wxBoxSizer(wxVERTICAL);
	subject_buttons_sizer->Add(remove_subject_button, 0, wxBOTTOM, 10);
	subject_buttons_sizer->Add(remove_all_subjects, 0, wxBOTTOM, 10);

	wxSizer * classes_sizer = new wxBoxSizer(wxHORIZONTAL);
	classes_sizer->Add(m_selected_classes_list, 0, wxRIGHT, 10);
	classes_sizer->Add(class_buttons_sizer, 0, 0);

	wxSizer * subjects_sizer = new wxBoxSizer(wxHORIZONTAL);
	subjects_sizer->Add(m_selected_subjects_list, 0, wxRIGHT, 10);
	subjects_sizer->Add(subject_buttons_sizer, 0, 0);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0, wxLEFT ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(classes_label, 0, wxLEFT, 15);
	sizer->Add(add_class_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(classes_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_label, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_subject_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_group, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);
	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	add_group->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnAddGroupButtonClicked, this);
	add_class->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnAddClassButtonClicked, this);
	add_subject->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnAddSubjectButtonClicked, this);
	remove_all_classes->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnRemoveAllClassesButtonClicked, this);
	remove_all_subjects->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnRemoveAllSubjectsButtonClicked, this);
	remove_subject_button->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnRemoveSubjectButtonClicked, this);
	remove_class_button->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnRemoveClassButtonClicked, this);
}

void AddClassGroupPane::OnAddGroupButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	int i, i_member = 0, j, n_subordinates = m_selected_classes_list->GetCount();
	int n_needs  = m_selected_subjects_list->GetCount();
	if(!m_name_text->GetValue().IsEmpty() && n_subordinates > 0 && n_needs > 0){
		Class c;
		c.name = copy_wx_string(m_name_text->GetValue());
		c.short_name = copy_wx_string(m_name_text->GetValue());
		c.size = 0;
		c.maximal_entry_period = school->n_periods_per_day -1;
		c.minimal_exit_period = 0;
		c.active = true;
		c.assignments = (Assignment**)calloc(n_needs + 1, sizeof(Assignment*));
		c.room_scores = NULL; /* TODO */
		c.subordinates = (int *) calloc(n_subordinates+1, sizeof(int));
		c.period_scores = (int *) calloc(school->n_periods + 1, sizeof(int));
		c.period_scores[school->n_periods] = -1;
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
				if(school->classes[i_class].period_scores[j] == 0 ){
					c.period_scores[j] = 0;
					break;
				} else {
					c.period_scores[j] += school->classes[i_class].period_scores[j];
				}
			}
			c.subordinates[i_class] = 1;
			++i_member;
		}
		for(i = 0; i < school->n_periods; ++i){
			if(c.period_scores[i] > 0){
				/* Garanteed above zero since each is >1 */
				c.period_scores[i] = c.period_scores[i]/m_selected_classes_list->GetCount();
			}
		}
		Assignment * alist = (Assignment *) calloc(n_needs, sizeof(Assignment));
		for(i = 0; i < n_needs; ++i){
			IntPairClientData * data = (IntPairClientData*)m_selected_subjects_list->GetClientObject(i);
			alist[i].subject = &(school->subjects[data->m_v1]);
			alist[i].amount = data->m_v2;
			c.assignments[i] = &alist[i];
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
		free(alist);
	}
}

void AddClassGroupPane::OnAddClassButtonClicked(wxCommandEvent & ev){
	int i_select = m_all_classes_list->GetSelection();
	if(i_select != wxNOT_FOUND){
		int i_class = ((IntClientData*)m_all_classes_list->GetClientObject(i_select))->m_value;
		wxString name = wxString::FromUTF8(m_owner->m_school->classes[i_class].name);
		m_selected_classes_list->Insert(name, m_selected_classes_list->GetCount(), new IntClientData(i_class));
		m_all_classes_list->Delete(i_select);
	}
}

void AddClassGroupPane::OnAddSubjectButtonClicked(wxCommandEvent & ev){
	int i_select = m_all_subjects_list->GetSelection();
	int nper = m_nper_text->GetValue();
	if(i_select != wxNOT_FOUND && nper > 0){
		int i_subj = ((IntClientData*)m_all_subjects_list->GetClientObject(i_select))->m_value;
		wxString name = wxString::FromUTF8(m_owner->m_school->subjects[i_subj].name);
		m_selected_subjects_list->Insert(name + wxString::Format(": %d", nper), m_selected_subjects_list->GetCount(), new IntPairClientData(i_subj, nper));
		m_all_subjects_list->Delete(i_select);
	}
}

void AddClassGroupPane::OnRemoveClassButtonClicked(wxCommandEvent & ev){
	int i_select = m_selected_classes_list->GetSelection();
	if(i_select != wxNOT_FOUND){
		int i_class = ((IntClientData*)m_selected_classes_list->GetClientObject(i_select))->m_value;
		wxString name = wxString::FromUTF8(m_owner->m_school->classes[i_class].name);
		m_all_classes_list->Insert(name, m_all_classes_list->GetCount(), new IntClientData(i_class));
		m_selected_classes_list->Delete(i_select);
	}
}

void AddClassGroupPane::OnRemoveSubjectButtonClicked(wxCommandEvent & ev){
	int i_select = m_selected_subjects_list->GetSelection();
	if(i_select != wxNOT_FOUND){
		int i_subj = ((IntPairClientData*)m_selected_subjects_list->GetClientObject(i_select))->m_v1;
		wxString name = wxString::FromUTF8(m_owner->m_school->subjects[i_subj].name);
		m_all_subjects_list->Insert(name, m_all_subjects_list->GetCount(), new IntClientData(i_subj));
		m_selected_subjects_list->Delete(i_select);
	}
}

void AddClassGroupPane::OnRemoveAllSubjectsButtonClicked(wxCommandEvent & ev){
	m_selected_subjects_list->Clear();
	m_all_subjects_list->Clear();
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_all_subjects_list->Insert(wxString::FromUTF8(m_owner->m_school->subjects[i].name), i, new IntClientData(i));
	}
}

void AddClassGroupPane::OnRemoveAllClassesButtonClicked(wxCommandEvent & ev){
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
	m_selected_subjects_list->Clear();
	m_all_subjects_list->Clear();
	for(int i = 0; i < m_owner->m_school->n_classes; ++i){
		m_all_classes_list->Insert(wxString::FromUTF8(m_owner->m_school->classes[i].name), i, new IntClientData(i));
	}
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_all_subjects_list->Insert(wxString::FromUTF8(m_owner->m_school->subjects[i].name), i, new IntClientData(i));
	}
}

AddClassGroupPane::~AddClassGroupPane(){

}
