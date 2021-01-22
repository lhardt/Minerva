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
	wxStaticText * max_ppcpd_label = new wxStaticText(this, wxID_ANY, wxT("MAX PPCPD"));
	wxStaticText * planning_periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_number_of_planning_periods);
	wxStaticText * planning_needs_room_label = new wxStaticText(this, wxID_ANY, wxT("Planning needs room"));
	wxStaticText * active_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_active);
	wxStaticText * dependency_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_dependency);
	wxNotebook   * notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	/* DAYS CODE */
	wxVector<wxString> day_names = wxVector<wxString>();
	for(i = 0; i < school->n_days;++i){
		day_names.push_back(wxString::FromUTF8(school->day_names[i]));
	}

	m_periods = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_teaches = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_days = new PosIntGridPane(m_owner, notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_owner->m_lang->str_max_number_of_periods,day_names);
	m_lecture_rooms = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_planning_rooms = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_groups = new ScoreGridPane(m_owner, notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_planning_twinning = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	notebook->InsertPage(0, m_periods, m_owner->m_lang->str_periods);
	notebook->InsertPage(1, m_teaches, m_owner->m_lang->str_subjects);
	notebook->InsertPage(2, m_days, m_owner->m_lang->str_days);
	notebook->InsertPage(3, m_planning_twinning, m_owner->m_lang->str_planning_time_twinning);
	notebook->InsertPage(4, m_lecture_rooms, m_owner->m_lang->str_lecture_rooms);
	notebook->InsertPage(5, m_planning_rooms, m_owner->m_lang->str_planning_rooms);
	notebook->InsertPage(6, m_groups, m_owner->m_lang->str_teacher_groups);
	m_teachers_list = new SearchableListPane(m_owner, this, wxID_ANY, wxDefaultPosition, wxSize(230,300));
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1));
	m_max_days_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_max_periods_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_max_ppd_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_max_ppcpd_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_planning_periods_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_planning_needs_room_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_active_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_dependency_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit, wxDefaultPosition, wxSize(200,30));
	m_cancel_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_cancel, wxDefaultPosition, wxSize(200,30));

	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(200,30));
	/* PERIODS CODE */
	ChoiceGrid * periods_grid =  m_periods->GetGrid();
	periods_grid->AddState(m_owner->m_lang->str_teacher_unavailable, wxColor(255,200,200));
	periods_grid->AddState(m_owner->m_lang->str_teacher_available, wxColor(200,200,255));
	// TODO substitute by the names
	periods_grid->SetDefaultColumnLabel(m_owner->m_lang->str_day);
	periods_grid->SetDefaultRowLabel(m_owner->m_lang->str_period);
	periods_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);

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
	fields_sz->Add(planning_periods_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_planning_periods_text, 0, wxEXPAND);
	fields_sz->Add(planning_needs_room_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_planning_needs_room_text, 0, wxEXPAND);
	fields_sz->Add(active_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_active_text, 0, wxEXPAND);
	fields_sz->Add(dependency_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	fields_sz->Add(m_dependency_text, 0, wxEXPAND);
	fields_sz->Add(m_cancel_btn, 0, wxEXPAND);
	fields_sz->Add(m_edit_btn, 0, wxEXPAND);

	fields_wrap->Add(fields_sz, 1, wxEXPAND|wxALL, 5);

	desc_sz->Add(fields_wrap, 0,wxEXPAND);
	desc_sz->Add(notebook, 1, wxEXPAND);
	desc_sz->Add(delete_btn, 0, wxEXPAND);

	sizer->Add(m_teachers_list, 0, wxEXPAND|wxALL, 15);
	sizer->Add(desc_sz, 1, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_teaches->GetSaveButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnSaveTeaches, this);
	m_teaches->GetCancelButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelTeaches, this);
	m_days->GetSaveButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnSaveDays, this);
	m_days->GetCancelButton()->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelDays, this);
	m_teachers_list->GetList()->Bind(wxEVT_LISTBOX, &ListTeachersPane::OnSelectionChanged, this);
	m_edit_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnDeleteButtonClicked, this);
	m_dependency_text->Bind(wxEVT_CHECKBOX, &ListTeachersPane::OnDependencyButtonClicked,this);
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
	m_dependency_text->Disable();
	m_groups->Hide();

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

