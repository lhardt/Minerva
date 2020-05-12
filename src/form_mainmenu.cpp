#include "gui.hpp"
#include <wx/wx.h>
#include "wx/ribbon/bar.h"
#include "wx/ribbon/buttonbar.h"
#include "wx/ribbon/gallery.h"
#include "wx/ribbon/toolbar.h"
#include <wx/artprov.h>

MainMenuForm::MainMenuForm(Application * owner)  : wxFrame(nullptr, wxID_ANY, "Horário Escolar Minerva", wxPoint(30,30), wxSize(800,600)){
	m_owner = owner;

	SetMinSize(wxSize(800,600));

	wxRibbonBar * m_ribbon = new wxRibbonBar(this,-1,wxDefaultPosition, wxSize(800,150), wxRIBBON_BAR_FLOW_HORIZONTAL | wxRIBBON_BAR_SHOW_PAGE_LABELS);

	wxRibbonButtonBar * m_rib_bbars[7][5];

	const wchar_t * const menu_names[7] = {L"Escola", L"Salas", L"Disciplinas", L"Professores", L"Turmas", L"Aulas", L"Horário"};
	const wchar_t * const smenu_names[10][10] = {
		{L"Arquivo", L"Dados", L"Ajuda", NULL},
		{L"Visualização", L"Cadastro", L"Preferências", L"Conferência", L"Ajuda", NULL},
		{L"Visualização", L"Cadastro", L"Preferências", L"Conferência", L"Ajuda", NULL},
		{L"Visualização", L"Cadastro", L"Preferências", L"Conferência", L"Ajuda", NULL},
		{L"Visualização", L"Cadastro", L"Preferências", L"Conferência", L"Ajuda", NULL},
		{L"Visualização", L"Cadastro", L"Preferências", L"Conferência", L"Ajuda", NULL},
		{L"Visualização", L"Geração",  L"Ajuda", NULL}
	};

	for(int i = 0; i < 7; ++i){
		m_ribbon_pages[i] = new wxRibbonPage(m_ribbon, wxID_ANY, wxString(menu_names[i]));
		for(int j = 0; smenu_names[i][j] != NULL; ++j){
			m_rib_bbars[i][j] = new wxRibbonButtonBar(new wxRibbonPanel(m_ribbon_pages[i], wxID_ANY, wxString(smenu_names[i][j])));
		}
	}

	auto image = wxArtProvider::GetBitmap(wxART_ADD_BOOKMARK, wxART_TOOLBAR, wxSize(32,32));

	/* ESCOLA */
	m_rib_bbars[0][0]->AddButton(wxID_ANY, wxT("Salvar Como"), image);
	m_rib_bbars[0][0]->AddButton(wxID_ANY, wxT("Fechar e Salvar"), image);
	m_rib_bbars[0][0]->AddButton(wxID_ANY, wxT("Fechar sem Salvar"), image);

	m_rib_bbars[0][1]->AddButton(wxID_ANY, wxT("Dados da Escola"), image);

	m_rib_bbars[0][2]->AddButton(wxID_ANY, wxT("Abrir Manual"), image);


	/* SALAS E CARACTERISTICAS DE SALAS */
	m_rib_bbars[1][0]->AddButton(wxID_ANY, wxT("Ver Características"), image);
	m_rib_bbars[1][0]->AddButton(wxID_ANY, wxT("Ver Salas"), image);

	m_rib_bbars[1][1]->AddButton(wxID_ANY, wxT("Adicionar Característica"), image);
	m_rib_bbars[1][1]->AddButton(wxID_ANY, wxT("Adicionar Sala"), image);

	m_rib_bbars[1][2]->AddButton(wxID_ANY, wxT("Preferência por Períodos"), image);

	m_rib_bbars[1][3]->AddButton(wxID_ANY, wxT("Conferir Tudo"), image);

	m_rib_bbars[1][4]->AddButton(wxID_ANY, wxT("Abrir Manual"), image);

	/* DISCIPLINAS */
	m_rib_bbars[2][0]->AddButton(wxID_ANY, wxT("Ver Disciplinas"), image);
	m_rib_bbars[2][0]->AddButton(wxID_ANY, wxT("Ver Grupos Disciplinares"), image);

	m_rib_bbars[2][1]->AddButton(wxID_ANY, wxT("Adicionar Disciplina"), image);
	m_rib_bbars[2][1]->AddButton(wxID_ANY, wxT("Adicionar Grupo Disciplinar"), image);

	m_rib_bbars[2][2]->AddButton(wxID_ANY, wxT("Preferência por Períodos"), image);
	m_rib_bbars[2][2]->AddButton(wxID_ANY, wxT("Preferência por Geminação"), image);
	m_rib_bbars[2][2]->AddButton(wxID_ANY, wxT("Preferência por Sala"), image);

	m_rib_bbars[2][3]->AddButton(wxID_ANY, wxT("Conferir Tudo"), image);

	m_rib_bbars[2][4]->AddButton(wxID_ANY, wxT("Abrir Manual"), image);

	/* PROFESSORES */
	m_rib_bbars[3][0]->AddButton(wxID_ANY, wxT("Ver Professores"), image);
	m_rib_bbars[3][0]->AddButton(wxID_ANY, wxT("Ver Grupos de Professores"), image);

	m_rib_bbars[3][1]->AddButton(wxID_ANY, wxT("Adicionar Professor"), image);
	m_rib_bbars[3][1]->AddButton(wxID_ANY, wxT("Adicionar Grupo de Professores"), image);

	m_rib_bbars[3][2]->AddButton(wxID_ANY, wxT("Preferência por Disciplina"), image);
	m_rib_bbars[3][2]->AddButton(wxID_ANY, wxT("Disponibilidade por Períodos"), image);
	m_rib_bbars[3][2]->AddButton(wxID_ANY, wxT("Preferência por Geminação"), image);
	m_rib_bbars[3][2]->AddButton(wxID_ANY, wxT("Preferência por Sala"), image);

	m_rib_bbars[3][3]->AddButton(wxID_ANY, wxT("Conferir Tudo"), image);

	m_rib_bbars[3][4]->AddButton(wxID_ANY, wxT("Abrir Manual"), image);

	/* TURMAS */
	m_rib_bbars[4][0]->AddButton(wxID_ANY, wxT("Ver Turmas"), image);
	m_rib_bbars[4][0]->AddButton(wxID_ANY, wxT("Ver Grupos de Turmas"), image);

	m_rib_bbars[4][1]->AddButton(wxID_ANY, wxT("Adicionar Turma"), image);
	m_rib_bbars[4][1]->AddButton(wxID_ANY, wxT("Adicionar Grupo de Turmas"), image);

	m_rib_bbars[4][2]->AddButton(wxID_ANY, wxT("Disponibilidade por Períodos"), image);
	m_rib_bbars[4][2]->AddButton(wxID_ANY, wxT("Preferência por Professores"), image);
	m_rib_bbars[4][2]->AddButton(wxID_ANY, wxT("Preferência por Geminação"), image);
	m_rib_bbars[4][2]->AddButton(wxID_ANY, wxT("Preferência por Sala"), image);

	m_rib_bbars[4][3]->AddButton(wxID_ANY, wxT("Conferir Tudo"), image);

	m_rib_bbars[4][4]->AddButton(wxID_ANY, wxT("Abrir Manual"), image);

	/* AULAS */
	m_rib_bbars[5][0]->AddButton(wxID_ANY, wxT("Ver Aulas por Turma"), image);
	m_rib_bbars[5][0]->AddButton(wxID_ANY, wxT("Ver Grupos de Turmas"), image);

	m_rib_bbars[5][1]->AddButton(wxID_ANY, wxT("Adicionar Aulas por Turma"), image);

	m_rib_bbars[5][2]->AddButton(wxID_ANY, wxT("Preferência por Períodos"), image);
	m_rib_bbars[5][2]->AddButton(wxID_ANY, wxT("Preferência por Professores"), image);
	m_rib_bbars[5][2]->AddButton(wxID_ANY, wxT("Preferência por Geminação"), image);
	m_rib_bbars[5][2]->AddButton(wxID_ANY, wxT("Preferência por Sala"), image);

	m_rib_bbars[5][3]->AddButton(wxID_ANY, wxT("Conferir Tudo"), image);

	m_rib_bbars[5][4]->AddButton(wxID_ANY, wxT("Abrir Manual"), image);

	/* HORÁRIO */
	m_rib_bbars[6][0]->AddButton(wxID_ANY, wxT("Visualizar Horário"), image);
	m_rib_bbars[6][0]->AddButton(wxID_ANY, wxT("Exportar Horário"), image);

	m_rib_bbars[6][1]->AddButton(wxID_ANY, wxT("Gerar"), image);
	m_rib_bbars[6][1]->AddButton(wxID_ANY, wxT("Criar Manualmente"), image);

	m_rib_bbars[6][2]->AddButton(wxID_ANY, wxT("Abrir Manual"), image);


	m_ribbon->AddPageHighlight(m_ribbon->GetPageCount() - 1);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_ribbon, 0, wxEXPAND);


	wxPanel * m_center_pane = new wxPanel(this, wxID_ANY, wxPoint(0,150), wxSize(800,425));
	m_center_pane->SetBackgroundColour(wxColor(0x29, 0x80, 0xb9));

	wxPanel * m_footer = new wxPanel(this, wxID_ANY, wxPoint(0,575), wxSize(800,25));
	m_footer->SetBackgroundColour(wxColor(0x25,0x75,0xb0));


	wxStaticText * m_footer_text = new wxStaticText(m_footer, wxID_ANY,wxT("Copyright (C) Léo Hardt 2019-2020. This program is free software protected by Mozilla Public License v2.0."),
	wxPoint(0,5), wxSize(800,20), wxST_NO_AUTORESIZE |wxALIGN_CENTRE | wxALIGN_CENTRE_HORIZONTAL);
	m_footer_text->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_footer_text->SetFont(*m_owner->m_small_font);


	m_ribbon->Realize();
}

MainMenuForm::~MainMenuForm(){

}
