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
#include <wx/hyperlink.h>

#include "gui_language.hpp"


extern "C" {
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

/* Grid table that only accepts positive integers. */
class PosIntGridTable : public wxGridTableBase{
 public:
	PosIntGridTable(int n_rows, int n_cols);
	~PosIntGridTable();
	virtual int GetNumberRows();
	virtual int GetNumberCols();
	virtual bool IsEmptyCell( int row, int col );
	virtual wxString GetValue( int row, int col );
	virtual void SetValue( int row, int col, const wxString& value );
	virtual bool AppendRows(unsigned long n_new_rows);
	virtual bool AppendCols(unsigned long n_new_cols);
	virtual void SetColLabelValue(int col, wxString & str);
	virtual void SetRowLabelValue(int row, wxString & str);
	virtual wxString GetColLabelValue(int col);
	virtual wxString GetRowLabelValue(int row);
 private:
	int n_rows;
	int n_cols;
	int * values;
	wxVector<wxString> col_labels;
	wxVector<wxString> row_labels;
};

class Notification : public wxPanel {
 public:
	Notification(Application * owner, wxWindow * parent, wxWindowID id, wxString text, wxPoint position = wxDefaultPosition, wxSize size = wxDefaultSize);
	Notification(Application * owner, wxWindow * parent, wxWindowID id, wxString text,  wxString action_str, wxPoint position = wxDefaultPosition, wxSize size = wxDefaultSize);

	void Start(int ms = 5000);
	wxHyperlinkCtrl * GetAction();
	wxTimer * GetTimer();
 private:
	wxStaticText * m_text;
	wxHyperlinkCtrl * m_action;
	wxTimer * m_timer;
	/* Just to prevent skipping of the event. */
	void OnHyperlikAction(wxHyperlinkEvent &);
	void OnCloseTimer(wxTimerEvent &);
	void Close();
};

class ChoiceGrid : public wxGrid {
 public:
	ChoiceGrid(Application * owner, wxWindow * parent, wxWindowID id = wxID_ANY, wxPoint position = wxDefaultPosition, wxSize size = wxDefaultSize);
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

	bool m_can_user_click = true;

	wxVector<int> GetValues(int i_day);
	wxVector<int> GetValues();

	void GridRemake(int n_cols, int n_rows);
	void SetPossibleValues(wxVector<wxString> values);
	void SetColName(int i_col, wxString name);
	void SetRowName(int i_row, wxString name);
	void SetBackgroundColors(wxVector<wxColor> values);
	void SetCellImmutable(int i_col, int i_row);
	void SetCellState(int i_row, int i_col, int state);
	void SetAllCellsState(int state);
	int  GetCellState(int i_row, int i_col);
	int  AddState(wxString state_name, wxColor state_value);
	void SetCanUserClick(bool can_user_click);
 private:
	void OnLeftClick(wxGridEvent &);
	Application * m_owner;
};

class ScoreGridPane : public wxScrolledWindow {
 public:
	~ScoreGridPane();
	ScoreGridPane(Application * owner,
				  wxWindow * parent,
				  wxWindowID id = wxID_ANY,
				  wxPoint pos = wxDefaultPosition,
				  wxSize sz = wxDefaultSize);

	/*NOTE: You have to .skip() those events in client code. */
	void 		  OnEditButtonClicked(wxCommandEvent &);
	void		  OnCancelButtonClicked(wxCommandEvent &);
	wxButton 	* GetCancelButton();
	wxButton 	* GetEditButton();
	ChoiceGrid  * GetGrid();
 private:
	Application * m_owner;
	ChoiceGrid  * m_grid;
	wxButton 	* m_edit_btn;
	wxButton 	* m_cancel_btn;
};

class PosIntGridPane : public wxScrolledWindow {
 public:
	~PosIntGridPane();
	PosIntGridPane(Application * owner,
					 wxWindow * parent,
					 wxWindowID id = wxID_ANY,
					 wxPoint pos = wxDefaultPosition,
					 wxSize sz = wxDefaultSize,
				 	 wxString column_name = wxT("Col"),
					 wxVector<wxString> row_names = wxVector<wxString>());
	/*NOTE: You have to .skip() those events in client code. */
	void 		  OnEditButtonClicked(wxCommandEvent &);
	void		  OnCancelButtonClicked(wxCommandEvent &);
	wxButton 	* GetCancelButton();
	wxButton 	* GetEditButton();
	wxGrid 		* GetGrid();
	int		 	* GetValues();
	void	 	  SetValues(int * values);
 private:
	Application * m_owner;
	wxGrid		* m_grid;
	wxButton 	* m_edit_btn;
	wxButton 	* m_cancel_btn;
};

class StringGridPane : public wxScrolledWindow {
 public:
	~StringGridPane();
	StringGridPane(Application * owner,
					 wxWindow * parent,
					 wxWindowID id = wxID_ANY,
					 wxPoint pos = wxDefaultPosition,
					 wxSize sz = wxDefaultSize,
				 	 wxString column_name = wxT("Col"),
					 wxVector<wxString> row_names = wxVector<wxString>());
	/*NOTE: You have to .skip() those events in client code. */
	void 		  OnEditButtonClicked(wxCommandEvent &);
	void		  OnCancelButtonClicked(wxCommandEvent &);
	wxButton 	* GetCancelButton();
	wxButton 	* GetEditButton();
	wxGrid		* GetGrid();
	wxVector<wxString> GetValues();
	void 		  SetValues(wxVector<wxString> values);
 private:
	Application * m_owner;
	wxGrid		* m_grid;
	wxButton 	* m_edit_btn;
	wxButton 	* m_cancel_btn;
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
	void AddNotification(Notification* n);
	void OnNotificationAction(wxHyperlinkEvent & ev);
	void OnNotificationTimer(wxTimerEvent & ev);

 private:
	wxRibbonBar * m_ribbon;
	wxRibbonPage * m_ribbon_pages[7];
	wxSizer* m_sizer; /* To add notifications */

	wxStaticText * m_page_title;
	wxStaticText * m_page_detail;

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

class AddRoomPane : public wxScrolledWindow {
 public:
	AddRoomPane(Application * owner, wxWindow * parent, wxPoint pos);
	~AddRoomPane();

 private:
	wxTextCtrl * m_name_text;
	wxSpinCtrl * m_capacity_text;
	ChoiceGrid * m_grid;
	wxStaticText * m_err_msg;

	Application * m_owner;

	void OnCreateButtonClicked(wxCommandEvent &);
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
	wxTextCtrl 		* m_name_text;
	ChoiceGrid 		* m_subjects_grid;
	wxStaticText 	* m_err_msg;
	Application 	* m_owner;
	void OnCreateButtonClicked(wxCommandEvent &);
	void ClearInsertedData();
};

class AddTeacherPane : public wxScrolledWindow {
 public:
	AddTeacherPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddTeacherPane();
 private:
	Application  * m_owner;

	wxTextCtrl   * m_name_text;
	ChoiceGrid   * m_subjects_grid;
	ChoiceGrid   * m_periods_grid;
	wxStaticText * m_err_msg;

	void OnAddTeacherButtonClicked(wxCommandEvent &);
	void ClearInsertedData();
};

class AddTeacherGroupPane : public wxScrolledWindow {
 public:
	AddTeacherGroupPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddTeacherGroupPane();
 private:
	wxTextCtrl * m_name_text;
	ChoiceGrid * m_teachers_grid;
	ChoiceGrid * m_subjects_grid;
	wxStaticText * m_err_msg;
	Application * m_owner;
	void OnAddGroupButtonClicked(wxCommandEvent &);
	void ClearInsertedData();
};

class AddClassPane : public wxScrolledWindow {
 public:
	AddClassPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddClassPane();
 private:
	wxTextCtrl 		* m_name_text;
	ChoiceGrid 		* m_periods;
	wxSpinCtrl 		* m_size_text;
	wxCheckBox 		* m_free_periods_checkbox;
	wxChoice 		* m_entry_text;
	wxChoice 		* m_exit_text;
	wxStaticText 	* m_err_msg;
	wxGrid 			* m_subjects_grid;
	Application * m_owner;
	int last_entry, last_exit;

	void OnAddClassButtonClicked(wxCommandEvent &);
	void OnPeriodChoice(wxCommandEvent &);
	void ClearInsertedData();
};

class AddClassGroupPane : public wxScrolledWindow {
 public:
	AddClassGroupPane(Application * owner, wxWindow* parent, wxPoint pos);
	~AddClassGroupPane();
 private:
	Application * m_owner;

	wxTextCtrl 		* m_name_text;
	wxStaticText 	* m_err_msg;
	ChoiceGrid		* m_classes_grid;
	wxGrid			* m_subjects_grid;

	void OnAddGroupButtonClicked(wxCommandEvent &);
	void ClearInsertedData();
};

/* LIST Panes. */

class DescSchoolPane : public wxScrolledWindow {
 public:
	DescSchoolPane(Application * owner, wxWindow * parent, wxPoint pos);
	~DescSchoolPane();

 private:
	Application * m_owner;
	wxButton * m_cancel_button;
	wxButton * m_edit_button;
	wxTextCtrl * m_name_text;
	wxNotebook * m_notebook;

	void OnRemoveButtonClicked(wxCommandEvent & );
	void OnCancelButtonClicked(wxCommandEvent & );
	void OnEditButtonClicked(wxCommandEvent & );
	wxScrolledWindow * MakeStatisticsPane();
};

class ListRoomsPane : public wxScrolledWindow {
 public:
   ListRoomsPane(Application * owner, wxWindow * parent, wxPoint pos);
   ~ListRoomsPane();
 private:
	wxListBox * m_rooms_list;
	wxTextCtrl * m_name_text;
	wxSpinCtrl * m_size_text;
	wxCheckBox * m_active_text;
	wxStaticText * m_err_msg;
	Application * m_owner;
	ScoreGridPane * m_periods;
	wxButton 	 * m_edit_btn;
	wxButton  	 * m_cancel_btn;

	int m_selected_index;

	void OnEditButtonClicked(wxCommandEvent &);
	void OnCancelButtonClicked(wxCommandEvent &);
	void OnDeleteButtonClicked(wxCommandEvent &);
	void OnSelectionChanged(wxCommandEvent &);
};

class ListSubjectsPane : public wxScrolledWindow {
 public:
	ListSubjectsPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListSubjectsPane();
 private:
	Application	* m_owner;
	wxListBox	* m_subjects_list;
	wxTextCtrl	* m_name_text;
	wxButton	* m_edit_btn;
	wxButton 	* m_cancel_btn;

	void OnEditButtonClicked(wxCommandEvent &);
	void OnCancelButtonClicked(wxCommandEvent &);
    void OnDeleteButtonClicked(wxCommandEvent &);
    void OnSelectionChanged(wxCommandEvent &);
};

class ListSubjectGroupsPane : public wxScrolledWindow {
 public:
	ListSubjectGroupsPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListSubjectGroupsPane();
 private:
	Application 	* m_owner;
	wxListBox 		* m_groups_list;
	wxTextCtrl	 	* m_name_text;
	ScoreGridPane 	* m_members;
	wxButton	 	* m_cancel_btn;
	wxButton	 	* m_edit_btn;


	void OnCancelButtonClicked(wxCommandEvent &);
	void OnEditButtonClicked(wxCommandEvent &);
    void OnDeleteButtonClicked(wxCommandEvent &);
    void OnSelectionChanged(wxCommandEvent &);
};

class ListTeachersPane : public wxScrolledWindow {
 public:
	ListTeachersPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListTeachersPane();
 private:
	wxListBox     * m_teachers_list;
	wxTextCtrl    * m_name_text;
	wxSpinCtrl	  * m_max_days_text;
	wxSpinCtrl    * m_max_periods_text;
	wxSpinCtrl	  * m_max_ppd_text;
	wxSpinCtrl    * m_planning_periods_text;
	wxCheckBox    * m_active_text;
	ScoreGridPane * m_periods;
	ScoreGridPane * m_teaches;
	PosIntGridPane * m_days;
	ScoreGridPane * m_rooms;
	ScoreGridPane * m_planning_twinning;
	wxButton 	  * m_cancel_btn;
	wxButton 	  * m_edit_btn;

	Application * m_owner;

	void OnEditButtonClicked(wxCommandEvent &);
	void OnCancelButtonClicked(wxCommandEvent &);
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
	wxTextCtrl   * m_name_text;
	wxSpinCtrl   * m_size_text;
	wxCheckBox   * m_free_periods_text;
	wxChoice 	 * m_entry_period_text;
	wxChoice     * m_exit_period_text;
	wxStaticText * m_subjects_text;
	wxCheckBox   * m_active_text;
	ScoreGridPane * m_periods;
	ScoreGridPane * m_rooms;
	PosIntGridPane * m_assignments;
	PosIntGridPane * m_groups;
	wxButton 	 * m_basic_edit_btn;
	wxButton	 * m_basic_cancel_btn;
	Application  * m_owner;

	void OnSelectionChanged(wxCommandEvent &);
	void OnEditButtonClicked(wxCommandEvent &);
	void OnCancelButtonClicked(wxCommandEvent &);
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
	wxChoice * class_choice;
	wxChoice * subject_choice;
	wxChoice * occurence_choice;

	wxButton * m_edit_btn;
	wxButton * m_cancel_btn;

	Application * m_owner;
	void OnClassSelectionChanged(wxCommandEvent &);
	void OnSubjectSelectionChanged(wxCommandEvent &);
	void OnEditButtonClicked(wxCommandEvent &);
	void OnCancelButtonClicked(wxCommandEvent &);
};

class ListPlanningTimesPane : public wxScrolledWindow {
 public:
	ListPlanningTimesPane(Application * owner, wxWindow * parent, wxPoint pos);
	~ListPlanningTimesPane();

 private:
	wxChoice 		* m_teacher_choice;
	wxChoice 		* m_occurence_choice;
	wxCheckBox 		* m_needs_room_text;
	ScoreGridPane 	* m_periods;
	ScoreGridPane 	* m_rooms;
	wxChoice 		* m_room_choice;
	wxChoice 		* m_period_choice;
	wxButton 		* m_edit_btn;
	wxButton 		* m_cancel_btn;
	Application 	* m_owner;

	void OnTeacherSelectionChanged(wxCommandEvent &);
	void OnOccurenceSelectionChanged(wxCommandEvent &);
	void OnEditButtonClicked(wxCommandEvent &);
	void OnCancelButtonClicked(wxCommandEvent &);
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
	wxStaticText * m_class_text;
	wxStaticText * m_teacher_text;
	wxStaticText * m_subject_text;
	wxStaticText * m_room_text;
	wxStaticText * m_day_text;
	wxStaticText * m_period_text;

	void OnRedrawGridRequest(wxCommandEvent &);
	void OnGridSelection(wxGridEvent & evt);
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
	void OpenHelpWindow();
	void CloseHelpWindow();
	void ChildNotify(int messageCode);
	void OnConfigUpdate();
	void LoadConfig();
	void UpdateFonts();
	bool SaveDatabase();
	void NotifyNewUnsavedData();

	const Language * m_lang;
	int m_font_sz;

	ManualWindow *	m_window_manual = nullptr;
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


/* This is not a complete list of widgets in the program.
 * It is, however, the list of ones that need an explicit
 * id.
 */
enum MinervaWidgetId {
	ID_SAVE = wxID_HIGHEST+1,
	ID_CLOSE,
	ID_UNDO,
	ID_REDO,
	ID_HELP,
	ID_SCHOOL_DATA,
	ID_OPEN_SCHOOL_MANUAL,

	ID_VIEW_ROOMS,
	ID_ADD_ROOM,
	ID_CHECK_ALL_ROOMS,
	ID_OPEN_ROOMS_MANUAL,

	ID_VIEW_SUBJECTS,
	ID_VIEW_SUBJECT_GROUPS,
	ID_ADD_SUBJECT,
	ID_ADD_SUBJECT_GROUP,
	ID_CHECK_ALL_SUBJECTS,
	ID_OPEN_SUBJECTS_MANUAL,

	ID_VIEW_TEACHERS,
	ID_VIEW_TEACHER_GROUPS,
	ID_ADD_TEACHER,
	ID_ADD_TEACHER_GROUP,
	ID_CHECK_ALL_TEACHERS,
	ID_OPEN_TEACHERS_MANUAL,

	ID_VIEW_CLASSES,
	ID_VIEW_CLASS_GROUPS,
	ID_ADD_CLASS,
	ID_ADD_CLASS_GROUP,
	ID_CHECK_ALL_CLASSES,
	ID_OPEN_CLASSES_MANUAL,

	ID_VIEW_LECTURES,
	ID_VIEW_PLANNING_TIMES,
	ID_CHECK_ALL_EVENTS,
	ID_OPEN_EVENTS_MANUAL,

	ID_VIEW_TIMETABLE,
	ID_EXPORT_TIMETABLE,
	ID_GENERATE_TIMETABLE,
	ID_CREATE_TIMETABLE,
	ID_OPEN_TIMETABLE_MANUAL
};

#endif /* GUI_H */
