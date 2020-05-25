#include "gui.hpp"
#include <wx/wx.h>
#include "wx/ribbon/bar.h"
#include "wx/ribbon/buttonbar.h"
#include "wx/ribbon/gallery.h"
#include "wx/ribbon/toolbar.h"
#include <wx/artprov.h>

#include "gui_language.hpp"
#include "art_metro.h"

MainMenuForm::MainMenuForm(Application * owner)  : wxFrame(nullptr, wxID_ANY, wxString::FromUTF8("Horário Escolar Minerva"), wxPoint(30,30), wxSize(800,600)){
	m_owner = owner;

	#ifdef __WXMSW__
		SetIcon(wxICON(aaaaaaaa));
	#endif

	SetMinSize(wxSize(800,600));
	SetBackgroundColour(wxColour(240,240,240));

	wxRibbonBar * m_ribbon = new wxRibbonBar(this,-1,wxDefaultPosition, wxSize(800,150), wxRIBBON_BAR_FLOW_HORIZONTAL | wxRIBBON_BAR_SHOW_PAGE_LABELS);
	m_ribbon->SetArtProvider(new wxRibbonMetroArtProvider());

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

	auto image	      = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, wxSize(32,32));
	auto image_add    = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, wxSize(32,32));
	auto image_help   = wxArtProvider::GetBitmap(wxART_HELP, wxART_TOOLBAR, wxSize(32,32));
	auto image_save   = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, wxSize(32,32));
	auto image_saveas = wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR, wxSize(32,32));
	auto image_list   = wxArtProvider::GetBitmap(wxART_LIST_VIEW, wxART_TOOLBAR, wxSize(32,32));
	auto image_close  = wxArtProvider::GetBitmap(wxART_CLOSE, wxART_TOOLBAR, wxSize(32,32));
	auto image_detail = wxArtProvider::GetBitmap(wxART_CLOSE, wxART_TOOLBAR, wxSize(32,32));

