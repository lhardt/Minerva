/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
#ifndef DECISIONS_H
#define DECICIONS_H


typedef struct PrimaryNode{
	int id;
	int parentId;
	Class * affected;
	Class * finalState;
	Meeting * conclusion;


	double score;
} PrimaryNode;

typedef struct PrimaryTree{
	int id;

	int alloc_sz;
	int last_id;
	/* DecisionNode with id N allocated at table[N]. */
	PrimaryNode * table;
} PrimaryTree;

typedef struct SecondaryNode{
	int id;

	int alloc_sz;
	int last_id;
	/* DecisionNode with id N allocated at table[N]. */
	SecondaryNode * table;
}

#endif /* DECISIONS_H */
