/*
 * Project Minerva.
 *   school_examples.h - examples for testing / demonstrating.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This file contains examples of schools that can be loaded
 * in the program and tested both by the user and the developer.
 */
#ifndef SCHOOL_EXAMPLES_H
#define SCHOOL_EXAMPLES_H

#include "types.h"

typedef struct SchoolExample {
	char * name;
	School school;
	int n_possible_solutions;
	Solution * possible_solutions;
} SchoolExample;



#endif /* SCHOOL_EXAMPLES_H */
