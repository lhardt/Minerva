#include "gui.hpp"

#include <iostream>

#include <wx/grid.h>
#include <wx/spinctrl.h>

extern "C"{
	#include "loader.h"
}

CreateSchoolForm::CreateSchoolForm(Application * owner)  : wxFrame(nullptr, wxID_ANY, wxString::FromUTF8("Horário Escolar Minerva"), wxPoint(30,30), wxSize(800,600)){
	m_owner = owner;


	#ifdef __WXMSW__
		SetIcon(wxICON(aaaaaaaa));
	#endif


	SetMinSize(wxSize(800,600));
	SetBackgroundColour(wxColour(240,240,240));

	wxPanel * m_right_pane = new wxPanel(this, wxID_ANY, wxPoint(450,0), wxSize(350,575));
	m_right_pane->SetBackgroundColour(wxColor(0x29, 0x80, 0xb9));

	wxPanel * m_footer = new wxPanel(this, wxID_ANY, wxPoint(0,575), wxSize(800,25));
	m_footer->SetBackgroundColour(wxColor(0x25,0x75,0xb0));


	wxStaticText * m_footer_text = new wxStaticText(m_footer, wxID_ANY,wxT("Copyright (C) Léo Hardt 2019-2020. This program is free software protected by Mozilla Public License v2.0."),
	wxPoint(0,5), wxSize(800,20), wxST_NO_AUTORESIZE |wxALIGN_CENTRE | wxALIGN_CENTRE_HORIZONTAL);
	m_footer_text->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_footer_text->SetFont(*m_owner->m_small_font);

	wxStaticText * m_title = new wxStaticText(this, wxID_ANY, wxT("Cadastrar Escola"), wxPoint(30,30), wxSize(100,25));
	m_title->SetFont(*m_owner->m_page_title_font);
	m_title->SetForegroundColour(wxColor(0x00, 0x00, 0x00));

	wxStaticText * m_school_name_label = new wxStaticText(this, wxID_ANY, wxT("Nome da Escola"), wxPoint(30,90), wxSize(200,15));
	m_school_name_label->SetFont(*m_owner->m_small_font);
	m_school_name_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));
	m_school_name_text = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(30,105), wxSize(200,25));

	wxStaticText * m_number_of_days_label = new wxStaticText(this, wxID_ANY, wxT("Número de Dias em um Ciclo"), wxPoint(30,145), wxSize(200,15));
	m_number_of_days_label->SetFont(*m_owner->m_small_font);
	m_number_of_days_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));
	m_number_of_days_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxPoint(30,160), wxSize(200,25));

	wxStaticText * m_number_of_periods_label = new wxStaticText(this, wxID_ANY, wxT("Número de Periodos por Dia"), wxPoint(30,200), wxSize(200,15));
	m_number_of_periods_label->SetFont(*m_owner->m_small_font);
	m_number_of_periods_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));
	m_number_of_periods_text = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxPoint(30,215), wxSize(200,25));

	m_number_of_periods_text->Bind(wxEVT_SPINCTRL, &CreateSchoolForm::OnGridSizeUpdated, this);
	m_number_of_days_text->Bind(wxEVT_SPINCTRL, &CreateSchoolForm::OnGridSizeUpdated, this);

	wxStaticText * m_names_label = new wxStaticText(this, wxID_ANY, wxT("A Escola Funciona em Quais Períodos?"), wxPoint(30,255), wxSize(400,15));
	m_names_label->SetFont(*m_owner->m_small_font);
	m_names_label->SetForegroundColour(wxColor(0x00, 0x00, 0x00));

	m_grid = new ChoiceGrid(this, wxID_ANY, wxPoint(30,270), wxSize(400,200));

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(wxT("Aberta"));
	grid_values.push_back(wxT("Fechada"));
	m_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200));
	m_grid->SetBackgroundColors(grid_colors);

	m_grid->m_basic_col_name = wxT("Dia");
	m_grid->m_basic_row_name = wxString::FromUTF8("Período");


	m_grid->GridRemake(m_number_of_days_text->GetValue(), m_number_of_periods_text->GetValue());

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxPoint(30,480), wxSize(300,30));
	m_err_msg->SetFont(*m_owner->m_small_font);
	m_err_msg->SetForegroundColour(wxColor(0xF0, 0x50, 0x50));


	m_button_create = new wxButton(this, wxID_ANY, wxT("Criar"), wxPoint(350, 500), wxSize(80,30));
	m_button_back = new wxButton(this, wxID_ANY, wxT("Voltar"), wxPoint(260, 500), wxSize(80,30));

	m_button_create->Bind(wxEVT_BUTTON, &CreateSchoolForm::OnCreateClicked, this);
	m_button_back->Bind(wxEVT_BUTTON, &CreateSchoolForm::OnBackClicked, this);

	wxStaticText* tutorial_title = new wxStaticText(m_right_pane, wxID_ANY, wxT("Cadastrar Escola"), wxPoint(30,30), wxSize(240,30));
	tutorial_title->SetFont(*m_owner->m_page_title_font);
	tutorial_title->SetForegroundColour(wxColor(255,255,255));
	wxStaticText* tutorial_text = new wxStaticText(m_right_pane, wxID_ANY, wxT(
			"Seja bem vindo ao software Minerva. Agradecemos por testá-lo e pedimos o favor que se encontrares algum erro, "
			"faça a gentileza de entrar em contato.\n\n"
			"Em caso de dúvidas, o software dispõe de um manual, esclarecendo o uso de suas funções.\n\n"
			"Para cadastrar uma escola, preencha os campos ao lado. Pense em cada \"escola\" aqui como uma entidade que "
			"precisa de um cronograma diferente. Esse nome será usado na tela inicial e no horário. Recomendamos um nome "
			"curto, como \"Escola Dom Pedro - Noite.\"\n\n"
			"O tamanho do ciclo é tamanho do horário, em dias. Geralmente são cinco dias. Se a escola só funcionar "
			"em três dias da semana, são três dias no ciclo. Se o horário será bisemanal, são 10 dias. E assim por diante. "
			"Caso tenha dificuldade para decidir, pense \"quantos dias demora para o horário voltar ao início?\".\n\n"
			"Por último, clique em qualquer período em que a escola não estiver aberta para fazer com que ela esteja fechada. Algumas escolas, por "
			"exemplo, ficam fechadas no 5º período da sexta-feira. Para fazer com que a escola volte a estar aberta, clique novamente."
		),
		wxPoint(30,70), wxSize(290,500)
	);
	tutorial_text->SetForegroundColour(wxColor(255,255,255));

	Refresh();
}

