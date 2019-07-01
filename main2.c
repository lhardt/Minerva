
/* 
 * Trying a version without periods. The solution would be
 * assigning teachers to classes before the program could
 * run.
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
	}
	return order;
}


typedef struct Teacher{
	char * name;
	int * period_list;
} Teacher;

typedef struct Class{
	char * name;
	struct TeacherQuantity{
		Teacher t;
		int n;
	} * teachers;
	int teachers_size;
} Class ;

typedef struct Meeting{
	Teacher t;
	Class c;
	int p;
} Meeting;

int count_all_classes(Class * c, int nclasses){
	int total = 0;
	for(int i = 0; i < nclasses; i++){
		for(int j = 0; j < c[i].teachers_size; j++){
			total += c[i].teachers[j].n;
		}
	}
	return total;
}

int int_list_size(int * list){
	int sz = 0;
	while(list[sz] >= 0) {
		sz++;
	}
	return sz;
}

int*** get_possible_meetings(Teacher * teachers, Class * classes, int nteach, int nclasses, int nperiods){
	Meeting * meetings = calloc( count_all_classes(classes, nclasses), sizeof(Meeting) );
	int *** possible_meetings = calloc(nclasses, sizeof(int**));
			
	printf("\ninf\n");
	for(int i  = 0; i < nclasses; i++){
		possible_meetings[i] = calloc(classes[i].teachers_size, sizeof(int*));
		for(int j = 0; j < classes[i].teachers_size; j++){
			Teacher t  = classes[i].teachers[j].t;
			int sz = int_list_size(t.period_list) + 1;
			possible_meetings[i][j] = calloc(sz, sizeof(int));
			
			for(int k = 0; k < sz; k++){
				possible_meetings[i][j][k] = t.period_list[k];
				

			}
		}
	}
}

void print_possible_meetings(int *** possible, Teacher * teachers, Class * classes, int nteach, int nclasses, int nperiods){
	for(int i = 0; i < nclasses; i++){
		for(int j = 0; j < classes[i].teachers_size; j++){
			printf("Possible meetings for class %s and %s (needed: %d) are [",
				 classes[i].name, classes[i].teachers[j].t.name, classes[i].teachers[j].n );
			for(int k = 0; possible_meetings[i][j][k] > 0; k++){
				printf("%d, ", possible_meetings[i][j][k]);
			}
			printf("];\n");
		}	
		printf("\n\n");
	}
}

int main(int argc, char ** argv){

	int p1[] = {1,2,3,4, -1};
	int p2[] = {1,2,3,4, -1};
	int p3[] = {1,2,3,4, -1};

	int dclasses[] = {1,2,3,4, -1};

	Teacher teachers[] = {
		{.name="Adão", .period_list=p1},
		{.name="Beto", .period_list=p2},
		{.name="Cris", .period_list=p3},
		{.name="Cris", .period_list=p3},
	};
	
	struct TeacherQuantity tc1[] = {
		{.t=teachers[0], .n=2}, {.t=teachers[2], .n=1},
		{.t=teachers[1], .n=1}, {.t=teachers[3], .n=1}	};

	struct TeacherQuantity tc2[] = {
		{.t=teachers[0], .n=1}, {.t=teachers[2], .n=1},
		{.t=teachers[1], .n=1}, {.t=teachers[3], .n=1}	};
	Class classes[] = {
		{.name="DS1", .teachers_size=4, .teachers=tc2},
		{.name="EL1", .teachers_size=4, .teachers=tc2},
		{.name="AD1", .teachers_size=4, .teachers=tc1},
	};
	
	Meeting * sched = solve(teachers, classes, 4, 3, 4);
	return 0;
}