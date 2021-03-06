
#include <stdio.h>

#include "logic.h"

#include "assert.h"
#include "maths.h"
#include "util.h"

/* GET TEACHER SUBJECT SCORE
 *		Returns the score of teaches for every teacher for this subject.
 *
 *	Development status:
 *
 *	TODO:
 *
 * */
int * get_teacher_subject_score(const School * school, int id_subject){
	LMH_ASSERT(school != NULL && id_subject >= 0);
	int * list = calloc(school->n_teachers + 1, sizeof(int));
	for(int i = 0; i < school->n_teaches; ++i){
		if(school->teaches[i].subject->id == id_subject){
			int i_teacher = get_teacher_index_by_id(school, school->teaches[i].teacher->id);
			LMH_ASSERT(i_teacher >= 0);
			LMH_ASSERT(list[i_teacher] == 0);
			list[i_teacher] = school->teaches[i].score;
		}
	}
	return list;
}

/* DETECT TEACHER CIRCULAR SUBORDINATION
 *		Tries to detect if some teacher subordinates itself, which is illegal.
 *
 * Development status:
 *		Implemented, not tested.
 */
bool detect_teacher_circular_subordination(const School * const school){
	int i = 0, i_path = 0, j = 0;
	bool found = false, backtrack = false;
	bool * visited = calloc(school->n_teachers, sizeof(bool));
	int * path = calloc(1 + school->n_teachers, sizeof(int));

	LMH_ASSERT(school != NULL);

	path[school->n_teachers] = -1;
	/* Tries to visit twice some node starting at each of the remaining. */
	for(i = 0; !found && i < school->n_teachers; ++i){
		if(NULL == school->teachers[i].subordinates){
			continue;
		}
		/* Reseting of variables */
		for(j = 0; j < school->n_teachers; ++j){
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
			for(; j < school->n_teachers; ++j){
				if(NULL != school->teachers[j].subordinates
							&& school->teachers[ path[i_path] ].subordinates[j] > 0){
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

/* FLATTEN TEACHER SUBORDINATION
*		Includes sub-subordinates in teachers' subordinate list.
*
* Development status:
*		Implemented.
*/
bool flatten_teacher_subordination(School * school){
	int i = 0, i_path = 0, j = 0;
	bool backtrack = false;
	int * path = calloc(10 + school->n_teachers, sizeof(int));

	LMH_ASSERT(school != NULL);
	LMH_ASSERT(false == detect_teacher_circular_subordination(school));

	path[school->n_teachers] = -1;
	/* Tries to visit twice some node starting at each of the remaining. */
	for(i = 0; i < school->n_teachers; ++i){
		if(NULL == school->teachers[i].subordinates){
			continue;
		}
		/* Reseting of variables */
		for(j = 0; j < school->n_teachers; ++j){
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
					school->teachers[i].subordinates[j] += 1;
				}
			}
			/* Provided that this node has children, tries to find one.
			 * If could not, backtrack=true.
			 */
			backtrack = true;
			if(NULL != school->teachers[ path[i_path] ].subordinates){
				for(j= path[i_path+1] +1; j < school->n_teachers; ++j){
					if(school->teachers[ path[i_path] ].subordinates[j] > 0){
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

/* FLATTEN CLASS SUBORDINATION
 *		Includes sub-subordinates in classes' subordinate list.
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

	LMH_ASSERT(school != NULL);
	LMH_ASSERT(false == detect_class_circular_subordination(school));

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

	LMH_ASSERT(school != NULL);

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

/* ROOT CONSISTENCY CHECK
 *		Tries to detect problems before the timetable generation.
 *		Returns true if the test passed.
 *
 *	Development status:
 *		partially implemented.
 *	TODO:
 *		Implement some way to see if all lectures have periods.
 * */
bool root_consistency_check(const School * const school, DecisionNode * node){

	LMH_ASSERT(school != NULL && node != NULL);

	return detect_teacher_circular_subordination(school)? false:
		   detect_class_circular_subordination(school)?
		   false:true;
}

/* NOTE expects flattened relations */
bool have_teachers_relation(const School * const school, const Teacher * const t1, const Teacher * const t2){
	int result = 0;

	LMH_ASSERT(school != NULL && t1 != NULL && t2 != NULL);

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

/* NOTE expects flattened relations */
bool have_classes_relation(const School * const school, const Class * const c1, const Class * const c2){
	int result = 0;

	LMH_ASSERT(school != NULL && c1 != NULL && c2 != NULL);

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

/* COUNT REQUIRED MEETINGS
 *		Counts required meetings in a school with two optional parameters.
 *
 *		If class or subject is passed as NULL, counts with all classes/subjects.
 *
 * Development status:
 *		Implemented, not tested.
 */
int count_required_meetings(School * school,Class * class,Subject * subject){
	int count = 0, i_class = 0, i_need = 0;

	LMH_ASSERT(school != NULL);

	if(school->classes != NULL && school->n_classes > 0){
		for(i_class = 0; i_class < school->n_classes; ++i_class){
			if( ((class == NULL) != (&school->classes[i_class] == class)) && school->classes[i_class].active){
				for(i_need = 0; school->classes[i_class].assignments[i_need] != NULL; ++i_need){
					if((subject == NULL) != (school->classes[i_class].assignments[i_need]->subject == subject)){
						count += school->classes[i_class].assignments[i_need]->amount;
						printf("Count adding in %d\n", school->classes[i_class].assignments[i_need]->amount);
					}
				}
			}
		}
	} else {
		printf("Count called with invalid school\n");
	}
	printf("Counted %d required meetings.\n", count);
	return count;
}

/* IS NODE INCONSISTENT
 * 		Checks if a given node can not possibly be a solution.
 *		This simply means that a meeting doesn't have a period/room/teacher to be in.
 *
 * 		Returns true if node is invalid.
 *
 *		Note: this does not mean that on false being returned, this node is
 *		garanteed to have a solution.
 *
 * Development status:
 *		Implemented, not tested.
 */
bool is_node_inconsistent(const School * const school, const DecisionNode * const node){
	int i_met = 0;
	Meeting * ref_met;

	LMH_ASSERT(school != NULL && node != NULL);

	for(i_met = 0; node->conclusion[i_met].m_class != NULL; i_met++){
		ref_met = &node->conclusion[i_met];
		/* Values must be set or have possibilities to be in */
		if(    (ref_met->room == NULL	 && 0 == non_zero_int_list_count(ref_met->possible_rooms))
			|| (ref_met->teacher == NULL && 0 == non_zero_int_list_count(ref_met->possible_teachers))
			|| (ref_met->period == -1	 && 0 == non_zero_int_list_count(ref_met->possible_periods))
		){
			return true;
		}
	}
	return false;
}

/* IS NODE FINAL
 * 		Checks if a given node has every variable set.
 *		This simply means that all meetings have period, teacher and room.
 *
 * 		Returns true if node is final.
 *
 * Development status:
 *		Implemented, not tested.
 */
bool is_node_final(const School * const school, const DecisionNode * const node){
	int i = 0;
	Meeting  curr;

	LMH_ASSERT(school != NULL && node != NULL);

	for(i = 0; node->conclusion[i].m_class != NULL; ++i){
		curr = node->conclusion[i];
		if(curr.period < 0 || curr.room == NULL || curr.teacher == NULL){
			return false;
		}
	}
	return true;
}

bool is_final_node_inconsistent(const School * const school, const DecisionNode * const node){
	int i = 0;
}

/* ELIM ANALOGOUS ORDERING
 *		Eliminates possibilities of same period for analogous ordering.
 * 		Meetings are analogous if class == class && subj == subj
 *
 * Development Status:
 *		Implemented, not tested.
 */
bool elim_analogous_ordering(const School * const school, DecisionNode * node){
	int i_meet = 0, j_meet = 0, elim_asc = -1, elim_desc;
	bool change = false;

	LMH_ASSERT(school != NULL && node != NULL);
	/* Reference to shorten indirection */
	Meeting * meetings = node->conclusion;
	/* Ascending */
	for(i_meet = 0; meetings[i_meet].m_class != NULL; i_meet++){
		for(j_meet = i_meet+1; meetings[j_meet].m_class != NULL; j_meet++){
			/* If they are analogous and not fixed */
			if(   meetings[i_meet].m_class == meetings[j_meet].m_class
			   && meetings[i_meet].subject == meetings[j_meet].subject
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

/* ELIM SEARCH FIXED MEETING
 *		Tries to search for a meeting that has only one option for one resource.
 *		For each found, calls elim_fixed_meeting. If found any, returns true.
 *
 * Development status:
 *		Implemented, not tested.
 */
bool elim_search_fixed_meeting(const School * const school, DecisionNode * node){
	int i_meet = 0, found = 0;
	bool change = false;
	/* Reference to shorten indirection. */
	Meeting * meeting;

	LMH_ASSERT(school != NULL && node != NULL);

	int * changed_meetings = calloc(node->owner->n_meetings + 1, sizeof(int));
	changed_meetings[node->owner->n_meetings] = -1;

	for(i_meet = 0; node->conclusion[i_meet].m_class != NULL; i_meet++){
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

	for(i_meet = 0; node->conclusion[i_meet].m_class != NULL; i_meet++){
		if(changed_meetings[i_meet]){
			elim_fixed_meeting(school, node, i_meet);
		}
	}
	free(changed_meetings);
	return change;
}

/* ROOM PERIOD ELIMINATION
*		Tries to exclude periods based on room availability
*		and vice-versa.
*
* Development Status:
*		Implemented, trivially tested
*/
bool room_period_elimination(const School * const school, DecisionNode * node, int affected_i){
	int i_per = 0, i_room = 0;
	Meeting * met = NULL;
	bool change = false;

	LMH_ASSERT(school != NULL && node != NULL && affected_i >= 0);

	met = &node->conclusion[affected_i];
	if(met->room != NULL){
		if(met->period >= 0){
			if(met->room->availability[met->period] == 0){
				// Theoretically, the program blocks tihs possibility by hindsight.
				node->is_consistent = false;
				change = true;
				printf("Here1.\n");
				print_meeting_list(stdout, node->conclusion);
			}
		} else {
			for(i_per = 0; met->possible_periods[i_per] >= 0; ++i_per){
				// TODO make this a consistent standard among other eliminations.
				int tmp = met->possible_periods[i_per] * met->room->availability[i_per];
				if(tmp != met->possible_periods[i_per]){
					met->possible_periods[i_per] = tmp;
					change = true;
				}
			}
		}
	} else if(met->period >= 0){
		for(i_room = 0; i_room < school->n_rooms && met->possible_rooms[i_room] >= 0; ++i_room){
			met->possible_rooms[i_room] *= school->rooms[i_room].availability[ met->period ];
		}
	}
	return change;
}

/* TEACHER PERIOD ELIMINATION
*		Tries to exclude periods based on those of the teacher chosen.
*		and vice-versa
*
* Development Status:
*		Implemented, trivially tested
*/
bool teacher_period_elimination(const School * const school, DecisionNode * node, int affected_i){
	Meeting * met = NULL;
	bool change = false;

	LMH_ASSERT(school != NULL && node != NULL && affected_i >= 0);

	met = &node->conclusion[affected_i];
	if(met->teacher != NULL){
		if(met->period >= 0 && met->teacher->lecture_period_scores[ met->period ] == 0){
			node->is_consistent = false;
			printf("Here2.");
			return true;
		}
		int * tperiods = met->teacher->lecture_period_scores;
		for(int i = 0; i < school->n_periods; ++i){
			int tmp = (tperiods[i] > 0 ? 1 : 0) * met->possible_periods[i];
			if(met->possible_periods[i] != tmp){
				change = true;
				met->possible_periods[i] = tmp;
			}
		}
	} else if(met->period >= 0){
		for(int i = 0; i < school->n_teachers; ++i){
			if(school->teachers[i].lecture_period_scores[ met->period ] == 0){
				if(met->possible_teachers[i] != 0){
					met->possible_teachers[i] = 0;
					change = true;
				}
			}
		}
	}
	return change;
}

/* TEACHER ROOM ELIMINATION
*		Tries to exclude rooms based on the preferences of the chosen teacher.
*		and vice-versa
*
* Development Status:
*	Implemented
*/
bool teacher_room_elimination(const School * const school, DecisionNode * node, int affected_i){
	Meeting * met = NULL;
	bool change = false;
	LMH_ASSERT(school != NULL && node != NULL && affected_i >= 0);

	met = &node->conclusion[affected_i];
	if(met->teacher != NULL){
		LMH_ASSERT(met->teacher->lecture_room_scores != NULL);

		if(met->room != NULL){
			int i_this_room = get_room_index_by_id(school, met->room->id);
			if(met->teacher->lecture_room_scores[i_this_room] == 0){
				node->is_consistent = false;
				change = true;
			}
		} else {
			for(int i_room = 0; i_room < school->n_rooms; ++i_room){
				if(met->teacher->lecture_room_scores[i_room] == 0 && met->possible_rooms[i_room] > 0){
					change = true;
					met->possible_rooms[i_room] = 0;
				}
			}
		}
	} else if(met->room != NULL){
		int i_this_room = get_room_index_by_id(school, met->room->id);
		for(int i_teacher = 0; i_teacher < school->n_teachers; ++i_teacher){
			if(school->teachers[i_teacher].lecture_room_scores[i_this_room] == 0 && met->possible_rooms[i_this_room] > 0){
				met->possible_rooms[i_this_room] = 0;
				change = true;
			}
		}
	}

	return change;
}

/* ELIM FIXED MEETING
 *		Fixes a meeting that has only one possibility
 *
 * Development status:
 *		Implemented, not tested.
 */
bool elim_fixed_meeting(const School * const school, DecisionNode * node, const int fix_meet_i){
	int i_meet = 0;
	bool change = false;

	LMH_ASSERT(school != NULL && node != NULL && fix_meet_i >= 0);

	Meeting * fixed = &node->conclusion[fix_meet_i];
	Meeting * meeting;

	if(node->conclusion[fix_meet_i].room != NULL){
		room_period_elimination(school, node, fix_meet_i);
	}
	if(node->conclusion[fix_meet_i].teacher != NULL){
		teacher_period_elimination(school, node, fix_meet_i);
	}
	if(node->conclusion[fix_meet_i].teacher != NULL || node->conclusion[fix_meet_i].room != NULL){
		teacher_room_elimination(school, node, fix_meet_i);
	}

	for(i_meet = 0; node->conclusion[i_meet].m_class != NULL; i_meet++){
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
		if(meeting->m_class == fixed->m_class
				&& fixed->period != -1
				&& meeting->period == -1
				&& meeting->possible_periods[fixed->period] > 0){
			meeting->possible_periods[fixed->period] = 0;
			change = true;
		}
		/* Rule 3. R cannot be allocated at P by any other meeting */
		if(meeting->room == fixed->room
				&& meeting->room != NULL
				&& fixed->period != -1
				&& meeting->period == -1
				&& meeting->possible_periods[fixed->period] > 0){
			meeting->possible_periods[fixed->period] = 0;
			change = true;
		}
		/* Rule 4. T teaches C in all periods of S for C */
		if(meeting->m_class == fixed->m_class
				&& meeting->subject == fixed->subject
				&& meeting->teacher == NULL
				&& fixed->teacher != NULL){
			meeting->teacher = fixed->teacher;
			change = true;
		}
		/* Rule 5. R accomodates every period of S to C */
		if(meeting->m_class == fixed->m_class
				&& meeting->subject == fixed->subject
				&& meeting->room == NULL
				&& fixed->room != NULL){
			if(meeting->room != fixed->room){
				meeting->room = fixed->room;
				change = true;
				room_period_elimination(school, node, i_meet);
			}
		}
		/* Rule 6. No super/subordinate of T can lecture at P. */
		if(fixed->teacher != NULL
				&& fixed->period != -1
				&& meeting->period == -1
				&& meeting->teacher != NULL
				&& have_teachers_relation(school,meeting->teacher, fixed->teacher) != 0){
			meeting->possible_periods[fixed->period] = 0;
			change = true;
		}
		/* Rule 7. No super/subordinate of C can attend lectures at P. */
		if(fixed->period != -1
				&& meeting->period == -1
				&& have_classes_relation(school, meeting->m_class, fixed->m_class) != 0){
			meeting->possible_periods[fixed->period] = 0;
			change = true;
		}
	}
	return change;
}

/* SUPER ROOM RULE
 *
 * Basic Explanation:
 *   If N classes happen in room R, and the limit is K,
 *    then any other class that needs more than N-K periods can't
 *    happen in that room.
 *
 * Things to consider:
 * - "limit" above is not a single quantity. It may be
 *  max_per_day, max_per_week, etc. in this function, it means school->n_meetings.
 */
bool elim_general_super_room(School * school, DecisionNode * node){
	int i_meet, i_room;
	bool change = false;

	LMH_ASSERT(school != NULL && node != NULL);

	int * room_remainders = calloc(school->n_rooms + 1, sizeof(int));
	Meeting * meet;

	// int period_remainders[MAX_PERIODS_PER_WEEK + 1] = {0};

	for(i_room = 0; i_room < school->n_rooms; i_room++){
		room_remainders[i_room] = school->n_periods;
	}

	for(i_meet = 0; node->conclusion[i_meet].m_class != NULL; i_meet++){
		// for()
		meet = &node->conclusion[i_meet];
		if(meet->room != NULL){
			room_remainders[(meet->room - school->rooms)]--;
		}
	}
	room_remainders[school->n_rooms] = -1;

	for(i_meet = 0; node->conclusion[i_meet].m_class != NULL; i_meet++){
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

/* ROOT ELIMINATION
 * 		Blackbox functions for eliminating possibilities, for the initial
 *		node created.
 *
 * Development Status:
 *		Implemented.
 */
bool root_elimination(const School * const school, DecisionNode * node){
	LMH_ASSERT(school != NULL && node != NULL);
	printf("=======================================ROOT ELIMINATION .\n");
	/* TODO expand to encompass more restrictions */
	bool change = false;
	do {
		Meeting * meetings = node->conclusion;
		change = false;
		change |= elim_analogous_ordering(school,node);
		change |= elim_search_fixed_meeting(school,node);
		for(int i = 0; i < node->owner->n_meetings; ++i){
			change |= elim_fixed_meeting(school, node, i);
			change |= teacher_room_elimination(school, node, i);
			change |= room_period_elimination(school, node, i);
			change |= teacher_period_elimination(school, node, i);
		}
	}while(change == true && node->is_consistent);
	return true;
}

/* NEW NODE ELIMINATION
 * 		Blackbox functions for eliminating possibilities, based on the
 *		type of node created.
 *
 * Development Status:
 *		Implemented.
 */
bool new_node_elimination(const School * const school, DecisionNode * node){
	bool change = false, changed = false;

	LMH_ASSERT(school != NULL && node != NULL);

	switch(node->type){
		case NODE_TEACHER:{
			node->conclusion[node->affected_meeting_index].teacher = node->fixed_teacher;
			break;
		}
		case NODE_ROOM:{
			node->conclusion[node->affected_meeting_index].room = node->fixed_room;
			break;
		}
		case NODE_PERIOD:{
			node->conclusion[node->affected_meeting_index].period = node->fixed_period;
			break;
		}
		case NODE_START: {
			root_elimination(school, node);
		}
		case NODE_NULL_TYPE:{
			break;
		}
	}
	// Theese alter the scores, therefore don't fix anything.
	// Any fixation must be accompanied by a call to elim_fixed_meeting
	printf("Fixed meetign is %d\n", node->affected_meeting_index);
	elim_fixed_meeting(school, node, node->affected_meeting_index);
	room_period_elimination(school, node, node->affected_meeting_index);
	teacher_period_elimination(school, node, node->affected_meeting_index);
	teacher_room_elimination(school, node, node->affected_meeting_index);
	do{
		change = false;
		change  |= elim_search_fixed_meeting(school,node);

		changed |= change;
	}while(change == true);

	node->is_final = is_node_final(school, node);
	node->is_consistent = !is_node_inconsistent(school, node);
	return changed;
}
