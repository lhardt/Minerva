/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
#ifndef COMBINATORICS_H
#define COMBINATORICS_H

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdint.h>
 #include <stdbool.h>

#include "types.h"

 /* Calculates the result of P!/(P-T)! */
 uint64_t factorial_division(uint64_t p, uint64_t t);

 /* Calculates the result of N! */
 uint64_t factorial(uint64_t n);

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
 uint64_t * decompose(uint64_t elements, uint64_t n);

 /* Orders elements of a list descendingly */
 void order_elements_desc(uint64_t * list, size_t size);

 /* Orders elements of a list ascendingly */
 void order_elements_asc(uint64_t * list, size_t size);

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
  uint64_t * get_first_order(size_t size);

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
bool get_next_order(uint64_t * order, size_t size);

 uint64_t * get_first_subset(size_t size);

uint64_t * get_next_subset(uint64_t * list, size_t size);


int str_list_len(const char * const list[]);

int int_list_len(const int * const list);

int not_null_int_list_len(const int * const list);

bool equal_lists(int * a, int * b);

/* Returns the index of the first positive integer in a list terminated
 * by -1.
 */
int find_first_positive(int * list);

/* Returns the index of the last positive integer in a list terminated
 * by -1.
 */
int find_last_positive(int * list);

int find_max_int(int * list);

int find_max_int_index(int * list);

void order_by_rank(int * list, int * rank);

void order_by_rank_not_null(int * list, int * rank);

/* Number of indexes where list_a[i] > 0 and list[b] > 0 */
int intersec_size(int * list_a, int * list_b);

#endif /* COMBINATORICS_H */
