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

#include <sqlite3.h>

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/ribbon/bar.h>

extern "C"{
	#include "types.h"
};

char * copy_wx_string(wxString str);

class Application;

enum AppFormType {
	FORM_NULL = 0,
	FORM_WELCOME,
	FORM_CREATE_SCHOOL,
	FORM_MAIN_MENU
};

class ChoiceGrid : public wxGrid {
 public:
	ChoiceGrid(wxWindow * parent, wxWindowID id = wxID_ANY, wxPoint position = wxDefaultPosition, wxSize size = wxDefaultSize);
	~ChoiceGrid();

	int m_n_cols;
	int m_n_rows;

	wxString m_immutable_cell_text;
	wxColor  m_immutable_cell_color;

	wxString m_basic_col_name = L"Coluna";
	wxString m_basic_row_name = L"Linha";
	wxVector<wxString> m_col_names;
	wxVector<wxString> m_row_names;

	wxVector<wxString> m_value_names;
	wxVector<wxColor>  m_background_colors;
	wxVector<int> possible_values;

	wxVector<int> GetValues(int i_day);
	wxVector<int> GetValues();

	void GridRemake(int n_cols, int n_rows);
	void SetPossibleValues(wxVector<wxString> values);
	void SetBackgroundColors(wxVector<wxColor> values);
	void SetCellImmutable(int i_col, int i_row);
 private:
	void OnLeftClick(wxGridEvent &);
};

class WelcomeForm : public wxFrame {
 public:
	WelcomeForm(Application * owner);
	~WelcomeForm();

 private:
	/* Components */
	wxButton * 		m_button_create = nullptr;
	wxButton * 		m_button_delete = nullptr;
	wxButton * 		m_button_import = nullptr;
	wxButton * 		m_button_open   = nullptr;
	wxChoice *	 	m_dropdown = nullptr;
	wxStaticBitmap * m_image;

	Application * m_owner;

	int * 			m_school_ids;
	char * * 		m_school_names;



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
	ChoiceGrid *	m_grid = nullptr;
	wxSpinCtrl * 	m_number_of_periods_text = nullptr;
	wxSpinCtrl * 	m_number_of_days_text = nullptr;
	wxTextCtrl * 	m_school_name_text = nullptr;

	wxStaticText* 	m_err_msg = nullptr;

	Application * m_owner;


	/* Handlers */
	void OnCreateClicked(wxCommandEvent &);
	void OnBackClicked(wxCommandEvent &);
	void OnGridLeftClick(wxGridEvent &);
	void OnGridSizeUpdated(wxSpinEvent &);

 private:
};

class MainMenuForm : public wxFrame{
 public:
	MainMenuForm(Application * owner);
	~MainMenuForm();

	wxRibbonBar * m_ribbon;
	wxRibbonPage * m_ribbon_pages[7];

	wxPanel * m_open_pane = nullptr;
	wxPanel * m_center_pane = nullptr;
	Application * m_owner = nullptr;

 private:
	void OnMenuItemClicked(wxCommandEvent&);

	void CloseOpenedPane();
};

/* ADD Panes. */

class AddFeaturePane : public wxPanel {
 public:
	AddFeaturePane(Application * owner, wxWindow * parent, wxPoint pos);
	~AddFeaturePane();

	wxTextCtrl * m_name_text;

	wxStaticText * m_err_msg;

	Application * m_owner;
 private:
	void OnCreateButtonClicked(wxCommandEvent &);
};

class AddRoomPane : public wxScrolledWindow {
 public:
	AddRoomPane(Application * owner, wxWindow * parent, wxPoint pos);
	~AddRoomPane();

 private:
	wxTextCtrl * m_name_text;
	wxSpinCtrl * m_capacity_text;
	ChoiceGrid * m_grid;

	wxChoice   * m_features;
	wxSpinCtrl * m_score_text;
	wxListBox  * m_added_features;

	wxStaticText * m_err_msg;

	Application * m_owner;

	int m_feature_values[MAX_FEATURES];

	void OnCreateButtonClicked(wxCommandEvent &);
	void OnAddFeatureClicked(wxCommandEvent &);
};

class AddSubjectPane : public wxScrolledWindow {
 public:
	AddSubjectPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddSubjectPane();


