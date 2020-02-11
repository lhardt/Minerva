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
#include "data_saver.h"
#include "util.h"

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

	int t1_per[] = {3,1,1,1,1,5, -1};
	int t2_per[] = {3,1,1,0,1,5, -1};
	int t3_per[] = {3,1,1,1,1,5, -1};

	int c1_per[] = {3,1,1,1,1,5, -1};
	int c2_per[] = {3,1,1,1,1,5, -1};
	int c3_per[] = {3,1,1,1,1,5, -1};


	Teacher teachers[] = {
		{
			.name="Adolf",
			.periods=t1_per,
			.max_meetings_per_day=10
		},
		{
			.name="Joseph",
			.periods=t2_per,
			.max_meetings_per_day=10
		},
		{
			.name="Winston",
			.periods=t3_per,
			.max_meetings_per_day=10
		},
		{
			.name=NULL,
			.periods=NULL,
		}
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
			.quantity = 2
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
			.quantity = 1
		},
		{
			.teacher = (&teachers[2]),
			.quantity = 2
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

	School school = {
		.name = "E.E.E.F. Germannia",
		.teachers = teachers,
		.classes  = classes,
		.n_class_days = 1,
		.n_periods_per_day = 5
	};

	GuessNode * parent_node = init_guess_tree(&school);
	GuessNode * current_node;
	current_node = parent_node;
	int descent_list[50] = {-1};
	int descent_i = 0;

	while(current_node != NULL && ! current_node->solved ) {
		bool go_further = make_guess(&school,current_node);
		if(go_further){
			descent_list[descent_i]++;
			descent_i++;
			current_node = &current_node->children[descent_list[descent_i]];
		} else {
			if(descent_i == 0){
				printf("Impossible to solve\n");
				break;
			}
			descent_i--;
			current_node = current_node->parent;
		}
	}
	if(current_node == NULL){
		printf("Impossible to solve\n");
	} else {
		printf("-----------------------------\n%21s\n-----------------------------\n", "SOLUTION NODE");
		print_meeting_list(current_node->conclusion);

	}
	destroy_node_tree(parent_node);

	printf("\nNumber of guesses created: %d\n", g_CURRENT_ID);
}

int main(){
	// test_init_meetings();
	// database_execute("db/test.db", CREATE_SCHOOL_TABLE);
	// database_execute("db/test.db", CREATE_TEACHER_TABLE);
	// database_execute("db/test.db", CREATE_CLASS_TABLE);
	// database_execute("db/test.db", CREATE_TEACHER_CLASS_TABLE);


	int t1_per[] = {3,1,1,1,1,5, -1};
	int t2_per[] = {3,1,1,0,1,5, -1};
	int t3_per[] = {3,1,1,1,1,5, -1};

	int c1_per[] = {3,1,1,1,1,5, -1};
	int c2_per[] = {3,1,1,1,1,5, -1};
	int c3_per[] = {3,1,1,1,1,5, -1};


	Teacher teachers[] = {
		{
			.name="Adolf",
			.periods=t1_per,
			.max_meetings_per_day=10
		},
		{
			.name="Joseph",
			.periods=t2_per,
			.max_meetings_per_day=10
		},
		{
			.name="Winston",
			.periods=t3_per,
			.max_meetings_per_day=10
		},
		{
			.name=NULL,
			.periods=NULL,
		}
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
			.quantity = 2
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
			.quantity = 1
		},
		{
			.teacher = (&teachers[2]),
			.quantity = 2
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

	print_teacher(&teachers[1]);
	print_ex_class(&classes[0]);

	return 0;
}
