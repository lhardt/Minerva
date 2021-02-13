/*
 * Project Minerva.
 *   types.h - backbone definitions for Minerva.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains structure definitions that are crucial
 * to every other module in this program.
 */
#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef enum Period {
	per_NULL     = -1,
} Period;

typedef enum MeetingType {
	meet_NULL = 0,
	meet_PLANNING,
	meet_LECTURE
} MeetingType;

typedef struct SubjectQuantity SubjectQuantity;

typedef struct Room {
	int    id;
	char * name;
	char * short_name;
	int    size;
	int  * availability; /* Negative-terminated score list. */
	bool   active; /* Logical delete. To delete and not corrupt solutions */
} Room;

typedef struct Subject {
	int    id;
	char * name;
	char * short_name;
	int  * in_groups; /* Negative-terminated score list (0 or 1 suffices in this case) */
} Subject;

typedef struct Teaches Teaches;
typedef struct Teacher {
	int    			id;
	char 		  * name;
	char 		  * short_name;
	int				max_days;
	int    			max_meetings_per_day;
	int				max_meetings_per_class_per_day;
	int    			max_meetings;
	bool			planning_needs_room;
	int    			num_planning_periods;
	bool 			active; /* Logical delete. To delete and not corrupt solutions */
	Teaches      ** teaches; /* Null-terminated pointer list */
	int 		  * subordinates; /* Negative-terminated 0 or 1 list. */
	int 		  * planning_room_scores;
	int 		  * lecture_room_scores;
	int			  * day_max_meetings;
	int			  * day_scores;
	int    		  * lecture_period_scores;
	int 		  * planning_period_scores;
	int		  	  * planning_twin_scores; /* Negative-terminated score list. List[i-1] is the score given to i periods on a row */
} Teacher;

struct Teaches {
	int 		id;
	Teacher   * teacher;
	Subject   * subject;
	int			max_per_day;
	int			max_per_class_per_day;
	int  		score; /* How much the teacher likes to lecture this subject */
	int  	  * room_scores; /* Negative-terminated score list */
	int  	  * period_scores; /* Negative-terminated score list */
	int		  * twin_scores; /* Negative-terminated score list. List[i-1] is the score given to i periods on a row */
};

typedef struct Assignment Assignment;
typedef struct Class {
	int 			id;
	char 		  * name;
	char 		  * short_name;
	int 			size;
	int 			maximal_entry_period;
	int				minimal_exit_period;
	int 		  * period_scores; /* Negative-terminated score list */
	int 		  * room_scores; /* Negative-terminated score list */
	bool 			can_have_free_periods_flag;
	int 		  * subordinates; /* Negative-terminated score list */
	int 		  * max_per_day_subject_group;
	Assignment 	 ** assignments; /* Null-terminated pointer list */
	bool			active; /* Logical delete. To delete and not corrupt solutions */
} Class;

struct Assignment {
	int 	  id;
	Subject * subject;
	Class   * m_class;
	int		  amount;
	int 	  max_per_day;
	int 	* possible_teachers; /* Negative-terminated score list*/
};

typedef struct Meeting {
	int 			id; /* NOTE: ids are meetingtype-specific. */
	MeetingType 	type;
	Class      	  * m_class; /* Naming problem with c++ keyword */
	Subject    	  * subject;
	Teacher    	  * teacher;
	Room 	   	  * room;
	int 	     	period;
	int    	      * possible_periods; /* Negative-terminated score list */
	int			  * possible_rooms; /* Negative-terminated score list */
	int			  * possible_teachers; /* Negative-terminated score list */
} Meeting;

typedef struct Solution {
	int 	  id;
	char 	* name;
	char 	* desc;
	char 	* gen_date;
	Meeting * meetings;
	int 	  n_meetings;
} Solution;

typedef struct School {
	int 		 id;
	char 	   * name;

	char      ** day_names;
	char      ** daily_period_names;
	char      ** period_names;
	char 	  ** subject_group_names;

	int		   * day_ids;
	int 	   * daily_period_ids;
	int		   * period_ids;
	int		   * subject_group_ids;

	Class      * classes;
	Teacher    * teachers;
	Subject	   * subjects;
	Room       * rooms;
	Teaches    * teaches;
	Meeting    * meetings;
	Solution   * solutions;
	Assignment * assignments;

	int 		 n_periods;
	int 		 n_classes;
	int 		 n_teachers;
	int			 n_subjects;
	int 		 n_rooms;
	int			 n_teaches;
	int 		 n_days;
	int 		 n_periods_per_day;
	int			 n_solutions;
	int			 n_lectures;
	int			 n_planning;
	int			 n_meetings;
	int			 n_subject_groups;
	int			 n_assignments;

	int		   * periods;
} School;

#endif /* TYPES_H */
