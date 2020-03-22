#include "types.h"
#include "decisions.h"
#include "primary_logic.h"

#include <stdlib.h>
#include <stdio.h>

static const int  FIRST_ALLOC_SZ = 100;
static const int EXPAND_ALLOC_SZ = 100;

static int g_TREE_LASTID = 0;

Teacher ** make_possible_teacher_list(School * school, Meeting * meeting){
	int i_teacher, i_teaches, i_list = 0;
	/* Reference to shorten indirection. */
	Teacher * teacher;

	Teacher ** list = calloc(school->n_teachers, sizeof(Teacher*));

	for(i_teacher = 0; i_teacher < school->n_teachers; i_teacher++){
		teacher = &(school->teachers[i_teacher]);
		for(i_teaches = 0; teacher->teaches[i_teaches] == NULL; i_teaches++){
			if(teacher->teaches[i_teaches]->subject == meeting->subj){
				list[i_list] = teacher;
				i_list++;
			}
		}
	}
	list[i_list] = NULL;

	return list;
}

Room ** make_possible_room_list(School * school, Meeting * meeting){
	int i_room, i_rfeat, i_list = 0, i_teaches;
	bool suitable;
	/* Reference to shorten indirection. */
	Room * room;

	Room ** list = calloc(school->n_rooms, sizeof(Room*));
	Teaches * teaches = NULL;

	for(i_teaches = 0; meeting->teacher->teaches[i_teaches] != NULL; i_teaches++ ){
		if( meeting->teacher->teaches[i_teaches]->subject  == meeting->subj){
			teaches = meeting->teacher->teaches[i_teaches];
			break;
		}
	}

	if(teaches == NULL){
		printf("Error: Teacher doesn't lecture on that subject!\n");
		return NULL;
	}

	for(i_room = 0; i_room < school->n_rooms; i_room++){
		room = &(school->rooms[i_room]);
		suitable = true;
		for(i_rfeat = 0; suitable && room->room_features[i_rfeat] >= 0; i_rfeat++){
			suitable &= (room->room_features[i_rfeat] >= teaches->min_features[i_rfeat]);
		}
		if(suitable){
			list[i_list] = room;
			i_list++;
		}
	}
	return list;
}

int * make_possible_period_list(School * school, Meeting * meeting){
	int i_per;

	int * possible_periods = calloc(school->n_periods, sizeof(int));

	for(i_per = 0; i_per < school->n_periods; i_per++){

		// TODO: melhorar este algorítmo. No momento,
		// ele calcula o mínimo entre os dois valores
		possible_periods[i_per] = meeting->class->periods[i_per];
	}
	return possible_periods;
}

DecisionTree * init_decision_tree(School * school){
	int n_meetings, i_class, i_meet = 0, i_need;
	/* References to shorten indirection. */
	Class * class;
	Meeting * conclusion;


	DecisionTree * tree = calloc(1, sizeof(DecisionTree));

	tree->alloc_sz = FIRST_ALLOC_SZ;

	tree->start = calloc(tree->alloc_sz, sizeof(DecisionNode));
	tree->start[0].id = ++g_TREE_LASTID;
	tree->last_index = tree->start[0].id;

	tree->start[0].type = NODE_START;
	tree->start[0].parent = NULL;
	tree->start[0].owner = tree;

	n_meetings = count_required_meetings(school, NULL, NULL);

	tree->start[0].conclusion = calloc(n_meetings + 1, sizeof(Meeting));

	conclusion = tree->start[0].conclusion;

	for(i_class = 0; i_class < school->n_classes; i_class++){
		class = &(school->classes[i_class]);
		for(i_need = 0; class->needs[i_need].subject != NULL; i_need++){
			conclusion[i_meet].class = class;
			conclusion[i_meet].subj  = class->needs[i_need].subject;
			conclusion[i_meet].teacher = NULL;
			conclusion[i_meet].room = NULL;
			conclusion[i_meet].period = -1;

			conclusion[i_meet].possible_teachers = make_possible_teacher_list(school,&conclusion[i_meet]);
			// conclusion[i_meet].possible_rooms = make_possible_room_list(school, &conclusion[i_meet]);
			conclusion[i_meet].possible_periods = make_possible_period_list(school,&conclusion[i_meet]);

		}
	}
	return tree;
}
