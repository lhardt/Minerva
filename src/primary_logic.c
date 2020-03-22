
#include <stdio.h>

#include "primary_logic.h"

int count_required_meetings(
				School * school,
				Class * class,
				Subject * subject
				){
	int count = 0, i_class = 0, i_need = 0;
	if(school != NULL && school->classes != NULL && school->n_classes > 0){
		for(i_class = 0; i_class < school->n_classes; i_class++){
			if( (class == NULL) != (&school->classes[i_class] == class) ){
				for(i_need = 0; school->classes[i_class].needs[i_need].subject != NULL; i_need++){
					if((subject == NULL) != (school->classes[i_class].needs[i_need].subject == subject)){
						count += school->classes[i_class].needs[i_need].quantity;
					}
				}
			}
		}
	} else {
		printf("Count called with invalid school\n");
	}
	return count;
}
