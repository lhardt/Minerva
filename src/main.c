/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "definitions.h"
#include "combinatorics.h"
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

	int t1_per[4] = {1,1,3, -1};
	int t2_per[4] = {1,3,1, -1};
	int t3_per[4] = {3,1,1, -1};

	int c1_per[4] = {1,1,1, -1};
	int c2_per[4] = {1,1,1, -1};
	int c3_per[4] = {1,1,1, -1};


	Teacher teachers[] = {
		{
			.name="Adolf",
			.periods=t1_per
		},
		// {
		// 	.name="Joseph",
		// 	.periods=t2_per
		// },
		// {
		// 	.name="Winston",
		// 	.periods=t3_per
		// },
		NULL
	};

	TeacherQuantity c1_tq[] = {
		{
			.teacher = (&teachers[0]),
			.quantity = 3
		},
		// {
		// 	.teacher = (&teachers[1]),
		// 	.quantity = 2
		// },
		// {
		// 	.teacher = (&teachers[2]),
		// 	.quantity = 1
		// },
		{
			.teacher = NULL,
			.quantity = 0
		}
	};

	TeacherQuantity c2_tq[] = {
		{
			.teacher = (&teachers[0]),
			.quantity = 1
		},
		{
			.teacher = (&teachers[1]),
			.quantity = 3
		},
		{
			.teacher = (&teachers[2]),
			.quantity = 1
		},
		{
			.teacher = NULL,
			.quantity = 0
		}
	};

	TeacherQuantity c3_tq[] = {
		{
			.teacher = (&teachers[0]),
			.quantity = 1
		},
		{
			.teacher = (&teachers[1]),
			.quantity = 3
		},
		{
			.teacher = (&teachers[2]),
			.quantity = 1
		},
		{
			.teacher = NULL,
			.quantity = 0
		}
	};

	ExtendedClass classes[] = {
		{
			.name="DS1",
			.teachers = c1_tq,
			.periods = c1_per
		},
		// {
		// 	.name="DS2",
		// 	.teachers = c2_tq,
		// 	.periods = c2_per
		// },
		// {
		// 	.name="DS3",
		// 	.teachers = c3_tq,
		// 	.periods = c3_per
		// },
		{
			.name=NULL
		}
	};

	Meeting * meets = initialize_all_meetings(classes);
	print_meeting_list(meets);

}

int main(){

	int l1[] = {1,2,3,4,5,-1};

	// int index = find_last_positive(l1);
	// printf("Last Positive Index:%d\n", index);

	Universe u = new_universe();
	test_init_meetings();
	return 0;
}
