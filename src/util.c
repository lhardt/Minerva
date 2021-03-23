/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "types.h"
#include "school_examples.h"

#define FREE_IF_NOT_NULL(x) do{ \
			if((x) != NULL){free(x);} \
		} while(0)


void print_teaches_list(Teaches ** teaches){
	printf("[");
	for(int i = 0 ; teaches[i] != NULL; ++i){
		printf("%s,  ", teaches[i]->subject->name);
	}
	printf("]\n");
}

void print_school_teaches_list(School *school){
	printf("[");
	for(int i = 0 ; i < school->n_teaches; ++i){
		printf("%s,  ", school->teaches[i].subject->name);
	}
	printf("]\n");
}


void debug_func(){
	School * school = (School *) calloc(1, sizeof(School));
	school->n_teachers = 2;
	school->n_teaches = 3;

	school->teachers = (Teacher *) calloc(3, sizeof(Teacher));
	school->teaches = (Teaches * ) calloc(3, sizeof(Teaches));
	school->subjects = (Subject *) calloc(4, sizeof(Subject));

	school->subjects[0] = (Subject){
		.id = 1,
		.name = "Math",

	};
	school->subjects[1] = (Subject){
		.id = 2,
		.name = "Port",

	};

	school->subjects[2] = (Subject){
		.id = 3,
		.name = "Hist",

	};
	school->teaches[0] = (Teaches){
			.id = 1,
			.teacher = &school->teachers[0],
			.subject = &school->subjects[0]
	};
	school->teaches[1] = (Teaches){
			.id = 2,
			.teacher = &school->teachers[1],
			.subject = &school->subjects[1]
	};
	school->teaches[2] = (Teaches){
			.id = 3,
			.teacher = &school->teachers[0],
			.subject = &school->subjects[2]
	};

	Teaches ** t1_teaches_ptr =(Teaches**) calloc(3 , sizeof(Teaches*));
	t1_teaches_ptr[0] = &school->teaches[0];
	t1_teaches_ptr[1] = &school->teaches[2];
	Teaches ** t2_teaches_ptr =(Teaches**) calloc(2 , sizeof(Teaches*));
	t2_teaches_ptr[0] = &school->teaches[1];

	school->teachers[0] = (Teacher){
		.id = 1,
		.name = "Rudolph",
		.short_name = "Rud",
		.teaches = t1_teaches_ptr,
	};
	school->teachers[1] = (Teacher){
		.id = 2,
		.name = "Dmitri",
		.short_name = "Dmi",
		.teaches = t2_teaches_ptr,
	};

	printf("----------\n");
	printf("n_teachers %d n_teaches %d teacher[0].teaches %d, teacher[1].teaches %d\n",
			school->n_teachers, school->n_teaches, school->teachers[0].teaches != NULL
			, school->teachers[1].teaches != NULL);
	print_teaches_list(school->teachers[0].teaches);
	print_teaches_list(school->teachers[1].teaches);
	printf("School->teaches (%d): ", school->n_teaches);
	print_school_teaches_list(school);
	printf("school->teaches[0].teacher %s\n", school->teaches[0].teacher->name);
	printf("----------\n");
	school_teacher_remove(school, 0, false);
	printf("----------\n");
	printf("n_teachers %d n_teaches %d teacher[0].teaches %d, teacher[1].teaches %d\n",
			school->n_teachers, school->n_teaches, school->teachers[0].teaches != NULL
			, school->teachers[1].teaches != NULL);
	printf("school->teaches[0].subj %s\n", school->teaches[0].subject->name);
	print_teaches_list(school->teachers[0].teaches);
	print_teaches_list(school->teachers[1].teaches);
	printf("School->teaches (%d): ", school->n_teaches);
	print_school_teaches_list(school);
	printf("school->teaches[0].teacher %s\n", school->teaches[0].teacher->name);
	printf("----------\n");
	// school_teaches_remove(school, 1, false);
	// printf("----------\n");
	// printf("n_teachers %d n_teaches %d teacher[0].teaches %d, teacher[1].teaches %d\n",
	// 		school->n_teachers, school->n_teaches, school->teachers[0].teaches != NULL
	// 		, school->teachers[1].teaches != NULL);
	// printf("school->teaches[0].subj %d\n", school->teaches[0].subject != NULL);
	// print_teaches_list(school->teachers[0].teaches);
	// print_teaches_list(school->teachers[1].teaches);
	// printf("school->teaches[0].teacher %s\n", school->teaches[0].teacher->name);
	// printf("School->teaches (%d): ", school->n_teaches);
	// print_school_teaches_list(school);
	// printf("----------\n");
}


/*********************************************************/
/*                     FREE Functions                    */
/*********************************************************/
void free_room(Room * room){
	LMH_ASSERT(room != NULL);
	FREE_IF_NOT_NULL(room->name);
	FREE_IF_NOT_NULL(room->short_name);
	FREE_IF_NOT_NULL(room->availability);
}
void free_subject(Subject * subject){
	LMH_ASSERT(subject != NULL);
	FREE_IF_NOT_NULL(subject->name);
	FREE_IF_NOT_NULL(subject->short_name);
	FREE_IF_NOT_NULL(subject->in_groups);
}
void free_teacher(Teacher * teacher){
	LMH_ASSERT(teacher != NULL);
	FREE_IF_NOT_NULL(teacher->name);
	FREE_IF_NOT_NULL(teacher->short_name);
	FREE_IF_NOT_NULL(teacher->teaches);
	FREE_IF_NOT_NULL(teacher->subordinates);
	FREE_IF_NOT_NULL(teacher->planning_room_scores);
	FREE_IF_NOT_NULL(teacher->lecture_room_scores);
	FREE_IF_NOT_NULL(teacher->day_max_meetings);
	FREE_IF_NOT_NULL(teacher->day_scores);
	FREE_IF_NOT_NULL(teacher->lecture_period_scores);
	FREE_IF_NOT_NULL(teacher->planning_period_scores);
	FREE_IF_NOT_NULL(teacher->planning_twin_scores);
}
void free_teaches(Teaches * teaches){
	LMH_ASSERT(teaches != NULL);
	FREE_IF_NOT_NULL(teaches->room_scores);
	FREE_IF_NOT_NULL(teaches->period_scores);
	FREE_IF_NOT_NULL(teaches->twin_scores);
}
void free_class(Class * class){
	LMH_ASSERT(class != NULL);
	FREE_IF_NOT_NULL(class->name);
	FREE_IF_NOT_NULL(class->short_name);
	FREE_IF_NOT_NULL(class->room_scores);
	FREE_IF_NOT_NULL(class->period_scores);
	FREE_IF_NOT_NULL(class->subordinates);
	FREE_IF_NOT_NULL(class->assignments);
}
void free_assignment(Assignment * assignment){
	LMH_ASSERT(assignment != NULL);
	FREE_IF_NOT_NULL(assignment->possible_teachers);
}
void free_meeting(Meeting * meeting){
	LMH_ASSERT(meeting != NULL);
	FREE_IF_NOT_NULL(meeting->possible_periods);
	FREE_IF_NOT_NULL(meeting->possible_rooms);
	FREE_IF_NOT_NULL(meeting->possible_teachers);
}
void free_meetings_list(Meeting * list){
	LMH_ASSERT(list != NULL);
	for(int i_met = 0; list[i_met].m_class != NULL; ++i_met){
		free_meeting(&list[i_met]);
	}
	free(list);
}
void free_node(DecisionNode * node){
	LMH_ASSERT(node != NULL);
	for(int i = 0; i < node->n_children; ++i){
		free_node(&node->children[i]);
	}
	FREE_IF_NOT_NULL(node->children);
	FREE_IF_NOT_NULL(node->children_score);
	FREE_IF_NOT_NULL(node->children_score_order);
	if(node->conclusion != NULL){
		free_meetings_list(node->conclusion);
	}
}
void free_solution(Solution * s){
	FREE_IF_NOT_NULL(s->name);
	FREE_IF_NOT_NULL(s->desc);
	for(int i = 0; i < s->n_meetings; ++i){
		free_meeting(&s->meetings[i]);
	}
	FREE_IF_NOT_NULL(s->meetings);
}
void free_school(School * s){
	int i;
	if(s != NULL){
		if(s->teachers != NULL){
			for(i = 0; i < s->n_teachers; ++i){
				free(s->teachers[i].planning_period_scores);
				free(s->teachers[i].lecture_period_scores);
			}
			free(s->teachers);
		}
		if(s->classes != NULL){
			for(i = 0; i < s->n_classes; ++i){
				free(s->classes[i].assignments);
			}
			free(s->classes);
		}

		free(s);
	}
}

/*********************************************************/
/*                   REALLOC Functions                   */
/*********************************************************/

/* We have elements s.a. meetings[i]->teacher, that point inside the block
 * allocated as school->teachers. So we can't use realloc because if it fails,
 * we won't be able to (universally) do ptr arithmetic
 * TODO optimize; confirm.
 */
