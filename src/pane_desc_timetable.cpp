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
	wxSizer * fields_sz = new wxFlexGridSizer(2,5,5);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * solution_choice_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_choose_a_timetable, wxDefaultPosition, wxSize(170,20));
	m_solution_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30));
	m_class_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30));
	m_teacher_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30));
	m_subject_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30));
	m_room_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(170,30));

	m_solution_choice->Insert(m_owner->m_lang->str_timetable__none, 0, new IntClientData(-1));
	for(i = 0; i < school->n_solutions; ++i){
		m_solution_choice->Insert(wxString::FromUTF8(school->solutions[i].name), i+1, new IntClientData(i));
	}

	m_class_choice->Insert(m_owner->m_lang->str_class__none, 0);
	for(i = 0; i < school->n_classes; ++i){
		m_class_choice->Insert(wxString::FromUTF8(school->classes[i].name), i+1, new IntClientData(i));
	}

	m_teacher_choice->Insert(m_owner->m_lang->str_teacher__none, 0);
	for(i = 0; i < school->n_teachers; ++i){
		m_teacher_choice->Insert(wxString::FromUTF8(school->teachers[i].name), i+1, new IntClientData(i));
	}

	m_subject_choice->Insert(m_owner->m_lang->str_subject__none, 0);
	for(i = 0; i < school->n_subjects; ++i){
		m_subject_choice->Insert(wxString::FromUTF8(school->subjects[i].name), i+1, new IntClientData(i));
	}

	m_room_choice->Insert(m_owner->m_lang->str_room__none, 0);
	if(school->rooms != NULL){
		for(i = 0; i < school->n_rooms; ++i){
			m_room_choice->Insert(wxString::FromUTF8(school->rooms[i].name), i+1, new IntClientData(i));
		}
	}
	m_solution_choice->SetSelection(0);
	m_class_choice->SetSelection(0);
	m_teacher_choice->SetSelection(0);
	m_subject_choice->SetSelection(0);
	m_room_choice->SetSelection(0);
	wxStaticText * class_choice_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class, wxDefaultPosition, wxSize(170,20));
	wxStaticText * teacher_choice_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teacher, wxDefaultPosition, wxSize(170,20));
	wxStaticText * subject_choice_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_subject, wxDefaultPosition, wxSize(170,20));
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
	wxStaticText * class_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class);
	wxStaticText * teacher_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teacher);
	wxStaticText * subject_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_subject);
	wxStaticText * room_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_room);
	wxStaticText * day_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_day);
	wxStaticText * period_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_period);

	class_label->SetFont(*m_owner->m_bold_text_font);
	teacher_label->SetFont(*m_owner->m_bold_text_font);
	subject_label->SetFont(*m_owner->m_bold_text_font);
	room_label->SetFont(*m_owner->m_bold_text_font);
	day_label->SetFont(*m_owner->m_bold_text_font);
	period_label->SetFont(*m_owner->m_bold_text_font);

	m_class_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_teacher_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_subject_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_room_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_day_text = new wxStaticText(this, wxID_ANY, wxT(""));
	m_period_text = new wxStaticText(this, wxID_ANY, wxT(""));

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

	fields_sz->Add(class_label);
	fields_sz->Add(m_class_text);
	fields_sz->Add(teacher_label);
	fields_sz->Add(m_teacher_text);
	fields_sz->Add(subject_label);
	fields_sz->Add(m_subject_text);
	fields_sz->Add(room_label);
	fields_sz->Add(m_room_text);
	fields_sz->Add(day_label);
	fields_sz->Add(m_day_text);
	fields_sz->Add(period_label);
	fields_sz->Add(m_period_text);

	desc_sz->Add(desc_title, 0, wxALL, 15);
	desc_sz->Add(fields_sz);

	grid_sz->Add(m_grid, 1, wxRIGHT | wxEXPAND, 5);
	grid_sz->Add(desc_sz, 0, wxEXPAND | wxLEFT, 5);

	sizer->Add(solution_choice_label, 0, wxLEFT | wxRIGHT | wxTOP, 15);
	sizer->Add(m_solution_choice, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);
	sizer->Add(labels_sz, 0, wxEXPAND | wxALIGN_CENTER | wxLEFT | wxRIGHT, 15);
	sizer->Add(choice_sz, 0, wxEXPAND | wxALIGN_CENTER | wxLEFT | wxRIGHT, 15);
	sizer->Add(grid_sz, 1, wxEXPAND | wxALL, 15);

	SetSizer(sizer);
	Layout();

	m_grid->Bind(wxEVT_GRID_SELECT_CELL, &DescTimetablePane::OnGridSelection, this);
}

