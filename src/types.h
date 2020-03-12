#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>


typedef struct Teacher{
	int id;
	char * name;
	int * periods;
	int max_meetings_per_day;
} Teacher;


typedef struct Class{
	int id;
	char * name;
	int  * periods;
} Class;


typedef struct TeacherQuantity{
	Teacher * teacher;
	int		  quantity;
} TeacherQuantity;


typedef struct ClassQuantity{
	Class * class;
	int 	quantity;
} ClassQuantity;


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


typedef struct Meeting{
	Teacher * teacher;
	ExtendedClass   * class;
	int 	  period;
	/* In the case that period = -1, possiblePeriods is a list of
	 * possible periods that this meeting can be in.
	 */
	int * 	  possible_periods;
} Meeting;


typedef struct {
	int id;
	/* The name of the school. */
	char * name;
	/** Number of days that can have classes. */
	int n_class_days;
	/** All periods that may have classes. */
	int n_periods_per_day;
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
