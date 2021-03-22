#include "gui.hpp"

extern "C" {
	#include "util.h"
	#include "loader.h"
};

ListTimetablesPane::ListTimetablesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(250,250,250));

	m_timetables_list = new SearchableListPane(m_owner, this, wxID_ANY);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * date_label = new wxStaticText(this, wxID_ANY, wxT("Data de Geração"));
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(180,-1));
	m_date_text = new wxStaticText(this, wxID_ANY, wxT("-- / -- / --"));

	wxButton * delete_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_delete);
	wxButton * export_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_export_timetable);
	wxNotebook * notebook = new wxNotebook(this, wxID_ANY);

	m_desc_text = new wxTextCtrl(notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	wxScrolledWindow * class_pane = new wxScrolledWindow(notebook, wxID_ANY);
	wxScrolledWindow * teacher_pane = new wxScrolledWindow(notebook, wxID_ANY);
	wxScrolledWindow * room_pane = new wxScrolledWindow(notebook, wxID_ANY);

	wxStaticText * class_label = new wxStaticText(class_pane, wxID_ANY, m_owner->m_lang->str_class);
	wxStaticText * teacher_label = new wxStaticText(teacher_pane, wxID_ANY, m_owner->m_lang->str_teacher);
	wxStaticText * room_label = new wxStaticText(room_pane, wxID_ANY, m_owner->m_lang->str_room);
	class_label->SetFont(*m_owner->m_small_font);
	teacher_label->SetFont(*m_owner->m_small_font);
	room_label->SetFont(*m_owner->m_small_font);
	m_class_picker = new wxChoice(class_pane, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_SORT);
	m_teacher_picker = new wxChoice(teacher_pane, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_SORT);
	m_room_picker = new wxChoice(room_pane, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_SORT);
	m_classes = new ChoiceGrid(m_owner, class_pane, wxID_ANY);
	m_teachers = new ChoiceGrid(m_owner, teacher_pane, wxID_ANY);
	m_rooms = new ChoiceGrid(m_owner, room_pane, wxID_ANY);

	wxSizer * class_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * teacher_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * room_sizer = new wxBoxSizer(wxVERTICAL);

	class_sizer->Add(class_label, 0, wxLEFT | wxTOP | wxRIGHT, 10);
	class_sizer->Add(m_class_picker, 0, wxLEFT | wxBOTTOM | wxRIGHT, 10);
	class_sizer->Add(m_classes, 0, wxBOTTOM | wxLEFT | wxRIGHT, 10);

	teacher_sizer->Add(teacher_label, 0, wxLEFT | wxTOP | wxRIGHT, 10);
	teacher_sizer->Add(m_teacher_picker, 0, wxLEFT | wxBOTTOM | wxRIGHT, 10);
	teacher_sizer->Add(m_teachers, 0, wxBOTTOM | wxLEFT | wxRIGHT, 10);

	room_sizer->Add(room_label, 0, wxLEFT | wxTOP | wxRIGHT, 10);
	room_sizer->Add(m_room_picker, 0, wxLEFT | wxBOTTOM | wxRIGHT, 10);
	room_sizer->Add(m_rooms, 0, wxBOTTOM | wxLEFT | wxRIGHT, 10);

	class_pane->SetSizerAndFit(class_sizer);
	teacher_pane->SetSizerAndFit(teacher_sizer);
	room_pane->SetSizerAndFit(room_sizer);

	notebook->AddPage(m_desc_text, wxT("Descrição"));
	notebook->AddPage(class_pane, m_owner->m_lang->str_classes);
	notebook->AddPage(teacher_pane, m_owner->m_lang->str_teachers);
	notebook->AddPage(room_pane, m_owner->m_lang->str_rooms);

	wxSizer * actions_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, m_owner->m_lang->str_actions);
	actions_sizer->Add(export_btn, 0, wxALL, 5);
	actions_sizer->Add(delete_btn, 0, wxALL, 5);

	wxSizer * basic_sizer = new wxFlexGridSizer(4,5,5);
	basic_sizer->Add(name_label, 0, wxALIGN_BOTTOM | wxRIGHT, 10);
	basic_sizer->Add(m_name_text);
	basic_sizer->Add(date_label, 0, wxALIGN_BOTTOM | wxLEFT | wxRIGHT, 10);
	basic_sizer->Add(m_date_text, 0, wxALIGN_BOTTOM);

	wxSizer * basic_container = new wxStaticBoxSizer(wxHORIZONTAL, this, m_owner->m_lang->str_basic_data);
	basic_container->Add(basic_sizer, 1, wxEXPAND | wxALL, 5);

	wxSizer * content_sizer = new wxBoxSizer(wxVERTICAL);
	content_sizer->Add(basic_container, 0, wxBOTTOM | wxEXPAND, 5);
	content_sizer->Add(notebook, 1, wxBOTTOM | wxEXPAND, 5);
	content_sizer->Add(actions_sizer, 0, wxEXPAND, 5);

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_timetables_list, 0, wxALL | wxEXPAND, 15);
	sizer->Add(content_sizer, 1, wxEXPAND | wxBOTTOM | wxTOP | wxRIGHT, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_class_picker->Enable(false);
	m_teacher_picker->Enable(false);
	m_room_picker->Enable(false);
	m_name_text->Enable(false);
	m_desc_text->Enable(false);

	m_timetables_list->GetList()->Bind(wxEVT_LISTBOX, &ListTimetablesPane::OnSelectionChanged, this);
	m_teacher_picker->Bind(wxEVT_CHOICE, &ListTimetablesPane::OnTeacherSelectionChaged, this);
	m_class_picker->Bind(wxEVT_CHOICE, &ListTimetablesPane::OnClassSelectionChaged, this);
	m_room_picker->Bind(wxEVT_CHOICE, &ListTimetablesPane::OnRoomSelectionChaged, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListTimetablesPane::OnDeleteButtonClicked, this);
	export_btn->Bind(wxEVT_BUTTON, &ListTimetablesPane::OnExportButtonClicked, this);
	Bind(DATA_CHANGE_EVENT, &ListTimetablesPane::OnDataChange, this);

	ShowData();
}

