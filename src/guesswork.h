#ifndef GUESSWORK_H
#define GUESSWORK_H

/* GLOBAL */
int g_CURRENT_ID = 0;

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
 * Get every meeting a score
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

void print_node_status(GuessNode * node){
	if(node->solved){
		printf("[GuessWork] Node (%d) solves the problem.\n", node->id);
	} else if(node->contradiction) {
		printf("[GuessWork] Node (%d) can't solve the problem.\n", node->id);
	} else {
		int parent_id = -1;
		if(node->parent != NULL){
			parent_id = node->parent->id;
		}
		printf("[GuessWork] Non-ending node (%d) with parent (%d).\n", node->id, parent_id);
	}
}

GuessNode * init_guess_tree(ExtendedClass * classes){
	GuessNode * initial_node = calloc(1, sizeof(GuessNode));
	initial_node->id = ++g_CURRENT_ID;
	initial_node->parent   = NULL;
	initial_node->children = NULL;

	initial_node->score = 0;

	initial_node->conclusion = initialize_all_meetings(classes);
	initial_node->solved = seems_solved(initial_node->conclusion);
	initial_node->contradiction = is_immediately_impossible(initial_node->conclusion);
	initial_node->score_order = order_by_score_discrepancy(initial_node->conclusion);
	initial_node->children = calloc(32, sizeof(GuessNode));
	initial_node->children_alloc_sz = 31;
	// printf("[GuessWork] Creating initial node with id %d.\n", initial_node->id);

	// print_node_status(initial_node);

	return initial_node;
}

/* Makes a guess and stores it on [children].
 * Returns true if it was possible to make the guess;
 * same node.
 */

bool make_guess(GuessNode * node){
	int i = 0, n_guesses = 0;
	// Initialization.
	for(n_guesses = 0; node->children[n_guesses].id != 0; n_guesses ++){ }

	// TODO: make so that we can backtrack earlier.
	// If we made all guesses possible, backtrack:
	if(n_guesses >= int_list_len(node->score_order)){
		printf("Can't make another guess. Backtrack or analyze another node.\n");
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
		explore_consequences(new_node->conclusion);
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
			explore_consequences(node->conclusion);
			node->solved	 = seems_solved(node->conclusion);
			node->score_order = order_by_score_discrepancy(node->conclusion);

		} else if(new_node->solved) {
			printf("Found a solution\n");
		}
		return true;
	}

	// Initialize new_node->score_order
}


void go_by_score_order(){

}

#endif /* GUESSWORK_H */
