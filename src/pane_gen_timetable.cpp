#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "solver.h"
	#include "util.h"
};

GenerateTimetablePane::GenerateTimetablePane(Application * owner, wxWindow * parent, wxPoint pos)
							: wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(250,250,250));

	wxStaticText * description = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_generate_timetable_text , wxDefaultPosition, wxSize(400, 50));
	wxStaticText * tt_name_label = new wxStaticText(this,wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * tt_desc_label = new wxStaticText(this,wxID_ANY, m_owner->m_lang->str_description);
	m_tt_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(250,-1));
	m_tt_desc_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(250,200), wxTE_MULTILINE);
	wxButton * gen_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_generate, wxDefaultPosition, wxSize(250,-1));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,-1));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(description, 0, wxLEFT | wxRIGHT | wxTOP, 15 );
	sizer->Add(tt_name_label, 0, wxLEFT | wxRIGHT, 15);
	sizer->Add(m_tt_name_text, 0, wxLEFT | wxBOTTOM, 15 );
	sizer->Add(tt_desc_label, 0, wxLEFT | wxRIGHT, 15);
	sizer->Add(m_tt_desc_text, 0, wxLEFT | wxBOTTOM, 15 );
	sizer->Add(gen_button, 0, wxLEFT | wxBOTTOM, 15 );
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15 );

	gen_button->Bind(wxEVT_BUTTON, &GenerateTimetablePane::OnButtonClicked, this);
	Bind(DATA_CHANGE_EVENT, &GenerateTimetablePane::OnDataChange, this);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();
}

void GenerateTimetablePane::OnDataChange(wxNotifyEvent & evt) {
	printf("Data change!\n");
}

void GenerateTimetablePane::OnButtonClicked(wxCommandEvent & ev){
	int i;
	School * school = m_owner->m_school;
	if(!m_tt_name_text->GetValue().IsEmpty()){
		m_err_msg->SetLabel(m_owner->m_lang->str_generating);

		char * name = copy_wx_string(m_tt_name_text->GetValue());
		char * desc = copy_wx_string(m_tt_desc_text->GetValue());
		if(can_insert_solution(school, name)){
			Action * act = new TimetableGenerateAction(m_owner, name, desc);
			bool success = m_owner->Do(act);

			if(success){
					m_err_msg->SetLabel(m_owner->m_lang->str_success);
					m_tt_name_text->SetValue(wxT(""));
			} else {
				m_err_msg->SetLabel(m_owner->m_lang->str_could_not_generate);
			}
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_repeated_name_error);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}

GenerateTimetablePane::~GenerateTimetablePane(){

}
