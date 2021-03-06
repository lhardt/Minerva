#include "gui.hpp"

#include <iostream>

#include <wx/grid.h>
#include <wx/spinctrl.h>

extern "C"{
	#include "loader.h"
}

CreateSchoolForm::CreateSchoolForm(Application * owner)  : wxFrame(nullptr, wxID_ANY, owner->m_lang->str_minerva_school_timetables, wxPoint(30,30), wxSize(800,600)){
	m_owner = owner;
	SetFont(*m_owner->m_text_font);

	#ifdef __WXMSW__
		SetIcon(wxICON(aaaaaaaa));
	#endif

	SetBackgroundColour(wxColour(250,250,250));

	wxScrolledWindow * m_right_pane = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(350,-1));
	m_right_pane->SetBackgroundColour(wxColor(0x29, 0x80, 0xb9));

	wxPanel * m_left_pane = new wxPanel(this, wxID_ANY, wxDefaultPosition);

	wxPanel * m_footer = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_footer->SetBackgroundColour(wxColor(0x25,0x75,0xb0));

	wxStaticText * m_footer_text = new wxStaticText(m_footer, wxID_ANY,m_owner->m_lang->str_copyright_notice, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE | wxALIGN_CENTRE_HORIZONTAL);
	m_footer_text->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));

	wxStaticText * m_title = new wxStaticText(m_left_pane, wxID_ANY, m_owner->m_lang->str_create_school);

	wxStaticText * m_school_name_label = new wxStaticText(m_left_pane, wxID_ANY, m_owner->m_lang->str_school_name);
	m_school_name_text = new wxTextCtrl(m_left_pane, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,-1));

	wxStaticText * m_number_of_days_label = new wxStaticText(m_left_pane, wxID_ANY, m_owner->m_lang->str_number_of_days_per_cycle);
	m_number_of_days_text = new wxSpinCtrl(m_left_pane, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,-1));

	wxStaticText * m_number_of_periods_label = new wxStaticText(m_left_pane, wxID_ANY, m_owner->m_lang->str_number_of_periods_per_day);
	m_number_of_periods_text = new wxSpinCtrl(m_left_pane, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,-1));

	wxStaticText * m_names_label = new wxStaticText(m_left_pane, wxID_ANY, m_owner->m_lang->str_in_what_periods_the_school_is_open);
	m_names_label->SetFont(*m_owner->m_small_font);

	m_grid = new ChoiceGrid(m_owner,m_left_pane, wxID_ANY, wxDefaultPosition, wxSize(300,200));
	m_grid->AddState(m_owner->m_lang->str_adj__open, wxColor(200,200,255));
	m_grid->AddState(m_owner->m_lang->str_adj__closed, wxColor(255,200,200));

	m_grid->SetDefaultColumnLabel(m_owner->m_lang->str_day);
	m_grid->SetDefaultRowLabel(m_owner->m_lang->str_period);

	m_number_of_periods_text->SetValue(2);
	m_number_of_days_text->SetValue(3);
	m_grid->GridRemake(3, 2);

	m_err_msg = new wxStaticText(m_left_pane, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
	m_err_msg->SetFont(*m_owner->m_small_font);
	m_err_msg->SetForegroundColour(wxColor(0xF0, 0x50, 0x50));

	m_button_create = new wxButton(m_left_pane, wxID_ANY, m_owner->m_lang->str_create, wxDefaultPosition, wxSize(80,30));
	m_button_back = new wxButton(m_left_pane, wxID_ANY, m_owner->m_lang->str_back, wxDefaultPosition, wxSize(80,30));

	m_button_create->Bind(wxEVT_BUTTON, &CreateSchoolForm::OnCreateClicked, this);
	m_button_back->Bind(wxEVT_BUTTON, &CreateSchoolForm::OnBackClicked, this);

	wxStaticText * m_tutorial_text = new wxStaticText(m_right_pane, wxID_ANY, m_owner->m_lang->str_create_school_manual_entry, wxDefaultPosition, wxSize(290,-1));
	m_tutorial_text->SetForegroundColour(wxColor(255,255,255));
	m_tutorial_text->Wrap(m_tutorial_text->GetClientSize().GetWidth());

	m_school_name_label->SetFont(*m_owner->m_small_font);
	m_title->SetFont(*m_owner->m_page_title_font);
	m_footer_text->SetFont(*m_owner->m_small_font);
	m_number_of_days_label->SetFont(*m_owner->m_small_font);
	m_number_of_periods_label->SetFont(*m_owner->m_small_font);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * sizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * left_pane_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * button_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * right_pane_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * footer_sz = new wxBoxSizer(wxVERTICAL);

	button_sz->Add(m_button_back, 0, wxRIGHT, 5);
	button_sz->Add(m_button_create, 0, 0);

	left_pane_sz->Add(m_title, 0, wxBOTTOM, 15);
	left_pane_sz->Add(m_school_name_label, 0, wxBOTTOM, 5);
	left_pane_sz->Add(m_school_name_text, 0, wxBOTTOM, 15);
	left_pane_sz->Add(m_number_of_days_label, 0, wxBOTTOM, 5);
	left_pane_sz->Add(m_number_of_days_text, 0, wxBOTTOM, 15);
	left_pane_sz->Add(m_number_of_periods_label, 0, wxBOTTOM, 5);
	left_pane_sz->Add(m_number_of_periods_text, 0, wxBOTTOM, 15);
	left_pane_sz->Add(m_names_label, 0, wxBOTTOM, 5);
	left_pane_sz->Add(m_grid, 0, wxEXPAND | wxBOTTOM, 15);
	left_pane_sz->Add(button_sz, 0, wxALIGN_RIGHT);
	left_pane_sz->Add(m_err_msg, 0,  wxLEFT | wxRIGHT, 15);
	m_left_pane->SetSizer(left_pane_sz);

	right_pane_sz->Add(m_tutorial_text, 1, wxALL, 20);
	m_right_pane->SetSizer(right_pane_sz);

	sizer2->Add(m_left_pane, 1, wxEXPAND | wxALL, 30);
	sizer2->Add(m_right_pane, 0, wxEXPAND);

	footer_sz->Add(m_footer_text, 1, wxEXPAND | wxALL, 2);
	m_footer->SetSizer(footer_sz);

	sizer->Add(sizer2, 1, wxEXPAND);
	sizer->Add(m_footer, 0, wxEXPAND);

	m_right_pane->SetScrollRate(5,5);
	m_right_pane->ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	m_right_pane->Layout();
	SetSizerAndFit(sizer);
	sizer->SetMinSize(800,600);

	m_number_of_periods_text->Bind(wxEVT_SPINCTRL, &CreateSchoolForm::OnGridSizeUpdated, this);
	m_number_of_days_text->Bind(wxEVT_SPINCTRL, &CreateSchoolForm::OnGridSizeUpdated, this);
}

