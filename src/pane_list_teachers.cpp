#include "gui.hpp"
#include <wx/spinctrl.h>
extern "C" {
	#include "loader.h"
	#include "assert.h"
	#include "util.h"
	#include "maths.h"
};

ListTeachersPane::ListTeachersPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(250,250,250));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * max_days_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_max_number_of_days);
	wxStaticText * max_periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_max_number_of_periods);
	wxStaticText * max_ppd_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_max_number_of_periods_per_day);
	wxStaticText * max_ppcpd_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_max_per_class_per_day);
	wxStaticText * planning_periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_number_of_planning_periods);
	wxStaticText * planning_needs_room_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_planning_needs_room);
	wxStaticText * active_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_active);
	wxNotebook   * notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	/* DAYS CODE */
	wxVector<wxString> day_names = wxVector<wxString>();
	for(i = 0; i < school->n_days;++i){
		day_names.push_back(wxString::FromUTF8(school->day_names[i]));
	}

	m_planning_periods = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_lecture_periods = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_teaches = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_days = new PosIntGridPane(m_owner, notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_owner->m_lang->str_max_number_of_periods,day_names);
	m_lecture_rooms = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_planning_rooms = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_groups = new ScoreGridPane(m_owner, notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_planning_twinning = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	notebook->AddPage(m_lecture_periods, m_owner->m_lang->str_lecture_periods);
	notebook->AddPage(m_planning_periods, m_owner->m_lang->str_planning_periods);
	notebook->AddPage(m_teaches, m_owner->m_lang->str_subjects);
	notebook->AddPage(m_days, m_owner->m_lang->str_days);
	notebook->AddPage(m_planning_twinning, m_owner->m_lang->str_planning_time_twinning);
	notebook->AddPage(m_lecture_rooms, m_owner->m_lang->str_lecture_rooms);
	notebook->AddPage(m_planning_rooms, m_owner->m_lang->str_planning_rooms);
	notebook->AddPage(m_groups, m_owner->m_lang->str_teacher_groups);
	m_teachers_list = new SearchableListPane(m_owner, this, wxID_ANY, wxDefaultPosition, wxSize(230,300));
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1));
	m_max_days_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_max_periods_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_max_ppd_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_max_ppcpd_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_planning_periods_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_planning_needs_room_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_active_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit, wxDefaultPosition, wxSize(200,30));
	m_cancel_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_cancel, wxDefaultPosition, wxSize(200,30));

	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(200,30));
	/* LECTURE PERIODS CODE */
	ChoiceGrid * lecture_periods_grid = m_lecture_periods->GetGrid();
	lecture_periods_grid->AddState(m_owner->m_lang->str_teacher_unavailable, wxColor(255,200,200));
	lecture_periods_grid->AddState(m_owner->m_lang->str_teacher_available, wxColor(200,200,255));
	// TODO substitute by the names (but in ClearData())
	lecture_periods_grid->SetDefaultColumnLabel(m_owner->m_lang->str_day);
	lecture_periods_grid->SetDefaultRowLabel(m_owner->m_lang->str_period);
	lecture_periods_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);
	/* PLANNING PERIODS CODE */
	ChoiceGrid * planning_periods_grid =  m_planning_periods->GetGrid();
	planning_periods_grid->AddState(m_owner->m_lang->str_teacher_unavailable, wxColor(255,200,200));
	planning_periods_grid->AddState(m_owner->m_lang->str_teacher_available, wxColor(200,200,255));
	// TODO substitute by the names
	planning_periods_grid->SetDefaultColumnLabel(m_owner->m_lang->str_day);
	planning_periods_grid->SetDefaultRowLabel(m_owner->m_lang->str_period);
	planning_periods_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);
	/* Groups Code */
	ChoiceGrid * groups_grid = m_groups->GetGrid();
	groups_grid->AddState(m_owner->m_lang->str_no, wxColor(255,200,200));
	groups_grid->AddState(m_owner->m_lang->str_yes, wxColor(200,200,255));
	groups_grid->SetColLabel(0, m_owner->m_lang->str_belongs);
	groups_grid->GridRemake(1, school->n_teachers);
	for(int i = 0; i < school->n_teachers; ++i){
		groups_grid->SetRowLabel(i, wxString::FromUTF8(school->teachers[i].name));
	}

	ChoiceGrid * teaches_grid = m_teaches->GetGrid();
	teaches_grid->AddState(m_owner->m_lang->str_no, wxColor(255,200,200));
	teaches_grid->AddState(m_owner->m_lang->str_yes, wxColor(200,200,255));
	teaches_grid->SetColLabel(0, m_owner->m_lang->str_teaches);
 	ChoiceGrid * lec_rooms_grid = m_lecture_rooms->GetGrid();
	lec_rooms_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	lec_rooms_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	lec_rooms_grid->SetColLabel(0,m_owner->m_lang->str_name);

	ChoiceGrid * planning_rooms_grid = m_planning_rooms->GetGrid();
	planning_rooms_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	planning_rooms_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	planning_rooms_grid->SetColLabel(0,m_owner->m_lang->str_name);

	ChoiceGrid * twinning_grid = m_planning_twinning->GetGrid();
	twinning_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	twinning_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	twinning_grid->SetColLabel(0,m_owner->m_lang->str_name);

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * fields_sz = new wxFlexGridSizer(4,5,5);
	wxSizer * fields_wrap = new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_basic_data);

	fields_sz->Add(name_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_name_text, 0, wxEXPAND);
	fields_sz->Add(max_days_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_max_days_text, 0,	 wxEXPAND);
	fields_sz->Add(max_periods_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_max_periods_text, 0, wxEXPAND);
	fields_sz->Add(max_ppd_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_max_ppd_text, 0, wxEXPAND);
	fields_sz->Add(max_ppcpd_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_max_ppcpd_text, 0, wxEXPAND);
	// fields_sz->Add(planning_periods_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	// fields_sz->Add(m_planning_periods_text, 0, wxEXPAND);
	// fields_sz->Add(planning_needs_room_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	// fields_sz->Add(m_planning_needs_room_text, 0, wxEXPAND);
	fields_sz->Add(active_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_active_text, 0, wxEXPAND);
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->Add(m_cancel_btn, 0, wxEXPAND);
	fields_sz->Add(m_edit_btn, 0, wxEXPAND);

	planning_periods_label->Hide();
	m_planning_periods_text->Hide();
	planning_needs_room_label->Hide();
	m_planning_needs_room_text->Hide();

	fields_wrap->Add(fields_sz, 1, wxEXPAND|wxALL, 5);

	wxSizer * actions_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, m_owner->m_lang->str_actions);
	actions_sizer->Add(delete_btn, 0, wxALL, 5);

	desc_sz->Add(fields_wrap, 0,wxEXPAND);
	desc_sz->Add(notebook, 1, wxEXPAND);
	desc_sz->Add(actions_sizer, 0, wxEXPAND);

	sizer->Add(m_teachers_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(desc_sz, 1, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_lecture_periods->GetSaveButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnSaveLecturePeriods, this);
	m_lecture_periods->GetCancelButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelLecturePeriods, this);
	m_planning_periods->GetSaveButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnSavePlanningPeriods, this);
	m_planning_periods->GetCancelButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelPlanningPeriods, this);
	m_teaches->GetSaveButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnSaveTeaches, this);
	m_teaches->GetCancelButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelTeaches, this);
	m_days->GetSaveButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnSaveDays, this);
	m_days->GetCancelButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelDays, this);
	m_lecture_rooms->GetSaveButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnSaveLectureRoom, this);
	m_lecture_rooms->GetCancelButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelLectureRoom, this);
	m_planning_rooms->GetSaveButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnSavePlanningRoom, this);
	m_planning_rooms->GetCancelButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelPlanningRoom, this);
	m_planning_twinning->GetSaveButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnSavePlannningTwinning, this);
	m_planning_twinning->GetCancelButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelPlannningTwinning, this);
	m_groups->GetSaveButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnSaveGroups, this);
	m_groups->GetCancelButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelGroups, this);
	m_teachers_list->GetList()->Bind(wxEVT_LISTBOX, &ListTeachersPane::OnSelectionChanged, this);
	m_edit_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnDeleteButtonClicked, this);
	Bind(DATA_CHANGE_EVENT, &ListTeachersPane::OnDataChange, this);


	m_cancel_btn->Hide();
	m_name_text->Disable();
	m_max_days_text->Disable();
	m_max_periods_text->Disable();
	m_max_ppd_text->Disable();
	m_max_ppcpd_text->Disable();
	m_planning_periods_text->Disable();
	m_planning_needs_room_text->Disable();
	m_active_text->Disable();
	ShowData();
}

