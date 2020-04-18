/*
 * Project Minerva.
 *   decisions.h - decision tree for timetabling.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains structures related to
 * decision trees for school timetabling.
 */
#ifndef DECISIONS_H
#define DECISIONS_H

#include "types.h"

typedef enum NodeType {
	NODE_START,
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
	/* Does not make it clear when nodes
	 * from different parents are sequentially
	 * allocated. This is an optimisation planned
	 * for the near future. The distinction of "end of list"
	 * is given by n_children.
	 */
	DecisionNode * children;
	DecisionTree * owner;
	int n_children;

	// TODO: threading.
	// bool is_locked;
	// thrd_t responsible;
	// bool is_child_locked;

	int * children_score;
	int * children_score_order;
	int children_alloc_sz;

	Meeting * next_affected_meeting;
	NodeType next_node_type;

	/* What decision was made. Is this field necessary? */
	Meeting * affected_meeting;
	/* The meeting list computed afterwards. */
	Meeting * conclusion;

	int is_final;
	int is_consistent;

	double score;
};

struct DecisionTree{
	int parent_node_id;
	/* Nodes may be stored linearly at start[i] */
	int alloc_sz;
	int last_index;

	int n_meetings;

	DecisionNode * start;
	DecisionNode * best_satisfaction_node;
};

#endif /* DECISIONS_H */
