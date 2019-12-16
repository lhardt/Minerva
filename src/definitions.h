#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>


typedef struct Teacher{
	char * name;
	int ** periods;
} Teacher;


typedef struct Class{
	char * name;
	int ** periods;
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
	TeacherQuantity * teachers;
	uint64_t  teachers_size;
	char    * name;
} ExtendedClass;


typedef struct ExtendedTeacher{
	ClassQuantity * classes;
	uint64_t classes_size;
	char * name;
} ExtendedTeacher;


typedef struct Meeting{
	Teacher * teacher;
	ExtendedClass   * class;
	int 	  period;
} Meeting;


typedef struct {
	/* The name of the school. */
	char * name;
	/** All periods that may have classes. */
	int  ** class_periods;
	/** The maximum number of meetings a teacher can attend in one week. */
	int max_meetings_teacher_per_week;
	/** The maximum number of meetings a teacher can attend in one day. */
	int max_meetings_teacher_per_day;
	/** The maximum number of equal meetings in a row that can happen. */
	int max_gemini_classes;
	// /** Where the interval splits classes. */
	// int interval_after;
	ExtendedClass * classes;
} School;


typedef struct Universe {
	char ** days;
	char ** periods;
	School  school;
	Teacher * teachers;
	Meeting * all_meetings;
} Universe;

int list_len(const char * const list[]){
	int len = 0;
	while(list[len] != NULL)
		len ++;
	return len;
}

#endif /* DEFINITIONS_H */
