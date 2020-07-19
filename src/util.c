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
			fprintf(out, "; S(%s)", meetings[i].subj->name);
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


void free_meetings_list(Meeting * list){
	int i_met;
	if(list != NULL){
		for(i_met = 0; list[i_met].m_class != NULL; ++i_met){
			free(list[i_met].possible_teachers);
			free(list[i_met].possible_periods);
			free(list[i_met].possible_rooms);
		}
	}
	free(list);
}

void free_node(DecisionNode * node){
	int i = 0;

	for(i = 0; i < node->n_children; ++i){
		free_node(&node->children[i]);
	}
	// if(node->children != NULL){
		free(node->children);
		free(node->children_score);
		free(node->children_score_order);
		free_meetings_list(node->conclusion);
	// }/
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
		fprintf(out,"\n\tperiods: ");
		print_int_list(out, t->periods);
		fprintf(out,"\n\trooms: ");
		print_int_list(out, t->rooms);
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
		fprintf(out, "\tn_features: 		 %d\n", s->n_features);
		fprintf(out, "\tn_classes: 		 %d\n", s->n_classes);
		fprintf(out, "\tn_teachers: 	 	 %d\n", s->n_teachers);
		fprintf(out, "\tn_subjects: 	 	 %d\n", s->n_subjects);
		fprintf(out, "\tn_rooms:	 	 	 %d\n", s->n_rooms);
		fprintf(out, "\tn_teaches: 	 	 %d\n", s->n_teaches);
		fprintf(out, "\tn_subj_groups: 	 %d\n", s->n_subject_groups);

		// printf("\t\t")
	}
}

Room * find_room_by_id(School * school, int id){
	int i;

	for(i = 0; i < school->n_rooms; ++i){
		if(school->rooms[i].id == id){
			return &(school->rooms[i]);
		}
	}
	return NULL;
}

Class * find_class_by_id(School * school, int id) {
	int i;

	for(i = 0; i < school->n_classes; ++i){
		if(school->classes[i].id == id){
			return &(school->classes[i]);
		}
	}
	return NULL;
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

int get_day_index_by_id(School * school, int id){
	int i;
	for(i = 0; i < school->n_days; ++i){
		if(school->day_ids[i] == id){
			return i;
		}
	}
	return -1;
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


void free_school(School * s){
	int i;
	if(s != NULL){
		if(s->teachers != NULL){
			for(i = 0; i < s->n_teachers; ++i){
				free(s->teachers[i].periods);
			}
			free(s->teachers);
		}
		if(s->feature_names != NULL){
			for(i = 0; i < s->n_features; ++i){
				free(s->feature_names[i]);
			}
			free(s->feature_names);
			free(s->feature_ids);
		}
		if(s->classes != NULL){
			for(i = 0; i < s->n_classes; ++i){
				free(s->classes[i].needs);
			}
			free(s->classes);
		}

		free(s);
	}
}

School * copy_school(const School * const s){
	int i = 0, j=0, index;
	School * copy = calloc(1, sizeof(School));

	copy->id = s->id;
	copy->name = copy_string(s->name);
	copy->n_periods = s->n_periods;
	copy->n_features = s->n_features;
	copy->n_classes = s->n_classes;
	copy->n_teachers = s->n_teachers;
	copy->n_subjects = s->n_subjects;
	copy->n_rooms = s->n_rooms;
	copy->n_days = s->n_days;
	copy->n_periods_per_day = s->n_periods_per_day;
	copy->n_solutions = s->n_solutions;
	copy->max_meetings_teacher_per_week = s->max_meetings_teacher_per_week;
	copy->max_meetings_teacher_per_day = s->max_meetings_teacher_per_day;
	copy->max_gemini_classes = s->max_gemini_classes;

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
	if(copy->n_features > 0){
		copy->feature_names = calloc(copy->n_features, sizeof(char*));
		copy->feature_ids = calloc(copy->n_features, sizeof(int));
		for(i = 0; i < copy->n_features; ++i){
			copy->feature_names[i] = copy_string(s->feature_names[i]);
			copy->feature_ids[i] = s->feature_ids[i];
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

			for(j = 0; j < copy->n_features; ++j){
				copy->rooms[i].room_features[j] = s->rooms[i].room_features[j];
			}
			for(j = 0; j < copy->n_periods; ++j){
				copy->rooms[i].disponibility[j] = s->rooms[i].disponibility[j];
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

	if(copy->n_classes > 0){
		copy->classes = calloc(copy->n_classes, sizeof(Class));
		for(i = 0; i < copy->n_classes; ++i){
			int n_needs;

			copy->classes[i].id = s->classes[i].id;
			copy->classes[i].name = copy_string(s->classes[i].name);
			copy->classes[i].short_name = copy_string(s->classes[i].short_name);
			copy->classes[i].size = s->classes[i].size;
			for(j = 0; j < copy->n_periods; ++j){
				copy->classes[i].periods[j] = s->classes[i].periods[j];
			}
			copy->classes[i].can_have_free_periods_flag = s->classes[i].can_have_free_periods_flag;
			copy->classes[i].maximal_entry_period = s->classes[i].maximal_entry_period;
			copy->classes[i].minimal_exit_period = s->classes[i].minimal_exit_period;
			copy->classes[i].abstract = s->classes[i].abstract;
			copy->classes[i].subordinates = int_list_n_copy(s->classes[i].subordinates,copy->n_classes);

			if(s->classes[i].needs != NULL){
				for(n_needs = 0; s->classes[i].needs[n_needs].subject != NULL; ++n_needs){
					/* Blank on purpouse */
				}
				copy->classes[i].needs = calloc(n_needs, sizeof(SubjectQuantity));
				for(j = 0; j < n_needs; ++j){
					index = s->classes[i].needs[j].subject - s->subjects;
					if(index >= 0 && index < s->n_subjects){
						copy->classes[i].needs[j].quantity = s->classes[i].needs[j].quantity;
						copy->classes[i].needs[j].subject = &copy->subjects[index];
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
