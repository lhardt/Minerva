/*
 * Project Minerva.
 *   main.cpp - program initial point.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains functions that start the program.
 */
#include "gui.hpp"

#include <wx/wx.h>

extern "C" {
	#include "loader.h"
	#include "util.h"
};

wxDEFINE_EVENT(DATA_CHANGE_EVENT, wxNotifyEvent);

bool Application::Do(Action * action){
	if(m_actions.Do(action)){
		if(m_form_main_menu){
			m_form_main_menu->NotifyNewAction(action);
		}
		return true;
	}
	return false;
}
bool Application::Undo(){
	if(m_actions.Undo()){
		this->NotifyNewUnsavedData();
		return true;
	}
	return false;
}


void Application::NotifyNewUnsavedData(){
	if(m_form_main_menu){
		m_form_main_menu->NotifyNewUnsavedData();
	}
}

void Application::OnConfigUpdate(){
	FILE * conf_file = fopen(".config", "w");

	if(conf_file){
		int lang_code;

		lang_code = (m_lang == &LANG_POR)?0:
		 			(m_lang == &LANG_ENG)?1:
					(m_lang == &LANG_SPA)?2:
					(m_lang == &LANG_DEU)?3:99;

		fprintf(conf_file,"%d;%d", lang_code, m_font_sz);

		fclose(conf_file);
	}
	UpdateFonts();
}

void Application::LoadConfig(){
	int lang;
	FILE * conf_file = fopen(".config", "r");
	if(conf_file){
		fscanf(conf_file,"%d;%d", &lang, &m_font_sz);

		switch(lang){
			case 0:{
				m_lang = &LANG_POR;
				break;
			}
			case 1:{
				m_lang = &LANG_ENG;
				break;
			}
			case 2:{
				m_lang = &LANG_SPA;
				break;
			}
			case 3:{
				m_lang = &LANG_DEU;
				break;
			}
			default:{
				printf("wtf dude\n");
			}
		}

		fclose(conf_file);
	} else {
		m_lang = &LANG_POR;
		m_font_sz = 10;
	}
	OnConfigUpdate();
}

void Application::UpdateFonts(){

	if(m_title_font != nullptr){
		delete m_title_font;
		delete m_page_title_font;
		delete m_bold_text_font;
		delete m_text_font;
		delete m_user_text_font;
		delete m_small_font;
	}
	m_title_font = new wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, true);
	m_page_title_font = new wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	m_bold_text_font  = new wxFont(m_font_sz, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	m_user_text_font  = new wxFont(m_font_sz, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_text_font  = new wxFont(m_font_sz, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_small_font = new wxFont(m_font_sz-2, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
}

char * copy_wx_string(wxString str){
	int len = strlen(str.ToUTF8().data());
	char * copy = (char*) calloc(len + 1, sizeof(char*));
	strncpy(copy, str.ToUTF8().data(), len);
	return copy;
}

IntPairClientData::IntPairClientData(int v1, int v2):
		m_v1(v1), m_v2(v2){
}

IntClientData::IntClientData(int value):
		m_value(value), m_name(""){
}

IntClientData::IntClientData(int value, wxString name):
		m_value(value), m_name(name){
}

bool Application::OnInit(){
	int errc;
	bool loaded;

	errc = sqlite3_open(":memory:",&m_database);
	loaded = load_backup(m_database, "./Database.db");

	if(m_database != nullptr && loaded && (errc == SQLITE_OK)){
		init_all_tables(stdout, m_database);

		m_island_image = new wxImage(400,400);
		m_island_image->AddHandler(new wxPNGHandler);
		m_island_image->LoadFile("res/floating.png", wxBITMAP_TYPE_PNG);

		LoadConfig();

		m_form_welcome = new WelcomeForm(this);
		m_form_welcome->Show();
		SetExitOnFrameDelete(true);
		return true;
	} else {
		printf("Could open in-memory-db? %d %s\n", errc, sqlite3_errmsg(m_database));
		printf("Could load back-up? %s\n", loaded?("Yes"):("No"));
		if(m_database == nullptr){
			int err_response = wxMessageBox(wxT("Erro!"),wxT("Não foi possível abrir o banco de dados."), wxOK);
			Exit();
		}

		return false;
	}
}

bool Application::SaveDatabase(){
	return save_backup(m_database, "./Database.db");
}

void Application::SwitchForm(AppFormType next){
	if(m_form_welcome != nullptr){
		m_form_welcome->Destroy();
		m_form_welcome = nullptr;
	}
	if(m_form_create_school != nullptr){
		m_form_create_school->Destroy();
		m_form_create_school = nullptr;
	}
	if(m_form_main_menu != nullptr){
		m_form_main_menu->Destroy();
		m_form_main_menu = nullptr;
	}
	if(m_form_settings != nullptr){
		m_form_settings->Destroy();
		m_form_settings = nullptr;
	}
	switch(next){
		case FORM_WELCOME:{
			m_form_welcome = new WelcomeForm(this);
			m_form_welcome->Show();
			break;
		}
		case FORM_CREATE_SCHOOL:{
			m_form_create_school = new CreateSchoolForm(this);
			m_form_create_school->Show();
			break;
		}
		case FORM_MAIN_MENU:{
			m_form_main_menu = new MainMenuForm(this);
			m_form_main_menu->Show();
			break;
		}
		case FORM_SETTINGS:{
			m_form_settings = new SettingsForm(this);
			m_form_settings->Show();
			break;
		}
		default: {
			printf("No form under this name.\n");
		}
	}
}

int Application::OnExit(){
	sqlite3_close(m_database);

	delete m_title_font;
	delete m_bold_text_font;
	delete m_user_text_font;
	delete m_text_font;
	delete m_small_font;
	delete m_page_title_font;
	delete m_island_image;

	if(m_school != NULL){
		free_school(m_school);
	}
}


wxIMPLEMENT_APP(Application);
