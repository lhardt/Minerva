#include "school_manager.hpp"

#include <wx/string.h>

#include "gui.hpp"
extern "C" {
	#include "assert.h"
	#include "util.h"
	#include "loader.h"
};

/*********************************************************/
/*                         Action                        */
/*********************************************************/

Action::Action(Application * owner) : m_owner(owner){
	LMH_ASSERT(m_owner != NULL && m_owner->m_school != NULL);
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
		return true;
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
		return true;
	}
	return false;
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
		return true;
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
	m_insert_id = -1;
}
RoomInsertAction::~RoomInsertAction(){
	/* If the state is done, then the data is in elsewhere. */
	if(m_state == state_UNDONE){
		free_room(&m_room);
	}
}
bool RoomInsertAction::Do(){
	LMH_ASSERT(m_state == state_UNDONE);
	m_state = state_DONE;
	m_insert_id = insert_room(stdout, m_owner->m_database, &m_room, m_owner->m_school, m_insert_id);
	if(m_insert_id != -1){
		m_room.id = m_insert_id;
		school_room_add(m_owner->m_school, &m_room);
		return true;
	}
	return false;
}
bool RoomInsertAction::Undo(){
	LMH_ASSERT(m_state == state_DONE);
	m_state = state_UNDONE;
	if(remove_room(stdout, m_owner->m_database, m_room.id)){
		school_room_remove(m_owner->m_school, get_room_index_by_id(m_owner->m_school, m_room.id), false);
		m_room.id = 0;
		return true;
	}
	return false;
}
wxString RoomInsertAction::Describe(){
	return wxT("RoomInsertAction");
}

/*********************************************************/
/*                    RoomDeleteAction                   */
/*********************************************************/

RoomDeleteAction::RoomDeleteAction(Application * owner, int i_room)  : Action(owner), m_i_room(i_room) {
	m_state = state_UNDONE;
}
RoomDeleteAction::~RoomDeleteAction(){
	/* If the state is done, then the data is in elsewhere. */
	if(m_state == state_DONE){
		free(m_room.name);
		free(m_room.short_name);
		free(m_room.availability);
		free(m_teaches_scores);
		free(m_class_scores);
		free(m_teacher_planning_scores);
		free(m_teacher_lecture_scores);
		free(m_meeting_scores);
		free(m_meetings);
	}
}
bool RoomDeleteAction::Do(){
	int i;
	LMH_ASSERT(m_state == state_UNDONE);
	School * school = m_owner->m_school;
	m_room = school->rooms[m_i_room];
	m_state = state_DONE;
	m_teaches_scores = (int *) calloc(school->n_teaches, sizeof(int));
	m_class_scores = (int *) calloc(school->n_classes, sizeof(int));
	m_teacher_planning_scores = (int *) calloc(school->n_teachers, sizeof(int));
	m_teacher_lecture_scores = (int *) calloc(school->n_teachers, sizeof(int));
	m_meeting_scores = (int *) calloc(school->n_meetings, sizeof(int));
	m_meetings = (bool *) calloc(school->n_meetings, sizeof(bool));

	for(i = 0; i < school->n_teachers; ++i){
		m_teacher_planning_scores[i] = school->teachers[i].planning_room_scores[m_i_room];
		m_teacher_lecture_scores[i] = school->teachers[i].lecture_room_scores[m_i_room];
	}
	for(i = 0; i < school->n_classes; ++i){
		m_class_scores[i] = school->classes[i].room_scores[m_i_room];
	}
	for(i = 0; i < school->n_teaches; ++i){
		m_teaches_scores[i] = school->teaches[i].room_scores[m_i_room];
	}
	for(i = 0; i < school->n_meetings; ++i){
		if(school->meetings[i].room != NULL && school->meetings[i].room->id == m_room.id){
			m_meetings[i] = true;
		}
		if(school->meetings[i].possible_rooms){
			m_meeting_scores[i] = school->meetings[i].possible_rooms[m_i_room];
		} /* No need for else because of calloc. */
	}
	if(remove_room(stdout, m_owner->m_database, m_room.id)){
		school_room_remove(m_owner->m_school, get_room_index_by_id(m_owner->m_school, m_room.id), false);
		return true;
	}
	return false;
}
bool RoomDeleteAction::Undo(){
	int i;
	m_state = state_UNDONE;
	School * school = m_owner->m_school;
	int id = insert_room(stdout, m_owner->m_database, &m_room, m_owner->m_school, m_room.id);
	if(id != -1){
		bool success = true;
		m_room.id = id;
		school_room_add(m_owner->m_school, &m_room);

		LMH_ASSERT(m_owner->m_school->rooms[m_i_room].id == id );
		/* These are surely allocated because of school_room_add. */
		success &= update_room_teacher_score(stdout, m_owner->m_database, id, m_teacher_planning_scores, m_teacher_lecture_scores, school);
		LMH_ASSERT(success);
		for(i = 0; i < school->n_teachers; ++i){
			school->teachers[i].lecture_room_scores[m_i_room] = m_teacher_lecture_scores[i];
			school->teachers[i].planning_room_scores[m_i_room] = m_teacher_planning_scores[i];
		}
		success &= update_room_class_score(stdout, m_owner->m_database, id, m_class_scores, school);
		LMH_ASSERT(success);
		for(i = 0; i < school->n_classes; ++i){
			school->classes[i].room_scores[m_i_room] = m_class_scores[i];
		}
		success &= update_room_teaches_score(stdout, m_owner->m_database, id, m_class_scores, school);
		LMH_ASSERT(success);
		for(i = 0; i < school->n_teaches; ++i){
			school->teaches[i].room_scores[m_i_room] = m_teaches_scores[i];
		}
		success &= update_room_meeting_score(stdout, m_owner->m_database, id, m_meeting_scores, school);
		LMH_ASSERT(success);
		for(i = 0; i < school->n_meetings; ++i){
			school->meetings[i].possible_rooms[m_i_room] = m_meeting_scores[i];
			if(m_meetings[i]){
				school->meetings[i].room = &school->rooms[m_i_room];
			}
		}
		free(m_teaches_scores);
		free(m_class_scores);
		free(m_teacher_planning_scores);
		free(m_teacher_lecture_scores);
		free(m_meeting_scores);
		free(m_meetings);
		return success;
	}
	return false;
}
wxString RoomDeleteAction::Describe(){
	return wxT("RoomDeleteAction");
}

