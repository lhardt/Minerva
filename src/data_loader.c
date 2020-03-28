
#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include "data_loader.h"

/* TODO: Assure PRAGMA foreign_keys = on */

const char * const CREATE_TABLE_SCHOOL =
			("CREATE TABLE IF NOT EXISTS School("
				"id						integer primary key,"
				"name					text,"
				"num_per_per_day		integer,"
				"num_days				integer,"
				"max_per_per_day		integer,"
				"max_per_per_week		integer,"
				"max_doubling			integer"
			")");
const char * const INSERT_TABLE_SCHOOL =
			("INSERT INTO School values(?,?,?,?,?,?)");
const char * const LASTID_TABLE_SCHOOL =
			("SELECT id FROM School where rowid = last_insert_rowid()");


const char * const CREATE_TABLE_DAILY_PERIOD =
			("CREATE TABLE IF NOT EXISTS DailyPeriod("
				"id 					integer primary key,"
				"name					text,"
				"index					integer,"
				"school_id				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_DAILY_PERIOD =
			("INSERT INTO DailyPeriod values(?,?,?,?)");
const char * const LASTID_TABLE_DAILY_PERIOD =
			("SELECT id FROM DailyPeriod where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_DAY =
			("CREATE TABLE IF NOT EXISTS Day("
				"id 					integer primary key,"
				"name					text,"
				"index					integer,"
				"school_id				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_DAY =
			("INSERT INTO Day values(?,?,?,?)");
const char * const LASTID_TABLE_DAY =
			("SELECT id FROM Day where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_PERIOD =
			("CREATE TABLE IF NOT EXISTS Period("
				"id 					integer primary key,"
				"name					text,"
				"school_operates_flag   integer,"
				"day_id					integer,"
				"school_id				integer,"
				"daily_period_id		integer,"
				"FOREIGN KEY (day_id) REFERENCES Day(id),"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_PERIOD =
			("INSERT INTO Period values(?,?,?,?,?,?)");
const char * const LASTID_TABLE_PERIOD =
			("SELECT id FROM Period where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_FEATURE =
			("CREATE TABLE IF NOT EXISTS Feature("
				"id 					integer primary key,"
				"name 					text"
			")");
const char * const INSERT_TABLE_FEATURE =
			("INSERT INTO Feature VALUES(?,?)");
const char * const LASTID_TABLE_FEATURE =
			("SELECT id FROM Feature where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_ROOM =
			("CREATE TABLE IF NOT EXISTS Room("
				"id 					integer primary key,"
				"name 					text,"
				"short_name 			text,"
				"school_id 				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_ROOM =
			("INSERT INTO Room VALUES(?,?,?,?)");
const char * const LASTID_TABLE_ROOM =
			("SELECT id FROM Room where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_ROOM_FEATURE =
			("CREATE TABLE IF NOT EXISTS RoomFeature("
				"id						integer primary key,"
				"id_room				integer,"
				"id_feature				integer,"
				"score					integer,"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"FOREIGN KEY (id_feature) REFERENCES Feature(id)"
			")");
const char * const INSERT_TABLE_ROOM_FEATURE =
			("INSERT INTO RoomFeature VALUES (?,?,?)");
const char * const LASTID_TABLE_ROOM_FEATURE =
			("SELECT id FROM RoomFeature where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_ROOM_AVALIBILITY =
			("CREATE TABLE IF NOT EXISTS RoomAvalibility("
				"id						integer primary key,"
				"room_id				integer,"
				"period_id				integer,"
				"score					integer,"
				"FOREIGN KEY (room_id) REFERENCES Room(id),"
				"FOREIGN KEY (period_id) REFERENCES Period(id)"
			")");
const char * const INSERT_TABLE_ROOM_AVALIBILITY =
			("INSERT INTO RoomAvalibility values(?,?,?)");
const char * const LASTID_TABLE_ROOM_AVALIBILITY =
			("SELECT id FROM RoomAvalibility where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_CLASS =
			("CREATE TABLE IF NOT EXISTS Class("
				"id 					integer primary key,"
				"name 					text,"
				"short_name 			text,"
				"size 					integer,"
				"abstract				integer,"
				"free_periods_flag 		integer,"
				"per_max_entrance 		integer,"
				"per_min_exit 			integer,"
				"school_id  			integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_CLASS =
			("INSERT INTO Class VALUES (?,?,?,?,?,?,?)");
const char * const LASTID_TABLE_CLASS =
			("SELECT id FROM Class where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_CLASS_ATTENDANCE =
			("CREATE TABLE IF NOT EXISTS ClassAttendance("
				"id 					integer primary key,"
				"id_class				integer,"
				"id_period				integer,"
				"id_att_type			integer,"
				"score      			integer,"
				"FOREIGN KEY (id_class) REFERENCES Class(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id)"
				"FOREIGN KEY (id_att_type) REFERENCES AttendanceType(id)"
			")");
const char * const INSERT_TABLE_CLASS_ATTENDANCE =
			("INSERT INTO ClassAttendance values(?,?,?,?)");
const char * const LASTID_TABLE_CLASS_ATTENDANCE =
			("SELECT id FROM ClassAttendance where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_CLASS_SUBORDINATION =
			("CREATE TABLE IF NOT EXISTS ClassSubordination("
				"id						integer primary key,"
				"id_sub					integer,"
				"id_sup					integer,"
				"FOREIGN KEY (id_sub) REFERENCES Class(id),"
				"FOREIGN KEY (id_sup) REFERENCES Class(id)"
			")");
const char * const INSERT_TABLE_CLASS_SUBORDINATION =
			("INSERT INTO ClassSubordination VALUES(?,?,?)");
const char * const LASTID_TABLE_CLASS_SUBORDINATION =
			("SELECT id FROM ClassSubordination where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_SUBJECT =
			("CREATE TABLE IF NOT EXISTS Subject("
				"id						integer primary key,"
				"name					text,"
				"short_name				text"
			")");
const char * const INSERT_TABLE_SUBJECT =
			("INSERT INTO Subject VALUES (?,?,?)");
const char * const LASTID_TABLE_SUBJECT =
			("SELECT id FROM Subject where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_CLASS_SUBJECT =
			("CREATE TABLE IF NOT EXISTS ClassSubject("
				"id 					integer primary key,"
				"class_id				integer,"
				"subject_id				integer,"
				"amount					integer,"
				"FOREIGN KEY (class_id) REFERENCES Class(id),"
				"FOREIGN KEY (subject_id) REFERENCES Subject(id)"
			")");
const char * const INSERT_TABLE_CLASS_SUBJECT =
			("INSERT INTO ClassSubject VALUES (?,?,?,?)");
const char * const LASTID_CLASS_SUBJECT =
			("SELECT id FROM ClassSubject where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_TEACHER =
			("CREATE TABLE IF NOT EXISTS Teacher("
				"id 					integer primary key,"
				"name					text,"
				"short_name				text,"
				"max_per_per_day		integer,"
				"max_per				integer,"
				"num_per_planning		integer,"
				"school_id				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_TEACHER =
			("INSERT INTO Teacher VALUES (?,?,?,?,?,?,?)");
const char * const LASTID_CLASS_TEACHER =
			("SELECT id FROM Teacher where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_TEACHES =
			("CREATE TABLE IF NOT EXISTS Teaches("
				"id 					integer primary key,"
				"id_teacher				integer,"
				"id_subject				integer,"
				"score  				integer," // NEW FIELD. ADD TO DIAGRAM
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_subject) REFERENCES Subject(id)"
			")");
const char * const INSERT_TABLE_TEACHES =
			("INSERT INTO Teaches VALUES (?,?,?)");
const char * const LASTID_CLASS_TEACHES =
			("SELECT id FROM Teaches where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_TEACHER_SUBORDINATION =
			("CREATE TABLE IF NOT EXISTS TeacherSubordination("
				"id						integer primary key,"
				"id_sub					integer,"
				"id_sup					integer,"
				"FOREIGN KEY (id_sub) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_sup) REFERENCES Teacher(id)"
			")");
const char * const INSERT_TABLE_TEACHER_SUBORDINATION =
			("INSERT INTO TeacherSubordination VALUES (?,?,?)");
const char * const LASTID_CLASS_TEACHER_SUBORDINATION =
			("SELECT id FROM TeacherSubordination where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_TEACHER_ATTENDANCE =
			("CREATE TABLE IF NOT EXISTS TeacherAttendance("
				"id						integer primary key,"
				"id_teacher				integer,"
				"id_period				integer,"
				"id_att_type			integer,"
				"score					integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id)"
				"FOREIGN KEY (id_att_type) REFERENCES AttendanceType(id)"
			")");
const char * const INSERT_TABLE_TEACHER_ATTENDANCE =
			("INSERT INTO TeacherAttendance VALUES (?,?,?,?,?)");
const char * const LASTID_CLASS_TEACHER_ATTENDANCE =
			("SELECT id FROM TeacherAttendance where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_ATTENDANCE_TYPE =
			("CREATE TABLE IF NOT EXISTS AttendanceType("
				"id						integer primary key,"
				"name					integer"
			")");
const char * const INSERT_TABLE_ATTENDANCE_TYPE =
			("INSERT INTO AttendanceType VALUES(?,?)");
const char * const LASTID_CLASS_TEACHER_TYPE =
			("SELECT id FROM AttendanceType where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_DEMAND =
			("CREATE TABLE IF NOT EXISTS Demand("
				"id						integer primary key,"
				"teaches_id				integer,"
				"feature_id				integer,"
				"min_score				integer,"
				"score  				integer,"
				"FOREIGN KEY (teaches_id) REFERENCES Teaches(id),"
				"FOREIGN KEY (feature_id) REFERENCES Feature(id)"
			")");
const char * const INSERT_TABLE_DEMAND =
			("INSERT INTO Demand VALUES (?,?,?)");
const char * const LASTID_CLASS_DEMAND =
			("SELECT id FROM Demand where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_TEACHES_PERIOD_PREFERENCE =
			("CREATE TABLE IF NOT EXISTS TeachesPeriodPreference("
				"id 					integer primary key,"
				"id_teaches				integer,"
				"id_period				integer,"
				"score					integer,"
				"FOREIGN KEY (id_teaches) REFERENCES Teaches(id),"
				"FOREIGN KEY (id_period)  REFERENCES Period(id)"
			")");
const char * const INSERT_TABLE_TEACHES_PERIOD_PREFERENCE =
			("INSERT INTO TeachesPeriodPreference VALUES (?,?,?,?)");
const char * const LASTID_CLASS_TEACHES_PERIOD_PREFERENCE =
			("SELECT id FROM TeachesPeriodPreference where rowid = last_insert_rowid()");

const char * const CREATE_TABLE_TEACHES_TWIN_PREFERENCE =
			("CREATE TABLE IF NOT EXISTS TeachesTwinPreference("
				"id 					integer primary key,"
				"id_teaches				integer,"
				"twin_val				integer,"
				"score					integer,"
				"FOREIGN KEY (id_teaches) REFERENCES Teaches(id)"
			")");
const char * const INSERT_TABLE_TEACHES_TWIN_PREFERENCE =
			("INSERT INTO TeachesTwinPreference values (?,?,?,?)");
const char * const LASTID_CLASS_TEACHES_TWIN_PREFERENCE =
			("SELECT id FROM TeachesTwinPreference where rowid = last_insert_rowid()");



/**
 * Creates a table based on one of the strings above.
 *
 * @param console_out: Buffer where warnings and messages are printed;
 * @param db: The database in which the data is created;
 * @param tablename: Table name. Printed to console_out;
 *
 * return: SQLite error code in case of failure.
 **/
int create_table_test(FILE * console_out, sqlite3* db,const char * const tablename, const char * const sql){
	char * errmsg;
	int errc;
	if(console_out){
		fprintf(console_out,"Creating, if not existant, table %s.\n", tablename);
	}
	errc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	if(errc != 0 && console_out){
		fprintf(console_out,"Errc %d %s\n", errc, sqlite3_errmsg(db));
		fprintf(console_out,"Errmsg %s\n", errmsg);
	}
	return errc;
}

/**
 * SQLite Callback to get the id of a newly inserted row.
 *
 * Parameters related to SQL.
 * @param id_field_ptr: the pointer at which the id data will be written
 */
int get_id_callback(void* id_field_ptr,int no_columns,char** text_columns,char** name_columns){
	if(no_columns == 1 && id_field_ptr != NULL){
		*((int *)id_field_ptr) = strtol(text_columns[0], NULL,10);
		printf("%s: %d\n", name_columns[0], *((int *)id_field_ptr));
		return true;
	}
	printf("CALLBACK FUNCTION MISUSE!");
	return false;

}

// bool insert_room(FILE * console_out, sqlite3* db, School * school) {
//
// 	sqlite3_stmt * stmt;
// 	int i;
// 	int errc;
//
// 	sqlite3_prepare(db, INSERT_TABLE_FEATURE, -1, &stmt, NULL);
// 	for(i = 0; i < school->n_features; i++){
// 		sqlite3_bind_text(stmt,2, school->feature_names[i], -1, SQLITE_TRANSIENT);
// 		sqlite3_bind_text(stmt,3, school->feature_names[i], -1, SQLITE_TRANSIENT);
// 		sqlite3_bind_text(stmt,4, school->feature_names[i], -1, SQLITE_TRANSIENT);
// 		sqlite3_bind_text(stmt,5, school->feature_names[i], -1, SQLITE_TRANSIENT);
//
// 		errc = sqlite3_step(stmt);
// 		errc |= sqlite3_exec(db, LASTID_TABLE_FEATURE, get_id_callback, &(school->feature_ids[i]), NULL);
//
// 		if(errc != SQLITE_DONE){
// 			if(console_out){
// 				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
// 			}
// 			return false;
// 		}
//
// 		sqlite3_reset(stmt);
// 	}
// 	sqlite3_finalize(stmt);
// 	return true;
// 		// "id 					integer primary key,"
// 		// "name 					text,"
// 		// "short_name 			text,"
// 		// "school_id 				integer,"
// }

/**
 * Inserts in the database all features in a school
 */

bool insert_feature(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i;
	int errc;

	sqlite3_prepare(db, INSERT_TABLE_FEATURE, -1, &stmt, NULL);
	for(i = 0; i < school->n_features; i++){
		sqlite3_bind_text(stmt,2, school->feature_names[i], -1, SQLITE_TRANSIENT);

		errc = sqlite3_step(stmt);
		errc |= sqlite3_exec(db, LASTID_TABLE_FEATURE, get_id_callback, &(school->feature_ids[i]), NULL);

		if(errc != SQLITE_DONE){
			if(console_out){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			}
			return false;
		}

		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
	return true;
}

/**
 * Inserts in the database all periods in a school
 */
bool insert_period(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i;
	int errc;

	sqlite3_prepare(db, INSERT_TABLE_PERIOD, -1, &stmt, NULL);
	for(i = 0; i < school->n_periods; i++){
		sqlite3_bind_text(stmt,2, school->period_names[i], -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt,3, i % school->n_periods_per_day);
		printf("Period: NPer %d NDay Theory %d DayId %d\n", i, i / school->n_periods_per_day, school->day_ids[i / school->n_periods_per_day]);
		sqlite3_bind_int(stmt,4, school->day_ids[i / school->n_periods_per_day	]);
		sqlite3_bind_int(stmt,5, school->id);
		sqlite3_bind_int(stmt,6, school->periods[i]);
		errc = sqlite3_step(stmt);
		errc |= sqlite3_exec(db, LASTID_TABLE_PERIOD, get_id_callback, &(school->period_ids[i]), NULL);

		if(errc != SQLITE_DONE){
			if(console_out){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			}
			return false;
		}

		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
	return true;
}

/**
 * Inserts in the database all daily period name from a School Data Structure.
 */
bool insert_daily_period(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i;
	int errc;

	sqlite3_prepare(db, INSERT_TABLE_DAILY_PERIOD, -1, &stmt, NULL);
	for(i = 0; i < school->n_periods_per_day; i++){
		sqlite3_bind_text(stmt,2, school->daily_period_names[i], -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt,3, i);
		sqlite3_bind_int(stmt,4, school->id);
		errc = sqlite3_step(stmt);
		errc |= sqlite3_exec(db, LASTID_TABLE_DAILY_PERIOD, get_id_callback, &(school->daily_period_ids[i]), NULL);

		if(errc != SQLITE_DONE){
			if(console_out){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			}
			return false;
		}

		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
	return true;
}

/**
 * Inserts in the database all day names from a School Data Structure.
 */
bool insert_day(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i;
	int errc;

	sqlite3_prepare(db, INSERT_TABLE_DAY, -1, &stmt, NULL);
	for(i = 0; i < school->n_days; i++){
		sqlite3_bind_text(stmt,2, school->day_names[i], -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt,3, i);
		sqlite3_bind_int(stmt,4, school->id);
		errc = sqlite3_step(stmt);
		errc |= sqlite3_exec(db, LASTID_TABLE_DAY, get_id_callback, &(school->day_ids[i]), NULL);
		// TODO: ATOMIC END.

		if(errc != SQLITE_DONE){
			if(console_out){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			}
			return false;
		}

		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
	return true;
}

/**
 * Inserts in the table all the day names in the School Data Structure.
 */
bool insert_school(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int errc;

	if(school){
		if(console_out && (school->name)){
			fprintf(console_out, "Inserting school (%s) into school table.\n", school->name);
		}
		errc = sqlite3_prepare(db, INSERT_TABLE_SCHOOL, -1, &stmt, NULL);

		if(errc != 0){
			if(console_out){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			}
			return false;
		}

		sqlite3_bind_text(stmt,2, school->name, -1,SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt,3, school->n_periods_per_day);
		sqlite3_bind_int(stmt,4, school->max_meetings_teacher_per_day);
		sqlite3_bind_int(stmt,5, school->max_meetings_teacher_per_week);
		sqlite3_bind_int(stmt,5, school->max_gemini_classes);

		// TODO: ATOMIC START
		errc = sqlite3_step(stmt);
		sqlite3_exec(db, LASTID_TABLE_SCHOOL, get_id_callback, &(school->id), NULL);
		// TODO: ATOMIC END.

		sqlite3_finalize(stmt);

		school->day_ids = calloc(school->n_days, sizeof((*school->day_ids)));
		school->daily_period_ids = calloc(school->n_periods_per_day, sizeof((*school->daily_period_ids)));
		school->period_ids = calloc(school->n_periods, sizeof((*school->period_ids)));
		school->feature_ids = calloc(school->n_features, sizeof((*school->feature_ids)));
		insert_day(console_out, db, school);
		insert_daily_period(console_out, db, school);
		insert_period(console_out, db, school);
		insert_feature(console_out, db, school);

		if(console_out){
			if(errc != SQLITE_DONE){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			} else {
				fprintf(console_out, "Inserted school with id (%d)\n", school->id);
			}
			return false;
		}
		return true;
	}
	return true;
}

/**
 * Tries to recreate every table. Ignores if table already exists.
 */
bool init_all_tables(FILE * console_out, char * db_filename){

	sqlite3 * db;
	if(console_out){
		fprintf(console_out,"Opening database.\n");
	}
	int errc = sqlite3_open(db_filename, &db);
	if(errc != SQLITE_OK){
		printf("Errc %d %s\n", errc, sqlite3_errmsg(db));
		return 1;
	}
	bool iserr;
	iserr = create_table_test(console_out,db,"School", CREATE_TABLE_SCHOOL)?1:
			create_table_test(console_out,db,"DailyPeriod", CREATE_TABLE_DAILY_PERIOD)?1:
			create_table_test(console_out,db,"Day", CREATE_TABLE_DAY)?1:
			create_table_test(console_out,db,"Period", CREATE_TABLE_PERIOD)?1:
			create_table_test(console_out,db,"AttendanceType", CREATE_TABLE_ATTENDANCE_TYPE)?1:
			create_table_test(console_out,db,"RoomAvalibility", CREATE_TABLE_ROOM_AVALIBILITY)?1:
			create_table_test(console_out,db,"Feature", CREATE_TABLE_FEATURE)?1:
			create_table_test(console_out,db,"Room", CREATE_TABLE_ROOM)?1:
			create_table_test(console_out,db,"Class", CREATE_TABLE_CLASS)?1:
			create_table_test(console_out,db,"ClassAttendance", CREATE_TABLE_CLASS_ATTENDANCE)?1:
			create_table_test(console_out,db,"Subject", CREATE_TABLE_SUBJECT)?1:
			create_table_test(console_out,db,"ClassSubject", CREATE_TABLE_CLASS_SUBJECT)?1:
			create_table_test(console_out,db,"Teacher", CREATE_TABLE_TEACHER)?1:
			create_table_test(console_out,db,"Teaches", CREATE_TABLE_TEACHES)?1:
			create_table_test(console_out,db,"TeacherSubordination", CREATE_TABLE_TEACHER_SUBORDINATION)?1:
			create_table_test(console_out,db,"TeacherAttendance", CREATE_TABLE_TEACHER_ATTENDANCE)?1:
			create_table_test(console_out,db,"Demand", CREATE_TABLE_DEMAND);
			create_table_test(console_out,db,"TeachesPeriodPreference", CREATE_TABLE_TEACHES_PERIOD_PREFERENCE)?1:
			create_table_test(console_out,db,"TeachesTwinPreference", CREATE_TABLE_TEACHES_TWIN_PREFERENCE);

	return iserr;
}
