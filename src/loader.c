/*
 * Project Minerva.
 *   loader.c - SQL data loader utility module.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains functions for saving and loading data
 * from SQL databases related to school timetabling.
 */
#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "loader.h"
#include "assert.h"

/* TODO: Assure PRAGMA foreign_keys = on */
/* TODO insert DisciplineGroups. */
const char * const CREATE_TABLE_SCHOOL =
			("CREATE TABLE IF NOT EXISTS School("
				"id						integer primary key autoincrement not null,"
				"name					text,"
				"num_per_per_day		integer,"
				"num_days				integer,"
				"max_per_per_day		integer,"
				"max_per_per_week		integer,"
				"max_doubling			integer"
			")");
const char * const INSERT_TABLE_SCHOOL =
	("INSERT INTO School(name, num_per_per_day, num_days, max_per_per_day, max_per_per_week, max_doubling) values(?,?,?,?,?,?)");
const char * const LASTID_TABLE_SCHOOL =
			("SELECT id FROM School where rowid = last_insert_rowid()");
const char * const SELECT_SCHOOL_NAMES =
		("SELECT id,name FROM School");
const char * const SELECT_SCHOOL_BY_ID =
		("SELECT * FROM School where id=?");

const char * const CREATE_TABLE_DAILY_PERIOD =
			("CREATE TABLE IF NOT EXISTS DailyPeriod("
				"id 					integer primary key,"
				"name					text,"
				"per_index				integer,"
				"school_id				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_DAILY_PERIOD =
			("INSERT INTO DailyPeriod values(?,?,?,?)");
const char * const LASTID_TABLE_DAILY_PERIOD =
			("SELECT id FROM DailyPeriod where rowid = last_insert_rowid()");
const char * const SELECT_DAILY_PERIOD_BY_SCHOOL_ID =
		("SELECT * FROM DailyPeriod WHERE school_id=?");

const char * const CREATE_TABLE_DAY =
			("CREATE TABLE IF NOT EXISTS Day("
				"id 					integer primary key,"
				"name					text,"
				"day_index				integer,"
				"school_id				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_DAY =
			("INSERT INTO Day values(?,?,?,?)");
const char * const LASTID_TABLE_DAY =
			("SELECT id FROM Day where rowid = last_insert_rowid()");
const char * const SELECT_DAY_BY_SCHOOL_ID =
		("SELECT * FROM Day WHERE school_id = ?");

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
const char * const SELECT_PERIOD_BY_SCHOOL_ID =
		("SELECT * FROM Period WHERE school_id = ?");

const char * const CREATE_TABLE_FEATURE =
			("CREATE TABLE IF NOT EXISTS Feature("
				"id 					integer primary key,"
				"name 					text,"
				"school_id 				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_FEATURE =
			("INSERT INTO Feature VALUES(?,?,?)");
const char * const LASTID_TABLE_FEATURE =
			("SELECT id FROM Feature where rowid = last_insert_rowid()");
const char * const SELECT_FEATURE_BY_SCHOOL_ID =
		("SELECT * FROM Feature WHERE school_id = ?");

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
const char * const SELECT_ROOM_BY_SCHOOL_ID =
			("SELECT * FROM Room WHERE school_id = ?");

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
const char * const SELECT_ROOM_FEATURE_BY_ROOM_ID =
			("SELECT * FROM RoomFeature WHERE id_room=?");

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
const char * const SELECT_ROOM_AVALIBILITY_BY_ROOM_ID =
			("SELECT * FROM RoomAvalibility WHERE room_id=?");

const char * const CREATE_TABLE_CLASS =
			("CREATE TABLE IF NOT EXISTS Class("
				"id 					integer primary key autoincrement not null,"
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
			("INSERT INTO Class(name, short_name, size, abstract, free_periods_flag, per_max_entrance, per_min_exit, school_id) VALUES (?,?,?,?,?,?,?,?)");
const char * const LASTID_TABLE_CLASS =
			("SELECT id FROM Class where rowid = last_insert_rowid()");
const char * const SELECT_CLASS_BY_SCHOOL_ID =
			("SELECT * FROM Class WHERE school_id = ?");

const char * const CREATE_TABLE_CLASS_ATTENDANCE =
			("CREATE TABLE IF NOT EXISTS ClassAttendance("
				"id 					integer primary key autoincrement not null,"
				"id_class				integer,"
				"id_period				integer,"
				"id_att_type			integer,"
				"score      			integer,"
				"FOREIGN KEY (id_class) REFERENCES Class(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id)"
				"FOREIGN KEY (id_att_type) REFERENCES AttendanceType(id)"
			")");
const char * const INSERT_TABLE_CLASS_ATTENDANCE =
			("INSERT INTO ClassAttendance(id_class, id_period, id_att_type, score) values(?,?,?,?)");
const char * const LASTID_TABLE_CLASS_ATTENDANCE =
			("SELECT id FROM ClassAttendance where rowid = last_insert_rowid()");
const char * const SELECT_CLASS_ATTENDANCE_BY_CLASS_ID =
			("SELECT * FROM ClassAttendance WHERE id_class = ?");

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
const char * const SELECT_CLASS_SUBORDINATION_BY_SUP_ID =
			("SELECT * FROM ClassSubordination WHERE id_sup=?");

const char * const CREATE_TABLE_SUBJECT =
			("CREATE TABLE IF NOT EXISTS Subject("
				"id						integer primary key,"
				"name					text,"
				"short_name				text"
				"school_id				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_SUBJECT =
			("INSERT INTO Subject VALUES (?,?,?)");
const char * const LASTID_TABLE_SUBJECT =
			("SELECT id FROM Subject where rowid = last_insert_rowid()");
const char * const SELECT_SUBJECT_BY_SCHOOL_ID =
			("SELECT * FROM Subject WHERE school_id = ?");

const char * const CREATE_TABLE_CLASS_SUBJECT =
			("CREATE TABLE IF NOT EXISTS ClassSubject("
				"id 					integer primary key,"
				"max_per_per_day		integer,"
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
const char * const SELECT_CLASS_SUBJECT_BY_CLASS_ID =
			("SELECT * FROM ClassSubject WHERE class_id = ?");

const char * const CREATE_TABLE_TEACHER =
			("CREATE TABLE IF NOT EXISTS Teacher("
				"id 					integer primary key,"
				"name					text,"
				"short_name				text,"
				"max_days				integer,"
				"max_per_day			integer,"
				"max_per_class_per_day	integer,"
				"max_per				integer,"
				"num_per_planning		integer,"
				"school_id				integer,"
				"FOREIGN KEY (school_id) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_TEACHER =
			("INSERT INTO Teacher VALUES (?,?,?,?,?,?,?)");
const char * const LASTID_TABLE_TEACHER =
			("SELECT id FROM Teacher where rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_BY_SCHOOL_ID =
			("SELECT * FROM Teacher WHERE school_id = ?");

const char * const CREATE_TABLE_TEACHER_DAY =
			("CREATE TABLE IF NOT EXISTS TeacherDay("
				"id						integer primary key,"
				"id_teacher				integer,"
				"id_day					integer,"
				"max_periods			integer,"
				"score					integer,"
				"FOREIGN KEY (id_teacher) references Teacher(id),"
				"FOREIGN KEY (id_day) references Day(id)"
			")");
const char * const INSERT_TABLE_TEACHER_DAY =
			("INSERT INTO TABLE TeacherDay VALUES (?,?,?,?,?)");
const char * const LASTID_TABLE_TEACHER_DAY =
			("SELECT id from TeacherDay where rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_DAY_BY_TEACHER_ID =
			("SELECT * FROM TeacherDay WHERE id_teacher = ?");

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
const char * const LASTID_TABLE_TEACHES =
			("SELECT id FROM Teaches where rowid = last_insert_rowid()");
const char * const SELECT_TEACHES_BY_TEACHER_ID =
			("SELECT * FROM Teaches WHERE id_teacher=?");

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
const char * const LASTID_TEACHER_SUBORDINATION =
			("SELECT id FROM TeacherSubordination where rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_SUBORDINATION_BY_SUP_ID =
			("SELECT * FROM TeacherSubordination WHERE id_sup=?");

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
const char * const LASTID_TEACHER_ATTENDANCE =
			("SELECT id FROM TeacherAttendance where rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_ATTENDANCE_BY_TEAHCER_ID =
			("SELECT * FROM TeacherAttendance WHERE id_teacher=?");

const char * const CREATE_TABLE_ATTENDANCE_TYPE =
			("CREATE TABLE IF NOT EXISTS AttendanceType("
				"id						integer primary key,"
				"name					integer"
			")");
const char * const INSERT_TABLE_ATTENDANCE_TYPE =
			("INSERT INTO AttendanceType VALUES(?,?)");
const char * const LASTID_ATTENDANCE_TYPE =
			("SELECT id FROM AttendanceType where rowid = last_insert_rowid()");
const char * const SELECT_ATTENDANCE_TYPE =
			("SELECT * FROM AttendanceType");

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
const char * const LASTID_DEMAND =
			("SELECT id FROM Demand where rowid = last_insert_rowid()");
const char * const SELECT_DEMAND_BY_TEACHES_ID =
			("SELECT * FROM Demand WHERE teaches_id=?");

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
const char * const LASTID_TEACHES_PERIOD_PREFERENCE =
			("SELECT id FROM TeachesPeriodPreference where rowid = last_insert_rowid()");
const char * const SELECT_TEACHES_PERIOD_PREFERENCE_BY_TEACHES_ID =
			("SELECT * FROM TeachesPeriodPreference WHERE id_teaches=?");

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
const char * const LASTID_TEACHES_TWIN_PREFERENCE =
			("SELECT id FROM TeachesTwinPreference where rowid = last_insert_rowid()");
const char * const SELECT_TEACHES_TWIN_PREFERENCE_BY_TEACHES_ID =
			("SELECT * FROM TeachesTwinPreference WHERE id_teaches=?");

const char * const CREATE_TABLE_MEETING =
			("CREATE TABLE IF NOT EXISTS Meeting("
				"id						integer primary key,"
				"n_solution				integer,"
				"id_period				integer,"
				"id_class				integer,"
				"id_teacher				integer,"
				"id_subject				integer,"
				"id_room				integer,"
				"id_school				integer,"
				"FOREIGN KEY (id_period) 	REFERENCES Period(id),"
				"FOREIGN KEY (id_class) 	REFERENCES Class(id),"
				"FOREIGN KEY (id_teacher) 	REFERENCES Teacher(id),"
				"FOREIGN KEY (id_subject)	REFERENCES Subject(id),"
				"FOREIGN KEY (id_room) 		REFERENCES Room(id),"
				"FOREIGN KEY (id_school)	REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_MEETING =
			("INSERT INTO Meeting(n_solution, id_period, id_class, id_teacher, id_subject, id_room, id_school) VALUES (?,?,?,?,?,?,?)");
const char * const LASTID_TABLE_MEETING =
			("SELECT id FROM Meeting where rowid = last_insert_rowid()");
const char * const SLEECT_MEETING_BY_SCHOOL_ID =
			("SELECT * FROM Meeting WHERE id_school=?");

/**
 * Creates a table based on one of the strings above.
 *
 * @param console_out: Buffer where warnings and messages are printed;
 * @param db: The database in which the data is created;
 * @param tablename: Table name. Printed to console_out;
 *
 * return: SQLite error code in case of failure.
 **/
int create_table(FILE * console_out, sqlite3* db,const char * const tablename, const char * const sql){
	char * errmsg;
	int errc;
	if(console_out){
		fprintf(console_out,"Creating, if not existent, table %s.\n", tablename);
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

// NOTE negative-terminated list
static bool insert_attendance(FILE * console_out, sqlite3* db, const char* const table_insert, int * period_ids, int * period_scores, int obj_id, School * school){
	int i = 0;
	int errc = 0;
	sqlite3_stmt * stmt;

	LMH_ASSERT(db != NULL);
	LMH_ASSERT(table_insert != NULL);
	LMH_ASSERT(period_ids != NULL);
	LMH_ASSERT(period_scores != NULL);
	LMH_ASSERT(school != NULL);

	sqlite3_prepare(db, table_insert, -1, &stmt, NULL);
	for(i = 0; period_scores[i] >= 0 && errc == 0; ++i){
		sqlite3_bind_int(stmt,1,obj_id);
		sqlite3_bind_int(stmt,2, period_ids[i]);
		sqlite3_bind_int(stmt,3, 1);
		sqlite3_bind_int(stmt,4, period_scores[i]);
		errc = sqlite3_step(stmt);

		if(errc != SQLITE_DONE){
			if(console_out){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			}
			return false;
		} else {
			errc = 0;
		}
		sqlite3_reset(stmt);

	}
	return true;
}

/* TODO test. */
int insert_teacher(FILE * console_out, sqlite3 * db, Teacher * teacher, School * school){
	sqlite3_stmt * stmt;
	int id = -1;
	int errc;

	LMH_ASSERT(db != NULL && teacher != NULL && school != NULL);


	sqlite3_prepare(db, INSERT_TABLE_TEACHER, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 2, teacher->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, teacher->short_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 4, teacher->max_days);
	sqlite3_bind_int(stmt, 5, teacher->max_meetings_per_day);
	sqlite3_bind_int(stmt, 6, teacher->max_meetings_per_class_per_day);
	sqlite3_bind_int(stmt, 7, teacher->max_meetings);
	sqlite3_bind_int(stmt, 8, teacher->num_planning_periods);
	sqlite3_bind_int(stmt, 9, school->id);

	errc = sqlite3_step(stmt);
	errc |= sqlite3_exec(db, LASTID_TABLE_TEACHER, get_id_callback, &id, NULL);

	if(errc != SQLITE_DONE){
		if(console_out){
			fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
		}
		return -1;
	}
	sqlite3_finalize(stmt);

	insert_attendance(console_out, db, INSERT_TABLE_TEACHER_ATTENDANCE, school->period_ids, teacher->periods, teacher->id, school);

	return id;
}

/* TODO test. */
int insert_class(FILE * console_out, sqlite3 * db, Class * class, School * school){
	sqlite3_stmt * stmt;
	int id = -1;
	int errc;

	LMH_ASSERT(db != NULL && class != NULL && school != NULL);

	sqlite3_prepare(db, INSERT_TABLE_CLASS, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 2, class->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, class->short_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 4, class->size);
	sqlite3_bind_int(stmt, 5, class->abstract);
	sqlite3_bind_int(stmt, 6, class->can_have_free_periods_flag);
	sqlite3_bind_int(stmt, 7, class->maximal_entry_period);
	sqlite3_bind_int(stmt, 8, class->minimal_exit_period);
	sqlite3_bind_int(stmt, 9, school->id);

	errc = sqlite3_step(stmt);
	errc |= sqlite3_exec(db, LASTID_TABLE_CLASS, get_id_callback, &id, NULL);

	if(errc != SQLITE_DONE){
		if(console_out){
			fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
		}
		return -1;
	}
	sqlite3_finalize(stmt);

	insert_attendance(console_out, db, INSERT_TABLE_CLASS_ATTENDANCE, school->period_ids, class->periods, class->id, school);

	return id;
}

/* TODO test. */
int insert_room(FILE * console_out, sqlite3 * db, Room * room, School * school){
	sqlite3_stmt * stmt;
	int id = -1;
	int errc;

	LMH_ASSERT(db != NULL && room != NULL && school != NULL);

	sqlite3_prepare(db, INSERT_TABLE_ROOM, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 2, room->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, room->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 4, school->id);

	errc = sqlite3_step(stmt);
	errc |= sqlite3_exec(db, LASTID_TABLE_ROOM, get_id_callback, &id, NULL);

	if(errc != SQLITE_DONE){
		if(console_out){
			fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
		}
		return -1;
	}
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, INSERT_TABLE_ROOM_AVALIBILITY, -1, &stmt, NULL);
	for(int i = 0; i < school->n_periods; ++i){
		sqlite3_bind_int(stmt, 2, room->id);
		sqlite3_bind_int(stmt, 3, school->period_ids[i]);
		sqlite3_bind_int(stmt, 4, room->disponibility[i]);

		sqlite3_step(stmt);
		sqlite3_reset(stmt);

	}
	return id;
}

/* TODO test. */
int insert_feature(FILE * console_out, sqlite3 * db, const char * const feature, School * school){
	sqlite3_stmt * stmt;
	int id = -1;
	int errc;

	LMH_ASSERT(db != NULL && feature != NULL && school != NULL);

	sqlite3_prepare(db, INSERT_TABLE_FEATURE, -1, &stmt, NULL);
	sqlite3_bind_text(stmt,2, feature, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,3, school->id);

	errc = sqlite3_step(stmt);
	errc |= sqlite3_exec(db, LASTID_TABLE_FEATURE, get_id_callback, &id, NULL);

	if(errc != SQLITE_DONE){
		if(console_out){
			fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
		}
		return -1;
	}

	sqlite3_finalize(stmt);
	return id;
}

/* TODO test. */
bool insert_all_meetings(FILE * console_out, sqlite3 * db, School * school){
	int i, errc = 0;
	sqlite3_stmt * stmt;
	printf("Inserting all meetings.\n");

	sqlite3_prepare(db, INSERT_TABLE_MEETING, -1, &stmt, NULL);
	for(i = 0; school->all_meetings[i].m_class != NULL; ++i){
		sqlite3_bind_int(stmt, 1, -1);
		sqlite3_bind_int(stmt, 2, school->period_ids[ school->all_meetings[i].period ]);
		sqlite3_bind_int(stmt, 3, school->all_meetings[i].m_class->id);
		sqlite3_bind_int(stmt, 4, school->all_meetings[i].teacher->id);
		sqlite3_bind_int(stmt, 5, school->all_meetings[i].subj->id);
		sqlite3_bind_int(stmt, 6, school->all_meetings[i].room->id);
		sqlite3_bind_int(stmt, 7, school->id);

		errc = sqlite3_step(stmt);
		errc |= sqlite3_exec(db, LASTID_TABLE_MEETING, get_id_callback, &(school->feature_ids[i]), NULL);

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

/* TODO TODO not implemented. */
bool insert_all_teaches(FILE * console_out, sqlite3 * db, School * school){
	printf("Inserting all teaches. NOT IMPLEMENTED\n");
	return false;
}

/* TODO TODO not implemented. */
bool insert_all_teachers(FILE * console_out, sqlite3 * db, School * school){
	printf("Insert all teachers. NOT IMPLEMENTED\n");
	return false;
}

/* TODO test. */
bool insert_all_classes(FILE * console_out, sqlite3 * db, School * school){
	int i, errc = 0;
	sqlite3_stmt * stmt;
	printf("Inserting all classes.\n");

	sqlite3_prepare(db, INSERT_TABLE_CLASS, -1, &stmt, NULL);
	for(i = 0; i < school->n_classes; i++){
		sqlite3_bind_text(stmt, 1, school->classes[i].name, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, school->classes[i].short_name, -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 3, school->classes[i].size);
		sqlite3_bind_int(stmt, 4, school->classes[i].abstract);
		sqlite3_bind_int(stmt, 5, school->classes[i].can_have_free_periods_flag);
		sqlite3_bind_int(stmt, 6, school->classes[i].maximal_entry_period);
		sqlite3_bind_int(stmt, 7, school->classes[i].minimal_exit_period);
		sqlite3_bind_int(stmt, 8, school->id);

		errc = sqlite3_step(stmt);
		errc |= sqlite3_exec(db, LASTID_TABLE_CLASS, get_id_callback, &(school->classes[i].id), NULL);

		if(errc != SQLITE_DONE){
			if(console_out){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			}
			return false;
		}

		insert_attendance(console_out, db, INSERT_TABLE_CLASS_ATTENDANCE, school->period_ids, school->classes[i].periods, school->classes[i].id, school);
		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
	return true;
}

/* TODO test. */
bool insert_all_features(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i;
	int errc;

	printf("Inserting all features.\n");

	school->feature_ids = calloc(school->n_features, sizeof((*school->feature_ids)));
	sqlite3_prepare(db, INSERT_TABLE_FEATURE, -1, &stmt, NULL);
	for(i = 0; i < school->n_features; i++){
		sqlite3_bind_text(stmt,2, school->feature_names[i], -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt,3, school->id);

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

/* TODO test. */
bool insert_all_periods(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i;
	int errc;

	printf("Inserting all periods.");

	school->period_ids = calloc(school->n_periods, sizeof((*school->period_ids)));
	sqlite3_prepare(db, INSERT_TABLE_PERIOD, -1, &stmt, NULL);
	for(i = 0; i < school->n_periods; i++){
		if(school->period_names != NULL){
			sqlite3_bind_text(stmt,2, school->period_names[i], -1, SQLITE_TRANSIENT);
		} else {
			sqlite3_bind_null(stmt,2);
		}
		sqlite3_bind_int(stmt,3, i % school->n_periods_per_day);
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

/* TODO more or less tested. */
bool insert_all_daily_periods(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i;
	int errc;

	fprintf(console_out, "Inserting all daily periods.\n");
	school->daily_period_ids = calloc(school->n_periods_per_day, sizeof((*school->daily_period_ids)));
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

/* TODO more or less tested. */
bool insert_all_days(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i;
	int errc;

	printf("Inserting all days.\n");

	school->day_ids = calloc(school->n_days, sizeof((*school->day_ids)));
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

/* TODO more or less tested. */
int insert_school(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt = NULL;
	int errc = 0;

	LMH_ASSERT(db != NULL && school != NULL && school->name != NULL);

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

		sqlite3_bind_text(stmt,1, school->name, -1,SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt,2, school->n_periods_per_day);
		sqlite3_bind_int(stmt,3, school->n_days);
		sqlite3_bind_int(stmt,4, school->max_meetings_teacher_per_day);
		sqlite3_bind_int(stmt,5, school->max_meetings_teacher_per_week);
		sqlite3_bind_int(stmt,6, school->max_gemini_classes);

		// TODO: ATOMIC START
		errc = sqlite3_step(stmt);
		sqlite3_exec(db, LASTID_TABLE_SCHOOL, get_id_callback, &(school->id), NULL);
		// TODO: ATOMIC END.

		sqlite3_finalize(stmt);

		if(errc != SQLITE_DONE){
			if(console_out){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			}
		} else {
			if(console_out){
				fprintf(console_out, "Inserted school with id (%d)\n", school->id);
			}
		}
		if(school->day_names != NULL){
			insert_all_days(console_out, db, school);
		}
		if(school->daily_period_names != NULL){
			insert_all_daily_periods(console_out, db, school);
		}
		if(school->day_names != NULL && school->daily_period_names != NULL){
			insert_all_periods(console_out, db, school);
		}
		if(school->feature_names != NULL){
			insert_all_features(console_out, db, school);
		}
		if(school->classes != NULL){
			insert_all_classes(console_out, db, school);
		}
		if(school->teachers != NULL){
			insert_all_teachers(console_out, db, school);
		}
		if(school->teaches != NULL){
			insert_all_teaches(console_out, db, school);
		}
		if(school->all_meetings != NULL){
			insert_all_meetings(console_out, db, school);
		}
	}
	return school->id;
}

/**
* Tries to recreate every table. Ignores if table already exists.
*/
sqlite3* init_all_tables(FILE * console_out, char * db_filename){

	sqlite3 * db;
	if(console_out){
		fprintf(console_out,"Opening database.\n");
	}
	int errc = sqlite3_open(db_filename, &db);
	if(errc != SQLITE_OK){
		printf("Errc %d %s\n", errc, sqlite3_errmsg(db));
		return NULL;
	}
	bool iserr;
	iserr = create_table(console_out,db,"School", CREATE_TABLE_SCHOOL)?1:
	create_table(console_out,db,"DailyPeriod", CREATE_TABLE_DAILY_PERIOD)?1:
	create_table(console_out,db,"Day", CREATE_TABLE_DAY)?1:
	create_table(console_out,db,"Period", CREATE_TABLE_PERIOD)?1:
	create_table(console_out,db,"AttendanceType", CREATE_TABLE_ATTENDANCE_TYPE)?1:
	create_table(console_out,db,"RoomAvalibility", CREATE_TABLE_ROOM_AVALIBILITY)?1:
	create_table(console_out,db,"Feature", CREATE_TABLE_FEATURE)?1:
	create_table(console_out,db,"Room", CREATE_TABLE_ROOM)?1:
	create_table(console_out,db,"Class", CREATE_TABLE_CLASS)?1:
	create_table(console_out,db,"ClassAttendance", CREATE_TABLE_CLASS_ATTENDANCE)?1:
	create_table(console_out,db,"Subject", CREATE_TABLE_SUBJECT)?1:
	create_table(console_out,db,"ClassSubject", CREATE_TABLE_CLASS_SUBJECT)?1:
	create_table(console_out,db,"Teacher", CREATE_TABLE_TEACHER)?1:
	create_table(console_out,db,"TeacherDay",CREATE_TABLE_TEACHER_DAY)?1:
	create_table(console_out,db,"Teaches", CREATE_TABLE_TEACHES)?1:
	create_table(console_out,db,"TeacherSubordination", CREATE_TABLE_TEACHER_SUBORDINATION)?1:
	create_table(console_out,db,"TeacherAttendance", CREATE_TABLE_TEACHER_ATTENDANCE)?1:
	create_table(console_out,db,"Demand", CREATE_TABLE_DEMAND);
	create_table(console_out,db,"TeachesPeriodPreference", CREATE_TABLE_TEACHES_PERIOD_PREFERENCE)?1:
	create_table(console_out,db,"TeachesTwinPreference", CREATE_TABLE_TEACHES_TWIN_PREFERENCE);
	create_table(console_out,db,"Meeting", CREATE_TABLE_MEETING);
	if(iserr){
		sqlite3_close(db);
		return NULL;
	} else {
		return db;
	}
}

/*
 * Returns a null-terminated list of strings, all freeable.
 * In case of error, returns null
 */
char** select_all_school_names(FILE * console_out, sqlite3* db, int ** ids){
	int i = 0, errc, str_sz, alloc_sz;
	sqlite3_stmt * stmt;
	const char * aux;
	char ** strings = NULL;

	errc = sqlite3_prepare_v2(db, SELECT_SCHOOL_NAMES, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		errc = sqlite3_step(stmt);
		alloc_sz = 10;
		strings = calloc(10, sizeof(char*));
		if(ids != NULL){
			*ids = calloc(10, sizeof(char *));
		}
		while( errc == SQLITE_ROW ){
			aux = sqlite3_column_text(stmt, 1);
			str_sz = sqlite3_column_bytes(stmt,1);
			if(ids != NULL){
				(*ids)[i] = sqlite3_column_int(stmt,0);
			}
			strings[i] = calloc( str_sz + 1, sizeof(char) );
			strncpy(strings[i], aux, str_sz);

			i++;
			if(i == alloc_sz){
				alloc_sz += 10;
				strings = realloc(strings, alloc_sz);
				if(*ids != NULL){
					*ids = realloc(*ids, alloc_sz);
				}
			}
			errc = sqlite3_step(stmt);
		}
	} else {
		fprintf(console_out, "Failed to select school names.\n");
	}
	sqlite3_finalize(stmt);
	return strings;
}

/* TODO TODO implement. */
Room * select_all_rooms_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_rooms){
	*n_rooms = 0;
	return NULL;
}

/* TODO TODO implement. */
Teacher * select_all_teachers_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_teachers){
	*n_teachers = 0;
	return NULL;
}

/* TODO TODO implement. */
Class * select_all_classes_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_classes){
	*n_classes = 0;
	return NULL;
}

/* TODO TODO implement. */
Teaches * select_all_teaches_by_school_id(FILE * console_out, sqlite3 * db, int id, int * n_teaches){
	*n_teaches = 0;
	return NULL;
}

/* TODO more or less tested. */
School * select_school_by_id(FILE * console_out, sqlite3* db, int id){
	int errc;
	sqlite3_stmt * stmt;
	School * school = NULL;
	const unsigned char * aux;

	fprintf(console_out,"Selecting school by id.");

	errc = sqlite3_prepare_v2(db, SELECT_SCHOOL_BY_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out,"Failed to prepare statement.");
		return NULL;
	}
	sqlite3_bind_int(stmt,1,id);
	errc = sqlite3_step(stmt);

	if( errc == SQLITE_ROW ){
		school = calloc(1, sizeof(School));
		aux = sqlite3_column_text(stmt, 1);
		sqlite3_column_bytes(stmt,1);

		school->id = id;
		school->name = calloc(1 + sqlite3_column_bytes(stmt,1), sizeof(char));
		strncpy(school->name, aux, 1 + sqlite3_column_bytes(stmt,1));
		school->n_periods_per_day = sqlite3_column_int(stmt,2);
		school->n_days = sqlite3_column_int(stmt,3);
		school->max_meetings_teacher_per_day = sqlite3_column_int(stmt,4);
		school->max_meetings_teacher_per_week = sqlite3_column_int(stmt,5);
	}
	sqlite3_finalize(stmt);

	school->rooms = select_all_rooms_by_school_id(stdout, db, id, &(school->n_rooms));
	school->classes = select_all_classes_by_school_id(stdout, db, id, &(school->n_classes));
	school->teachers = select_all_teachers_by_school_id(stdout, db, id, &(school->n_teachers));
	school->teaches = select_all_teaches_by_school_id(stdout, db, id, &(school->n_teaches));

	return school;
}