/*********************************************************/
/*                  SubjectInsertAction                  */
/*********************************************************/

SubjectInsertAction::SubjectInsertAction(Application * owner, Subject subject) : Action(owner), m_subject(subject){
	m_state = state_UNDONE;
	m_subject.id = -1;
}
SubjectInsertAction::~SubjectInsertAction(){
	if(m_state == state_UNDONE){
		free(m_subject.name);
		free(m_subject.short_name);
	}
}
bool SubjectInsertAction::Do(){
	insert_subject(stdout, m_owner->m_database, &m_subject, m_owner->m_school, m_subject.id);
	if(m_subject.id != -1){
		school_subject_add(m_owner->m_school, &m_subject);
		return true;
	}
	return false;
}
bool SubjectInsertAction::Undo(){
	bool success = remove_subject(stdout, m_owner->m_database, m_subject.id);
	if(success){
		school_subject_remove(m_owner->m_school, get_subject_index_by_id(m_owner->m_school, m_subject.id), false);
		return true;
	}
	return false;
}
wxString SubjectInsertAction::Describe(){
	return wxT("SubjectInsertAction");
}

/*********************************************************/
/*                  SubjectDeleteAction                  */
/*********************************************************/

SubjectDeleteAction::SubjectDeleteAction(Application * owner, int subj_id) : Action(owner){
	m_state = state_UNMADE;
	m_subject = *(find_subject_by_id(m_owner->m_school,subj_id));
}
SubjectDeleteAction::~SubjectDeleteAction(){
	if(m_state == state_DONE){
		free_subject(&m_subject);
		for(int i = 0; m_assignments[i].m_class != NULL; ++i){
			free_assignment(&m_assignments[i]);
		}
		for(int i = 0; m_teaches[i].teacher != NULL; ++i){
			free_teaches(&m_teaches[i]);
		}
		for(int i = 0; m_meetings[i].type != meet_NULL; ++i){
			free_meeting(&m_meetings[i]);
		}
		free(m_assignments);
		free(m_teaches);
		free(m_meetings);
	}
}
bool SubjectDeleteAction::Do(){
	int n_assignments = 0, n_teaches = 0, n_meetings = 0;
	if(remove_subject(stdout, m_owner->m_database, m_subject.id)){
		m_state = state_DONE;
		School * school = m_owner->m_school;
		/* TODO: is this 'update' needed? Will there be any information that will change? */
		m_subject = *(find_subject_by_id(m_owner->m_school, m_subject.id));
		for(int i = 0; i < school->n_assignments; ++i){
			if(school->assignments[i].subject->id == m_subject.id){
				++n_assignments;
			}
		}
		for(int i = 0; i < school->n_teaches; ++i){
			if(school->teaches[i].subject->id == m_subject.id){
				++n_teaches;
			}
		}
		for(int i = 0; i < school->n_meetings; ++i){
			if(school->meetings[i].subject != NULL && school->meetings[i].subject->id == m_subject.id){
				++n_meetings;
			}
		}

		m_assignments = (Assignment*) calloc(n_assignments + 1, sizeof(Assignment));
		m_teaches = (Teaches*) calloc(n_teaches + 1, sizeof(Teaches));
		m_meetings = (Meeting*) calloc(n_meetings + 1, sizeof(Meeting));

		int i_assignment = 0;
		for(int i = 0; i < school->n_assignments; ++i){
			if(school->assignments[i].subject->id == m_subject.id){
				m_assignments[i_assignment] = school->assignments[i];
				++i_assignment;
			}
		}
		int i_teaches = 0;
		for(int i = 0; i < school->n_teaches; ++i){
			if(school->teaches[i].subject->id == m_subject.id){
				m_teaches[i_teaches] = school->teaches[i];
				++i_teaches;
			}
		}
		int i_meeting = 0;
		for(int i = 0; i < school->n_meetings; ++i){
			if(school->meetings[i].subject != NULL && school->meetings[i].subject->id == m_subject.id){
				m_meetings[i_meeting] = school->meetings[i];
				++i_meeting;
			}
		}
		school_subject_remove(school, get_subject_index_by_id(school,m_subject.id), false);
		return true;
	}
	return false;
}
bool SubjectDeleteAction::Undo(){
	School * school = m_owner->m_school;
	int retval = insert_subject(stdout, m_owner->m_database, &m_subject, school, m_subject.id);
	if(retval != -1){
		m_state = state_UNDONE;
		m_subject.id = retval;
		school_subject_add(school, &m_subject);

		int n_teaches, n_assignments, n_meetings;
		for(n_teaches = 0; m_teaches[n_teaches].teacher != NULL; ++n_teaches){ }
		for(n_assignments = 0; m_assignments[n_assignments].m_class != NULL; ++n_assignments){ }
		for(n_meetings = 0; m_meetings[n_meetings].type != meet_NULL; ++n_meetings){ }

		if(n_teaches > 0){
			for(int i = 0; i < n_teaches; ++i){
				bool success = insert_or_update_teaches(stdout, m_owner->m_database, &m_teaches[i], school);
				LMH_ASSERT(success);
			}
			if(school->teaches == NULL || school->n_teaches == 0){
				school->teaches = (Teaches*) calloc(n_teaches + 1, sizeof(Teaches));
				school->teaches = 0;
			} else {
				school->teaches = (Teaches*) realloc(school->teaches, (school->n_teaches + n_teaches + 1)*sizeof(Teaches));
			}
			for(int i = 0; i < n_teaches; ++i){
				school->teaches[school->n_teaches + i] = m_teaches[i];
			}
			school->n_teaches += n_teaches;
		}
		if(n_assignments > 0){
			for(int i = 0; i < n_assignments; ++i){
				bool success = insert_or_update_assignment(stdout, m_owner->m_database, &m_assignments[i], school);
				LMH_ASSERT(success);
			}
			if(school->assignments == NULL || school->n_assignments == 0){
				school->assignments = (Assignment*) calloc(n_assignments +1, sizeof(Assignment));
			} else {
				school->assignments = (Assignment*) realloc(school->assignments, (school->n_assignments + n_assignments + 1) * sizeof(Assignment));
			}
			for(int i = 0; i < n_assignments; ++i){
				school->assignments[school->n_assignments + i] = m_assignments[i];
			}
			school->n_assignments += n_assignments;
		}
		if(n_meetings > 0){
			bool success = insert_meetings_list(stdout, m_owner->m_database, m_meetings, school);
			LMH_ASSERT(success);
			if(school->meetings == NULL) {
				school->meetings = (Meeting*) calloc(n_meetings + 1, sizeof(Meeting));
				school->n_meetings = 0;
			} else {
				school->meetings = (Meeting*) calloc(school->n_meetings + n_meetings + 1, sizeof(Meeting));
			}
			for(int i = 0; i < n_meetings; ++i){
				school->meetings[school->n_meetings + i] = m_meetings[i];
			}
			school->n_meetings += n_meetings;
		}

		free(m_assignments);
		free(m_teaches);
		free(m_meetings);
		return true;
	}
	return false;
}
wxString SubjectDeleteAction::Describe(){
	return wxT("SubjectDeleteAction");
}

