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
#include <wx/notebook.h>
#include <wx/artprov.h>
#include <wx/event.h>

#include "gui_language.hpp"


extern "C"{
	#include "types.h"
};

char * copy_wx_string(wxString str);

class IntPairClientData : public wxClientData{
public:
	IntPairClientData(int v1, int v2);
	int m_v1;
	int m_v2;
};

class IntClientData : public wxClientData{
public:
	IntClientData(int value);
	IntClientData(int value, wxString name);
	int m_value;
	wxString m_name;
};

class Application;

enum AppFormType {
	FORM_NULL = 0,
	FORM_WELCOME,
	FORM_CREATE_SCHOOL,
	FORM_MAIN_MENU,
	FORM_SETTINGS
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

class HoverToolTip : public wxStaticBitmap {
 public:
	HoverToolTip(wxWindow * parent,
				wxWindowID  id = wxID_ANY,
				wxString txt = wxT(""),
				wxBitmap bmp = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, wxSize(32,32)),
				wxPoint pos = wxDefaultPosition,
				wxSize sz = wxDefaultSize
			);

 	~HoverToolTip();
 private:

	wxString m_text;
	void OnHover(wxMouseEvent& evt);
};

class WelcomeForm : public wxFrame {
 public:
	WelcomeForm(Application * owner);
	~WelcomeForm();

 private:
	/* Components */
	wxButton * 		m_button_create = nullptr;
	wxButton * 		m_button_settings = nullptr;
	wxButton * 		m_button_help = nullptr;
	wxButton * 		m_button_open   = nullptr;
	wxChoice *	 	m_dropdown = nullptr;
	wxStaticBitmap * m_image;

	Application * m_owner;

	/* Handlers */
	void OnCreateClicked(wxCommandEvent &);
	void OnSettingsClicked(wxCommandEvent &);
	void OnOpenClicked(wxCommandEvent &);
	void OnHelpClicked(wxCommandEvent &);
};

class CreateSchoolForm : public wxFrame {
 public:
	CreateSchoolForm(Application * owner);
	~CreateSchoolForm();
 private:
	Application  * m_owner;
	/* Components */
	wxButton     *	m_button_create = nullptr;
	wxButton     *	m_button_back = nullptr;
	ChoiceGrid   *	m_grid = nullptr;
	wxSpinCtrl   * 	m_number_of_periods_text = nullptr;
	wxSpinCtrl   * 	m_number_of_days_text = nullptr;
	wxTextCtrl   * 	m_school_name_text = nullptr;
	wxStaticText * 	m_err_msg = nullptr;

	/* Handlers */
	void OnCreateClicked(wxCommandEvent &);
	void OnBackClicked(wxCommandEvent &);
	void OnGridLeftClick(wxGridEvent &);
	void OnGridSizeUpdated(wxSpinEvent &);
};

class MainMenuForm : public wxFrame{
 public:
	MainMenuForm(Application * owner);
	~MainMenuForm();

	void NotifyNewUnsavedData();
 private:
	wxRibbonBar * m_ribbon;
	wxRibbonPage * m_ribbon_pages[7];

	wxPanel * m_open_pane = nullptr;
	wxPanel * m_center_pane = nullptr;
	Application * m_owner = nullptr;
	wxToolBar * m_toolbar = nullptr;
	void OnMenuItemClicked(wxCommandEvent&);
	void OnToolbarEvent(wxCommandEvent&);
	void OnCloseCommand(wxCommandEvent&);
	void OnCloseClose(wxCloseEvent&);
	bool OnClose();
	void OnSave(wxCommandEvent&);
	void OnUndo(wxCommandEvent&);
	void OnRedo(wxCommandEvent&);

	void CloseOpenedPane();
};

class SettingsForm : public wxFrame {
 public:
	SettingsForm(Application * owner);
	~SettingsForm();
 private:
	Application * m_owner = nullptr;
	wxChoice * m_lang_choice;
	wxChoice * m_font_size_choice;

	void OnBackButtonClicked(wxCommandEvent& evt);
	void OnSaveButtonClicked(wxCommandEvent& evt);
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
	void ClearInsertedData();
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

	void OnRemoveFeatureClicked(wxCommandEvent &);
	void OnRemoveAllClicked(wxCommandEvent &);
	void OnCreateButtonClicked(wxCommandEvent &);
	void OnAddFeatureClicked(wxCommandEvent &);
	void ClearInsertedData();
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
	void ClearInsertedData();
};

class AddSubjectGroupPane : public wxScrolledWindow	 {
 public:
	AddSubjectGroupPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddSubjectGroupPane();

 private:
	wxTextCtrl * m_name_text;
	wxChoice * m_subjects_choice;
	wxListBox * m_subjects_list;
	wxStaticText * m_err_msg;
	Application * m_owner;

