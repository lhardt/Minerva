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

int insert_class(FILE * console_out, sqlite3* db, Class * c, School * school);
int insert_teacher(FILE * console_out, sqlite3* db, Teacher * t, School * school);
int insert_teaches(FILE * console_out, sqlite3* db, Teaches * t, School * school);
int insert_feature(FILE * console_out, sqlite3* db, const char * const feature, School * school);
int insert_room(FILE * console_out, sqlite3* db, Room * room, School * school);
int insert_meeting(FILE * console_out, sqlite3* db, Meeting * meet, School * school);
int insert_subject(FILE * console_out, sqlite3* db, Subject * subject, School * school);
int insert_subject_in_group(FILE * console_out,sqlite3 * db, int subj_id, int group_id);
int insert_subject_group(FILE * console_out,sqlite3 * db, School * school, char * group_name);
int insert_solution(FILE * console_out, sqlite3 * db, School * school, Solution * sol);

bool remove_class(FILE * console_out, sqlite3* db, int id);
bool remove_teacher(FILE * console_out, sqlite3* db, int id);
bool remove_teaches(FILE * console_out, sqlite3* db, int id);
bool remove_feature(FILE * console_out, sqlite3* db, int id);
bool remove_room(FILE * console_out, sqlite3* db, int id);
bool remove_meeting(FILE * console_out, sqlite3* db, int id);
bool remove_subject(FILE * console_out, sqlite3* db, int id);
bool remove_subject_group(FILE * console_out, sqlite3* db, int id);
bool remove_school(FILE * console_out, sqlite3* db, int id);

char** select_all_school_names(FILE * console_out, sqlite3* db, int ** ids);
School * select_school_by_id(FILE * console_out, sqlite3* db, int id);

bool save_backup(sqlite3 * memory_db, const char * const filename);
bool load_backup(sqlite3 * memory_db, const char * const filename);

#endif /* DATA_LOADER_H */