/*********************************************************/
/*                SubjectNameUpdateAction                */
/*********************************************************/

SubjectBasicDataUpdateAction::SubjectBasicDataUpdateAction(Application * owner, Subject subject) :
			Action(owner), m_subject(subject){
	m_state = state_UNDONE;
}
SubjectBasicDataUpdateAction::~SubjectBasicDataUpdateAction(){
	free(m_subject.name);
	free(m_subject.short_name);
}
bool SubjectBasicDataUpdateAction::Do(){
	bool success = update_subject_basic_data(stdout, m_owner->m_database, m_subject.id, &m_subject, m_owner->m_school);
	if(success){
		Subject * to_update = find_subject_by_id(m_owner->m_school, m_subject.id);
		Subject tmp = (Subject){
			.name = to_update->name,
			.short_name = to_update->short_name,
		};
		to_update->name = m_subject.name;
		to_update->short_name = m_subject.short_name;

		m_subject.name = tmp.name;
		m_subject.short_name = tmp.short_name;
		return true;
	}
	return false;
}
bool SubjectBasicDataUpdateAction::Undo(){
	bool success = update_subject_basic_data(stdout, m_owner->m_database, m_subject.id, &m_subject, m_owner->m_school);
	if(success){
		Subject * to_update = find_subject_by_id(m_owner->m_school, m_subject.id);
		Subject tmp = (Subject){
			.name = to_update->name,
			.short_name = to_update->short_name,
		};
		to_update->name = m_subject.name;
		to_update->short_name = m_subject.short_name;

		m_subject.name = tmp.name;
		m_subject.short_name = tmp.short_name;
		return true;
	}
	return false;
}
wxString SubjectBasicDataUpdateAction::Describe(){
	return wxT("SubjectBasicDataUpdateAction");
}