bool realloc_teachers(School * school, int n_teachers, int i_start){
	LMH_ASSERT(school != NULL && n_teachers > 0 && n_teachers > school->n_teachers);
	LMH_ASSERT(i_start >= 0 && i_start <= school->n_teachers);
	Teacher * tmp = calloc(n_teachers+1, sizeof(Teacher));
	if(! tmp ){
		LMH_ASSERT(tmp != NULL);
		return false;
	}
	for(int i = 0; i < i_start; ++i){
		tmp[i] = school->teachers[i];
	}
	for(int i = i_start; i < school->n_teachers; ++i){
		tmp[i + n_teachers - school->n_teachers] = school->teachers[i];
	}
	/* Needs to repoint every reference to every teacher */
	for(int i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].teacher != NULL){
			int teacher_i = get_teacher_index_by_id(school, school->teaches[i].teacher->id);
			LMH_ASSERT(teacher_i != -1);
			if(teacher_i < i_start){
				school->teaches[i].teacher = & tmp[teacher_i];
			} else {
				school->teaches[i].teacher = & tmp[teacher_i + n_teachers - school->n_teachers];
			}
		}
	}
	for(int i = 0; i < school->n_meetings; ++i){
		if(school->meetings[i].teacher != NULL){
			int teacher_i = get_teacher_index_by_id(school, school->meetings[i].teacher->id);
			LMH_ASSERT(teacher_i != -1);
			if(teacher_i < i_start){
				school->meetings[i].teacher = & tmp[teacher_i];
			} else {
				school->meetings[i].teacher = & tmp[teacher_i + n_teachers - school->n_teachers];
			}
		}
	}
	for(int i = 0; i < school->n_solutions; ++i){
		int n_meetings = school->solutions[i].n_meetings;
		Meeting * meetings = school->solutions[i].meetings;
		for(int j = 0; j < n_meetings; ++j){
			if(meetings[j].teacher != NULL){
				int teacher_i = get_teacher_index_by_id(school, meetings[j].teacher->id);
				LMH_ASSERT(teacher_i != -1);
				if(teacher_i < i_start){
					meetings[j].teacher = & tmp[teacher_i];
				} else {
					meetings[j].teacher = & tmp[teacher_i + n_teachers - school->n_teachers];
				}
			}
		}
	}
	free(school->teachers);
	school->teachers = tmp;
	return true;
}
bool realloc_teaches(School * school, int n_teaches, int i_start){
	LMH_ASSERT(school != NULL && n_teaches > 0 && n_teaches > school->n_teaches);
	Teaches * tmp = calloc(n_teaches + 1, sizeof(Teaches));
	LMH_ASSERT(tmp != NULL);
	if(! tmp ){
		return false;
	}
	if(school->teaches != NULL){
		LMH_ASSERT(i_start <= school->n_teaches && i_start >= 0);
		for(int i = 0; i < i_start; ++i){
			tmp[i] = school->teaches[i];
		}
		for(int i = i_start; i < school->n_teaches; ++i){
			tmp[i + n_teaches - school->n_teaches] = school->teaches[i];
		}

		for(int i = 0; i < school->n_teachers; ++i){
			Teacher * teacher = & school->teachers[i];

			if(teacher->teaches == NULL) continue;

			for(int j = 0; teacher->teaches[j] != NULL; ++j){
				int teaches_i = get_teaches_index_by_id(school, teacher->teaches[j]->id);
				if(teaches_i != -1){
					printf("Teaches_i was %d when id was %d\n", teaches_i, teacher->teaches[j]->id);
					if(teaches_i >= i_start){
						teacher->teaches[j] = & tmp[teaches_i + n_teaches - school->n_teaches];
					} else {
						teacher->teaches[j] = & tmp[teaches_i];
					}
				} else {
					printf("Looking at an uninserted teaches...\n");
				}
			}
		}
		free(school->teaches);
	}
	school->teaches = tmp;
	return true;
}
bool realloc_classes(School * school, int n_classes){
	LMH_ASSERT(school != NULL && n_classes > 0 && n_classes > school->n_classes);
	Class * tmp = calloc(n_classes + 1, sizeof(Class));
	if(! tmp ){
		LMH_ASSERT(tmp != NULL);
		return false;
	}
	if(school->classes != NULL){
		for(int i = 0; i < school->n_classes; ++i){
			tmp[i] = school->classes[i];
		}
		for(int i = 0; i < school->n_assignments; ++i){
			int i_class = get_class_index_by_id(school, school->assignments[i].m_class->id);
			LMH_ASSERT(i_class != -1);
			school->assignments[i].m_class = &tmp[i_class];
		}
		for(int i = 0; i < school->n_meetings; ++i){
			int i_class = get_class_index_by_id(school, school->meetings[i].m_class->id);
			LMH_ASSERT(i_class != -1);
			school->meetings[i].m_class = &tmp[i_class];
		}
		for(int i = 0; i < school->n_solutions; ++i){
			Solution * solution = &school->solutions[i];
			for(int j = 0; j < solution->n_meetings; ++j) {
				int i_class = get_class_index_by_id(school, solution->meetings[j].m_class->id);
				LMH_ASSERT(i_class != -1);
				solution->meetings[j].m_class = &tmp[i_class];
			}
		}
		free(school->classes);
	}
	school->classes = tmp;
	return true;
}
bool realloc_subjects(School * school, int n_subjects){
	LMH_ASSERT(school != NULL && n_subjects > 0 && n_subjects > school->n_subjects);
	Subject * tmp = calloc(n_subjects + 1, sizeof(Subject));
	if(! tmp ){
		LMH_ASSERT(tmp != NULL);
		return false;
	}
	if(school->subjects){
		for(int i = 0; i < school->n_subjects; ++i){
			tmp[i] = school->subjects[i];
		}
		for(int i = 0; i < school->n_assignments; ++i){
			int i_subj = get_subject_index_by_id(school, school->assignments[i].subject->id);
			LMH_ASSERT(i_subj != 0);
			school->assignments[i].subject = &tmp[i_subj];
		}
		for(int i = 0; i < school->n_teaches; ++i){
			int i_subj = get_subject_index_by_id(school, school->teaches[i].subject->id);
			LMH_ASSERT(i_subj != 0);
			school->teaches[i].subject = &tmp[i_subj];
		}
		for(int i = 0; i < school->n_meetings; ++i){
			int i_subj = get_subject_index_by_id(school, school->meetings[i].subject->id);
			LMH_ASSERT(i_subj != 0);
			school->meetings[i].subject = &tmp[i_subj];
		}
		for(int i = 0; i < school->n_solutions; ++i){
			Solution * solution = & school->solutions[i];
			for(int j = 0; j < solution->n_meetings; ++j){
				int i_subj = get_subject_index_by_id(school, solution->meetings[j].subject->id);
				LMH_ASSERT(i_subj != 0);
				solution->meetings[j].subject = & tmp[i_subj];
			}
		}
		free(school->subjects);
	}

	school->subjects = tmp;
	return true;
}
bool realloc_rooms(School * school, int n_rooms){
	LMH_ASSERT(school != NULL && n_rooms > 0 && n_rooms > school->n_rooms);
	Room * tmp = calloc(n_rooms + 1, sizeof(Room));
	if(! tmp ){
		LMH_ASSERT(tmp != NULL);
		return false;
	}
	if(school->rooms != NULL){
		for(int i = 0; i < school->n_rooms; ++i){
			tmp[i] = school->rooms[i];
		}
		for(int i = 0; i < school->n_meetings; ++i){
			int i_room = get_room_index_by_id(school, school->meetings[i].room->id);
			LMH_ASSERT(i_room != 0);
			school->meetings[i].room = &tmp[i_room];
		}
		for(int i = 0; i < school->n_solutions; ++i){
			Solution * solution = & school->solutions[i];
			for(int j = 0; j < solution->n_meetings; ++j){
				int i_room = get_room_index_by_id(school, solution->meetings[j].room->id);
				LMH_ASSERT(i_room != 0);
				solution->meetings[j].room = &tmp[i_room];
			}
		}
		free(school->rooms);
	}
	school->rooms = tmp;
	return true;
}
bool realloc_assignments(School * school, int n_assignments){
	LMH_ASSERT(school != NULL && n_assignments > 0 && n_assignments > school->n_assignments);
	Assignment * tmp = calloc(n_assignments + 1, sizeof(Assignment));
	if(!tmp){
		LMH_ASSERT(tmp != NULL);
		return false;
	}
	if(school->assignments != NULL){
		for(int i = 0; i < school->n_assignments; ++i){
			tmp[i] = school->assignments[i];
		}
		for(int i = 0; i < school->n_classes; ++i){
			Assignment ** c_assignments = school->classes[i].assignments;
			if(c_assignments != NULL){
				for(int j = 0; c_assignments[j] != NULL; ++j){
					int i_assignment = get_assignment_index_by_id(school, c_assignments[j]->id);
					c_assignments[j] = & tmp[i_assignment];
				}
			}
		}
		free(school->assignments);
	}
	school->assignments = tmp;
	return true;
}

bool displace_teachers(School * school, Teacher * target,  int start, int n_teachers, int new_start){
	// we aren't checking if new_start <= school->n_teachers because this function
	// may be used to insert new teachers afterwards.
	LMH_ASSERT(school != NULL && start >= 0 && n_teachers >= 0 && new_start >= 0 );

	if(target == NULL){
		target = school->teachers;
	} else {
		for(int i = 0; i < school->n_teachers; ++i){
			// TODO: maybe if we didn't insert everyone it would be more efficient
			// after all, we are already redoing part of this in the next lines.
			target[i] = school->teachers[i];
		}
	}

	for(int i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].teacher != NULL){
			int teacher_i = get_teacher_index_by_id(school, school->teaches[i].teacher->id);
			LMH_ASSERT(teacher_i != -1);
			if(teacher_i >= start && teacher_i < start + n_teachers){
				school->teaches[i].teacher = & target[teacher_i - start + new_start];
			} else {
				school->teaches[i].teacher = & target[teacher_i];
			}
		}
	}

	for(int i = 0; i < school->n_meetings; ++i){
		if(school->meetings[i].teacher != NULL){
			int teacher_i = get_teacher_index_by_id(school, school->meetings[i].teacher->id);
			LMH_ASSERT(teacher_i != -1);
			if(teacher_i >= start && teacher_i < start + n_teachers){
				school->meetings[i].teacher = & target[teacher_i - start + new_start];
			} else {
				school->meetings[i].teacher = & target[teacher_i];
			}
		}
	}

	for(int i = 0; i < school->n_solutions; ++i){
		Solution * solution = & school->solutions[i];
		for(int j = 0; j < solution->n_meetings; ++j){
			if(solution->meetings[j].teacher != NULL){
				int teacher_i = get_teacher_index_by_id(school, solution->meetings[j].teacher->id);
				LMH_ASSERT(teacher_i != -1);
				if(teacher_i >= start && teacher_i < start + n_teachers){
					solution->meetings[j].teacher = & target[teacher_i - start + new_start];
				} else {
					solution->meetings[j].teacher = & target[teacher_i];
				}
			}
		}
	}

	if(new_start > start){
		for(int i = n_teachers - 1; i > 0; --i){
			target[new_start + i] = school->teachers[start + i];
		}
	} else {
		for(int i = 0; i < school->n_teachers; ++i){
			target[new_start + i] = school->teachers[start + i];
		}
	}
	return true;
}

