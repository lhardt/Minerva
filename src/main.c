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
	ExtendedClass c[3] = {
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
	ExtendedClass c[3] = {
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
	ExtendedClass c[3] = {
		{.teachers=tq1, .teachers_size=3, .name="DS3"},
		{.teachers=tq1, .teachers_size=3, .name="EL3"},
		{.teachers=tq1, .teachers_size=3, .name="AD3"}
	};
	print_solution(solve(t,c,3,3,3), 9, 3);
}

void solving_test_2(){
	Teacher * teachers = calloc(2,sizeof(Teacher));
	ExtendedClass * classes = calloc(2, sizeof(ExtendedClass));
	// In this school, there are 3 periods per day.
	int periods[15] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
	// Teacher of the 1st year.
	teachers[0].name = "Aline";
	teachers[0].periods = periods;
	// Teacher of the 2nd year.
	teachers[1].name = "Carla";
	teachers[1].periods = periods;


	classes[0].name = "Primário";
	TeacherQuantity tPrimary;
	tPrimary.quantity = 3 * 5;
	tPrimary.teacher = &teachers[0];
	classes[0].teachers = &tPrimary;
	classes[0].teachers_size++;

	classes[1].name = "Secundário";
	TeacherQuantity tSecondary;
	tSecondary.quantity = 3 * 5;
	tSecondary.teacher = &teachers[1];
	classes[1].teachers = &tSecondary;
	classes[1].teachers_size++;

	Meeting * solution = solve(teachers,classes,2,2,3*5);
	if(solution != NULL){
		print_solution(solution, 2*5*3 , 3* 5);
		free(solution);
	} else {
		printf("Impossible to solve.");
	}
	free(classes);
	free(teachers);
}

void solving_test_3(){
	Teacher * teachers = calloc(3,sizeof(Teacher));
	ExtendedClass * classes = calloc(3, sizeof(ExtendedClass));
	// In this school, there are 4 periods per day.
	int * periods = get_first_order_int(4 * 5);
	// Teacher of English.
	teachers[0].name = "Eng";
	teachers[0].periods = periods;
	// Teacher of Spanish.
	teachers[1].name = "Spa";
	teachers[1].periods = periods;

	// Teacher of Mathematics. -- Cannot stay for the last period
	int p_euler[15] = {0,1,2,4,5,6,8,9,10,12,13,14,16,17,18};
	teachers[2].name = "Mat";
	teachers[2].periods = periods;

	TeacherQuantity teacherQuantity[3] = {
		{.teacher=&teachers[0], .quantity=10},
		{.teacher=&teachers[1], .quantity=5},
		{.teacher=&teachers[2], .quantity=5}
	};

	classes[0].name = "Turma A";
	classes[0].teachers = teacherQuantity;
	classes[0].teachers_size = 3;

	classes[1].name = "Turma B";
	classes[1].teachers = teacherQuantity;
	classes[1].teachers_size = 3;

	Meeting * solution = solve(teachers,classes,2,3,4*5);
	if(solution != NULL){
		print_solution(solution, 40, 20);
		free(solution);
	} else {
		printf("Impossible to solve.\n");
	}
	free(periods);
	free(classes);
	free(teachers);
}

void solving_test_4(){

}

void initializing_test_1(){
	int p1[4] = {0,1,2,-1};
	int p2[4] = {0,1,3,-1};

	Teacher t[3] = {
		{.name="Alexander", .periods=p1},
		{.name="Basile", .periods=p2},
		{.name="Karkov", .periods=p1}
	};
	TeacherQuantity tq1[3] = {
		{.teacher=(&t[0]),.quantity=2},
		{.teacher=(&t[1]),.quantity=2},
		{.teacher=(&t[2]),.quantity=1},
	};
	ExtendedClass c[3] = {
		{.teachers=tq1, .teachers_size=3, .name="DS3"},
		{.teachers=tq1, .teachers_size=3, .name="EL3"},
		{.teachers=tq1, .teachers_size=3, .name="AD3"}
	};

	see_options(t, c, 3, 3, 3);
	// Meeting * meetings = initialize_all_meetings(t,c,3);
	// for(int i = 0; meetings[i].teacher != NULL; i++){
		// printf("Meeting %d: %s  %-10s  %d\n", i, meetings[i].class->name, meetings[i].teacher->name, meetings[i].period);
	// }
}



int main(){
	initializing_test_1();
	return 0;
}
