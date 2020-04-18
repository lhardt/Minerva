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

Meeting * create_timetable(School * school);

DecisionTree * init_decision_tree(School * school);

void primary_search(DecisionNode * tree, int limit_sec);

void explore_consequences(DecisionTree * tree, int limit_sec, Meeting * affected, Teacher * fixed_teacher, Class * fixed_class, int fixed_period);


#endif /* SOLVER_H */