/*********************************************************/
/*                     COPY Functions                    */
/*********************************************************/
/* Can't assume that strdup exists. */
char* copy_string(const char * const str){
	int len;
	char * copy = NULL;
	if(str != NULL){
		len = strlen(str);
		copy = malloc((len+1) * sizeof(char));
		strncpy(copy, str, len);
		copy[len] = '\0';
	}
	return copy;
}

Meeting * copy_meetings_list(const Meeting * const list){
	Meeting * copy;
	int i, n;
	for(n = 0; list[n].type != meet_NULL; ++n){ }

	copy = calloc(n+1, sizeof(Meeting));
	for(i = 0; i < n; ++i){
		copy[i] = list[i];
		copy[i].possible_periods = int_list_copy(copy[i].possible_periods);
		copy[i].possible_rooms = int_list_copy(copy[i].possible_rooms);
		copy[i].possible_teachers = int_list_copy(copy[i].possible_teachers);
	}
	return copy;
}

/* INT LIST COPY
 *		allocates an exact copy of the null-terminated int list.
 *
 * Development status:
 *		Implemented.
 */
int * int_list_copy(const int * const list){
	int i, n, *copy;

	for(n = 0; list[n] >= 0; ++n){ }

	copy = calloc(n+1, sizeof(int));

	for(i = 0; i < n; ++i){
		copy[i] = list[i];
	}
	copy[n] = -1;
	return copy;
}

int * int_list_n_copy(const int * const list, int n){
	int i, *copy;

	copy = calloc(n+1, sizeof(int));

	for(i = 0; i < n; ++i){
		copy[i] = list[i];
	}
	copy[n] = -1;
	return copy;
}

School * copy_school(const School * const s){
	int i = 0, j=0, index;
	School * copy = calloc(1, sizeof(School));

	copy->id = s->id;
	copy->name = copy_string(s->name);
	copy->n_periods = s->n_periods;
	copy->n_classes = s->n_classes;
	copy->n_teachers = s->n_teachers;
	copy->n_subjects = s->n_subjects;
	copy->n_rooms = s->n_rooms;
	copy->n_days = s->n_days;
	copy->n_periods_per_day = s->n_periods_per_day;
	copy->n_solutions = s->n_solutions;

	if(copy->n_days > 0){
		copy->day_names = calloc(copy->n_days, sizeof(char*));
		copy->day_ids = calloc(copy->n_days, sizeof(int));
		for(i = 0; i < copy->n_days; ++i){
			copy->day_names[i] = copy_string(s->day_names[i]);
			copy->day_ids[i] = s->day_ids[i];
		}
	}
	if(copy->n_periods_per_day > 0){
		copy->daily_period_names = calloc(copy->n_periods_per_day, sizeof(char*));
		copy->daily_period_ids = calloc(copy->n_periods_per_day, sizeof(int));
		for(i = 0; i < copy->n_periods_per_day; ++i){
			copy->daily_period_names[i] = copy_string(s->daily_period_names[i]);
			copy->daily_period_ids[i] = s->daily_period_ids[i];
		}
	}
	if(copy->n_periods > 0){
		copy->period_names = calloc(copy->n_periods, sizeof(char*));
		copy->period_ids = calloc(copy->n_periods, sizeof(int));
		for(i = 0; i < copy->n_periods; ++i){
			copy->period_names[i] = copy_string(s->period_names[i]);
			copy->period_ids[i] = s->period_ids[i];
		}
	}
	if(copy->n_subject_groups > 0){
		copy->subject_group_names = calloc(copy->n_subject_groups, sizeof(char*));
		copy->subject_group_ids = calloc(copy->n_subject_groups, sizeof(int));
		for(i = 0; i < copy->n_subject_groups; ++i){
			copy->subject_group_names[i] = copy_string(s->subject_group_names[i]);
			copy->subject_group_ids[i] = s->subject_group_ids[i];
		}
	}
	if(copy->n_periods > 0){
		for(i = 0; i < copy->n_periods; ++i){
			copy->periods[i] = s->periods[i];
		}
	}
	if(copy->n_subjects > 0){
		copy->subjects = calloc(copy->n_subjects, sizeof(Subject));
		for(i = 0; i < copy->n_subjects; ++i){
			copy->subjects[i].id = s->subjects[i].id;
			copy->subjects[i].name = copy_string(s->subjects[i].name);
			copy->subjects[i].short_name = copy_string(s->subjects[i].short_name);
			for(j = 0; j < s->n_subject_groups; ++j){
				copy->subjects[i].in_groups[j] = s->subjects[i].in_groups[j];
			}
		}
	}
	if(copy->n_rooms > 0){
		copy->rooms = calloc(copy->n_rooms, sizeof(Room));
		for(i = 0; i < copy->n_rooms; ++i){
			copy->rooms[i].id = s->rooms[i].id;
			copy->rooms[i].name = copy_string(s->rooms[i].name);
			copy->rooms[i].short_name = copy_string(s->rooms[i].short_name);
			copy->rooms[i].size = s->rooms[i].size;

			for(j = 0; j < copy->n_periods; ++j){
				copy->rooms[i].availability[j] = s->rooms[i].availability[j];
			}
		}
	}
	if(copy->n_teachers > 0){
		copy->teachers = calloc(copy->n_teachers, sizeof(Teacher));
		for(i = 0; i < copy->n_teachers; ++i){
			copy->teachers[i].id = s->teachers[i].id;
			copy->teachers[i].name = copy_string(s->teachers[i].name);
			copy->teachers[i].short_name = copy_string(s->teachers[i].short_name);
			copy->teachers[i].max_days = s->teachers[i].max_days;
			copy->teachers[i].max_meetings = s->teachers[i].max_meetings;
			copy->teachers[i].max_meetings_per_day = s->teachers[i].max_meetings_per_day;
			copy->teachers[i].max_meetings_per_class_per_day = s->teachers[i].max_meetings_per_class_per_day;
			copy->teachers[i].num_planning_periods = s->teachers[i].num_planning_periods;
			for(j = 0; j < copy->n_days; ++j){
				copy->teachers[i].day_max_meetings[j] = s->teachers[i].day_max_meetings[j];
				copy->teachers[i].day_scores[j] = s->teachers[i].day_scores[j];
			}
		}
	}
	if(copy->n_teaches > 0){
		copy->teaches = calloc(copy->n_teaches, sizeof(Teaches));
		for(i = 0; i < copy->n_teaches; ++i){
			if(s->teaches[i].teacher != NULL){
				index = s->teaches[i].teacher - s->teachers;
				if(index >= 0 && index < copy->n_teaches){
					copy->teaches[i].teacher = &copy->teachers[index];
				} else {
					printf("wtf");
				}
			} else {
				break;
			}
			if(s->teaches[i].subject != NULL){
				index = s->teaches[i].subject - s->subjects;
				if(index >= 0 && index < copy->n_subjects){
					copy->teaches[i].subject = &copy->subjects[index];
				} else {
					printf("wtf");
				}
			} else {
				break;
			}
		}
	}
	if(copy->n_teaches > 0 && copy->n_teachers > 0){
		for(i = 0; i < copy->n_teachers; ++i){
			if(s->teachers[i].teaches != NULL){
				int n_teaches;
				for(n_teaches = 0; s->teachers[i].teaches[n_teaches]->subject != NULL; ++n_teaches ){
					/* Intentionally Blank */
				}
				copy->teachers[i].teaches = calloc(n_teaches + 1, sizeof(Teaches *));
				for(j = 0; j < n_teaches; ++j){
					int index = s->teachers[i].teaches[j]->subject - s->subjects;
					copy->teachers[i].teaches[j] = &copy->teaches[index];
				}
			}
		}
	}

	// TODO  Copy Assignemnts

	if(copy->n_classes > 0){
		copy->classes = calloc(copy->n_classes, sizeof(Class));
		for(i = 0; i < copy->n_classes; ++i){
			int n_needs;

			copy->classes[i].id = s->classes[i].id;
			copy->classes[i].name = copy_string(s->classes[i].name);
			copy->classes[i].short_name = copy_string(s->classes[i].short_name);
			copy->classes[i].size = s->classes[i].size;
			for(j = 0; j < copy->n_periods; ++j){
				copy->classes[i].period_scores[j] = s->classes[i].period_scores[j];
			}
			copy->classes[i].can_have_free_periods_flag = s->classes[i].can_have_free_periods_flag;
			copy->classes[i].maximal_entry_period = s->classes[i].maximal_entry_period;
			copy->classes[i].minimal_exit_period = s->classes[i].minimal_exit_period;
			copy->classes[i].subordinates = int_list_n_copy(s->classes[i].subordinates,copy->n_classes);

			if(s->classes[i].assignments != NULL){
				for(n_needs = 0; s->classes[i].assignments[n_needs]->subject != NULL; ++n_needs){
					/* Blank on purpouse */
				}
				copy->classes[i].assignments = calloc(n_needs, sizeof(Assignment*));
				for(j = 0; j < n_needs; ++j){
					if(index >= 0 && index < s->n_subjects){
						index = s->classes[i].assignments[j] - s->assignments;
						copy->classes[i].assignments[j] = &copy->assignments[index];
					}
				}
			}
		}
	}

	if(copy->n_solutions > 0){
		copy->solutions = calloc(copy->n_solutions, sizeof(Solution));
		for(i = 0; i < copy->n_solutions; ++i){
			Solution * new_sol = &copy->solutions[i];
			Solution * old_sol = &s->solutions[i];
			new_sol->id = old_sol->id;
			new_sol->n_meetings = old_sol->n_meetings;
			new_sol->name = copy_string(old_sol->name);
			new_sol->desc = copy_string(old_sol->desc);

			if(old_sol->meetings != NULL && old_sol->n_meetings > 0){
				new_sol->meetings = copy_meetings_list(old_sol->meetings);
				new_sol->n_meetings = old_sol->n_meetings;

				/* Needs to do 'repointing's */

				for(j = 0; j < new_sol->n_meetings; ++j){
					if(new_sol->meetings[j].m_class != NULL){
						// the old m_class was indexed on s->school.classes.
						index = new_sol->meetings[j].m_class - s->classes;
						if(index >= 0 && index < copy->n_classes){
							new_sol->meetings[j].m_class = &copy->classes[index];
						} else {
							printf("Wtf");
						}
					} else {
						break;
					}

					if(new_sol->meetings[j].teacher != NULL){
						index = new_sol->meetings[j].teacher - s->teachers;
						if(index >= 0 && index < copy->n_teachers){
							new_sol->meetings[j].teacher = &copy->teachers[index];
						} else {
							printf("Wtf");
						}
					} else {
						break;
					}

					if(new_sol->meetings[j].room != NULL){
						index = new_sol->meetings[j].room - s->rooms;
						if(index >= 0 && index < copy->n_rooms){
							new_sol->meetings[j].room = &copy->rooms[index];
						} else {
							printf("Wtf");
						}
					} else {
						break;
					}
				}
			}
		}
	}

	return copy;
}

