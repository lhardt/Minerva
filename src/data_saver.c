#include <stdio.h>
#include <sqlite3.h>

/* TODO: requer SQL> PRAGMA FOREIGN_KEYS = ON */

const char * const CREATE_SCHOOL_TABLE =
	"CREATE TABLE School("
	" id integer primary key,"
	" name text,"
	" n_class_days integer,"
	" n_periods_per_day integer,"
	" max_meetings_teacher_per_week integer,"
	" max_meetings_teacher_per_day integer,"
	" max_gemini_classes"
	")";

const char * const CREATE_TEACHER_TABLE =
	"CREATE TABLE Teacher("
	" id integer primary key,"
	" max_meetings_per_day integer,"
	" periods blob"
	")";

const char * const CREATE_CLASS_TABLE =
	"CREATE TABLE Class("
	" id integer primary key,"
	" name text,"
	" periods blob"
	")";

const char * const CREATE_TEACHER_CLASS_TABLE =
	"CREATE TABLE TeacherClass("
	" id integer primary key,"
	" id_teacher integer,"
	" id_class integer,"
	" quantity integer,"
	" FOREIGN KEY(id_teacher) REFERENCES teacher(id),"
	" FOREIGN KEY(id_class)   REFERENCES class(id)"
	")";

int database_execute(const char * filename, const char * const statement){
	int err = 0;
	sqlite3 * database;

	err = sqlite3_open(filename, &database);
	if(err != 0){
		printf("Sqlite 3 Error. Code: %d, Message: %s\n",
				sqlite3_errcode(database),
				sqlite3_errmsg(database));
	}
	err = sqlite3_exec(database, statement, NULL, NULL, NULL);
	if(err != 0){
		printf("Sqlite 3 Error. Code: %d, Message: %s\n",
				sqlite3_errcode(database),
				sqlite3_errmsg(database));
	}

	sqlite3_close(database);
	return err;
}
