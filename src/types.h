/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_FEATURES          (64)
#define MAX_PERIODS_PER_DAY   (32)
#define MAX_PERIODS_PER_WEEK  (1024)
#define MAX_DAYS ((MAX_PERIODS_PER_WEEK)/(MAX_PERIODS_PER_DAY))

typedef enum Period {
	_NULL     = -1,
	_PLANNING = -2,
	_NOT_SET  = -3,
} Period;

typedef struct ClassQuantity ClassQuantity;
typedef struct TeacherQuantity TeacherQuantity;
typedef struct SubjectQuantity SubjectQuantity;

typedef struct RoomFeature {
	int id;
	char * name;

} RoomFeature;

typedef struct Room {
	int id;
	char * name;

	int size;

	/*this is not a list of fetures. Is rather a score given to all fetures, being 0 absent. */
	int room_features[MAX_FEATURES];
} Room;

typedef struct Subject {
	int id;
	char * name;
	char * short_name;

	int gemini_score[MAX_PERIODS_PER_DAY];
	int week_position_score[MAX_PERIODS_PER_WEEK];
} Subject;

typedef struct SubjectGroup {
	int    id;
	char * name;
	char * short_name;

	Subject * subjects;
};

typedef struct Teacher Teacher;
struct Teacher{
	int    id;
	char * name;
	char * short_name;

	int    periods[MAX_PERIODS_PER_WEEK];
	int    max_meetings_per_day;
	int    max_meetings_per_week;
	int    num_planning_periods;
	bool   one_day_planning_periods;
	int	   preferred_planning_periods;

	Subject       * teaches;
	ClassQuantity * possible_classes;
	ClassQuantity * classes;

	Teacher*      * subordinates;

	int per_day_number_score[MAX_DAYS];
};


typedef struct Class{
	int id;
	char * name;
	char * short_name;

	int periods[MAX_PERIODS_PER_WEEK];
	bool can_have_free_periods_flag;
	int minimal_exit_period;
	int maximal_entry_period;

	SubjectQuantity * needs;
} Class;


struct TeacherQuantity{
	Teacher * teacher;
	int		  quantity;
};


struct ClassQuantity{
	Class * class;
	int 	quantity;
};

typedef struct SubjectQuantity{
	Subject    * disc;
	int			 quantity;
};

typedef struct ExtendedClass{
	int id;
	TeacherQuantity * teachers;
	int * periods;
	char    * name;
} ExtendedClass;


typedef struct ExtendedTeacher{
	int id;
	ClassQuantity * classes;
	int * periods;
	char * name;
} ExtendedTeacher;

/* TODO: The possible_* vectors create a significant overhead
 * on the memory used by the tree.
 */
typedef struct Meeting{
	/* Initially fixed */
	Class      * class;
	Subject    * disc;

	Teacher    * teacher;
	Room 	   * room;
	int 	     period;

	int        * possible_periods;
	Room* 	   * possible_rooms;
	Teacher*   * possible_teachers;

} Meeting;


typedef struct {
	int id;
	/* The name of the school. */
	char * name;
	/** Number of days that can have classes. */
	int n_class_days;
	/** All periods that may have classes. */
	int n_periods_per_day;
	/** True or false value for if the school operates in that time or not */
	bool periods[MAX_PERIODS_PER_WEEK];
	/** The classes in school */
	ExtendedClass * classes;
	/** The teachers in school*/
	Teacher * teachers;
	/** The maximum number of meetings a teacher can attend in one week. */
	int max_meetings_teacher_per_week;
	/** The maximum number of meetings a teacher can attend in one day. */
	int max_meetings_teacher_per_day;
	/** The maximum number of equal meetings in a row that can happen. */
	int max_gemini_classes;
} School;


typedef enum EditActionActionType {
	kTestEmptyEnum
} EditActionActionType;

typedef enum EditActionObjectType {
	kTestEmptyEnum2
} EditActionObjectType;

typedef struct EditAction {
	int id;

	EditActionActionType action;
	EditActionObjectType object;
	void * data_before;
	void * data_after;
} EditAction;


typedef struct Universe {
	/* Current state */
	char ** days;
	char ** periods;
	School  school;
	Teacher * teachers;
	Meeting * all_meetings;
	/* Past states */
	EditAction * all_actions;
	int current_action_index;
} Universe;

#endif /* DEFINITIONS_H */
