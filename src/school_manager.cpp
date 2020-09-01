#include "school_manager.hpp"

#include <wx/string.h>

#include "gui.hpp"
extern "C" {
	#include "assert.h"
	#include "util.h"
	#include "loader.h"
	#include "preprocess.h"
};

/*********************************************************/
/*                         Action                        */
/*********************************************************/

Action::Action(Application * owner) : m_owner(owner){

}

Action::~Action(){

}

bool Action::Do() {
	printf("Unoverriden do\n");
	return true;
}
bool Action::Undo() {
	printf("Unoverriden undo\n");
	return true;
}

wxString Action::Describe(){
	return wxT("Action");
}

/*********************************************************/
/*                 SchoolNameUpdateAction                */
/*********************************************************/

SchoolNameUpdateAction::SchoolNameUpdateAction(Application * owner, char * name) : Action(owner), m_name(name) {

}

SchoolNameUpdateAction::~SchoolNameUpdateAction(){
	free(m_name);
}

bool SchoolNameUpdateAction::Do(){
	printf("Updating name from %s to %s\n", m_owner->m_school->name, m_name);
	if(update_school_name(stdout, m_owner->m_database, m_owner->m_school->id, m_name)){
		char * temp = m_owner->m_school->name;
		m_owner->m_school->name = m_name;
		m_name = temp;
		return true;
	}
	return false;
}

bool SchoolNameUpdateAction::Undo(){
	printf("Undoing name update from %s to %s\n", m_owner->m_school->name, m_name);
	if(update_school_name(stdout, m_owner->m_database, m_owner->m_school->id, m_name)){
		char * temp = m_owner->m_school->name;
		m_owner->m_school->name = m_name;
		m_name = temp;
		return true;
	}
	return false;
}

wxString SchoolNameUpdateAction::Describe(){
	return wxT("SchoolNameUpdateAction");
}

/*********************************************************/
/*               SchoolPeriodsUpdateAction               */
/*********************************************************/

SchoolPeriodsUpdateAction::SchoolPeriodsUpdateAction(Application * owner, int * values) : Action(owner), m_values(values) {

}

SchoolPeriodsUpdateAction::~SchoolPeriodsUpdateAction(){
	free(m_values);
}

bool SchoolPeriodsUpdateAction::Do(){
	if(update_school_period_scores(stdout, m_owner->m_database, m_owner->m_school->n_periods, m_owner->m_school->period_ids, m_values)){
		int * temp = m_owner->m_school->periods;
		m_owner->m_school->periods = m_values;
		m_values = temp;
		return true;
	}
	return false;
}

bool SchoolPeriodsUpdateAction::Undo(){
	printf("Undoing School Periods Update\n");
	if(update_school_period_scores(stdout, m_owner->m_database, m_owner->m_school->n_periods, m_owner->m_school->period_ids, m_values)){
		int * temp = m_owner->m_school->periods;
		m_owner->m_school->periods = m_values;
		m_values = temp;
		return true;
	}
	return false;
}

wxString SchoolPeriodsUpdateAction::Describe(){
	return wxT("SchoolPeriodsUpdateAction");
}

/*********************************************************/
/*              DailyPeriodNamesUpdateAction             */
/*********************************************************/

DailyPeriodNamesUpdateAction::DailyPeriodNamesUpdateAction(Application * owner, char ** names) : Action(owner), m_names(names){

}

DailyPeriodNamesUpdateAction::~DailyPeriodNamesUpdateAction() {
	for(int i = 0; i < m_owner->m_school->n_periods_per_day; ++i){
		free(m_names[i]);
	}
	free(m_names);
}

bool DailyPeriodNamesUpdateAction::Do(){
	if(update_daily_period_names(stdout, m_owner->m_database, m_owner->m_school->n_periods_per_day, m_owner->m_school->daily_period_ids, m_names)){
		char ** temp = m_owner->m_school->daily_period_names;
		m_owner->m_school->daily_period_names = m_names;
		m_names = temp;
		return false;
	}
	return false;
}

bool DailyPeriodNamesUpdateAction::Undo(){
	if(update_daily_period_names(stdout, m_owner->m_database, m_owner->m_school->n_periods_per_day, m_owner->m_school->daily_period_ids, m_names)){
		char ** temp = m_owner->m_school->daily_period_names;
		m_owner->m_school->daily_period_names = m_names;
		m_names = temp;
		return true;
	}
	return false;
}

