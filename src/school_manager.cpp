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
	printf("Unoverriden undo\n");
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
	if(update_school_name(stdout, m_owner->m_database, m_owner->m_school->id, m_name)){
		char * temp = m_owner->m_school->name;
		m_owner->m_school->name = m_name;
		m_name = temp;
	}
}

void SchoolNameUpdateAction::Undo(){
	printf("Undoing name update from %s to %s\n", m_owner->m_school->name, m_name);
	if(update_school_name(stdout, m_owner->m_database, m_owner->m_school->id, m_name)){
		char * temp = m_owner->m_school->name;
		m_owner->m_school->name = m_name;
		m_name = temp;
	}
}

wxString SchoolNameUpdateAction::Describe(){
	return wxT("SchoolNameUpdateAction");
}

SchoolPeriodsUpdateAction::SchoolPeriodsUpdateAction(Application * owner, int * values) : Action(owner), m_values(values) {

}

SchoolPeriodsUpdateAction::~SchoolPeriodsUpdateAction(){
	free(m_values);
}

void SchoolPeriodsUpdateAction::Do(){
	if(update_school_period_scores(stdout, m_owner->m_database, m_owner->m_school->n_periods, m_owner->m_school->period_ids, m_values)){
		int * temp = m_owner->m_school->periods;
		m_owner->m_school->periods = m_values;
		m_values = temp;
	} else {
		printf("Couldn't do it.\n");
	}
}

void SchoolPeriodsUpdateAction::Undo(){
	printf("Undoing School Periods Update\n");
	if(update_school_period_scores(stdout, m_owner->m_database, m_owner->m_school->n_periods, m_owner->m_school->period_ids, m_values)){
		int * temp = m_owner->m_school->periods;
		m_owner->m_school->periods = m_values;
		m_values = temp;
	} else {
		printf("Couldn't do it.\n");

	}
}

wxString SchoolPeriodsUpdateAction::Describe(){
	return wxT("SchoolPeriodsUpdateAction");
}

DailyPeriodNamesUpdateAction::DailyPeriodNamesUpdateAction(Application * owner, char ** names) : Action(owner), m_names(names){

}

DailyPeriodNamesUpdateAction::~DailyPeriodNamesUpdateAction() {
	for(int i = 0; i < m_owner->m_school->n_periods_per_day; ++i){
		free(m_names[i]);
	}
	free(m_names);
}

void DailyPeriodNamesUpdateAction::Do(){
	if(update_daily_period_names(stdout, m_owner->m_database, m_owner->m_school->n_periods_per_day, m_owner->m_school->daily_period_ids, m_names)){
		char ** temp = m_owner->m_school->daily_period_names;
		m_owner->m_school->daily_period_names = m_names;
		m_names = temp;
	}
}

void DailyPeriodNamesUpdateAction::Undo(){
	if(update_daily_period_names(stdout, m_owner->m_database, m_owner->m_school->n_periods_per_day, m_owner->m_school->daily_period_ids, m_names)){
		char ** temp = m_owner->m_school->daily_period_names;
		m_owner->m_school->daily_period_names = m_names;
		m_names = temp;
	}
}

wxString DailyPeriodNamesUpdateAction::Describe() {
	return wxT("DailyPeriodNamesUpdateAction");
}

DayNamesUpdateAction::DayNamesUpdateAction(Application * owner, char ** names) : Action(owner), m_names(names){

}

DayNamesUpdateAction::~DayNamesUpdateAction() {
	for(int i = 0; i < m_owner->m_school->n_days; ++i){
		free(m_names[i]);
	}
	free(m_names);
}

void DayNamesUpdateAction::Do(){
	if(update_day_names(stdout, m_owner->m_database, m_owner->m_school->n_days, m_owner->m_school->day_ids, m_names)){
		char ** temp = m_owner->m_school->day_names;
		m_owner->m_school->day_names = m_names;
		m_names = temp;
	}
}

void DayNamesUpdateAction::Undo(){
	if(update_day_names(stdout, m_owner->m_database, m_owner->m_school->n_days, m_owner->m_school->day_ids, m_names)){
		char ** temp = m_owner->m_school->day_names;
		m_owner->m_school->day_names = m_names;
		m_names = temp;
	}
}

wxString DayNamesUpdateAction::Describe() {
	return wxT("DayNamesUpdateAction");
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
	if(!m_undo_list.empty()){
		Action * last_action = m_undo_list.back();
		last_action->Undo();
		m_redo_list.push_back(last_action);
		m_undo_list.pop_back();
	}
}
void ActionManager::Redo() {
	if(!m_redo_list.empty()){
		Action * last_action = m_redo_list.back();
		last_action->Do();
		m_undo_list.push_back(last_action);
		m_redo_list.pop_back();
	}
}
