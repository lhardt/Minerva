/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#ifndef DATA_SAVER_H
#define DATA_SAVER_H


extern const char * const CREATE_SCHOOL_TABLE;

extern const char * const CREATE_TEACHER_TABLE;

extern const char * const CREATE_CLASS_TABLE;

extern const char * const CREATE_TEACHER_CLASS_TABLE;

int database_execute(const char * filename, const char * const statement);


#endif /* DATA_SAVER_H */