/*********************************************************/
/*               SubjectGroupInsertAction                */
/*********************************************************/

SubjectGroupInsertAction::SubjectGroupInsertAction(Application * owner, char * name, int * members) : Action(owner), m_name(name){
	m_state = state_UNMADE;
	m_members = members;
	m_id = -1;
}
SubjectGroupInsertAction::SubjectGroupInsertAction(Application * owner, char * name) : Action(owner), m_name(name){
	m_state = state_UNMADE;
	m_members = NULL;
	m_id = -1;
}
SubjectGroupInsertAction::~SubjectGroupInsertAction(){
	free(m_members);
}
bool SubjectGroupInsertAction::Do(){
	School * school = m_owner->m_school;
	int newid = insert_subject_group(stdout, m_owner->m_database, m_owner->m_school, m_name, m_id);
	LMH_ASSERT(m_name != NULL);
	if(newid != -1){
		LMH_ASSERT((m_id == -1) != (newid == m_id));
		m_id = newid;
		int sgroup_i = school_subjectgroup_add(m_owner->m_school, m_name, m_id);
		if(m_members != NULL){
			for(int i = 0; i < school->n_subjects; ++i){
				if(m_members[i] > 0){
					int sing_id = insert_subject_in_group(stdout, m_owner->m_database, school->subjects[i].id, m_id);
					LMH_ASSERT(sing_id != -1);
					school->subjects[i].in_groups[sgroup_i] = m_members[i];
				}
				LMH_ASSERT(m_members[i] != -1);
			}
		}
		m_state = state_DONE;
		return true;
	}
	return false;
}
bool SubjectGroupInsertAction::Undo(){
	printf("Before undo, id was %d\n", m_id);
	if(remove_subject_group(stdout, m_owner->m_database, m_id)){
		school_subjectgroup_remove(m_owner->m_school, get_subject_group_index_by_id(m_owner->m_school,m_id), false);
		printf("After undo, id was %d\n", m_id);
		return true;
	}
	return false;
}
wxString SubjectGroupInsertAction::Describe(){
	return wxT("SubjectGroupInsertAction");
}

/*********************************************************/
/*               SubjectGroupDeleteAction                */
/*********************************************************/

