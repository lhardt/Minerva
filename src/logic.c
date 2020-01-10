/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "combinatorics.h"
#include "definitions.h"
#include "logic.h"


void print_meeting(Meeting meeting, int i_met){
	printf("Meeting %d: T%s C%s P%d ", i_met, meeting.teacher->name, meeting.class->name, meeting.period);
	print_int_list(meeting.possible_periods);
}



int get_number_of_missing_meetings(ExtendedClass * ex, Meeting * meetings, Teacher * teacher, ExtendedClass * class){
	int n_met = 0, i_met = 0, i_tq = 0, i_class = 0;
	ExtendedClass * c_i;
	Teacher * t_i;

	if(ex == NULL){
		return 0;
	}
	/* Count all required meetings */
	for(i_class = 0; ex[i_class].name != NULL; i_class++){
		c_i = &(ex[i_class]);
		/* This class will only be counted if
		 * is the same as the search asked for
		 */
		if(class == NULL || c_i == class){
			for(i_tq = 0; c_i->teachers[i_tq].teacher != NULL; i_tq ++){
				t_i = c_i->teachers[i_tq].teacher;
				/* This teacher will only be counted if
				 * is the same as the search asked for
				 */
				 if(teacher == NULL || t_i == teacher){
					 n_met += c_i->teachers[i_tq].quantity;
				}
			}
		}
	}
	/* Then discount all the already registered */
	if(meetings != NULL){
		for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
			c_i = (meetings[i_met].class);
			t_i = (meetings[i_met].teacher);

			if((class == NULL || c_i == class) && (teacher == NULL || t_i == teacher )){
				n_met--;
			}
		}
	}
	return n_met;
}

/* TODO: test. */
int get_number_of_meetings_within(Meeting * meetings, Teacher * with_teacher, ExtendedClass * with_class, int per_start, int per_end, int n_per){
	int i_met, i_per, n_met = 0, ctr = 0;

	if(per_end == -1){
		per_end = n_per;
	}
	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		if((with_teacher == NULL || with_teacher == meetings[i_met].teacher)
		   && ((with_class == NULL || with_class == meetings[i_met].class))){
			if(meetings[i_met].period != -1){
				if( per_start <= meetings[i_met].period && per_end > meetings[i_met].period){
					ctr++;
				}
			} else {
				for(i_per = 0; i_per < per_start; i_per++){
					/* If that meeting is not inside the constraint*/
					if(meetings[i_met].possible_periods[i_per] != 0){
						continue;
					}
				}

				for(i_per = per_end; meetings[i_met].possible_periods[i_per] >= 0; i_per++){
					/* If that meeting is not inside the constraint*/
					if(meetings[i_met].possible_periods[i_per] != 0){
						continue;
					}
				}
				ctr++;
			}
		}
	}
	return ctr;
}

/* Calculates how both the teacher and the class like, for every period,
 * that their meeting be on that period
 */
int * get_preliminary_meeting_score(int * teacher_score, int * class_score){
	int i, *meeting_score, list_size = 31;

	meeting_score = calloc(32, sizeof(int));

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
			list_size += 32;
			meeting_score = realloc(meeting_score,list_size * sizeof(int));
		}
	}
	meeting_score[i] = -1;
	return meeting_score;
}

/* If the same teacher has two meetings with the same class,
 * all of them in the periods [1,2,3], we know that the
 * first can't be in the period 3, and the third can't be in the period 1
 * because they must coexist.
 */
