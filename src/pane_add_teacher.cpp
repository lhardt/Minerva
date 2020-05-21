#include "gui.hpp"

AddTeacherPane::AddTeacherPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i;
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Adicionar Professor"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, wxT("Nome do Professor"), wxDefaultPosition, wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxStaticText * subjects_label = new wxStaticText(this, wxID_ANY, wxT("Adicione Disciplinas que o Professor leciona."), wxDefaultPosition, wxSize(400,15));
	subjects_label->SetFont(*m_owner->m_small_font);

	wxArrayString arr;
	for(i = 0; i < m_owner->m_school->n_subjects; ++i){
		arr.push_back(wxString::FromUTF8(m_owner->m_school->subjects[i].name));
	}

	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	m_all_subjects_list = new wxComboBox(this, wxID_ANY, wxT("Disciplinas"), wxDefaultPosition, wxSize(310,30),arr);
	wxButton * add_subject = new wxButton(this, wxID_ANY, wxT("Adicionar Disciplina"), wxDefaultPosition, wxSize(180,30));
	add_subject->Bind(wxEVT_BUTTON, &AddTeacherPane::OnAddSubjectButtonClicked, this);

	add_sizer->Add(m_all_subjects_list,0,wxRIGHT,10);
	add_sizer->Add(add_subject,0,wxRIGHT,10);

	for(i = 0; i < MAX_SUBJECTS; ++i){
		m_teaches_subj[i] = false;
	}

	wxSizer * subjects_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton * remove_subject = new wxButton(this, wxID_ANY, wxT("Remover"), wxDefaultPosition, wxSize(180,30));
	m_teaches_subjects_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	subjects_sizer->Add(m_teaches_subjects_list, 0, wxRIGHT, 10);
	subjects_sizer->Add(remove_subject, 0, 0);

	wxButton * add_teacher = new wxButton(this, wxID_ANY, wxT("Adicionar Professor"), wxDefaultPosition, wxSize(180,30));
	add_teacher->Bind(wxEVT_BUTTON, &AddTeacherPane::OnAddTeacherButtonClicked, this);

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxPoint(30, 180), wxSize(300,30));
	m_err_msg->SetFont(*m_owner->m_small_font);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0, wxLEFT ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_label, 0, wxLEFT, 15);
	sizer->Add(add_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(subjects_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_teacher, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(m_err_msg, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();
}

void AddTeacherPane::OnAddSubjectButtonClicked(wxCommandEvent & ev){
	if(m_all_subjects_list->GetSelection() != wxNOT_FOUND && m_teaches_subj[ m_all_subjects_list->GetSelection() ] == false ){
		wxString text = wxString::FromUTF8(m_owner->m_school->subjects[ m_all_subjects_list->GetSelection() ].name);
		m_teaches_subj[ m_all_subjects_list->GetSelection() ] = true;
		m_teaches_subjects_list->InsertItems(1, &text, m_teaches_subjects_list->GetCount());
	} else {

	}
}

void AddTeacherPane::OnAddTeacherButtonClicked(wxCommandEvent & ev){
	printf("Button clicked\n");
}

AddTeacherPane::~AddTeacherPane(){

}