wxString DailyPeriodNamesUpdateAction::Describe() {
	return wxT("DailyPeriodNamesUpdateAction");
}

/*********************************************************/
/*                  DayNamesUpdateAction                 */
/*********************************************************/

DayNamesUpdateAction::DayNamesUpdateAction(Application * owner, char ** names) : Action(owner), m_names(names){

}

DayNamesUpdateAction::~DayNamesUpdateAction() {
	for(int i = 0; i < m_owner->m_school->n_days; ++i){
		free(m_names[i]);
	}
	free(m_names);
}

bool DayNamesUpdateAction::Do(){
	if(update_day_names(stdout, m_owner->m_database, m_owner->m_school->n_days, m_owner->m_school->day_ids, m_names)){
		char ** temp = m_owner->m_school->day_names;
		m_owner->m_school->day_names = m_names;
		m_names = temp;
		return true;
	}
	return false;
}

bool DayNamesUpdateAction::Undo(){
	if(update_day_names(stdout, m_owner->m_database, m_owner->m_school->n_days, m_owner->m_school->day_ids, m_names)){
		char ** temp = m_owner->m_school->day_names;
		m_owner->m_school->day_names = m_names;
		m_names = temp;
		return true;
	}
	return false;
}

wxString DayNamesUpdateAction::Describe() {
	return wxT("DayNamesUpdateAction");
}

/*********************************************************/
/*                 PeriodNameUpdateAction                */
/*********************************************************/

PeriodNameUpdateAction::PeriodNameUpdateAction(Application * owner, char ** names) : Action(owner), m_names(names){

}

PeriodNameUpdateAction::~PeriodNameUpdateAction() {
	for(int i = 0; i < m_owner->m_school->n_periods; ++i){
		free(m_names[i]);
	}
	free(m_names);
}

bool PeriodNameUpdateAction::Do(){
	if(update_period_names(stdout, m_owner->m_database, m_owner->m_school->n_periods, m_owner->m_school->period_ids, m_names)){
		char ** temp = m_owner->m_school->period_names;
		m_owner->m_school->period_names = m_names;
		m_names = temp;
		return true;
	}
	return false;
}

bool PeriodNameUpdateAction::Undo(){
	if(update_period_names(stdout, m_owner->m_database, m_owner->m_school->n_periods, m_owner->m_school->period_ids, m_names)){
		char ** temp = m_owner->m_school->period_names;
		m_owner->m_school->period_names = m_names;
		m_names = temp;
		return true;
	}
	return false;
}

wxString PeriodNameUpdateAction::Describe(){
	return wxT("PeriodNameUpdateAction");
}

/*********************************************************/
/*               RoomBasicDataUpdateAction               */
/*********************************************************/
RoomBasicDataUpdateAction::RoomBasicDataUpdateAction(Application * owner, Room room, int room_id) : Action(owner), m_room(room), m_room_id(room_id) {

}
RoomBasicDataUpdateAction::~RoomBasicDataUpdateAction(){
	free(m_room.name);
	free(m_room.short_name);
}
bool RoomBasicDataUpdateAction::Do(){
	if(update_room_basic_data(stdout, m_owner->m_database, m_room_id, &m_room, m_owner->m_school)){
		Room * to_update = find_room_by_id(m_owner->m_school, m_room_id);
		LMH_ASSERT(to_update != NULL);
		Room tmp = (Room) {
			.name = to_update->name,
			.short_name = to_update->short_name,
			.size = to_update->size,
			.active = to_update->active
		};
		to_update->name = m_room.name;
		to_update->short_name = m_room.short_name;
		to_update->active = m_room.active;
		to_update->size = m_room.size;

		m_room.name = tmp.name;
		m_room.active = tmp.active;
		m_room.short_name = tmp.short_name;
		m_room.size = tmp.size;
	}
	return true;
}
bool RoomBasicDataUpdateAction::Undo(){
	if(update_room_basic_data(stdout, m_owner->m_database, m_room_id, &m_room, m_owner->m_school)){
		Room * to_update = find_room_by_id(m_owner->m_school, m_room_id);
		LMH_ASSERT(to_update != NULL);
		Room tmp = (Room) {
			.name = to_update->name,
			.short_name = to_update->short_name,
			.size = to_update->size,
			.active = to_update->active
		};
		to_update->name = m_room.name;
		to_update->short_name = m_room.short_name;
		to_update->active = m_room.active;
		to_update->size = m_room.size;

		m_room.name = tmp.name;
		m_room.active = tmp.active;
		m_room.short_name = tmp.short_name;
		m_room.size = tmp.size;
	}
	return false;
}
wxString RoomBasicDataUpdateAction::Describe(){
	return wxT("RoomBasicDataUpdateAction");
}