	void OnAddSubjectButtonClicked(wxCommandEvent &);
	void OnRemoveSubjectButtonClicked(wxCommandEvent &);
	void OnRemoveAllButtonClicked(wxCommandEvent &);
	void OnCreateButtonClicked(wxCommandEvent &);
	void ClearInsertedData();
};

class AddTeacherPane : public wxScrolledWindow {
 public:
	AddTeacherPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddTeacherPane();
 private:
	wxTextCtrl * m_name_text;
	wxChoice   * m_all_subjects_list;
	wxListBox  * m_teaches_subjects_list;
	wxStaticText * m_err_msg;
	ChoiceGrid * m_grid;
	Application * m_owner;

	void OnAddTeacherButtonClicked(wxCommandEvent &);
	void OnAddSubjectButtonClicked(wxCommandEvent &);
	void OnRemoveAllButtonClicked(wxCommandEvent &);
	void OnRemoveSubjectButtonClicked(wxCommandEvent &);
	void ClearInsertedData();
};

class AddTeacherGroupPane : public wxScrolledWindow {
 public:
	AddTeacherGroupPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddTeacherGroupPane();
 private:
	wxTextCtrl * m_name_text;
	wxChoice * m_all_teachers_list;
	wxListBox  * m_selected_teachers_list;
	wxChoice * m_all_subjects_list;
	wxListBox  * m_selected_subjects_list;
	wxStaticText * m_err_msg;
	Application * m_owner;

	void OnRemoveTeacherButtonClicked(wxCommandEvent & evt);
	void OnRemoveSubjectButtonClicked(wxCommandEvent & evt);
	void OnRemoveAllTeachersButtonClicked(wxCommandEvent &);
	void OnRemoveAllSubjectsButtonClicked(wxCommandEvent &);
	void OnAddTeacherButtonClicked(wxCommandEvent & evt);
	void OnAddSubjectButtonClicked(wxCommandEvent & evt);
	void OnAddGroupButtonClicked(wxCommandEvent &);
	void ClearInsertedData();
};

class AddClassPane : public wxScrolledWindow {
 public:
	AddClassPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddClassPane();
 private:

	wxTextCtrl * m_name_text;
	ChoiceGrid * m_periods;
	wxChoice   * m_all_subjects_list;
	wxListBox  * m_selected_subjects_list;
	wxSpinCtrl * m_score_text;
	wxSpinCtrl * m_size_text;
	wxCheckBox * m_free_periods_checkbox;
	int last_entry, last_exit;


	wxChoice * m_entry_text;
	wxChoice * m_exit_text;

	wxStaticText * m_err_msg;

	Application * m_owner;

	void OnAddSubjectButtonClicked(wxCommandEvent &);
	void OnAddClassButtonClicked(wxCommandEvent &);
	void OnRemoveSubjectButtonClicked(wxCommandEvent & ev);
	void OnRemoveAllButtonClicked(wxCommandEvent & ev);
	void OnPeriodChoice(wxCommandEvent &);
	void ClearInsertedData();
};

class AddClassGroupPane : public wxScrolledWindow {
 public:
	AddClassGroupPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddClassGroupPane();
 private:
	Application * m_owner;

	wxTextCtrl * m_name_text;
	wxChoice * m_all_classes_list;
	wxListBox  * m_selected_classes_list;
	wxStaticText * m_err_msg;

	void OnAddGroupButtonClicked(wxCommandEvent &);
	void OnAddClassButtonClicked(wxCommandEvent &);
	void OnRemoveAllButtonClicked(wxCommandEvent &);
	void ClearInsertedData();
};

class AddLecturePane : public wxScrolledWindow {
 public:
	AddLecturePane(Application * owner, wxWindow * parent, wxPoint pos);
	~AddLecturePane();

 private:
	Application * m_owner;
};

/* LIST Panes. */

class DescSchoolPane : public wxScrolledWindow {
 public:
	DescSchoolPane(Application * owner, wxWindow * parent, wxPoint pos);
	~DescSchoolPane();

 private:
	Application * m_owner;
	void OnRemoveButtonClicked(wxCommandEvent & );
};

class ListFeaturesPane : public wxScrolledWindow {
 public:
	ListFeaturesPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListFeaturesPane();
 private:

	wxStaticText*  m_name_text;


	wxListBox * m_features_list;
	Application * m_owner;

	int m_selected_index;

	void OnEditButtonClicked(wxCommandEvent &);
	void OnRemoveButtonClicked(wxCommandEvent &);
	void OnSelectionChange(wxCommandEvent &);
};

