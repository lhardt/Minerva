#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>

typedef struct {
	/* The name of the school. */
	char * name;
	/* Number of class days in the week. */
	int num_class_days;
	/* Number of classes per day. */
	int num_class_per_day;
	/* The maximum number of meetings a teacher can attend in one week. */
	int max_meetings_teacher_per_week;
	/* The maximum number of meetings a teacher can attend in one day. */
	int max_meetings_teacher_per_day;
	/* The maximum number of equal meetings in a row can happen. */
	int max_gemini_classes;
	/* Where the interval splits classes. */
	int interval_after;
} School;

typedef struct {
	char * name;
	int  * periods;
} Teacher;

typedef struct {
	char * name;
} Class;

typedef struct {
	Teacher * teacher;
	int		  quantity;
} TeacherQuantity;

typedef struct {
	Class * class;
	int 	quantity;
} ClassQuantity;

typedef struct {
	TeacherQuantity * teachers;
	uint64_t  teachers_size;
	char    * name;
} ExtendedClass;

typedef struct {
	ClassQuantity * classes;
	uint64_t classes_size;
	char * name;
} ExtendedTeacher;

typedef struct {
	Teacher * teacher;
	ExtendedClass   * class;
	int 	  period;
} Meeting;

#endif /* DEFINITIONS_H */
