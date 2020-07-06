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
	wxSizer * grid_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_desc_timetable, wxPoint(30,30), wxSize(200,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxArrayString solution_arr;
	solution_arr.push_back(m_owner->m_lang->str_timetable__none);
	if(school->solutions != NULL){
		for(i = 0; i < school->n_solutions; ++i){
			solution_arr.push_back(wxString::FromUTF8(school->solutions[i].name));
		}

	}
	m_solution_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30), solution_arr);
	m_solution_choice->SetSelection(0);
	wxStaticText * solution_choice_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_choose_a_timetable, wxDefaultPosition, wxSize(170,20));

	wxArrayString class_arr;
	class_arr.push_back(m_owner->m_lang->str_class__none);
	if(school->classes != NULL){
		for(i = 0; i < school->n_classes; ++i){
			class_arr.push_back(wxString::FromUTF8(school->classes[i].name));
		}

	}
	m_class_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30), class_arr);
	m_class_choice->SetSelection(0);
	wxStaticText * class_choice_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class, wxDefaultPosition, wxSize(170,20));

	wxArrayString teacher_arr;
	teacher_arr.push_back(m_owner->m_lang->str_teacher__none);
	if(school->teachers != NULL){
		for(i = 0; i < school->n_teachers; ++i){
			teacher_arr.push_back(wxString::FromUTF8(school->teachers[i].name));
		}

	}
	m_teacher_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30), teacher_arr);
	m_teacher_choice->SetSelection(0);
	wxStaticText * teacher_choice_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teacher, wxDefaultPosition, wxSize(170,20));

	wxArrayString subj_arr;
	subj_arr.push_back(m_owner->m_lang->str_subject__none);
	if(school->subjects != NULL){
		for(i = 0; i < school->n_subjects; ++i){
			subj_arr.push_back(wxString::FromUTF8(school->subjects[i].name));
		}

	}
	m_subject_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30), subj_arr);
	m_subject_choice->SetSelection(0);
	wxStaticText * subject_choice_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_subject, wxDefaultPosition, wxSize(170,20));

	wxArrayString room_arr;
	room_arr.push_back(m_owner->m_lang->str_room__none);
	if(school->rooms != NULL){
		for(i = 0; i < school->n_rooms; ++i){
			room_arr.push_back(wxString::FromUTF8(school->rooms[i].name));
		}
	}
	m_room_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30), room_arr);
	m_room_choice->SetSelection(0);
	wxStaticText * room_choice_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_room, wxDefaultPosition, wxSize(170,20));

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

	wxStaticText * desc_title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_lecture_desc, wxDefaultPosition, wxSize(200,30));

	wxButton * detail_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_detail, wxDefaultPosition, wxSize(200,30));

	m_solution_choice->Bind(wxEVT_CHOICE, &DescTimetablePane::OnRedrawGridRequest, this);
	m_class_choice->Bind(wxEVT_CHOICE, &DescTimetablePane::OnRedrawGridRequest, this);
	m_teacher_choice->Bind(wxEVT_CHOICE, &DescTimetablePane::OnRedrawGridRequest, this);
	m_subject_choice->Bind(wxEVT_CHOICE, &DescTimetablePane::OnRedrawGridRequest, this);

	labels_sz->Add(class_choice_label, 1, wxLEFT|wxRIGHT, 5);
	labels_sz->Add(teacher_choice_label, 1,wxLEFT|wxRIGHT, 5);
	labels_sz->Add(room_choice_label, 1, wxLEFT|wxRIGHT, 5);
	labels_sz->Add(subject_choice_label, 1, wxLEFT|wxRIGHT, 5);

	choice_sz->Add(m_class_choice, 1, wxALL, 5);
	choice_sz->Add(m_teacher_choice, 1, wxALL, 5);
	choice_sz->Add(m_room_choice, 1, wxALL, 5);
	choice_sz->Add(m_subject_choice, 1, wxALL, 5);

	desc_sz->Add(desc_title, 0, wxALL, 15);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(detail_btn, 0, wxEXPAND | wxALL, 15);

	grid_sz->Add(m_grid, 1, wxRIGHT | wxEXPAND, 5);
	grid_sz->Add(desc_sz, 0, wxEXPAND | wxLEFT, 5);

	sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 15);
	sizer->Add(solution_choice_label, 0, wxLEFT | wxRIGHT, 20);
	sizer->Add(m_solution_choice, 0, wxLEFT | wxRIGHT | wxBOTTOM, 20);
	sizer->Add(labels_sz, 0, wxEXPAND | wxALIGN_CENTER | wxLEFT | wxRIGHT, 15);
	sizer->Add(choice_sz, 0, wxEXPAND | wxALIGN_CENTER | wxLEFT | wxRIGHT, 15);
	sizer->Add(grid_sz, 1, wxEXPAND | wxALL, 15);

	this->SetSizerAndFit(sizer);
}

