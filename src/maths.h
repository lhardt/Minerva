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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "types.h"

/* FACTORIAL DIVISION
 *		Calculates the result of P!/(P-T)!
 *
 * Development Status:
 *		Implemented,
 */
uint64_t factorial_division(const uint64_t p, const uint64_t t);

/* FACTORIAL
 * 		Calculates the result of N!
 *
 * Development Status:
 * 		Implemented,
 */
uint64_t factorial(const uint64_t n);

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
uint64_t * create_list_by_index(const uint64_t elements, const uint64_t n);

/* ORDER ELEMENTS DESC
 *		Orders elements of a list descendingly
 * Alters the list.
 *
 * Development status:
 * 		Implemented
 */
void order_elements_desc(uint64_t * list, const size_t size);

/* ORDER ELEMENTS ASC
 *		Orders elements of a list ascendingly
 * Alters the list.
 *
 * Development Status:
 *		Implemented.
 */
void order_elements_asc(uint64_t * list, const size_t size);

/* GET FIRST ORDER
 *		Returns the first order of n elements, namely [0,1,2,3... n].
 *
 * Development status:
 *		Implemented.
 */
uint64_t * get_first_order(const size_t size);

/* GET FIRST ORDER INT
 *		Returns the first order of n elements, namely [0,1,2,3... n].
 *
 * Development status:
 *		Implemented.
 */
int * get_first_order_int(const int size);

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
bool get_next_order(uint64_t * order, size_t size);

/* GET FIRST SUBSET
 * 		returns the first index of subsets of that size (just an empty list)
 *
 * Development status:
 * 		Implemented
 */
uint64_t * get_first_subset(size_t size);

/* GET NEXT SUBSET
 *		returns the next subset (from dec to asc)
 *
 * Development status:
 *		Implemented
 */
uint64_t * get_next_subset(uint64_t * list, const size_t size);

/* STR LIST LEN
 *		returns the size of a null-terminated list of pointers.
 *
 * Development status:
 *		Implemented
 */
int str_list_len(const char * const list[]);

/* INT LIST LEN
 *		returns the length of a negative-terminated int list.
 *
 * Development status:
 *		Implemented
 */
int int_list_len(const int * const list);

/* NOT NUL INT LIST LEN
 *		returns the number of positive ints in a negative-terminated list
 *
 * Development status:
 *		Implemented
 */
int not_null_int_list_len(const int * const list);

/* ARE INT LISTS EQUAL
 *		returns true if all elements of two negative-terminated lists are equal
 *
 * Development status:
 * 		Implemented.
 */
bool are_int_lists_equal(const int * const a, const int * const b);

/* FIND FIRST POSITIVE
 *		Returns the index of the first positive value in a non-negative int list.
 *
 * Returns -1 in case that there was no such element.
 *
 * Development status:
 *		Implemented
 */
int find_first_positive(const int * const list);

/* FIND LAST POSITIVE
 * 		Returns the index of the last positive value in a non-negative int list.
 *
 * Returns -1 in case that there was no such element.
 *
 * Development status:
 *		Implemented
 */
int find_last_positive(const int * const list);

/* FIND MAX INT
 *		Returns the index of the largest value in a non-negative int list.
 *
 * Development status:
 *		Implemented
 */
int find_max_int(const int * const list);

/* ORDER BY VALUE
 *		Orders indexes according to value in a list
 *
 * Returns a list of indexes.
 *
 * Development status:
 * 		Implemented.
 */
int find_max_int_index(const int * const list);

/* ORDER BY VALUE
 *		Orders indexes according to value in a list
 *
 * Returns a list of indexes.
 *
 * Development status:
 * 		Implemented.
 */
int * order_by_value(const int * const values);

/* ORDER BY VALUE NOT ZERO
 *		returns a list of indexes, by ascending order.
 *
 * The list returned may end before values's list, because it
 * does not include indexes s.t. values[list[i]] = 0.
 *
 * Development status:
 *		Implemented.
 */
int * order_by_value_not_zero(const int * const values);

/* INT LIST INTERSEC SIZE
 *		calculates the number of times taht a[i] > 0 & b[i] > 0.
 *
 * Development status:
 *		Implemented.
 */
void order_by_rank_not_null(int * list, int * rank);

/* INT LIST INTERSEC SIZE
 *		calculates the number of times taht a[i] > 0 & b[i] > 0.
 *
 * Development status:
 *		Implemented.
 */
int int_list_intersec_size(const int * const list_a, const int * const list_b);

#endif /* MATHS_H */
