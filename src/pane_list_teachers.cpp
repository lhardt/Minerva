#include "gui.hpp"
#include <wx/spinctrl.h>
extern "C" {
	#include "loader.h"
	#include "preprocess.h"
};

ListTeachersPane::ListTeachersPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * max_days_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_max_number_of_days);
	wxStaticText * max_periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_max_number_of_periods);
	wxStaticText * max_ppd_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_max_number_of_periods_per_day);
	wxStaticText * planning_periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_number_of_planning_periods);
	wxStaticText * active_label = new wxStaticText(this, wxID_ANY, wxT("Ativo"));
	wxNotebook   * notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	/* DAYS CODE */
	wxVector<wxString> day_names = wxVector<wxString>();
	for(i = 0; i < school->n_days;++i){
		day_names.push_back(wxString::FromUTF8(school->day_names[i]));
	}

	m_periods = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_teaches = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_days = new PosIntGridPane(m_owner, notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxT("Max. Per"),day_names);
	m_rooms = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	wxScrolledWindow * groups = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	new wxStaticText(groups, wxID_ANY, wxT("O professor $x não participa de nenhum grupo. (TO DO)"), wxDefaultPosition, wxDefaultSize);
	m_planning_twinning = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	notebook->InsertPage(0,m_periods, wxT("Períodos"));
	notebook->InsertPage(1,m_teaches, wxT("Disciplinas"));
	notebook->InsertPage(2,m_days, wxT("Dias"));
	notebook->InsertPage(3,m_planning_twinning, wxT("Geminação do Planejamento"));
	notebook->InsertPage(4,m_rooms, wxT("Salas"));
	notebook->InsertPage(5, groups, wxT("Grupos de Profs."));

	m_teachers_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(230,300));
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_max_days_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_max_periods_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_max_ppd_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_planning_periods_text = new wxSpinCtrl(this, wxID_ANY, wxT(""));
	m_active_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit, wxDefaultPosition, wxSize(200,30));
	m_cancel_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_cancel, wxDefaultPosition, wxSize(200,30));

	wxButton * delete_btn = new wxButton(this, wxID_ANY,m_owner->m_lang->str_remove, wxDefaultPosition, wxSize(200,30));

	/* PERIODS CODE */
	ChoiceGrid * periods_grid =  m_periods->GetGrid();
	periods_grid->AddState(m_owner->m_lang->str_teacher_available, wxColor(200,200,255));
	periods_grid->AddState(m_owner->m_lang->str_teacher_unavailable, wxColor(255,200,200));
	// TODO substitute by the names
	periods_grid->m_basic_col_name = m_owner->m_lang->str_day;
	periods_grid->m_basic_row_name = m_owner->m_lang->str_period;
	periods_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);

	/* TEACHES CODE */
	ChoiceGrid * teaches_grid = m_teaches->GetGrid();
	teaches_grid->AddState(wxT("Leciona"), wxColor(200,200,255));
	teaches_grid->AddState(wxT("Não Leciona"), wxColor(255,200,200));

	teaches_grid->SetColName(i, wxT("Leciona"));
	for(i = 0; i < school->n_subjects;++i){
		teaches_grid->SetRowName(i, wxString::FromUTF8(school->subjects[i].name));
	}
	teaches_grid->GridRemake(1,school->n_subjects);

	// ROOM PANE CODE
	ChoiceGrid * rooms_grid = m_rooms->GetGrid();
	rooms_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	rooms_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	rooms_grid->SetColName(0,m_owner->m_lang->str_name);
	for(i = 0; i < school->n_rooms; ++i){
		rooms_grid->SetRowName(i, wxString::FromUTF8(school->rooms[i].name));
	}
	rooms_grid->GridRemake(1, school->n_rooms);

	// Planning twinning code
	ChoiceGrid * twinning_grid = m_planning_twinning->GetGrid();
	twinning_grid->AddState(m_owner->m_lang->str_class_available, wxColor(200,200,255));
	twinning_grid->AddState(m_owner->m_lang->str_class_unavailable, wxColor(255,200,200));
	twinning_grid->SetColName(0,m_owner->m_lang->str_name);
	for(i = 0; i < school->n_periods_per_day; ++i){
		twinning_grid->SetRowName(i, wxString::Format("%d", i+1));
	}
	twinning_grid->GridRemake(1, school->n_periods_per_day);

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * fields_sz = new wxFlexGridSizer(4,5,5);
	wxSizer * fields_wrap = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Dados Básicos"));

	fields_sz->Add(name_label);
	fields_sz->Add(m_name_text);
	fields_sz->Add(max_days_label);
	fields_sz->Add(m_max_days_text);
	fields_sz->Add(max_periods_label);
	fields_sz->Add(m_max_periods_text);
	fields_sz->Add(max_ppd_label);
	fields_sz->Add(m_max_ppd_text);
	fields_sz->Add(planning_periods_label);
	fields_sz->Add(m_planning_periods_text);
	fields_sz->Add(active_label);
	fields_sz->Add(m_active_text);
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->Add(m_cancel_btn);
	fields_sz->Add(m_edit_btn);

	fields_wrap->Add(fields_sz, 0, wxEXPAND|wxALL, 5);

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

	m_teachers_list->Bind(wxEVT_LISTBOX, &ListTeachersPane::OnSelectionChanged, this);
	m_edit_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnCancelButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnDeleteButtonClicked, this);

	for(i = 0; i < school->n_teachers; ++i){
		m_teachers_list->Insert(wxString::FromUTF8(school->teachers[i].name), i, new IntClientData(i));
	}
}

