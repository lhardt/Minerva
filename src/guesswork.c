/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "definitions.h"
#include "combinatorics.h"
#include "logic.h"
#include "guesswork.h"

/* GLOBAL */
int g_CURRENT_ID = 0;

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
int * order_by_score_discrepancy(Meeting * meetings){
	int i_met, j_met, i_ord, j_ord, max, n_met, i_per, n_per;
	int * scores;
	int * order;

	for(n_met = 0; meetings[n_met].teacher != NULL; n_met++){}

	scores = calloc(1+ n_met, sizeof(int));
	order  = calloc(1+ n_met, sizeof(int));

	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		if(meetings[i_met].period != -1){
			continue;
		} else {
			max = -1;
			for(i_per = 0; meetings[i_met].possible_periods[i_per] >= 0; i_per++){
				scores[i_met] -= meetings[i_met].possible_periods[i_per];
				if(meetings[i_met].possible_periods[i_per] > max){
					max = meetings[i_met].possible_periods[i_per];
				}
			}

			scores[i_met] += max * i_per;
		}
	}
	scores[i_met] = -1;
	order_by_rank_not_null(order, scores);
	free(scores);

	return order;
}

/* Starts a guess tree with a class list. */
GuessNode * init_guess_tree(School * school){
	GuessNode * initial_node = calloc(1, sizeof(GuessNode));
	initial_node->id = ++g_CURRENT_ID;
	initial_node->parent   = NULL;
	initial_node->children = NULL;

	initial_node->score = 0;

	initial_node->conclusion = initialize_all_meetings(school, NULL);
	initial_node->solved = seems_solved(initial_node->conclusion);
	initial_node->contradiction = is_immediately_impossible(initial_node->conclusion);
	initial_node->score_order = order_by_score_discrepancy(initial_node->conclusion);
	initial_node->children = calloc(32, sizeof(GuessNode));
	initial_node->children_alloc_sz = 31;

	return initial_node;
}

/* Destroys a prevoious allocation of a node. */
void destroy_node(GuessNode * node){
	int i_child = 0;
	if(node->children != NULL){
		for(i_child = 0; node->children[i_child].id > 0; i_child++){
			destroy_node(&node->children[i_child]);
		}
		free(node->children);
		if(node->score_order != NULL){
			free(node->score_order);
		}
		if(node->conclusion != NULL){
			destroy_meeting_list(node->conclusion);
		}
	}
}

/* Destroys a previous allocation of a node tree. */
void destroy_node_tree(GuessNode * node){
	destroy_node(node);
	free(node);

}

/* Makes a guess and stores it on node.children.
 * Returns true if it was possible to make the guess;
 */
bool make_guess(School * school, GuessNode * node){
	int i = 0, n_guesses = 0;
	// Initialization.
	for(n_guesses = 0; node->children[n_guesses].id != 0; n_guesses ++){ }

	// TODO: make so that we can backtrack earlier.
	// If we made all guesses possible, backtrack:
	if(n_guesses >= int_list_len(node->score_order)){
		//printf("Can't make another guess. Backtrack or analyze another node.\n");
		return false;
	} else {
		GuessNode * new_node = &node->children[n_guesses];
		new_node->id = ++g_CURRENT_ID;
		// printf("[GuessWork] Creating node (%d) with parent (%d).\n", new_node->id, node->id );
		new_node->conclusion = make_meetings_copy(node->conclusion);
		// Setting the guess value.
		// The n other guesses have used the n prior score orders.
		Meeting * guess_met = &(new_node->conclusion[node->score_order[0]]);
		int guess_val = find_max_int_index(guess_met->possible_periods);
		// Since we are now fixing a value, we must propagate the consequences of that fixation
		guess_met->period = guess_val;
		propagate_meeting_fixation(new_node->conclusion, node->score_order[n_guesses]);
		// Check for consequences
		explore_consequences(school, new_node->conclusion);
		new_node->contradiction = is_immediately_impossible(new_node->conclusion);
		new_node->solved	 = seems_solved(new_node->conclusion);
		new_node->score_order = order_by_score_discrepancy(new_node->conclusion);
		new_node->children = calloc(32, sizeof(GuessNode));
		new_node->children_alloc_sz = 31;
		// print_node_status(new_node);
		// If new_node is a contradiction, then eliminate that possibility from node
		if(new_node->contradiction){
			// the guess we made was invalid. So invalidate it on node.
			node->conclusion[node->score_order[n_guesses]].possible_periods[guess_val] = 0;
			// propagate_meeting_fixation(new_node->conclusion, node->score_order[n_guesses]);

			// TODO: make a function like propagate_meeting_annihilation(),
			// that works well on this case. explore_consequences() does not
			// work well here. I don't know fully well the consequences of it.
			// One thing that I know it does is that if there is only one option
			// remaining in the possible_periods now, it will collapse to
			// period.

			// TODO: figure out a way of rewarding the school preferences inside
			// node->score. Ideally, it would be stricly in the order computed by
			// score_order that we find the best timetables
			explore_consequences(school,node->conclusion);
			node->solved	 = seems_solved(node->conclusion);
			free(node->score_order);
			node->score_order = order_by_score_discrepancy(node->conclusion);

		} else if(new_node->solved) {
			printf("Found a solution\n");
		}
		return true;
	}

	// Initialize new_node->score_order
}
