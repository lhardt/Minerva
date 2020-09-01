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

class Action {
public:
	Action(Application * owner);
	~Action();
	virtual void Do();
	virtual void Undo();
	virtual wxString Describe();
	Application * m_owner;
};

class SchoolNameUpdateAction : public Action {
public:
	SchoolNameUpdateAction(Application * m_owner, char * name);
	~SchoolNameUpdateAction();
	void Do();
	void Undo();
	wxString Describe();
	char * m_name;
};

class SchoolPeriodsUpdateAction : public Action {
public:
	SchoolPeriodsUpdateAction(Application * m_owner, int * values);
	~SchoolPeriodsUpdateAction();
	void Do();
	void Undo();
	wxString Describe();
	int * m_values;
};

class DayNamesUpdateAction : public Action {
public:
	DayNamesUpdateAction(Application * m_owner, char ** names);
	~DayNamesUpdateAction();
	void Do();
	void Undo();
	wxString Describe();
	char ** m_names;
};

class DailyPeriodNamesUpdateAction : public Action {
public:
	DailyPeriodNamesUpdateAction(Application * m_owner, char ** names);
	~DailyPeriodNamesUpdateAction();
	void Do();
	void Undo();
	wxString Describe();
	char ** m_names;
};

class PeriodNameUpdateAction : public Action {
public:
	PeriodNameUpdateAction(Application * m_owner, char ** names);
	~PeriodNameUpdateAction();
	void Do();
	void Undo();
	wxString Describe();
	char ** m_names;
};

class ActionManager{
public:
	Application * m_owner;
	void Do(Action* act);
	void Undo();
	void Redo();

	std::vector<Action*> m_undo_list;
	std::vector<Action*> m_redo_list;
};

#endif /* SCHOOL_MANAGER_HPP */
