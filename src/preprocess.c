/*
 * Project Minerva.
 *   preprocess.h - timetable preprocessing.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains functions to foresee
 * and change how the timetable will be made.
 */
#include "preprocess.h"

#include <stdlib.h>
#include <string.h>

#include "assert.h"

void school_subject_remove(School * school, int subj_i){
	int i, j, k;
	LMH_ASSERT(school != NULL && subj_i >= 0);
	for(i = 0; i < school->n_teaches; ++i){
		/* Push all behind to the front if this is about the deleted. */
		if(school->teaches[i].subject->id == school->subjects[subj_i].id){
			for(j = i; j < school->n_teaches; ++j){
				school->teaches[j] = school->teaches[j+1];
			}
			--school->n_teaches;
			--i;
		}
	}

	if(school->solutions != NULL){
		for(i = 0; i < school->n_solutions; ++i){
			Meeting * m_list = school->solutions[i].meetings;
			for(j = 0; m_list[j].m_class != NULL; ++j){
				Meeting * meet = & m_list[j];
				if(meet->subject->id == school->subjects[subj_i].id){
					if(meet->possible_periods != NULL){
						free(meet->possible_periods);
					}
					if(meet->possible_rooms != NULL){
						free(meet->possible_rooms);
					}
					if(meet->possible_teachers != NULL){
						free(meet->possible_teachers);
					}


					for(k = j; m_list[k].m_class != NULL; ++k){
						m_list[k] = m_list[k+1];
					}
					--j;
				}
			}
		}
	}

	for(i = subj_i; i < school->n_subjects; ++i){
		school->subjects[i] = school->subjects[i + 1];
	}
	--school->n_subjects;
}

Meeting * create_meeting_list_for_class(School * school, Class * c){
	int i,j,n;
	LMH_ASSERT(school != NULL && c != NULL);
	Meeting * meetings = NULL;
	if(c->assignments != NULL){
		for(n = 0; c->assignments[n] != NULL; ++n){
			/* Blank on purpouse */
		}
		if( n != 0 ){
			meetings = calloc( n+1, sizeof(Meeting));
			for(i = 0; i < n; ++i){
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
			meetings[n].type = meet_NULL;
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

int school_class_assignments_add(School * school, Class * c){
	int i, n;
	LMH_ASSERT(school != NULL && c != NULL && c->assignments != NULL);

	for(n = 0; c->assignments[n] != NULL; ++n){
		/* Blank */
	}
	printf("School nassignments was %d\n", school->n_assignments);
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

int school_class_add(School * school, const Class * const c){
	if(school->classes == NULL || school->n_classes == 0){
		school->classes = calloc(11, sizeof(Class));
	} else if(school->n_classes % 10 == 0) {
		school->classes = realloc(school->classes,(school->n_classes + 11) * sizeof(Class));
	}
	school->classes[ school->n_classes ] = *c;
	if(c->assignments){
		school_class_assignments_add(school, c);
	}
	return school->n_classes++;
}

void school_teacher_add(School * school, const Teacher * const t){
	int n_teaches = 0, i_teaches = 0;

	if(school->teachers == NULL || school->n_teachers == 0){
		school->teachers = calloc(11, sizeof(Teacher));
		school->n_teachers = 0;
	} else if(school->n_teachers % 10 == 0){
		school->teachers = realloc(school->teachers, (school->n_teachers + 11)*sizeof(Teacher));
	}
	school->teachers[ school->n_teachers ] = *t;

	if(t->teaches){
		for(n_teaches = 0; t->teaches[n_teaches] != NULL; ++n_teaches){
			/* Blank on purpouse */
		}
		/* Correcting teacher addresses and mallocs. */
		if(school->teaches == NULL || school->n_teaches == 0){
			school->teaches = calloc(11 + (n_teaches - n_teaches % 10), sizeof(Teaches));
			school->n_teaches = 0;
		} else if(school->n_teaches %10 == 0){
			school->teaches = realloc(school->teaches, (11 + (school->n_teaches + n_teaches - (school->n_teaches + n_teaches) % 10)) * sizeof(Teaches));
		}

		for(i_teaches = 0; i_teaches < n_teaches; ++i_teaches){
			school->teaches[school->n_teaches] = * t->teaches[i_teaches];
			t->teaches[i_teaches] = &school->teaches[school->n_teaches];
			++ school->n_teaches;
		}
	}

	++school->n_teachers;
}

void school_teacher_remove(School * school, int i_remove){
	int i,j;
	/* TODO Check for subordinates too. */
	for(i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].teacher->id == school->teachers[i_remove].id){
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
	for(i = i_remove; i < school->n_teachers; ++i){
		school->teachers[i] = school->teachers[i+1];
	}
	--school->n_teachers;
}

void school_class_remove(School * school, int class_i){
	int i,j;
	/* TODO: Check for subordinates too. */
	Meeting * m_list;
	Class * class = &school->classes[class_i];
	for(i = 0; i < school->n_solutions; ++i){
		m_list=  school->solutions[i].meetings;
		for(j = 0; m_list[j].m_class != NULL; ++j){
			if(m_list[i].m_class->id == class->id){
				for(j = i; m_list[j].m_class != NULL; ++j){
					m_list[j] = m_list[j+1];
				}
			}
		}
	}

	if(class->name != NULL){
		free(class->name);
	}
	if(class->short_name != NULL){
		free(class->short_name);
	}
	if(class->room_scores != NULL){
		free(class->room_scores);
	}
	if(class->subordinates != NULL){
		free(class->subordinates);
	}
	if(class->assignments != NULL){
		free(class->assignments);
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

void school_room_remove(School * school, int room_i){
	int i,j;
	LMH_ASSERT(school != NULL && room_i < school->n_rooms);
	free(school->rooms[room_i].name);
	free(school->rooms[room_i].short_name);
	free(school->rooms[room_i].availability);
	if(school->solutions != NULL){
		for(i = 0; i < school->n_solutions; ++i){
			Meeting * m_list = school->solutions[i].meetings;
			for(j = 0; m_list[j].m_class != NULL; ++j){
				if(m_list[j].room->id == school->rooms[room_i].id){
					m_list[j].room = NULL;
				}
			}
		}
	}
	for(i = room_i; i < school->n_rooms; ++i){
		school->rooms[i] = school->rooms[i+ 1];
	}

	for(i = 0; i < school->n_teachers; ++i){
		if(school->teachers[i].lecture_room_scores){
			printf("A - 1\n");
			remove_from_int_list(school->teachers[i].lecture_room_scores, room_i);
		}
		if(school->teachers[i].planning_room_scores){
			printf("A - 2\n");
			remove_from_int_list(school->teachers[i].planning_room_scores, room_i);
		}
	}
	for(i = 0; i < school->n_classes; ++i){
		if(school->classes[i].room_scores){
			printf("A - 3\n");
			remove_from_int_list(school->classes[i].room_scores, room_i);
		}
	}
	for(i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].room_scores){
			printf("A - 4\n");
			remove_from_int_list(school->teaches[i].room_scores, room_i);
		}
	}
	for(i = 0; i < school->n_meetings; ++i){
		if(school->meetings[i].possible_rooms){
			printf("A - 5\n");
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
