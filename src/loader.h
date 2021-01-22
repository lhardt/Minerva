/*
 * Project Minerva.
 *   loader.h - SQL data loader utility module.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains functions for saving and loading data
 * from SQL databases related to school timetabling.
 */
#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "types.h"

#include <stdio.h>
#include <sqlite3.h>

/* INIT ALL TABLES
 *		Initializes, if they don't exist, all tables in the file.
 *		Also a convenient way of opening a database.
 *
 * Development Status:
 *		Implemented.
 */
bool init_all_tables(FILE* console_out, sqlite3* db);

/*	INSERT SCHOOL
 *		Inserts a school in the database, disregarding null pointers.
 * 		If school.classes = null, then does not insert classes, for ex.
 *
 * Development Status:
 *		Implemented
 **/
int insert_school(FILE * console_out, sqlite3* db, School * school);

/* Use -1 as an id in case you don't have one yet */
int insert_class(FILE * console_out, sqlite3* db, Class * c, School * school, int optional_id);
int insert_teacher(FILE * console_out, sqlite3* db, Teacher * t, School * school, int optional_id);
// int insert_teaches(FILE * console_out, sqlite3* db, Teaches * t, School * school, int optional_id);
bool insert_or_update_teaches(FILE * console_out, sqlite3* db, Teaches * t, School * school);
bool insert_or_update_assignment(FILE * console_out, sqlite3 * db, Assignment * assignment, School * school);
int insert_room(FILE * console_out, sqlite3* db, Room * room, School * school, int optional_id);
// int insert_meeting(FILE * console_out, sqlite3* db, Meeting * meet, School * school, int optional_id);
bool insert_meetings_list(FILE * console_out, sqlite3* db, Meeting * meetings, School * school);
int insert_subject(FILE * console_out, sqlite3* db, Subject * subject, School * school, int optional_id);
int insert_subject_in_group(FILE * console_out,sqlite3 * db, int subj_id, int group_id);
int insert_subject_group(FILE * console_out,sqlite3 * db, School * school, char * group_name, int optional_id);
int insert_solution(FILE * console_out, sqlite3 * db, School * school, Solution * sol, int optional_id);

bool remove_class(FILE * console_out, sqlite3* db, int id);
bool remove_teacher(FILE * console_out, sqlite3* db, int id);
bool remove_teaches(FILE * console_out, sqlite3* db, int id);
bool remove_room(FILE * console_out, sqlite3* db, int id);
bool remove_meeting(FILE * console_out, sqlite3* db, int id);
bool remove_subject(FILE * console_out, sqlite3* db, int id);
bool remove_subject_group(FILE * console_out, sqlite3* db, int id);
bool remove_school(FILE * console_out, sqlite3* db, int id);
bool remove_solution(FILE * console_out, sqlite3* db, int id);

bool update_school_name(FILE * console_out, sqlite3 * db, int id, char * name);
bool update_school_period_scores(FILE * console_out, sqlite3 * db, int n_periods, int * period_ids, int * scores);
bool update_day_names(FILE * console_out, sqlite3 * db, int n_days, int * day_ids, char ** names);
bool update_daily_period_names(FILE * console_out, sqlite3 * db, int n_days, int * day_ids, char ** names);
bool update_period_names(FILE * console_out, sqlite3 * db, int n_days, int * day_ids, char ** names);
bool update_room_basic_data(FILE * console_out, sqlite3* db, int room_id, Room * room, School * school);
bool update_room_availability(FILE * console_out, sqlite3* db, int room_id, int * availability, School * school);
bool update_subject_basic_data(FILE * console_out, sqlite3 * db, int subj_id, Subject * subject, School * school);
bool update_teacher_basic_data(FILE * console_out, sqlite3 * db, Teacher * teacher, School * school);
bool update_class_max_per_day_subjectgroup(FILE * console_out, sqlite3 * db, int class_id, int id_group, int max);
bool update_subject_group_name(FILE * console_out, sqlite3 * db, int sgr_id, char * new_name);
bool update_subject_group_members(FILE * console_out, sqlite3 * db, int sgr_id, int * members, School * school);
bool update_room_class_score(FILE * console_out, sqlite3 * db, int room_id, int * scores,  School * school);
bool update_room_teacher_score(FILE * console_out, sqlite3 * db, int room_id, int * plan_scr, int * lec_scr,  School * school);
bool update_room_teaches_score(FILE * console_out, sqlite3 * db, int room_id, int * scores, School * school);
bool update_room_meeting_score(FILE * console_out, sqlite3 * db, int room_id, int * scores, School * school);
bool update_teacher_assignment_score(FILE * console_out, sqlite3 * db, int teacher_id, int * scores, School * school);
bool update_teacher_meeting_fixation(FILE * console_out, sqlite3 * db, int teacher_id, bool * fixed, School * school);
bool update_teacher_day_max_per(FILE * console_out, sqlite3 * db, int id_teacher, int * scores, School * school);

char** select_all_school_names(FILE * console_out, sqlite3* db, int ** ids);
School * select_school_by_id(FILE * console_out, sqlite3* db, int id);

bool save_backup(sqlite3 * memory_db, const char * const filename);
bool load_backup(sqlite3 * memory_db, const char * const filename);

#endif /* DATA_LOADER_H */
