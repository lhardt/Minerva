
#include <stdio.h>

#include "combinatorics.h"
#include "primary_logic.h"

int count_required_meetings(
				School * school,
				Class * class,
				Subject * subject
				){
	int count = 0, i_class = 0, i_need = 0;
	if(school != NULL && school->classes != NULL && school->n_classes > 0){
		for(i_class = 0; i_class < school->n_classes; i_class++){
			if( (class == NULL) != (&school->classes[i_class] == class) ){
				for(i_need = 0; school->classes[i_class].needs[i_need].subject != NULL; i_need++){
					if((subject == NULL) != (school->classes[i_class].needs[i_need].subject == subject)){
						count += school->classes[i_class].needs[i_need].quantity;
					}
				}
			}
		}
	} else {
		printf("Count called with invalid school\n");
	}
	return count;
}

/* Meetings are analogous if class == class && subj == subj */
bool elim_analogous_ordering(School * school, DecisionNode * node){
	int i_meet = 0, j_meet = 0, impossible = -1;
	bool change = false;
	/* Reference to shorten indirection */
	Meeting * meetings = node->conclusion;
	/* Ascending */
	for(i_meet = 0; meetings[i_meet].class != NULL; i_meet++){
		for(j_meet = i_meet+1; meetings[j_meet].class != NULL; j_meet++){
			/* If they are analogous and not fixed */
			if(   meetings[i_meet].class == meetings[j_meet].class
			   && meetings[i_meet].subj == meetings[j_meet].subj
		       && meetings[i_meet].period == -1
		       && meetings[j_meet].period == -1){
				impossible = find_first_positive(meetings[i_meet].possible_periods);
				if(meetings[j_meet].possible_periods[impossible] != 0){
					meetings[j_meet].possible_periods[impossible] = 0;
					change = true;
					printf("Fix analogous %d %d\n", i_meet, j_meet);
				}
			}
		}
	}
	return change;
}

/* Propagates only in the criterion of simultaneity
 * The criterions of max_per_day or max_per_week are
 *  dealt with in other functions
 */
// bool elim_propagate_room_fixation(School * school, DecisionNode * node, int fix_meet){
// 	int i_meet = 0;
//
// 	/* Reference to shorten indirection. */
// 	Meeting * meeting;
// 	for(i_meet = 0; node->conclusion[i_meet].class != NULL; i_meet++){
// 		meeting = &node->conclusion[i_meet];
// 		if(meeting->period != -1 && )
// 	}
//
// }

bool elim_search_fixed_meeting(School * school, DecisionNode * node){
	int i_meet = 0;
	bool change = false;
	/* Reference to shorten indirection. */
	Meeting * meeting;
	for(i_meet = 0; node->conclusion[i_meet].class != NULL; i_meet++){
		meeting = &node->conclusion[i_meet];
		/* Check for fixed teachers */
		if(meeting->teacher == NULL && 1 == not_null_int_list_len(meeting->possible_teachers)){
			meeting->teacher = &school->teachers[find_first_positive(meeting->possible_teachers)];
			printf("Fixed teacher\n");
			change = true;
		}
		/* Check for fixed rooms */
		if(meeting->room == NULL && 1 == not_null_int_list_len(meeting->possible_rooms)){
			meeting->room = &school->rooms[find_first_positive(meeting->possible_rooms)];
			printf("Fixed room %d on meeting %d \n", find_first_positive(meeting->possible_rooms), i_meet);
			change = true;
		}
		/* Check for fixed periods */
		if(meeting->period == -1 && 1 == not_null_int_list_len(meeting->possible_periods)){
			meeting->period = find_first_positive(meeting->possible_periods);
			change = true;

			printf("Fixed period\n");
		}
	}
	return change;
}
