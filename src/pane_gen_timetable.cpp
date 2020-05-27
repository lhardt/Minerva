#include "gui.hpp"

extern "C" {
	#include "loader.h"
	#include "solver.h"
	#include "util.h"
};


GenerateTimetablePane::GenerateTimetablePane(Application * owner, wxWindow * parent, wxPoint pos)
							: wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * t = new wxStaticText(this, wxID_ANY, wxT("Aperte o botão e verás..."), wxPoint(30,50), wxSize(70,15));
	wxButton * button = new wxButton(this, wxID_ANY, wxT("Gerar!"), wxPoint(30,70), wxSize(70,30));

	button->Bind(wxEVT_BUTTON, &GenerateTimetablePane::OnButtonClicked, this);

}

void GenerateTimetablePane::OnButtonClicked(wxCommandEvent & ev){
	printf("Tentando gerar o horário.");

	print_school(stdout, m_owner->m_school);

	DecisionTree * tree = init_decision_tree(m_owner->m_school);
	print_meeting_list(stdout,tree->start[0].conclusion);

	Meeting * generated_timetable = create_timetable(m_owner->m_school);

	if(generated_timetable != nullptr){
		print_meeting_list(stdout,generated_timetable);

		m_owner->m_school->all_meetings = generated_timetable;
		bool success = insert_all_meetings(stdout, m_owner->m_database, m_owner->m_school);
		if(!success){
			free_meetings_list(generated_timetable);
		}
	} else {
		printf("Was nullptr\n");
	}
}

GenerateTimetablePane::~GenerateTimetablePane(){

}
