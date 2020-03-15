/*
 * Project Minerva.
 *
 * Copyright (C) Léo H. 2019-2020.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "decisions.h"
#include "util.h"
#include "data_loader.h"

int main(){
	// printf("Hello, world.\n");
	init_all_tables(stdout, "db/test.db");
	return 0;
}
