/*
 * Project Minerva.
 *   school_examples.h - examples for testing / demonstrating.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This file contains examples of schools that can be loaded
 * in the program and tested both by the user and the developer.
 */
#include "school_examples.h"

#include <stdlib.h> /* Definition on NULL */
#include <stdbool.h>
#include <string.h>

#include "util.h"

// void free_school_example(SchoolExample * ex){
//
// }
//
// SchoolExample * make_example_1(){
// 	char * day_names[] = {"OnlyDay", NULL};
// 	char * daily_period_names[] = {"OnlyPPD", NULL};
// 	char * period_names[] = {"OnlyPer", NULL};
//
// 	Subject subjects[] = {
// 		{
// 			.id = 1,
// 			.name = "OnlySubj",
// 			.short_name = "OS",
// 			.in_groups = {-1},
// 		}
// 	};
// 	int t1_periods[] = {1,-1};
// 	Teacher teachers[] = {
// 		{
// 			.id = 1,
// 			.name = "OnlyTeacher",
// 			.short_name = "O.T.",
// 			.max_days = 1,
// 			.max_meetings = 1,
// 			.max_meetings_per_day = 1,
// 			.max_meetings_per_class_per_day = 1,
// 			.num_planning_periods = 0,
// 			.day_max_meetings = {1,-1},
// 			.day_scores = {1,-1},
// 			// .teaches = t1_teaches,
// 			.possible_classes = NULL,
// 			.periods = &t1_periods[0],
// 			.rooms = NULL
// 		}
// 	};
//
// 	Teaches teaches[] = {
// 		{
// 			.id = 1,
// 			.teacher = &teachers[0],
// 			.subject = &subjects[0],
// 			.features = {0,-1},
// 			.min_features = {0,-1}
// 		},
// 		{
// 			.id = -1,
// 			.teacher = NULL,
// 			.subject = NULL
// 		}
// 	};
//
// 	Teaches *t1_teaches[] = {
// 		&teaches[0], NULL
// 	};
//
// 	teachers[0].teaches = t1_teaches;
//
// 	Room rooms[] = {
// 		{
// 			.id = 1,
// 			.name = "OnlyRoom",
// 			.short_name = "O.R.",
// 			.size = 5,
// 			.room_features = {1,-1},
// 			.disponibility = {1,-1}
// 		}
// 	};
//
// 	SubjectQuantity c_needs[] = {
// 		{
// 			.subject = &subjects[0],
// 			.quantity = 1
// 		}, {
// 			.subject = NULL,
// 			.quantity = -1
// 		}
// 	};
//
// 	Class classes[] = {
// 		{
// 			.id = 1,
// 			.name = "OnlyClass",
// 			.short_name = "O.C.",
// 			.size=5,
// 			.periods={1,-1},
// 			.rooms = NULL,
// 			.can_have_free_periods_flag = false,
// 			.maximal_entry_period = 0,
// 			.minimal_exit_period = 0,
// 			.abstract = false,
// 			.subordinates = NULL,
// 			.needs = c_needs
// 		},
// 		{
// 			.id = -1,
// 			.name = NULL
// 		}
// 	};
//
//
//
// 	SchoolExample ex = {
// 		.name = "One Example",
// 		.school = {
// 			.id = 1,
// 			.name = "One School",
//
// 			.n_periods = 1,
// 			.n_features = 0,
// 			.n_classes = 1,
// 			.n_teachers = 1,
// 			.n_subjects = 1,
// 			.n_rooms = 1,
// 			.n_teaches = 0,
// 			.n_days = 1,
// 			.n_periods_per_day = 1,
// 			.n_solutions = 0,
// 			.n_subject_groups = 0,
//
// 			.day_names = day_names,
// 			.daily_period_names = daily_period_names,
// 			.period_names = period_names,
//
// 			.feature_names=NULL,
// 			.subject_group_names=NULL,
//
// 			.classes = classes,
// 			.teaches = teaches,
// 			.teachers = teachers,
// 			.rooms = rooms
// 		},
// 		.n_possible_solutions = 1,
//
// 	};
//
// 	Meeting sol1_meetings[] = {
// 		{
// 			.m_class = &ex.school.classes[0],
// 			.teacher = &ex.school.teachers[0],
// 			.room = &ex.school.rooms[0],
// 			.subj = &ex.school.subjects[0],
// 			.period = 0
// 		},
// 		{
// 			.m_class = NULL
// 		}
// 	};
//
// 	Solution sol[] = {
// 		{
// 			.id = 1,
// 			.name = NULL,
// 			.desc = NULL,
// 			.meetings = sol1_meetings
// 		}
// 	};
//
// 	ex.possible_solutions = sol;
//
// 	/* Copy while everything is on the stack */
// 	return copy_example(&ex);
// }