CreateSchoolForm::~CreateSchoolForm(){

}

void CreateSchoolForm::OnGridSizeUpdated(wxSpinEvent & ev){
	m_grid->GridRemake(m_number_of_days_text->GetValue(),m_number_of_periods_text->GetValue());
}

void CreateSchoolForm::OnBackClicked(wxCommandEvent & ev){
	m_owner->SwitchForm(FORM_WELCOME);
	this->Destroy();
}

void CreateSchoolForm::OnCreateClicked(wxCommandEvent & ev){
	int i, str_sz;
	if(!m_school_name_text->IsEmpty() && m_number_of_periods_text->GetValue() > 0 && m_number_of_days_text->GetValue() > 0 ){
		School * school = new School();
		str_sz = strlen(m_school_name_text->GetValue().ToUTF8().data());
		school->name = (char *) calloc(str_sz + 1, sizeof(char));
		strncpy(school->name, m_school_name_text->GetValue().ToUTF8().data(), str_sz);

		school->n_days = m_number_of_days_text->GetValue();
		school->n_periods_per_day = m_number_of_periods_text->GetValue();

		school->day_names = (char **) calloc(school->n_days, sizeof(char*));
		for(i = 0; i < school->n_days; ++i){
			str_sz = strlen(m_grid->GetCellValue(0,1 + i).ToUTF8().data());
			school->day_names[i] = (char *) calloc(1 + str_sz, sizeof(char));
			strncpy(school->day_names[i], m_grid->GetCellValue(0,1 + i).ToUTF8().data(), str_sz);
		}

		school->daily_period_names = (char**) calloc(school->n_periods_per_day, sizeof(char*));
		for(i = 0; i < school->n_periods_per_day; ++i){
			str_sz = strlen((m_grid->GetCellValue(1 + i,0).ToUTF8().data()));
			school->daily_period_names[i] = (char*) calloc( 1 + str_sz, sizeof(char));
			strncpy(school->daily_period_names[i],m_grid->GetCellValue(1+i,0).ToUTF8().data(), str_sz);
			printf("Daily period name: %s. Theoretical name %s\n",school->daily_period_names[i],m_grid->GetCellValue(1+i,0).ToUTF8().data());
		}

		school->n_periods = school->n_days * school->n_periods_per_day;
		school->period_names = (char**) calloc( school->n_periods + 1, sizeof(char*));
		for(i = 0; i < school->n_periods; ++i){
			school->period_names[i] = (char*)calloc(1,sizeof(char));
			school->periods[i] = wxT("Aberta") == m_grid->GetCellValue(1 + (i % school->n_periods_per_day), 1 +  (i / school->n_periods_per_day));
		}

		int errc = insert_school(stdout, m_owner->m_database, school);

		if(errc != -1){
			m_owner->m_school = school;
			m_owner->SwitchForm(FORM_MAIN_MENU);
			this->Destroy();
		} else {
			m_err_msg->SetLabel(wxT("Não foi possível inserir no banco.\n"));
		}
	} else {
		m_err_msg->SetLabel(wxT("Preencha todos os campos corretamente"));
	}

}
