#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

ListLecturesPane::ListLecturesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * class_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class);
	wxStaticText * subject_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_subject);
	wxStaticText * occurence_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_occurence);
	wxStaticText * teacher_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teacher);
	wxStaticText * room_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_room);
	wxStaticText * period_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_period);

	wxNotebook 	  * notebook = new wxNotebook(this, wxID_ANY);
	ScoreGridPane * m_periods = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	ScoreGridPane * m_teachers = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	ScoreGridPane * m_rooms = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	notebook->InsertPage(0, m_periods, m_owner->m_lang->str_periods);
	notebook->InsertPage(1, m_teachers, m_owner->m_lang->str_teachers);
	notebook->InsertPage(2, m_rooms, m_owner->m_lang->str_rooms);

	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);

	class_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	subject_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	occurence_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));

	wxChoice * teacher_text = new wxChoice(this, wxID_ANY);
	wxChoice * room_text = new wxChoice(this, wxID_ANY);
	wxChoice * period_text = new wxChoice(this, wxID_ANY);

	class_label->SetFont(*m_owner->m_small_font);
	subject_label->SetFont(*m_owner->m_small_font);
	occurence_label->SetFont(*m_owner->m_small_font);

	wxSizer * sizer   = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * left_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * right_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * fields_wrap = new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_data_to_fix);
	wxSizer * fields_sz = new wxGridSizer(4,5,5);

	left_sz->Add(class_label, 0, wxBOTTOM, 5);
	left_sz->Add(class_choice, 0, wxBOTTOM, 10);
	left_sz->Add(subject_label, 0, wxBOTTOM, 5);
	left_sz->Add(subject_choice, 0, wxBOTTOM, 10);
	left_sz->Add(occurence_label, 0, wxBOTTOM, 5);
	left_sz->Add(occurence_choice, 0, wxBOTTOM, 10);

	fields_sz->Add(teacher_label);
	fields_sz->Add(teacher_text);
	fields_sz->Add(room_label);
	fields_sz->Add(room_text);
	fields_sz->Add(period_label);
	fields_sz->Add(period_text);
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->Add(m_cancel_btn, 0, wxEXPAND);
	fields_sz->Add(m_edit_btn, 0, wxEXPAND);

	fields_wrap->Add(fields_sz, 0, wxEXPAND | wxALL, 10);
	right_sz->Add(fields_wrap, 0, wxEXPAND | wxBOTTOM, 10);
	right_sz->Add(notebook, 1, wxEXPAND | wxBOTTOM, 10);

	sizer->Add(left_sz, 0, wxALL, 10);
	sizer->Add(right_sz, 0, wxRIGHT | wxTOP | wxBOTTOM, 10);

	this->SetSizer(sizer);

	for(i = 0; i < school->n_classes; ++i){
		class_choice->Insert(wxString::FromUTF8(school->classes[i].name), i, new IntClientData(i));
	}

	m_cancel_btn->Hide();

	class_choice->Bind(wxEVT_CHOICE, &ListLecturesPane::OnClassSelectionChanged, this);
	subject_choice->Bind(wxEVT_CHOICE, &ListLecturesPane::OnSubjectSelectionChanged, this);
	m_edit_btn->Bind(wxEVT_CHOICE, &ListLecturesPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_CHOICE, &ListLecturesPane::OnCancelButtonClicked, this);
	Bind(DATA_CHANGE_EVENT, &ListLecturesPane::OnDataChange, this);

	// m_periods code
	ChoiceGrid * periods_grid = m_periods->GetGrid();
	periods_grid->AddState(m_owner->m_lang->str_adj__open, wxColor(200,200,255));
	periods_grid->AddState(m_owner->m_lang->str_adj__closed, wxColor(255,200,200));
	periods_grid->m_basic_col_name = m_owner->m_lang->str_day;
	periods_grid->m_basic_row_name = m_owner->m_lang->str_period;
	periods_grid->GridRemake(school->n_days, school->n_periods_per_day);

	Layout();
}

void ListLecturesPane::OnDataChange(wxNotifyEvent & evt) {
	printf("Data change!\n");
}

void ListLecturesPane::OnCancelButtonClicked(wxCommandEvent&){

}

void ListLecturesPane::OnEditButtonClicked(wxCommandEvent&){
	if(m_cancel_btn->IsShown()){
		m_cancel_btn->Hide();
		m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	} else {
		m_cancel_btn->Show();
		m_edit_btn->SetLabel(m_owner->m_lang->str_save);
	}
}


void ListLecturesPane::OnClassSelectionChanged(wxCommandEvent & ev){
	int n_inserted = 0, i = 0, i_class;
	School * school = m_owner->m_school;
	printf("event\n");
	subject_choice->Clear();
	occurence_choice->Clear();
	if(class_choice->GetSelection() != wxNOT_FOUND){
		i_class = ((IntClientData*)class_choice->GetClientObject(class_choice->GetSelection()))->m_value;
		for(i = 0; i < school->n_assignments; ++i){
			if(school->assignments[i].m_class->id == school->classes[i_class].id){
				subject_choice->Insert(wxString::FromUTF8(school->assignments[i].subject->name), n_inserted, new IntClientData(i));
				++n_inserted;
			}
		}
	}
}

void ListLecturesPane::OnSubjectSelectionChanged(wxCommandEvent & ev){
	int i;
	School * school = m_owner->m_school;
	occurence_choice->Clear();
	if(subject_choice->GetSelection() != wxNOT_FOUND){
		for(i = 0; i < school->assignments[i].amount; ++i){
			// TODO  check if i > 26
			occurence_choice->Insert(wxString::Format(wxT("%C"), 'A' + i), i);
		}
	}
}

ListLecturesPane::~ListLecturesPane(){

}
