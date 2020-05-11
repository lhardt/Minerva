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
#include <wx/wx.h>
#include "gui.hpp"

bool Application::OnInit(){

	m_title_font = new wxFont(22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, true);
	m_text_font  = new wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_small_font = new wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_page_title_font = new wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, true);

	m_form_welcome = new WelcomeForm(this);
	m_form_welcome->Show();
	return true;
}

void Application::SwitchForm(AppFormType next){
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
	}
}


wxIMPLEMENT_APP(Application);