void ListTimetablesPane::OnDeleteButtonClicked(wxCommandEvent &){
	//TODO make action
	int i_select = m_timetables_list->GetList()->GetSelection() ;
	if(i_select != wxNOT_FOUND){
		int id_solution = ((IntClientData*)m_timetables_list->GetList()->GetClientObject(i_select))->m_value;
		if(remove_solution(stdout, m_owner->m_database, id_solution)){
			school_solution_remove(m_owner->m_school, get_solution_index_by_id(m_owner->m_school, id_solution), true);
			ShowData();
			m_owner->NotifyNewUnsavedData();
		} else {
			printf("Couldn't");
		}
	}
}

void ListTimetablesPane::OnExportButtonClicked(wxCommandEvent &){
	printf("Export button clicked. To do!\n");
}

void ListTimetablesPane::OnClassSelectionChaged(wxCommandEvent &){
	School * school = m_owner->m_school;
	if(m_class_picker->GetSelection() != wxNOT_FOUND){
		int i_sol_select = m_timetables_list->GetList()->GetSelection();
		int solution_id = ((IntClientData*)m_timetables_list->GetList()->GetClientObject(i_sol_select))->m_value;
		int class_id = ((IntClientData*)m_class_picker->GetClientObject(m_class_picker->GetSelection()))->m_value;
		Solution * sol = find_solution_by_id(m_owner->m_school, solution_id);

		m_classes->SetAllCellsState(-1);
		for(int i = 0; i < sol->n_meetings; ++i){
			if(class_id == sol->meetings[i].m_class->id && sol->meetings[i].type == meet_LECTURE){
				int period = sol->meetings[i].period;
				int teacher_i = get_teacher_index_by_id(school, sol->meetings[i].teacher->id);
				m_classes->SetCellState(period % m_owner->m_school->n_periods_per_day, period / m_owner->m_school->n_periods_per_day, teacher_i);
			}
		}
	}
}

