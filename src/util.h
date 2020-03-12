/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */

#ifndef UTIL_H
#define UTIL_H

#include "types.h"

void print_int_list(const int * const list);

void print_meeting_list(const Meeting * const meetings);

void print_teacher(const Teacher * const t);

void print_ex_teacher(const ExtendedTeacher * const t);

void print_teacher_quantity(const TeacherQuantity * const tq);

void print_class(const Class * const c);

void print_ex_class(const ExtendedClass * const c);

void print_class_quantity(const ClassQuantity * const cq);

void print_school(const School * const s);


#endif /* UTIL_H */
