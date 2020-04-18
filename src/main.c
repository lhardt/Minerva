/*
 * Project Minerva.
 *   main.c - debbuging main function.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains functions that are not mandatory for making
 * a good timetable. However, in eliminting options sooner, it may
 * actually reduce time and memory spent by the computer.
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "assert.h"
#include "types.h"
#include "util.h"
#include "logic.h"
#include "decisions.h"
#include "solver.h"
#include "loader.h"

School * test_case_1(){

	School school;

	char * day_names[]    = {"Monday", "Friday", ""};
	char * daily_period_names[] = {"P1", "P2", "P3", ""};
	char * period_names[] = {"M1", "M2", "M3", "F1", "F2", "F3"};
	char * features[] = {"N. Tables"};
	char * teacher_names[] = {"T1", "T2", "T3", "T4", NULL};
	char * class_names[] = {"C1", "C2", "C3", NULL};
	int    perfect_periods[] = {1,1,1,1,1,1, -1};

	char * subject_names[] = {"Math", "Engl", "Germ", "Biol", ""};
	char *    room_names[] = {"R1", "R2", "R3", NULL};

	school.name = "TestCase1 School";
	school.n_periods = 6;
	school.n_days    = 2;
	school.n_periods_per_day = 3;
	school.max_meetings_teacher_per_week = 4;
	school.max_meetings_teacher_per_day  = 3;
	school.max_gemini_classes            = 2;
	school.day_names = day_names;
	school.daily_period_names = daily_period_names;
	school.period_names = period_names;
	school.teaches = calloc(5, sizeof(Teaches));

	school.n_features = 1;
	school.feature_names = features;

	Subject subjects[5];
	for(int i = 0; i < 4; i++){
		subjects[i] = (Subject){
				.id=i,
				.name=subject_names[i],
				.short_name=subject_names[i],
				.gemini_score = perfect_periods,
				.week_position_score = perfect_periods
		};
	}
	subjects[4] = (Subject){
		.id = -1,
		.name = NULL,
		.short_name = NULL
	};

	for(int i = 0; i < 4; i++){
		school.teaches[i] = (Teaches){
			.id=i,
			.teacher = &school.teachers[i],
			.subject = &subjects[i],
			.score=1,
			.min_features = {1}
		};
	}
	school.teaches[4] = (Teaches){
		.id = -1,
		.teacher = NULL,
		.subject = NULL,
	};


	school.n_teachers = 4;
	school.teachers = calloc(5, sizeof(Teacher));
	for(int i = 0; i < school.n_teachers; i++){
		school.teachers[i] = (Teacher){.id=i, .name=teacher_names[i],
				.short_name = teacher_names[i],
				.periods = perfect_periods,
				.max_meetings_per_day = 3,
				.max_meetings = 6,
				.num_planning_periods = 0,
				.teaches = calloc(2, sizeof(Subject*))
		};
		school.teachers[i].teaches[0] = &school.teaches[i];
		school.teachers[i].teaches[1] = NULL;
	}

	school.n_rooms = 3;
	school.rooms = calloc(4,sizeof(Room));
	for(int i = 0; i < 3; i++){
		school.rooms[i] = (Room){
			.id = i,
			.name = room_names[i],
			.short_name = room_names[i],
			.size = 10 + 10*i,
			.room_features = {1},
			.disponibility  = {1}
		};
		school.rooms[i].disponibility[school.n_periods] = -1;
	}
	school.rooms[3] = (Room){
		.id = -1,
		.name = NULL,
		.short_name = NULL,
		.size = -1,
	};

	SubjectQuantity class_needs[] = {
			{.subject=&subjects[0], .quantity=2},
			{.subject=&subjects[1], .quantity=1},
			{.subject=&subjects[2], .quantity=1},
			{.subject=&subjects[3], .quantity=1},
			{.subject=NULL}
	};

	school.n_classes = 3;
	school.classes = calloc(3, sizeof(Class));
	for(int i = 0; i < school.n_classes; i++){
		school.classes[i] = (Class){
			.id = i,
			.name = class_names[i],
			.short_name = class_names[i],
			.size = 10 + 10*i,
			// .periods = perfect_periods,
			.can_have_free_periods_flag = true,
			.minimal_exit_period = 0,
			.maximal_entry_period = 3,
			.needs = class_needs
		};
		int j;
		for(j = 0; perfect_periods[j] >= 0 ; j++){
			school.classes[i].periods[j] = perfect_periods[j];
		}
		school.classes[i].periods[j] = -1;
	}

	DecisionTree * tree = init_decision_tree(&school);

	int nmet = 0;
	while(tree->start[0].conclusion[nmet].class != NULL){
		nmet++;
	}
	printf("Nmet: %d\n", nmet);

	print_meeting_list(stdout, tree->start[0].conclusion);

	elim_analogous_ordering(&school, &tree->start[0]);

	bool change = true;
	while(change){
		change = false;
		change |= elim_search_fixed_meeting(&school, &tree->start[0]);
		change |= elim_general_super_room(&school, &tree->start[0]);
	}
	printf("\n\n");
	print_meeting_list(stdout, tree->start[0].conclusion);


	return NULL;
}

void test_insert_school(){
	init_all_tables(stdout, "db/test.db");

	School school;

	char * day_names[]    = {"Monday,", "Tuesday", "Wednsday", "Thrusday,", "Friday", ""};
	char * daily_period_names[] = {"M11", "M12", "M13", "M14", "M15", "T11", "T12", "T13", "T14", "T15"};
	char * period_names[] = {"M21", "M22", "M23", "M24", "M25", "T21", "T22", "T23", "T24", "T25",
						     "M31", "M32", "M33", "M34", "M35", "T31", "T32", "T33", "T34", "T35",
						     "M41", "M42", "M43", "M44", "M45", "T41", "T42", "T43", "T44", "T45",
  						     "M51", "M52", "M53", "M54", "M55", "T51", "T52", "T53", "T54", "T55"};
	char * features[] = {"Num. Computers", "N. Tables", "Projector", "Blackboard", "Ventilation", "Air Conditioning"};

	school.name = "PeriodSchool";
	school.n_periods = 50;
	school.n_days    = 5;
	school.n_periods_per_day = 10;
	school.max_meetings_teacher_per_week = 12;
	school.max_meetings_teacher_per_day  = 4;
	school.max_gemini_classes            = 3;
	school.day_names = day_names;
	school.daily_period_names = daily_period_names;
	school.period_names = period_names;

	school.n_features = 6;
	school.feature_names = features;



	sqlite3 * db;
	int errc = sqlite3_open("db/test.db", &db);
	if(errc != SQLITE_OK){
		printf("Errc %d %s\n", errc, sqlite3_errmsg(db));
	}

	insert_school(stdout, db, &school);
}

int main(){
	test_case_1();
	// LMH_ASSERT(false, "Assertion failed. ");

	return 0;
}
