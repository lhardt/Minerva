/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "util.h"
#include "decisions.h"
#include "primary_logic.h"
#include "data_loader.h"

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
	school.teaches = calloc(4, sizeof(Teaches));

	school.n_features = 1;
	school.feature_names = features;

	Subject subjects[4];
	for(int i = 0; i < 4; i++){
		subjects[i] = (Subject){
				.id=i,
				.name=subject_names[i],
				.short_name=subject_names[i],
				.gemini_score = perfect_periods,
				.week_position_score = perfect_periods
		};
	}

	school.n_teachers = 4;
	school.teachers = calloc(5, sizeof(Teacher));
	for(int i = 0; i < school.n_teachers; i++){
		school.teachers[i] = (Teacher){.id=i, .name=teacher_names[i],
				.short_name = teacher_names[i],
				.periods = perfect_periods,
				.max_meetings_per_day = 3,
				.max_meetings_per_week = 6,
				.num_planning_periods = 0,
				.teaches = calloc(1, sizeof(Subject*))
		};
		school.teachers[i].teaches[0] = &school.teaches[i];
	}

	school.rooms = NULL;

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
			.periods = perfect_periods,
			.can_have_free_periods_flag = true,
			.minimal_exit_period = 0,
			.maximal_entry_period = 3,
			.needs = class_needs
		};
	}

	DecisionTree * tree = init_decision_tree(&school);

	int nmet = 0;
	while(tree->start[0].conclusion[nmet].class != NULL)
		nmet++;
	printf("Nmet: %d\n", nmet);

	// for(int i = 0; i < school.n_classes; i++){
	// 	int number = count_required_meetings(&school, &school.classes[i], NULL);
	// 	printf("Class %d:%s Required Meetings %d\n",
	// 			i, (school.classes[i].name==NULL?("nul"):school.classes[i].name), number);
	//
	// 	for(int j = 0; j < 4; j++){
	// 		int number = count_required_meetings(&school, &school.classes[i], &subjects[j]);
	// 		printf("Class %d:%s Subject %d %s Required Meetings %d\n",
	// 				i, (school.classes[i].name==NULL?("nul"):school.classes[i].name),
	// 				j, subject_names[j], number);
	//
	// 	}
	// }

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
	// printf("Hello, world.\n");
	test_case_1();

	return 0;
}