SubjectGroupDeleteAction::SubjectGroupDeleteAction(Application * owner, int i_group) : Action(owner){
	School * school = m_owner->m_school;
	LMH_ASSERT(i_group >= 0 && i_group < school->n_subject_groups);
	m_id = school->subject_group_ids[i_group];
	m_name = school->subject_group_names[i_group];
	m_state = state_UNMADE;
}
SubjectGroupDeleteAction::~SubjectGroupDeleteAction(){
	if(m_state != state_UNMADE){
		free(m_in_group);
		free(m_class_max_per_day);
		if(m_state == state_DONE){
			free(m_name);
		}
	}
}
bool SubjectGroupDeleteAction::Do(){
	School * school = m_owner->m_school;
	int i_group = get_subject_group_index_by_id(school,m_id);
	LMH_ASSERT(i_group >= 0 && i_group < school->n_subject_groups);
	m_in_group = (int*) calloc(school->n_subjects + 1, sizeof(int));
	m_in_group[school->n_subjects] = -1;
	m_class_max_per_day =  (int*) calloc(school->n_classes + 1, sizeof(int));
	m_class_max_per_day[school->n_classes] = -1;

	for(int i = 0; i < school->n_subjects; ++i){
		if(school->subjects[i].in_groups != NULL){
			m_in_group[i] = school->subjects[i].in_groups[i_group];
		} else {
			m_in_group[i] = 0;
		}
	}
	for(int i = 0; i < school->n_classes; ++i){
		if(school->classes[i].max_per_day_subject_group){
			m_class_max_per_day[i] = school->classes[i].max_per_day_subject_group[i_group];
		} else {
			m_class_max_per_day[i] = 0;
		}
	}

	if(remove_subject_group(stdout, m_owner->m_database, m_id)){
		school_subjectgroup_remove(m_owner->m_school, i_group, false);
		m_state = state_DONE;
		return true;
	}
	return false;
}
bool SubjectGroupDeleteAction::Undo(){
	School * school = m_owner->m_school;
	m_id = insert_subject_group(stdout, m_owner->m_database, school, m_name, m_id);
	if(m_id > 0){
		int i_group = school_subjectgroup_add(school, m_name,m_id);
		for(int i = 0; i < school->n_subjects; ++i){
			if(m_in_group[i] > 0){
				insert_subject_in_group(stdout, m_owner->m_database, school->subjects[i].id, m_id);
			}
			school->subjects[i].in_groups[i_group] = 1;
		}
		for(int i = 0; i < school->n_classes; ++i){
			if(school->classes[i].max_per_day_subject_group == NULL){
				printf("This should never happen if we done do/undo/redo properly\n");
				school->classes[i].max_per_day_subject_group = (int*) calloc(school->n_subject_groups + 1, sizeof(int));
				school->classes[i].max_per_day_subject_group[school->n_subject_groups] = -1;
			}
			update_class_max_per_day_subjectgroup(stdout, m_owner->m_database, school->classes[i].id, m_id, m_class_max_per_day[i]);
			school->classes[i].max_per_day_subject_group[i_group] = m_class_max_per_day[i];
		}
		m_state = state_UNDONE;
		return true;
	}
	return false;
}
wxString SubjectGroupDeleteAction::Describe(){
	return wxT("SubjectGroupDeleteAction");
}

/*********************************************************/
/*             SubjectGroupNameUpdateAction              */
/*********************************************************/

SubjectGroupNameUpdateAction::SubjectGroupNameUpdateAction(Application * owner, int i_sgr, char * new_name) : Action(owner){
	LMH_ASSERT(owner != NULL && i_sgr < m_owner->m_school->n_subject_groups && i_sgr >= 0 && new_name != NULL);
	m_name = new_name;
	m_id = m_owner->m_school->subject_group_ids[i_sgr];
}
SubjectGroupNameUpdateAction::~SubjectGroupNameUpdateAction(){
	free(m_name);
}
bool SubjectGroupNameUpdateAction::Do(){
	if(update_subject_group_name(stdout, m_owner->m_database, m_id, m_name)){
		School * school = m_owner->m_school;
		int i_sgr = get_subject_group_index_by_id(school, m_id);
		LMH_ASSERT(i_sgr >= 0);

		char * name_temp = school->subject_group_names[i_sgr];
		school->subject_group_names[i_sgr] = m_name;
		m_name = name_temp;

		return true;
	}
	return false;
}
bool SubjectGroupNameUpdateAction::Undo(){
	if(update_subject_group_name(stdout, m_owner->m_database, m_id, m_name)){
		School * school = m_owner->m_school;
		int i_sgr = get_subject_group_index_by_id(school, m_id);
		LMH_ASSERT(i_sgr >= 0);

		char * name_temp = school->subject_group_names[i_sgr];
		school->subject_group_names[i_sgr] = m_name;
		m_name = name_temp;

		return true;
	}
	return false;
}
wxString SubjectGroupNameUpdateAction::Describe(){
	return wxT("SubjectGroupNameUpdateAction");
}

/*********************************************************/
/*            SubjectGroupMembersUpdateAction            */
/*********************************************************/

SubjectGroupMembersUpdateAction::SubjectGroupMembersUpdateAction(Application * owner, int id_sgroup, int * members) : Action(owner){
	LMH_ASSERT(owner != NULL && id_sgroup > 0 && members != NULL);
	m_id = id_sgroup;
	m_members = members;
}
SubjectGroupMembersUpdateAction::~SubjectGroupMembersUpdateAction(){
	free(m_members);
}
bool SubjectGroupMembersUpdateAction::Do(){
	School * school = m_owner->m_school;
	if(update_subject_group_members(stdout, m_owner->m_database, m_id, m_members, school)){
		int i_group = get_subject_group_index_by_id(school, m_id);
		LMH_ASSERT(i_group != -1);
		for(int i = 0; i < school->n_subjects; ++i){
			LMH_ASSERT(school->subjects[i].in_groups != NULL);
			int tmp = school->subjects[i].in_groups[i_group];
			school->subjects[i].in_groups[i_group] = m_members[i];
			m_members[i] = tmp;
		}
	}
	return true;
}
bool SubjectGroupMembersUpdateAction::Undo(){
	return Do();
}
wxString SubjectGroupMembersUpdateAction::Describe(){
	return wxT("SubjectGroupMembersUpdateAction");
}

