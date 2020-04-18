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
 *		Implemented, Tested
 */
uint64_t factorial_division(const uint64_t p, const uint64_t t){
	uint64_t i = p;
	uint64_t total = 1;
	LMH_ASSERT(p >= t && p >= 0 && t >= 0, "invalid par");
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
 * 		Implemented, Tested
 */
uint64_t factorial(const uint64_t n){
	LMH_ASSERT(n>=0, "invalid par");
	return factorial_division(n,0);
}

/* CREATE LIST BY INDEX
 * 		Gets one combination of K elements, based on its index.
 *
 * Development Status:
 *		Implemented, TODO test
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
 *		Orders elements of a list descendingly.
 *
 * Development status:
 * 		Implemented, Tested
 */
int * order_elements_desc(const int * const list){
	int min = 0, tmp = 0;
	int i, j, i_min = 0, n;

	LMH_ASSERT(list != NULL, "null par");

	for(n = 0; list[n] >= 0; n++){ }

	int * ordered = calloc(n+1, sizeof(int));

	for(i = 0; i <= n; i++){
		ordered[i] = list[i];
	}

	for(i = 0; i < n; ++i){
		min = ordered[i];
		i_min = i;
		for(j = i + 1; j < n; ++j){
			if(ordered[j] >= min){
				i_min = j;
				min = ordered[i_min];
			}
		}
		tmp = ordered[i];
		ordered[i] = ordered[i_min];
		ordered[i_min] = tmp;
	}
	return ordered;
}

/* ORDER ELEMENTS ASC
 *		Orders elements of a list ascendingly
 * Alters the list.
 *
 * Development Status:
 *		Implemented, Tested
 */
int * order_elements_asc(const int * const list){
	int min = 0, tmp = 0;
	int i, j, i_min = 0, n;

	LMH_ASSERT(list != NULL, "null par");

	for(n = 0; list[n] >= 0; n++){ }

	int * ordered = calloc(n+1, sizeof(int));

	for(i = 0; i <= n; i++){
		ordered[i] = list[i];
	}

	for(i = 0; i < n; ++i){
		min = ordered[i];
		i_min = i;
		for(j = i + 1; j < n; ++j){
			if(ordered[j] <= min){
				i_min = j;
				min = ordered[i_min];
			}
		}
		tmp = ordered[i];
		ordered[i] = ordered[i_min];
		ordered[i_min] = tmp;
	}
	return ordered;
}

/* GET FIRST ORDER INT
 *		Returns the first order of n elements, namely [0,1,2,3... n].
 *
 * Development status:
 *		Implemented, TODO test
 */
int * get_first_order(const int size){
	int i, *list;
	LMH_ASSERT(size >= 0, "null par");
	list = calloc(size + 1, sizeof(int));
	for(i = 0; i < size; ++i){
		list[i] = i;
	}
	list[size] = -1;
	return list;
}

/* GET NEXT ORDER
 *		returns the next order of elements (from dec to asc)
 *
 * 		Imagine that you have a particular list of numbers.
 *		The corresponding set has a list of (nummerable) possible orders,
 * 		so the list [1,2,3,4,5] has these possible orders:
 * 		[[1,2,3,4,5], [1,2,3,5,4], [1,2,4,3,5], ... [5,4,3,2,1]]
 *
 * 		What this function does is that it jumps from one in the
 * 		above list to the next.
 *
 * 		When we have the last element on that list, say, [3,2,1],
 * 		this function just doesn't alter anything.
 *
 * Development status:
 * 		Implemented, TODO test
 */
bool get_next_order(int * order){
	int i, j;
	int * sublist;

	int size;
	for(size = 0; order[size] >= 0; size++) { }

	i = size - 2;
	// While it' a descending list, it's good.
	while( i >= 0 && order[1+i] < order[i]){
		--i;
	}
	if(i >= 0){
		/* We need to swap the ith element */
		int next_el_index = i+1;
		// Calculating what is the next element in the list.
		// It's the smallest number bigger than next[i-1]
		for(int j = i+2; j < size; ++j){
			if( order[i] < order[j] && order[j] <= order[next_el_index]){
				next_el_index = j;
			}
		}
		int tmp = order[i];
		order[i] = order[next_el_index];
		order[next_el_index] = tmp;
		sublist = order_elements_asc(&order[i+1]);
		for(j = i+1; j <= size; j++){
			order[i] = sublist[i];
		}
		free(sublist);
	} else {
		return false;
	}
	return true;
}

/* GET FIRST SUBSET
 * 		returns the first index of subsets of that size (just an empty list)
 *
 * Development status:
 * 		Implemented, TODO test
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
 *		Implemented, TODO test
 */
bool get_next_subset(int * list){
	uint64_t max = 0;
	int tmp;
	for(int i = 0; list[i] != -1; ++i){
		if(list[i] >= max){
			max = list[i];
		}
	}
	tmp = list[max];
	list[max] = -1;
	bool hasNext = get_next_order(list);
	list[max] = tmp;
	if(hasNext){
		// count which elements we have until now
		for(int i = 0; i <= max + 1; ++i){
			list[i] = i;
		}
		return true;
	}
	return false;
}

/* STR LIST LEN
 *		returns the size of a null-terminated list of pointers.
 *
 * Development status:
 *		Implemented, Tested.
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
 *		Implemented, Tested
 */
int int_list_len(const int * const list){
	int len = 0;
	LMH_ASSERT(list != NULL, "null par");
	while(list[len] >= 0)
		++len;
	return len;
}

/* NON ZERO INT LIST COUNT
 *		returns the number of positive ints in a negative-terminated list
 *
 * Development status:
 *		Implemented, Tested
 */
int non_zero_int_list_count(const int * const list){
	int i = 0, ctr = 0;
	LMH_ASSERT(list != NULL, "null par");
	while(list[i] >= 0){
		if(list[i] > 0){
			++ctr;
		}
		++i;
	}
	return ctr;
}

/* ARE INT LISTS EQUAL
 *		returns true if all elements of two negative-terminated lists are equal
 *
 * Development status:
 * 		Implemented, Tested
 */
bool are_int_lists_equal(const int * const a, const int * const b){
	int i = 0;
	LMH_ASSERT(a != NULL && b != NULL, "null par");
	if(a == b){
		return true;
	}
	while(a[i] != -1 && b[i] == a[i]){
		++i;
	}
	return (a[i] < 0 && b[i] < 0) || a[i] == b[i];
}


/* FIND FIRST POSITIVE
 *		Returns the index of the first positive value in a non-negative int list.
 *
 * Returns -1 in case that there was no such element.
 *
 * Development status:
 *		Implemented, Tested
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
 *		Implemented, Tested
 */
int find_last_positive(const int * const list){
	int index = 0;
	LMH_ASSERT(list != NULL, "null par");
	while(list[index] >= 0){
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
 * If there are multiple instances of a same number, returns the first.
 *
 * Development status:
 *		Implemented, Tested
 */
int find_max_int(const int * const list){
	int i = 1, max = 0;
	LMH_ASSERT(list != NULL, "null par");
	if(list[0] < 0){
		return -1;
	}
	while(list[i] != -1){
		if(list[max] < list[i])
			max = i;
		++i;
	}
	return max;
}

/* INT LIST BOTH POSITIVE CTR
 *		calculates the number of times taht a[i] > 0 & b[i] > 0.
 *
 * Development status:
 *		Implemented, Tested
 */
int int_list_both_positive_ctr(const int * const list_a, const int * const list_b){
	int n = 0, i;
	for( i = 0; list_a[i] >= 0 && list_b[i] >= 0; ++i){
		if(list_a[i] > 0 && list_b[i] > 0){
			++n;
		}
	}
	return n;
}
