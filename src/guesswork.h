#ifndef GUESSWORK_H
#define GUESSWORK_H


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

	order_by_rank(order, scores);
	printf("Discrepancies vs ");
	printf("Order by Discrepancy: \n");
	print_int_list(scores);
	print_int_list(order);


	return order;
}

#endif /* GUESSWORK_H */