//wxART_INFORMATION
	/* ESCOLA */
	// m_rib_bbars[0][0]->AddButton(LHID_OF(LHN_SAVE_AS),wxT("Salvar Como"), image_save);
	// m_rib_bbars[0][0]->AddButton(LHID_OF(LHN_SAVE_AND_CLOSE),wxT("Fechar e Salvar"), image_save);
	// m_rib_bbars[0][0]->AddButton(LHID_OF(LHN_CLOSE_WITHOUT_SAVE),wxT("Fechar sem Salvar"), image_close);
	m_rib_bbars[0][1]->AddButton(LHID_OF(LHN_SCHOOL_DATA),wxT("Dados da Escola"), image_detail);
	m_rib_bbars[0][2]->AddButton(LHID_OF(LHN_OPEN_SCHOOL_MANUAL),wxT("Abrir Manual"), image_help);
	/* SALAS E CARACTERISTICAS DE SALAS */
	m_rib_bbars[1][0]->AddButton(LHID_OF(LHN_SEE_FEATURES), wxT("Ver Características"), image_list);
	m_rib_bbars[1][0]->AddButton(LHID_OF(LHN_SEE_ROOMS), wxT("Ver Salas"), image_list);
	m_rib_bbars[1][1]->AddButton(LHID_OF(LHN_ADD_FEATURE), wxT("Adicionar Característica"), image_add);
	m_rib_bbars[1][1]->AddButton(LHID_OF(LHN_ADD_ROOM), wxT("Adicionar Sala"), image_add);
	m_rib_bbars[1][2]->AddButton(LHID_OF(LHN_ROOM_PERIOD_PREF), wxT("Preferência por Períodos"), image_detail);
	m_rib_bbars[1][3]->AddButton(LHID_OF(LHN_CHECK_ALL_ROOMS), wxT("Conferir Tudo"), image_detail);
	m_rib_bbars[1][4]->AddButton(LHID_OF(LHN_OPEN_ROOMS_MANUAL), wxT("Abrir Manual"), image_detail);

	m_ribbon->Bind(wxEVT_RIBBONBUTTONBAR_CLICKED, &MainMenuForm::OnMenuItemClicked, this);

	/* DISCIPLINAS */
	m_rib_bbars[2][0]->AddButton(LHID_OF(LHN_SEE_SUBJECTS), wxT("Ver Disciplinas"), image_list);
	m_rib_bbars[2][0]->AddButton(LHID_OF(LHN_SEE_SUBJECT_GROUPS), wxT("Ver Grupos Disciplinares"), image_list);
	m_rib_bbars[2][1]->AddButton(LHID_OF(LHN_ADD_SUBJECT), wxT("Adicionar Disciplina"), image_add);
	m_rib_bbars[2][1]->AddButton(LHID_OF(LHN_ADD_SUBJECT_GROUP), wxT("Adicionar Grupo Disciplinar"), image_add);
	m_rib_bbars[2][2]->AddButton(LHID_OF(LHN_SUBJECT_PERIOD_PREF), wxT("Preferência por Períodos"), image_detail);
	m_rib_bbars[2][2]->AddButton(LHID_OF(LHN_SUBJECT_TWIN_PREF), wxT("Preferência por Geminação"), image_detail);
	m_rib_bbars[2][2]->AddButton(LHID_OF(LHN_SUBJECT_ROOM_PREF), wxT("Preferência por Sala"), image_detail);
	m_rib_bbars[2][3]->AddButton(LHID_OF(LHN_CHECK_ALL_SUBJECTS), wxT("Conferir Tudo"), image);
	m_rib_bbars[2][4]->AddButton(LHID_OF(LHN_OPEN_SUBJECTS_MANUAL), wxT("Abrir Manual"), image_help);
	/* PROFESSORES */
	m_rib_bbars[3][0]->AddButton(LHID_OF(LHN_SEE_TEACHERS), wxT("Ver Professores"), image);
	m_rib_bbars[3][0]->AddButton(LHID_OF(LHN_SEE_TEACHER_GROUPS), wxT("Ver Grupos de Professores"), image);
	m_rib_bbars[3][1]->AddButton(LHID_OF(LHN_ADD_TEACHER), wxT("Adicionar Professor"), image);
	m_rib_bbars[3][1]->AddButton(LHID_OF(LHN_ADD_TEACHER_GROUP), wxT("Adicionar Grupo de Professores"), image);
	m_rib_bbars[3][2]->AddButton(LHID_OF(LHN_TEACHER_SUBJECT_PREF), wxT("Preferência por Disciplina"), image);
	m_rib_bbars[3][2]->AddButton(LHID_OF(LHN_TEACHER_PERIOD_PREF), wxT("Disponibilidade por Períodos"), image);
	m_rib_bbars[3][2]->AddButton(LHID_OF(LHN_TEACHER_TWIN_PREF), wxT("Preferência por Geminação"), image);
	m_rib_bbars[3][2]->AddButton(LHID_OF(LHN_TEACHER_ROOM_PREF), wxT("Preferência por Sala"), image);
	m_rib_bbars[3][3]->AddButton(LHID_OF(LHN_CHECK_ALL_TEACHERS), wxT("Conferir Tudo"), image);
	m_rib_bbars[3][4]->AddButton(LHID_OF(LHN_OPEN_TEACHERS_MANUAL), wxT("Abrir Manual"), image);
	/* TURMAS */
	m_rib_bbars[4][0]->AddButton(LHID_OF(LHN_SEE_CLASSES), wxT("Ver Turmas"), image);
	m_rib_bbars[4][0]->AddButton(LHID_OF(LHN_SEE_CLASS_GROUPS), wxT("Ver Grupos de Turmas"), image);
	m_rib_bbars[4][1]->AddButton(LHID_OF(LHN_ADD_CLASS), wxT("Adicionar Turma"), image);
	m_rib_bbars[4][1]->AddButton(LHID_OF(LHN_ADD_CLASS_GROUP), wxT("Adicionar Grupo de Turmas"), image);
	m_rib_bbars[4][2]->AddButton(LHID_OF(LHN_CLASS_PERIOD_PREF), wxT("Disponibilidade por Períodos"), image);
	m_rib_bbars[4][2]->AddButton(LHID_OF(LHN_CLASS_TEACHER_PREF), wxT("Preferência por Professores"), image);
	m_rib_bbars[4][2]->AddButton(LHID_OF(LHN_CLASS_TWIN_PREF), wxT("Preferência por Geminação"), image);
	m_rib_bbars[4][2]->AddButton(LHID_OF(LHN_CLASS_ROOM_PREF), wxT("Preferência por Sala"), image);
	m_rib_bbars[4][3]->AddButton(LHID_OF(LHN_CHECK_ALL_CLASSES), wxT("Conferir Tudo"), image);
	m_rib_bbars[4][4]->AddButton(LHID_OF(LHN_OPEN_CLASSES_MANUAL), wxT("Abrir Manual"), image);
	/* AULAS */
	m_rib_bbars[5][0]->AddButton(LHID_OF(LHN_SEE_LECTURES), wxT("Ver Aulas por Turma"), image);
	m_rib_bbars[5][1]->AddButton(LHID_OF(LHN_ADD_LECTURES), wxT("Adicionar Aulas por Turma"), image);
	m_rib_bbars[5][2]->AddButton(LHID_OF(LHN_LECTURE_PERIOD_PREF), wxT("Preferência por Períodos"), image);
	m_rib_bbars[5][2]->AddButton(LHID_OF(LHN_LECTURE_TEACHER_PREF), wxT("Preferência por Professores"), image);
	m_rib_bbars[5][2]->AddButton(LHID_OF(LHN_LECTURE_TWIN_PREF), wxT("Preferência por Geminação"), image);
	m_rib_bbars[5][2]->AddButton(LHID_OF(LHN_LECTURE_ROOM_PREF), wxT("Preferência por Sala"), image);
	m_rib_bbars[5][3]->AddButton(LHID_OF(LHN_CHECK_ALL_LECTURES), wxT("Conferir Tudo"), image);
	m_rib_bbars[5][4]->AddButton(LHID_OF(LHN_OPEN_LECTURES_MAUAL), wxT("Abrir Manual"), image);
	/* HORÁRIO */
	m_rib_bbars[6][0]->AddButton(LHID_OF(LHN_SEE_TIMETABLE), wxT("Visualizar Horário"), image);
	m_rib_bbars[6][0]->AddButton(LHID_OF(LHN_EXPORT_TIMETABLE), wxT("Exportar Horário"), image);
	m_rib_bbars[6][1]->AddButton(LHID_OF(LHN_GENERATE_TIMETABLE), wxT("Gerar"), image);
	m_rib_bbars[6][1]->AddButton(LHID_OF(LHN_CREATE_TIMETABLE), wxT("Criar Manualmente"), image);
	m_rib_bbars[6][2]->AddButton(LHID_OF(LHN_OPEN_TIMETABLE_MANUAL), wxT("Abrir Manual"), image);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_ribbon, 0, wxEXPAND);


	m_center_pane = new wxPanel(this, wxID_ANY, wxPoint(0,150), wxSize(800,425));
	m_center_pane->SetBackgroundColour(wxColor(0x29, 0x80, 0xb9));
	wxPanel * m_footer = new wxPanel(this, wxID_ANY, wxPoint(0,575), wxSize(800,25));

	sizer->Add(m_center_pane, 1, wxEXPAND);

	m_footer->SetBackgroundColour(wxColor(0x25,0x75,0xb0));
	wxStaticText * m_footer_text = new wxStaticText(m_footer, wxID_ANY,wxT("Copyright (C) Léo Hardt 2019-2020. This program is free software protected by Mozilla Public License v2.0."),
													wxPoint(0,5), wxSize(800,20), wxST_NO_AUTORESIZE |wxALIGN_CENTRE | wxALIGN_CENTRE_HORIZONTAL);
	m_footer_text->SetForegroundColour(wxColor(0xFF, 0xFF, 0xFF));
	m_footer_text->SetFont(*m_owner->m_small_font);

	wxSizer * footer_sizer = new wxBoxSizer(wxHORIZONTAL);
	footer_sizer->Add(m_footer_text,1,wxALIGN_CENTER);
	m_footer->SetSizerAndFit(footer_sizer);

	sizer->Add(m_footer, 0, wxEXPAND);


	m_ribbon->AddPageHighlight(m_ribbon->GetPageCount() - 1);
	m_ribbon->Realize();

	SetSizerAndFit(sizer);
}

