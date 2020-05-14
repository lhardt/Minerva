/*
 * Project Minerva.
 *   score.c - scoring decision nodes.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains functions to evaluate both the
 * likelihood of a node having a valid timetable in its
 * domain, as well as the acceptability of such timetable,
 * given the soft constraints.
 */
#include "score.h"

#include "types.h"
#include "decisions.h"
#include "assert.h"

int get_max_teacher_score_discrepancy(const School * const school, const DecisionNode * const parent, int * res_index){
	int max_disc = -1, curr_disc = -1, i = 0, max_i = 0;

	LMH_ASSERT(school != NULL && parent != NULL && res_index != NULL);

	for(i = 0; parent->conclusion[i].m_class != NULL; ++i){
		if(parent->conclusion[i].teacher != NULL){
			continue;
		}
		curr_disc = int_list_discrepancy(parent->conclusion[i].possible_teachers);

		if(max_disc == -1 || curr_disc > max_disc){
			max_disc = curr_disc;
			max_i = i;
		}
	}

	*res_index = max_i;
	return max_disc;
}

int get_max_room_score_discrepancy(const School * const school, const DecisionNode * const parent, int * res_index){
	int max_disc = -1, curr_disc = -1, i = 0, max_i = 0;

	LMH_ASSERT(school != NULL && parent != NULL && res_index != NULL);

	for(i = 0; parent->conclusion[i].m_class != NULL; ++i){
		if(parent->conclusion[i].room != NULL){
			continue;
		}
		curr_disc = int_list_discrepancy(parent->conclusion[i].possible_rooms);

		if(max_disc == -1 || curr_disc > max_disc){
			max_disc = curr_disc;
			max_i = i;
		}
	}

	*res_index = max_i;
	return max_disc;
}

int get_max_period_score_discrepancy(const School * const school, const DecisionNode * const parent, int * res_index){
	int max_disc = -1, curr_disc = -1, i = 0, max_i = 0;

	LMH_ASSERT(school != NULL && parent != NULL && res_index != NULL);

	for(i = 0; parent->conclusion[i].m_class != NULL; ++i){
		if(parent->conclusion[i].period != -1){
			continue;
		}
		curr_disc = int_list_discrepancy(parent->conclusion[i].possible_periods);

		if(max_disc == -1 || curr_disc > max_disc){
			max_disc = curr_disc;
			max_i = i;
		}
	}

	*res_index = max_i;
	return max_disc;
}

/* SCORE POSSIBLE CHILDREN
 *		Ranks and orders possible children before they are created.
 *
 * Development Status:
 *		Implemented.
 */
bool score_possible_children(const School * const school, DecisionNode * parent){
	int i_child, max_period, max_room, max_teacher, i_max_per, i_max_room, i_max_teacher;
	int i_list, * list; /* generalization tool. not allocated. */

	LMH_ASSERT(school != NULL && parent != NULL);

	max_period = get_max_period_score_discrepancy(school, parent, &i_max_per);
	max_room = get_max_room_score_discrepancy(school, parent, &i_max_room);
	max_teacher = get_max_teacher_score_discrepancy(school, parent, &i_max_teacher);
	// TODO use weights. (ex. 3*max_room).
	if(max_period <= 0 && max_room <= 0 && max_teacher <= 0){
		printf("nowhere to go\n");
		return false;
	}
	if(max_period >= max_room && max_period >= max_teacher){
		parent->next_node_type = NODE_PERIOD;
		parent->next_affected_meeting_index = i_max_per;
		list = parent->conclusion[i_max_per].possible_periods;
		parent->children_alloc_sz = school->n_periods;
	} else if(max_room >= max_period && max_room >= max_teacher){
		parent->next_node_type = NODE_ROOM;
		parent->next_affected_meeting_index = i_max_room;
		list = parent->conclusion[i_max_room].possible_rooms;
		parent->children_alloc_sz = school->n_rooms;
	} else if(max_teacher >= max_period && max_teacher >= max_room){
		parent->next_node_type = NODE_TEACHER;
		parent->next_affected_meeting_index = i_max_teacher;
		list = parent->conclusion[i_max_teacher].possible_teachers;
		parent->children_alloc_sz = school->n_teachers;
	}

	parent->children_score = calloc(parent->children_alloc_sz +1, sizeof(int));
	parent->children = calloc(parent->children_alloc_sz +1, sizeof(DecisionNode));

	i_child = 0;

	for(i_child = 0; i_child < parent->children_alloc_sz; ++i_child){
		parent->children_score[i_child] = -1;
	}

	for(i_list = 0; list[i_list] >= 0; ++i_list){
		parent->children_score[i_list] = list[i_list];
	}

	parent->children_score_order = get_desc_order_indexes(list);

	return true;
}

/* NEW NODE EVALUATION
 * 		Gives this node its score.
 *
 * Developent status:
 *		Implemented.
 */
bool new_node_evaluation(const School * const school, DecisionNode * node){

	LMH_ASSERT(school != NULL && node != NULL);

	if(node->parent == NULL){
		printf("Error. null parent\n");
		node->score = 1;
	} else {
		node->score = node->parent->children_score[ node->parent->children_score_order[node - node->parent->children]];
	}
	return true;
}
