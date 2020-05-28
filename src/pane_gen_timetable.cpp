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

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Soluções Geradas."), wxDefaultPosition, wxSize(400,40));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * description = new wxStaticText(this, wxID_ANY, wxT("Aqui poderemos ver as soluções já geradas."), wxDefaultPosition, wxSize(400,-1));
	wxStaticText * last_gen_label = new wxStaticText(this, wxID_ANY, wxT("Últimas grades geradas:"), wxDefaultPosition, wxSize(400,-1));
	wxButton * gen_button = new wxButton(this, wxID_ANY, wxT("Gerar Nova Grade Horária"), wxDefaultPosition, wxSize(70,30));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,30));

	sizer->Add(title, 0, wxALL | wxCENTER, 15);
	sizer->Add(description, 0, wxLEFT | wxTOP, 15 );
	sizer->Add(last_gen_label, 0, wxLEFT | wxTOP, 15 );
	sizer->Add(gen_button, 0, wxEXPAND | wxALL, 15 );
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15 );

	gen_button->Bind(wxEVT_BUTTON, &GenerateTimetablePane::OnButtonClicked, this);

	SetSizerAndFit(sizer);
}

void GenerateTimetablePane::OnButtonClicked(wxCommandEvent & ev){
	int i;
	School * school = m_owner->m_school;
	DecisionTree * tree = init_decision_tree(m_owner->m_school);
	print_meeting_list(stdout,tree->start[0].conclusion);
	m_err_msg->SetLabel(wxT("Gerando o novo horário."));

	Solution gen_solution;
	gen_solution.meetings = create_timetable(m_owner->m_school);

	if(gen_solution.meetings != nullptr){
		for(i = 0; gen_solution.meetings[i].m_class != NULL; ++i){

		}
		gen_solution.n_meetings = i;

		print_meeting_list(stdout,gen_solution.meetings);

		// m_owner->m_school->all_meetings = generated_timetable;
		bool success = insert_solution(stdout, m_owner->m_database, school, &gen_solution);
		if(success){
			if(school->solutions == NULL || school->n_solutions == 0){
				school->solutions = (Solution*) calloc(2, sizeof(Solution));
				school->n_solutions = 0;
			} else {
				school->solutions = (Solution*) realloc(school->solutions, (school->n_solutions + 1) * sizeof(Solution));
			}
			school->solutions[school->n_solutions] = gen_solution;
			++ school->n_solutions;
			m_err_msg->SetLabel(wxT("Gerada com sucesso."));
		} else {
			free_meetings_list(gen_solution.meetings);
			m_err_msg->SetLabel(wxT("Não foi possível gravar a solução."));
		}
	} else {
		m_err_msg->SetLabel(wxT("Não foi possível gerar a grade horária."));
	}
}

GenerateTimetablePane::~GenerateTimetablePane(){

}
