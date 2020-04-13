/*
 * Project Minerva.
 *   decisions.h - decision tree for timetabling.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains structures and functions for the creation
 * and development of a decision tree in school timetabling.
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
	DecisionNode * children;
	DecisionTree * owner;

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
};


DecisionTree * init_decision_tree(School * school);

void primary_search(DecisionNode * tree, int limit_sec);

void explore_consequences(DecisionTree * tree, int limit_sec, Meeting * affected, Teacher * fixed_teacher, Class * fixed_class, int fixed_period);

#endif /* DECISIONS_H */
