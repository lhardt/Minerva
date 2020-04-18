/*
 * Project Minerva.
 *   logic.h - constraint propagation for timetabling.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains functions for making sure that all decision nodes
 * are valid. It also has some counting functions for utility.
 */
 #ifndef LOGIC_H
 #define LOGIC_H

 #include <stdlib.h>

 #include "decisions.h"

/** GENERAL PURPOUSE COUNTING FUNCTION.
 *
 * Counts the number of meetings that must happen in a school.
 * All parameters except for the first are optional.
 *
 * Default values:
 *	Teacher: NULL;
 *	Class: NULL;
 */
int count_required_meetings(School * school, Class * class, Subject * subject);

/**
 * Checks if a given node can not possibly be a solution.
 *
 * This simply means that no meeting can have no periods to be in.
 *
 */
bool is_node_invalid(School * school, DecisionNode * node);

/* PERIOD OVERFLOW RULE:
 *
 * Logic Type: Teacher-Subject-Class;
 *
 * Simp	le Explanation:
 *   If teacher A teachers subject D
 *   And class C needs subject D,
 *     If A has 4 free periods and C needs 5 periods of D,
 *       Then A cannot teach D to C.
 *
 * Things to consider:
 * - The 'free' periods are really the size of the intersection btwn free(A) and free(C);
 * - Consider max_per_day in the calculation
 */
bool elim_period_overflow(DecisionNode * node);

/* PLANNING DAY RULE:
 *
 * Logic Type: Teacher-Period;
 *
 * Simple Explanation:
 *   If teacher A needs 5 consecutive free periods
 *     And now only in periods X he has 5 periods free,
 *       Then A has the X periods as planning day.
 *
 * Things to consider:
 * -
 */
bool elim_planning_day(DecisionNode * node);

/* ANALOGOUS ORDERING RULE:
 *
 * Logic Type: Class-Teacher-Period;
 *
 * Simple Explanation:
 *   If teacher A teaches class C 3 times (L1, L2, L3) a week
 *       Then L2 and L3 can not happen in their first meeting;
 *            L1 and L2 can not happen in their last meeting;
 *            L1 can not happen in their second last meeting;
 *            L3 can not happen in their second meeting;
 *
 * Things to consider:
 * - This creates an order between periods;
 * - Therefore it must happen after any user interference (because messing with one period messes with all the other twins);
 * - But it may be reversed as L1.possible = L1.possible U L2.possible U L3.possible.
 * - make sure we do not include fixed meetings in the reversal union.
 */
bool elim_analogous_ordering(School * school, DecisionNode * node);
bool reverse_analogous_ordering(School * school, DecisionNode * node);

/* FIXED MEETING RULE:
 *
 * Logic Type: Period-*;
 *
 * Basic Explanation:
 *   If teacher T teaches subject S to class C at period P, in room R
 *      Then: 1 T cannot teach any other class at P;
 *          - 2 C cannot attend any other lecture at P;
 *		    - 3 R cannot be allocated at P by any other meeting;
 *			- 4 T teaches C in all periods of S for C;
 *			- 5 R accomodates every period of S to C;
 *			- 6 Any super/subordinate of T cannot lecture at P;
 *			- 7 Any super/subordinate of C cannot attend lectures at P.
 *
 * Things to consider:
 * - TODO This algorithm **should** work even if one of the parameters (A,C, or R)
 *        is not set. The exclusion, then, happens only partially.
 * - if the room
 */
bool elim_search_fixed_meeting(School * school, DecisionNode * node);
bool elim_fixed_meeting(School * school, DecisionNode * node, int fixed_meeting_index);

/* GOOD ROOM RULE - Room: TODO better name
 *
 * Logic Type: Room-Meeting;
 *
 * Basic Explanation:
 *   If class C demands X feautres for a teacher-discipline D,
 *       Then those classes cannot happen in any room without X features;
 *	 If class C has room R for a discipline D,
 *		 Then no teacher which needs more than those features cannot lecture D to C;
 *
 *
 * Things to consider:
 *  -
 */
bool elim_good_room(DecisionNode * node);

/* TEACHER MAX PER DAY RULE:
 *
 * Logic Type: Teacher-Period;
 *
 * Basic Explanation:
 *   If teacher T already teaches Nt classes in a day Dk and his maximum is Nmax:
 *       Then, for any group of non-allocated analogous meetings Mg(M1,M2, ...):
 *			then only more (Nmax-Ntk) meetings in Mg can happen in Dk (otherwise Nt would be bigger Nmax).
 *
 *			Consider the days that thome meetings can happen (D1, D2, ..., Dz)
 *				On D1, just the first (Nmax-Nt1) periods can happen
 *      	    On D2, just the first (Nmax-Nt1-NP(D1)) can happen,
 *      	    On D3, just the first (Nmax-Nt1-(NP(D1) + NP(D2))) can happen.
 *			It also happens in reverse order:
 *				On Dz, just the last (Nmax-Nt1) periods can happen
 *				On Dz-1, just the last (Nmax-Nt1-NP(Dz)) periods can happen
 *				On Dz-2, just the last (Nmax-Nt1-NP(Dz)-NP(Dz-1)) periods can happen
 *
 *			where NP(k) is the number of periods that those meetings can happen in Day K
 * 				(it is obvious that if NP(k) <= Nmax for any k)
 * Things to consider:
 *  -
 */
bool elim_teacher_max_per_day(School * school, DecisionNode * node);


/* GROUP MAX PER DAY RULE:
 *
 * Logic Type: Room-Class-Period;
 *
 * Basic Explanation:
 * 	 If class C has a max per day of group G of M,
 *			but M classes of G are already allocated,
 *		Then no more lectures of G can happen in that day
 *
 * Things to consider:
 * - The algorithm can be made to have the hindsight of the function above.
 */
bool elim_group_max_per_day(School * school, DecisionNode * node);

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
bool elim_general_super_room(School * school, DecisionNode * node);

bool elim_super_room_daily(School * school, DecisionNode * node, int day);

/* ROOT CONSISTENCY CHECK
 *
 * Returns false if it is garanteed that no timetable can be made.
 *
 * TODO: implement
 */
bool root_consistency_check(School * school, DecisionNode * node);

/* ROOT ELIMINATION
 *
 * Tries to eliminate, as much as possible, upfront.
 * Making this avoids entire branches of the tree to grow.
 *
 * TODO: Implement.
 */
bool root_elimination(School * school, DecisionNode * node);

/* NEW NODE ELIMINATIONS
 *
 * Blackbox functions for eliminating possibilities,
 * based on the type of node created.
 *
 * TODO: Implement.
 */
bool new_node_elimination(School * school, DecisionNode * node);
bool new_teacher_node_elimination(School * school, DecisionNode * node);
bool new_room_node_elimination(School * school, DecisionNode * node);
bool new_period_node_elimination(School * school, DecisionNode * node);

#endif /* LOGIC_H */
