/*
 * Project Minerva.
 *   maths.c - mathematics utility library.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains general combinatorics functions counting
 * and ordering functions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "maths.h"
#include "assert.h"

/* FACTORIAL DIVISION
 *		Calculates the result of P!/(P-T)!
 *
 * Development Status:
 *		Implemented,
 */
uint64_t factorial_division(const uint64_t p, const uint64_t t){
	uint64_t i = p;
	uint64_t total = 1;
	LMH_ASSERT(p >= t && p > 0, "invalid par");
	while(i > t){
		total *= i;
		--i;
	}
	return total;
}

/* FACTORIAL
 * 		Calculates the result of N!
 *
 * Development Status:
 * 		Implemented,
 */
uint64_t factorial(const uint64_t n){
	LMH_ASSERT(n>=0, "invalid par");
	return factorial_division(n,0);
}

/* CREATE LIST BY INDEX
 * 		Gets one combination of K elements, based on its index.
 *
 * Development Status:
 *		Implemented,
 *
 * Example:
 *   With n=5, we have the numbers {1,2,3,4,5},
 *   we can form the combinations (in order):
 *     1- 12345
 *     2- 12354
 *     3- 12435
 *     ...
 *     120- 54321
 *	So when I = 3 and K = 5, we have {1,2,4,3,5} as a result.
 */
uint64_t * create_list_by_index(const uint64_t elements_, const uint64_t n){
	uint64_t * decomposition, elements = elements_;
	LMH_ASSERT(elements >= 0 && n >= 0, "null par");
	decomposition = calloc(n, sizeof(uint64_t));
	int i = 0, j = n-1;
	while(j >= 0){
		decomposition[i] = 1+ elements/factorial(j);
		elements %= factorial(j);
		--j;
		++i;
	}
	// Correction of indexes
	bool * used_numbers = calloc(n, sizeof(bool));
	for(i = 0; i < n; ++i){
		used_numbers[i] = false;
	}
	for(i = 0; i < n; ++i){
		while(used_numbers[ decomposition[i] ] == true){
			++decomposition[i];
		}
		used_numbers[ decomposition[i] ] = true;
		for(j = i+1; j < n; ++j){
			if(decomposition[i] <= decomposition[j]){
				++decomposition[j];
			}
		}
	}
	free(used_numbers);
	return decomposition;
}

/* ORDER ELEMENTS DESC
 *		Orders elements of a list descendingly
 * Alters the list.
 *
 * Development status:
 * 		Implemented
 */
