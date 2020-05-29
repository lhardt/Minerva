#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

DescTimetablePane::DescTimetablePane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * labels_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * choice_sz = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Grade Horária Gerada"), wxPoint(30,30), wxSize(200,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxArrayString solution_arr;
	solution_arr.push_back("Nenhuma");
	if(school->solutions != NULL){
		for(i = 0; i < school->n_solutions; ++i){
			solution_arr.push_back(wxString::FromUTF8(school->solutions[i].name));
		}

	}
	m_solution_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30), solution_arr);
	m_solution_choice->SetSelection(0);
	wxStaticText * solution_choice_label = new wxStaticText(this, wxID_ANY, wxT("Escolha Uma Grade Gerada"), wxDefaultPosition, wxSize(170,20));

	wxArrayString class_arr;
	class_arr.push_back("Nenhuma");
	if(school->classes != NULL){
		for(i = 0; i < school->n_classes; ++i){
			class_arr.push_back(wxString::FromUTF8(school->classes[i].name));
		}

	}
	m_class_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30), class_arr);
	m_class_choice->SetSelection(0);
	wxStaticText * class_choice_label = new wxStaticText(this, wxID_ANY, wxT("Escolha Uma Turma"), wxDefaultPosition, wxSize(170,20));

	wxArrayString teacher_arr;
	teacher_arr.push_back("Nenhum");
	if(school->teachers != NULL){
		for(i = 0; i < school->n_teachers; ++i){
			teacher_arr.push_back(wxString::FromUTF8(school->teachers[i].name));
		}

	}
	m_teacher_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30), teacher_arr);
	m_teacher_choice->SetSelection(0);
	wxStaticText * teacher_choice_label = new wxStaticText(this, wxID_ANY, wxT("Ou um Professor"), wxDefaultPosition, wxSize(170,20));

	wxArrayString subj_arr;
	subj_arr.push_back("Nenhum");
	if(school->subjects != NULL){
		for(i = 0; i < school->n_subjects; ++i){
			subj_arr.push_back(wxString::FromUTF8(school->subjects[i].name));
		}

	}
	m_subject_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30), subj_arr);
	m_subject_choice->SetSelection(0);
	wxStaticText * subject_choice_label = new wxStaticText(this, wxID_ANY, wxT("(opc.) uma Disciplina"), wxDefaultPosition, wxSize(170,20));

	m_grid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	m_grid->CreateGrid(1+school->n_periods_per_day, 1+school->n_days);
	m_grid->HideColLabels();
	m_grid->HideRowLabels();

	for(i = 0; i < school->n_days; ++i){
		m_grid->SetCellValue(0,1 + i, wxString::FromUTF8(school->day_names[i]));
	}

	for(i = 0; i < school->n_periods_per_day; ++i){
		m_grid->SetCellValue(1 + i,0, wxString::FromUTF8(school->daily_period_names[i]));
	}

	m_solution_choice->Bind(wxEVT_CHOICE, &DescTimetablePane::OnRedrawGridRequest, this);
	m_class_choice->Bind(wxEVT_CHOICE, &DescTimetablePane::OnRedrawGridRequest, this);
	m_teacher_choice->Bind(wxEVT_CHOICE, &DescTimetablePane::OnRedrawGridRequest, this);
	m_subject_choice->Bind(wxEVT_CHOICE, &DescTimetablePane::OnRedrawGridRequest, this);

	labels_sz->Add(solution_choice_label, 1, wxLEFT|wxRIGHT, 5);
	labels_sz->Add(class_choice_label, 1, wxLEFT|wxRIGHT, 5);
	labels_sz->Add(teacher_choice_label, 1,wxLEFT|wxRIGHT, 5);
	labels_sz->Add(subject_choice_label, 1, wxLEFT|wxRIGHT, 5);

	choice_sz->Add(m_solution_choice, 1, wxALL, 5);
	choice_sz->Add(m_class_choice, 1, wxALL, 5);
	choice_sz->Add(m_teacher_choice, 1, wxALL, 5);
	choice_sz->Add(m_subject_choice, 1, wxALL, 5);

	sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 15);
	sizer->Add(labels_sz, 0, 15);
	sizer->Add(choice_sz, 0, 15);
	sizer->Add(m_grid, 1, wxEXPAND | wxALL, 15);

	this->SetSizerAndFit(sizer);
}

void DescTimetablePane::OnRedrawGridRequest(wxCommandEvent & evt){
	int i, j;
	School * school = m_owner->m_school;
	Meeting * chosen_solution = (m_solution_choice->GetSelection() == 0)?(NULL):(school->solutions[ m_solution_choice->GetSelection() -1].meetings);
	Class   * chosen_class    = (m_class_choice->GetSelection() == 0)?(NULL):&(school->classes[ m_class_choice->GetSelection() -1]);
	Teacher * chosen_teacher  = (m_teacher_choice->GetSelection() == 0)?(NULL):&(school->teachers[ m_teacher_choice->GetSelection() -1]);
	Subject * chosen_subject  = (m_subject_choice->GetSelection() == 0)?(NULL):&(school->subjects[ m_subject_choice->GetSelection() -1]);

	for(i = 0; i < school->n_periods_per_day; ++i){
		for(j = 0;j < school->n_days; ++j){
			m_grid->SetCellValue(1 + i, 1 + j,wxT(""));
		}
	}
	if(chosen_solution != NULL && chosen_class != NULL){
		for(int i = 0; chosen_solution[i].m_class != NULL; ++i){
			if(chosen_solution[i].m_class->id == chosen_class->id){
				int i_per = chosen_solution[i].period;
				if(chosen_subject == NULL || (chosen_subject->id == chosen_solution[i].subj->id)){
					if(chosen_teacher == NULL){
						m_grid->SetCellValue(1 + (i_per % school->n_periods_per_day),1 +  (i_per / school->n_periods_per_day), chosen_solution[i].teacher->name);
					} else if(chosen_solution[i].teacher->id == chosen_teacher->id){
						m_grid->SetCellValue(1 + (i_per % school->n_periods_per_day),1 +  (i_per / school->n_periods_per_day), wxT("x"));
					}
				}
			}
		}
	} else if(chosen_solution != NULL && chosen_teacher != NULL){
		for(int i = 0; chosen_solution[i].m_class != NULL; ++i){
			if(chosen_solution[i].teacher->id == chosen_teacher->id){
				int i_per = chosen_solution[i].period;
				if(chosen_subject == NULL || (chosen_subject->id == chosen_solution[i].subj->id)){
					m_grid->SetCellValue(1 + (i_per % school->n_periods_per_day),1 +  (i_per / school->n_periods_per_day), chosen_solution[i].m_class->name);
				}
			}
		}
	}
}

DescTimetablePane::~DescTimetablePane(){

}
