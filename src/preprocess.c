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

void school_subject_remove(School * school, int subj_i){
	int i, j, k;
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

void school_class_add(School * school, const Class * const c){
	int i, j, n;
	if(school->classes == NULL || school->n_classes == 0){
		school->classes = calloc(11, sizeof(Class));
	} else if(school->n_classes % 10 == 0) {
		school->classes = realloc(school->classes,(school->n_classes + 11) * sizeof(Class));
	}
	school->classes[ school->n_classes ] = *c;

	if(c->assignments != NULL){
		for(n = 0; c->assignments[n] != NULL; ++n){
			/* Blank on purpouse */
		}
		if( n != 0 ){
			if(school->meetings == NULL || school->n_meetings == 0){
				school->meetings = calloc( (n - n%10) + 11, sizeof(Meeting));
			} else if(school->n_meetings % 10 == 0){
				school->meetings = realloc(school->meetings, (n + 11)*sizeof(Meeting));
			}
			for(i = 0; i < n; ++i){
				for(j = 0; j < c->assignments[i]->amount; ++j){
					/* c points to a temporary variable */
					school->meetings[school->n_meetings].m_class = &school->classes[ school->n_classes ];
					school->meetings[school->n_meetings].subject = c->assignments[i]->subject;
				}
			}
		}
	}
	++school->n_classes;
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

// void school_feature_remove(School * school, int feature_i){
// 	int i, j;
//
// 	if(school->n_features > feature_i && feature_i >= 0){
// 		free(school->feature_names[feature_i]);
// 		for(i = feature_i; i < school->n_features && school->feature_ids[i] >= 0; ++i){
// 			school->feature_names[i] = school->feature_names[i+1];
// 			school->feature_ids[i] = school->feature_ids[i+1];
// 		}
//
// 		/* If there are features right of the deleted, push them back, in all structures. */
// 		if(school->feature_ids[feature_i+1] != -1){
// 			if(school->rooms != NULL && school->n_rooms > 0){
// 				for(i = 0;  i < school->n_rooms; ++i ){
// 					for(j = feature_i + 1;  school->rooms[i].room_features && school->rooms[i].room_features[j] >= 0; ++j){
// 						/* copies the -1 terminator too. */
// 						school->rooms[i].room_features[j] = school->rooms[i].room_features[j+1];
// 					}
// 				}
// 			}
//
// 			if(school->teaches != NULL && school->n_teaches > 0){
// 				for(i = 0; i < school->n_teaches; ++i){
// 					for(j = feature_i + 1; school->teaches[i].features[j] >= 0; ++j){
// 						/* copies the -1 terminator too. */
// 						school->teaches[i].features[j] = school->teaches[i].features[j+1];
// 					}
// 					/* May have a different terminator. */
// 					for(j = feature_i + 1; school->teaches[i].features[j] >= 0; ++j){
// 						school->teaches[i].min_features[j] = school->teaches[i].min_features[j+1];
// 					}
// 				}
// 			}
// 		}
// 		--school->n_features;
// 	}
// }

void school_room_add(School * school, const Room * const room){
	if(school->n_rooms == 0){
		school->rooms = (Room*) calloc(11, sizeof(Room));
	} else if(school->n_rooms % 10 == 0) {
		school->rooms = (Room *) realloc(school->rooms, (school->n_rooms + 11 - (school->n_rooms % 10))*sizeof(Room));
	}
	school->rooms[school->n_rooms] = *room;
	school->n_rooms++;
}

void school_room_remove(School * school, int room_i){

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
