#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

LecturePeriodPrefPane::LecturePeriodPrefPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));
	School * school = m_owner->m_school;

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_period_preference, wxDefaultPosition, wxSize(300,30));
	wxStaticText * class_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class);
	wxStaticText * subj_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class);
	wxStaticText * occurence_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class);
	wxStaticText * periods_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_periods, wxDefaultPosition, wxSize(300,30));

	m_class_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	m_subj_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	m_occurence_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	m_periods_grid = new ChoiceGrid(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));
	wxButton * save_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_save);
	wxButton * back_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_back);

	title->SetFont(*m_owner->m_page_title_font);
	class_label->SetFont(*m_owner->m_small_font);
	subj_label->SetFont(*m_owner->m_small_font);
	occurence_label->SetFont(*m_owner->m_small_font);

	wxVector<wxString> possible_values;
	possible_values.push_back(m_owner->m_lang->str_score_0);
	possible_values.push_back(m_owner->m_lang->str_score_1);
	possible_values.push_back(m_owner->m_lang->str_score_2);
	possible_values.push_back(m_owner->m_lang->str_score_3);
	possible_values.push_back(m_owner->m_lang->str_score_4);

	wxVector<wxColor> possible_backgrounds;
	possible_backgrounds.push_back(wxColor(0xff,0xab,0xab));
	possible_backgrounds.push_back(wxColor(0xff,0xbe,0xbc));
	possible_backgrounds.push_back(wxColor(0xff,0xf5,0xba));
	possible_backgrounds.push_back(wxColor(0xac,0xe7,0xff));
	possible_backgrounds.push_back(wxColor(0x85,0xe3,0xff));

	m_periods_grid->SetPossibleValues(possible_values);
	m_periods_grid->SetBackgroundColors(possible_backgrounds);

	m_periods_grid->m_basic_col_name = m_owner->m_lang->str_day;
	m_periods_grid->m_basic_row_name = m_owner->m_lang->str_period;

	m_periods_grid->GridRemake(school->n_days,school->n_periods_per_day);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * content_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * left_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * right_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(title, 0, wxBOTTOM, 15);
	sizer->Add(content_sizer, 0, wxBOTTOM, 15);
	content_sizer->Add(left_sizer, 0, wxRIGHT, 15);
	content_sizer->Add(right_sizer);
	left_sizer->Add(class_label);
	left_sizer->Add(m_class_choice, 0, wxBOTTOM, 15);
	left_sizer->Add(subj_label);
	left_sizer->Add(m_subj_choice, 0, wxBOTTOM, 15);
	left_sizer->Add(occurence_label);
	left_sizer->Add(m_occurence_choice, 0, wxBOTTOM, 15);
	right_sizer->Add(periods_label, 0, wxBOTTOM, 15);
	right_sizer->Add(m_periods_grid, 1, wxBOTTOM, 15);
	right_sizer->Add(btn_sizer);
	btn_sizer->Add(back_btn, 1, wxRIGHT, 5);
	btn_sizer->Add(save_btn, 1);

	wxSizer * wrapper_sz = new wxBoxSizer(wxVERTICAL);
	wrapper_sz->Add(sizer, 1, wxEXPAND | wxALL, 15);
	SetSizerAndFit(wrapper_sz);
	Layout();
	Refresh();

	save_btn->Bind(wxEVT_BUTTON, &LecturePeriodPrefPane::OnSaveButtonClicked, this);
	back_btn->Bind(wxEVT_BUTTON, &LecturePeriodPrefPane::OnBackButtonClicked, this);
}

void LecturePeriodPrefPane::OnSaveButtonClicked(wxCommandEvent & ev){

}

void LecturePeriodPrefPane::OnBackButtonClicked(wxCommandEvent & ev){

}

LecturePeriodPrefPane::~LecturePeriodPrefPane(){

}
