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
#include "solver.h"

#include <stdlib.h>
#include <stdio.h>

#include "assert.h"
#include "decisions.h"
#include "logic.h"
#include "score.h"
#include "types.h"
#include "util.h"

int * make_possible_teacher_list(School * school, Meeting * meeting){
	int i_teacher, i_teaches;
	/* Reference to shorten indirection. */
	Teacher * teacher;

	LMH_ASSERT( (school != NULL) && (meeting != NULL));

	int * scores = calloc(school->n_teachers + 1, sizeof(int));
	scores[school->n_teachers] = -1;


	for(i_teacher = 0; i_teacher < school->n_teachers; i_teacher++){
		teacher = &(school->teachers[i_teacher]);
		for(i_teaches = 0; teacher->teaches[i_teaches] != NULL; i_teaches++){
			if(teacher->teaches[i_teaches]->subject == meeting->subject){
				scores[i_teacher] = teacher->teaches[i_teaches]->score;
				printf("Setting score of %d to meeting (subj: %s) with teacher %s (i_teacher %d)\n", scores[i_teacher], meeting->subject->name, teacher->name, i_teacher);
			}
		}
	}
	return scores;
}

int* make_possible_room_list(School * school, Meeting * meeting){
	int i_room, i_list = 0;
	/* Reference to shorten indirection. */
	Room * room;

	int * list = calloc(school->n_rooms + 1, sizeof(int));

	for(i_room = 0; i_room < school->n_rooms; i_room++){
		room = &(school->rooms[i_room]);
		if(room->size >= meeting->m_class->size){
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
		possible_periods[i_per] = school->periods[i_per]?(meeting->m_class->period_scores[i_per]):0;
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

	tree->start = calloc(1, sizeof(DecisionNode));

	tree->n_meetings = count_required_meetings(school, NULL, NULL);
	* tree->start = (DecisionNode) {
		.id=0,
		.parent = NULL,
		.type = NODE_START,
		.owner = tree,
		.conclusion = calloc(tree->n_meetings + 1, sizeof(Meeting)),
		.children = NULL,
		.n_children = 0,
		.children_score = NULL,
		.children_score_order = NULL,
		.children_alloc_sz = 0,
		.next_affected_meeting_index = -1,
		.affected_meeting_index = -1,
		.is_final = false,
		.is_consistent = true,
		.score = 1
	};
	conclusion = tree->start->conclusion;

	for(i_class = 0; i_class < school->n_classes; i_class++){
		class = &(school->classes[i_class]);
		for(i_need = 0; class->assignments[i_need] != NULL; i_need++){
			for(i_quant = 0; i_quant < class->assignments[i_need]->amount; i_quant++){
				conclusion[i_meet].type = meet_LECTURE;
				conclusion[i_meet].m_class = class;
				conclusion[i_meet].subject = class->assignments[i_need]->subject;
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


DecisionNode * make_decision(const School * const school, DecisionNode * parent){
	int i_child = 0;
	DecisionNode * child = NULL;
	bool has_children = true;

	if(!parent->is_final && parent->is_consistent){
		if(parent->children_score == NULL){
			has_children = score_possible_children(school, parent);
		} else {
			/* Find first not initialized */
			for(i_child = 0; parent->children_score_order[i_child] >= 0; ++i_child){
				if(i_child >= parent->children_alloc_sz){
					parent->children_alloc_sz += 16;
					parent->children = realloc(parent->children, parent->children_alloc_sz);
					break;
				}
				if(parent->children[i_child].type == NODE_NULL_TYPE){
					break;
				}
			}
		}
		if( !has_children || parent->children_score_order[i_child] < 0){
			return NULL;
		}
		child = &parent->children[i_child];
		*child = (DecisionNode){
			.id=1,
			.type=parent->next_node_type,
			.parent=parent,
			.children=NULL,
			.owner=parent->owner,
			.n_children = 0,
			.children_score = NULL,
			.children_score_order = NULL,
			.children_alloc_sz = 0,
			.next_affected_meeting_index = -1,
			.affected_meeting_index = parent->next_affected_meeting_index,
			.is_final = false,
			.is_consistent = true,
			.conclusion = copy_meetings_list(parent->conclusion),
			.score = -1
		};

		parent->n_children++;

		switch(parent->next_node_type){
			case NODE_TEACHER:{
				child->fixed_teacher = &(school->teachers[parent->children_score_order[i_child]]);
				break;
			}
			case NODE_ROOM:{
				child->fixed_room = &(school->rooms[parent->children_score_order[i_child]]);
				break;
			}
			case NODE_PERIOD:{
				child->fixed_period = parent->children_score_order[i_child];
			}
			case NODE_START:
			case NODE_NULL_TYPE:{
				break;
			}
		}
		new_node_elimination(school, child);
		child->score = new_node_evaluation(school, child);
	} else {
		printf("Was final. No can do. Parent isfinal %d, Parent isConsistent %d\n", parent->is_final, parent->is_consistent);
	}
	return child;
}


DecisionNode * dfs_timetable_create(const School * const school , DecisionNode* start){
	DecisionNode * curr = start,* next;
	curr = start;
	next = NULL;
	int i = 0;

	root_elimination(school, start);
	start->is_final = is_node_final(school, start);
	start->is_consistent = !is_node_inconsistent(school, start);

	while(curr != NULL && i < 100 && !curr->is_final){
		next = make_decision(school, curr);
		printf("======================= %2d ======================\n", i);
		print_meeting_list(stdout,next->conclusion);
		if(next == NULL || !curr->is_consistent){
			curr = curr->parent;
		} else {
			curr = next;
		}
		++i;
	}

	return curr;
}

// TODO: copy school.
Meeting * create_timetable(School * school){
	bool valid = false;
	Meeting * ret_val = NULL;

	LMH_ASSERT(school != NULL);

	// school = make_school_copy(in_school);

	/* Preprocess */
	flatten_teacher_subordination(school);
	flatten_class_subordination(school);

	DecisionTree * tree = init_decision_tree(school);

	valid = root_consistency_check(school, (tree->start));
	if(!valid){
		return NULL;
	}

	DecisionNode * solution = dfs_timetable_create(school, tree->start);

	if(solution != NULL){
		ret_val = copy_meetings_list(solution->conclusion);
	}

	free_node(tree->start);
	free(tree->start);
	free(tree);

	return ret_val;
}
