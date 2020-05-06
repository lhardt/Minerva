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
#ifndef SCORE_H
#define SCORE_H

#include "decisions.h"
#include "maths.h"
#include "types.h"

/* Gives this node its score.
 *
 * TODO: Implement.
 */
bool new_node_evaluation(const School * const school, DecisionNode * node);

/* Gives a complete timetable its score.
 * More exensive than the former
 *
 * TODO: Implement.
 */
bool leaf_node_evaluation(School * school, DecisionNode * node);

/* Ranks and orders possible children before they are created.
 *
 * TODO: Implement.
 */
bool score_possible_children(const School * const school, DecisionNode * node);

/* Chooses the next direct children of node to be created.
 * Returns -1 if there is no more direct children to be created.
 *
 * TODO: Implement.
 */
int  pick_next_creation(School * school, DecisionNode * node);


#endif /* SCORE_H */
