/*
 * Project Minerva.
 *   solver.h - timetable creator.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module has a solver function and some
 * auxiliary to the user that is hands-on with his timetable.
 */
#ifndef SOLVER_H
#define SOLVER_H

#include "types.h"
#include "decisions.h"

/* CREATE TIMETABLE
 * 		Tries to create, from start to finish, a timetable for the school.
 *
 * 		Returns NULL on failure.
 *
 * Development status:
 *		-
 */
Meeting * create_timetable(School * school);

/* INIT DECISION TREE
 *		Creates a decision tree for this timetabling problem.
 *
 *		Returns NULL on failure.
 *
 * Development status:
 * 		-
 */
DecisionTree * init_decision_tree(School * school);

/* PRIMARY SEARCH
 * 		Tries to create new nodes.
 *
 *		Returns false on nothing done.
 *
 * Development status:
 *		-
 */
bool primary_search(DecisionNode * tree, int limit_sec);

/* EXPLORE CONSEQUENCES
 *		Tries to find what this decision will mean in other nodes.
 *
 *		Returns true if the decision seems sound.
 *
 * Development status:
 *		-
 */
bool explore_consequences(DecisionTree * tree, int limit_sec, Meeting * affected, Teacher * fixed_teacher, Class * fixed_class, int fixed_period);


#endif /* SOLVER_H */
