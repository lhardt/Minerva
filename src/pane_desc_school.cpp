#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "util.h"
};

DescSchoolPane::DescSchoolPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));
	SetFont(*m_owner->m_text_font);

	wxStaticText * school_name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_school_name, wxDefaultPosition, wxDefaultSize);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxString::FromUTF8(school->name), wxDefaultPosition, wxDefaultSize);
	wxStaticText * school_nppd_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_number_of_periods_per_day, wxDefaultPosition, wxDefaultSize);
	wxStaticText * school_nppd_text = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%d"),school->n_periods_per_day), wxDefaultPosition, wxDefaultSize);
	wxStaticText * school_nday_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_number_of_days_per_cycle, wxDefaultPosition,wxDefaultSize);
	wxStaticText * school_nday_text = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%d"),school->n_days), wxDefaultPosition,wxDefaultSize);
	wxButton 	 * delete_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_delete);
	m_edit_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_cancel_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	wxButton 	 * duplicate_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_duplicate);
	m_notebook = new wxNotebook(this, wxID_ANY);

	m_name_action = new SchoolNameUpdateAction(owner, copy_string(owner->m_school->name));
	m_name_text->Disable();

	wxVector<wxString> daily_period_row_names = wxVector<wxString>();
	for(i = 0; i < school->n_periods_per_day; ++i){
		daily_period_row_names.push_back(wxString::Format("%s %d", m_owner->m_lang->str_period, 1+i));
	}

	wxVector<wxString> day_row_names = wxVector<wxString>();
	for(i = 0; i < school->n_days; ++i){
		day_row_names.push_back(wxString::Format("%s %d", m_owner->m_lang->str_day, 1+i));
	}

	wxVector<wxString> per_row_names = wxVector<wxString>();
	for(i = 0; i < school->n_periods; ++i){
		per_row_names.push_back(wxString::Format("%s %d", m_owner->m_lang->str_period, 1+i));
	}

	StringGridPane * m_days = new StringGridPane(m_owner,m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_owner->m_lang->str_name, day_row_names);
	StringGridPane * m_daily_periods = new StringGridPane(m_owner,m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_owner->m_lang->str_name, daily_period_row_names);
	StringGridPane * m_period_names = new StringGridPane(m_owner,m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_owner->m_lang->str_name, per_row_names);
	ScoreGridPane * m_periods = new ScoreGridPane(m_owner, m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_notebook->AddPage(m_periods, m_owner->m_lang->str_periods);
	m_notebook->AddPage(m_days, m_owner->m_lang->str_day_names);
	m_notebook->AddPage(m_daily_periods, m_owner->m_lang->str_daily_period_names);
	m_notebook->AddPage(m_period_names, m_owner->m_lang->str_period_names);
	m_notebook->AddPage(MakeStatisticsPane(), m_owner->m_lang->str_statistics);

	wxGrid * day_names_grid = m_days->GetGrid();
	day_names_grid->SetColSize(0, 200);
	for(i = 0; i < school->n_days; ++i){
		day_names_grid->SetCellValue(i,0, wxString::FromUTF8(school->day_names[i]));
	}

	wxGrid * daily_period_names_grid = m_daily_periods->GetGrid();
	daily_period_names_grid->SetColSize(0, 200);
	for(i = 0; i < school->n_periods_per_day; ++i){
		daily_period_names_grid->SetCellValue(i,0, wxString::FromUTF8(school->daily_period_names[i]));
	}

	wxGrid * per_names_grid = m_period_names->GetGrid();
	per_names_grid->SetColSize(0, 200);
	for(i = 0; i < school->n_periods; ++i){
		per_names_grid->SetCellValue(i,0, wxString::FromUTF8(school->period_names[i]));
	}

	ChoiceGrid * periods_grid = m_periods->GetGrid();
	periods_grid->SetCanUserClick(false);
	periods_grid->AddState(m_owner->m_lang->str_adj__closed, wxColor(255,200,200));
	periods_grid->AddState(m_owner->m_lang->str_adj__open, wxColor(200,200,255));

	periods_grid->m_basic_col_name = m_owner->m_lang->str_day;
	periods_grid->m_basic_row_name = m_owner->m_lang->str_period;

	periods_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);

	for(int i = 0; i < school->n_periods; ++i){
		periods_grid->SetCellState(i % school->n_periods_per_day, i / school->n_periods_per_day, school->periods[i]?1:0);
	}

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * content_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * button_sz = new wxStaticBoxSizer(wxHORIZONTAL, this, m_owner->m_lang->str_actions);
	wxStaticBoxSizer * text_wrap = new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_basic_data);
	wxGridSizer * text_sz = new wxGridSizer(4,5,5);

	text_wrap->Add(text_sz, 1, wxEXPAND | wxALL, 5);

	sizer->Add(content_sz, 0, wxEXPAND| wxALL, 15);
	content_sz->Add(text_wrap, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 15);
	content_sz->Add(m_notebook, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 15);
	content_sz->Add(button_sz, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);

	text_sz->Add(school_name_label,1,wxEXPAND);
	text_sz->Add(m_name_text,1,wxEXPAND);
	text_sz->AddStretchSpacer();
	text_sz->AddStretchSpacer();
	text_sz->Add(school_nppd_label,1,wxEXPAND);
	text_sz->Add(school_nppd_text,1,wxEXPAND);
	text_sz->AddStretchSpacer();
	text_sz->AddStretchSpacer();
	text_sz->Add(school_nday_label,1,wxEXPAND);
	text_sz->Add(school_nday_text,1,wxEXPAND);
	text_sz->Add(m_cancel_button,1,wxEXPAND);
	text_sz->Add(m_edit_button,1,wxEXPAND);

	button_sz->Add(delete_button, 0, wxALL, 15);
	button_sz->Add(duplicate_button, 0, wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_cancel_button->Hide();

	delete_button->Bind(wxEVT_BUTTON, &DescSchoolPane::OnRemoveButtonClicked, this);
	m_edit_button->Bind(wxEVT_BUTTON, &DescSchoolPane::OnEditButtonClicked, this);
	m_cancel_button->Bind(wxEVT_BUTTON, &DescSchoolPane::OnCancelButtonClicked, this);
}

