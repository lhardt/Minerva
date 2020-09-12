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
	for(n = 0; list[n].m_class != NULL; ++n){ }

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
	int i;

	for(i = 0; i < school->n_classes; ++i){
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
	int i;

	for(i = 0; i < school->n_subjects; ++i){
		if(school->subjects[i].id == id){
			return &(school->subjects[i]);
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
		if(school->assignments[i].m_class->id == id_class && school->assignments[i].subject->id == id_subj){
			return &school->assignments[i];
		}
	}
	printf("Did not find assignment with idcl %d and idsubj %d\n", id_class, id_subj);
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


/*********************************************************/
/*                 ADD AND REMOVE Functions              */
/*********************************************************/

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
	LMH_ASSERT(list_ptr != NULL && at <= n_old && at > 0 && n_old >= 0);
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
void school_subject_remove(School * school, int subj_i, bool must_delete){
	int i, j, k;
	LMH_ASSERT(school != NULL && subj_i >= 0);
	for(i = 0; i < school->n_teaches; ++i){
		/* Push all behind to the front if this is about the deleted. */
		if(school->teaches[i].subject->id == school->subjects[subj_i].id){
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
	if(school->meetings != NULL){
		for(i = 0; i < school->n_meetings; ++i){
			if(school->meetings[i].subject->id == school->subjects[subj_i].id){
				free_meeting(&school->meetings[i]);
				for(j = i; j < school->n_meetings; ++j){
					school->meetings[j] = school->meetings[j+1];
				}
				--i;
				school->n_meetings--;
			}
		}
	}
	if(school->solutions != NULL){
		for(i = 0; i < school->n_solutions; ++i){
			Meeting * m_list = school->solutions[i].meetings;
			if(m_list != NULL){
				for(j = 0; m_list[j].type != meet_NULL; ++j){
					if(m_list[j].type == meet_LECTURE && m_list[j].subject->id == school->subjects[subj_i].id){
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
int school_class_assignments_add(School * school, Class * c){
	int i, n;
	LMH_ASSERT(school != NULL && c != NULL && c->assignments != NULL);

	for(n = 0; c->assignments[n] != NULL; ++n){
		/* Blank */
	}
	if(school->n_assignments == 0){
		school->assignments = calloc(n + 1, sizeof(Assignment));
	} else {
		school->assignments = realloc(school->assignments, (school->n_assignments + n+1)*sizeof(Assignment));
	}

	for(i = 0; i < n; ++i){
		school->assignments[school->n_assignments + i] = * c->assignments[i];
		c->assignments[i] = &school->assignments[school->n_assignments + i];
	}

	school->n_assignments += n;
	return school->n_assignments;
}
void school_class_add(School * school, Class * c, int pos){
	LMH_ASSERT(school != NULL && c != NULL && pos >= 0 && pos <= school->n_classes);
	if(school->classes == NULL || school->n_classes == 0){
		school->classes = calloc(2, sizeof(Class));
		school->n_classes = 0;
	} else {
		school->classes = realloc(school->classes, (school->n_classes + 2) * sizeof(Class));
	}
	for(int i = school->n_classes; i > pos; --i){
		school->classes[i] = school->classes[i-1];
	}
	school->classes[ pos ] = *c;
	school->classes[pos+1] = (Class) {
		.id=0,
		.name=NULL,
		.short_name=NULL
	};
	if(c->assignments){
		school_class_assignments_add(school, c);
	}
	++school->n_classes;
}
void school_teacher_add(School * school, const Teacher * const t, int pos){
	int n_teaches = 0, i_teaches = 0;
	LMH_ASSERT(school != NULL && t != NULL && pos >= 0 && pos <= school->n_teachers);
	if(school->teachers == NULL || school->n_teachers == 0){
		school->teachers = calloc(2, sizeof(Teacher));
		school->n_teachers = 0;
	} else {
		school->teachers = realloc(school->teachers, (school->n_teachers + 2)*sizeof(Teacher));
	}
	for(int i = school->n_teachers; i > pos; --i){
		school->teachers[i] = school->teachers[i-1];
	}
	school->teachers[ pos ] = *t;

	if(t->teaches){
		for(n_teaches = 0; t->teaches[n_teaches] != NULL; ++n_teaches){
			/* Blank on purpouse */
		}
		/* Teacheses are ordered by the id of the teacher. */
		int i_start = 0;
		if(school->teaches == NULL || school->n_teaches == 0){
			school->teaches = calloc(n_teaches + 1, sizeof(Teaches));
			school->n_teaches = 0;
		} else if(school->n_teaches %10 == 0){
			school->teaches = realloc(school->teaches, (school->n_teaches + n_teaches +1) * sizeof(Teaches));
			for(i_start = 0; i_start < school->n_teaches && school->teaches[i_start].teacher->id < t->id; ++i_start){
				/* Stops where we need to insert our list of teaches. */
			}
		}
		for(i_teaches = 0; i_teaches < n_teaches; ++i_teaches){
			school->teaches[i_start + i_teaches] = * t->teaches[i_teaches];
			t->teaches[i_teaches] = &school->teaches[i_start + i_teaches];
			++ school->n_teaches;
		}
	}
	for(int i = 0; i < school->n_assignments; ++i){
		/* TODO predefine as zero seems inconvenient for the user. */
		add_zero_to_score_list_at(&(school->assignments[i].possible_teachers), school->n_teachers, pos);
	}

	++school->n_teachers;
}
void school_teacher_remove(School * school, int i_remove, bool must_delete){
	int i,j;
	/* TODO Check for subordinates too. */
	for(i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].teacher->id == school->teachers[i_remove].id){
			if(must_delete){
				/* Else it will be still referenced by the teacher */
				free_teaches(&school->teaches[i]);
			}
			for(j = i; j < school->n_teaches; ++j){
				school->teaches[j] = school->teaches[j+1];
			}
			--i;
			--school->n_teaches;
		}
	}
	if(school->solutions != NULL){
		for(i = 0; i < school->n_solutions; ++i){
			Meeting * m_list = school->solutions[i].meetings;
			for(j = 0; m_list[j].m_class != NULL; ++j ){
				if(m_list[j].teacher->id == school->teachers[i_remove].id){
					m_list[j].teacher = NULL;
				}
			}
		}
	}
	if(must_delete){
		free_teacher(&school->teachers[i_remove]);
	}
	for(i = i_remove; i < school->n_teachers; ++i){
		school->teachers[i] = school->teachers[i+1];
	}

	--school->n_teachers;
}
void school_class_remove(School * school, int class_i, bool must_delete){
	int i,j;
	/* TODO: Check for subordinates too. */
	Meeting * m_list;
	Class * class = &school->classes[class_i];
	for(i = 0; i < school->n_solutions; ++i){
		m_list=  school->solutions[i].meetings;
		for(j = 0; m_list[j].type != meet_NULL; ++j){
			if(m_list[j].type == meet_LECTURE && m_list[i].m_class->id == class->id){
				if(must_delete){

				}
				for(j = i; m_list[j].m_class != NULL; ++j){
					m_list[j] = m_list[j+1];
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
}
void school_room_add(School * school, const Room * const room){
	int i;
	if(school->n_rooms == 0){
		school->rooms = (Room*) calloc(11, sizeof(Room));
	} else if(school->n_rooms % 10 == 0) {
		school->rooms = (Room *) realloc(school->rooms, (school->n_rooms + 11 - (school->n_rooms % 10))*sizeof(Room));
	}
	school->rooms[school->n_rooms] = *room;

	for(i = 0; i < school->n_teachers; ++i){
		add_zeroes_to_score_list(&school->teachers[i].lecture_room_scores, school->n_rooms, school->n_rooms + 1);
		add_zeroes_to_score_list(&school->teachers[i].planning_room_scores, school->n_rooms, school->n_rooms + 1);
	}
	for(i = 0; i < school->n_classes; ++i){
		add_zeroes_to_score_list(&school->classes[i].room_scores, school->n_rooms, school->n_rooms + 1);
	}
	for(i = 0; i < school->n_teaches; ++i){
		add_zeroes_to_score_list(&school->teaches[i].room_scores, school->n_rooms, school->n_rooms + 1);
	}
	for(i = 0; i < school->n_meetings; ++i){
		add_zeroes_to_score_list(&school->meetings[i].possible_rooms, school->n_rooms, school->n_rooms + 1);
	}
	school->n_rooms++;
}
void school_room_remove(School * school, int room_i, bool must_delete){
	int i,j;
	LMH_ASSERT(school != NULL && room_i < school->n_rooms);
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
		if(school->meetings[i].possible_rooms){
			remove_from_int_list(school->meetings[i].possible_rooms, room_i);
		}
	}
	--(school->n_rooms);
}
void school_subject_add(School * school, const Subject * const subject){
	if(school->subjects == NULL || school->n_subjects == 0){
		school->subjects = (Subject*)calloc(10, sizeof(Subject));
		school->n_subjects = 0;
	} else if(school->n_subjects % 10 == 0) {
		school->subjects = (Subject*)realloc(school->subjects,(school->n_subjects +11)*sizeof(Subject));
	}
	school->subjects[ school->n_subjects ] = *subject;
	school->n_subjects++;
}
void school_subjectgroup_add(School * school, const char * const name, int id){
	/* TODO realloc class->in_groups */
	if(school->n_subject_groups == 0 || school->subject_group_names == NULL){
		school->subject_group_names = (char **) calloc(11, sizeof(char *));
		school->subject_group_ids = (int *) calloc(11, sizeof(int));
		school->n_subject_groups = 0;
	} else if(school->n_subject_groups % 10 == 0) {
		school->subject_group_names = (char **) realloc(school->subject_group_names, (school->n_subject_groups + 11) * sizeof(char*));
		school->subject_group_ids = (int *) realloc(school->subject_group_ids, (school->n_subject_groups + 11) * sizeof(int*));
	}
	school->subject_group_names[school->n_subject_groups] = name;
	school->subject_group_ids[school->n_subject_groups] = id;
	++school->n_subject_groups;
}

/*********************************************************/
/*                     OTHER Functions                   */
/*********************************************************/

Meeting * create_meeting_list_for_class(School * school, Class * c){
	int i,j, n_per_total = 0;
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
					meetings[j] = (Meeting){
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
					++school->n_meetings;
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
