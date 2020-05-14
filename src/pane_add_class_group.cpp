#include "gui.hpp"

AddClassGroupPane::AddClassGroupPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Adicionar Grupo de Turmas"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxStaticText * name_label = new wxStaticText(this, wxID_ANY, wxT("Nome do Grupo"), wxDefaultPosition, wxSize(200,15));
	name_label->SetFont(*m_owner->m_small_font);
	m_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,30));

	wxStaticText * classes_label = new wxStaticText(this, wxID_ANY, wxT("Adicione Turmas ao Grupo."), wxDefaultPosition, wxSize(400,15));
	classes_label->SetFont(*m_owner->m_small_font);

	wxSizer * add_sizer = new wxBoxSizer(wxHORIZONTAL);
	m_all_classes_list = new wxComboBox(this, wxID_ANY, wxT("Turmas"), wxDefaultPosition, wxSize(310,30));
	wxButton * add_class = new wxButton(this, wxID_ANY, wxT("Adicionar Turma"), wxDefaultPosition, wxSize(180,30));
	add_sizer->Add(m_all_classes_list,0,wxRIGHT,10);
	add_sizer->Add(add_class,0,wxRIGHT,10);


	wxSizer * classes_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton * remove_button = new wxButton(this, wxID_ANY, wxT("Remover"), wxDefaultPosition, wxSize(180,30));
	m_selected_classes_list = new wxListBox(this,wxID_ANY,wxDefaultPosition, wxSize(310,300));
	classes_sizer->Add(m_selected_classes_list, 0, wxRIGHT, 10);
	classes_sizer->Add(remove_button, 0, 0);

	wxButton * add_group = new wxButton(this, wxID_ANY, wxT("Adicionar Grupo"), wxDefaultPosition, wxSize(180,30));
	add_group->Bind(wxEVT_BUTTON, &AddClassGroupPane::OnAddButtonClicked, this);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(name_label, 0, wxLEFT ,15);
	sizer->Add(m_name_text, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(classes_label, 0, wxLEFT, 15);
	sizer->Add(add_sizer, 0, wxLEFT | wxBOTTOM,15);
	sizer->Add(classes_sizer, 0, wxLEFT | wxBOTTOM, 15);
	sizer->Add(add_group, 0, wxLEFT | wxBOTTOM, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

}

void AddClassGroupPane::OnAddButtonClicked(wxCommandEvent & ev){
	printf("Clicked\n");
}

AddClassGroupPane::~AddClassGroupPane(){

}
