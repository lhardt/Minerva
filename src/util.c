/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */
#include <stdio.h>
#include "types.h"
#include "util.h"

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
