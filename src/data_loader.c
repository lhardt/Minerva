
#include <sqlite3.h>
#include <stdio.h>
#include "data_loader.h"

/* Do not forget PRAGMA foreign_keys = on */

const char * const CREATE_TABLE_SCHOOL =
			("CREATE TABLE IF NOT EXISTS School("
				"id						integer primary key,"
				"name					text,"
				"num_per_per_day		integer,"
				"max_per_per_day		integer,"
				"max_per_per_week		integer,"
				"max_doubling			integer"
			")");

const char * const CREATE_TABLE_DAILY_PERIOD =
			("CREATE TABLE IF NOT EXISTS DailyPeriod("
				"id 					integer primary key,"
				"name					text,"
				"day_index				integer,"
				"school_id				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");

const char * const CREATE_TABLE_DAY =
			("CREATE TABLE IF NOT EXISTS Day("
				"id 					integer primary key,"
				"name					text,"
				"school_index			integer,"
				"school_id				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");

const char * const CREATE_TABLE_PERIOD =
			("CREATE TABLE IF NOT EXISTS Period("
				"id						integer primary key,"
				"name					text,"
				"day_index				integer,"
				"day_id					integer,"
				"school_id				integer,"
				"FOREIGN KEY (day_id) REFERENCES Day(id),"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");

const char * const CREATE_TABLE_ROOM_AVALIBILITY =
			("CREATE TABLE IF NOT EXISTS RoomAvalibility("
				"id						integer primary key,"
				"room_id				integer,"
				"period_id				integer,"
				"FOREIGN KEY (room_id) REFERENCES Room(id),"
				"FOREIGN KEY (period_id) REFERENCES Period(id)"
			")");

const char * const CREATE_TABLE_FEATURE =
			("CREATE TABLE IF NOT EXISTS Feature("
				"id 					integer primary key,"
				"name 					text"
			")");

const char * const CREATE_TABLE_ROOM =
			("CREATE TABLE IF NOT EXISTS Room("
				"id 					integer primary key,"
				"name 					text,"
				"short_name 			text,"
				"school_id 				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");

const char * const CREATE_TABLE_ROOM_FEATURE =
			("CREATE TABLE IF NOT EXISTS RoomFeature("
				"id						integer primary key,"
				"id_room				integer,"
				"id_feature				integer,"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"FOREIGN KEY (id_feature) REFERENCES Feature(id)"
			")");

const char * const CREATE_TABLE_CLASS =
			("CREATE TABLE IF NOT EXISTS Class("
				"id 					integer primary key,"
				"name 					text,"
				"short_name 			text,"
				"size 					integer,"
				"free_periods_flag 		integer,"
				"per_max_entrance 		integer,"
				"per_min_exit 			integer"
			")");

const char * const CREATE_TABLE_CLASS_ATTENDANCE =
			("CREATE TABLE IF NOT EXISTS ClassAttendance("
				"id 					integer primary key,"
				"id_class				integer,"
				"id_period				integer,"
				"id_att_type			integer,"
				"FOREIGN KEY (id_class) REFERENCES Class(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id)"
				"FOREIGN KEY (id_att_type) REFERENCES AttendanceType(id)"
			")");

const char * const CREATE_TABLE_CLASS_SUBORDINATION =
			("CREATE TABLE IF NOT EXISTS ClassSubordination("
				"id						integer primary key,"
				"id_sub					integer,"
				"id_sup					integer,"
				"FOREIGN KEY (id_sub) REFERENCES Class(id),"
				"FOREIGN KEY (id_sup) REFERENCES Class(id)"
			")");

const char * const CREATE_TABLE_SUBJECT =
			("CREATE TABLE IF NOT EXISTS Subject("
				"id						integer primary key,"
				"name					text,"
				"short_name				text"
			")");

const char * const CREATE_TABLE_CLASS_SUBJECT =
			("CREATE TABLE IF NOT EXISTS ClassSubject("
				"id 					integer primary key,"
				"class_id				integer,"
				"subject_id				integer,"
				"amount					integer,"
				"FOREIGN KEY (class_id) REFERENCES Class(id),"
				"FOREIGN KEY (subject_id) REFERENCES Subject(id)"
			")");

const char * const CREATE_TABLE_TEACHER =
			("CREATE TABLE IF NOT EXISTS Teacher("
				"id 					integer primary key,"
				"name					text,"
				"short_name				text,"
				"max_per_per_day		integer,"
				"max_per_per_week		integer,"
				"num_per_planning		integer,"
				"school_id				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");

const char * const CREATE_TABLE_TEACHES =
			("CREATE TABLE IF NOT EXISTS Teaches("
				"id 					integer primary key,"
				"id_teacher				integer,"
				"id_subject				integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_subject) REFERENCES Subject(id)"
			")");

const char * const CREATE_TABLE_TEACHER_SUBORDINATION =
			("CREATE TABLE IF NOT EXISTS TeacherSubordination("
				"id						integer primary key,"
				"id_sub					integer,"
				"id_sup					integer,"
				"FOREIGN KEY (id_sub) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_sup) REFERENCES Teacher(id)"
			")");

const char * const CREATE_TABLE_TEACHER_ATTENDANCE =
			("CREATE TABLE IF NOT EXISTS TeacherAttendance("
				"id						integer primary key,"
				"id_teacher				integer,"
				"id_period				integer,"
				"id_period				integer,"
				"id_att_type			integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id)"
				"FOREIGN KEY (id_att_type) REFERENCES AttendanceType(id)"
			")");

const char * const CREATE_TABLE_ATTENDANCE_TYPE =
			("CREATE TABLE IF NOT EXISTS AttendanceType("
				"id						integer primary key,"
				"name					integer"
			")");

const char * const CREATE_TABLE_DEMAND =
			("CREATE TABLE IF NOT EXISTS Demand("
				"id						integer primary key,"
				"teaches_id				integer,"
				"feature_id				integer,"
				"FOREIGN KEY (teaches_id) REFERENCES Teaches(id),"
				"FOREIGN KEY (feature_id) REFERENCES Feature(id)"
			")");

const char * const CREATE_TABLE_TEACHER_ATTENDANCE_PREFERENCE =
			("CREATE TABLE IF NOT EXISTS TeacherAttendancePreference("
			 	"id 					integer primary key,"
				"id_teacher				integer,"
				"id_period				integer,"
				"value					integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_period)  REFERENCES Period(id)"
			")");

const char * const CREATE_TABLE_TEACHES_PERIOD_PREFERENCE =
			("CREATE TABLE IF NOT EXISTS TeachesPeriodPreference("
				"id 					integer primary key,"
				"id_teaches				integer,"
				"id_period				integer,"
				"value					integer,"
				"FOREIGN KEY (id_teaches) REFERENCES Teaches(id),"
				"FOREIGN KEY (id_period)  REFERENCES Period(id)"
			")");

const char * const CREATE_TABLE_TEACHES_TWIN_PREFERENCE =
			("CREATE TABLE IF NOT EXISTS TeachesTwinPreference("
				"id 					integer primary key,"
				"id_teaches				integer,"
				"twin_val				integer,"
				"value					integer,"
				"FOREIGN KEY (id_teaches) REFERENCES Teaches(id)"
			")");

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
			create_table_test(console_out,db,"TeachesTwinPreference", CREATE_TABLE_TEACHES_TWIN_PREFERENCE)?1:
			create_table_test(console_out,db,"TeacherAttendancePreference", CREATE_TABLE_TEACHER_ATTENDANCE_PREFERENCE);

	return iserr;
}