MainMenuForm::~MainMenuForm(){

}

void MainMenuForm::CloseOpenedPane(){
	if(m_open_pane != nullptr){
		m_open_pane->Destroy();
		m_open_pane = nullptr;
	}
}


void MainMenuForm::OnMenuItemClicked(wxCommandEvent & ev){
	switch(ev.GetId()){
		case LHID_OF(LHN_ADD_ROOM): {
			CloseOpenedPane();
			m_open_pane = new AddRoomPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_FEATURE): {
			CloseOpenedPane();
			m_open_pane = new AddFeaturePane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_SUBJECT):{
			CloseOpenedPane();
			m_open_pane = new AddSubjectPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_SUBJECT_GROUP):{
			CloseOpenedPane();
			m_open_pane = new AddSubjectGroupPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_TEACHER):{
			CloseOpenedPane();
			m_open_pane = new AddTeacherPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_TEACHER_GROUP):{
			CloseOpenedPane();
			m_open_pane = new AddTeacherGroupPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_CLASS):{
			CloseOpenedPane();
			m_open_pane = new AddClassPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_ADD_CLASS_GROUP):{
			CloseOpenedPane();
			m_open_pane = new AddClassGroupPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_FEATURES):{
			CloseOpenedPane();
			m_open_pane = new ListFeaturesPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_ROOMS):{
			CloseOpenedPane();
			m_open_pane = new ListRoomsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_SUBJECTS):{
			CloseOpenedPane();
			m_open_pane = new ListSubjectsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}

		case LHID_OF(LHN_SEE_SUBJECT_GROUPS):{
			CloseOpenedPane();
			m_open_pane =new ListSubjectGroupsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_TEACHERS):{
			CloseOpenedPane();
			m_open_pane = new ListTeachersPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}

		case LHID_OF(LHN_SEE_TEACHER_GROUPS):{
			CloseOpenedPane();
			m_open_pane = new ListTeacherGroupsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_CLASSES):{
			CloseOpenedPane();
			m_open_pane = new ListClassesPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_SEE_CLASS_GROUPS):{
			CloseOpenedPane();
			m_open_pane = new ListClassGroupsPane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_GENERATE_TIMETABLE):{
			CloseOpenedPane();
			m_open_pane = new GenerateTimetablePane(m_owner, m_center_pane, wxPoint(100,15));
			break;
		}
		case LHID_OF(LHN_OPEN_SCHOOL_MANUAL):{
			if(m_owner->m_window_manual == nullptr){
				m_owner->m_window_manual = new ManualWindow(m_owner);
				m_owner->m_window_manual->Show();
			}
			break;
		}
		default:{
			printf("Event not implemented.\n");
			break;
		}
	}
	if(m_open_pane != nullptr){
		wxSizer * center_sizer = new wxBoxSizer(wxVERTICAL);
		center_sizer->Add(m_open_pane,1, wxEXPAND | wxALL, 15);
		m_center_pane->SetSizerAndFit(center_sizer);
	}
	Layout();
}
