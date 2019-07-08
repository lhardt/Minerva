/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#include "planner.h"

void consistency_test_2(){
	// Possible schedule
	//        0   1   2
	// T1:    p1  p2  p3
	// T2:    p2  p3  p1
	// T3:    p3  p1  p2
	int p[3] = {0,1,2};

	Teacher t[3] = {
		{.name="Alexander", .periods=p},
		{.name="Basile", .periods=p},
		{.name="Karkov", .periods=p}
	};
	TeacherQuantity tq1[3] = {
		{.teacher=(&t[0]),.quantity=1},
		{.teacher=(&t[1]),.quantity=1},
		{.teacher=(&t[2]),.quantity=1},
	};
	Class c[3] = {
		{.teachers=tq1, .teachers_size=3, .name="DS3"},
		{.teachers=tq1, .teachers_size=3, .name="EL3"},
		{.teachers=tq1, .teachers_size=3, .name="AD3"}
	};
	Meeting partial_sol_test[9] = {
		{.period=0, .teacher=&t[0], .class=&c[0]},
		{.period=0, .teacher=&t[1], .class=&c[1]},
		{.period=0, .teacher=&t[2], .class=&c[2]},

		{.period=1, .teacher=&t[1], .class=&c[0]},
		{.period=1, .teacher=&t[2], .class=&c[1]},
		{.period=1, .teacher=&t[0], .class=&c[2]},

		{.period=2, .teacher=&t[2], .class=&c[0]},
		{.period=2, .teacher=&t[0], .class=&c[1]},
		{.period=2, .teacher=&t[1], .class=&c[2]}
	};

	print_solution(partial_sol_test, 9, 3);
	for(int i = 0; i < 9; i++){
		printf("CONSISTENCY TEST %3d: %s\n", i+1, (is_last_item_consistent(partial_sol_test, i, 3, 3, 3)?"True":"False"));
	}
}

void consistency_test_1(){
	int p[3] = {0,1,2};
	Teacher t[3] = {
		{.name="Alexander", .periods=p},
		{.name="Basile", .periods=p},
		{.name="Karkov", .periods=p}
	};
	TeacherQuantity tq1[3] = {
		{.teacher=(&t[0]),.quantity=1},
		{.teacher=(&t[1]),.quantity=1},
		{.teacher=(&t[2]),.quantity=1},
	};
	Class c[3] = {
		{.teachers=tq1, .teachers_size=3, .name="DS3"},
		{.teachers=tq1, .teachers_size=3, .name="EL3"},
		{.teachers=tq1, .teachers_size=3, .name="AD3"}
	};

	Meeting partial_sol_test[9] = {
		{.period=0, .teacher=&t[0], .class=&c[0]},
		{.period=0, .teacher=&t[1], .class=&c[1]},
		{.period=0, .teacher=&t[2], .class=&c[2]},

		{.period=1, .teacher=&t[0], .class=&c[0]},
		{.period=1, .teacher=&t[1], .class=&c[1]},
		{.period=1, .teacher=&t[2], .class=&c[2]},

		{.period=2, .teacher=&t[1], .class=&c[0]},
		{.period=2, .teacher=&t[2], .class=&c[1]},
		{.period=2, .teacher=&t[0], .class=&c[2]}
	};
	// This schedule:
	//        0   1   2
	// T1:    p1  p1  p2
	// T2:    p2  p2  p3
	// T3:    p3  p3  p1

	print_solution(partial_sol_test, 9, 3);
	for(int i = 0; i < 9; i++){
		printf("CONSISTENCY TEST %3d: \n", i+1);
		char * passed = (is_last_item_consistent(partial_sol_test, i, 3, 3, 3)?"True":"False");
		printf("Result : %s\n", passed);
	}
}

void solving_test_1(){
	uint64_t n_turmas = 3;
	uint64_t n_prof   = 3;
	uint64_t n_period = 4;

	int p[3] = {0,1,2};

	Teacher t[3] = {
		{.name="Alexander", .periods=p},
		{.name="Basile", .periods=p},
		{.name="Karkov", .periods=p}
	};
	TeacherQuantity tq1[3] = {
		{.teacher=(&t[0]),.quantity=1},
		{.teacher=(&t[1]),.quantity=1},
		{.teacher=(&t[2]),.quantity=1},
	};
	Class c[3] = {
		{.teachers=tq1, .teachers_size=3, .name="DS3"},
		{.teachers=tq1, .teachers_size=3, .name="EL3"},
		{.teachers=tq1, .teachers_size=3, .name="AD3"}
	};
	print_solution(solve(t,c,3,3,3), 9, 3);
}

int main(){
	solving_test_1();
	return 0;
}
