#include "gui.hpp"

ListPlanningTimesPane::ListPlanningTimesPane(Application * owner, wxWindow * parent, wxPoint pos)  : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * teacher_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teacher);
	wxStaticText * occurence_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_occurence);

	teacher_label->SetFont(*m_owner->m_small_font);
	occurence_label->SetFont(*m_owner->m_small_font);

	wxStaticText * needs_room_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_needs_room);
	wxStaticText * room_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_room);
	wxStaticText * period_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_period);

	m_teacher_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	m_occurence_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	m_needs_room_text = new wxCheckBox(this, wxID_ANY, wxT(""));
	m_room_choice = new wxChoice(this, wxID_ANY);
	m_period_choice = new wxChoice(this, wxID_ANY);
	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);

	wxNotebook * notebook = new wxNotebook(this, wxID_ANY);
	m_periods = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	m_rooms = new ScoreGridPane(m_owner, notebook, wxID_ANY);
	notebook->InsertPage(0, m_periods, m_owner->m_lang->str_periods);
	notebook->InsertPage(1, m_rooms, m_owner->m_lang->str_rooms);

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * left_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * right_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * fields_wrap = new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_data_to_fix);
	wxSizer * fields_sz = new wxGridSizer(4,5,5);

	left_sz->Add(teacher_label);
	left_sz->Add(m_teacher_choice, 0, wxBOTTOM, 10);
	left_sz->Add(occurence_label);
	left_sz->Add(m_occurence_choice, 0, wxBOTTOM, 10);

	fields_sz->Add(room_label);
	fields_sz->Add(m_room_choice);
	fields_sz->Add(period_label);
	fields_sz->Add(m_period_choice);
	fields_sz->Add(needs_room_label);
	fields_sz->Add(m_needs_room_text);
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->AddStretchSpacer();
	fields_sz->Add(m_cancel_btn);
	fields_sz->Add(m_edit_btn);

	fields_wrap->Add(fields_sz, 0, wxALL, 5);
	right_sz->Add(fields_wrap, 0, wxEXPAND | wxBOTTOM, 10);
	right_sz->Add(notebook, 0, wxEXPAND, 10);

	sizer->Add(left_sz, 0, wxALL, 15);
	sizer->Add(right_sz, 1, wxALL, 15);

	this->SetSizer(sizer);
	Layout();

	m_teacher_choice->Bind(wxEVT_CHOICE, &ListPlanningTimesPane::OnTeacherSelectionChanged, this);
	m_occurence_choice->Bind(wxEVT_CHOICE, &ListPlanningTimesPane::OnOccurenceSelectionChanged, this);
	m_edit_btn->Bind(wxEVT_BUTTON, &ListPlanningTimesPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &ListPlanningTimesPane::OnCancelButtonClicked, this);
	Bind(DATA_CHANGE_EVENT, &ListPlanningTimesPane::OnDataChange, this);
}

void ListPlanningTimesPane::OnDataChange(wxNotifyEvent & evt) {
	printf("Data change!\n");
}

void ListPlanningTimesPane::OnTeacherSelectionChanged(wxCommandEvent &) {

}

void ListPlanningTimesPane::OnOccurenceSelectionChanged(wxCommandEvent &) {

}

void ListPlanningTimesPane::OnEditButtonClicked(wxCommandEvent &) {

}

void ListPlanningTimesPane::OnCancelButtonClicked(wxCommandEvent &){

}

ListPlanningTimesPane::~ListPlanningTimesPane(){

}
