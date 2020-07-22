/*
 * Project Minerva.
 *   school_manager.hpp - UNDO/REDO manager for the School structure..
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

class DataManager;

class Action {
public:
	virtual ~Action();
	virtual void Do() = 0;
	virtual void Undo() = 0;
	virtual wxString Describe() {
		return wxT("Action");
	}

	DataManager * m_manager;
};

class SchoolChangeAction : public Action {
public:
	~SchoolChangeAction();
	SchoolChangeAction(DataManager * owner, School * new_school);
	void Do();
	void Undo();
	wxString Describe();
	School * m_school;
};

class AddClassAction : public Action {
public:
	~AddClassAction();
	AddClassAction(DataManager * owner, Class * _class);

	void Do();
	void Undo();

	Class * m_class;
};

class DataManager{
public:
	School * m_school;
	sqlite3 * m_database;

	void DoAndSave(Action * act);
	void RedoAndSave();
	/* Actions simplified */
	void ChangeSchool(School * new_school);
	void AddClass(Class & new_class);

private:
	std::vector<Action*> undo_list;
	std::vector<Action*> redo_list;
	friend class Action;
};

#endif /* SCHOOL_MANAGER_HPP */
