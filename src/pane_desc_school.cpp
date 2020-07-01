#include "gui.hpp"

DescSchoolPane::DescSchoolPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));
	SetFont(*m_owner->m_text_font);


	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_school_details, wxDefaultPosition, wxSize(300,25));

	wxStaticText * school_name_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_school_name, wxDefaultPosition, wxDefaultSize);
	wxStaticText * school_name_text = new wxStaticText(this, wxID_ANY, wxString::FromUTF8(school->name), wxDefaultPosition, wxDefaultSize);
	wxStaticText * school_nppd_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_number_of_periods_per_day, wxDefaultPosition, wxDefaultSize);
	wxStaticText * school_nppd_text = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%d"),school->n_periods_per_day), wxDefaultPosition, wxDefaultSize);
	wxStaticText * school_nday_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_number_of_days_per_cycle, wxDefaultPosition,wxDefaultSize);
	wxStaticText * school_nday_text = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%d"),school->n_days), wxDefaultPosition,wxDefaultSize);

	wxButton * delete_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_delete);

	title->SetFont(*m_owner->m_page_title_font);

	school_name_label->SetFont(*m_owner->m_bold_text_font);
	school_nppd_label->SetFont(*m_owner->m_bold_text_font);
	school_nday_label->SetFont(*m_owner->m_bold_text_font);

	wxGridSizer * text_sz = new wxGridSizer(2,10,10);

	text_sz->Add(school_name_label,1,wxEXPAND);
	text_sz->Add(school_name_text,1,wxEXPAND);
	text_sz->Add(school_nppd_label,1,wxEXPAND);
	text_sz->Add(school_nppd_text,1,wxEXPAND);
	text_sz->Add(school_nday_label,1,wxEXPAND);
	text_sz->Add(school_nday_text,1,wxEXPAND);

	wxStaticText * grid_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_periods, wxDefaultPosition, wxSize(250,15));
	grid_label->SetFont(*m_owner->m_small_font);

	ChoiceGrid * m_grid = new ChoiceGrid(this, wxID_ANY, wxDefaultPosition, wxSize(500,200));

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(m_owner->m_lang->str_adj__open);
	grid_values.push_back(m_owner->m_lang->str_adj__closed);
	m_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200));
	m_grid->SetBackgroundColors(grid_colors);

	m_grid->m_basic_col_name = m_owner->m_lang->str_day;
	m_grid->m_basic_row_name = m_owner->m_lang->str_period;

	m_grid->GridRemake(m_owner->m_school->n_days,m_owner->m_school->n_periods_per_day);

	for(int i = 0; i < school->n_periods; ++i){
		if(school->periods[i] == false){
			m_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), m_owner->m_lang->str_adj__closed);
			m_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),wxColor(255,200,200));
		} else {
			m_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), m_owner->m_lang->str_adj__open);
			m_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),wxColor(200,200,255));
		}
		m_grid->SetReadOnly(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), true);
	}

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * content_sz = new wxBoxSizer(wxVERTICAL);

	sizer->Add(content_sz, 0,  wxALL, 15);
	content_sz->Add(title, 0, wxALL, 15);
	content_sz->Add(text_sz, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 15);
	content_sz->Add(grid_label, 0, wxLEFT | wxRIGHT, 15);
	content_sz->Add(m_grid, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);
	content_sz->Add(delete_button, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);

	delete_button->Bind(wxEVT_BUTTON, &DescSchoolPane::OnRemoveButtonClicked, this);
}

void DescSchoolPane::OnRemoveButtonClicked(wxCommandEvent & ){
	wxMessageDialog * dialog = new wxMessageDialog(nullptr, m_owner->m_lang->str_are_you_sure, m_owner->m_lang->str_school_deletion_popup_text, wxCANCEL | wxOK);
	dialog->SetYesNoLabels(m_owner->m_lang->str_yes, m_owner->m_lang->str_no);
	int confirmation = dialog->ShowModal();
	if(confirmation == wxOK){
		printf("To delete school\n");
	}
}

DescSchoolPane::~DescSchoolPane(){

}