bool eliminate_clone_meeting_period_options(Meeting * meetings) {
	int i_met, j_met, first_possible, last_possible;
	bool changed_something = false;

	/* Searching for left clones */
	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		first_possible = find_first_positive(meetings[i_met].possible_periods);
		if(first_possible == -1)
			continue;
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
	/* Searching for right clones */
	for(i_met = i_met -1; i_met > 0; i_met--){
		last_possible = find_last_positive(meetings[i_met].possible_periods);
		if(last_possible == -1)
			continue;
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

/* Let's suppose that a teacher T has six clone meetings to attend,
 * and that he, at maximum, can attend four of them per day. It is
 * clear, then, that he cannot attend to the meetings 5 and 6 in
 * his first day in office.
 */
 bool eliminate_clone_meeting_maximum_options(Meeting * meetings, Teacher * teachers,
	 										  ExtendedClass * classes, int n_days, int n_periods_per_day){
	int i_day = 0, i_teach = 0, i_class = 0, i_met = 0, k = 0, i_per = 0, i_clone;
	bool changed_something;
	// just remembering that if we do the filtertering of
	// possibilities in chronological order, we can do
	// period = day1, then period = day2, etc. without worrying
	// about past periods
	for(i_teach = 0; teachers[i_teach].name != NULL; i_teach++){
		for(i_class = 0; classes[i_class].periods != NULL; i_class++){

			k = get_number_of_meetings_within(meetings, &teachers[i_teach], &classes[i_class], 0, -1, n_days * n_periods_per_day);

			i_day = 0;
			while(k > teachers[i_teach].max_meetings_per_day){
				k -= n_periods_per_day;
				// K last *clone* meetings can't be in this day
				// but there can be a fixed meeting out of order here,
				// so we need to filter that too.
				for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
					if(meetings[i_met].teacher == &teachers[i_teach] &&
					   meetings[i_met].class   == &classes[i_class] &&
					   (meetings[i_met].period == -1 || meetings[i_met].period < (k))){
						   i_clone++;
					}

					if(i_clone > k){
						for(i_per = 0; i_per < n_periods_per_day; i_per++){
							if(meetings[i_met].possible_periods[i_per] != 0){
								meetings[i_met].possible_periods[i_per] = 0;
								changed_something = true;
								printf("changed something");
							}
						}
					}
				}
				i_day++;
			}
		}
	}
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
	/* Just garantee that this function was not called by accident; */
	if(fixed_meeting.period != -1){
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
	} else {
		printf("propagate_meeting_fixation called by accident!\n");
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
				// printf("It's not possible to make this meeting.\n");
			}
		}
	}
	return changed_something;
}

bool explore_consequences(Meeting * meetings){
	bool change = true;
	eliminate_clone_meeting_period_options(meetings);
	while(change){
		change = false;
		change |= check_for_fixed_meetings(meetings);
	}
	return change;
}

/* Based on a list of classes, generates the list of all meetings
 * that must happen with those classes. That includes the possible
 * periods that the meeting can happen, with some minor constraint
 * propagation.
 */
Meeting * initialize_all_meetings(ExtendedClass * classes, Teacher * teachers, Meeting * fixed_meetings, int n_days, int n_periods_per_day){
 	int i_class = 0, i_teach = 0;
	int i_meeting = 0, i_quant = 0;
	int n_meeting = get_number_of_missing_meetings(classes, NULL, NULL, NULL);
	Meeting * meetings = calloc( n_meeting + 1, sizeof(Meeting));

	if(fixed_meetings != NULL){
		/* Copying the fixed meetings before. */
		for(i_meeting = 0; fixed_meetings[i_meeting].teacher != NULL; i_meeting++){
			meetings[i_meeting] = fixed_meetings[i_meeting];
		}
	}

	for(i_class = 0; classes[i_class].name != NULL; i_class++){
		for(i_teach = 0; classes[i_class].teachers[i_teach].teacher != NULL; i_teach++){
			TeacherQuantity tq = classes[i_class].teachers[i_teach];
			tq.quantity = get_number_of_missing_meetings(classes, meetings, tq.teacher, &classes[i_class]);
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
	explore_consequences(meetings);

	eliminate_clone_meeting_maximum_options(meetings, teachers,classes, n_days, n_periods_per_day);
	return meetings;
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

/* Returns true if all meetings have a fixed period.
 *
 * Pressuposes that every choice made was valid.
 */
bool seems_solved(Meeting * meetings){
	int i_met = 0;
	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		if(meetings[i_met].period == -1)
			return false;
	}
	return true;
}

/* Frees the memory used both by make_meetings_copy or by
 * initialize_all_meetings.
 */
void destroy_meeting_list(Meeting * meetings){
	int i_met;
	for(i_met = 0; meetings[i_met].teacher != NULL; i_met++){
		free(meetings[i_met].possible_periods);
	}
	free(meetings);
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
