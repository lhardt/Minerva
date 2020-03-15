#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "types.h"

bool init_all_tables(FILE* console_out, char * db_filename);

bool  load_school(char * db_filename, int id, School ** load_ptr);
bool  load_all_schools(char * db_filename, School ** load_ptr);

bool  save_state(char * db_filename, School * school);

bool  load_xml(char * xml_filename, School ** load_ptr);
bool  export_to_xml(char * xml_filename, School * school);

#endif /* DATA_LOADER_H */
