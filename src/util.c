/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include "types.h"

void print_int_list(FILE * out, const int * const list){
	int i = 0;

	if(list == NULL){
		fprintf(out, "(nul)");
		return;
	}
	if(list[0] != -1){
		fprintf(out, "[%d", list[0]);
		for(i = 1; list[i] != -1; i++){
			fprintf(out, ", %d", list[i]);
		}
		fprintf(out,"]");
	} else {
		fprintf(out, "[]");
	}
}

void print_sized_int_list(FILE * out, const int * const list, const int size){
	int i = 0;

	if(list == NULL){
		fprintf(out, "(nul)");
		return;
	}
	if(size > 0){
		fprintf(out, "[%d", list[0]);
		for(i = 1; i < size; i++){
			fprintf(out, ", %d", list[i]);
		}
		fprintf(out,"]");
	} else {
		fprintf(out, "[]");
	}
}

void print_bool_list(FILE * out, const bool * const list, const int size){
	int i = 0;
	if(list == NULL){
		fprintf(out, "(nul)");
		return;
	}
	if(size == 0){
		fprintf(out, "[]");
	}
	if(size > 0){
		fprintf(out,"[%c", list[i]?'t':'f');
		for(i = 1; i < size; i++){
			printf(", %c", list[i]?'t':'f');
		}
		fprintf(out,"]");
	}
}

void print_meeting_list(FILE * out, const Meeting * const meetings){
	int i = 0;

	if(meetings == NULL){
		printf("Returning on null");
		return;
	}

	for(i = 0; meetings[i].class != NULL; i++){
		fprintf(out, "Meeting %2d %s: T(%s) ",
					i,
					meetings[i].class->name,
					(meetings[i].teacher == NULL? "":meetings[i].teacher->name));
	    print_int_list(out,meetings[i].possible_teachers);
		fprintf(out, "; R(%s) ", meetings[i].room ==NULL? "":meetings[i].room->name);
		print_int_list(out,meetings[i].possible_rooms);
		fprintf(out, "; P(%d) ", meetings[i].period);
		print_int_list(out,meetings[i].possible_periods);
		fprintf(out,"\n");
	}
}

void print_short_meeting_list(FILE * out, const Meeting * const meetings){
	int i = 0;

	if(meetings == NULL){
		printf("Returning on null");
		return;
	}

	for(i = 0; meetings[i].class != NULL; i++){
		fprintf(out, "Meeting %2d %s: T(%s) ",
					i,
					meetings[i].class->name,
					(meetings[i].teacher == NULL? "":meetings[i].teacher->name));
		fprintf(out, "; R(%s) ", meetings[i].room ==NULL? "":meetings[i].room->name);
		fprintf(out, "; P(%d) ", meetings[i].period);
		fprintf(out,"\n");
	}
}

Meeting * copy_meetings_list(const Meeting * const list){
	Meeting * copy;
	int i, n;
	for(n = 0; list[n].class != NULL; ++n){ }

	copy = calloc(n+1, sizeof(Meeting));
	for(i = 0; i < n; ++i){
		copy[i] = list[i];
		copy[i].possible_periods = int_list_copy(copy[i].possible_periods);
		copy[i].possible_rooms = int_list_copy(copy[i].possible_rooms);
		copy[i].possible_teachers = int_list_copy(copy[i].possible_teachers);
	}
	return copy;
}
/* INT LIST COPY
 *		allocates an exact copy of the null-terminated int list.
 *
 * Development status:
 *		Implemented.
 */
int * int_list_copy(const int * const list){
	int i, n, *copy;

	for(n = 0; list[n] >= 0; ++n){ }

	copy = calloc(n+1, sizeof(int));

	for(i = 0; i <= n; ++i){
		copy[i] = list[i];
	}
	return copy;
}

void free_meetings_list(Meeting * list){
	int i_met;
	if(list != NULL){
		for(i_met = 0; list[i_met].class != NULL; ++i_met){
			free(list[i_met].possible_teachers);
			free(list[i_met].possible_periods);
			free(list[i_met].possible_rooms);
		}
	}
	free(list);
}

void free_node(DecisionNode * node){
	int i = 0;

	for(i = 0; i < node->n_children; ++i){
		free_node(&node->children[i]);
	}
	// if(node->children != NULL){
		free(node->children);
		free(node->children_score);
		free(node->children_score_order);
		free_meetings_list(node->conclusion);
	// }/
}