/* XXX probably unreadable code. But it just copies. */
SchoolExample * copy_example(const SchoolExample * const ex){
	int i, j, index;

	SchoolExample * copy = calloc(1, sizeof(SchoolExample));
	School * copied_school = copy_school(&ex->school);
	copy->school = *copied_school;
	free(copied_school); // Shallow free.
	copy->n_possible_solutions = ex->n_possible_solutions;
	copy->possible_solutions = calloc(ex->n_possible_solutions, sizeof(Solution));

	for(i = 0; i < copy->n_possible_solutions; ++i){
		Solution * new_sol = &copy->possible_solutions[i];
		Solution * old_sol = &ex->possible_solutions[i];
		new_sol->id = old_sol->id;
		new_sol->n_meetings = old_sol->n_meetings;
		new_sol->name = copy_string(old_sol->name);
		new_sol->desc = copy_string(old_sol->desc);

		if(old_sol->meetings != NULL && old_sol->n_meetings > 0){
			new_sol->meetings = copy_meetings_list(old_sol->meetings);
			new_sol->n_meetings = old_sol->n_meetings;

			/* Needs to do 'repointing's */

			for(j = 0; j < new_sol->n_meetings; ++j){
				if(new_sol->meetings[j].m_class != NULL){
					// the old m_class was indexed on ex->school.classes.
					index = new_sol->meetings[j].m_class - ex->school.classes;
					if(index >= 0 && index < copy->school.n_classes){
						new_sol->meetings[j].m_class = &copy->school.classes[index];
					} else {
						printf("Wtf");
					}
				} else {
					break;
				}

				if(new_sol->meetings[j].teacher != NULL){
					index = new_sol->meetings[j].teacher - ex->school.teachers;
					if(index >= 0 && index < copy->school.n_teachers){
						new_sol->meetings[j].teacher = &copy->school.teachers[index];
					} else {
						printf("Wtf");
					}
				} else {
					break;
				}

				if(new_sol->meetings[j].room != NULL){
					index = new_sol->meetings[j].room - ex->school.rooms;
					if(index >= 0 && index < copy->school.n_rooms){
						new_sol->meetings[j].room = &copy->school.rooms[index];
					} else {
						printf("Wtf");
					}
				} else {
					break;
				}
			}
		}
	}
	return copy;
}

/*********************************************************/
/*                    PRINT Functions                    */
/*********************************************************/

void print_int_list(FILE * out, const int * const list){
	int i = 0;

	if(list == NULL){
		fprintf(out, "(nul)");
		return;
	}
	if(list[0] != -1){
		fprintf(out, "[%d", list[0]);
		for(i = 1; list[i] != -1; i++){
			fprintf(out, ", %d", list[i]);
		}
		fprintf(out,"]");
	} else {
		fprintf(out, "[]");
	}
}
void print_sized_int_list(FILE * out, const int * const list, const int size){
	int i = 0;

	if(list == NULL){
		fprintf(out, "(nul)");
		return;
	}
	if(size > 0){
		fprintf(out, "[%d", list[0]);
		for(i = 1; i < size; i++){
			fprintf(out, ", %d", list[i]);
		}
		fprintf(out,"]");
	} else {
		fprintf(out, "[]");
	}
}
void print_bool_list(FILE * out, const bool * const list, const int size){
	int i = 0;
	if(list == NULL){
		fprintf(out, "(nul)");
		return;
	}
	if(size == 0){
		fprintf(out, "[]");
	}
	if(size > 0){
		fprintf(out,"[%c", list[i]?'t':'f');
		for(i = 1; i < size; i++){
			printf(", %c", list[i]?'t':'f');
		}
		fprintf(out,"]");
	}
}
void print_meeting_list(FILE * out, const Meeting * const meetings){
	int i = 0;

	if(meetings == NULL){
		printf("Returning on null");
		return;
	}

	if(meetings[0].m_class == NULL){
		printf("No meetings\n");
	} else {
		for(i = 0; meetings[i].m_class != NULL; i++){
			fprintf(out, "Meeting %2d %s: T(%s) ",
					i,
					meetings[i].m_class->name,
					(meetings[i].teacher == NULL? "":meetings[i].teacher->name));
			print_int_list(out,meetings[i].possible_teachers);
			fprintf(out, "; R(%s) ", meetings[i].room ==NULL? "":meetings[i].room->name);
			print_int_list(out,meetings[i].possible_rooms);
			fprintf(out, "; P(%d) ", meetings[i].period);
			print_int_list(out,meetings[i].possible_periods);
			fprintf(out, "; S(%s)", meetings[i].subject->name);
			fprintf(out,"\n");
		}
	}
}
void print_short_meeting_list(FILE * out, const Meeting * const meetings){
	int i = 0;

	if(meetings == NULL){
		printf("Returning on null");
		return;
	}

	for(i = 0; meetings[i].m_class != NULL; i++){
		fprintf(out, "Meeting %2d %s: T(%s) ",
					i,
					meetings[i].m_class->name,
					(meetings[i].teacher == NULL? "":meetings[i].teacher->name));
		fprintf(out, "; R(%s) ", meetings[i].room ==NULL? "":meetings[i].room->name);
		fprintf(out, "; P(%d) ", meetings[i].period);
		fprintf(out,"\n");
	}
}
void print_teacher(FILE * out, const Teacher * const t){
	if(t && out){
		fprintf(out, "Teacher: \n\tid: %d", t->id);
		fprintf(out, "\n\tname: %s", t->name);
		fprintf(out, "\n\tsname: %s", t->short_name);
		fprintf(out, "\n\tmaxdays: %d", t->max_days);
		fprintf(out, "\n\tmax_meet: %d", t->max_meetings);
		fprintf(out, "\n\tmax_pd: %d", t->max_meetings_per_day);
		fprintf(out, "\n\tmax_pcpd: %d", t->max_meetings_per_class_per_day);
		fprintf(out, "\n\tnum_plan: %d", t->num_planning_periods);
		fprintf(out, "\n\tday_max_meet: ");
		print_int_list(out, t->day_max_meetings);
		fprintf(out,"\n\tday_scores: ");
		print_int_list(out, t->day_scores);
		fprintf(out,"\n\tlecture_period_scores: ");
		print_int_list(out, t->lecture_period_scores);
		fprintf(out,"\n\tplanning_period_scores: ");
		print_int_list(out, t->planning_period_scores);
		fprintf(out,"\n\tplanning_twin_scores: ");
		print_int_list(out, t->planning_twin_scores);
		fprintf(out,"\n\tlecture_room_scores: ");
		print_int_list(out, t->lecture_room_scores);
		fprintf(out,"\n\tplanning_room_scores: ");
		print_int_list(out, t->planning_room_scores);
		fprintf(out,"\n\tsubordinates: ");
		print_int_list(out, t->subordinates);
		fprintf(out, "\n\tteaches: ");
		for(int i = 0; t->teaches != NULL && t->teaches[i] != NULL && t->teaches[i]->subject != NULL; ++i){
			printf("(%s, %d), ", t->teaches[i]->subject->name, t->teaches[i]->score);
		}
		printf("\n");
	}
}
void print_school(FILE * out, const School * const s){
	if(s && out){
		fprintf(out, "School:\n\tname:	 %s\n", s->name);
		fprintf(out, "\tn_periods: 		 %d\n", s->n_periods);
		fprintf(out, "\tn_periods_per_day: %d\n", s->n_periods_per_day);
		fprintf(out, "\tn_days: 			 %d\n", s->n_days);
		fprintf(out, "\tn_per: 			 %d\n", s->n_periods);
		fprintf(out, "\tn_classes: 		 %d\n", s->n_classes);
		fprintf(out, "\tn_teachers: 	 	 %d\n", s->n_teachers);
		fprintf(out, "\tn_subjects: 	 	 %d\n", s->n_subjects);
		fprintf(out, "\tn_rooms:	 	 	 %d\n", s->n_rooms);
		fprintf(out, "\tn_teaches: 	 	 %d\n", s->n_teaches);
		fprintf(out, "\tn_subj_groups: 	 %d\n", s->n_subject_groups);

		// printf("\t\t")
	}
}

