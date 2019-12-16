/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#ifndef PLANNER_H
#define PLANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "definitions.h"
#include "combinatorics.h"


void print_solution(Meeting * solution, uint64_t n_sol, uint64_t n_per){
	if(solution != NULL){
		for(int i = 0; i < n_per; i++){
			printf("%-4d: ", i);
			for(int j = 0; j < n_sol; j++){
				if(solution[j].period == i){
					printf("|%-10s ", solution[j].teacher->name);
					printf("%-5s |", solution[j].class->name);
				}
			}
			printf("\n");
		}
	} else {
		printf("Cannot print null solution;\n");
	}
}

uint64_t get_number_of_meetings(){}

// TODO: Test and substitute the next function for a call to this one.
bool is_meeting_consistent(Meeting m, Meeting * schedule){
	uint64_t last_quantity = -1;
	uint64_t given_lectures = 1;
	// Finding the corresponding teacher-class pair.
	for(int i = 0; i < m.class->teachers_size; i++){
		if(m.class->teachers[i].teacher == m.teacher){
			last_quantity = m.class->teachers[i].quantity;
			break;
		}
	}
	if(last_quantity != -1){
		for(int i = 0; schedule[i].teacher != NULL; i++){
			if(schedule[i].period != -1){

				bool sameTeacher = m.teacher == schedule[i].teacher;
				bool sameClass   = m.class == schedule[i].class;
				bool samePeriod  = m.period == schedule[i].period;

				if((sameClass || sameTeacher) && samePeriod){
					return false;
				}
				if(sameTeacher && sameClass){
					given_lectures++;
				}
			}
		}
		// printf("\nreturning lect %d %d\n", last_quantity, given_lectures);
		return last_quantity >= given_lectures;
	}
	return false;
}

/* If the addition of the last item of the list violates no constraint,
 * considering the other elements.
 */
bool is_last_item_consistent(
			  Meeting  * schedule,
		 	  uint64_t i_last,
		 	  uint64_t n_turmas,
			  uint64_t n_prof,
		   	  uint64_t n_period){
	Meeting last = schedule[i_last];
	uint64_t last_quantity = -1;
	uint64_t given_lectures = 1;
	// Finding the corresponding teacher-class pair.
	for(int i = 0; i < last.class->teachers_size; i++){
		if(last.class->teachers[i].teacher == last.teacher){
			last_quantity = last.class->teachers[i].quantity;
			break;
		}
	}
	// Finding if the teacher was free in that period
	bool avalible = false;
	for(int i = 0; last.teacher->periods[i] >= 0; i++){
		if(last.teacher->periods[i] == last.period){
			avalible = true;
			break;
		}
	}
	if(avalible && last_quantity != -1){
		for(int i = 0; i < i_last; i++){
			bool sameTeacher = (last.teacher == schedule[i].teacher);
			bool sameClass   = (last.class == schedule[i].class);
			bool samePeriod  = (last.period == schedule[i].period);

			if((sameClass || sameTeacher) && samePeriod){
				return false;
			}
			if(sameTeacher && sameClass){
				given_lectures++;
			}
		}
		return last_quantity >= given_lectures;
	}
	return false;
}

/* Checks if each and every class has no more
 * required classes than periods avalible.
 *
 * Similarly for teachers.
 */
bool check_sanity(
			  Teacher * teachers,
			  ExtendedClass * classes,
			  uint64_t n_classes,
			  uint64_t n_teach,
		  	  uint64_t n_per){
	int i_class = 0, i_cteach=0, class_sum=0, i_teach = 0;
	// Quantity of periods each teacher will have to attend.
	int * teacher_sum = calloc(n_teach, sizeof(Teacher));
	bool sane = true;
	for(i_class = 0; i_class < n_classes; i_class++){
		class_sum = 0;
		for(i_cteach = 0; i_cteach < classes[i_class].teachers_size; i_cteach++){
			// The sum of periods for this particular class.
			class_sum += classes[i_class].teachers[i_cteach].quantity;
			// Pointer arithmetic: as we know, all teachers are in the teachers vector.
			// So it must be the case that any TeacherQuantity has a pointer to an item
			// of that list. So we can find the index subtracting pointers.
			i_teach = ((Teacher*)classes[i_class].teachers[i_cteach].teacher)-teachers;
			// The sum of periods for this particular teacher.
			teacher_sum[i_teach] += classes[i_class].teachers[i_cteach].quantity;
		}
		// If this class has more periods with teachers than in the school
		if(class_sum > n_per){
			sane = false;
			break;
		}
	}
	for(i_teach = 0; i_teach < n_teach; i_teach++){
		if(teacher_sum	[i_teach] > n_per){
			sane = false;
			break;
		}
	}
	free(teacher_sum);
	return sane;
}

