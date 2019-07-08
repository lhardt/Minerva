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

/* Calculates the result of P!/(P-T)! */
uint64_t factorial_division(uint64_t p, uint64_t t){
	uint64_t i = p;
	uint64_t total = 1;
	while(i > t){
		total *= i;
		i--;
	}
	return total;
}

/* Calculates the result of N! */
uint64_t factorial(uint64_t n){
	uint64_t r = 1;
	while( n > 0){
		r = r*n;
		n--;
	}
	return r;
}

/* Gets one combination of K elements, based on its index.
 *
 * Example:
 *   With the numbers {1,2,3,4,5},
 *   we can form the combinations (in order):
 *     1- 12345
 *     2- 12354
 *     3- 12435
 *     ...
 *     120- 54321
 *	So when I = 3 and K = 5, we have {1,2,4,3,5} as a result.
 */
uint64_t * decompose(uint64_t combination, uint64_t n){
	uint64_t * decomposition = calloc(n, sizeof(uint64_t));
	int i = 0, j = n-1;

	while(j >= 0){
		decomposition[i] = 1+ combination/factorial(j);
		combination %= factorial(j);
		j--;
		i++;
	}
	// Correction of indexes
	bool * used_numbers = calloc(n, sizeof(bool));
	for(i = 0; i < n; i++){
		used_numbers[i] = false;
	}
	for(i = 0; i < n; i++){
		while(used_numbers[ decomposition[i] ] == true){
			decomposition[i]++;
		}
		used_numbers[ decomposition[i] ] = true;
		for(j = i+1; j < n; j++){
			if(decomposition[i] <= decomposition[j]){
				decomposition[j]++;
			}
		}
	}
	free(used_numbers);

	return decomposition;
}

/* Orders elements of a list descendingly */
void order_elements_desc(uint64_t * list, size_t size){
	uint64_t max = 0, tmp = 0;
	int i, j, iMax = 0;
	for(i = 0; i < size; i++){
		for(j = i; j < size; j++){
			if(list[j] > max){
				max = list[j];
				iMax = j;
			}
		}
		tmp = list[i];
		list[i] = max;
		list[iMax] = tmp;
		max = 0;
	}
}

/* Orders elements of a list ascendingly */
void order_elements_asc(uint64_t * list, size_t size){
	uint64_t min = -1, tmp = 0;
	int i, j, iMin = 0;
	for(i = 0; i < size; i++){
		for(j = i; j < size; j++){
			if(list[j] < min){
				min = list[j];
				iMin = j;
			}
		}
		tmp = list[i];
		list[i] = min;
		list[iMin] = tmp;
		min = -1;
	}
}

/* Jumps to the next possible order of those elements.
 *
 * Imagine that you have a particular list of numbers.
 *
 * The corresponding set has a list of (nummerable) possible orders,
 * so the list [1,2,3,4,5] has these possible orders:
 *
 * [[1,2,3,4,5], [1,2,3,5,4], [1,2,4,3,5], ... [5,4,3,2,1]]
 *
 * What this function does is that it jumps from one in the
 * above list to the next.
 *
 * When we have the last element on that list, say, [3,2,1],
 * this function just doesn't alter anything.
 */
uint64_t * get_first_order(size_t size){
	uint64_t * list = calloc(size, sizeof(uint64_t));
	for(int i = 0; i < size; i++){
		list[i] = i;
	}
	return list;
}

/* Jumps to the next possible order of those elements.
 *
 * Imagine that you have a particular list of numbers.
 *
 * The corresponding set has a list of (nummerable) possible orders,
 * so the list [1,2,3,4,5] has these possible orders:
 *
 * [[1,2,3,4,5], [1,2,3,5,4], [1,2,4,3,5], ... [5,4,3,2,1]]
 *
 * What this function does is that it jumps from one in the
 * above list to the next.
 *
 * When we have the last element on that list, say, [3,2,1],
 * this function just doesn't alter anything.
 */
uint64_t * get_next_order(uint64_t * order, size_t size){
	int i = size-2, j = 0;
	// While it' a descending list, it's good.
	while( i >= 0 && order[1+i] < order[i]){
		i--;
	}
	if(i >= 0){
		/* We need to swap the ith element */
		uint64_t next_element = -1;
		size_t next_el_index = i+1;
		// Calculating what is the next element in the list.
		// It's the smallest number bigger than next[i-1]
		for(int j = i+2; j < size; j++){
			if( order[i] < order[j] && order[j] <= order[next_el_index]){
				next_el_index = j;
			}
		}
		uint64_t tmp = order[i];
		order[i] = order[next_el_index];
		order[next_el_index] = tmp;
		order_elements_asc(&order[i+1], size-i-1);
	} else return NULL;
	return order;
}

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
			printf("\nIn the period %d: \n", i);
			for(int j = 0; j < n_sol; j++){
				if(solution[j].period == i){
					printf("%s ", solution[j].teacher->name);
					printf("%s \n", solution[j].class->name);
				}
			}
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
	return solution;
}