void ListTeachersPane::OnSaveTeaches(wxCommandEvent & evt){
	int i_select = m_teachers_list->GetList()->GetSelection();
	School * school = m_owner->m_school;
	if(i_select != wxNOT_FOUND){
		int id_teacher = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		LMH_ASSERT(id_teacher > 0);
		int * subjects = (int*) calloc(school->n_subjects + 1, sizeof(int));
		subjects[school->n_subjects] = -1;

		ChoiceGrid * teaches_grid = m_teaches->GetGrid();
		for(int i = 0; i < school->n_subjects; ++i){
			subjects[i] = teaches_grid->GetCellState(i, 0);
		}
		TeacherSubjectsUpdateAction * act = new TeacherSubjectsUpdateAction(m_owner, id_teacher, subjects);
		bool success = m_owner->Do(act);
		if(success) {
			evt.Skip();
		}
	}
}
void ListTeachersPane::OnCancelTeaches(wxCommandEvent & evt){
	int i_select = m_teachers_list->GetList()->GetSelection();
	School * school = m_owner->m_school;
	if(i_select != wxNOT_FOUND){
		int id_teacher = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		LMH_ASSERT(id_teacher > 0);
		Teacher * t = find_teacher_by_id(school, id_teacher);

		LMH_ASSERT(t != NULL);

		ChoiceGrid * teaches_grid = m_teaches->GetGrid();
		teaches_grid->SetAllCellsState(0);
		for(int i  = 0; i < school->n_teaches; ++i){
			if(school->teaches[i].teacher->id == t->id){
				int i_subj = get_subject_index_by_id(school, school->teaches[i].subject->id);
				LMH_ASSERT(i_subj >= 0);
				teaches_grid->SetCellState(i_subj,0, 1);
			}
		}
	}
	evt.Skip();
}

