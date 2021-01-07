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

	wxStaticText * description = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_generate_timetable_text , wxDefaultPosition, wxSize(400,-1));
	wxStaticText * tt_name_label = new wxStaticText(this,wxID_ANY, m_owner->m_lang->str_name);
	wxStaticText * tt_desc_label = new wxStaticText(this,wxID_ANY, wxT("Descrição"));
	m_tt_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(250,-1));
	m_tt_desc_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(250,200), wxTE_MULTILINE);
	wxButton * gen_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_generate, wxDefaultPosition, wxSize(250,-1));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,-1));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(description, 0, wxALL, 15 );
	sizer->Add(tt_name_label, 0, wxLEFT | wxRIGHT | wxTOP, 15);
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
		DecisionTree * tree = init_decision_tree(m_owner->m_school);
		for(int i = 0; i < school->n_teachers; ++i){
			print_teacher(stdout, &school->teachers[i]);
		}
		print_meeting_list(stdout,tree->start[0].conclusion);
		m_err_msg->SetLabel(m_owner->m_lang->str_generating);

		Solution gen_solution;
		gen_solution.name = copy_wx_string(m_tt_name_text->GetValue());
		gen_solution.desc = copy_wx_string(m_tt_desc_text->GetValue());
		gen_solution.gen_date = (char *) calloc(21, sizeof(char));
		gen_solution.meetings = create_timetable(m_owner->m_school);

		time_t rawtime;
		struct tm * time_info;
		time(&rawtime);
		time_info = localtime(&rawtime);
		strftime(gen_solution.gen_date, 20, "%d/%m/%y, %H:%M", time_info);

		if(gen_solution.meetings != nullptr){
			for(i = 0; gen_solution.meetings[i].type != meet_NULL; ++i){
				/* Blank counter */
			}
			gen_solution.n_meetings = i;

			print_meeting_list(stdout,gen_solution.meetings);

			bool success = insert_solution(stdout, m_owner->m_database, school, &gen_solution, -1);
			if(success){
				school_solution_add(school, &gen_solution);
				m_err_msg->SetLabel(m_owner->m_lang->str_success);
				m_owner->NotifyNewUnsavedData();
				m_tt_name_text->SetValue(wxT(""));
			} else {
				free_meetings_list(gen_solution.meetings);
				m_err_msg->SetLabel(m_owner->m_lang->str_could_not_insert_on_db);
			}
		} else {
			m_err_msg->SetLabel(m_owner->m_lang->str_could_not_generate);
		}
	} else {
		m_err_msg->SetLabel(m_owner->m_lang->str_fill_the_form_correctly);
	}
}

GenerateTimetablePane::~GenerateTimetablePane(){

}
