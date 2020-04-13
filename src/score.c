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

#include "types.h"
#include "decisions.h"

bool score_children(School * school, DecisionNode * node){
	// parent->children_score = calloc(32 * sizeof(int));
	// parent->children_score_order = calloc(32 * sizeof(int));
	// parent->children = calloc(32 * sizeof(DecisionNode));
	// parent->children_alloc_sz = 31;
	// i_child = 0;
	// for(i_score = 0; i_score < 32; ++i_score){
	// 	parent->children_score[i_score] = -1;
	// 	parent->children_score_order[i_score] = -1;
	// }
	return false;
}