void ListTimetablesPane::OnTeacherSelectionChaged(wxCommandEvent &){
	School * school = m_owner->m_school;
	if(m_teacher_picker->GetSelection() != wxNOT_FOUND){
		int i_sol_select = m_timetables_list->GetList()->GetSelection();
		int solution_id = ((IntClientData*)m_timetables_list->GetList()->GetClientObject(i_sol_select))->m_value;
		int teacher_id = ((IntClientData*)m_teacher_picker->GetClientObject(m_teacher_picker->GetSelection()))->m_value;
		Solution * sol = find_solution_by_id(m_owner->m_school, solution_id);

		m_teachers->SetAllCellsState(-1);
		for(int i = 0; i < sol->n_meetings; ++i){
			int period = sol->meetings[i].period;
			if(teacher_id == sol->meetings[i].teacher->id){
				if(sol->meetings[i].type == meet_LECTURE){
					int class_i = get_class_index_by_id(school, sol->meetings[i].m_class->id);
					m_teachers->SetCellState(period % m_owner->m_school->n_periods_per_day, period / m_owner->m_school->n_periods_per_day, class_i);
				} else if(sol->meetings[i].type == meet_PLANNING){
					m_teachers->SetCellState(period % m_owner->m_school->n_periods_per_day, period / m_owner->m_school->n_periods_per_day, school->n_classes);
				} else {
					m_teachers->SetCellState(period % m_owner->m_school->n_periods_per_day, period / m_owner->m_school->n_periods_per_day, school->n_classes + 1);
				}
			}
		}
	}
}

void ListTimetablesPane::OnRoomSelectionChaged(wxCommandEvent &){
	School * school = m_owner->m_school;
	if(m_room_picker->GetSelection() != wxNOT_FOUND){
		int i_sol_select = m_timetables_list->GetList()->GetSelection();
		int solution_id = ((IntClientData*)m_timetables_list->GetList()->GetClientObject(i_sol_select))->m_value;
		int room_id = ((IntClientData*)m_room_picker->GetClientObject(m_room_picker->GetSelection()))->m_value;
		Solution * sol = find_solution_by_id(m_owner->m_school, solution_id);

		m_rooms->SetAllCellsState(-1);
		for(int i = 0; i < sol->n_meetings; ++i){
			int period = sol->meetings[i].period;
			if(room_id == sol->meetings[i].room->id){
				if(sol->meetings[i].type == meet_LECTURE){
					int class_i = get_class_index_by_id(school, sol->meetings[i].m_class->id);
					m_rooms->SetCellState(period % m_owner->m_school->n_periods_per_day, period / m_owner->m_school->n_periods_per_day, class_i);
				} else if(sol->meetings[i].type == meet_PLANNING){
					int teacher_i = get_teacher_index_by_id(school, sol->meetings[i].teacher->id);
					m_rooms->SetCellState(period % m_owner->m_school->n_periods_per_day, period / m_owner->m_school->n_periods_per_day, school->n_classes + 1 + teacher_i);
				} else {
					m_rooms->SetCellState(period % m_owner->m_school->n_periods_per_day, period / m_owner->m_school->n_periods_per_day, school->n_classes);
				}
			}
		}
	}
}

void ListTimetablesPane::OnSelectionChanged(wxCommandEvent &){
	int i_select = m_timetables_list->GetList()->GetSelection();
	if(i_select != wxNOT_FOUND) {
		int solution_id = ((IntClientData*)m_timetables_list->GetList()->GetClientObject(i_select))->m_value;
		Solution * sol = find_solution_by_id(m_owner->m_school, solution_id);
		m_name_text->SetValue(wxString::FromUTF8(sol->name));
		m_desc_text->SetValue(wxString::FromUTF8(sol->desc));
		m_date_text->SetLabel(wxString::FromUTF8(sol->gen_date));

		m_class_picker->Enable(true);
		m_teacher_picker->Enable(true);
		m_room_picker->Enable(true);
	} else {
		m_class_picker->Enable(false);
		m_teacher_picker->Enable(false);
		m_room_picker->Enable(false);
	}
}