void ListTeachersPane::OnSaveDays(wxCommandEvent & evt){
	int i_select = m_teachers_list->GetList()->GetSelection();
	School * school = m_owner->m_school;
	if(i_select != wxNOT_FOUND){
		int id_teacher = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		LMH_ASSERT(id_teacher > 0);
		int * day_max = (int*) calloc(school->n_days + 1, sizeof(int));
		day_max[school->n_days] = -1;

		wxGrid* days_grid = m_days->GetGrid();
		for(int i = 0; i < school->n_days; ++i){
			day_max[i] = wxAtoi(days_grid->GetCellValue(i, 0));
			printf("Day max[%d] was %d\n", i, day_max[i]);
		}
		TeacherDaysUpdateAction * act = new TeacherDaysUpdateAction(m_owner, id_teacher, day_max);

		bool success = m_owner->Do(act);
		if(success){
			evt.Skip();
		}
	}
}
void ListTeachersPane::OnCancelDays(wxCommandEvent & evt){
	School * school = m_owner->m_school;

	int i_select = m_teachers_list->GetList()->GetSelection();
	int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
	Teacher * t = find_teacher_by_id(school, teacher_id);

	LMH_ASSERT(teacher_id != -1 && t != NULL);

	for(int i = 0; i < school->n_days; ++i){
		m_days->SetCellValue(i,0, t->day_max_meetings[i]);
	}
	evt.Skip();
}

