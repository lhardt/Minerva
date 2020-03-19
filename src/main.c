/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "decisions.h"
#include "util.h"
#include "data_loader.h"

int main(){
	// printf("Hello, world.\n");
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
		return 1;
	}

	insert_school(stdout, db, &school);

	return 0;
}
