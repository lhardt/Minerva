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

#include "types.h"

class SchoolManager;

class Action {
public:
	virtual ~Action();
	virtual void Do() = 0;
	virtual void Undo() = 0;
	virtual wxString Describe() {
		return wxT("Action");
	}

	SchoolManager * m_manager;
};

class SchoolChangeAction : public Action {
public:
	~SchoolChangeAction();
	SchoolChangeAction(SchoolManager * owner, School * new_school);
	void Do();
	void Undo();
	wxString Describe();
	School * m_school;
};

class SchoolManager{
public:
	School * m_school;
	void ChangeSchool(School * new_school);
	void DoAndSave(Action * act);
	void RedoAndSave();
private:
	std::vector<Action*> undo_list;
	std::vector<Action*> redo_list;
	friend class Action;
};

#endif /* SCHOOL_MANAGER_HPP */