class ListRoomsPane : public wxScrolledWindow {
 public:
   ListRoomsPane(Application * owner, wxWindow * parent, wxPoint pos);
   ~ListRoomsPane();
 private:
	wxListBox * m_rooms_list;
	wxStaticText * m_name_text;
	wxStaticText * m_size_text;
	wxStaticText * m_features_text;
	ChoiceGrid * m_periods_grid;
	wxStaticText * m_err_msg;
	Application * m_owner;

	int m_selected_index;

	void OnEditButtonClicked(wxCommandEvent &);
	void OnDeleteButtonClicked(wxCommandEvent &);
	void OnSelectionChanged(wxCommandEvent &);
};

class ListSubjectsPane : public wxScrolledWindow {
 public:
	ListSubjectsPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListSubjectsPane();
 private:
	Application * m_owner;
	wxListBox * m_subjects_list;
	wxStaticText* m_name_text;

	void OnEditButtonClicked(wxCommandEvent &);
    void OnDeleteButtonClicked(wxCommandEvent &);
    void OnSelectionChanged(wxCommandEvent &);
};

class ListSubjectGroupsPane : public wxScrolledWindow {
 public:
	ListSubjectGroupsPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListSubjectGroupsPane();
 private:
	Application * m_owner;

	wxListBox * m_groups_list;
	wxStaticText * m_name_text;
	wxStaticText * m_members_text;

	void OnEditButtonClicked(wxCommandEvent &);
    void OnDeleteButtonClicked(wxCommandEvent &);
    void OnSelectionChanged(wxCommandEvent &);
};

class ListTeachersPane : public wxScrolledWindow {
 public:
	ListTeachersPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListTeachersPane();
 private:

	wxListBox    * m_teachers_list;
	wxStaticText * m_name_text;
	wxStaticText * m_max_days_text;
	wxStaticText * m_max_periods_text;
	wxStaticText * m_max_ppd_text;
	wxStaticText * m_planning_periods_text;
	wxStaticText * m_teaches_text;
	ChoiceGrid   * m_periods_grid;

	Application * m_owner;

	void OnEditButtonClicked(wxCommandEvent &);
    void OnDeleteButtonClicked(wxCommandEvent &);
    void OnSelectionChanged(wxCommandEvent &);
};

class ListTeacherGroupsPane : public wxScrolledWindow {
 public:
	ListTeacherGroupsPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListTeacherGroupsPane();
 private:
	Application * m_owner;

	wxListBox * m_groups_list;
	wxStaticText * m_name_text;
	wxStaticText * m_members_text;
	wxStaticText * m_err_msg;

	void OnEditButtonClicked(wxCommandEvent &);
	void OnDeleteButtonClicked(wxCommandEvent &);
	void OnSelectionChanged(wxCommandEvent &);
};

class ListClassesPane : public wxScrolledWindow {
 public:
	ListClassesPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListClassesPane();
 private:

	wxListBox 	 * m_classes_list;
	wxStaticText * m_name_text;
	wxStaticText * m_size_text;
	wxStaticText * m_free_periods_text;
	wxStaticText * m_entry_period_text;
	wxStaticText * m_exit_period_text;
	wxStaticText * m_subjects_text;
	ChoiceGrid   * m_periods_grid;

	Application * m_owner;

	void OnSelectionChanged(wxCommandEvent &);
	void OnEditButtonClicked(wxCommandEvent &);
	void OnRemoveButtonClicked(wxCommandEvent &);
};

class ListClassGroupsPane : public wxScrolledWindow {
 public:
	ListClassGroupsPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListClassGroupsPane();
 private:
	Application * m_owner;

	wxListBox * m_groups_list;
	wxStaticText * m_name_text;
	wxStaticText * m_members_text;
	ChoiceGrid * m_periods_grid;
	wxStaticText * m_err_msg;

	void OnSelectionChanged(wxCommandEvent &);
	void OnEditButtonClicked(wxCommandEvent &);
	void OnDeleteButtonClicked(wxCommandEvent &);
};

class ListLecturesPane : public wxScrolledWindow {
 public:
	ListLecturesPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListLecturesPane();

 private:
	Application * m_owner;
};

/* PREFERENCE Panes. */

class RoomPeriodPrefPane : public wxScrolledWindow {
 public:
	RoomPeriodPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~RoomPeriodPrefPane();

 private:
	Application * m_owner;

	wxChoice * m_room_choice;
};

class SubjectPeriodPrefPane : public wxScrolledWindow {
 public:
	SubjectPeriodPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~SubjectPeriodPrefPane();

 private:
	Application * m_owner;
};

class SubjectRoomPrefPane : public wxScrolledWindow {
 public:
	SubjectRoomPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~SubjectRoomPrefPane();

 private:
	Application * m_owner;
};

