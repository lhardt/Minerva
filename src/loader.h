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

#include <sqlite3.h>

bool init_all_tables(FILE* console_out, char * db_filename);

bool insert_school(FILE * console_out, sqlite3* db, School * school);

bool  load_school(char * db_filename, int id, School ** load_ptr);
bool  load_all_schools(char * db_filename, School ** load_ptr);

bool  save_state(char * db_filename, School * school);

bool  load_xml(char * xml_filename, School ** load_ptr);
bool  export_to_xml(char * xml_filename, School * school);

#endif /* DATA_LOADER_H */