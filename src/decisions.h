/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
#ifndef DECISIONS_H
#define DECICIONS_H

#include "types.h"

typedef enum NodeType {
	NODE_ROOM,
	NODE_TEACHER,
	NODE_PERIOD
} NodeType;

typedef struct DecisionTree DecisionTree;
typedef struct DecisionNode DecisionNode;
struct DecisionNode{
	int id;
	NodeType type;
	DecisionNode * parent;
	DecisionTree * owner;

	// TODO: threading.
	// bool is_locked;
	// thrd_t responsible;
	// bool is_child_locked;

	/* What decision was made. Is this field necessary? */
	Meeting * affected_meeting;
	/* The meeting list computed afterwards. */
	Meeting * conclusion;

	double score;
};

struct DecisionTree{
	int id;
	/* Nodes may be stored linearly at start[i] */
	int alloc_sz;
	int last_id;

	DecisionNode * start;
};

/* Generates a tree based on the initial data informed by user */
DecisionTree * make_primary_tree(School * school);
/* Tries to find solutions based on the current situation */
void primary_search(DecisionNode * tree, int limit_sec);

void explore_consequences(DecisionTree * tree, int limit_sec, Meeting * affected, Teacher * fixed_teacher, Class * fixed_class, int fixed_period);



#endif /* DECISIONS_H */
