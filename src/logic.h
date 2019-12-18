#ifndef LOGIC_H
#define LOGIC_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "definitions.h"


/* Returns the number of meetings this list of classes
 *  will have in total.
 */
int get_number_of_meetings(ExtendedClass * ex){
	int n_meetings = 0, i_class = 0, i_tq = 0;
	ExtendedClass * c;

	for(; ex[i_class].name != NULL; i_class++){
		c = &(ex[i_class]);
		for(i_tq = 0; c->teachers[i_tq].teacher != NULL; i_tq++){
			n_meetings += c->teachers[i_tq].quantity;
		}
	}

	return n_meetings;
}


/* Calculates how both the teacher and the class like, for every period,
 * that their meeting be on that period
 */
int * get_preliminary_meeting_score(int * teacher_score, int * class_score){
	int i, *meeting_score, list_size = 63;

	meeting_score = calloc(64, sizeof(int));

	for(i = 0; teacher_score[i] != -1 && class_score[i] != -1; i++){
		// Nullify the score in case one of them is not avalible.
		if(teacher_score[i] == 0 || class_score[i] == 0){
			meeting_score[i] = 0;
		} else {
			// TODO: rethink this scoring system.
			// I want it to be fair both to the teacher and class
			// scores for that period, but maybe more dispersed.
			// The -1 is because both are larger than 1.
			meeting_score[i] = teacher_score[i] + class_score[i] -1;
		}
		if(i == list_size-1){
			list_size += 64;
			meeting_score = realloc(meeting_score,list_size * sizeof(int));
		}
	}
	meeting_score[i] = -1;
	return meeting_score;
}

/* If the same teacher has two meetings with the same class,
 * all of them in the periods [1,2,3], we know that the second
 * can't be in the period 1 because they must coexist.
 */
bool eliminate_left_clone_meeting_period_options(Meeting * meetings) {
	int i_met, j_met, first_possible;
	bool changed_something = false;

	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		first_possible = find_first_positive(meetings[i_met].possible_periods);
		if(first_possible == -1)
			continue;
		// no clone meeting can be in the 1st period of meetings[i] of
		// this meeting
		for(j_met = i_met+1; meetings[j_met].teacher != NULL; j_met++){
			bool is_clone =
				   (meetings[i_met].teacher == meetings[j_met].teacher)
				&& (meetings[i_met].class   == meetings[j_met].class  );
			if(is_clone && meetings[j_met].possible_periods[first_possible] != 0){
				changed_something = true;
				meetings[j_met].possible_periods[first_possible] = 0;
			}
		}
	}
	return changed_something;
}

/* If the same teacher has two meetings with the same class,
 * all of them in the periods [1,2,3], we know that the
 * first can't be in the period 3, because they must coexist.
 */
bool eliminate_right_clone_meeting_period_options(Meeting * meetings) {
	int n_met, i_met, j_met, last_possible;
	bool changed_something = false;

	for(n_met = 0; meetings[n_met].teacher != NULL; n_met++){
	}
	for(i_met = n_met -1; i_met > 0; i_met--){
		last_possible = find_last_positive(meetings[i_met].possible_periods);
		if(last_possible == -1)
			continue;
		// no clone meeting can be in the 1st period of meetings[i] of
		// this meeting
		for(j_met = i_met-1; j_met >= 0; j_met--){
			bool is_clone =
				(meetings[i_met].teacher == meetings[j_met].teacher)
				&& (meetings[i_met].class   == meetings[j_met].class  );
			if(is_clone && meetings[j_met].possible_periods[last_possible] != 0){
				changed_something = true;
				meetings[j_met].possible_periods[last_possible] = 0;
			}
		}
	}
	return changed_something;
}

/* If the same teacher has two meetings with the same class,
 * all of them in the periods [1,2,3], we know that the
 * first can't be in the period 3, and the third can't be in the period 1
 * because they must coexist.
 */
bool eliminate_clone_meeting_period_options(Meeting * meetings) {
	bool changed_something = false;
	changed_something |= eliminate_left_clone_meeting_period_options(meetings);
	changed_something |= eliminate_right_clone_meeting_period_options(meetings);
	return changed_something;
}

/* If we just fixed that a meeting with class A and teacher P will
 * happen, we know that neither P nor A can be in another meeting
 * at the same time.
 */
bool propagate_meeting_fixation(Meeting * meetings, int i_fixed){
	int i_met;
	bool changed_something = false;
	Meeting fixed_meeting = meetings[i_fixed];
	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		// if the teacher is already allocated, no need to check anything
		if(i_met == i_fixed || meetings[i_met].period != -1){
			continue;
		} else {
			if( meetings[i_met].teacher == fixed_meeting.teacher
				|| meetings[i_met].class   == fixed_meeting.class
			){
				// Then the teacher doesn't have that period avalible
				// anymore.
				changed_something = true;
				meetings[i_met].possible_periods[ fixed_meeting.period ] = 0;
			}
		}
	}
	return changed_something;
}

