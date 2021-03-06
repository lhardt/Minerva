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
	SubjectGroupMembersUpdateAction(Application * owner, int sgr_id, int * members);
	~SubjectGroupMembersUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int 	m_id;
	int	*	m_members;
};

class TeacherInsertAction : public Action {
public:
	TeacherInsertAction(Application * owner, Teacher t);
	~TeacherInsertAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	Teacher		m_teacher;
	Teaches *   m_teaches;
	int * 		m_subj_ids;
	ActionState m_state;
};

class TeacherDeleteAction : public Action {
public:
	TeacherDeleteAction(Application * owner, int id);
	~TeacherDeleteAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	Teacher		m_teacher;
	Teaches   * m_teaches;
	int 	  * m_assignment_pref;
	int	      * m_meeting_pref;
	bool 	  * m_set_on_meetings;
	ActionState m_state;
};

class TeacherBasicDataUpdateAction : public Action {
public:
	TeacherBasicDataUpdateAction(Application * owner, Teacher t, int id_teacher);
	~TeacherBasicDataUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int		m_id;
	Teacher m_teacher;
};

class TeacherLecturePeriodUpdateAction : public Action {
public:
	TeacherLecturePeriodUpdateAction(Application * owner, int id_teacher, int * scores);
	~TeacherLecturePeriodUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int		m_id;
	int   * m_scores;
};

class TeacherSubjectsUpdateAction : public Action {
public:
	TeacherSubjectsUpdateAction(Application * owner, int id_teacher, int * subjects);
	~TeacherSubjectsUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int 	m_id;
	int *	m_subjects;
};

class TeacherPlanningPeriodsUpdateAction : public Action {
public:
	TeacherPlanningPeriodsUpdateAction(Application * owner, int id_teacher, int * scores);
	~TeacherPlanningPeriodsUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int 		m_id;
	int   	  * m_scores;
};

class TeacherLecturePeriodsUpdateAction : public Action {
public:
	TeacherLecturePeriodsUpdateAction(Application * owner, int id_teacher, int * scores);
	~TeacherLecturePeriodsUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int 		m_id;
	int   	  * m_scores;
};

class TeacherDaysUpdateAction : public Action {
public:
	TeacherDaysUpdateAction(Application * owner, int id_teacher, int * max_per_day);
	~TeacherDaysUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int 		m_id;
	int   	  * m_max_per_day;
};

class TeacherTwinningUpdateAction : public Action {
public:
	TeacherTwinningUpdateAction(Application * owner, int id_teacher, int * twinning_scores);
	~TeacherTwinningUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int			m_id;
	int		  * m_twinning_scores;
};

class TeacherLectureRoomUpdateAction : public Action {
public:
	TeacherLectureRoomUpdateAction(Application * owner, int id_teacher, int * room_scores);
	~TeacherLectureRoomUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int			m_id;
	int 	  * m_scores;
};

class TeacherPlanningRoomUpdateAction : public Action {
public:
	TeacherPlanningRoomUpdateAction(Application * owner, int id_teacher, int * room_scores);
	~TeacherPlanningRoomUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int			m_id;
	int		  * m_scores;
};

class TeacherSubordinationUpdateAction : public Action {
public:
	TeacherSubordinationUpdateAction(Application * owner, int id_teacher, int * subordinates);
	~TeacherSubordinationUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int			m_id;
	int		  * m_subordinates;
};

class TeacherInGroupsUpdateAction : public Action {
public:
	TeacherInGroupsUpdateAction(Application * owner, int id_teacher, int * in_groups);
	~TeacherInGroupsUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();

private:
	int		m_id;
	int   * m_in_groups;
};

class ClassInsertAction : public Action {
public:
	ClassInsertAction(Application * owner, Class _class);
	~ClassInsertAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	Class		m_class;
	ActionState m_state;
};

class ClassAvailabilityUpdateAction : public Action {
public:
	ClassAvailabilityUpdateAction(Application * owner, int class_id, int * scores);
	~ClassAvailabilityUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int			m_id;
	int 	  * m_scores;
};

class ClassAssignmentsUpdateAction : public Action {
public:
	ClassAssignmentsUpdateAction(Application * owner, int class_id, int n_assignments, Assignment* assignments);
	~ClassAssignmentsUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int			m_id;
	int			m_n_assignments;

	// Warning: the following members cannot be used here, becausse this pointers may change along the reallocs.
	// 		m_assignments.subject;
	// 		m_assignments.m_class;
	Assignment * m_assignments;
	// Therefore we use an alternative;
	int		   * m_subject_ids;
	int		   * m_class_ids; // TODO: TOTALLY REDUNDANT. m_class_ids[x] will(should) always be m_id.
};

class ClassBasicDataUpdateAction : public Action {
public:
	ClassBasicDataUpdateAction(Application * owner, int id, Class c);
	~ClassBasicDataUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	Class m_class;
};

class ClassRoomsUpdateAction : public Action {
public:
	ClassRoomsUpdateAction(Application * owner, int id_class, int * scores);
	~ClassRoomsUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int			m_id;
	int		  * m_scores;
};

class ClassSubjectGroupsUpdateAction : public Action {
public:
	ClassSubjectGroupsUpdateAction(Application * owner, int id_class, int * max);
	~ClassSubjectGroupsUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int			m_id;
	int       * m_max;
};

class ClassInGroupsUpdateAction : public Action {
public:
	ClassInGroupsUpdateAction(Application * owner, int id_class, int * in_groups);
	~ClassInGroupsUpdateAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	int			m_id;
	int       * m_in_groups;
};

class ClassDeleteAction : public Action {
public:
	ClassDeleteAction(Application * owner, int id_class);
	~ClassDeleteAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	Class m_class;
	Assignment * m_assignments;
	int * m_subj_ids; // Ids of the subjects that compose m_Assignments, since the pointers can change with realloc
	int * m_sub_to; // This class is subordinated to what other classes?
};

class TimetableGenerateAction : public Action {
public:
	TimetableGenerateAction(Application * owner, char * name, char * desc);
	bool Do();
	bool Undo();
	wxString Describe();
private:
	Solution m_solution;
};

class TimetableDeleteAction : public Action {
public:
	TimetableDeleteAction(Application * owner, int id);
	~TimetableDeleteAction();
	bool Do();
	bool Undo();
	wxString Describe();
private:
	Solution m_solution;
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