void ListTeachersPane::OnSavePlannningTwinning(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * twinning_grid = m_planning_twinning->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select >= 0){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		Teacher * t = find_teacher_by_id(school, teacher_id);

		int * data = (int *) calloc(1 + school->n_periods_per_day, sizeof(int));
		bool success = false;

		LMH_ASSERT(data);

		for(int i = 0; i < school->n_periods_per_day; ++i){
			data[i] = twinning_grid->GetCellState(i,0);
		}
		data[school->n_periods_per_day] = -1;

		Action * action = new TeacherTwinningUpdateAction(m_owner, t->id, data);
		success = m_owner->Do(action);
		if(success){
			evt.Skip();
		}
	}
}
void ListTeachersPane::OnCancelPlannningTwinning(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * twinning_grid = m_planning_twinning->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select >= 0){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		Teacher * t = find_teacher_by_id(school, teacher_id);

		if(t->planning_twin_scores){
			for(int i = 0; i < school->n_periods_per_day; ++i){
				twinning_grid->SetCellState(i, 0, t->planning_twin_scores[i]);
			}
		}
		evt.Skip();
	}
}

void ListTeachersPane::OnSaveLectureRoom(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * grid = m_lecture_rooms->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		bool success = false;
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		int * scores = (int*) calloc(school->n_rooms + 1, sizeof(int));
		scores[school->n_rooms] = -1;

		for(int i = 0; i < school->n_rooms; ++i){
			int cell_state = grid->GetCellState(i,0);
			scores[i] = cell_state == -1 ? 0 : cell_state;
		}

		Action * act = new TeacherLectureRoomUpdateAction(m_owner, teacher_id, scores);
		success = m_owner->Do(act);

		if(success){
			evt.Skip();
		}
	}
}
void ListTeachersPane::OnCancelLectureRoom(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * grid = m_lecture_rooms->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		Teacher * t = find_teacher_by_id(school, teacher_id);
		LMH_ASSERT(t->lecture_room_scores != NULL);
		for(int i = 0; i < school->n_rooms; ++i){
			grid->SetCellState(i,0, t->lecture_room_scores[i]);
		}

		evt.Skip();
	}
}

void ListTeachersPane::OnSavePlanningRoom(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * grid = m_planning_rooms->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		bool success = false;
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		int * scores = (int*) calloc(school->n_rooms + 1, sizeof(int));
		scores[school->n_rooms] = -1;

		for(int i = 0; i < school->n_rooms; ++i){
			int cell_state = grid->GetCellState(i,0);
			scores[i] = cell_state == -1 ? 0 : cell_state;
		}
		Action * act = new TeacherPlanningRoomUpdateAction(m_owner, teacher_id, scores);
		success = m_owner->Do(act);

		if(success){
			evt.Skip();
		}
	}
}
void ListTeachersPane::OnCancelPlanningRoom(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * grid = m_planning_rooms->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		Teacher * t = find_teacher_by_id(school, teacher_id);
		LMH_ASSERT(t->planning_room_scores != NULL);
		for(int i = 0; i < school->n_rooms; ++i){
			grid->SetCellState(i,0, t->planning_room_scores[i]);
		}
		evt.Skip();
	}
}

void ListTeachersPane::OnSavePlanningPeriods(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * grid = m_planning_periods->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		bool success = false;
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		int * scores = (int*) calloc(school->n_periods + 1, sizeof(int));

		for(int i = 0; i < school->n_periods; ++i){
			scores[i] = grid->GetCellState(i % school->n_periods_per_day,i / school->n_periods_per_day);
		}
		scores[school->n_periods] = -1;

		Action * act = new TeacherPlanningPeriodsUpdateAction(m_owner, teacher_id, scores);
		success = m_owner->Do(act);
		if(success){
			evt.Skip();
		}
	}
}
void ListTeachersPane::OnCancelPlanningPeriods(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * grid = m_planning_periods->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		LMH_ASSERT(teacher_id > 0);
		Teacher * t = find_teacher_by_id(m_owner->m_school, teacher_id);
		LMH_ASSERT(t != NULL);

		for(int i = 0; i < school->n_periods; ++i){
			grid->SetCellState(i % school->n_periods_per_day,i / school->n_periods_per_day, t->planning_period_scores[i]);
		}

		evt.Skip();
	}
}

