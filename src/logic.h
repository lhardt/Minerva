#ifndef LOGIC_H
#define LOGIC_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "definitions.h"


int get_number_of_meetings(ExtendedClass * ex){
	int n_meetings = 0, i_class = 0, i_tq = 0;
	ExtendedClass * c;

	for(; ex[i_class].name != NULL; i_class++){
		c = &(ex[i_class]);
		for(i_tq = 0; c->teachers[i_tq].teacher != NULL; i_tq++){
			n_meetings += c->teachers[i_tq].quantity;
		}
	}

	return n_meetings;
}

Meeting * initialize_all_meetings(ExtendedClass * classes){
 	int i_class = 0, i_teach = 0;
	int i_meeting = 0, i_quant = 0;
	int n_meeting = get_number_of_meetings(classes);
	Meeting * meetings = calloc( n_meeting + 1, sizeof(Meeting));
	for(i_class = 0; classes[i_class].name != NULL; i_class++){
		for(i_teach = 0; classes[i_class].teachers[i_teach].teacher != NULL; i_teach++){
			TeacherQuantity tq = classes[i_class].teachers[i_teach];
			for(i_quant = 0; i_quant < tq.quantity; i_quant++){
				meetings[i_meeting].teacher = tq.teacher;
				meetings[i_meeting].class   = &classes[i_class];
				meetings[i_meeting].period = -1;
				/* Orders likeableness of this period based on the least
				 * score of the teacher and the class.
				 */
				meetings[i_meeting].possible_periods =
					get_meeting_score(
						meetings[i_meeting].teacher->periods,
						meetings[i_meeting].class->periods
				);

				i_meeting++;
			}
		}
	}
	return meetings;
}

void print_meeting_list(Meeting * meetings){
	int i = 0;
	while(meetings[i].teacher != NULL){
		printf("Meeting %2d: %-5s %-9s ", i, meetings[i].class->name, meetings[i].teacher->name, meetings[i].period);
		if(meetings[i].period != -1){
			printf("%d\n", meetings[i].period);
		} else {
			printf("[");
			for(int j = 0; meetings[i].possible_periods[j] >= 0; j++){
				printf("%d, ", meetings[i].possible_periods[j]);
			}
			printf("]\n");
		}

		i++;
	}
}



#endif /* LOGIC_H */
