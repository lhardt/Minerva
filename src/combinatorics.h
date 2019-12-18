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

#include "definitions.h"


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
	return factorial_division(n,1);
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
 uint64_t * decompose(uint64_t elements, uint64_t n){
 	uint64_t * decomposition = calloc(n, sizeof(uint64_t));
 	int i = 0, j = n-1;

 	while(j >= 0){
 		decomposition[i] = 1+ elements/factorial(j);
 		elements %= factorial(j);
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
 	uint64_t * list = calloc(size + 1, sizeof(uint64_t));
 	for(size_t i = 0; i < size; i++){
 		list[i] = i;
 	}
 	list[size] = -1;
 	return list;
  }
  int * get_first_order_int(int size){
 	int * list = calloc(size + 1, sizeof(int));
 	for(int i = 0; i < size; i++){
 		list[i] = i;
 	}
 	list[size] = -1;
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
bool get_next_order(uint64_t * order, size_t size){
 	int i = size-2;
 	// While it' a descending list, it's good.
 	while( i >= 0 && order[1+i] < order[i]){
 		i--;
 	}
 	if(i >= 0){
 		/* We need to swap the ith element */
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
 	} else {
		return false;
	}
	return true;
 }

 uint64_t * get_first_subset(size_t size){
	uint64_t * list = calloc(size+1, sizeof(uint64_t));
	for(int i = 0; i <= size; i++){
		list[i] = -1;
	}
	list[0] = 0;
	return list;
}

uint64_t * get_next_subset(uint64_t * list, size_t size){
	uint64_t max = 0;
	for(int i = 0; list[i] != -1; i++){
		if(list[i] >= max)
			max = list[i];
	}
	bool hasNext = get_next_order(list,max);
	if(hasNext){
		// count which elements we have until now
		for(int i = 0; i <= max + 1; i++){
			list[i] = i;
		}
		return list;
	} else return NULL;

}


int str_list_len(const char * const list[]){
	int len = 0;
	while(list[len] != NULL)
		len ++;
	return len;
}

int int_list_len(const int * const list){
	int len = 0;
	while(list[len] != -1)
		len ++;
	return len;
}

int not_null_int_list_len(const int * const list){
	int i = 0, len = 0;
	while(list[i] != -1){
		if(list[i] > 0){
			len++;
		}
		i ++;
	}
	return len;
}

void print_int_list(const int * const list){
	int i = 0;
	while(list[i] != -1){
		printf("List[%d]: %d\n", i, list[i]);
		i++;
	}
}

bool equal_lists(int * a, int * b){
	int i = 0;
	while(a[i] != -1 && b[i] == a[i]){
		i++;
	}
	return a[i] == b[i];
}


/* Returns the index of the first positive integer in a list terminated
 * by -1.
 */
int find_first_positive(int * list){
	int index = 0;
	while(list[index] == 0){
		index++;
	}
	if(list[index] == -1){
		return -1;
	} else {
		return index;
	}
}

/* Returns the index of the last positive integer in a list terminated
 * by -1.
 */
int find_last_positive(int * list){
	int index = 0;
	while(list[index] != -1){
		index++;
	}
	// the current value was -1;
	index--;
	while(list[index] == 0 && index >= 0) {
		index--;
	}
	// may return -1 in case of error.
	return index;
}
