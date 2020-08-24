#include "gui.hpp"

#include <wx/spinctrl.h>
extern "C" {
	#include "loader.h"
};

AddClassGroupPane::AddClassGroupPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	this->m_owner = owner;
	School * school = m_owner->m_school;
	int i;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_group_name);
	wxStaticText * classes_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_add_classes_to_the_group);
	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_subjects);
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""));

	name_label->SetFont(*m_owner->m_small_font);
	classes_label->SetFont(*m_owner->m_small_font);
	subjects_label->SetFont(*m_owner->m_small_font);

	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1));
	m_all_subjects_list = new wxChoice(this, wxID_ANY,  wxDefaultPosition, wxSize(180,30));
	m_nper_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(120,30));
	wxButton * add_subject = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_subject, wxDefaultPosition, wxSize(180,-1));
	m_selected_subjects_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	wxButton * remove_subject_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(180,-1));
	wxButton * remove_all_subjects = new wxButton(this, wxID_ANY, m_owner->m_lang->str_remove_all, wxDefaultPosition, wxSize(180,-1));
	wxButton * add_group = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_group, wxDefaultPosition, wxSize(180,-1));

	m_classes_grid = new ChoiceGrid(m_owner, this, wxID_ANY);

	m_classes_grid->SetColName(0, "WHATNAME");
	for(i = 0; i < school->n_classes; ++i){
		m_classes_grid->SetRowName(i, wxString::FromUTF8(school->classes[i].name));
	}
	m_classes_grid->AddState(wxT("Pertence"), wxColor(200,200,255));
	m_classes_grid->AddState(wxT("Não Pertence"), wxColor(255,200,200));
	m_classes_grid->GridRemake(1, school->n_classes);

	for(i = 0; i < school->n_subjects; ++i){
		m_all_subjects_list->Insert(wxString::FromUTF8(school->subjects[i].name), i, new IntClientData(i));
	}

	wxSizer * add_subject_sizer = new wxBoxSizer(wxHORIZONTAL);
	add_subject_sizer->Add(m_all_subjects_list,0,wxRIGHT,10);
	add_subject_sizer->Add(m_nper_text,0,wxRIGHT,10);
	add_subject_sizer->Add(add_subject,0,wxRIGHT,10);

	wxSizer * subject_buttons_sizer = new wxBoxSizer(wxVERTICAL);
	subject_buttons_sizer->Add(remove_subject_button, 0, wxBOTTOM, 10);
	subject_buttons_sizer->Add(remove_all_subjects, 0, wxBOTTOM, 10);

	wxSizer * subjects_sizer = new wxBoxSizer(wxHORIZONTAL);
	subjects_sizer->Add(m_selected_subjects_list, 0, wxRIGHT, 10);
	subjects_sizer->Add(subject_buttons_sizer, 0, 0);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(name_label, 0, wxLEFT | wxTOP,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(classes_label, 0, wxLEFT, 15);
	sizer->Add(m_classes_grid, 0, wxLEFT, 15);
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
	add_subject->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnAddSubjectButtonClicked, this);
	remove_all_subjects->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnRemoveAllSubjectsButtonClicked, this);
	remove_subject_button->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnRemoveSubjectButtonClicked, this);
}

void AddClassGroupPane::OnAddGroupButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	int i, n_members = 0, j;
	int n_needs  = m_selected_subjects_list->GetCount();

	for(int i_class = 0; i_class < school->n_classes; ++i_class){
		int retval = m_classes_grid->GetCellState(0,i_class);
		if(retval == 0){
			++n_members;
		}
		printf("i %d Retval: %d \n", i_class, retval);
	}
	printf("going to the if %d && %d && %d \n", !m_name_text->GetValue().IsEmpty(), n_members,n_needs);
	if(!m_name_text->GetValue().IsEmpty() && n_members > 0 && n_needs > 0){
		printf("passed the if\n");
		Class c;
		c.name = copy_wx_string(m_name_text->GetValue());
		c.short_name = copy_wx_string(m_name_text->GetValue());
		c.size = 0;
		c.maximal_entry_period = school->n_periods_per_day -1;
		c.can_have_free_periods_flag = true;
		c.minimal_exit_period = 0;
		c.active = true;
		c.assignments = (Assignment**)calloc(n_needs + 1, sizeof(Assignment*));
		c.room_scores = NULL; /* TODO */
		c.max_per_day_subject_group = NULL;
		c.subordinates = (int *) calloc(n_members+1, sizeof(int));
		c.period_scores = (int *) calloc(school->n_periods + 1, sizeof(int));
		c.period_scores[school->n_periods] = -1;
		for(int i_class = 0; i_class < school->n_classes; ++i_class){
			if(m_classes_grid->GetCellState(0,i_class) == 0){
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
					if(n_members == 0 ||  c.max_per_day_subject_group[j] > school->classes[i_class].max_per_day_subject_group[j]){
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
			}
		}
		for(i = 0; i < school->n_periods; ++i){
			if(c.period_scores[i] > 0){
				/* Garanteed above zero since each is >1 */
				c.period_scores[i] = c.period_scores[i]/n_members;
			}
		}
		Assignment * alist = (Assignment *) calloc(n_needs, sizeof(Assignment));
		for(i = 0; i < n_needs; ++i){
			IntPairClientData * data = (IntPairClientData*)m_selected_subjects_list->GetClientObject(i);
			alist[i].subject = &(school->subjects[data->m_v1]);
			alist[i].amount = data->m_v2;
			alist[i].m_class = &c;
			alist[i].max_per_day = -1;
			alist[i].possible_teachers = NULL;
			c.assignments[i] = &alist[i];
		}
		printf("trying to insert\n");
		/* TODO  populate * rooms; */
		bool success = insert_class(stdout, m_owner->m_database, &c, school);
		if(success){
			school->classes[i] = c;
			++school->n_classes;

			ClearInsertedData();
			m_err_msg->SetLabel(m_owner->m_lang->str_success);
			m_owner->NotifyNewUnsavedData();
		} else {
			printf("could not insert\n");
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
		free(alist);
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

void AddClassGroupPane::ClearInsertedData(){
	m_name_text->Clear();
	m_selected_subjects_list->Clear();
	m_all_subjects_list->Clear();
	for(int i = 0; i < m_owner->m_school->n_classes; ++i){
		m_classes_grid->SetCellState(i,0,0);
	}
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_all_subjects_list->Insert(wxString::FromUTF8(m_owner->m_school->subjects[i].name), i, new IntClientData(i));
	}
}

AddClassGroupPane::~AddClassGroupPane(){

}