void ListTeachersPane::OnSaveLecturePeriods(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * grid = m_lecture_periods->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		bool success = false;
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		int * scores = (int*) calloc(school->n_periods + 1, sizeof(int));

		for(int i = 0; i < school->n_periods; ++i){
			scores[i] = grid->GetCellState(i % school->n_periods_per_day,i / school->n_periods_per_day);
		}

		Action * act = new TeacherLecturePeriodsUpdateAction(m_owner, teacher_id, scores);
		success = m_owner->Do(act);
		if(success){
			evt.Skip();
		}
	}
}
void ListTeachersPane::OnCancelLecturePeriods(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * grid = m_lecture_periods->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		LMH_ASSERT(teacher_id > 0);
		Teacher * t = find_teacher_by_id(m_owner->m_school, teacher_id);
		LMH_ASSERT(t != NULL);

		for(int i = 0; i < school->n_periods; ++i){
			grid->SetCellState(i % school->n_periods_per_day,i / school->n_periods_per_day, t->lecture_period_scores[i]);
		}

		evt.Skip();
	}
}

void ListTeachersPane::OnSaveGroups(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		LMH_ASSERT(teacher_id > 0);

		int * in_groups = m_groups->GetValues();
		for(int i = 0; i < school->n_teachers; ++i){
			if(in_groups[i] == -1) in_groups[i] = 0;
		}

		Action * act = new TeacherInGroupsUpdateAction(m_owner, teacher_id, in_groups);
		if(m_owner->Do(act)){
			evt.Skip();
		} else {
			printf("Não doo'u\n");
		}
	}
}

void ListTeachersPane::OnCancelGroups(wxCommandEvent & evt){
	School * school = m_owner->m_school;
	ChoiceGrid * groups_grid = m_groups->GetGrid();
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		LMH_ASSERT(teacher_id > 0);
		int teacher_i = get_teacher_index_by_id(school, teacher_id);
		Teacher * t = &school->teachers[teacher_i];
		LMH_ASSERT(t != NULL);

		for(int i = 0; i < school->n_teachers; ++i){
			if(school->teachers[i].id == teacher_id){
				groups_grid->SetCellState(i,0,-1);
			} else {
				int * subordinates = school->teachers[i].subordinates;
				if(subordinates != NULL && subordinates[teacher_i] > 0){
					groups_grid->SetCellState(i,0,1);
				} else {
					groups_grid->SetCellState(i,0,0);
				}
			}
		}
		evt.Skip();
	}
}

void ListTeachersPane::OnEditButtonClicked(wxCommandEvent &) {
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND && m_cancel_btn->IsShown()){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;

		Teacher new_data = {
			.name = copy_wx_string(m_name_text->GetValue()),
			.short_name = copy_wx_string(m_name_text->GetValue()),
			.max_days = m_max_days_text->GetValue(),
			.max_meetings_per_day = m_max_ppd_text->GetValue(),
			.max_meetings_per_class_per_day = m_max_ppcpd_text->GetValue(),
			.max_meetings = m_max_periods_text->GetValue(),
			.planning_needs_room = m_planning_needs_room_text->GetValue(),
			.num_planning_periods = m_planning_periods_text->GetValue(),
			.active = m_active_text->GetValue()
		};

		Action * act = new TeacherBasicDataUpdateAction(m_owner, new_data, teacher_id);
		bool success = m_owner->Do(act);
		if(success){
			m_cancel_btn->Hide();
			m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
			m_name_text->Disable();
			m_max_days_text->Disable();
			m_max_periods_text->Disable();
			m_max_ppd_text->Disable();
			m_max_ppcpd_text->Disable();
			m_planning_periods_text->Disable();
			m_planning_needs_room_text->Disable();
			m_active_text->Disable();
		} else {
			printf("Failure");
		}
	} else {
		m_cancel_btn->Show();
		m_edit_btn->SetLabel(m_owner->m_lang->str_save);
		m_name_text->Enable();
		m_max_days_text->Enable();
		m_max_periods_text->Enable();
		m_max_ppd_text->Enable();
		m_max_ppcpd_text->Enable();
		m_planning_periods_text->Enable();
		m_planning_needs_room_text->Enable();
		m_active_text->Enable();
	}
}
void ListTeachersPane::OnCancelButtonClicked(wxCommandEvent &){
	School * school = m_owner->m_school;
	int i_select = m_teachers_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		Teacher * t = find_teacher_by_id(school, teacher_id);
		m_name_text->SetValue(t->name);
		m_max_days_text->SetValue(t->max_days);
		m_max_periods_text->SetValue(t->max_meetings);
		m_max_ppd_text->SetValue(t->max_meetings_per_day);
		m_max_ppcpd_text->SetValue(t->max_meetings_per_class_per_day);
		m_planning_periods_text->SetValue(t->num_planning_periods);
		m_active_text->SetValue(t->active);
	}

	m_cancel_btn->Hide();
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	m_name_text->Disable();
	m_max_days_text->Disable();
	m_max_periods_text->Disable();
	m_max_ppd_text->Disable();
	m_max_ppcpd_text->Disable();
	m_planning_periods_text->Disable();
	m_active_text->Disable();
}

