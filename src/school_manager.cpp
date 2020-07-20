#include "school_manager.hpp"

#include <wx/string.h>
#include "util.h"

Action::~Action(){

}

SchoolChangeAction::SchoolChangeAction(SchoolManager * manager, School * new_school) :  m_school(new_school){
	m_manager = manager;
}

void SchoolChangeAction::Do(){
	/* Shallow. Preserves all memory and swaps. */
	School tmp;
	tmp = *(m_manager->m_school);
	*(m_manager->m_school) = *m_school;
	*m_school = tmp;
}

void SchoolChangeAction::Undo(){
	/* Shallow. Preserves all memory and swaps. */
	School tmp;
	tmp = *(m_manager->m_school);
	*(m_manager->m_school) = *m_school;
	*m_school = tmp;
}

wxString SchoolChangeAction::Describe(){
	return wxT("SchoolChangeAction");
}

SchoolChangeAction::~SchoolChangeAction(){
	/* TODO: should free one of them? */
}

void SchoolManager::ChangeSchool(School * school){
	SchoolChangeAction *act = new SchoolChangeAction(this, m_school);
	DoAndSave(act);
}

void SchoolManager::DoAndSave(Action * act){
	act->Do();
	undo_list.push_back(act);

	for(auto& action_ptr : redo_list){
		delete action_ptr;
	}
	redo_list.clear();
}

void SchoolManager::RedoAndSave(){
	if(!undo_list.empty()){
		Action * to_redo = undo_list.back();
		to_redo->Do();
		redo_list.push_back(to_redo);
		undo_list.pop_back();
	}
}