class TeacherSubjPrefPane : public wxScrolledWindow {
 public:
	TeacherSubjPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~TeacherSubjPrefPane();

 private:
	Application * m_owner;
};

class TeacherTwinPrefPane : public wxScrolledWindow {
public:
	TeacherTwinPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~TeacherTwinPrefPane();
private:
	Application * m_owner;
};

class TeacherPeriodPrefPane : public wxScrolledWindow {
 public:
	TeacherPeriodPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~TeacherPeriodPrefPane();
 private:
	Application * m_owner;
};

class TeacherRoomPrefPane : public wxScrolledWindow {
 public:
	TeacherRoomPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~TeacherRoomPrefPane();

 private:
	Application * m_owner;
};

class ClassPeriodPrefPane : public wxScrolledWindow {
 public:
	ClassPeriodPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ClassPeriodPrefPane();
 private:
	Application * m_owner;
};

class ClassTeacherPrefPane : public wxScrolledWindow {
 public:
	ClassTeacherPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ClassTeacherPrefPane();

 private:
	Application * m_owner;
};

class ClassRoomPrefPane : public wxScrolledWindow {
 public:
	ClassRoomPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ClassRoomPrefPane();

 private:
	Application * m_owner;
};

class LecturePeriodPrefPane : public wxScrolledWindow {
 public:
	LecturePeriodPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~LecturePeriodPrefPane();

 private:
	Application * m_owner;
};

class LectureTeacherPrefPane : public wxScrolledWindow {
 public:
	LectureTeacherPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~LectureTeacherPrefPane();

 private:
	Application * m_owner;
};

class LectureTwinPrefPane : public wxScrolledWindow {
 public:
	LectureTwinPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~LectureTwinPrefPane();

 private:
	Application * m_owner;
};

class LectureRoomPrefPane : public wxScrolledWindow {
 public:
	LectureRoomPrefPane(Application * owner, wxWindow * parent, wxPoint pos);
	~LectureRoomPrefPane();

 private:
	Application * m_owner;
};

/* GENERATION Panes. */

class GenerateTimetablePane : public wxScrolledWindow {
 public:
	GenerateTimetablePane(Application * owner, wxWindow * parent, wxPoint pos);
	~GenerateTimetablePane();
 private:
	Application * m_owner;
	wxStaticText * m_err_msg;
	wxTextCtrl * m_tt_name_text;
	void OnButtonClicked(wxCommandEvent &);
};

class DescTimetablePane : public wxScrolledWindow {
 public:
	DescTimetablePane(Application * owner, wxWindow * parent, wxPoint pos);
	~DescTimetablePane();
 private:
	Application * m_owner;

	wxChoice * m_solution_choice;
	wxChoice * m_class_choice;
	wxChoice * m_teacher_choice;
	wxChoice * m_subject_choice;
	wxChoice * m_room_choice;
	wxGrid   * m_grid;

	void OnRedrawGridRequest(wxCommandEvent &);
};

/* Manual */

class ManualWindow : public wxFrame {
 public:
	ManualWindow(Application * owner);
	~ManualWindow();

 private:
	Application * m_owner;
	wxNotebook * m_notebook;

	wxWindow * PopulateSchoolWindow();

	wxWindow * PopulateRoomsWindow();
	wxWindow * PopulateSubjectsWindow();
	wxWindow * PopulateTeachersWindow();
	wxWindow * PopulateClassesWindow();
	wxWindow * PopulateLecturesWindow();
	wxWindow * PopulateTimetableWindow();
	wxWindow * PopulateSoftwareWindow();
};

/* Application */

class Application : public wxApp {
 private:
	WelcomeForm * 		m_form_welcome = nullptr;
	CreateSchoolForm * 	m_form_create_school = nullptr;
	MainMenuForm * 		m_form_main_menu = nullptr;
	SettingsForm * 	m_form_settings = nullptr;
 public:
	virtual bool OnInit();
	virtual int  OnExit();
	void SwitchForm(AppFormType next);
	void ChildNotify(int messageCode);
	void OnConfigUpdate();
	void LoadConfig();
	void UpdateFonts();
	bool SaveDatabase();
	void NotifyNewUnsavedData();

	const Language * m_lang;
	int m_font_sz;

	ManualWindow *		m_window_manual = nullptr;
	wxFont * 		m_title_font = nullptr;
	wxFont * 		m_page_title_font = nullptr;
	wxFont * 		m_small_font = nullptr;
	wxFont * 		m_bold_text_font = nullptr;
	wxFont * 		m_user_text_font = nullptr;
	wxFont * 		m_text_font = nullptr;

	wxImage *		m_island_image = nullptr;

	sqlite3 *		m_database = nullptr;
	School *		m_school = nullptr;
};


#endif /* GUI_H */
