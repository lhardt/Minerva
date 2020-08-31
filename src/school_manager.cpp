#include "school_manager.hpp"

#include <wx/string.h>

#include "gui.hpp"
extern "C" {
	#include "util.h"
	#include "loader.h"
};


Action::Action(Application * owner) : m_owner(owner){

}

Action::~Action(){

}

void Action::Do() {
	printf("Unoverriden do\n");
}
void Action::Undo() {

}

wxString Action::Describe(){
	return wxT("Action");
}


SchoolNameUpdateAction::SchoolNameUpdateAction(Application * owner, char * name) : Action(owner), m_name(name) {

}

SchoolNameUpdateAction::~SchoolNameUpdateAction(){
	free(m_name);
}

void SchoolNameUpdateAction::Do(){
	printf("Updating name from %s to %s\n", m_owner->m_school->name, m_name);
	update_school_name(stdout, m_owner->m_database, m_owner->m_school->id, m_name);
	char * temp = m_owner->m_school->name;
	m_owner->m_school->name = m_name;
	m_name = temp;
}

void SchoolNameUpdateAction::Undo(){
	printf("Undoing name update from %s to %s\n", m_owner->m_school->name, m_name);
	update_school_name(stdout, m_owner->m_database, m_owner->m_school->id, m_name);
	char * temp = m_owner->m_school->name;
	m_owner->m_school->name = m_name;
	m_name = temp;
}

wxString SchoolNameUpdateAction::Describe(){
	return wxT("SchoolNameUpdateAction");
}

void ActionManager::Do(Action* act) {
	act->Do();
	m_undo_list.push_back(act);
	if(m_redo_list.size() > 0){
		for(Action * redo_act : m_redo_list){
			delete redo_act;
		}
	}
	m_redo_list.clear();
}

void ActionManager::Undo() {

}
