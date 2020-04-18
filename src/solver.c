/*
 * Project Minerva.
 *   solver.c - timetabling problem solver.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains structures and functions for the creation
 * and development of a decision tree in school timetabling.
 */

#include "assert.h"
#include "types.h"
#include "logic.h"
#include "decisions.h"
#include "solver.h"

#include <stdlib.h>
#include <stdio.h>

static const int  FIRST_ALLOC_SZ = 100;
// static const int EXPAND_ALLOC_SZ = 100;

static int g_TREE_LASTID = 0;

int * make_possible_teacher_list(School * school, Meeting * meeting){
	int i_teacher, i_teaches;
	/* Reference to shorten indirection. */
	Teacher * teacher;

	LMH_ASSERT( (school != NULL) && (meeting != NULL), "null parameters" );

	int * scores = calloc(school->n_teachers + 1, sizeof(int));
	scores[school->n_teachers] = -1;


	for(i_teacher = 0; i_teacher < school->n_teachers; i_teacher++){
		teacher = &(school->teachers[i_teacher]);
		for(i_teaches = 0; teacher->teaches[i_teaches] != NULL; i_teaches++){
			if(teacher->teaches[i_teaches]->subject == meeting->subj){
				scores[i_teacher] = teacher->teaches[i_teaches]->score;
			} else {
				/* TODO: written for clarity. calloc makes it redundant */
				scores[i_teacher] = 0;
			}
		}
	}
	return scores;
}

/* It is impossible to
 *
 *
 */
int* make_possible_room_list(School * school, Meeting * meeting){
	int i_room, i_list = 0;
	/* Reference to shorten indirection. */
	Room * room;

	int * list = calloc(school->n_rooms + 1, sizeof(int));

	for(i_room = 0; i_room < school->n_rooms; i_room++){
		room = &(school->rooms[i_room]);
		if(room->size >= meeting->class->size){
			list[i_room] = 1;
		}
		i_list++;
	}
	list[school->n_rooms] = -1;
	return list;
}

int * make_possible_period_list(School * school, Meeting * meeting){
	int i_per;

	int * possible_periods = calloc(school->n_periods + 1, sizeof(int));

	for(i_per = 0; i_per < school->n_periods; i_per++){

		// TODO: melhorar este algorítmo. No momento,
		// ele calcula o mínimo entre os dois valores
		possible_periods[i_per] = meeting->class->periods[i_per];
	}
	possible_periods[school->n_periods] = -1;
	return possible_periods;
}

DecisionTree * init_decision_tree(School * school){
	int i_class, i_meet = 0, i_need, i_quant;
	/* References to shorten indirection. */
	Class * class;
	Meeting * conclusion;


	DecisionTree * tree = calloc(1, sizeof(DecisionTree));

	tree->alloc_sz = FIRST_ALLOC_SZ;

	tree->start = calloc(tree->alloc_sz, sizeof(DecisionNode));
	tree->start[0].id = ++g_TREE_LASTID;
	tree->last_index = tree->start[0].id;

	tree->start[0].type = NODE_START;
	tree->start[0].parent = NULL;
	tree->start[0].owner = tree;

	tree->n_meetings = count_required_meetings(school, NULL, NULL);

	tree->start[0].conclusion = calloc(tree->n_meetings + 1, sizeof(Meeting));

	conclusion = tree->start[0].conclusion;

	for(i_class = 0; i_class < school->n_classes; i_class++){
		class = &(school->classes[i_class]);
		for(i_need = 0; class->needs[i_need].subject != NULL; i_need++){
			for(i_quant = 0; i_quant < class->needs[i_need].quantity; i_quant++){
				conclusion[i_meet].class = class;
				conclusion[i_meet].subj  = class->needs[i_need].subject;
				conclusion[i_meet].teacher = NULL;
				conclusion[i_meet].room = NULL;
				conclusion[i_meet].period = -1;

				conclusion[i_meet].possible_teachers = make_possible_teacher_list(school,&conclusion[i_meet]);
				conclusion[i_meet].possible_rooms = make_possible_room_list(school, &conclusion[i_meet]);
				conclusion[i_meet].possible_periods = make_possible_period_list(school,&conclusion[i_meet]);
				i_meet++;
			}
		}
	}
	return tree;
}

int make_decision(School * school, DecisionNode * parent){
	// int i_child, i_score;
	//
	//
	// if(!parent->is_final){
	// 	if(parent->children_score == NULL){
	// 		score_children(school, parent);
	// 	} else {
	// 		for(i_child = 0; parent->children_score[i_child] >= 0; ++i_child){ }
	// 		//
	// 		// 	if(i_child >= children_alloc_sz){
	// 		// 		parent->children_alloc_sz += 16;
	// 		// 		parent->children = realloc(parent->children, parent->children_alloc_sz);
	// 		// 		parent->children_score =
	// 		// 	}
	// 	}
	// }
	return 0;
}


Meeting * create_timetable(School * school){
	bool valid = false;
	/* currhead refers to the top node being changed */
	// DecisionNode * curr, currhead;

	LMH_ASSERT(school != NULL, "null par");

	/* Preprocess  -- ? */
	/* Initialize  */
	DecisionTree * tree = init_decision_tree(school);
	/* Diagnose inconsistencies */
	valid = root_consistency_check(school, &(tree->start));
	if(!valid){
		return NULL;
	}
	/* Create a valid timetable */
	// curr = tree->start;



	/* The tree will have, at most, 3x n_meetings depth */
	int * min_score_by_depth = calloc( 3 * tree->n_meetings + 1, sizeof(int));
	min_score_by_depth[3 * tree->n_meetings] = -1;
	while(true){

	}
	/* Then optimize it */


	return NULL;
}