/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
#ifndef DECISIONS_H
#define DECICIONS_H

#include "types.h"

typedef struct PrimaryNode PrimaryNode;
struct PrimaryNode{
	int id;
	PrimaryNode * parent;
	/* What decision was made. Is dthis field necessary? */
	Class * affected;
	/* The consequences of this arrangement */
	Class * finalState;
	Meeting * conclusion;


	double score;
};

typedef struct PrimaryTree{
	int id;

	int alloc_sz;
	int last_id;
	/* DecisionNode with id N allocated at table[N]. */
	PrimaryNode * table;
} PrimaryTree;

typedef struct SecondaryNode SecondaryNode;
struct SecondaryNode{
	int id;
	SecondaryNode * parent;

	Meeting * meetings;
};

typedef struct SecondaryTree{
	int id;

	int alloc_sz;
	int last_id;
	/* DecisionNode with id N allocated at table[N]. */
	SecondaryNode * table;
} SecondaryTree;

#endif /* DECISIONS_H */
