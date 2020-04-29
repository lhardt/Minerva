
#include <stdio.h>

#include "logic.h"

#include "assert.h"
#include "util.h"
#include "maths.h"


bool detect_teacher_circular_subordination(School * school){
	return false;
}

/* FLATTEN CLASS SUBORDINATION
 *		A preprocessing tool before the core functions run.
 *
 * Returns true on success.
 *
 * Warning: this function must be called *after* detect_circular.
 *
 * Development Status:
 *		Implemented. tested.
 */
bool flatten_class_subordination(School * school){
	int i = 0, i_path = 0, j = 0;
	bool backtrack = false;
	int * path = calloc(10 + school->n_classes, sizeof(int));

	LMH_ASSERT(school != NULL, "nul par");
	LMH_ASSERT(false == detect_class_circular_subordination(school), "nul par");

	path[school->n_classes] = -1;
	/* Tries to visit twice some node starting at each of the remaining. */
	for(i = 0; i < school->n_classes; ++i){
		if(NULL == school->classes[i].subordinates){
			continue;
		}
		/* Reseting of variables */
		for(j = 0; j < school->n_classes; ++j){
			path[j] = -1;
		}
		i_path = -1;
		backtrack = false;
		j = i;
		/* Depth-frst  search in the 'subordination tree' */
		while( true ){
			if(backtrack){
				if(i_path > 0) {
					path[i_path+1] = -1;
					i_path--;
				} else {
					break;
				}
			} else {
				path[++i_path] = j;
				if(i_path > 1){
					school->classes[i].subordinates[j] += 1;
				}
			}
			/* Provided that this node has children, tries to find one.
			 * If could not, backtrack=true.
			 */
			backtrack = true;
			if(NULL != school->classes[ path[i_path] ].subordinates){
				for(j= path[i_path+1] +1; j < school->n_classes; ++j){
					if(school->classes[ path[i_path] ].subordinates[j] > 0){
						backtrack = false;
						break;
					}
				}
			}
		}
	}
	free(path);
	return true;
}

/* DETECT CLASS CIRCULAR SUBORDINATION
 *		Tries to detect if some class subordinates itself, which is illegal.
 *
 * Development Status:
 *		Implemented, tested.
 */
bool detect_class_circular_subordination(const School * const school){
	int i = 0, i_path = 0, j = 0;
	bool found = false, backtrack = false;
	bool * visited = calloc(school->n_classes, sizeof(bool));
	int * path = calloc(1 + school->n_classes, sizeof(int));

	LMH_ASSERT(school != NULL, "nul par");

	path[school->n_classes] = -1;
	/* Tries to visit twice some node starting at each of the remaining. */
	for(i = 0; !found && i < school->n_classes; ++i){
		if(NULL == school->classes[i].subordinates){
			continue;
		}
		/* Reseting of variables */
		for(j = 0; j < school->n_classes; ++j){
			visited[j] = false;
			path[j] = -1;
		}
		i_path = -1;
		backtrack = false;
		j = i;
		/* Depth-frst  search in the 'subordination tree' */
		while( !found ){
			if(backtrack){
				if(i_path > 0) {
					visited[path[i_path]] = false;
					i_path--;
				} else {
					break;
				}
			} else {
				visited[j] = true;
				path[++i_path] = j;
			}
			backtrack = true;
			/* Tries to next/first child index (j). Backtrack flag = true if could not.
			 * The trick here is that we know that every j < i was already visited
			 * because of the for() loop we are in.
			 */
			j = (path[i_path+1]+1 >= i)?path[i_path+1]+1:i;
			for(; j < school->n_classes; ++j){
				if(NULL != school->classes[j].subordinates
							&& school->classes[ path[i_path] ].subordinates[j] > 0){
					if(visited[j]){
						path[++i_path] = j;
						found = true;
					}
					backtrack = false;
					break;
				}
			}
		}
	}
	free(visited);
	free(path);
	return found;
}

bool root_consistency_check(School * school, DecisionNode * node){
	return true;
}

/* NOTE: expects plain relationships, not composite ones
 * If t1 subordinates t2, returns +1
 * If t2 subordinates t1, returns -1
 * Else returns  0
 */
bool have_teachers_relation(School * school, Teacher * t1, Teacher * t2){
	int result = 0;
	int t1_index = (t1 - school->teachers);
	int t2_index = (t2 - school->teachers);
	if(t1->subordinates && t1->subordinates[t2_index] > 0) {
		result = 1;
	}
	if(t1->subordinates && t1->subordinates[t1_index] > 0) {
		result = -1;
	}
	return result;
}

/* NOTE: expects plain relationships, not composite ones
 * If c1 subordinates c2, returns +1
 * If c2 subordinates c1, returns -1
 * Else returns  0
 */
bool have_classes_relation(School * school, Class * c1, Class * c2){
	int result = 0;
	int c1_index = (c1 - school->classes);
	int c2_index = (c2 - school->classes);

	if(c1->subordinates && c1->subordinates[c2_index] > 0){
		result = 1;
	}
	if(c2->subordinates && c2->subordinates[c1_index] > 0){
		result = -1;
	}

	return result;
}

int count_required_meetings(
				School * school,
				Class * class,
				Subject * subject
				){
	int count = 0, i_class = 0, i_need = 0;

	LMH_ASSERT(school != NULL, "null par");

	if(school->classes != NULL && school->n_classes > 0){
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
	int i_meet = 0, j_meet = 0, elim_asc = -1, elim_desc;
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

				elim_asc = find_first_positive(meetings[i_meet].possible_periods);
				if(meetings[j_meet].possible_periods[elim_asc] > 0){
					meetings[j_meet].possible_periods[elim_asc] = 0;
					change = true;
				}
				elim_desc = find_last_positive(meetings[j_meet].possible_periods);
				if(meetings[i_meet].possible_periods[elim_desc] > 0){
					meetings[i_meet].possible_periods[elim_desc] = 0;
					change = true;
				}
			}
		}
	}
	return change;
}