void ListTeachersPane::OnCancelTeaches(wxCommandEvent &){

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

void ListTeachersPane::OnCancelDays(wxCommandEvent &){

}


void ListTeachersPane::ShowData(){
	int i;

	ChoiceGrid * periods_grid =  m_periods->GetGrid();
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
		periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, school->periods[i] ? 1 : ChoiceGrid::CELL_STATE_LOCKED);
	}

	m_teachers_list->Clear();
	for(i = 0; i < school->n_teachers; ++i){
		if(school->teachers[i].subordinates && (find_first_positive(school->teachers[i].subordinates) >= 0)){
			m_teachers_list->AddItem(school->teachers[i].id, wxString::Format("(%s) %s", m_owner->m_lang->str_group, wxString::FromUTF8(school->teachers[i].name)));
		} else {
			m_teachers_list->AddItem(school->teachers[i].id, wxString::FromUTF8(school->teachers[i].name));
		}
	}
}

void ListTeachersPane::OnDataChange(wxNotifyEvent & evt) {
	ShowData();
}

void ListTeachersPane::OnDependencyButtonClicked(wxCommandEvent &){
	if(m_dependency_text->GetValue()){
		m_groups->Show();
	} else {
		m_groups->Hide();
	}
	Layout();
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
			m_dependency_text->Disable();
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
		m_dependency_text->Enable();
	}
}

void ListTeachersPane::OnCancelButtonClicked(wxCommandEvent &){
	m_cancel_btn->Hide();
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	m_name_text->Disable();
	m_max_days_text->Disable();
	m_max_periods_text->Disable();
	m_max_ppd_text->Disable();
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
			wxMessageDialog * dialog = new wxMessageDialog(nullptr, wxT("(TODO lang) Error! Could not remove, because it is in a timetable already"), m_owner->m_lang->str_error, wxOK);
			dialog->ShowModal();
		}
	}
}
void ListTeachersPane::OnSelectionChanged(wxCommandEvent &) {
	int i_select = m_teachers_list->GetList()->GetSelection();
	School * school = m_owner->m_school;
	if(school != NULL && school->teachers != NULL && i_select != wxNOT_FOUND){
		ChoiceGrid * periods_grid = m_periods->GetGrid();
		ChoiceGrid * teaches_grid = m_teaches->GetGrid();
		wxGrid * days_grid = m_days->GetGrid();
		int teacher_id = ((IntClientData*)m_teachers_list->GetList()->GetClientObject(i_select))->m_value;
		Teacher * t = find_teacher_by_id(school, teacher_id);
		teaches_grid->SetAllCellsState(0);

		m_name_text->SetValue(wxString::FromUTF8(t->name));
		m_max_days_text->SetValue(t->max_days);
		m_max_periods_text->SetValue(t->max_meetings);
		m_max_ppd_text->SetValue(t->max_meetings_per_day);
		m_max_ppcpd_text->SetValue(t->max_meetings_per_class_per_day);
		m_planning_periods_text->SetValue(t->num_planning_periods);
		m_planning_needs_room_text->SetValue(t->planning_needs_room);
		if(t->teaches != NULL){
			for(int i = 0; t->teaches[i] != NULL; ++i){
				int subj_i = get_subject_index_by_id(school, t->teaches[i]->subject->id);
				teaches_grid->SetCellState(subj_i, 0, t->teaches[i]->score > 0 ? 1:0);
			}
		} else {
			printf("T->teaches was null\n");
		}
		for(int i = 0; i < school->n_periods; ++i){
			if(school->periods[i]){
				periods_grid->SetCellState(i % school->n_periods_per_day,i / school->n_periods_per_day, t->lecture_period_scores[i] > 0? 1:0);
			}
		}
		for(int i = 0; i < school->n_days; ++i){
			days_grid->SetCellValue(i,0, wxString::Format("%d", t->day_max_meetings[i]));
		}
		FitInside();
	}
}

ListTeachersPane::~ListTeachersPane(){

}