/*********************************************************/
/*                 FIND AND GET Functions                */
/*********************************************************/

Room * find_room_by_id(School * school, int id){
	int i;

	for(i = 0; i < school->n_rooms; ++i){
		if(school->rooms[i].id == id){
			return &(school->rooms[i]);
		}
	}
	return NULL;
}
int get_room_index_by_id(School * school, int id){
	int i;

	for(i = 0; i < school->n_rooms; ++i){
		if(school->rooms[i].id == id){
			return i;
		}
	}
	return -1;
}
Class * find_class_by_id(School * school, int id) {
	int i;
	for(i = 0; i < school->n_classes; ++i){
		if(school->classes[i].id == id){
			return &(school->classes[i]);
		}
	}
	printf("Did not find class.\n");
	return NULL;
}
int get_class_index_by_id(School * school, int id){
	for(int i = 0; i < school->n_classes; ++i){
		if(school->classes[i].id == id){
			return i;
		}
	}
	return -1;
}
Teacher * find_teacher_by_id(School * school, int id){
	int i;

	for(i = 0; i < school->n_teachers; ++i){
		if(school->teachers[i].id == id){
			return &(school->teachers[i]);
		}
	}
	return NULL;
}
int get_teacher_index_by_id(School * school, int id){
	int i;

	for(i = 0; i < school->n_teachers; ++i){
		if(school->teachers[i].id == id){
			return i;
		}
	}
	return -1;
}
Meeting * find_meeting_by_id(School * school, int id){
	int i;

	for(i = 0; i < school->n_meetings; ++i){
		if(school->meetings[i].id == id){
			return &(school->meetings[i]);
		}
	}
	return NULL;
}
Subject * find_subject_by_id(School * school, int id){
	for(int i = 0; i < school->n_subjects; ++i){
		if(school->subjects[i].id == id){
			return &(school->subjects[i]);
		}
	}
	return NULL;
}
Teaches * 		find_teaches_by_teacher_subj_id(School * school, int id_teacher, int id_subj){
	for(int i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].teacher->id == id_teacher && school->teaches[i].subject->id == id_subj){
			return &school->teaches[i];
		}
	}
	return NULL;
}
Teaches * 		find_teaches_by_id(School * school, int id){
	for(int i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].id == id){
			return &school->teaches[i];
		}
	}
	return NULL;
}
Solution * find_solution_by_id(School * school, int id){
	int i;
	for(i = 0; i < school->n_solutions; ++i){
		if(school->solutions[i].id == id){
			return &(school->solutions[i]);
		}
	}
	return NULL;
}
int get_subject_index_by_id(School * school, int id){
	int i;

	for(i = 0; i < school->n_subjects; ++i){
		if(school->subjects[i].id == id){
			return i;
		}
	}
	return -1;
}
int get_day_index_by_id(School * school, int id){
	int i;
	for(i = 0; i < school->n_days; ++i){
		if(school->day_ids[i] == id){
			return i;
		}
	}
	return -1;
}
int get_per_index_by_id(School * school, int id){
	int i;
	for(i = 0; i < school->n_periods; ++i){
		if(school->period_ids[i] == id){
			return i;
		}
	}
	return -1;
}
Assignment * find_assignment_by_class_subj_id(School * school, int id_class, int id_subj){
	for(int i = 0; i < school->n_assignments; ++i){
		LMH_ASSERT(NULL != school->assignments[i].m_class);
		LMH_ASSERT(NULL != school->assignments[i].subject);
		if(school->assignments[i].m_class->id == id_class && school->assignments[i].subject->id == id_subj){
			return &school->assignments[i];
		}
	}
	return NULL;
}
Assignment * find_assignment_by_id(School * school, int id){
	for(int i = 0; i < school->n_assignments; ++i){
		if(school->assignments[i].id == id){
			return &school->assignments[i];
		}
	}
	return NULL;
}
int get_daily_period_index_by_id(School * school, int id){
	int i;
	for(i = 0; i < school->n_periods_per_day; ++i){
		if(school->daily_period_ids[i] == id){
			return i;
		}
	}
	return -1;
}
int get_subject_group_index_by_id(School * school, int id){
	int i;
	for(i = 0; i < school->n_subject_groups; ++i){
		if(school->subject_group_ids[i] == id){
			return i;
		}
	}
	return -1;
}
int get_solution_index_by_id(School * school, int id){
	int i;
	for(i = 0; i < school->n_solutions; ++i){
		if(school->solutions[i].id == id){
			return i;
		}
	}
	return -1;
}
int get_teaches_index_by_teacher_subj_id(School * school, int id_teacher, int id_subj){
	int i;
	for(i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].teacher->id == id_teacher && school->teaches[i].subject->id == id_subj){
			return i;
		}
	}
	return -1;
}
int 			get_teaches_index_by_id(School * school, int id){
	int i;
	for(i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].id == id){
			return i;
		}
	}
	return -1;
}
int				get_assignment_index_by_id(School * school, int id){
	LMH_ASSERT(school != NULL && id > 0);
	for(int i = 0; i < school->n_assignments; ++i){
		if(school->assignments[i].id == id){
			return i;
		}
	}
	return -1;
}
int				get_assignment_index_by_class_subj_id(School * school, int id_class, int id_subj){
	LMH_ASSERT(school!= NULL && id_class > 0 && id_subj > 0);
	for(int i = 0; i < school->n_assignments; ++i){
		Assignment * assignment = & school->assignments[i];
		LMH_ASSERT(assignment != NULL);
		LMH_ASSERT(assignment->id > 0);
		LMH_ASSERT(assignment->m_class);
		LMH_ASSERT(assignment->subject);
		if(assignment->m_class->id == id_class && assignment->subject->id == id_subj){
			return i;
		}
	}
	return -1;
}

/*********************************************************/
/*                 ADD AND REMOVE Functions              */
/*********************************************************/

bool can_insert_class(School * school, Class * c){
	LMH_ASSERT(school != NULL && c != NULL);
	for(int i = 0; i < school->n_classes; ++i){
		if(0 == strcmp(c->name, school->classes[i].name)){
			return false;
		}
	}
	return true;
}
bool can_insert_room(School * school, Room * r){
	LMH_ASSERT(school != NULL && r != NULL);
	for(int i = 0; i < school->n_rooms; ++i){
		if(0 == strcmp(r->name, school->rooms[i].name)){
			return false;
		}
	}
	return true;
}
bool can_insert_teacher(School * school, Teacher * t){
	LMH_ASSERT(school != NULL && t != NULL);
	for(int i = 0; i < school->n_teachers; ++i){
		if(0 == strcmp(t->name, school->teachers[i].name)){
			return false;
		}
	}
	return true;
}
bool can_insert_subject(School * school, Subject * s){
	LMH_ASSERT(school != NULL && s != NULL);
	for(int i = 0; i < school->n_subjects; ++i){
		if(0 == strcmp(s->name, school->subjects[i].name)){
			return false;
		}
	}
	return true;
}
bool can_insert_subject_group(School * school, char * name, int * members){
	// It does not care about members because two subjectgroups can have the same members,
	// for example, none, so that they can be altered later.
	LMH_ASSERT(school != NULL && name != NULL);
	for(int i = 0; i < school->n_subject_groups; ++i){
		if(0 == strcmp(name, school->subject_group_names[i])){
			return false;
		}
	}
	return true;
}

bool can_remove_class(School * school, int id){
	for(int i = 0; i < school->n_solutions; ++i){
		Meeting * m_list = school->solutions[i].meetings;
		if(m_list != NULL){
			for(int j = 0; m_list[j].type != meet_NULL; ++j){
				if(m_list[j].type == meet_LECTURE && m_list[j].m_class && m_list[j].m_class->id == id){
					return false;
				}
			}
		}
	}
	return true;
}
bool can_remove_room(School * school, int id){
	for(int i = 0; i < school->n_solutions; ++i){
		Meeting * m_list = school->solutions[i].meetings;
		if(m_list != NULL){
			for(int j = 0; m_list[j].type != meet_NULL; ++j){
				if(m_list[j].type != meet_NULL && m_list[j].room && m_list[j].room->id == id){
					return false;
				}
			}
		}
	}
	return true;
}
bool can_remove_teacher(School * school, int id){
	for(int i = 0; i < school->n_solutions; ++i){
		Meeting * m_list = school->solutions[i].meetings;
		if(m_list != NULL){
			for(int j = 0; m_list[j].type != meet_NULL; ++j){
				if(m_list[j].type != meet_NULL && m_list[j].teacher && m_list[j].teacher->id == id){
					return false;
				}
			}
		}
	}
	return true;
}
bool can_remove_subject(School * school, int id){
	for(int i = 0; i < school->n_solutions; ++i){
		Meeting * m_list = school->solutions[i].meetings;
		if(m_list != NULL){
			for(int j = 0; m_list[j].type != meet_NULL; ++j){
				if(m_list[j].type != meet_NULL && m_list[j].subject && m_list[j].subject->id == id){
					return false;
				}
			}
		}
	}
	return true;
}