void order_elements_desc(uint64_t * list, const size_t size){
	uint64_t max = 0, tmp = 0;
	int i, j, iMax = 0;
	for(i = 0; i < size; ++i){
		for(j = i; j < size; ++j){
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

/* ORDER ELEMENTS ASC
 *		Orders elements of a list ascendingly
 * Alters the list.
 *
 * Development Status:
 *		Implemented.
 */
void order_elements_asc(uint64_t * list, const size_t size){
	uint64_t min = -1, tmp = 0;
	int i, j, i_min = 0;
	for(i = 0; i < size; ++i){
		for(j = i; j < size; ++j){
			if(list[j] < min){
				min = list[j];
				i_min = j;
			}
		}
		tmp = list[i];
		list[i] = min;
		list[i_min] = tmp;
		min = -1;
	}
}

/* GET FIRST ORDER
 *		Returns the first order of n elements, namely [0,1,2,3... n].
 *
 * Development status:
 *		Implemented.
 */
uint64_t * get_first_order(const size_t size){
	uint64_t * list = calloc(size + 1, sizeof(uint64_t));
	for(size_t i = 0; i < size; ++i){
		list[i] = i;
	}
	list[size] = -1;
	return list;
}

/* GET FIRST ORDER INT
 *		Returns the first order of n elements, namely [0,1,2,3... n].
 *
 * Development status:
 *		Implemented.
 */
int * get_first_order_int(const int size){
	int * list = calloc(size + 1, sizeof(int));
	for(int i = 0; i < size; ++i){
		list[i] = i;
	}
	list[size] = -1;
	return list;
}

/* GET NEXT ORDER
 *		returns the next order of elements (from dec to asc)
 *
 * Jumps to the next possible order of those elements.
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
 *
 * Development status:
 * 		Implemented
 */
bool get_next_order(uint64_t * order, const size_t size){
	int i = size-2;
	// While it' a descending list, it's good.
	while( i >= 0 && order[1+i] < order[i]){
		--i;
	}
	if(i >= 0){
		/* We need to swap the ith element */
		size_t next_el_index = i+1;
		// Calculating what is the next element in the list.
		// It's the smallest number bigger than next[i-1]
		for(int j = i+2; j < size; ++j){
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

/* GET FIRST SUBSET
 * 		returns the first index of subsets of that size (just an empty list)
 *
 * Development status:
 * 		Implemented
 */
uint64_t * get_first_subset(const size_t size){
	uint64_t * list = calloc(size+1, sizeof(uint64_t));
	for(int i = 0; i <= size; ++i){
		list[i] = -1;
	}
	list[0] = 0;
	return list;
}

/* GET NEXT SUBSET
 *		returns the next subset (from dec to asc)
 *
 * Development status:
 *		Implemented
 */
uint64_t * get_next_subset(uint64_t * list, const size_t size){
	uint64_t max = 0;
	for(int i = 0; list[i] != -1; ++i){
		if(list[i] >= max)
			max = list[i];
	}
	bool hasNext = get_next_order(list,max);
	if(hasNext){
		// count which elements we have until now
		for(int i = 0; i <= max + 1; ++i){
			list[i] = i;
		}
		return list;
	}
	return NULL;
}

/* STR LIST LEN
 *		returns the size of a null-terminated list of pointers.
 *
 * Development status:
 *		Implemented
 */
int str_list_len(const char * const list[]){
	int len = 0;
	LMH_ASSERT(list != NULL, "null par");
	while(list[len] != NULL)
		++len;
	return len;
}

/* INT LIST LEN
 *		returns the length of a negative-terminated int list.
 *
 * Development status:
 *		Implemented
 */
int int_list_len(const int * const list){
	int len = 0;
	LMH_ASSERT(list != NULL, "null par");
	while(list[len] >= 0)
		++len;
	return len;
}

/* NOT NUL INT LIST LEN
 *		returns the number of positive ints in a negative-terminated list
 *
 * Development status:
 *		Implemented
 */
int not_null_int_list_len(const int * const list){
	int i = 0, len = 0;
	LMH_ASSERT(list != NULL, "null par");
	while(list[i] != -1){
		if(list[i] > 0){
			++len;
		}
		++i;
	}
	return len;
}

/* ARE INT LISTS EQUAL
 *		returns true if all elements of two negative-terminated lists are equal
 *
 * Development status:
 * 		Implemented.
 */
bool are_int_lists_equal(const int * const a, const int * const b){
	int i = 0;
	LMH_ASSERT(a != NULL && b != NULL, "null par");
	while(a[i] != -1 && b[i] == a[i]){
		++i;
	}
	return a[i] == b[i];
}


/* FIND FIRST POSITIVE
 *		Returns the index of the first positive value in a non-negative int list.
 *
 * Returns -1 in case that there was no such element.
 *
 * Development status:
 *		Implemented
 */
int find_first_positive(const int * const list){
	int index = 0;
	while(list[index] == 0){
		++index;
	}
	if(list[index] <= 0){
		return -1;
	} else {
		return index;
	}
}

/* FIND LAST POSITIVE
 * 		Returns the index of the last positive value in a non-negative int list.
 *
 * Returns -1 in case that there was no such element.
 *
 * Development status:
 *		Implemented
 */
int find_last_positive(const int * const list){
	int index = 0;
	LMH_ASSERT(list != NULL, "null par");
	while(list[index] != -1){
		++index;
	}
	/* the current value was -1; */
	--index;
	while( index >= 0 && list[index] == 0) {
		--index;
	}
	/* may return -1 in case of error. */
	return index;
}

/* FIND MAX INT
 *		Returns the index of the largest value in a non-negative int list.
 *
 * Development status:
 *		Implemented
 */
int find_max_int(const int * const list){
	int index = 0, max = -1;
	LMH_ASSERT(list != NULL, "null par");
	while(list[index] != -1){
		if(max < list[index])
			max = list[index];
		++index;
	}
	return index;
}

/* ORDER BY VALUE
 *		Orders indexes according to value in a list
 *
 * Returns a list of indexes.
 *
 * Development status:
 * 		Implemented.
 */
int * order_by_value(const int * const values){
	int i = 0, j = 0, k = 0, n, max = -1, i_list = 0;
	int * list;
	LMH_ASSERT(values != NULL, "null par");
	for(n = 0; values[n] >= 0; ++n){ }
	list = calloc(n + 1, sizeof(int));
	for(i = 0; i <= n; i++){
		list[i] = -1;
	}
	for(i = 0; i < n - j; ++i){
		max = n;
		for(k = 0; k < n; ++k){
			if(list[k] != -1)
				continue;
			if(values[k] > values[max]){
				max = k;
			}
		}
		list[i_list] = max;
		++i_list;
	}
	return list;
}

/* ORDER BY VALUE NOT ZERO
 *		returns a list of indexes, by ascending order.
 *
 * The list returned may end before values's list, because it
 * does not include indexes s.t. values[list[i]] = 0.
 *
 * Development status:
 *		Implemented.
 */
int * order_by_value_not_zero(const int * const values){
	int i = 0, k = 0, n, max = -1, i_list = 0;
	LMH_ASSERT(values != NULL, "null par");
	int * list;
	for(n = 0; values[n] >= 0; ++n){ }
	list= calloc(n+1 , sizeof(int));
	for(i = 0; i <= n; i++){
		list[i] = -1;
	}
	for(i = 0; values[i] >= 0; ++i){
		if(values[i] == 0){
			list[i] = 0;
		}
	}
	for(i = 0; i < n; ++i){
		max = n;
		for(k = 0; k < n; ++k){
			if(list[k] != -1)
				continue;
			if(values[k] > values[max]){
				max = k;
			}
		}
		list[i_list] = max;
		++i_list;
	}
	for(i = 0; i < n; ++i){
		if(list[i] == n){
			list[i] = -1;
			break;
		}
	}
	return list;
}

/* INT LIST INTERSEC SIZE
 *		calculates the number of times taht a[i] > 0 & b[i] > 0.
 *
 * Development status:
 *		Implemented.
 */
int int_list_intersec_size(const int * const list_a, const int * const list_b){
	int n = 0, i;
	for( i = 0; list_a[i] >= 0 && list_b[i] >= 0; ++i){
		if(list_a[i] > 0 && list_b[i] > 0){
			++n;
		}
	}
	return n;
}