/* Checks if there are meetings with just one possible period
 * and sets it to be. Also propagates meeting fixation.
 */
bool check_for_fixed_meetings(Meeting * meetings){
	int i_met = 0, length = 0, changed_something = false;
	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		if(meetings[i_met].period == -1){
			length = not_null_int_list_len(meetings[i_met].possible_periods);
			if(length == 1){
				changed_something = true;
				meetings[i_met].period = find_first_positive(meetings[i_met].possible_periods);
				propagate_meeting_fixation(meetings, i_met);
			} else if(length == 0){
				printf("It's not possible to make this meeting.\n");
			}
		}
	}
	return changed_something;
}

/* Based on a list of classes, generates the list of all meetings
 * that must happen with those classes. That includes the possible
 * periods that the meeting can happen, with some minor constraint
 * propagation.
 */
Meeting * initialize_all_meetings(ExtendedClass * classes){
 	int i_class = 0, i_teach = 0;
	int i_meeting = 0, i_quant = 0;
	int n_meeting = get_number_of_meetings(classes);
	Meeting * meetings = calloc( n_meeting + 1, sizeof(Meeting));
	for(i_class = 0; classes[i_class].name != NULL; i_class++){
		for(i_teach = 0; classes[i_class].teachers[i_teach].teacher != NULL; i_teach++){
			TeacherQuantity tq = classes[i_class].teachers[i_teach];
			for(i_quant = 0; i_quant < tq.quantity; i_quant++){
				meetings[i_meeting].teacher = tq.teacher;
				meetings[i_meeting].class   = &classes[i_class];
				meetings[i_meeting].period = -1;
				meetings[i_meeting].possible_periods =
					get_preliminary_meeting_score(
						meetings[i_meeting].teacher->periods,
						meetings[i_meeting].class->periods
				);
				i_meeting++;
			}
		}
	}
	bool change = true;
	while(change){
		change = false;
		change |= eliminate_clone_meeting_period_options(meetings);
		change |= check_for_fixed_meetings(meetings);
	}
	return meetings;
}

/* Creates an identical copy of the meetings list given,
 * while still retaining the original teacher and class pointers.
 */
Meeting * make_meetings_copy(const Meeting * const meetings){
	Meeting * copy;
	int n_met, i_met, i_per;
	for(n_met = 0; meetings[n_met].teacher != NULL; n_met++){
	}

	copy = calloc(n_met + 1, sizeof(Meeting));

	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		copy[i_met].teacher = meetings[i_met].teacher;
		copy[i_met].class   = meetings[i_met].class;
		copy[i_met].period = meetings[i_met].period;
		copy[i_met].possible_periods = calloc(1+int_list_len(meetings[i_met].possible_periods), sizeof(int));
		for(i_per =0; meetings[i_met].possible_periods[i_per] >= 0; i_per ++ ){
			copy[i_met].possible_periods[i_per] = meetings[i_met].possible_periods[i_per];
		}
		copy[i_met].possible_periods[i_per] = meetings[i_met].possible_periods[i_per];

	}
	return copy;
}

void destroy_meetings(Meeting * meetings){
	int i_met;
	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		free(meetings[i_met].possible_periods);
	}
	free(meetings);
}

/* Makes a quick check to see if this meeting list
 * is immediately contradictory.
 *
 * That is, provided that the meetings list was
 * created right,
 * - there is no teacher or class that has two meetings
 *   at the same time;
 * - there is no yet blank meeting with no possibility
 */
bool is_immediately_impossible(Meeting * meetings){
	int i_met = 0, j_met = 0;
	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		/* Return true if there is another meeting at the same time
		 * wih the same class or teacher
		 */
		for(j_met = i_met+1; meetings[j_met].teacher != NULL; j_met++){
			bool same_teach = meetings[i_met].teacher == meetings[j_met].teacher;
			bool same_class = meetings[i_met].class   == meetings[j_met].class;
			int per_i = meetings[i_met].period;
			int per_j = meetings[j_met].period;
			if((same_teach || same_class) && (per_i != -1) && (per_i == per_j)){
				return true;
			}
		}
		/* Return true if there is no option for that period. */
		if(meetings[i_met].period == -1
			&& find_first_positive(meetings[i_met].possible_periods) == -1){
			return true;
		}
	}

	return false;
}

bool seems_solved(Meeting * meetings){
	int i_met = 0;
	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		if(meetings[i_met].period == -1)
			return false;
	}
	return true;
}

#endif /* LOGIC_H */
