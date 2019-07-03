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

// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------

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

// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------

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

/* If the last item of the list violates no constraint,
 * considering the other elements.
 */
bool check_last_item_consistency(
			  Meeting  * schedule,
		 	  uint64_t i_last,
		 	  uint64_t n_turmas,
			  uint64_t n_prof,
		   	  uint64_t n_period){
	Meeting last = schedule[i_last];
	uint64_t given_lectures = 0;
	// TODO: be careful comparing references.
	// For the other tuples
	for(int i = 0; i < i_last; i++){
		if(last.teacher == schedule[i].teacher){
			// If the teacher is omnipresent
			if(last.period == schedule[i].period)
				return false;
			// If the number of classes exceeds
			if(last.class == schedule[i].class)
				given_lectures++;
		}
		if(last.class == schedule[i].class){
			// If the class has 2 teachers at the same time
			if(last.period == schedule[i].period)
				return false;
		}
	}
	// If the number of given lectures does not exceed
	uint64_t last_quantity = 0;
	for(int i = 0; i < last.class->teachers_size; i++){
		if(last.class->teachers[i].teacher == last.teacher){
			last_quantity = last.class->teachers[i].quantity;
		}
	}
	// Otherwise they would have too many lectures;
	return last_quantity >= given_lectures;
}

// get_avalible_teachers_for_those_classes(Class * classes){
//
// }


Meeting* solve(
			  Teacher * teachers,
			  Class * classes,
			  uint64_t n_classes,
			  uint64_t n_teach,
		  	  uint64_t n_per){

	uint64_t n_sol = n_per * n_classes * n_teach;

	int i_sol = 0, i_per = 0, i_class = 0;

	Meeting * solution = calloc(n_sol, sizeof(Meeting));
	// possibility of backtracking
	uint64_t ** orders = calloc(n_per, sizeof(uint64_t *));
	// REMEMBER! We don't copy, as we compare by reference
	Teacher ** avalible_teachers = calloc(n_teach, sizeof(Teacher*));
	while(i_per < n_per){
		if(i_per < 0){
			//  If it backtracked all the way.
			printf("impossible to solve.\n");
			break;
		}
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
				i_per--;
				i_sol -= n_classes;
				continue;
			}
		}
		// All the next orders must be nullified if we are to start over
		for(int i = i_per+1; i < n_per; i++){
			if(orders[i]){
				free(orders[i]);
				orders[i] = NULL;
			} else break;
		}
		// Assignment of that order to the corresponding variables;
		bool accepted = true;
		for(i_class = 0; i_class < n_classes; i_class++){
			solution[i_sol].class = &classes[i_class];
			solution[i_sol].period = i_per;
			solution[i_sol].teacher = &teachers[ orders[i_per][i_class] ];
			if(!check_last_item_consistency(solution, i_sol, n_classes, n_teach, n_per)){
				accepted = false;
				break;
			}
			i_sol++;
		}
		if(accepted){
			i_per++;
		} else {
			i_per--;
			i_sol -= n_classes;
		}
	}
	return solution;
}

void print_solution(Meeting * solution, uint64_t n_sol, uint64_t n_per){
	// ---
	// decent version
	// ---
	// for(int i = 0; i < n_per; i++){
	// 	printf("\n\nIn the period %d: \n", i);
	// 	for(int j = 0; j < n_sol; j++){
	// 		if(solution[j].period == i){
				// if(solution[j].teacher == NULL)
				// 	printf("0 ");
				// else printf("%s ", solution[j].teacher->name);
				// if(solution[j].class == NULL)
				// 	printf("0\n");
				// else printf("%s \n", solution[j].class->name);
	//
	// 			//printf("%s %s\n", solution[j].teacher->name, solution[j].class->name);
	// 		}
	// 	}
	// }

	// debug version
	for(int i = 0; i < n_sol; i++){
		if(solution[i].teacher == NULL)
			printf("         0");
		else printf("%10s", solution[i].teacher->name);
		if(solution[i].class == NULL)
			printf("         0");
		else printf("%10s", solution[i].class->name);
		printf("%10d\n", solution[i].period);
	}
}


int main(){
	uint64_t n_turmas = 3;
	uint64_t n_prof   = 3;
	uint64_t n_period = 3;

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
	// Possible schedule
	//        1   2   3
	// T1:    p1  p2  p3
	// T2:    p2  p3  p1
	// T3:    p3  p1  p2

	//solve(n_turmas, n_prof, n_period, aval_prof);
	//test_decompose();
	//test_fact_div();
	return 0;
}