void ListTeachersPane::OnDeleteButtonClicked(wxCommandEvent &) {
	int i_select = m_teachers_list->GetList()->GetSelection();
	School * school = m_owner->m_school;
	bool success = false;
	if(i_select != wxNOT_FOUND){
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		if(can_remove_teacher(school, teacher_id)){
			TeacherDeleteAction * action = new TeacherDeleteAction(m_owner, teacher_id);
			success = m_owner->Do(action);
			if(success){
				m_teachers_list->RemoveItem(teacher_id);
			} else {
				wxMessageDialog * dialog = new wxMessageDialog(nullptr, wxT("Could not delete teacher..."), m_owner->m_lang->str_error, wxOK);
				dialog->ShowModal();
			}
		} else {
			wxMessageDialog * dialog = new wxMessageDialog(nullptr, m_owner->m_lang->str_couldnt_delete_because_timetable, m_owner->m_lang->str_error, wxOK);
			dialog->ShowModal();
		}
	}
}

void ListTeachersPane::OnSelectionChanged(wxCommandEvent &) {
	int i_select = m_teachers_list->GetList()->GetSelection();
	School * school = m_owner->m_school;
	if(school != NULL && school->teachers != NULL && i_select != wxNOT_FOUND){
		ChoiceGrid * lecture_periods_grid = m_lecture_periods->GetGrid();
		ChoiceGrid * planning_periods_grid = m_planning_periods->GetGrid();
		ChoiceGrid * teaches_grid = m_teaches->GetGrid();
		ChoiceGrid * twinning_grid = m_planning_twinning->GetGrid();
		ChoiceGrid * lec_rooms_grid = m_lecture_rooms->GetGrid();
		ChoiceGrid * planning_rooms_grid = m_planning_rooms->GetGrid();
		wxGrid * days_grid = m_days->GetGrid();
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		int teacher_i = get_teacher_index_by_id(school, teacher_id);
		Teacher * t = &school->teachers[teacher_i];
		teaches_grid->SetAllCellsState(0);

		m_name_text->SetValue(wxString::FromUTF8(t->name));
		m_max_days_text->SetValue(t->max_days);
		m_max_periods_text->SetValue(t->max_meetings);
		m_max_ppd_text->SetValue(t->max_meetings_per_day);
		m_max_ppcpd_text->SetValue(t->max_meetings_per_class_per_day);
		m_planning_periods_text->SetValue(t->num_planning_periods);
		m_planning_needs_room_text->SetValue(t->planning_needs_room);
		m_active_text->SetValue(t->active);
		if(t->teaches != NULL){
			for(int i = 0; t->teaches[i] != NULL; ++i){
				int subj_i = get_subject_index_by_id(school, t->teaches[i]->subject->id);
				teaches_grid->SetCellState(subj_i, 0, t->teaches[i]->score > 0 ? 1:0);
			}
		} else {
			teaches_grid->SetAllCellsState(0);
			printf("T->teaches was null\n");
		}
		for(int i = 0; i < school->n_periods; ++i){
			if(school->periods[i]){
				planning_periods_grid->SetCellState(i % school->n_periods_per_day,i / school->n_periods_per_day, t->planning_period_scores[i] > 0? 1:0);
				lecture_periods_grid->SetCellState(i % school->n_periods_per_day,i / school->n_periods_per_day, t->lecture_period_scores[i] > 0? 1:0);
			}
		}
		if(t->planning_twin_scores){
			for(int i = 0; i < school->n_periods_per_day; ++i){
				twinning_grid->SetCellState(i, 0, t->planning_twin_scores[i]);
			}
		}
		for(int i = 0; i < school->n_days; ++i){
			days_grid->SetCellValue(i,0, wxString::Format("%d", t->day_max_meetings[i]));
		}

		for(int i = 0; i < school->n_rooms; ++i){
			lec_rooms_grid->SetCellState(i,0, t->lecture_room_scores[i]);
			planning_rooms_grid->SetCellState(i,0, t->planning_room_scores[i]);
		}

		ChoiceGrid * groups_grid = m_groups->GetGrid();
		for(int i = 0; i < school->n_teachers; ++i){
			if(school->teachers[i].id == teacher_id){
				groups_grid->SetCellState(i,0,-1);
			} else {
				int * subordinates = school->teachers[i].subordinates;
				if(subordinates != NULL && subordinates[teacher_i] > 0){
					groups_grid->SetCellState(i,0,1);
				} else {
					groups_grid->SetCellState(i,0,0);
				}
			}
		}
		FitInside();
	}
}


