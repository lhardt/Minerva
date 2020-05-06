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

int get_max_teacher_score_discrepancy(const School * const school, const DecisionNode * const parent, int * res_index){
	int max_disc = -1, curr_disc = -1, i = 0, max_i = 0;

		// TODO.

	*res_index = max_i;
	return max_disc;
}

bool score_possible_children(const School * const school, DecisionNode * parent){
	int i_child;
	parent->children_score = calloc(32, sizeof(int));
	parent->children_score_order = calloc(32, sizeof(int));
	parent->children = calloc(32, sizeof(DecisionNode));
	parent->children_alloc_sz = 31;

	i_child = 0;

	for(i_child = 0; i_child < 32; ++i_child){
		parent->children_score[i_child] = -1;
		parent->children_score_order[i_child] = -1;
	}

	// TODO

	return false;
}