 private:

	wxTextCtrl * m_name_text;
	Application * m_owner;
	wxStaticText * m_err_msg;

	void OnCreateButtonClicked(wxCommandEvent &);

};

class AddSubjectGroupPane : public wxScrolledWindow	 {
 public:
	AddSubjectGroupPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddSubjectGroupPane();

	wxTextCtrl * m_name_text;
	wxComboBox * m_subjects;
	wxListBox * subjects_list_box;

	Application * m_owner;
 private:
	void OnCreateButtonClicked(wxCommandEvent &);
};

class AddTeacherPane : public wxScrolledWindow {
 public:
	AddTeacherPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddTeacherPane();
 private:

	wxTextCtrl * m_name_text;
	wxComboBox * m_all_subjects_list;
	wxListBox  * m_teaches_subjects_list;
	wxStaticText * m_err_msg;
	ChoiceGrid * m_grid;

	bool m_teaches_subj[MAX_SUBJECTS];

	Application * m_owner;

	void OnAddTeacherButtonClicked(wxCommandEvent &);
	void OnAddSubjectButtonClicked(wxCommandEvent &);
	void OnRemoveAllButtonClicked(wxCommandEvent &);
};

class AddTeacherGroupPane : public wxScrolledWindow {
 public:
	AddTeacherGroupPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddTeacherGroupPane();
 private:

	wxTextCtrl * m_name_text;
	wxComboBox * m_all_teachers_list;
	wxListBox  * m_selected_teachers_list;

	Application * m_owner;

	void OnAddButtonClicked(wxCommandEvent &);
};

class AddClassPane : public wxScrolledWindow {
 public:
	AddClassPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddClassPane();
 private:

	wxTextCtrl * m_name_text;
	ChoiceGrid * m_periods;
	wxComboBox * m_all_subjects_list;
	wxListBox  * m_selected_subjects_list;

	Application * m_owner;

	void OnAddButtonClicked(wxCommandEvent &);

};

class AddClassGroupPane : public wxScrolledWindow {
 public:
	AddClassGroupPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddClassGroupPane();
 private:
	wxTextCtrl * m_name_text;
	wxComboBox * m_all_classes_list;
	wxListBox  * m_selected_classes_list;
	Application * m_owner;

	void OnAddButtonClicked(wxCommandEvent &);
};

/* LIST Panes. */

class DescSchoolPane : public wxScrolledWindow {
public:
	DescSchoolPane(Application * owner, wxWindow * parent, wxPoint pos);
	~DescSchoolPane();

private:

	Application * m_owner;
};

class ListFeaturesPane : public wxScrolledWindow {
 public:
	ListFeaturesPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListFeaturesPane();
 private:

	wxListBox * m_features_list;
	Application * m_owner;
};

class ListRoomsPane : public wxScrolledWindow {
 public:
   ListRoomsPane(Application * owner, wxWindow * parent, wxPoint pos);
   ~ListRoomsPane();
 private:

   wxListBox * m_rooms_list;

   Application * m_owner;
};

class ListSubjectsPane : public wxScrolledWindow {
 public:
	ListSubjectsPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListSubjectsPane();
 private:
	Application * m_owner;
	wxListBox * m_subjects_list;

};

class ListSubjectGroupsPane : public wxScrolledWindow {
 public:
	ListSubjectGroupsPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListSubjectGroupsPane();
 private:
	Application * m_owner;
};

class ListTeachersPane : public wxScrolledWindow {
 public:
	ListTeachersPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListTeachersPane();
 private:
   Application * m_owner;
};

class ListTeacherGroupsPane : public wxScrolledWindow {
 public:
	ListTeacherGroupsPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListTeacherGroupsPane();
 private:
   Application * m_owner;
};

class ListClassesPane : public wxScrolledWindow {
 public:
	ListClassesPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListClassesPane();
 private:

	wxListBox * m_classes_list;

	Application * m_owner;
};

class ListClassGroupsPane : public wxScrolledWindow {
 public:
	ListClassGroupsPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListClassGroupsPane();
 private:
   Application * m_owner;
};

/* Application */

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

	sqlite3 *		m_database = nullptr;
	School *		m_school = nullptr;
};


#endif /* GUI_H */