void DescTimetablePane::OnRedrawGridRequest(wxCommandEvent & evt){
	int i, j;
	School * school = m_owner->m_school;

	bool chose_solution = m_solution_choice->GetSelection() != 0;
	bool chose_class = m_class_choice->GetSelection() != 0;
	bool chose_teacher = m_teacher_choice->GetSelection() != 0;
	bool chose_subject = m_subject_choice->GetSelection() != 0;
	bool chose_room = m_room_choice->GetSelection() != 0;

	for(i = 0; i < school->n_periods_per_day; ++i){
		for(j = 0;j < school->n_days; ++j){
			m_grid->SetCellValue(1 + i, 1 + j,wxT(""));
		}
	}
	if(!chose_solution){
		return;
	}
	Meeting * chosen_solution = school->solutions[ m_solution_choice->GetSelection()-1].meetings;
	for(int i = 0; chosen_solution[i].m_class != NULL; ++i){
		char * class_name    = chosen_solution[i].m_class->name;
		char * teacher_name  = chosen_solution[i].teacher->name;
		char * subject_name  = chosen_solution[i].subj->name;
		char * room_name = chosen_solution[i].room->name;
		/* Should not print if this meeting is about another room / class / teacher / subj. */
		bool this_class = !chose_class || school->classes[ m_class_choice->GetSelection()-1 ].id == chosen_solution[i].m_class->id;
		bool this_teacher = !chose_teacher || school->teachers[ m_teacher_choice->GetSelection() -1].id == chosen_solution[i].teacher->id;
		bool this_subject = !chose_subject || school->subjects[m_subject_choice->GetSelection() -1].id == chosen_solution[i].subj->id;
		bool this_room = !chose_room || school->rooms[m_room_choice->GetSelection() - 1].id == chosen_solution[i].room->id;

		if(this_class && this_teacher && this_subject && this_room){
			int x = 1 + chosen_solution[i].period % school->n_periods_per_day;
			int y = 1 +  chosen_solution[i].period / school->n_periods_per_day;
			if(chose_class){
			 	if(chose_teacher && chose_subject){
					m_grid->SetCellValue(x, y, wxT("x"));
				} else if(chose_subject){
					m_grid->SetCellValue(x, y, wxString::FromUTF8(teacher_name));
				} else if(chose_teacher){
					m_grid->SetCellValue(x, y, wxString::FromUTF8(subject_name));
				} else {
					m_grid->SetCellValue(x, y, wxString::FromUTF8(teacher_name) + wxT("(") + wxString::FromUTF8(subject_name) + wxT(")"));
				}
			} else if(chose_teacher){
				/*We know that chose_class = false */
				if(chose_subject){
					m_grid->SetCellValue(x, y, wxString::FromUTF8(class_name));
				} else {
					m_grid->SetCellValue(x, y, wxString::FromUTF8(class_name) + wxT(" (") + wxString::FromUTF8(subject_name) + wxT(")"));
				}
			}
			/* Avoiding naming rooms when there's nothing selected. */
			if(!chose_room && (chose_class || chose_teacher)){
				m_grid->SetCellValue(x,y, m_grid->GetCellValue(x,y) + wxString::FromUTF8(room_name));
			}
		}
	}
}

DescTimetablePane::~DescTimetablePane(){

}
