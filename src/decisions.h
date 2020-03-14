/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
#ifndef DECISIONS_H
#define DECICIONS_H

#include "types.h"

typedef struct PrimaryNode PrimaryNode;
typedef struct PrimaryTree PrimaryTree;

struct PrimaryNode{
	int id;
	PrimaryNode * parent;
	PrimaryTree * owner;

	// TODO: threading.
	// bool is_locked;
	// thrd_t responsible;
	// bool is_child_locked;

	/* What decision was made. Is dthis field necessary? */
	Meeting * affected;
	/* The consequences of this arrangement */
	Class * finalState;
	/* The meeting list computed afterwards. */
	Meeting * conclusion;

	double score;
};

struct PrimaryTree{
	int id;

	/* Nodes may be stored linearly at start[i] */
	int alloc_sz;
	int last_id;

	PrimaryNode * start;
};

typedef struct SecondaryNode SecondaryNode;
struct SecondaryNode{
	int id;
	SecondaryNode * parent;

	Meeting * affected;
	Meeting * meetings;
};

typedef struct SecondaryTree{
	int id;
	/* Nodes may be stored linearly at start[i] */
	int alloc_sz;
	int last_id;

	SecondaryNode * start;
} SecondaryTree;

PrimaryTree * make_primary_tree(School * school);

void primary_search(PrimaryTree * tree, int limit_sec);

void secondary_search(PrimaryTree * tree, int limit_sec);


#endif /* DECISIONS_H */
