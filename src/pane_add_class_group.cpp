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
	wxButton * add_group = new wxButton(this, wxID_ANY, m_owner->m_lang->str_add_group, wxDefaultPosition, wxSize(180,-1));
	m_subjects_grid = new wxGrid(this, wxID_ANY);
	m_classes_grid = new ChoiceGrid(m_owner, this, wxID_ANY);

	PosIntGridTable * grid_table = new PosIntGridTable(school->n_subjects,1);

	wxString col_name = wxT("Quantidade");
	grid_table->SetColLabelValue(0,col_name);
	for(i = 0; i < school->n_subjects; ++i){
		wxString row_name = wxString::FromUTF8(school->subjects[i].name);
		grid_table->SetRowLabelValue(i, row_name);
	}
	m_subjects_grid->SetTable(grid_table, true);

	m_classes_grid->SetColName(0, "WHATNAME");
	for(i = 0; i < school->n_classes; ++i){
		m_classes_grid->SetRowName(i, wxString::FromUTF8(school->classes[i].name));
	}
	m_classes_grid->AddState(wxT("Pertence"), wxColor(200,200,255));
	m_classes_grid->AddState(wxT("Não Pertence"), wxColor(255,200,200));
	m_classes_grid->GridRemake(1, school->n_classes);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(name_label, 0, wxLEFT | wxTOP,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(classes_label, 0, wxLEFT, 15);
	sizer->Add(m_classes_grid, 0, wxLEFT, 15);
	sizer->Add(subjects_label, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_subjects_grid, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_group, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);
	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	add_group->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnAddGroupButtonClicked, this);
}

void AddClassGroupPane::OnAddGroupButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	int i, i_subject, n_members = 0, j;
	int n_total_periods = 0, n_subjects = 0;

	for(i_subject = 0; i_subject < school->n_subjects; ++i_subject){
		long this_class_n_per = 0;
		bool is_number = m_subjects_grid->GetCellValue(i_subject,0).ToLong(&this_class_n_per);
		if(is_number && this_class_n_per > 0){
			++n_subjects;
			n_total_periods += (int)this_class_n_per;
		}
	}
	for(int i_class = 0; i_class < school->n_classes; ++i_class){
		if(m_classes_grid->GetCellState(0,i_class) == 0){
			++n_members;
		}
	}
	printf("In front of the if. %d %d %d \n ", !m_name_text->GetValue().IsEmpty(), n_members, n_subjects);
	if(!m_name_text->GetValue().IsEmpty() && n_members > 0 && n_subjects > 0){
		Class c;
		c.name = copy_wx_string(m_name_text->GetValue());
		c.short_name = copy_wx_string(m_name_text->GetValue());
		c.size = 0;
		c.maximal_entry_period = school->n_periods_per_day -1;
		c.can_have_free_periods_flag = true;
		c.minimal_exit_period = 0;
		c.active = true;
		c.assignments = (Assignment**)calloc(n_subjects + 1, sizeof(Assignment*));
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
		Assignment * alist = (Assignment *) calloc(n_subjects, sizeof(Assignment));
		int i_assignment = 0;
		for(int i_subject = 0; i_subject < school->n_subjects; ++i_subject){
			long data;
			bool is_number = m_subjects_grid->GetCellValue(0, i_subject).ToLong(&data);
			if(is_number && data > 0){
				alist[i_assignment].subject = &(school->subjects[i_subject]);
				alist[i_assignment].amount = (int)data;
				alist[i_assignment].m_class = &c;
				alist[i_assignment].max_per_day = -1;
				alist[i_assignment].possible_teachers = NULL;
				c.assignments[i_assignment] = &alist[i_assignment];
				++i_assignment;
			}
		}
		printf("trying to insert\n");
		/* TODO  populate * rooms; */
		bool success = insert_class(stdout, m_owner->m_database, &c, school);
		if(success){
			school->classes[school->n_classes] = c;
			m_classes_grid->InsertRows(school->n_classes+1);
			m_classes_grid->SetRowName(school->n_classes, wxString::FromUTF8(c.name));
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


void AddClassGroupPane::ClearInsertedData(){
	m_name_text->Clear();
	for(int i = 0; i < m_owner->m_school->n_classes; ++i){
		m_classes_grid->SetCellState(0,i,0);
	}
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		m_subjects_grid->SetCellValue(i,0,wxT("0"));
	}
}

AddClassGroupPane::~AddClassGroupPane(){

}