Meeting* solve(
			  Teacher * teachers,
			  ExtendedClass * classes,
			  uint64_t n_classes,
			  uint64_t n_teach,
		  	  uint64_t n_per){
	bool sane = check_sanity(teachers, classes, n_classes, n_teach, n_per);
	if(sane){
		int i_meet = 0, i_per = 0, i_class = 0;
		Meeting * solution = calloc(n_per * n_classes * n_teach, sizeof(Meeting));
		uint64_t ** orders = calloc(n_per, sizeof(uint64_t *));
		while(i_per < n_per && i_per >= 0){
			// TODO: if n_teach > n_classes (and it almost certainly is),
			// getting the next order gives more redundancy
			// Which makes a already terrible algorithm into a worse one
			if(orders[i_per] == NULL){
				orders[i_per] = get_first_order(n_teach);
			} else {
				// Backtracking mechanism
				bool hasNext = get_next_order(orders[i_per], n_teach);
				// If it needs to backtrack further;
				if(! hasNext){
					free(orders[i_per]);
					orders[i_per] = NULL;
					i_per--;
					i_meet -= n_classes;
					for(int i = 0; i < n_classes; i++){
						solution[i_meet + i].class= 0;
						solution[i_meet + i].teacher = 0;
						solution[i_meet + i].period = 0;
					}
					continue;
				}
			}
			// Assignment of that order to the corresponding variables;
			for(i_class = 0; i_class < n_classes; i_class++){
				solution[i_meet + i_class].class   = &classes[i_class];
				solution[i_meet + i_class].teacher = &teachers[ orders[i_per][i_class] ];
				solution[i_meet + i_class].period  = i_per;
			}
			bool accepted = true;
			for(i_class = n_classes-1; i_class >= 0; i_class--){
				if(!is_last_item_consistent(solution, i_meet+i_class, n_classes, n_teach, n_per)){
					accepted = false;
					break;
				}
			}
			if(accepted){
				i_meet += n_classes;
				i_per++;
			}
		}
		if(orders[0] != NULL){
			free(orders[0]);
		}
		free(orders);
		if(i_per < 0){
			//  If it backtracked all the way.
			return NULL;
		}
		return solution;
	} else {
		printf("It's insane!\n");
		return NULL;
	}

}

Meeting * initialize_all_meetings(
			  Teacher * teachers,
			  ExtendedClass * classes,
			  uint64_t n_classes){
    uint64_t i_class = 0, i_teach = 0;
	uint64_t i_meeting = 0, i_quant = 0;
	uint64_t n_meeting = get_number_of_meetings(classes,n_classes);
	Meeting * meetings = calloc( n_meeting + 1, sizeof(Meeting));
	for(i_class = 0; i_class < n_classes; i_class++){
		for(i_teach = 0; i_teach < classes[i_class].teachers_size; i_teach++){
			TeacherQuantity tq = classes[i_class].teachers[i_teach];
			for(i_quant = 0; i_quant < tq.quantity; i_quant++){
				meetings[i_meeting].teacher = tq.teacher;
				meetings[i_meeting].class   = &classes[i_class];
				meetings[i_meeting].period = -1;
				i_meeting++;
			}
		}
	}
	return meetings;
}

int ** copy_teacher_periods(Teacher * teachers, uint64_t n_teach, uint64_t n_period){
	uint64_t i_teach, i_per;
	int ** aval = calloc(n_teach + 1, sizeof(int *));
	for(i_teach = 0; i_teach < n_teach; i_teach++){
		aval[i_teach] = calloc(n_period + 1, sizeof(int) );
		for(i_per = 0; teachers[i_teach].periods[i_per] >= 0; i_per++){
			aval[i_teach][i_per] = teachers[i_teach].periods[i_per];
		}
		aval[i_teach][i_per] = teachers[i_teach].periods[i_per];
	}
	return aval;
}

void print_int_list(int * list){
	int i = 0;
	printf("[");
	while(list[i] != -1){
		printf("%d, ", list[i]);
		i++;
	}
	printf("];\n");
}

// Without considering other classes with the same teacher
void see_options_1(
			  Teacher * teachers,
			  ExtendedClass * classes,
			  uint64_t n_classes,
			  uint64_t n_teach,
		  	  uint64_t n_per){
	uint64_t i_met = 0;
	Meeting * met = initialize_all_meetings(teachers,classes,n_classes);
	int ** periods  = copy_teacher_periods(teachers,n_teach,n_per);
	for(i_met = 0; met[i_met].teacher != NULL; i_met++){
		printf("Meeting %2d: %s  %-9s %d ", i_met, met[i_met].class->name, met[i_met].teacher->name, met[i_met].period);
		int teacher_index = (met[i_met].teacher) - teachers;
		print_int_list(periods[teacher_index]);
	}
}

void see_options(
			  Teacher * teachers,
			  ExtendedClass * classes,
			  uint64_t n_classes,
			  uint64_t n_teach,
		  	  uint64_t n_per){
	uint64_t i_met = 0, i_per = 0;
	Meeting * met = initialize_all_meetings(teachers,classes,n_classes);

	met[0].period = 1;

	int ** periods  = copy_teacher_periods(teachers,n_teach,n_per);
	for(i_met = 0; met[i_met].teacher != NULL; i_met++){
		printf("Meeting %2d: %s  %-9s ", i_met, met[i_met].class->name, met[i_met].teacher->name, met[i_met].period);
		// TODO: if(period  == -1 ) then show avalible. else, show the period.
		if(met[i_met].period != -1){
			printf("%d\n", met[i_met].period);
		} else {
			int teacher_index = (met[i_met].teacher) - teachers;
			Meeting test_meeting;
			test_meeting.teacher = met[i_met].teacher;
			test_meeting.class = met[i_met].class;
			printf("[");
			for(i_per = 0; periods[teacher_index][i_per] >= 0; i_per++){
				// We want to print just those that are avalible
				// so we imagine "is this meeting consistent with the others?"
				// if yes, then it's a valid option.
				test_meeting.period = i_per;
				if(is_meeting_consistent(test_meeting,met)){
					printf("%d, ", periods[teacher_index][i_per]);
				} else {
					// printf("f, ");
				}
			}
			printf("];\n");
		}
	}
}

#endif /* PLANNER_H */
