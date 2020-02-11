/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#include <stdio.h>

#include "util.h"
#include "definitions.h"

void print_int_list(const int * const list){
	int i = 0;
	printf("[");
	while(list[i] != -1){
		printf("%2d", list[i]);
		if(list[i+1] != -1){
			printf(", ");
		}
		i++;
	}
	printf("]");
}

void print_meeting_list(const Meeting * const meetings){
	int i = 0;
	while(meetings[i].teacher != NULL){
		printf("Meeting %2d: %-5s %-9s ", i, meetings[i].class->name, meetings[i].teacher->name);
		if(meetings[i].period != -1){
			printf("%d", meetings[i].period);
		} else {
			printf("[");
			for(int j = 0; meetings[i].possible_periods[j] >= 0; j++){
				printf("%d, ", meetings[i].possible_periods[j]);
			}
			printf("]");
		}
		printf("\n");

		i++;
	}
}

void print_teacher(const Teacher * const t){
	printf("Teacher(%d)[\n\tName: %s;\n\tPeriods: ", t->id, t->name);
	print_int_list(t->periods);
	printf(";\n];\n");
}

void print_teacher_quantity(const TeacherQuantity * const tq){
	printf("TeacherQuantity[%s, %d]", tq->teacher->name,  tq->quantity);
}


void print_ex_teacher(const ExtendedTeacher * const t){
	printf("ExTeacher(%d)[\n\tName: %s;\n\tPeriods: ", t->id, t->name);
	print_int_list(t->periods);
	printf(";\n\tCQuantities: [\n\t");
	for(int i = 0; t->classes[i].class != NULL; i++){
		printf("\t");
		print_class_quantity(&t->classes[i]);
		printf(",\n\t");
	}
	printf("];\n];\n");
}

void print_class(const Class * const c){
	printf("Class(%d)[\n\tName: %s;\n\tPeriods: ", c->id, c->name);
	print_int_list(c->periods);

	printf(";\n];\n");
}

void print_class_quantity(const ClassQuantity * const cq){
	printf("ClassQuantity[%s, %d]", cq->class->name,  cq->quantity);
}

void print_ex_class(const ExtendedClass * const c){
	printf("ExClass(%d)[\n\tName: %s;\n\tPeriods: ", c->id, c->name);
	print_int_list(c->periods);
	printf(";\n\tTQuantities: [\n\t");
	for(int i = 0; c->teachers[i].teacher != NULL; i++){
		printf("\t");
		print_teacher_quantity(&c->teachers[i]);
		printf(",\n\t");
	}
	printf("];\n];\n");
}
