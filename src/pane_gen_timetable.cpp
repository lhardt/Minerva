#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "solver.h"
	#include "util.h"
};


GenerateTimetablePane::GenerateTimetablePane(Application * owner, wxWindow * parent, wxPoint pos)
							: wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400), wxSIMPLE_BORDER){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(250,250,250));

	wxStaticText * description = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_generate_timetable_text , wxDefaultPosition, wxSize(400,-1));
	wxStaticText * last_gen_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_last_generated_timetables, wxDefaultPosition, wxSize(400,-1));
	wxStaticText * tt_name_label = new wxStaticText(this,wxID_ANY, m_owner->m_lang->str_name);
	m_tt_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(150,30));
	wxButton * gen_button = new wxButton(this, wxID_ANY, m_owner->m_lang->str_generate, wxDefaultPosition, wxSize(150,30));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,30));

	if(school->solutions){
		for(i = 0; i < school->n_solutions; ++i){
			last_gen_label->SetLabel(last_gen_label->GetLabel()  + "\n- " + wxString::FromUTF8(school->solutions[i].name));
		}
	}

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * gen_sizer = new wxBoxSizer(wxHORIZONTAL);

	gen_sizer->Add(m_tt_name_text, 1, wxRIGHT, 10);
	gen_sizer->Add(gen_button, 1, wxRIGHT, 10);

	sizer->Add(description, 0, wxALL, 15 );
	sizer->Add(last_gen_label, 0, wxLEFT | wxTOP, 15 );
	sizer->Add(tt_name_label, 0, wxLEFT | wxRIGHT | wxTOP, 15);
	sizer->Add(gen_sizer, 0, wxLEFT | wxBOTTOM, 15 );
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15 );

	gen_button->Bind(wxEVT_BUTTON, &GenerateTimetablePane::OnButtonClicked, this);
	Bind(DATA_CHANGE_EVENT, &GenerateTimetablePane::OnDataChange, this);

	SetSizerAndFit(sizer);
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
		gen_solution.desc = copy_wx_string(m_tt_name_text->GetValue());
		gen_solution.meetings = create_timetable(m_owner->m_school);

		if(gen_solution.meetings != nullptr){
			for(i = 0; gen_solution.meetings[i].type != meet_NULL; ++i){
				/* Blank counter */
			}
			gen_solution.n_meetings = i;

			print_meeting_list(stdout,gen_solution.meetings);

			bool success = insert_solution(stdout, m_owner->m_database, school, &gen_solution, -1);
			if(success){
				if(school->solutions == NULL || school->n_solutions == 0){
					school->solutions = (Solution*) calloc(2, sizeof(Solution));
					school->n_solutions = 0;
				} else {
					school->solutions = (Solution*) realloc(school->solutions, (school->n_solutions + 1) * sizeof(Solution));
				}
				school->solutions[school->n_solutions] = gen_solution;
				++ school->n_solutions;
				m_err_msg->SetLabel(m_owner->m_lang->str_success);
				m_owner->NotifyNewUnsavedData();
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
