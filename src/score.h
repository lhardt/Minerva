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

/* Gives this node its score. */
bool rank_partial_node(School * school, DecisionNode * node);

/* Gives this node its score. */
bool rank_final_node(School * school, DecisionNode * node);

/* Ranks possible children nodes. */
bool score_children(School * school, DecisionNode * node);

#endif /* SCORE_H */