bool elim_search_fixed_meeting(School * school, DecisionNode * node){
	int i_meet = 0, found = 0;
	bool change = false;
	/* Reference to shorten indirection. */
	Meeting * meeting;

	int * changed_meetings = calloc(node->owner->n_meetings + 1, sizeof(int));
	changed_meetings[node->owner->n_meetings + 1] = -1;

	for(i_meet = 0; node->conclusion[i_meet].class != NULL; i_meet++){
		meeting = &node->conclusion[i_meet];
		/* Check for fixed teachers */
		if(meeting->teacher == NULL && 1 == non_zero_int_list_count(meeting->possible_teachers)){
			meeting->teacher = &school->teachers[find_first_positive(meeting->possible_teachers)];
			++changed_meetings[i_meet];
			change = true;
		}
		/* Check for fixed rooms */
		if(meeting->room == NULL && 1 == non_zero_int_list_count(meeting->possible_rooms)){
			found = find_first_positive(meeting->possible_rooms);
			meeting->room = &school->rooms[found];
			++changed_meetings[i_meet];
			change = true;
		}
		/* Check for fixed periods */
		if(meeting->period == -1 && 1 == non_zero_int_list_count(meeting->possible_periods)){
			found = find_first_positive(meeting->possible_periods);
			meeting->period = found;
			++changed_meetings[i_meet];
			change = true;
		}
	}

	for(i_meet = 0; node->conclusion[i_meet].class != NULL; i_meet++){
		if(changed_meetings[i_meet]){
			elim_fixed_meeting(school, node, i_meet);
		}
	}
	return change;
}

bool elim_fixed_meeting(School * school, DecisionNode * node, int fix_meet_i){
	int i_meet = 0;
	bool change = false;

	Meeting * fixed = &node->conclusion[fix_meet_i];
	Meeting * meeting;

	for(i_meet = 0; node->conclusion[i_meet].class != NULL; i_meet++){
		if(i_meet == fix_meet_i){
			continue;
		}
		meeting = &node->conclusion[i_meet];
		/* Rule 1. T cannot teach any other class at P */
		if(meeting->teacher == fixed->teacher
				&& meeting->teacher != NULL
				&& fixed->period != -1
				&& meeting->period == -1
				&& meeting->possible_periods[fixed->period] > 0){
			meeting->possible_periods[fixed->period] = 0;
			change = true;
		}
		/* Rule 2. C cannot attend any other lecture at P */
		if(meeting->class == fixed->class
				&& fixed->period != 1
				&& meeting->period == -1
				&& meeting->possible_periods[fixed->period] > 0){
			meeting->possible_periods[fixed->period] = 0;
			change = true;
		}
		/* Rule 3. R cannot be allocated at P by any other meeting */
		if(meeting->room == fixed->room
				&& meeting->room != NULL
				&& fixed->period != 1
				&& meeting->period == -1
				&& meeting->possible_periods[fixed->period] > 0){
			meeting->possible_periods[fixed->period] = 0;
			change = true;
		}
		/* Rule 4. T teaches C in all periods of S for C */
		if(meeting->class == fixed->class
				&& meeting->subj == fixed->subj
				&& meeting->teacher == NULL
				&& fixed->teacher != NULL){
			meeting->teacher = fixed->teacher;
			change = true;
		}
		/* Rule 5. R accomodates every period of S to C */
		if(meeting->class == fixed->class
				&& meeting->subj == fixed->subj
				&& meeting->room == NULL
				&& fixed->room != NULL){
			meeting->room = fixed->room;
			change = true;
		}
		/* Rule 6. Any super/subordinate of T cannot lecture at P. */
		if(fixed->teacher != NULL
				&& fixed->period != -1
				&& meeting->period == -1
				&& meeting->teacher != NULL
				&& have_teachers_relation(school,meeting->teacher, fixed->teacher) != 0){
			meeting->possible_periods[fixed->period] = 0;
			change = true;
		}
		/* Rule 7. Any super/subordinate of C cannot attend lectures at P. */
		if(fixed->period != -1
				&& meeting->period == -1
				&& have_classes_relation(school, meeting->class, fixed->class) != 0){
			meeting->possible_periods[fixed->period] = 0;
			change = true;
		}
	}
	return change;
}

bool elim_general_super_room(School * school, DecisionNode * node){
	int i_meet, i_room;
	bool change = false;

	int * room_remainders = calloc(school->n_rooms + 1, sizeof(int));
	Meeting * meet;

	// int period_remainders[MAX_PERIODS_PER_WEEK + 1] = {0};

	for(i_room = 0; i_room < school->n_rooms; i_room++){
		room_remainders[i_room] = school->n_periods;
	}

	for(i_meet = 0; node->conclusion[i_meet].class != NULL; i_meet++){
		// for()
		meet = &node->conclusion[i_meet];
		if(meet->room != NULL){
			room_remainders[(meet->room - school->rooms)]--;
		}
	}
	room_remainders[school->n_rooms] = -1;

	for(i_meet = 0; node->conclusion[i_meet].class != NULL; i_meet++){
		meet = &node->conclusion[i_meet];
		if(meet->room == NULL){
			for(i_room = 0; i_room < school->n_rooms; i_room++){
				if( meet->possible_rooms[i_room] >=  room_remainders[i_room]){
					change = true;
					meet->possible_rooms[i_room] = 0;
				}
			}
		}
	}
	return change;
}