/* if *list_ptr == NULL, calloc. Else realloc */
void add_zeroes_to_score_list(int ** list_ptr, int n_old, int n_new){
	LMH_ASSERT(list_ptr != NULL && n_old >= 0 && n_old < n_new);
	int i;

	if(*list_ptr == NULL){
		*list_ptr = calloc(n_new + 1, sizeof(int));
	} else {
		LMH_ASSERT(*list_ptr != NULL);
		*list_ptr  = realloc(*list_ptr, (n_new + 1) * sizeof(int));
		for(i = n_old; i < n_new; ++i){
			(*list_ptr)[i] = 0;
		}
	}
	(*list_ptr)[n_new] = -1;
}
void add_zero_to_score_list_at(int ** list_ptr, int n_old, int at){
	LMH_ASSERT(list_ptr != NULL && at <= n_old && at >= 0 && n_old >= 0);
	if(*list_ptr == NULL){
		*list_ptr = calloc(n_old + 2, sizeof(int));
		(*list_ptr)[n_old+1] = -1;
	} else {
		*list_ptr = realloc(*list_ptr, (n_old + 2)*sizeof(int));
		for(int i = n_old + 1; i > at; --i){
			(*list_ptr)[i] = (*list_ptr)[i-1];
		}
	}
}
void remove_from_int_list(int * list, int i_remove){
	int i;
	LMH_ASSERT(list != NULL && i_remove >= 0);
	for(i = i_remove; list[i] != -1; ++i){
		list[i] = list[i+1];
	}
}

void remove_from_ptr_list(void ** list, int i_remove){
	int i;
	LMH_ASSERT(list != NULL && i_remove >= 0);
	for(i = i_remove; list[i] != NULL; ++i){
		list[i] = list[i+1];
	}
}

int school_teacher_add(School * school, const Teacher * const t){
	int n_teaches = 0, pos = 0;
	LMH_ASSERT(school != NULL && t != NULL);
	if(school->teachers == NULL){
		school->teachers = calloc(2, sizeof(Teacher));
		school->n_teachers = 0;
	} else {
		for(pos = 0; pos < school->n_teachers && t->id > school->teachers[pos].id; ++pos){
			/* Blank. Teachers are ordered by id. */
		}
		bool success = realloc_teachers(school, school->n_teachers+1, pos);
		// **TODO** -- decent memory management.
		LMH_ASSERT(success);
	}
	school->teachers[ pos ] = *t;
	++school->n_teachers;
	for(int i = 0; i < school->n_meetings; ++i){
		if(school->meetings[i].teacher != NULL){
			int teacher_i = get_teacher_index_by_id(school, school->meetings[i].teacher->id);
			if(teacher_i >= pos){
				school->meetings[i].teacher = &(school->teachers[teacher_i + 1]);
			}
		}
	}
	for(int i = 0; i < school->n_assignments; ++i){
		/* TODO predefine as zero seems inconvenient for the user. */
		add_zero_to_score_list_at(&(school->assignments[i].possible_teachers), school->n_teachers-1, pos);
	}
	for(int i = 0; i < school->n_meetings; ++i){
		add_zero_to_score_list_at(&(school->meetings[i].possible_teachers), school->n_teachers-1, pos);
	}
	for(int i = 0; i < school->n_solutions; ++i){
		Meeting * meetings = school->solutions[i].meetings;
		if(meetings != NULL){
			for(int j = 0; j < school->solutions[i].n_meetings; ++i){
				if(meetings[j].teacher != NULL){
					int i_teacher = get_teacher_index_by_id(school, meetings[j].teacher->id);
					if(i_teacher >= pos){
						meetings[j].teacher = &(school->teachers[i_teacher + 1]);
					}
				}
			}
		}
	}
	if(t->teaches){
		for(n_teaches = 0; t->teaches[n_teaches] != NULL; ++n_teaches){
			/* Counting n_teaches */
		}
		for(int i = 0; i < n_teaches; ++i){
			int i_teaches = school_teaches_add(school, t->teaches[i], false);
			school->teachers[ pos ].teaches[i] = & school->teaches[i_teaches];
			school->teaches[i_teaches].teacher = & school->teachers[pos];
		}
	}
	return pos;
}
int school_teaches_add(School * school, Teaches * teaches, bool alter_teacher_teaches_list){
	LMH_ASSERT(school != NULL && teaches != NULL);
	int pos = 0;
	if(school->teaches == NULL){
		school->teaches = calloc(2, sizeof(Teaches));
		school->n_teaches = 0;
	} else {
		for(int pos = 0; pos < school->n_teaches && teaches->id > school->teaches[pos].id; ++pos){
			/* Blank */
		}
		realloc_teaches(school, school->n_teaches + 1, pos);
	}
	school->teaches[pos] = *teaches;

	Teacher * teacher = find_teacher_by_id(school, teaches->teacher->id);
	LMH_ASSERT(teacher != NULL);
	// TODO : Maybe?
	// LMH_ASSERT(teacher == school->teaches[pos].teacher);
	if(alter_teacher_teaches_list){
		int tt_ctr = 0;
		bool already_pointed = false;
		if(teacher->teaches == NULL){
			teacher->teaches = calloc(2, sizeof(Teaches*));
		} else {
			for(tt_ctr = 0; teacher->teaches[tt_ctr] != NULL; ++tt_ctr){
				if(teacher->teaches[tt_ctr] == teaches){
					already_pointed = true;
					teacher->teaches[tt_ctr] = & school->teaches[pos];
				}
			}
			if(!already_pointed){
				teacher->teaches = realloc(teacher->teaches, (tt_ctr + 2)* sizeof(Teaches*));
			}
		}
		if(!already_pointed) {
			// TODO : would it be necessary to order by id here?
			teacher->teaches[tt_ctr] = &(school->teaches[pos]);
		}
		teacher->teaches[tt_ctr+1] = NULL;
	}
	++school->n_teaches;
	return pos;
}
int school_class_add(School * school, Class * c){
	int pos = 0;
	LMH_ASSERT(school != NULL && c != NULL);
	if(school->classes == NULL || school->n_classes == 0){
		school->classes = calloc(2, sizeof(Class));
		school->n_classes = 0;
	} else {
		realloc_classes(school, school->n_classes + 1);
		for(pos = 0; pos < school->n_classes && c->id > school->classes[pos].id; ++pos){
			/* Blank */
		}
	}
	for(int i = school->n_classes; i > pos; --i){
		school->classes[i] = school->classes[i-1];
	}
	printf("adding a class %d with name %s \n", c->id, c->name);
	school->classes[ pos ] = *c;

	/* As the classes after pos moved, we need to 'repoint' every reference to any of them. */
	for(int i = 0; i < school->n_assignments; ++i){
		int i_class = get_class_index_by_id(school, school->assignments[i].m_class->id);
		if(i_class >= pos){
			/* This class has moved one index over */
			school->assignments[i].m_class = &(school->classes[i_class+1]);
		}
	}
	for(int i = 0; i < school->n_meetings; ++i){
		if(school->meetings[i].type == meet_LECTURE){
			int i_class = get_class_index_by_id(school, school->meetings[i].m_class->id);
			if(i_class >= pos){
				school->meetings[i].m_class = &(school->classes[i_class + 1]);
			}
		}
	}
	for(int i = 0; i < school->n_solutions; ++i){
		Meeting * meetings = school->solutions[i].meetings;
		if(meetings != NULL){
			for(int j = 0; j < school->solutions[i].n_meetings; ++i){
				if(meetings[j].type == meet_LECTURE){
					int i_class = get_class_index_by_id(school, meetings[j].m_class->id);
					if(i_class >= pos){
						meetings[j].m_class = &(school->classes[i_class + 1]);
					}
				}
			}
		}
	}
	if(c->assignments){
		int i = 0;
		for(i = 0; c->assignments[i] != NULL; ++i){
			c->assignments[i]->m_class = & school->classes[ pos ];
			school_assignment_add(school, c->assignments[i], false);
		}
	}
	LMH_TODO(); // call  create_meeting_list_for_class  and  school_meeting_list_add_and_bind;

	++school->n_classes;
	return pos;
}
int school_subject_add(School * school, const Subject * const subject){
	int pos = 0;
	LMH_ASSERT(school != NULL && subject != NULL);
	if(school->subjects == NULL || school->n_subjects == 0){
		school->subjects = calloc(2, sizeof(Subject));
		school->n_subjects = 0;
	} else {
		realloc_subjects(school,school->n_subjects + 1);
		// school->subjects = realloc(school->n_subjects + 2, sizeof(Subject));
		for(pos = 0; pos < school->n_subjects && school->subjects[pos].id < subject->id; ++pos){
			/* Blank */
		}
	}
	for(int i = school->n_subjects; i > pos; --i){
		school->subjects[i] = school->subjects[i-1];
	}
	school->subjects[ pos ] = *subject;

	for(int i = 0; i < school->n_assignments; ++i){
		int i_subject = get_subject_index_by_id(school, school->assignments[i].subject->id);
		if(i_subject >= pos){
			school->assignments[i].subject = &(school->subjects[i_subject + 1]);
		}
	}
	for(int i = 0; i < school->n_solutions; ++i){
		Meeting * meetings = school->solutions[i].meetings;
		if(meetings != NULL){
			for(int j = 0; j < school->solutions[i].n_meetings; ++i){
				if(meetings[j].type == meet_LECTURE){
					int i_subject = get_subject_index_by_id(school, meetings[j].subject->id);
					if(i_subject >= pos){
						meetings[j].subject = &(school->subjects[i_subject + 1]);
					}
				}
			}
		}
	}
	school->n_subjects++;
	return pos;
}
int school_room_add(School * school, const Room * const room){
	int i, pos = 0;
	LMH_ASSERT(school != NULL && room != NULL);
	if(school->n_rooms == 0){
		school->rooms = (Room*) calloc(2, sizeof(Room));
	} else {
		realloc_rooms(school, school->n_rooms + 1);
		// school->rooms = (Room *) realloc(school->rooms, (school->n_rooms + 2)*sizeof(Room));
		for(pos = 0; pos < school->n_rooms && school->rooms[pos].id < room->id; ++pos){
			/* Blank */
		}
	}

	for(i = school->n_rooms; i > pos; --i){
		school->rooms[i] = school->rooms[i-1];
	}
	school->rooms[pos] = *room;

	for(i = 0; i < school->n_teachers; ++i){
		add_zero_to_score_list_at(&school->teachers[i].lecture_room_scores, school->n_rooms, pos);
		add_zero_to_score_list_at(&school->teachers[i].planning_room_scores, school->n_rooms, pos);
	}
	for(i = 0; i < school->n_classes; ++i){
		add_zero_to_score_list_at(&school->classes[i].room_scores, school->n_rooms, pos);
	}
	for(i = 0; i < school->n_teaches; ++i){
		add_zero_to_score_list_at(&school->teaches[i].room_scores, school->n_rooms, pos);
	}
	for(i = 0; i < school->n_meetings; ++i){
		add_zero_to_score_list_at(&school->meetings[i].possible_rooms, school->n_rooms, pos);
		if(school->meetings[i].room != NULL){
			int i_room = get_room_index_by_id(school, school->meetings[i].room->id);
			if(i_room >= pos){
				school->meetings[i].room = &(school->rooms[i_room + 1]);
			}
		}
	}
	for(i = 0; i < school->n_solutions; ++i){
		Meeting * meetings = school->solutions[i].meetings;
		for(int j = 0; j < school->solutions[i].n_meetings; ++j){
			if(meetings[j].room != NULL){
				int i_room = get_room_index_by_id(school, meetings[j].room->id);
				if(i_room >= pos){
					meetings[j].room = &(school->rooms[j+1]);
				}
			}
		}
	}

	school->n_rooms++;
	return pos;
}
int school_subjectgroup_add(School * school, const char * const name, int id){
	/* TODO realloc class->in_groups */
	int pos = 0;
	LMH_ASSERT(school != NULL && name != NULL && id > 0);
	if(school->n_subject_groups == 0 || school->subject_group_names == NULL){
		school->subject_group_names = calloc(2, sizeof(char *));
		school->subject_group_ids = calloc(2, sizeof(int));
		school->n_subject_groups = 0;
	} else {
		school->subject_group_names = realloc(school->subject_group_names, (school->n_subject_groups + 2) * sizeof(char*));
		school->subject_group_ids = realloc(school->subject_group_ids, (school->n_subject_groups + 2) * sizeof(int*));
		for(pos = 0; pos < school->n_subject_groups && school->subject_group_ids[pos] < id; ++pos){
			/* Blank */
		}
	}
	for(int i = school->n_subject_groups; i > pos; --i){
		school->subject_group_names[i] = school->subject_group_names[i-1];
		school->subject_group_ids[i] = school->subject_group_ids[i-1];
	}
	school->subject_group_names[pos] = name;
	school->subject_group_ids[pos] = id;

	for(int i = 0; i < school->n_subjects; ++i){
		add_zero_to_score_list_at(&(school->subjects[i].in_groups), school->n_subject_groups, pos);
	}
	for(int i = 0; i < school->n_classes; ++i){
		add_zero_to_score_list_at(&(school->classes[i].max_per_day_subject_group), school->n_subject_groups, pos);
	}
	++school->n_subject_groups;
	return pos;
}
void school_solution_add(School * school, const Solution * const sol){
	LMH_ASSERT(school != NULL && sol != NULL);
	int i, i_insert = 0;
	if(school->solutions == NULL || school->n_solutions == 0){
		school->solutions = (Solution*) calloc(2, sizeof(Solution));
		school->n_solutions = 0;
	} else {
		school->solutions = (Solution*) realloc(school->solutions, (school->n_solutions + 1) * sizeof(Solution));
		for(i = 0; school->solutions[i].id < sol->id && i < school->n_solutions; ++i){
			/* Blank */
		}
		i_insert = i;
	}
	for(i = school->n_solutions; i > i_insert; --i){
		school->solutions[i] = school->solutions[i-1];
	}
	school->solutions[i_insert] = *sol;
	++ school->n_solutions;
}

