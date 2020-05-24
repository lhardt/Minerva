#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

ListTeachersPane::ListTeachersPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * body_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * butn_sz = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Lista de Professores"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	m_teachers_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));
	m_name_text = new wxStaticText(this, wxID_ANY, wxT("Nome: "), wxDefaultPosition, wxSize(300,20));
	m_max_days_text = new wxStaticText(this, wxID_ANY, wxString::FromUTF8("N° Máximo de Dias: "), wxDefaultPosition, wxSize(300,20));
	m_max_periods_text = new wxStaticText(this, wxID_ANY, wxString::FromUTF8("N° Máximo de Períodos: "), wxDefaultPosition, wxSize(300,20));
	m_max_ppd_text = new wxStaticText(this, wxID_ANY, wxString::FromUTF8("N° Máximo de Períodos por Dia: "), wxDefaultPosition,wxSize(300,20));
	m_planning_periods_text = new wxStaticText(this, wxID_ANY, wxT("N° de Períodos de Planejamento: "), wxDefaultPosition,wxSize(300,20));
	m_teaches_text = new wxStaticText(this, wxID_ANY, wxT("Ensina: "), wxDefaultPosition,wxSize(300,20));

	wxStaticText * periods_text = new wxStaticText(this, wxID_ANY, wxT("Períodos em que ele está disponível"), wxDefaultPosition,wxSize(300,20));
	m_periods_grid = new ChoiceGrid(this, wxID_ANY, wxDefaultPosition,wxSize(300,20));

	wxButton * edit_btn = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,wxT("Remover"), wxDefaultPosition, wxSize(200,30));

	edit_btn->Disable();

	if(school->n_teachers > 0){
		wxArrayString list;
		for(i = 0; i < school->n_teachers; ++i){
			list.Add(wxString::FromUTF8(school->teachers[i].name));
		}
		m_teachers_list->InsertItems(list,0);
	}

	m_teachers_list->Bind(wxEVT_LISTBOX, &ListTeachersPane::OnSelectionChanged, this);
	edit_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnEditButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListTeachersPane::OnDeleteButtonClicked, this);


	butn_sz->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	butn_sz->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	desc_sz->Add(m_name_text, 0, wxBOTTOM, 5);
	desc_sz->Add(m_max_days_text, 0, wxBOTTOM, 5);
	desc_sz->Add(m_max_periods_text, 0, wxBOTTOM, 5);
	desc_sz->Add(m_max_ppd_text, 0, wxBOTTOM, 5);
	desc_sz->Add(m_planning_periods_text, 0, wxBOTTOM, 5);
	desc_sz->Add(m_teaches_text, 0, wxBOTTOM, 5);
	desc_sz->Add(periods_text, 0, wxTOP | wxLEFT, 15);
	desc_sz->Add(m_periods_grid, 0, wxBOTTOM|wxLEFT, 15);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(butn_sz, 0, 0);

	body_sz->Add(m_teachers_list, 0, wxEXPAND|wxALL, 15);
	body_sz->Add(desc_sz, 1, wxEXPAND|wxALL, 15);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(body_sz, 1, wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();
}

void ListTeachersPane::OnEditButtonClicked(wxCommandEvent &) {
	printf("Not Implemented.\n");
}
void ListTeachersPane::OnDeleteButtonClicked(wxCommandEvent &) {
	int i_select = m_teachers_list->GetSelection();
	School * school = m_owner->m_school;
	bool success = false;
	if(i_select != wxNOT_FOUND){
		success = remove_teacher(stdout, m_owner->m_database, school->teachers[i_select].id);
		if(success){
			printf("Deleted successfully\n");
		} else {
			printf("Couldn't\n");
		}
	}
}
void ListTeachersPane::OnSelectionChanged(wxCommandEvent &) {
	int i_select = m_teachers_list->GetSelection();
	School * school = m_owner->m_school;
	if(school != NULL && school->teachers != NULL && i_select != wxNOT_FOUND){
		Teacher * t = & school->teachers[i_select];

		m_name_text->SetLabel(wxString::Format("Nome: %s", wxString::FromUTF8(t->name)));
		m_max_days_text->SetLabel(wxString::Format(wxString::FromUTF8("N° Máximo de Dias: %d"), (t->max_days)));
		m_max_periods_text->SetLabel(wxString::Format(wxString::FromUTF8("N° Máximo de Períodos: %d"), (t->max_meetings)));
		m_max_ppd_text->SetLabel(wxString::Format(wxString::FromUTF8("N° Máximo de Períodos por Dia: %d"), (t->max_meetings_per_day)));
		m_planning_periods_text->SetLabel(wxString::Format(wxString::FromUTF8("N° de Períodos de Planejamento: %d"), (t->num_planning_periods)));
		m_teaches_text->SetLabel(wxT("Ensina: "));

		if(t->teaches != NULL){
			for(int i = 0; t->teaches[i] != NULL; ++i){
				m_teaches_text->SetLabel(m_teaches_text->GetLabel() +
							wxString::Format("\n\t%s", wxString::FromUTF8(t->teaches[i]->subject->name)));
			}
		}
	}
}

ListTeachersPane::~ListTeachersPane(){

}