void ListTeachersPane::OnEditButtonClicked(wxCommandEvent &) {
	if(m_cancel_btn->IsShown()){
		m_cancel_btn->Hide();
		m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
		m_name_text->Disable();
		m_max_days_text->Disable();
		m_max_periods_text->Disable();
		m_max_ppd_text->Disable();
		m_planning_periods_text->Disable();
		m_active_text->Disable();
	} else {
		m_cancel_btn->Show();
		m_edit_btn->SetLabel(m_owner->m_lang->str_save);
		m_name_text->Enable();
		m_max_days_text->Enable();
		m_max_periods_text->Enable();
		m_max_ppd_text->Enable();
		m_planning_periods_text->Enable();
		m_active_text->Enable();
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
	int i_select = m_teachers_list->GetSelection();
	School * school = m_owner->m_school;
	bool success = false;
	if(i_select != wxNOT_FOUND){
		success = remove_teacher(stdout, m_owner->m_database, school->teachers[i_select].id);
		if(success){
			school_teacher_remove(school, i_select);
			m_owner->NotifyNewUnsavedData();
		} else {
			printf("Couldn't delete teacher\n");
		}
	}
}
void ListTeachersPane::OnSelectionChanged(wxCommandEvent &) {
	int i_select = m_teachers_list->GetSelection();
	School * school = m_owner->m_school;
	if(school != NULL && school->teachers != NULL && i_select != wxNOT_FOUND){
		ChoiceGrid * periods_grid = m_periods->GetGrid();
		Teacher * t = & school->teachers[i_select];

		m_name_text->SetLabel(wxString::FromUTF8(t->name));
		m_max_days_text->SetLabel(wxString::Format("%d", (t->max_days)));
		m_max_periods_text->SetLabel(wxString::Format("%d", (t->max_meetings)));
		m_max_ppd_text->SetLabel(wxString::Format("%d", (t->max_meetings_per_day)));
		m_planning_periods_text->SetLabel(wxString::Format("%d", (t->num_planning_periods)));
		// if(t->teaches != NULL){
		// 	for(int i = 0; t->teaches[i] != NULL; ++i){
		// 		m_teaches_text->SetLabel(m_teaches_text->GetLabel() +
		// 					wxString::Format(" %s;", wxString::FromUTF8(t->teaches[i]->subject->name)));
		// 	}
		// }
		// m_teaches_text->Wrap(300);

		periods_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);
		for(int i = 0; i < school->n_periods; ++i){
			if(school->periods[i] == false){
				periods_grid->SetCellImmutable(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day));
			} else {
				periods_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
						wxString::Format("%s" , (t->lecture_period_scores[i] > 0?m_owner->m_lang->str_teacher_available:m_owner->m_lang->str_teacher_unavailable) ));
				periods_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
						(t->lecture_period_scores[i] > 0?wxColor(200,200,255):wxColor(255,200,200)));
			}
			periods_grid->SetReadOnly(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), true);
		}
		FitInside();
	}
}

ListTeachersPane::~ListTeachersPane(){

}