void DescTimetablePane::OnGridSelection(wxGridEvent & evt) {
	int i_met, col = evt.GetCol(), row = evt.GetRow();
	bool chose_solution = m_solution_choice->GetSelection() != 0;
	bool chose_class = m_class_choice->GetSelection() != 0;
	bool chose_teacher = m_teacher_choice->GetSelection() != 0;
	School * school = m_owner->m_school;
	if(col > 0 && row > 0 && chose_solution && (chose_class || chose_teacher) ){
		Solution * solution = & (m_owner->m_school->solutions[m_solution_choice->GetSelection()-1]);
		int i_per = (col-1) * school->n_periods_per_day + (row-1);

		for(i_met = 0; i_met < solution->n_meetings; ++i_met){
			Meeting * met = &(solution->meetings[i_met]);
			if(i_per == met->period
					&& (!chose_class || met->m_class == &school->classes[ m_class_choice->GetSelection()-1])
					&& (!chose_teacher || met->teacher == &school->teachers[m_teacher_choice->GetSelection()-1])){
				m_class_text->SetLabel(wxString::FromUTF8(met->m_class->name));
				m_teacher_text->SetLabel(wxString::FromUTF8(met->teacher->name));
				m_subject_text->SetLabel(wxString::FromUTF8(met->subject->name));
				m_room_text->SetLabel(wxString::FromUTF8(met->room->name));
				m_day_text->SetLabel(wxString::FromUTF8(school->day_names[met->period / school->n_periods_per_day]));
				m_period_text->SetLabel(wxString::FromUTF8(school->daily_period_names[met->period % school->n_periods_per_day]));

				break;
			}
		}

	}
}

void DescTimetablePane::OnRedrawGridRequest(wxCommandEvent & evt){
	int i, j;
	School * school = m_owner->m_school;

	bool chose_solution = m_solution_choice->GetSelection() != 0;
	bool chose_class = m_class_choice->GetSelection() != 0;
	bool chose_teacher = m_teacher_choice->GetSelection() != 0;
	bool chose_subject = m_subject_choice->GetSelection() != 0;
	bool chose_room = m_room_choice->GetSelection() != 0;

	m_class_text->SetLabel("");
	m_teacher_text->SetLabel("");
	m_subject_text->SetLabel("");
	m_room_text->SetLabel("");
	m_day_text->SetLabel("");
	m_period_text->SetLabel("");

	for(i = 0; i < school->n_periods_per_day; ++i){
		for(j = 0;j < school->n_days; ++j){
			m_grid->SetCellValue(1 + i, 1 + j,wxT(""));
		}
	}
	if(!chose_solution){
		return;
	}
	Meeting * chosen_solution = school->solutions[ m_solution_choice->GetSelection()-1].meetings;
	for(int i = 0; i < school->solutions[ m_solution_choice->GetSelection()-1].n_meetings; ++i){
		char * class_name    = chosen_solution[i].m_class->name;
		char * teacher_name  = chosen_solution[i].teacher->name;
		char * subject_name  = chosen_solution[i].subject->name;
		char * room_name = chosen_solution[i].room->name;
		/* Should not print if this meeting is about another room / class / teacher / subject. */
		bool this_class = !chose_class || school->classes[ m_class_choice->GetSelection()-1 ].id == chosen_solution[i].m_class->id;
		bool this_teacher = !chose_teacher || school->teachers[ m_teacher_choice->GetSelection() -1].id == chosen_solution[i].teacher->id;
		bool this_subject = !chose_subject || school->subjects[m_subject_choice->GetSelection() -1].id == chosen_solution[i].subject->id;
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
