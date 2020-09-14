/*
 * Project Minerva.
 *   util.h - utility library for timetabling.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains functions for interacting with
 * the user or debugger via streams.
 */
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#include "types.h"
#include "decisions.h"
#include "school_examples.h"

/*********************************************************/
/*                     FREE Functions                    */
/*********************************************************/

void free_school(School * school);
void free_meetings_list(Meeting * list);
void free_node(DecisionNode * node);
void free_class(Class * c);
void free_room(Room * r);
void free_subject(Subject * s);
void free_teacher(Teacher * t);
void free_teaches(Teaches * t);
void free_meeting(Meeting * t);
void free_solution(Solution * s);

/*********************************************************/
/*                     COPY Functions                    */
/*********************************************************/

char * 			copy_string(const char * const string);
int * 			int_list_copy(const int * const list);
int * 			int_list_n_copy(const int * const list, int n);
Meeting * 		copy_meetings_list(const Meeting * const meetings);
School * 		copy_school(const School * const school);
SchoolExample * copy_example(const SchoolExample * const ex);
Solution * 		copy_soluiton(const Solution * const solution);

/*********************************************************/
/*                    PRINT Functions                    */
/*********************************************************/

void print_int_list(FILE * out, const int * const list);
void print_sized_int_list(FILE * out, const int * const list, const int size);
void print_bool_list(FILE * out, const bool * const list, const int size);
void print_meeting_list(FILE * out, const Meeting * const meetings);
void print_short_meeting_list(FILE * out, const Meeting * const meetings);
void print_room(FILE * out, const Room * const t);
void print_teacher(FILE * out, const Teacher * const t);
void print_class(FILE * out, const Class * const c);
void print_school(FILE * out, const School * const s);

/*********************************************************/
/*                 FIND AND GET Functions                */
/*********************************************************/

/* Returns nullptr or -1 on not founding */
int 			get_room_index_by_id(School * school, int id);
int 			get_class_index_by_id(School * school, int id);
int 			get_teacher_index_by_id(School * school, int id);
int 			get_subject_index_by_id(School * school, int id);
int 			get_per_index_by_id(School * school, int id);
int 			get_day_index_by_id(School * school, int id);
int 			get_daily_period_index_by_id(School * school, int id);
int 			get_subject_group_index_by_id(School * school, int id);
int 			get_solution_index_by_id(School * school, int id);
Room * 			find_room_by_id(School * school, int id);
Class * 		find_class_by_id(School * school, int id);
Teacher * 		find_teacher_by_id(School * school, int id);
Meeting * 		find_meeting_by_id(School * school, int id);
Subject * 		find_subject_by_id(School * school, int id);
Solution * 		find_solution_by_id(School * school, int id);
Assignment * 	find_assignment_by_class_subj_id(School * school, int id_class, int id_subj);
Assignment * 	find_assignment_by_id(School * school, int id);

/*********************************************************/
/*                 ADD AND REMOVE Functions              */
/*********************************************************/

bool can_remove_class(School * school, int id);
bool can_remove_room(School * school, int id);
bool can_remove_teacher(School * school, int id);
bool can_remove_subject(School * school, int id);

/* if *list_ptr == NULL, calloc. Else realloc */
void add_zeroes_to_score_list(int ** list_ptr, int n_old, int n_new);
void add_zero_to_score_list_at(int ** list_ptr, int n_old, int at);
void remove_from_int_list(int * list, int i_remove);

void school_teacher_add(School * school, const Teacher * const teacher);
void school_class_add(School * school, Class * c);
void school_subject_add(School * school, const Subject * const subject);
void school_room_add(School * school, const Room * const room);
// TODO needed? void school_meeting_add(School * school, Meeting * meet);
void school_subjectgroup_add(School * school, const char * const name, int id);
void school_solution_add(School * school, const Solution * const sol);

void school_class_remove(School * school, int class_i, bool must_delete);
void school_teacher_remove(School * school, int teacher_i, bool must_delete);
void school_subject_remove(School * school, int subject_i, bool must_delete);
void school_room_remove(School * school, int room_i, bool must_delete);
void school_meeting_remove(School * school, int meeting_i);
void school_subjectgroup_remove(School * school, int i);
void school_solution_remove(School * school, int solution_i, bool must_delete);
/* XXX: using must_delete = false and not storing the meetings associated will result in memory leakage */

void school_init_meeting_list(School * school);

void school_meeting_change(School * school, int meeting_i);

void school_on_preference_changed(School * school);

/*********************************************************/
/*                     OTHER Functions                   */
/*********************************************************/

Meeting * create_meeting_list_for_class(School * school, Class * c);
void school_meeting_list_add_and_bind(School * school, int i_class, Meeting * meetings);

#endif /* UTIL_H */