/*********************************************************/
/*                  TeacherInsertAction                  */
/*********************************************************/
TeacherInsertAction::TeacherInsertAction(Application * owner, Teacher t) : Action(owner){
	m_owner = owner;
	m_teacher = t;
	m_teacher.id = -1;
	m_teaches = NULL;
}
TeacherInsertAction::~TeacherInsertAction(){
	if(m_state == state_UNDONE){
		free_teacher(&m_teacher);
	}
}
bool TeacherInsertAction::Do(){
	// TODO : MAKE WORK WITH SUBORDINATES
	if(m_teaches != NULL){
		for(int i = 0; m_teaches[i].teacher != NULL; ++i){
			m_teacher.teaches[i] = &m_teaches[i];

		}
	}
	int res_id = insert_teacher(stdout, m_owner->m_database, &m_teacher, m_owner->m_school, m_teacher.id);
	if(res_id != -1){
		school_teacher_add(m_owner->m_school, & m_teacher);
		free(m_teaches);
		this->m_teaches = NULL;
		return true;
	}
	return false;
}
bool TeacherInsertAction::Undo(){
	// TODO : MAKE WORK WITH SUBORDINATES
	if(remove_teacher(stdout, m_owner->m_database, m_teacher.id)){

		School * school = m_owner->m_school;
		int i_teacher = get_teacher_index_by_id(m_owner->m_school, m_teacher.id);
		LMH_ASSERT(i_teacher >= 0);

		int teaches_ctr;
		if(school->teachers[i_teacher].teaches != NULL){
			for(teaches_ctr = 0; school->teachers[i_teacher].teaches[teaches_ctr] != NULL; ++teaches_ctr){
				/**/
			}
			m_teaches = (Teaches *) calloc(teaches_ctr+1, sizeof(Teaches));
			for(int i = 0; school->teachers[i_teacher].teaches[i] != NULL; ++i){
				m_teaches[i] = *(school->teachers[i_teacher].teaches[i]);
			}
		}
		school_teacher_remove(school, i_teacher, false);
		return true;
	}
	return false;
}
wxString TeacherInsertAction::Describe(){
	return wxT("TeacherInsertAction");
}


/*********************************************************/
/*                  TeacherDeleteAction                  */
/*********************************************************/
TeacherDeleteAction::TeacherDeleteAction(Application * owner, int id) : Action(owner){
	m_owner = owner;
	m_teacher = * find_teacher_by_id(owner->m_school, id);
	m_state = state_UNDONE;
}

