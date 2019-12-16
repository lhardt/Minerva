/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "definitions.h"
#include "logic.h"

Universe new_universe(){
	const char * const days[] = {
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
		"Sun",
		NULL
	};

	const char * const periods[] = {
		"1st",
		"2nd",
		"3rd",
		"4th",
		"5th",
		"6th",
		NULL
	};

	Universe u;
	int i = 0;

	u.days = calloc(8, sizeof(char *));
	int days_len = list_len(days);
	for(int i = 0; i < days_len; i++){
		u.days[i] = calloc(1+ strlen(days[i]), sizeof(char));
		strcpy(u.days[i],days[i]);
	}

	u.periods = calloc(7, sizeof(char *));
	int periods_len = list_len(periods);
	for(int i = 0; i < periods_len; i++){
		u.periods[i] = calloc(1 + strlen(periods[i]), sizeof(char));
		strcpy(u.periods[i],periods[i]);
	}
	return u;
}

void test_init_meetings(){

	int t1_per[3] = {1,1,1};


	Teacher teachers[] = {
		{
			.name="Adolf",
		},
		{
			.name="Joseph",
		}
	};

	TeacherQuantity c1_tq[] = {
		{
			.teacher = (&teachers[0]),
			.quantity = 3
		},
		{
			.teacher = (&teachers[1]),
			.quantity = 2
		},
	};

	TeacherQuantity c2_tq[] = {
		{
			.teacher = (&teachers[1]),
			.quantity = 1
		},
		{
			.teacher = (&teachers[0]),
			.quantity = 3
		},
	};

	ExtendedClass classes[] = {
		{
			.name="DS1",
			.teachers_size = 2,
			.teachers = c1_tq
		},
		{
			.name="DS2",
			.teachers_size = 2,
			.teachers = c2_tq
		},
		{
			.name="DS3",
			.teachers_size = 1,
			.teachers = c1_tq
		}
	};

	Meeting * meets = initialize_all_meetings(classes,3);
	// printf("N_Meetings: %d\n", n_meet);
	print_meeting_list(meets);

}

int main(){
	Universe u = new_universe();

	test_init_meetings();
	return 0;
}
