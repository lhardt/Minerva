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
	NODE_NULL_TYPE = 0,
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

	/* NOTE these are created before the children and used
	 * as measure of what nodes must be created next.
	 * not the other way around.
	 */
	int * children_score;
	int * children_score_order;
	int children_alloc_sz;

	int next_affected_meeting_index;
	NodeType next_node_type;

	/* What decision was made. Is this field necessary? */
	int affected_meeting_index;
	/* Defined by this.type. */
	union{
		Teacher * fixed_teacher;
		int		  fixed_period;
		Room	* fixed_room;
	};
	/* The meeting list computed afterwards. */
	Meeting * conclusion;

	int is_final;
	int is_consistent;

	int score;
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
