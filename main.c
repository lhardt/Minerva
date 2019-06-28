#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct Teacher {
	int id;
	char * name;
	// Goes until periodList[i] is negative.
	int * periodList;
	int * disciplines;
} Teacher;

typedef struct Class {
	int id;
	char * name;
	// Goes until periodList[i] is negative.
	int * periodList;
	int * disciplines;
} Class;

typedef struct TeacherClass {
	int idTeacher;
	int idClass;
	int period;
	int discipline;
} TeacherClass;

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

void print_int_list(int * list){
	int i = 0;
	printf("[");
	while(list[i] >= 0){
		printf("%d, ", list[i]);
		i++;
	}
	printf("];\n");
}

/* Removes the ith elements from the list */
int * remove_from_list(int * list, int i){
	int n = i;
	while(list[n] >= 0){
		list[n] = list[1+n];
		n++;
	}
}

/* Finds the element with that value in the list. */
int find_in_list(int * list, int val){
	int i = 0;
	while(list[i] >= 0 && list[i] != val)
		i++;
	return i;
}

/* Returns the id of each avalible teacher at one given period. */
int* get_avalible_teachers(Teacher * teach, int nteach, int period){
	int * avalible_teachers = malloc((1+nteach) * sizeof(int));
	int iteach = 0, j = 0, iaval = 0;
	for(iteach = 0; iteach < nteach; iteach++){
		for(j = 0; teach[iteach].periodList[j] >= 0; j++){
			if(teach[iteach].periodList[j] == period){
				avalible_teachers[iaval] = teach[iteach].id;
				iaval++;
				break;
			}
		}
	}
	avalible_teachers[iaval] = -1;
	return avalible_teachers;
}

/* Returns the id of each avalible class at one given period.  */
int* get_avalible_classes(Class * classes, int nclass, int period){
	int * avalible = calloc((1+nclass), sizeof(int));
	int iclass = 0, iper, iaval = 0;
	for(; iclass < nclass ; iclass++){
		for(iper = 0; classes[iclass].periodList[iper] > 0; iper++){
			if(period == classes[iclass].periodList[iper]){
				avalible[iaval] = classes[iclass].id;
				iaval++;
				break;
			}
		}
	}
	avalible[iaval] = -1;
	return avalible;
}

/* Prints the schedule */
void print_teacher_class_list(TeacherClass * list){
	for(int i = 0; list[i].idTeacher >= 0; i++){
		printf("List[%d]: Period<%d> Teacher<%d> Class<%d>\n", i, list[i].period, list[i].idTeacher, list[i].idClass);
	}
}

/* Get how many periods with a teacher a class still doesn't have*/
int get_remaining_periods(TeacherClass * list, Class class, int disc){
	int remaining = 0;
	// Get first how many periods of that discipline the class needs
	for(int i = 0; class.disciplines[i] >= 0; i++){
		if(class.disciplines[i] == disc){
			remaining++;
		}
	}
	for(int i = 0; list[i].idTeacher >= 0; i++){
		if( list[i].discipline == disc && list[i].idClass == class.id){
			remaining--;
		}
	}
	return remaining;
}

Class get_class_by_id(Class * list, int nlist, int id){
	for(int i = 0; i < nlist; i++){
		if(list[i].id == id)
			return list[i];
	}
	printf("Não achei...");
	return list[nlist];
}

Teacher get_teacher_by_id(Teacher * list, int nlist, int id){
	for(int i = 0; i < nlist; i++){
		if(list[i].id == id)
			return list[i];
	}
	printf("Não achei...");
	return list[nlist];
}

/* Creates a new blank schedule */
TeacherClass* new_teacher_class_list(int maxsize){
	TeacherClass * list = malloc(maxsize * sizeof(TeacherClass));
	for(int i = 0; i < maxsize; i++){
		list[i].discipline = -1;
		list[i].idClass = -1;
		list[i].period = -1;
		list[i].idTeacher = -1;
	}
	return list;
}

TeacherClass* schedule(Teacher * teach, Class * class, int * periods, int nteach, int nclass, int nper){
	/* Ideia de outro algoritmo:
	 *
 	 * Primeiro pegamos todas as possibilidades de relações professor-turma. Ex:
 	 * Professor X pode dar aula para turma A nos períodos {1,2,3,4}.
	 *
	 * Depois ordemanos essas constraints por quantidade de opções.
	 *
	 * Depois mantemos uma lista de opções. Ex:
	 * {"Tal constraint tinha opções 1,2,3. Escolhemos 1.",
	 *  "Tal constraint tinha opções 1,2,3. Escolhemos 5.",
 	 *  etc; }
	 *
	 * A partir daí temos um backtraking eficiente.
	*/
	TeacherClass * sched = new_teacher_class_list(nteach*nclass*nper);
	printf("scheduling...\n");
	int iclass, iteach, iper, nsched = 0;
	// For each period
	for(iper = 0; periods[iper] >= 0; iper++){
		int * avteachers = get_avalible_teachers(teach, nteach, periods[iper]);
		int * avclasses  = get_avalible_classes(class, nclass, periods[iper]);
		// For each free class
		for(iclass = 0; avclasses[iclass] >= 0; iclass++){
			// For each free teacher
			for(iteach = 0; avteachers[iteach] >= 0; iteach++){
				// TODO Só funciona com a primeira disciplina do professor.
				// if for that discipline, there are still classes remaining
				int remaining = get_remaining_periods(sched,
						get_class_by_id(class, nclass,avclasses[iclass]),
						get_teacher_by_id(teach, nteach, avteachers[iteach]).disciplines[0]);
				if( 0 < remaining){
					sched[nsched].idClass = avclasses[iclass];
					sched[nsched].idTeacher = avteachers[iteach];
					sched[nsched].period  = periods[iper];
					sched[nsched].discipline = get_teacher_by_id(teach, nteach, avteachers[iteach]).disciplines[0];
					remove_from_list(avteachers, iteach);
					remove_from_list(avclasses, iclass);
					iclass--; iteach--;
					nsched++;
					break;
				}
			}
		}
		free(avteachers);
		free(avclasses);
	}
	return sched;
}

int main(){
	int all_periods[] = {1,2,3,4 -1};
	int p1[] = {1,2,3,4, -1};
	int p2[] = {1,2,3,4, -1};
	int p3[] = {1,2,3,4, -1};

	int dclasses[] = {1,2,3,4, -1};

	int d1[]= {1};
	int d2[]= {2};
	int d3[]= {3};
	int d4[]= {4};
	// Ids altos pra saber se confundi com índices
	Class classes[] = {
		{.id=11, .name="DS1", .periodList=p1, .disciplines=dclasses},
		{.id=12, .name="EL1", .periodList=p2, .disciplines=dclasses},
		{.id=13, .name="AD1", .periodList=p3, .disciplines=dclasses},
	};
	Teacher teachers[] = {
		{.id=10, .name="Adão", .periodList=p1, .disciplines=d1},
		{.id=20, .name="Beto", .periodList=p2, .disciplines=d2},
		{.id=30, .name="Cris", .periodList=p3, .disciplines=d3},
		{.id=40, .name="Cris", .periodList=p3, .disciplines=d4},
	};
	TeacherClass * sched = schedule(teachers, classes, all_periods, 4, 3, 4);
	print_teacher_class_list(sched);
	return 0;
}
