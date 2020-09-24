/*
 * Project Minerva.
 *   school_manager.hpp - UNDO/REDO manager for the School structure.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 */
#ifndef SCHOOL_MANAGER_HPP
#define SCHOOL_MANAGER_HPP

#include <vector> // TODO: isn't Deque better?
#include <wx/string.h>
#include <sqlite3.h>

#include "types.h"

class ActionManager;
class Application;

enum ActionState {
	state_UNMADE = 0,
	state_DONE,
	state_UNDONE
};

class Action {
public:
	Action(Application * owner);
	~Action();
	virtual bool Do();
	virtual bool Undo();
	virtual wxString Describe();
	Application * m_owner;
};

class SchoolNameUpdateAction : public Action {
public:
	SchoolNameUpdateAction(Application * owner, char * name);
	~SchoolNameUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
	char * m_name;
};

class SchoolPeriodsUpdateAction : public Action {
public:
	SchoolPeriodsUpdateAction(Application * owner, int * values);
	~SchoolPeriodsUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
	int * m_values;
};

class DayNamesUpdateAction : public Action {
public:
	DayNamesUpdateAction(Application * owner, char ** names);
	~DayNamesUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
	char ** m_names;
};

class DailyPeriodNamesUpdateAction : public Action {
public:
	DailyPeriodNamesUpdateAction(Application * owner, char ** names);
	~DailyPeriodNamesUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
	char ** m_names;
};

class PeriodNameUpdateAction : public Action {
public:
	PeriodNameUpdateAction(Application * owner, char ** names);
	~PeriodNameUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
	char ** m_names;
};

class RoomBasicDataUpdateAction : public Action {
public:
	RoomBasicDataUpdateAction(Application * owner, Room room, int room_id);
	~RoomBasicDataUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
	Room m_room;
	int m_room_id;
};

class RoomAvailabilityUpdateAction : public Action {
public:
	RoomAvailabilityUpdateAction(Application * owner, int * period_scores, int room_id);
	~RoomAvailabilityUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
	int * m_period_scores;
	int m_room_id;
};

class RoomInsertAction : public Action {
public:
	RoomInsertAction(Application * owner, Room room);
	~RoomInsertAction();
	bool Do();
	bool Undo();
	wxString Describe();
	Room 		m_room;
	ActionState m_state;
	int 		m_insert_id;
};

class RoomDeleteAction : public Action {
public:
	RoomDeleteAction(Application * owner, int i_room);
	~RoomDeleteAction();
	bool Do();
	bool Undo();
	wxString Describe();
	Room  m_room;
	int   m_i_room;
	int * m_teaches_scores;
	int * m_class_scores;
	int * m_teacher_planning_scores;
	int * m_teacher_lecture_scores;
	int * m_meeting_scores;
	bool * m_meetings;
	ActionState m_state;
};

class SubjectInsertAction : public Action {
public:
	SubjectInsertAction(Application * owner, Subject s);
	~SubjectInsertAction();
	bool Do();
	bool Undo();
	wxString Describe();
	Subject m_subject;
	ActionState m_state;
};

class SubjectDeleteAction : public Action{
public:
	SubjectDeleteAction(Application * owner, int id);
	~SubjectDeleteAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	Subject m_subject;
	Assignment * m_assignments;
	Teaches * m_teaches;
	Meeting * m_meetings;
	ActionState m_state;
};

class SubjectBasicDataUpdateAction : public Action {
public:
	SubjectBasicDataUpdateAction(Application * owner, Subject data);
	~SubjectBasicDataUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();

	Subject m_subject;
	ActionState m_state;
};

class SubjectGroupInsertAction : public Action {
public:
	SubjectGroupInsertAction(Application * owner, char * name, int * members);
	SubjectGroupInsertAction(Application * owner, char * name);
	~SubjectGroupInsertAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	char * m_name;
	int * m_members;
	int m_id;
	ActionState m_state;
};

class SubjectGroupDeleteAction : public Action{
public:
	SubjectGroupDeleteAction(Application * owner, int i_group);
	~SubjectGroupDeleteAction();
	bool Do();
	bool Undo();
	wxString Describe();

private:
	int    m_id;
	char * m_name;
	int  * m_in_group;
	int  * m_class_max_per_day;
	ActionState m_state;
};

class SubjectGroupNameUpdateAction : public Action {
public:
	SubjectGroupNameUpdateAction(Application * owner, int i_sgr, char * new_name);
	~SubjectGroupNameUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int		m_id;
	char * 	m_name;
};

class SubjectGroupMembersUpdateAction : public Action {
public:
	SubjectGroupMembersUpdateAction(Application * owner);
	~SubjectGroupMembersUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
};

class ActionManager{
public:
	Application * m_owner;
	bool Do(Action* act);
	bool Undo();
	bool Redo();

	std::vector<Action*> m_undo_list;
	std::vector<Action*> m_redo_list;
};

#endif /* SCHOOL_MANAGER_HPP */
