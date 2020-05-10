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
	DecisionNode * children;
	DecisionTree * owner;
	int n_children;

	/* TODO: threading.
	 *
	 * bool is_locked;
	 * thrd_t responsible;
	 * bool is_child_locked;
	 */

	 /* Variables created before the children themselves and used
	 * as measure of what nodes must be created next.
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
	/* NOTE: planned subdividing nodes into trees
	 * to allocate a tree for each thread. not necessary
	 * by now.
	 */
	int parent_node_id;
	
	int n_meetings;

	DecisionNode * start;
	DecisionNode * best_satisfaction_node;
};

#endif /* DECISIONS_H */
