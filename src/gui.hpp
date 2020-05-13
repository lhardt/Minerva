/*
 * Project Minerva.
 *   gui.hpp - graphical user interface basic structures.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains general structures for the graphical user interface.
 */
#ifndef GUI_H
#define GUI_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/ribbon/bar.h>


class Application;

enum AppFormType {
	FORM_NULL = 0,
	FORM_WELCOME,
	FORM_CREATE_SCHOOL,
	FORM_MAIN_MENU
};

class WelcomeForm : public wxFrame {
public:
	WelcomeForm(Application * owner);
	~WelcomeForm();

	/* Components */
	wxButton * 		m_button_create = nullptr;
	wxButton * 		m_button_delete = nullptr;
	wxButton * 		m_button_import = nullptr;
	wxButton * 		m_button_open   = nullptr;
	wxComboBox * 	m_dropdown = nullptr;
	wxStaticBitmap * m_image;

	Application * m_owner;

	/* Handlers */
	void OnButtonClicked(wxCommandEvent&);
	void OnQuitClicked(wxCommandEvent &);
	void OnCreateClicked(wxCommandEvent &);
	void OnDeleteClicked(wxCommandEvent &);
	void OnImportClicked(wxCommandEvent &);
	void OnOpenClicked(wxCommandEvent &);
};

class CreateSchoolForm : public wxFrame {
public:
	CreateSchoolForm(Application * owner);
	~CreateSchoolForm();
	/* Components */
	wxButton *		m_button_create = nullptr;
	wxButton *		m_button_back = nullptr;
	wxGrid * 		m_grid = nullptr;
	wxSpinCtrl * 	m_number_of_periods_text = nullptr;
	wxSpinCtrl * 	m_number_of_days_text = nullptr;

	Application * m_owner;


	/* Handlers */
	void OnCreateClicked(wxCommandEvent &);
	void OnBackClicked(wxCommandEvent &);
	void OnGridLeftClick(wxGridEvent &);
	void OnGridSizeUpdated(wxSpinEvent &);

private:
	void GridRemake();
};

class MainMenuForm : public wxFrame{
public:
	MainMenuForm(Application * owner);
	~MainMenuForm();

	wxRibbonBar * m_ribbon;
	wxRibbonPage * m_ribbon_pages[7];

	wxPanel * m_open_pane = nullptr;

	Application * m_owner = nullptr;

private:
	void OnCreateCharacteristicClicked(wxCommandEvent &);
	void OnCreateRoomClicked(wxCommandEvent &);

	void OnMenuItemClicked(wxCommandEvent&);
};

class CreateCharacteristicPane : public wxPanel {
public:
	CreateCharacteristicPane(Application * owner, wxWindow * parent, wxPoint pos);
	~CreateCharacteristicPane();



	Application * m_owner;
private:
};

class CreateRoomPane : public wxPanel {
public:
	CreateRoomPane(Application * owner, wxWindow * parent, wxPoint pos);
	~CreateRoomPane();



	Application * m_owner;
private:
};

class Application : public wxApp {
private:
	WelcomeForm * 		m_form_welcome = nullptr;
	CreateSchoolForm * 	m_form_create_school = nullptr;
	MainMenuForm * 		m_form_main_menu = nullptr;


public:
	virtual bool OnInit();
	void SwitchForm(AppFormType next);
	void ChildNotify(int messageCode);

	wxFont * 		m_title_font = nullptr;
	wxFont * 		m_page_title_font = nullptr;
	wxFont * 		m_text_font = nullptr;
	wxFont * 		m_small_font = nullptr;

	wxImage *		m_island_image = nullptr;
};


#endif /* GUI_H */