bool TeacherDeleteAction::Do(){
	// TODO : MAKE WORK WITH SUBORDINATES
	School * school = m_owner->m_school;
	if(remove_teacher(stdout, m_owner->m_database, m_teacher.id)){
		int i_teacher = get_teacher_index_by_id(m_owner->m_school, m_teacher.id);
		int teaches_ctr = 0;
		LMH_ASSERT(i_teacher >= 0);

		m_teacher = school->teachers[i_teacher];

		if(m_teacher.teaches != NULL){
			for(teaches_ctr = 0; m_teacher.teaches[teaches_ctr] != NULL; ++teaches_ctr){
				/* Counting teaches_ctr */
			}

			m_teaches = (Teaches *) calloc(teaches_ctr + 1, sizeof(Teaches));
			for(int i = 0; i < teaches_ctr; ++i){
				// It was previously stored in school->teaches and
				// we need to save it from there because there it
				// will be deleted.
				m_teaches[i] = * (m_teacher.teaches[i]);
				m_teaches[i].teacher = &m_teacher;
				m_teacher.teaches[i] = &m_teaches[i];
			}
		} else {
			m_teaches = NULL;
		}
		m_assignment_pref = (int *) calloc(school->n_assignments + 1, sizeof(int));
		for(int i = 0; i < school->n_assignments; ++i){
			m_assignment_pref[i] = school->assignments[i].possible_teachers[i_teacher];
		}

		m_meeting_pref = (int *) calloc(school->n_meetings + 1, sizeof(int));
		m_set_on_meetings = (bool *) calloc(school->n_meetings + 1, sizeof(bool));
		for(int i = 0; i < school->n_meetings; ++i){
			m_meeting_pref[i] = school->meetings[i].possible_teachers[i_teacher];
			m_set_on_meetings[i] = (m_teacher.id == school->meetings[i].teacher->id);
		}

		school_teacher_remove(m_owner->m_school, i_teacher, false);

		for(int i = 0; i < teaches_ctr; ++i){
			m_teacher.teaches[i] = &m_teaches[i];
		}
		return true;
	}
	return false;
}
bool TeacherDeleteAction::Undo(){
	School * school = m_owner->m_school;
	int id = insert_teacher(stdout, m_owner->m_database, &m_teacher, school, m_teacher.id);
	if(id != -1){
		bool success = true;
		int i_teacher = school_teacher_add(school, &m_teacher);

		// TODO!!!

		// don't need to do anything with this one.
		// already inserted by the insert_teacher & school_teacher_add funcs
		free(m_teaches);

		// success &= update_teacher_meeting_score(stdout, m_owner->m_database, id, m_meeting_pref, school);
		// WTF
		// success &= update_teacher_meeting_fixation(stdout, m_owner->m_database, id, m_set_on_meetings, school);
		for(int i = 0; i < school->n_meetings; ++i){
			if(m_set_on_meetings[i]){
				school->meetings[i].teacher = & school->teachers[i_teacher];
			}
			// TODO: Should i copy the meeting->possible_teacher value from assignment->possible_teacher?
		}
		success &= update_teacher_assignment_score(stdout, m_owner->m_database, id, m_assignment_pref, school);
		for(int i = 0; i < school->n_assignments; ++i){
			school->assignments[i].possible_teachers[i_teacher] = m_assignment_pref[i];
		}

		LMH_ASSERT(success);

		free(m_set_on_meetings);
		free(m_assignment_pref);
		free(m_meeting_pref);
		return success;
	}
	return false;
}
TeacherDeleteAction::~TeacherDeleteAction(){
	if(m_state == state_DONE){
		free_teacher(&m_teacher);
	}
}
wxString TeacherDeleteAction::Describe(){
	return wxT("TeacherDeleteAction");
}


/*********************************************************/
/*             TeacherBasicDataUpdateAction              */
/*********************************************************/
TeacherBasicDataUpdateAction::TeacherBasicDataUpdateAction(Application * owner, Teacher t, int id) : Action(owner), m_teacher(t){
	m_teacher.id = id;
	LMH_ASSERT(get_teacher_index_by_id(m_owner->m_school, id) >= 0);
}
TeacherBasicDataUpdateAction::~TeacherBasicDataUpdateAction(){
	free_teacher(&m_teacher);
}
bool TeacherBasicDataUpdateAction::Do(){
	School * school = m_owner->m_school;
	if(update_teacher_basic_data(stdout, m_owner->m_database, &m_teacher, m_owner->m_school)){
		int i_teacher = get_teacher_index_by_id(m_owner->m_school, m_teacher.id);
		LMH_ASSERT(i_teacher >= 0);
		Teacher tmp = (Teacher){
			.name = school->teachers[i_teacher].name,
			.short_name = school->teachers[i_teacher].short_name,
			.max_days = school->teachers[i_teacher].max_days,
			.max_meetings_per_day = school->teachers[i_teacher].max_meetings_per_day,
			.max_meetings_per_class_per_day = school->teachers[i_teacher].max_meetings_per_class_per_day,
			.max_meetings = school->teachers[i_teacher].max_meetings,
			.planning_needs_room = school->teachers[i_teacher].planning_needs_room,
			.num_planning_periods = school->teachers[i_teacher].num_planning_periods,
			.active = school->teachers[i_teacher].active
		};
		school->teachers[i_teacher].name = m_teacher.name;
		school->teachers[i_teacher].short_name = m_teacher.short_name;
		school->teachers[i_teacher].max_days = m_teacher.max_days;
		school->teachers[i_teacher].max_meetings_per_day = m_teacher.max_meetings_per_day;
		school->teachers[i_teacher].max_meetings_per_class_per_day = m_teacher.max_meetings_per_class_per_day;
		school->teachers[i_teacher].max_meetings = m_teacher.max_meetings;
		school->teachers[i_teacher].planning_needs_room = m_teacher.planning_needs_room;
		school->teachers[i_teacher].num_planning_periods = m_teacher.num_planning_periods;
		school->teachers[i_teacher].active = m_teacher.active;

		m_teacher = (Teacher){
			.id = school->teachers[i_teacher].id,
			.name = tmp.name,
			.short_name = tmp.short_name,
			.max_days = tmp.max_days,
			.max_meetings_per_day = tmp.max_meetings_per_day,
			.max_meetings_per_class_per_day = tmp.max_meetings_per_class_per_day,
			.max_meetings = tmp.max_meetings,
			.planning_needs_room = tmp.planning_needs_room,
			.active = tmp.active
		};
		return true;
	}
	return false;
}
bool TeacherBasicDataUpdateAction::Undo(){
	return Do();
}
wxString TeacherBasicDataUpdateAction::Describe(){
	return wxT("TeacherBasicDataUpdateAction");
}