CreateSchoolForm::~CreateSchoolForm(){

}

void CreateSchoolForm::OnGridSizeUpdated(wxSpinEvent & ev){
	m_grid->GridRemake(m_number_of_days_text->GetValue(),m_number_of_periods_text->GetValue());
	Layout();
}

void CreateSchoolForm::OnBackClicked(wxCommandEvent & ev){
	m_owner->SwitchForm(FORM_WELCOME);
}

void CreateSchoolForm::OnCreateClicked(wxCommandEvent & ev){
	int i, str_sz;
	if(!m_school_name_text->IsEmpty() && m_number_of_periods_text->GetValue() > 0 && m_number_of_days_text->GetValue() > 0 ){
		School * school = new School();
		str_sz = strlen(m_school_name_text->GetValue().ToUTF8().data());
		school->name = (char *) calloc(str_sz + 1, sizeof(char));
		strncpy(school->name, m_school_name_text->GetValue().ToUTF8().data(), str_sz);

		school->n_days = m_number_of_days_text->GetValue();
		school->n_periods_per_day = m_number_of_periods_text->GetValue();

		school->day_names = (char **) calloc(school->n_days, sizeof(char*));
		for(i = 0; i < school->n_days; ++i){
			school->day_names[i] = copy_wx_string(m_grid->GetColLabelValue(i));
		}
		school->daily_period_names = (char**) calloc(school->n_periods_per_day, sizeof(char*));
		for(i = 0; i < school->n_periods_per_day; ++i){
			school->daily_period_names[i] = copy_wx_string(m_grid->GetRowLabelValue(i));
		}
		school->n_periods = school->n_days * school->n_periods_per_day;
		school->period_names = (char**) calloc( school->n_periods + 1, sizeof(char*));
		school->periods = (int*) calloc( school->n_periods + 1, sizeof(int));
		for(i = 0; i < school->n_periods; ++i){
			school->period_names[i] = (char*)calloc(1,sizeof(char));
			// Open is state 0, Closed is state 1;
			school->periods[i] = 1 - m_grid->GetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day);
		}

		int errc = insert_school(stdout, m_owner->m_database, school);

		if(errc != -1){
			m_owner->m_school = school;
			m_owner->SwitchForm(FORM_MAIN_MENU);
			m_owner->SaveDatabase();
			m_owner->NotifyNewUnsavedData();
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}

}
