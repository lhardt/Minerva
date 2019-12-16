#ifndef LOGIC_H
#define LOGIC_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "definitions.h"

/* Testado. */
int get_number_of_meetings(ExtendedClass * ex, int n_classes){
	int n_meetings = 0, i_class = 0, i_tq = 0;
	ExtendedClass * c;

	for(; i_class < n_classes; i_class++){
		c = &(ex[i_class]);
		for(i_tq = 0; i_tq < c->teachers_size; i_tq++){
			n_meetings += c->teachers[i_tq].quantity;
		}
	}

	return n_meetings;
}

Meeting * initialize_all_meetings(
			  ExtendedClass * classes,
			  int n_classes){
 	int i_class = 0, i_teach = 0;
	int i_meeting = 0, i_quant = 0;
	int n_meeting = get_number_of_meetings(classes,n_classes);
	Meeting * meetings = calloc( n_meeting + 1, sizeof(Meeting));
	for(i_class = 0; i_class < n_classes; i_class++){
		for(i_teach = 0; i_teach < classes[i_class].teachers_size; i_teach++){
			TeacherQuantity tq = classes[i_class].teachers[i_teach];
			for(i_quant = 0; i_quant < tq.quantity; i_quant++){
				meetings[i_meeting].teacher = tq.teacher;
				meetings[i_meeting].class   = &classes[i_class];
				meetings[i_meeting].period = -1;
				i_meeting++;
			}
		}
	}
	return meetings;
}

void print_meeting_list(Meeting * meetings){
	int i = 0;
	while(meetings[i].teacher != NULL){
		printf("Meeting %2d: %-5s %-9s %d\n", i, meetings[i].class->name, meetings[i].teacher->name, meetings[i].period);
		i++;
	}
}


#endif /* LOGIC_H */
