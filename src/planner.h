/*
 * Project Германия.
 *
 * Copyright (C) Léo H. 2019.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "combinatorics.h"

typedef struct {
	char * name;
	int  * periods;
} Teacher;

typedef struct {
	Teacher * teacher;
	int		  quantity;
} TeacherQuantity;

typedef struct {
	TeacherQuantity * teachers;
	uint64_t  teachers_size;
	char    * name;
} Class;

typedef struct {
	Teacher * teacher;
	Class   * class;
	int 	  period;
} Meeting;

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
		// debug version

		// 	for(int i = 0; i < n_sol; i++){
		// 		printf(" %9s", solution[i].teacher->name);
		// 		printf(" %9s", solution[i].class->name);
		// 		printf(" %9d\n", solution[i].period);
		// 	}
	} else {
		printf("Impossible to solve;\n");
	}
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
	if(last_quantity != -1){
		for(int i = 0; i < i_last; i++){
			bool sameTeacher = last.teacher == schedule[i].teacher;
			bool sameClass   = last.class == schedule[i].class;
			bool samePeriod  = last.period == schedule[i].period;

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

Meeting* solve(
			  Teacher * teachers,
			  Class * classes,
			  uint64_t n_classes,
			  uint64_t n_teach,
		  	  uint64_t n_per){

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
			uint64_t * tmp = get_next_order(orders[i_per], n_teach);
			// If it needs to backtrack further;
			if(tmp != NULL){
				orders[i_per] = tmp;
			} else{
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
		free(solution);
		return NULL;
	}
	printf("i_per = %d\n", i_per);
	return solution;
}
