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

#include "types.h"

void print_int_list(FILE * out, const int * const list);

void print_sized_int_list(FILE * out, const int * const list, const int size);

void print_bool_list(FILE * out, const bool * const list, const int size);

void print_meeting_list(FILE * out, const Meeting * const meetings);

void print_room(FILE * out, const Room * const t);

void print_teacher(FILE * out, const Teacher * const t);

void print_class(FILE * out, const Class * const c);

void print_school(FILE * out, const School * const s);

#endif /* UTIL_H */
