/*
 * Project Minerva.
 *   maths.h - mathematics utility library.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains general combinatorics functions counting
 * and ordering functions.
 */
#ifndef MATHS_H
#define MATHS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

/* FACTORIAL DIVISION
 *		Calculates the result of P!/(P-T)!
 *
 * Development Status:
 *		Implemented, Tested
 */
uint64_t factorial_division(const uint64_t p, const uint64_t t);

/* FACTORIAL
 * 		Calculates the result of N!
 *
 * Development Status:
 * 		Implemented, Tested
 */
uint64_t factorial(const uint64_t n);

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
uint64_t * create_list_by_index(const uint64_t elements, const uint64_t n);

/* ORDER ELEMENTS DESC
 *		Orders elements of a list descendingly
 *
 * Development status:
 * 		Implemented, Tested
 */
int * order_elements_desc(const int * const list);

/* ORDER ELEMENTS ASC
 *		Orders elements of a list ascendingly
 * Alters the list.
 *
 * Development Status:
 *		Implemented, Tested
 */
int * order_elements_asc(const int * const list);

/* GET DESC ORDER INDEXES
 *		Orders elemens of a list descendingly, using an auxiliary array.
 *
 * Development Status:
 *		Implemented.
 */
int * get_desc_order_indexes(const int * list);

/* GET FIRST ORDER
 *		Returns the first order of n elements, namely [0,1,2,3... n].
 *
 * Development status:
 *		Implemented, TODO test
 */
int * get_first_order(const int size);

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
bool get_next_order(int * order);

/* GET FIRST SUBSET
 * 		returns the first index of subsets of that size (just an empty list)
 *
 * Development status:
 * 		Implemented, TODO test
 */
uint64_t * get_first_subset(size_t size);

/* GET NEXT SUBSET
 *		returns the next subset (from dec to asc)
 *
 * Development status:
 *		Implemented, TODO test
 */
bool get_next_subset(int * list);

/* STR LIST LEN
 *		returns the size of a null-terminated list of pointers.
 *
 * Development status:
 *		Implemented, Tested.
 */
int str_list_len(const char * const list[]);

/* INT LIST LEN
 *		returns the length of a negative-terminated int list.
 *
 * Development status:
 *		Implemented, Tested.
 */
int int_list_len(const int * const list);

/* NON ZERO INT LIST COUNT
 *		returns the number of positive ints in a negative-terminated list
 *
 * Development status:
 *		Implemented, Tested.
 */
int non_zero_int_list_count(const int * const list);

/* ARE INT LISTS EQUAL
 *		returns true if all elements of two negative-terminated lists are equal
 *
 * Development status:
 * 		Implemented, Tested.
 */
bool are_int_lists_equal(const int * const a, const int * const b);

/* FIND FIRST POSITIVE
 *		Returns the index of the first positive value in a non-negative int list.
 *
 * Returns -1 in case that there was no such element.
 *
 * Development status:
 *		Implemented, Tested.
 */
int find_first_positive(const int * const list);

/* FIND LAST POSITIVE
 * 		Returns the index of the last positive value in a non-negative int list.
 *
 * Returns -1 in case that there was no such element.
 *
 * Development status:
 *		Implemented, Tested.
 */
int find_last_positive(const int * const list);

/* FIND MAX INT
 *		Returns the index of the largest value in a non-negative int list.
 *
 * Development status:
 *		Implemented, Tested.
 */
int find_max_int(const int * const list);

/* INT LIST BOTH POSITIVE CTR
 *		calculates the number of times taht a[i] > 0 & b[i] > 0.
 *
 * Development status:
 *		Implemented, Tested.
 */
int int_list_both_positive_ctr(const int * const a, const int * const b);

/* INT LIST DISCREPANCY
 *		the higher the result, more is there a 'clear frontrunner'.
 *		If the result is 0, all elements are 0.
 * Development status:
 *		Implemented.
 */
int int_list_discrepancy(const int * const a);


#endif /* MATHS_H */
