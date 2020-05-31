#include "gui.hpp"

AddSubjectGroupPane::AddSubjectGroupPane(Application * owner, wxWindow * parent, wxPoint pos)  : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Adicionar Grupo de Disciplinas"), wxPoint(30,30), wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, wxT("Nome do Grupo"), wxDefaultPosition, wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, wxT("Adicione Disciplinas desse grupo."), wxDefaultPosition, wxSize(400,15));
	subjects_label->SetFont(*m_owner->m_small_font);

	wxArrayString subj_names;
	for(i = 0; i < school->n_subjects; ++i){
		added_subjects[i] = false;
		subj_names.Add(wxString::FromUTF8(school->subjects[i].name));
	}

	m_subjects = new wxComboBox(this, wxID_ANY, wxT("Disciplinas"), wxDefaultPosition, wxSize(310,30), subj_names);
	wxButton * add_subject = new wxButton(this, wxID_ANY, wxT("Adicionar Disciplina"), wxDefaultPosition, wxSize(180,30));

	wxButton * remove_subject = new wxButton(this, wxID_ANY, wxT("Remover"), wxDefaultPosition, wxSize(180,30));
	wxButton * remove_all_subjects = new wxButton(this, wxID_ANY, wxT("Remover Todas"), wxDefaultPosition, wxSize(180,30));
	m_subjects_list_box = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));

	wxButton * add_group = new wxButton(this, wxID_ANY, wxT("Adicionar Grupo"), wxDefaultPosition, wxSize(180,30));

	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	add_sizer->Add(m_subjects,0,wxRIGHT,10);
	add_sizer->Add(add_subject,0,wxRIGHT,10);

	wxSizer * subjects_sizer = new wxBoxSizer(wxHORIZONTAL);
	subjects_sizer->Add(m_subjects_list_box, 0, wxRIGHT, 10);
	subjects_sizer->Add(remove_subject, 0, 0);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0, wxLEFT ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_label, 0, wxLEFT, 15);
	sizer->Add(add_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_group, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	add_subject->Bind(wxEVT_BUTTON, &AddSubjectGroupPane::OnAddSubjectButtonClicked, this);
	remove_all_subjects->Bind(wxEVT_BUTTON, &AddSubjectGroupPane::OnRemoveAllButtonClicked, this);
	add_group->Bind(wxEVT_BUTTON, AddSubjectGroupPane::OnCreateButtonClicked, this);
}

void AddSubjectGroupPane::OnAddSubjectButtonClicked(wxCommandEvent & evt){
	int add_i = m_subjects_list_box->GetSelection();
	if(add_i != wxNOT_FOUND && added_subjects[add_i] == false){
		added_subjects[add_i] = true;
		wxString name = wxString::FromUTF8(m_owner->m_school->subjects[add_i].name);
		m_subjects_list->InsertItems(1, &name);
	}
}

void AddSubjectGroupPane::OnRemoveAllButtonClicked(wxCommandEvent & evt){
	for(int i = 0; i < m_owner->m_school->n_subjects; ++i){
		added_subjects[i] = false;
	}
	m_subjects_list_box->Clear();
}

void AddSubjectGroupPane::OnCreateButtonClicked(wxCommandEvent & evt){

}

AddSubjectGroupPane::~AddSubjectGroupPane(){

}