void ListTimetablesPane::ShowData(){
	School * school = m_owner->m_school;
	m_name_text->SetValue(wxT(""));
	m_date_text->SetLabel(wxT("-- / -- / --"));
	m_timetables_list->Clear();
	for(int i = 0; i < school->n_solutions; ++i){
		m_timetables_list->AddItem(school->solutions[i].id, wxString::FromUTF8(school->solutions[i].name));
	}

	m_teachers->GridRemake(school->n_periods_per_day, school->n_days);
	m_rooms->GridRemake(school->n_periods_per_day, school->n_days);
	m_classes->GridRemake(school->n_periods_per_day, school->n_days);

	/* Rooms Code */
	/* Teachers Code */
	for(int i = 0; i < school->n_classes; ++i){
		m_teachers->AddState(wxString::FromUTF8(school->classes[i].name), wxColor(255,255,255));
		m_rooms->AddState(wxString::FromUTF8(school->classes[i].name), wxColor(255,255,255));
	}
	m_rooms->AddState(wxT("Empty"), wxColor(255,200,200));
	for(int i = 0; i < school->n_teachers; ++i){
		m_rooms->AddState(wxString::FromUTF8(school->teachers[i].name), wxColor(255,255,255));
	}
	m_teachers->AddState(wxT("Planning"), wxColor(255,255,255));
	m_teachers->AddState(wxT("Free"), wxColor(255,255,255));

	/* Classes Code */
	for(int i = 0; i < school->n_subjects; ++i){
		m_classes->AddState(wxString::FromUTF8(school->subjects[i].name), wxColor(255,255,255));
	}
	m_classes->AddState(wxT("Free"), wxColor(255,255,255));

	for(int i = 0; i < school->n_days; ++i){
		m_classes->SetColLabel(i, wxString::FromUTF8(school->day_names[i]));
		m_teachers->SetColLabel(i, wxString::FromUTF8(school->day_names[i]));
		m_rooms->SetColLabel(i, wxString::FromUTF8(school->day_names[i]));
	}
	for(int i = 0; i < school->n_periods_per_day; ++i){
		m_classes->SetRowLabel(i, wxString::FromUTF8(school->daily_period_names[i]));
		m_teachers->SetRowLabel(i, wxString::FromUTF8(school->daily_period_names[i]));
		m_rooms->SetRowLabel(i, wxString::FromUTF8(school->daily_period_names[i]));
	}
	m_teachers->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);
	m_classes->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);
	m_rooms->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);
	m_teachers->SetCanUserClick(false);
	m_classes->SetCanUserClick(false);
	m_rooms->SetCanUserClick(false);
	m_rooms->SetAllCellsState(-1);
	m_teachers->SetAllCellsState(-1);
	m_classes->SetAllCellsState(-1);

	for(int i = 0; i < school->n_classes; ++i){
		m_class_picker->Append(wxString::FromUTF8(school->classes[i].name), new IntClientData(school->classes[i].id));
	}
	for(int i = 0; i < school->n_teachers; ++i){
		m_teacher_picker->Append(wxString::FromUTF8(school->teachers[i].name), new IntClientData(school->teachers[i].id));
	}
	for(int i = 0; i < school->n_rooms; ++i){
		m_room_picker->Append(wxString::FromUTF8(school->rooms[i].name), new IntClientData(school->rooms[i].id));
	}
}

ListTimetablesPane::~ListTimetablesPane(){

}

void ListTimetablesPane::OnDataChange(wxNotifyEvent & ){
	ShowData();
}
