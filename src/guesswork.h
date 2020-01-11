/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#ifndef GUESSWORK_H
#define GUESSWORK_H

/* GLOBAL */
extern int g_CURRENT_ID;

typedef struct GuessNode GuessNode;

struct GuessNode{
	int id;
	/* What happens if we make the guess */
	Meeting * conclusion;
	/* A fast way for the guess parent to know what is happening */
	int i_met;
	int guess;
	/* Parent points to one GuessNode. Children points to an invalid-ended list. */
	GuessNode * parent;
	GuessNode * children;
	int children_alloc_sz;
	/* Where to make the next guess */
	int * score_order;
	/* Flags for communication with guess parent */
	bool contradiction;
	bool solved;
	/* No idea of how to make this. Maybe to sum with score_discrepancy. */
	int score;
};


/* If we are to guess, it's better if we guess at Acchiles hells.
 * My best guess for where they are is the following:
 *
 * Get every meeting a score, based on the possible_periods list.
 * - if there is an item with high priority, then the score gets higher.
 * - if there are less meetings with high scores, then the overall score
 *   gets lower.
 *
 * So [1,1,1,0,0] would have a better score than [1,1,0,0,0]
 * and [3,2,0,0,0] would have a higher score than [2,2,0,0,0].
 */
int * order_by_score_discrepancy(Meeting * meetings);

/* Starts a guess tree with a class list. */
GuessNode * init_guess_tree(School * school);

/* Destroys a prevoious allocation of a node. */
void destroy_node(GuessNode * node);

/* Destroys a previous allocation of a node tree. */
void destroy_node_tree(GuessNode * node);

/* Makes a guess and stores it on node.children.
 * Returns true if it was possible to make the guess;
 */
bool make_guess(School * school, GuessNode * node);

#endif /* GUESSWORK_H */