void ListTeachersPane::ShowData(){
	int i;

	ChoiceGrid * lecture_periods_grid =  m_lecture_periods->GetGrid();
	ChoiceGrid * planning_periods_grid =  m_planning_periods->GetGrid();
	ChoiceGrid * teaches_grid = m_teaches->GetGrid();
	ChoiceGrid * lec_rooms_grid = m_lecture_rooms->GetGrid();
	ChoiceGrid * planning_rooms_grid = m_planning_rooms->GetGrid();
	ChoiceGrid * twinning_grid = m_planning_twinning->GetGrid();

	School * school = m_owner->m_school;

	for(i = 0; i < school->n_subjects;++i){
		teaches_grid->SetRowLabel(i, wxString::FromUTF8(school->subjects[i].name));
	}
	teaches_grid->GridRemake(1,school->n_subjects);

	for(i = 0; i < school->n_rooms; ++i){
		lec_rooms_grid->SetRowLabel(i, wxString::FromUTF8(school->rooms[i].name));
	}
	lec_rooms_grid->GridRemake(1, school->n_rooms);

	for(i = 0; i < school->n_rooms; ++i){
		planning_rooms_grid->SetRowLabel(i, wxString::FromUTF8(school->rooms[i].name));
	}
	planning_rooms_grid->GridRemake(1, school->n_rooms);

	for(i = 0; i < school->n_periods_per_day; ++i){
		twinning_grid->SetRowLabel(i, wxString::Format("%d", i+1));
	}
	twinning_grid->GridRemake(1, school->n_periods_per_day);

	for(i = 0; i < school->n_periods; ++i){
		lecture_periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, school->periods[i] ? 1 : ChoiceGrid::CELL_STATE_LOCKED);
		planning_periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, school->periods[i] ? 1 : ChoiceGrid::CELL_STATE_LOCKED);
	}

	m_teachers_list->Clear();
	for(i = 0; i < school->n_teachers; ++i){
		wxString str = wxString::FromUTF8(school->teachers[i].name);
		if(school->teachers[i].subordinates && (find_first_positive(school->teachers[i].subordinates) >= 0)){
			str = wxString::Format("(%s) %s", m_owner->m_lang->str_group, str);
		}
		if(! school->teachers[i].active){
			str = wxString::Format("(%s) %s", m_owner->m_lang->str_inactive, str);
		}
		m_teachers_list->AddItem(school->teachers[i].id, str);
	}
}

void ListTeachersPane::OnDataChange(wxNotifyEvent & evt) {
	ShowData();
}

ListTeachersPane::~ListTeachersPane(){

}
