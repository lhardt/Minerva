/*
 * Project Minerva.
 *   preprocess.h - timetable preprocessing.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains functions to foresee
 * and change how the timetable will be made.
 */
#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "types.h"

void school_init_meeting_list(School * school);

void school_feature_add(School * school, const char * const feature_name);
void school_feature_remove(School * school, int feature_i);

void school_teacher_add(School * school, const Teacher * const teacher);
void school_teacher_remove(School * school, int teacher_i);

void school_class_add(School * school, const Class * const c);
void school_class_remove(School * school, int class_i);

void school_subject_add(School * school, Subject * subject);
void school_subject_remove(School * school, int subject_i);

void school_room_add(School * school, Teacher * teacher);
void school_room_remove(School * school, int room_i);

void school_meeting_add(School * school, Meeting * meet);
void school_meeting_change(School * school, int meeting_i);
void school_meeting_remove(School * school, int meeting_i);

void school_on_preference_changed(School * school);

#endif /* PREPROCESS_H */