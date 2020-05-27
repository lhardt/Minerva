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
	wxStaticText * last_gen = new wxStaticText(this, wxID_ANY, wxT("**A ser implementado**: Lista de últimas grades geradas para visualização"), wxDefaultPosition, wxSize(400,-1));
	wxButton * gen_button = new wxButton(this, wxID_ANY, wxT("Gerar Nova Grade Horária"), wxDefaultPosition, wxSize(70,30));
	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,30));

	sizer->Add(title, 0, wxALL | wxCENTER, 15);
	sizer->Add(description, 0, wxLEFT | wxTOP, 15 );
	sizer->Add(last_gen, 0, wxLEFT | wxTOP, 15 );
	sizer->Add(gen_button, 0, wxEXPAND | wxALL, 15 );
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15 );

	gen_button->Bind(wxEVT_BUTTON, &GenerateTimetablePane::OnButtonClicked, this);

	SetSizerAndFit(sizer);
}

void GenerateTimetablePane::OnButtonClicked(wxCommandEvent & ev){
	m_err_msg->SetLabel(wxT("Apagando última tentativa e gerando o novo horário."));

	DecisionTree * tree = init_decision_tree(m_owner->m_school);
	print_meeting_list(stdout,tree->start[0].conclusion);

	Meeting * generated_timetable = create_timetable(m_owner->m_school);

	if(generated_timetable != nullptr){

		print_meeting_list(stdout,generated_timetable);

		m_owner->m_school->all_meetings = generated_timetable;
		bool success = insert_all_meetings(stdout, m_owner->m_database, m_owner->m_school);
		if(success){
			m_err_msg->SetLabel(wxT("Gerada com sucesso."));
		} else {
			free_meetings_list(generated_timetable);
			m_err_msg->SetLabel(wxT("Não foi possível gravar a solução."));
		}
	} else {
		m_err_msg->SetLabel(wxT("Não foi possível gerar a grade horária."));
	}
}

GenerateTimetablePane::~GenerateTimetablePane(){

}
