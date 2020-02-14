#ifndef ACTIONS_H
#define ACTIONS_H

#include "definitions.h"

typedef enum EditActionActionType {

} EditActionActionType;

typedef enum EditActionObjectType {

} EditActionObjectType;

typedef struct EditAction {
	/* Unique id for every action */
	int id;

	EditActionActionType action;
	EditActionObjectType object;
	void * data_before;
	void * data_after;
};

void undo_action(Universe * u);

void redo_action(Universe * u);

void make_action(Universe * u, EditAction * action);


#endif /* ACTIONS_H*/
