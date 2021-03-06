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

/* NEW NODE EVALUATION
 * 		Gives this node its score.
 *
 * Developent status:
 *		Implemented.
 */
bool new_node_evaluation(const School * const school, DecisionNode * node);

/* LEAF NODE EVALUATION
 * 		Gives a complete timetable its score.
 * 		More exensive than new_node_evaluation
 *
 * TODO: Implement.
 */
bool leaf_node_evaluation(School * school, DecisionNode * node);

/* SCORE POSSIBLE CHILDREN
 *		Ranks and orders possible children before they are created.
 *
 * Development Status:
 *		Implemented.
 */
bool score_possible_children(const School * const school, DecisionNode * node);

#endif /* SCORE_H */