/*********************************************************/
/*             RoomAvailabilityUpdateAction              */
/*********************************************************/
RoomAvailabilityUpdateAction::RoomAvailabilityUpdateAction(Application * owner, int * period_scores, int room_id) : Action(owner), m_period_scores(period_scores), m_room_id(room_id) {

}
RoomAvailabilityUpdateAction::~RoomAvailabilityUpdateAction() {
	free(m_period_scores);
}
bool RoomAvailabilityUpdateAction::Do(){
	if(update_room_availability(stdout, m_owner->m_database, m_room_id, m_period_scores, m_owner->m_school)){
		Room * to_update = find_room_by_id(m_owner->m_school, m_room_id);
		LMH_ASSERT(to_update != NULL);
		int * temp = to_update->availability;
		to_update->availability = m_period_scores;
		m_period_scores = temp;
		return true;
	}
	return false;
}
bool RoomAvailabilityUpdateAction::Undo(){
	if(update_room_availability(stdout, m_owner->m_database, m_room_id, m_period_scores, m_owner->m_school)){
		Room * to_update = find_room_by_id(m_owner->m_school, m_room_id);
		LMH_ASSERT(to_update != NULL);
		int * temp = to_update->availability;
		to_update->availability = m_period_scores;
		m_period_scores = temp;
		return true;
	}
	return false;
}
wxString RoomAvailabilityUpdateAction::Describe(){
	return wxT("RoomAvailabilityUpdateAction");
}

/*********************************************************/
/*                    RoomInsertAction                   */
/*********************************************************/
RoomInsertAction::RoomInsertAction(Application * owner, Room room)  : Action(owner), m_room(room) {
	m_state = state_UNDONE;
}
RoomInsertAction::~RoomInsertAction(){
	/* If the state is done, then the data is in elsewhere. */
	if(m_state == state_UNDONE){
		free(m_room.name);
		free(m_room.short_name);
		free(m_room.availability);
	}
}
bool RoomInsertAction::Do(){
	m_state = state_DONE;
	int id = insert_room(stdout, m_owner->m_database, &m_room, m_owner->m_school);
	if(id != -1){
		m_room.id = id;
		school_room_add(m_owner->m_school, &m_room);
		return true;
	}
	return true;
}
bool RoomInsertAction::Undo(){
	m_state = state_UNDONE;
	if(remove_room(stdout, m_owner->m_database, m_room.id)){
		m_room.id = 0;
		school_room_remove(m_owner->m_school, get_room_index_by_id(m_owner->m_school, m_room.id));
		return true;
	}
	return false;
}
wxString RoomInsertAction::Describe(){
	return wxT("RoomInsertAction");
}


/*********************************************************/
/*                     ActionManager                     */
/*********************************************************/

bool ActionManager::Do(Action* act) {
	if(act->Do()){
		m_undo_list.push_back(act);
		if(m_redo_list.size() > 0){
			for(Action * redo_act : m_redo_list){
				delete redo_act;
			}
		}
		m_redo_list.clear();
		return true;
	}
	return false;
}

bool ActionManager::Undo() {
	if(!m_undo_list.empty()){
		Action * last_action = m_undo_list.back();
		if(last_action->Undo()){
			m_redo_list.push_back(last_action);
			m_undo_list.pop_back();
			return true;
		}
	}
	return false;
}
bool ActionManager::Redo() {
	if(!m_redo_list.empty()){
		Action * last_action = m_redo_list.back();
		if(last_action->Do()){
			m_undo_list.push_back(last_action);
			m_redo_list.pop_back();
			return true;
		}
	}
	return false;
}
