#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

LecturePeriodPrefPane::LecturePeriodPrefPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_period_preference, wxDefaultPosition, wxSize(300,30));
	wxStaticText * class_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class);
	wxStaticText * subj_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class);
	wxStaticText * occurence_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class);
	wxStaticText * teachers_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_teachers, wxDefaultPosition, wxSize(300,30));

	m_class_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	m_subj_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	m_occurence_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));

	title->SetFont(*m_owner->m_page_title_font);
	class_label->SetFont(*m_owner->m_small_font);
	subj_label->SetFont(*m_owner->m_small_font);
	occurence_label->SetFont(*m_owner->m_small_font);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * content_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * left_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * right_sizer = new wxBoxSizer(wxVERTICAL);
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
	right_sizer->Add(teachers_label);

	wxSizer * wrapper_sz = new wxBoxSizer(wxVERTICAL);
	wrapper_sz->Add(sizer, 1, wxEXPAND | wxALL, 15);
	SetSizerAndFit(wrapper_sz);
	Layout();
	Refresh();
}

LecturePeriodPrefPane::~LecturePeriodPrefPane(){

}
