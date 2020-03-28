/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
#ifndef TYPES_H
#define TYPES_H

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
typedef struct RoomFeatureQuantity RoomFeatureQuantity;

typedef struct RoomFeature {
	int id;
	char * name;

} RoomFeature;

typedef struct Room {
	int id;
	char * name;
	char * short_name;
	int size;
	/*this is not a list of fetures. Is rather a score given to all fetures, being 0 absent. */
	/* Last value must be -1. */
	int room_features[MAX_FEATURES + 1];
	int disponibility[MAX_PERIODS_PER_WEEK + 1];
} Room;

typedef struct Subject {
	int id;
	char * name;
	char * short_name;

	int * gemini_score;  	   // TODO is it necessary? Not present in diagram
	int * week_position_score; // TODO is it necessary? Not present in diagram
} Subject;

typedef struct Teaches Teaches;
typedef struct Teacher Teacher;
struct Teacher{
	int    			id;
	char 		  * name;
	char 		  * short_name;

	int    		  * periods;
	int    			max_meetings_per_day;
	int    			max_meetings;
	int    			num_planning_periods;
	// bool   			one_day_planning_periods; DEFAULT TO YES.

	Teaches      ** teaches;
	ClassQuantity * possible_classes;
	ClassQuantity * classes;

	Teacher      ** subordinates;

	int 		  * per_day_number_score;
};

struct Teaches{
	int id;
	Teacher * teacher;
	Subject * subject;
	int  score;
	int  features[MAX_FEATURES + 1];
	int  min_features[MAX_FEATURES + 1];
};

typedef struct Class Class;
struct Class{
	int 			id;
	char 		  * name;
	char 		  * short_name;

	int 			size;
	/* Not a list, but a list of scores */
	int 		    periods[MAX_PERIODS_PER_WEEK+1];
	bool 			can_have_free_periods_flag;
	int 			minimal_exit_period;
	int 			maximal_entry_period;
	/* If the class is abstract, the features are inherited,
	 *  but the classes dont need to be together. For preprocessing.
	 * Before the nucleus is ran, this should already have been substituted
	 *  in the subordinate classes. 
	 */
	bool abstract;

	Class       * * subordinates;

	SubjectQuantity * needs;
};


struct TeacherQuantity{
	Teacher * 		teacher;
	int				quantity;
};


struct ClassQuantity{
	Class * class;
	int 	quantity;
};

struct SubjectQuantity{
	Subject    	  * subject;
	int			 	quantity;
};

struct RoomFeatureQuantity{
	RoomFeature * feature;
	int			  quantity;
};

typedef struct ExtendedClass{
	int 			  id;
	TeacherQuantity * teachers;
	int 			* periods;
	char    		* name;
} ExtendedClass;


typedef struct ExtendedTeacher{
	int 			id;
	ClassQuantity * classes;
	int 		  * periods;
	char 		  * name;
} ExtendedTeacher;

/* TODO: The possible_* vectors create a significant overhead
 * on the memory used by the tree.
 */
typedef struct Meeting{
	/* Initially fixed */
	Class      	  * class;
	Subject    	  * subj;

	Teacher    	  * teacher;
	Room 	   	  * room;
	int 	     	period;

	/* A score given to each teacher, period and room, where
	 * possible_periods[i] is the score of periods[i] in this
	 * meeting. Similarly the others*/
	int    	      * possible_periods;
	int			  * possible_rooms;
	int			  * possible_teachers;
} Meeting;

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


typedef struct School {
	/* Current state */
	int 		 id;
	char 	   * name;

	char      ** day_names;
	char      ** daily_period_names;
	char      ** period_names;
	char 	  ** feature_names;

	int		   * day_ids;
	int 	   * daily_period_ids;
	int		   * feature_ids;
	int		   * period_ids;

	Class      * classes;
	Teacher    * teachers;
	Room       * rooms;
	Teaches    * teaches;
	Meeting    * all_meetings;

	int 		 n_periods;
	int 		 n_days;
	int 		 n_periods_per_day;
	int 		 n_features;
	int 		 n_rooms;
	int			 n_teaches;
	int 		 n_teachers;
	int 		 n_classes;

	bool 		 periods[MAX_PERIODS_PER_WEEK];

	int		 	 max_meetings_teacher_per_week;
	int 		 max_meetings_teacher_per_day;
	int 		 max_gemini_classes;

	/* Past states */
	EditAction * all_actions;
	int 		 current_action_index;
} School;

#endif /* TYPES_H */