void school_subjectgroup_remove(School * school, int i_sg, bool must_delete){
	LMH_ASSERT(school != NULL && i_sg >= 0 && i_sg <= school->n_subject_groups);
	if(must_delete){
		free(school->subject_group_names[i_sg]);
	}
	for(int i = i_sg; i < school->n_subject_groups; ++i){
		school->subject_group_names[i] = school->subject_group_names[i+1];
		school->subject_group_ids[i] = school->subject_group_ids[i+1];
	}
	for(int i = 0; i < school->n_classes; ++i){
		int * mpd = school->classes[i].max_per_day_subject_group;
		if(mpd != NULL){
			for(int j = i_sg+1; j < school->n_subjects; ++j){
				mpd[j-1] = mpd[j];
			}
		}
	}
	for(int i = 0; i < school->n_subjects; ++i){
		int * ing = school->subjects[i].in_groups;
		if(ing != NULL){
			for(int j = i_sg+1; j < school->n_subject_groups; ++j){
				ing[j-1] = ing[j];
			}
		}
	}
	--school->n_subject_groups;
}

void school_subject_remove(School * school, int subj_i, bool must_delete){
	int i, j, k;
	int removed_subj_id = school->subjects[subj_i].id;
	LMH_ASSERT(school != NULL && subj_i >= 0);
	for(i = 0; i < school->n_teaches; ++i){
		/* Push all behind to the front if this is about the deleted. */
		if(school->teaches[i].subject->id == removed_subj_id){
			if(must_delete){
				free_teaches(&school->teaches[i]);
			}
			for(j = i; j < school->n_teaches; ++j){
				school->teaches[j] = school->teaches[j+1];
			}
			--school->n_teaches;
			--i;
		}
	}
	for(i = 0; i < school->n_assignments; ++i){
		if(school->assignments[i].subject->id == removed_subj_id){
			school_assignment_remove(school, i, must_delete);
			--i;
		}
	}
	if(school->meetings != NULL){
		for(i = 0; i < school->n_meetings; ++i){
			if(school->meetings[i].subject->id == removed_subj_id){
				free_meeting(&school->meetings[i]);
				for(j = i; j < school->n_meetings; ++j){
					school->meetings[j] = school->meetings[j+1];
				}
				--i;
				school->n_meetings--;
			}
		}
	}
	/* This should never be necessary, because of the can_remove_subject */
	if(school->solutions != NULL){
		for(i = 0; i < school->n_solutions; ++i){
			Meeting * m_list = school->solutions[i].meetings;
			if(m_list != NULL){
				for(j = 0; m_list[j].type != meet_NULL; ++j){
					if(m_list[j].type == meet_LECTURE && m_list[j].subject->id == removed_subj_id){
						free_meeting(& m_list[j]);
						for(k = j; m_list[k].type != meet_NULL; ++k){
							m_list[k] = m_list[k+1];
						}
						--j;
					}
				}
			}
		}
	}
	if(must_delete){
		free_subject(&school->subjects[subj_i]);
	}
	for(i = subj_i; i < school->n_subjects; ++i){
		school->subjects[i] = school->subjects[i + 1];
	}
	--school->n_subjects;
}
void school_teacher_remove(School * school, int i_remove, bool must_delete){
	int i,j;
	LMH_ASSERT(school != NULL && i_remove < school->n_teachers && i_remove >= 0);
	/* TODO Check for subordinates too. */
	for(i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].teacher->id == school->teachers[i_remove].id){
			school_teaches_remove(school, i, must_delete);
			--i;
		}
	}
	if(school->solutions != NULL){
		for(i = 0; i < school->n_solutions; ++i){
			Meeting * m_list = school->solutions[i].meetings;
			if(m_list != NULL){
				for(j = 0; m_list[j].m_class != NULL; ++j ){
					if(m_list[j].teacher->id == school->teachers[i_remove].id){
						m_list[j].teacher = NULL;
					}
				}
			}
		}
	}
	if(must_delete){
		free_teacher(&school->teachers[i_remove]);
	}
	for(i = i_remove; i < school->n_teachers-1; ++i){
		school->teachers[i] = school->teachers[i+1];
	}
	--school->n_teachers;
}
void school_teaches_remove(School * school, int i_remove, bool must_delete){
	LMH_ASSERT(school != NULL && i_remove >= 0 && i_remove < school->n_teaches);
	if(must_delete){
		free_teaches(&school->teaches[i_remove]);
	}

	int n_tt, i_teacher_remove = -1;
	Teacher * t = school->teaches[i_remove].teacher;
	LMH_ASSERT(find_teacher_by_id(school, t->id) == t);
	for(n_tt = 0; t->teaches[n_tt] != NULL; ++n_tt){
		if(i_teacher_remove == -1 && t->teaches[n_tt]->id == school->teaches[i_remove].id){
			i_teacher_remove = n_tt;
		}
	}
	LMH_ASSERT(i_teacher_remove >= 0);
	for(int i = i_teacher_remove; i < n_tt; ++i){
		t->teaches[i] = t->teaches[i+1];
	} /* The last one will receive NULL */
	--n_tt;

	for(int i = 0; i < school->n_teachers; ++i){
		for(int j = 0; school->teachers[i].teaches[j] != NULL; ++j){
			int i_teaches = get_teaches_index_by_id(school, school->teachers[i].teaches[j]->id);
			LMH_ASSERT(i_teaches != -1);
			LMH_ASSERT(i_teaches != i_remove);
			if(i_teaches > i_remove){
				school->teachers[i].teaches[j] = & school->teaches[i_teaches-1];
			}
		}
	}
	for(int i = i_remove; i < school->n_teaches-1; ++i){
		school->teaches[i] = school->teaches[i+1];
	}
	--school->n_teaches;

}
void school_class_remove(School * school, int class_i, bool must_delete){
	int i,j;
	Meeting * m_list;
	/* TODO: Check for subordinates too. */
	LMH_ASSERT(school != NULL && class_i < school->n_classes && class_i >= 0);
	Class * class = &school->classes[class_i];

	for(int i = 0; i < school->n_assignments; ++i){
		if(school->assignments[i].m_class->id == class->id){
			school_assignment_remove(school, i, must_delete);
			--i;
		}
	}
	for(i = 0; i < school->n_solutions; ++i){
		m_list =  school->solutions[i].meetings;
		if(m_list != NULL){
			for(j = 0; m_list[j].type != meet_NULL; ++j){
				if(m_list[j].type == meet_LECTURE && m_list[j].m_class->id == class->id){
					if(must_delete){
						/* On paper, this won't do anything, since solution.meeetings
						 * should not have possible_$x values */
						free_meeting(&m_list[j]);
					}
					for(int k = j; m_list[k].m_class != NULL; ++k){
						m_list[k] = m_list[k + 1];
					}
				}
			}
		}
	}
	if(must_delete){
		free_class(class);
	}
	for(i = class_i; i < school->n_classes-1; ++i){
		school->classes[i] = school->classes[i+1];
	}
	--school->n_classes;
}
void school_room_remove(School * school, int room_i, bool must_delete){
	int i,j;
	LMH_ASSERT(school != NULL && room_i < school->n_rooms && room_i >= 0);
	if(school->solutions != NULL){
		for(i = 0; i < school->n_solutions; ++i){
			Meeting * m_list = school->solutions[i].meetings;
			if(m_list != NULL){
				for(j = 0; m_list[j].type != meet_NULL; ++j){
					if(m_list[j].room->id == school->rooms[room_i].id){
						m_list[j].room = NULL;
					}
				}
			}
		}
	}
	for(i = room_i; i < school->n_rooms; ++i){
		school->rooms[i] = school->rooms[i+ 1];
	}

	for(i = 0; i < school->n_teachers; ++i){
		if(school->teachers[i].lecture_room_scores){
			remove_from_int_list(school->teachers[i].lecture_room_scores, room_i);
		}
		if(school->teachers[i].planning_room_scores){
			remove_from_int_list(school->teachers[i].planning_room_scores, room_i);
		}
	}
	for(i = 0; i < school->n_classes; ++i){
		if(school->classes[i].room_scores){
			remove_from_int_list(school->classes[i].room_scores, room_i);
		}
	}
	for(i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].room_scores){
			remove_from_int_list(school->teaches[i].room_scores, room_i);
		}
	}
	for(i = 0; i < school->n_meetings; ++i){
		// if(school->meetings[i].room != NULL){
		// 	int referenced_i = get_room_index_by_id(school,school->meetings[i].room->id);
		// 	if(referenced_i == room_i){
		// 		// TODO How can we retrieve this in the UNDO?
		// 		referenced_i = NULL;
		// 	}
		// }

		if(school->meetings[i].possible_rooms){
			remove_from_int_list(school->meetings[i].possible_rooms, room_i);
		}
	}
	--(school->n_rooms);
}
void school_solution_remove(School * school, int i_solution, bool must_delete){
	LMH_ASSERT(school != NULL && i_solution >= 0 && i_solution < school->n_solutions);
	if(must_delete){
		free_solution(&school->solutions[i_solution]);
	}
	for(int i = i_solution; i < school->n_solutions; ++i){
		school->solutions[i] = school->solutions[i+1];
	}
	--school->n_solutions;
}

