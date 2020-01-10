/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#ifndef LOGIC_H
#define LOGIC_H

#include "definitions.h"

/* Returns the number of meetings to be added, considering
 * - the already present meeting list;
 * - the specific teacher;
 * - the specific class;
 *
 * In case of passing NULL, the search ignores the nullified parameter.
 *
 * WARNING: expects that iff class1 == class2, then &class1 = &class2;
 * */
int get_number_of_missing_meetings(ExtendedClass * ex, Meeting * meetings, Teacher * teacher, ExtendedClass * class);


/* Another general purpouse search method.
 * Searches within _meetings_ for :
 * - those whose period is in the interval [start, end)
 * 		- if end is -1, we assume it until the end of the meetings list
 * - those whose possible periods lie all in the interval [start,end)
 * - those with the same teacher as with_teacher;
 * - those with the same class as with_class;
 */
int get_number_of_meetings_within(Meeting * meetings, Teacher * with_teacher, ExtendedClass * with_class, int per_start, int per_end, int n_per);

/* Calculates how both the teacher and the class like, for every period,
 * that their meeting be on that period
 */
int * get_preliminary_meeting_score(int * teacher_score, int * class_score);

/* If the same teacher has two meetings with the same class,
 * all of them in the periods [1,2,3], we know that the
 * first can't be in the period 3, and the third can't be in the period 1
 * because they must coexist.
 */
bool eliminate_clone_meeting_period_options(Meeting * meetings);

/* Let's suppose that a teacher T has six clone meetings to attend,
 * and that he, at maximum, can attend four of them per day. It is
 * clear, then, that he cannot attend to the meetings 5 and 6 in
 * his first day in office.
 */
bool eliminate_clone_meeting_maximum_options(Meeting * meetings, Teacher * teachers, ExtendedClass * classes, int n_days, int n_periods_per_day);

/* If we just fixed that a meeting with class A and teacher P will
 * happen, we know that neither P nor A can be in another meeting
 * at the same time.
 */
bool propagate_meeting_fixation(Meeting * meetings, int i_fixed);

/* Checks if there are meetings with just one possible period
 * and sets it to be. Also propagates meeting fixation.
 */
bool check_for_fixed_meetings(Meeting * meetings);

bool explore_consequences(Meeting * meetings);

/* Based on a list of classes, generates the list of all meetings
 * that must happen with those classes. That includes the possible
 * periods that the meeting can happen, with some minor constraint
 * propagation.
 */
Meeting * initialize_all_meetings(ExtendedClass * classes, Teacher * teachers, Meeting * fixed_meetings, int n_days, int n_periods_per_day);

/* Makes a quick check to see if this meeting list
 * is immediately contradictory.
 *
 * That is, provided that the meetings list was
 * created right,
 * - there is no teacher or class that has two meetings
 *   at the same time;
 * - there is no yet blank meeting with no possibility
 */
bool is_immediately_impossible(Meeting * meetings);

/* Returns true if all meetings have a fixed period.
 *
 * Pressuposes that every choice made was valid.
 */
bool seems_solved(Meeting * meetings);

/* Frees the memory used both by make_meetings_copy or by
 * initialize_all_meetings.
 */
void destroy_meeting_list(Meeting * meetings);

/* Creates an identical copy of the meetings list given,
 * while still retaining the original teacher and class pointers.
 */
Meeting * make_meetings_copy(const Meeting * const meetings);



#endif /* LOGIC_H */