/*********************************************************/
/*              TeacherSubjectsUpdateAction              */
/*********************************************************/

TeacherSubjectsUpdateAction::TeacherSubjectsUpdateAction(Application * owner, int id_teacher, int * subjects) : Action(owner){
	m_id = id_teacher;
	m_subjects = subjects;
}
TeacherSubjectsUpdateAction::~TeacherSubjectsUpdateAction(){
	free(m_subjects);
}
bool TeacherSubjectsUpdateAction::Do(){
	int i = 0;
	bool success = true;
	School * school = m_owner->m_school;
	for(i = 0; i < school->n_subjects; ++i){
		int i_teaches = get_teaches_index_by_teacher_subj_id(school, m_id, school->subjects[i].id);
		Teacher * this_teacher = find_teacher_by_id(school, m_id);
		if(i_teaches >= 0 && m_subjects[i] == 0){
			if(remove_teaches(stdout, m_owner->m_database, school->teaches[i_teaches].id)){
				school_teaches_remove(school, i_teaches, false);
			} else {
				printf("remove teaches without success\n");
				success = false;
				break;
			}
		} else if((i_teaches == -1 && m_subjects[i] > 0) || (i_teaches >= 0 && m_subjects[i] != school->teaches[i_teaches].score)){
			Teaches t = (Teaches) {
				.teacher = this_teacher,
				.subject = &(school->subjects[i]),
				.max_per_day = this_teacher->max_meetings_per_day,
				.max_per_class_per_day = this_teacher->max_meetings_per_class_per_day,
				.score   = m_subjects[i],
				.room_scores = this_teacher->lecture_room_scores == NULL?NULL:int_list_copy(this_teacher->lecture_room_scores),
				.period_scores = this_teacher->lecture_period_scores == NULL?NULL:int_list_copy(this_teacher->lecture_period_scores),
				.twin_scores = (int*)calloc(school->n_periods_per_day + 1, sizeof(int)),
			};
			t.twin_scores[school->n_periods_per_day] = -1;
			success = insert_or_update_teaches(stdout, m_owner->m_database, &t, school);
			if(success){
				if(i_teaches < 0){
					school_teaches_add(school, &t, true);
				} else {
					Teaches * to_edit = &(school->teaches[i_teaches]);
					to_edit->teacher = t.teacher;
					to_edit->subject = t.subject;
					to_edit->max_per_day = t.max_per_day;
					to_edit->max_per_class_per_day = t.max_per_class_per_day;
					to_edit->score = t.score;
					to_edit->room_scores = t.room_scores;
					to_edit->period_scores = t.period_scores;
					to_edit->twin_scores = t.twin_scores;
				}
			} else {
				printf("Insert teaches without success\n");
			}
		}
	}
	return success;
}
bool TeacherSubjectsUpdateAction::Undo(){
	return Do();
}
wxString TeacherSubjectsUpdateAction::Describe(){
	return wxT("TeacherSubjectsUpdateAction");
}

/*********************************************************/
/*                TeacherDaysUpdateAction                */
/*********************************************************/

TeacherDaysUpdateAction::TeacherDaysUpdateAction(Application * owner, int id_teacher, int * max_per_day) : Action(owner){
	m_max_per_day = max_per_day;
	m_id = id_teacher;
}
TeacherDaysUpdateAction::~TeacherDaysUpdateAction(){
	free(m_max_per_day);
}
bool TeacherDaysUpdateAction::Do() {
	School * school = m_owner->m_school;
	if(update_teacher_day_max_per(m_owner->std_out, m_owner->m_database, m_id, m_max_per_day, school)){

		Teacher * teacher = find_teacher_by_id(m_owner->m_school, m_id);
		LMH_ASSERT(teacher != NULL);

		// memcpy?
		for(int i = 0; i < school->n_days; ++i){
			int tmp = teacher->day_max_meetings[i];
			teacher->day_max_meetings[i] = m_max_per_day[i];
			m_max_per_day[i] = tmp;
		}
		return true;
	}
	return false;
}
bool TeacherDaysUpdateAction::Undo() {
	return Do();
}
wxString TeacherDaysUpdateAction::Describe(){
	return wxT("TeacherDaysUpdateAction");
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