wxScrolledWindow* DescSchoolPane::MakeStatisticsPane(){
	School * school = m_owner->m_school;
	wxScrolledWindow * wnd = new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	wxString string = "";
	string << m_owner->m_lang->str_number_of_classes << ": " << school->n_classes << "\n";
	string << m_owner->m_lang->str_number_of_teachers << ": " << school->n_teachers << "\n";
	string << m_owner->m_lang->str_number_of_rooms << ": " << school->n_rooms << "\n";
	string << m_owner->m_lang->str_number_of_subjects << ": " << school->n_subjects << "\n";
	string << m_owner->m_lang->str_number_of_subject_groups << ": " << school->n_subject_groups << "\n";
	// TODO complement information
	wxStaticText * text = new wxStaticText(wnd, wxID_ANY, string, wxDefaultPosition, wxDefaultSize);

	text->SetLabel(string);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(text, 0, wxALL, 15);
	wnd->SetSizer(sizer);
	return wnd;
}

void DescSchoolPane::OnEditButtonClicked(wxCommandEvent & ){
	if(m_name_text->IsEnabled()){
		m_cancel_button->Hide();
		m_name_text->Disable();
		m_edit_button->SetLabel((m_owner->m_lang->str_edit));
		free(m_name_action->m_name);
		m_name_action->m_name = copy_wx_string(m_name_text->GetValue());
		m_owner->Do(m_name_action);
	} else {
		m_cancel_button->Show();
		m_name_text->Enable();
		m_edit_button->SetLabel((m_owner->m_lang->str_save));
	}
}
void DescSchoolPane::OnCancelButtonClicked(wxCommandEvent & ){
	m_cancel_button->Hide();
	m_name_text->Disable();
	m_edit_button->SetLabel((m_owner->m_lang->str_edit));
	m_name_text->SetValue(wxString::FromUTF8(m_name_action->m_name));
	free(m_name_action->m_name);
	m_name_action->m_name = copy_string(m_owner->m_school->name);
}

void DescSchoolPane::OnRemoveButtonClicked(wxCommandEvent & ){
	wxMessageDialog * dialog = new wxMessageDialog(nullptr, m_owner->m_lang->str_school_deletion_popup_text, m_owner->m_lang->str_are_you_sure, wxYES | wxNO);
	dialog->SetYesNoLabels(m_owner->m_lang->str_yes, m_owner->m_lang->str_no);
	int confirmation = dialog->ShowModal();
	if(confirmation == wxID_YES){
		bool success = remove_school(stdout, m_owner->m_database, m_owner->m_school->id);
		if(success){
			free_school(m_owner->m_school);
			m_owner->m_school = nullptr;
			m_owner->SwitchForm(FORM_WELCOME);
			m_owner->SaveDatabase();
			Destroy();
		}
	}
}

DescSchoolPane::~DescSchoolPane(){

}
