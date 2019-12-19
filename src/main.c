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
#include "guesswork.h"

void print_meeting_list(Meeting * meetings){
	int i = 0;
	while(meetings[i].teacher != NULL){
		printf("Meeting %2d: %-5s %-9s ", i, meetings[i].class->name, meetings[i].teacher->name);
		if(meetings[i].period != -1){
			printf("%d\n", meetings[i].period);
		} else {
			printf("[");
			for(int j = 0; meetings[i].possible_periods[j] >= 0; j++){
				printf("%d, ", meetings[i].possible_periods[j]);
			}
			printf("]\n");
		}

		i++;
	}
}


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
	int days_len = str_list_len(days);
	for(i = 0; i < days_len; i++){
		u.days[i] = calloc(1+ strlen(days[i]), sizeof(char));
		strcpy(u.days[i],days[i]);
	}

	u.periods = calloc(7, sizeof(char *));
	int periods_len = str_list_len(periods);
	for(i = 0; i < periods_len; i++){
		u.periods[i] = calloc(1 + strlen(periods[i]), sizeof(char));
		strcpy(u.periods[i],periods[i]);
	}
	return u;
}

void test_init_meetings(){

	int t1_per[] = {1,1,1,1,1, -1};
	int t2_per[] = {2,1,1,0,1, -1};
	int t3_per[] = {1,1,1,1,1, -1};

	int c1_per[] = {1,1,1,1,1, -1};
	int c2_per[] = {1,1,1,1,1, -1};
	int c3_per[] = {1,1,1,1,1, -1};


	Teacher teachers[] = {
		{
			.name="Adolf",
			.periods=t1_per
		},
		{
			.name="Joseph",
			.periods=t2_per
		},
		{
			.name="Winston",
			.periods=t3_per
		},
		NULL
	};

	TeacherQuantity c1_tq[] = {
		{
			.teacher = (&teachers[0]),
			.quantity = 3
		},
		{
			.teacher = (&teachers[1]),
			.quantity = 1
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

	TeacherQuantity c2_tq[] = {
		{
			.teacher = (&teachers[0]),
			.quantity = 1
		},
		{
			.teacher = (&teachers[1]),
			.quantity = 1
		},
		{
			.teacher = (&teachers[2]),
			.quantity = 3
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
		{
			.name="DS2",
			.teachers = c2_tq,
			.periods = c2_per
		},
		{
			.name="DS3",
			.teachers = c3_tq,
			.periods = c3_per
		},
		{
			.name=NULL
		}
	};

	Meeting * meets = initialize_all_meetings(classes);

	print_meeting_list(meets);
	printf("Seems immediately impossible? %s.\n", (is_immediately_impossible(meets))?("Yes"):("No"));
	printf("Seems solved? %s.\n", (seems_solved(meets))?("Yes"):("No"));

	order_by_score_discrepancy(meets);

	destroy_meetings(meets);

}

int main(){
	test_init_meetings();
	// int rank[] = {1,-1};
	// int list[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	// order_by_rank(list,rank);
	// printf("Rank: ");
	// print_int_list(rank);
	// printf("List: ");
	// print_int_list(list);
	return 0;
}