void school_assignment_add(School * school, Assignment * assignment, bool add_to_class_assignments){
	LMH_ASSERT(school != NULL && assignment != NULL && assignment->id > 0);

	if(school->assignments == NULL){
		school->assignments = calloc(2, sizeof(Assignment));
	} else {
		realloc_assignments(school, school->n_assignments + 1);
	}

	int pos = 0;
	for(int i = 0; i < school->n_assignments; ++i){
		if(school->assignments[i].id > assignment->id){
			pos = i;
			break;
		}
	}
	for(int i = 0; i < school->n_classes; ++i){
		Assignment ** c_assignments = school->classes[i].assignments;
		if(c_assignments != NULL){
			for(int j = 0; c_assignments[j] != NULL; ++j){
				int i_assignment = get_assignment_index_by_id(school, c_assignments[j]->id);
				LMH_ASSERT(i_assignment >= 0);
				if(i_assignment >= pos){
					c_assignments[j] = & school->assignments[i_assignment+1];
				}
			}
		}
	}
	for(int i = pos; i < school->n_assignments; ++i){
		school->assignments[i+1] = school->assignments[i];
	}

	school->assignments[pos] = *assignment;

	if(add_to_class_assignments){
		Class * c = assignment->m_class;
		int sz_c_assignments = 0;
		for(sz_c_assignments = 0; c->assignments[sz_c_assignments] != NULL; ++sz_c_assignments){ }
		c->assignments = realloc(c->assignments, (sz_c_assignments + 2) * sizeof(Assignment*));
		c->assignments[sz_c_assignments] = &school->assignments[pos];
		c->assignments[sz_c_assignments+1] = NULL;
	}

	++ school->n_assignments;
}
void school_assignment_remove(School * school, int i_assignment, bool must_delete){
	LMH_ASSERT(school != NULL && i_assignment >= 0 && i_assignment < school->n_assignments);
	/* Repoints all class references to all assignments past i_assignment */
	for(int i = 0; i < school->n_classes; ++i){
		Assignment ** c_assignments = school->classes[i].assignments;
		for(int j = 0; c_assignments[j] != NULL; ++j){
			int this_i = get_assignment_index_by_id(school, c_assignments[j]->id);
			if(this_i == i_assignment){
				/* Needs to be deleted */
				remove_from_ptr_list(c_assignments,j);
				--j;
			} else if(this_i > i_assignment){
				/* Transposed backwards */
				c_assignments[j] = &(school->assignments[this_i - 1]);
			}
		}
	}
	if(must_delete){
		free_assignment(& school->assignments[i_assignment]);
	}
	/* Removes from that position */
	for(int i = i_assignment; i < school->n_assignments-1; ++i){
		school->assignments[i] = school->assignments[i+1];
	}

	--school->n_assignments;
}

/*********************************************************/
/*                     OTHER Functions                   */
/*********************************************************/

Meeting * create_meeting_list_for_class(School * school, Class * c){
	int i,j, n_per_total = 0, i_meeting = 0;
	LMH_ASSERT(school != NULL && c != NULL);
	Meeting * meetings = NULL;
	if(c->assignments != NULL){
		for(i = 0; c->assignments[i] != NULL; ++i){
			n_per_total += c->assignments[i]->amount;
		}
		if( n_per_total != 0 ){
			meetings = calloc( n_per_total+1, sizeof(Meeting));
			for(i = 0; c->assignments[i] != NULL; ++i){
				for(j = 0; j < c->assignments[i]->amount; ++j){
					meetings[i_meeting] = (Meeting){
						.id = 0,
						.type = meet_LECTURE,
						.m_class = c,
						.subject = c->assignments[i]->subject,
						.teacher = NULL,
						.room = NULL,
						.period = 0,
						.possible_periods = NULL,
						.possible_rooms = NULL,
						.possible_teachers = NULL
					};
					++i_meeting;
				}
			}
			meetings[n_per_total].type = meet_NULL;
		}
	}
	return meetings;
}

void school_meeting_list_add_and_bind(School * school, int class_i, Meeting * meetings){
	int i_meet = 0, n = 0;
	LMH_ASSERT(school != NULL && meetings != NULL && class_i >= 0 && class_i < school->n_classes);

	for(n = 0; meetings[n].type == meet_LECTURE; ++n){
		/* Blank */
	}
	LMH_ASSERT(n > 0);

	if(school->n_meetings == 0){
		school->meetings = calloc(n + 1, sizeof(Meeting));
	} else {
		school->meetings = realloc(school->meetings, (school->n_meetings + n + 1)*sizeof(Meeting));
	}

	for(i_meet = 0; i_meet < n; ++i_meet){
		school->meetings[school->n_meetings] = (Meeting){
			.id = meetings[i_meet].id,
			.type = meet_LECTURE,
			.m_class = &school->classes[class_i],
			.subject = meetings[i_meet].subject,
			.teacher = meetings[i_meet].teacher,
			.room = meetings[i_meet].room,
			.period = meetings[i_meet].period,
			.possible_periods = meetings[i_meet].possible_periods,
			.possible_rooms = meetings[i_meet].possible_rooms,
			.possible_teachers = meetings[i_meet].possible_teachers,
		};
		++school->n_meetings;
	}
}
