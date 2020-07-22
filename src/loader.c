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
#include "loader.h"

#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "assert.h"

/* TODO: Assure PRAGMA foreign_keys = on */
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
const char * const DELETE_SCHOOL_BY_ID =
		("DELETE FROM School WHERE id=?");

const char * const CREATE_TABLE_DAILY_PERIOD =
			("CREATE TABLE IF NOT EXISTS DailyPeriod("
				"id 					integer primary key,"
				"name					text,"
				"per_index				integer,"
				"id_school				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_DAILY_PERIOD =
			("INSERT INTO DailyPeriod values(?,?,?,?)");
const char * const LASTID_TABLE_DAILY_PERIOD =
			("SELECT id FROM DailyPeriod where rowid = last_insert_rowid()");
const char * const SELECT_DAILY_PERIOD_BY_SCHOOL_ID =
			("SELECT * FROM DailyPeriod WHERE id_school=? ORDER BY per_index");
const char * const DELETE_DAILY_PERIOD_BY_SCHOOL_ID =
			("DELETE FROM DailyPeriod WHERE id_school=?");

const char * const CREATE_TABLE_DAY =
			("CREATE TABLE IF NOT EXISTS Day("
				"id 					integer primary key,"
				"name					text,"
				"day_index				integer,"
				"id_school				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_DAY =
			("INSERT INTO Day values(?,?,?,?)");
const char * const LASTID_TABLE_DAY =
			("SELECT id FROM Day where rowid = last_insert_rowid()");
const char * const SELECT_DAY_BY_SCHOOL_ID =
		("SELECT * FROM Day WHERE id_school = ? ORDER BY day_index");
const char * const DELETE_DAY_BY_SCHOOL_ID =
		("DELETE FROM Day WHERE id_school = ?");

const char * const CREATE_TABLE_PERIOD =
			("CREATE TABLE IF NOT EXISTS Period("
				"id 					integer primary key,"
				"name					text,"
				"school_operates_flag   integer,"
				"day_id					integer,"
				"daily_period_id		integer,"
				"id_school				integer,"
				"FOREIGN KEY (day_id) REFERENCES Day(id),"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_PERIOD =
			("INSERT INTO Period values(?,?,?,?,?,?)");
const char * const LASTID_TABLE_PERIOD =
			("SELECT id FROM Period where rowid = last_insert_rowid()");
const char * const SELECT_PERIOD_BY_SCHOOL_ID =
		("SELECT * FROM Period WHERE id_school = ?");
const char * const DELETE_PERIOD_BY_SCHOOL_ID =
		("DELETE FROM Period WHERE id_school = ?");

const char * const CREATE_TABLE_FEATURE =
			("CREATE TABLE IF NOT EXISTS Feature("
				"id 					integer primary key,"
				"name 					text,"
				"id_school 				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_FEATURE =
			("INSERT INTO Feature VALUES(?,?,?)");
const char * const LASTID_TABLE_FEATURE =
			("SELECT id FROM Feature where rowid = last_insert_rowid()");
const char * const SELECT_FEATURE_BY_SCHOOL_ID =
		("SELECT * FROM Feature WHERE id_school = ?");
const char * const DELETE_FEATURE_BY_ID =
		("DELETE FROM Feature WHERE id=?");
const char * const DELETE_FEATURE_BY_SCHOOL_ID =
		("DELETE FROM Feature WHERE id_school=?");

const char * const CREATE_TABLE_ROOM =
			("CREATE TABLE IF NOT EXISTS Room("
				"id 					integer primary key autoincrement,"
				"name 					text,"
				"short_name 			text,"
				"size		 			integer,"
				"id_school 				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_ROOM =
			("INSERT INTO Room(name, short_name, size, id_school) VALUES(?,?,?,?)");
const char * const LASTID_TABLE_ROOM =
			("SELECT id FROM Room where rowid = last_insert_rowid()");
const char * const SELECT_ROOM_BY_SCHOOL_ID =
			("SELECT * FROM Room WHERE id_school = ?");
const char * const DELETE_ROOM_BY_ID =
			("DELETE FROM Room WHERE id=?");
const char * const DELETE_ROOM_BY_SCHOOL_ID =
			("DELETE FROM Room WHERE id_school=?");

const char * const CREATE_TABLE_ROOM_FEATURE =
			("CREATE TABLE IF NOT EXISTS RoomFeature("
				"id						integer primary key autoincrement not null,"
				"id_room				integer,"
				"id_feature				integer,"
				"score					integer,"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"FOREIGN KEY (id_feature) REFERENCES Feature(id)"
			")");
const char * const INSERT_TABLE_ROOM_FEATURE =
			("INSERT INTO RoomFeature(id_room, id_feature, score) VALUES (?,?,?)");
const char * const LASTID_TABLE_ROOM_FEATURE =
			("SELECT id FROM RoomFeature where rowid = last_insert_rowid()");
const char * const SELECT_ROOM_FEATURE_BY_ROOM_ID =
			("SELECT * FROM RoomFeature WHERE id_room=?");
const char * const DELETE_ROOM_FEATURE_BY_FEATURE_ID =
			("DELETE FROM RoomFeature WHERE id_feature=?");
const char * const DELETE_ROOM_FEATURE_BY_ROOM_ID =
			("DELETE FROM RoomFeature WHERE id_room=?");
const char * const DELETE_ROOM_FEATURE_BY_ID =
			("DELETE FROM RoomFeature WHERE id=?");
const char * const DELETE_ROOM_FEATURE_BY_SCHOOL_ID =
			("DELETE FROM RoomFeature WHERE EXISTS("
				"SELECT id FROM Room WHERE Room.id = RoomFeature.id_room "
					"AND Room.id_school=?"
			")");

const char * const CREATE_TABLE_ROOM_AVALIBILITY =
			("CREATE TABLE IF NOT EXISTS RoomAvalibility("
				"id						integer primary key autoincrement not null,"
				"id_room				integer,"
				"period_id				integer,"
				"score					integer,"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"FOREIGN KEY (period_id) REFERENCES Period(id)"
			")");
const char * const INSERT_TABLE_ROOM_AVALIBILITY =
			("INSERT INTO RoomAvalibility(id_room, period_id, score) values(?,?,?)");
const char * const LASTID_TABLE_ROOM_AVALIBILITY =
			("SELECT id FROM RoomAvalibility where rowid = last_insert_rowid()");
const char * const SELECT_ROOM_AVAILIBILITY_BY_ROOM_ID =
			("SELECT * FROM RoomAvalibility WHERE id_room=?");
const char * const DELETE_ROOM_AVAILIBILITY_BY_ROOM_ID =
			("DELETE FROM RoomAvalibility WHERE id_room=?");
const char * const DELETE_ROOM_AVAILIBILITY_BY_SCHOOL_ID =
			("DELETE FROM RoomAvailibility WHERE EXISTS("
				"SELECT id FROM Room WHERE Room.id = RoomAvailibility.id_room "
					"AND Room.id_school=?"
			")");

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
				"id_school  			integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_CLASS =
			("INSERT INTO Class(name, short_name, size, abstract, free_periods_flag, per_max_entrance, per_min_exit, id_school) VALUES (?,?,?,?,?,?,?,?)");
const char * const LASTID_TABLE_CLASS =
			("SELECT id FROM Class where rowid = last_insert_rowid()");
const char * const SELECT_CLASS_BY_SCHOOL_ID =
			("SELECT * FROM Class WHERE id_school=?");
const char * const DELETE_CLASS_BY_ID =
		("DELETE FROM Class WHERE id=?");
const char * const DELETE_CLASS_BY_SCHOOL_ID =
		("DELETE FROM Class WHERE id_school=?");

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
const char * const DELETE_CLASS_ATTENDANCE_BY_CLASS_ID =
			("DELETE FROM ClassAttendance WHERE id_class=?");
const char * const DELETE_CLASS_ATTENDANCE_BY_SCHOOL_ID =
			("DELETE FROM ClassAttendance WHERE EXISTS ("
				"SELECT id FROM Class WHERE Class.id=ClassAttendance.id_class "
					"AND Class.id_school = ?"
			")");

const char * const CREATE_TABLE_CLASS_SUBORDINATION =
			("CREATE TABLE IF NOT EXISTS ClassSubordination("
				"id						integer primary key autoincrement not null,"
				"id_sub					integer,"
				"id_sup					integer,"
				"FOREIGN KEY (id_sub) REFERENCES Class(id),"
				"FOREIGN KEY (id_sup) REFERENCES Class(id)"
			")");
const char * const INSERT_TABLE_CLASS_SUBORDINATION =
			("INSERT INTO ClassSubordination(id_sub, id_sup) VALUES(?,?)");
const char * const LASTID_TABLE_CLASS_SUBORDINATION =
			("SELECT id FROM ClassSubordination where rowid = last_insert_rowid()");
const char * const SELECT_CLASS_SUBORDINATION_BY_SUP_ID =
			("SELECT * FROM ClassSubordination WHERE id_sup=?");
const char * const DELETE_CLASS_SUBORDINATION_BY_SUP_ID =
			("DELETE FROM ClassSubordination WHERE id_sup = ?");
const char * const DELETE_CLASS_SUBORDINATION_BY_SUB_ID =
			("DELETE FROM ClassSubordination WHERE id_sub = ?");
const char * const DELETE_CLASS_Subordination_BY_SCHOOL_ID =
			/* Deleting by supid is enough, and equivalent to by subid */
			("DELETE FROM ClassSubordination WHERE EXISTS ("
				"SELECT id FROM Class WHERE Class.id=ClassSubordination.id_sup "
					"AND Class.id_school = ?"
			")");

const char * const CREATE_TABLE_CLASS_ROOM =
			("CREATE TABLE ClassRoom("
				"id						integer primary key autoincrement not null,"
				"score					integer,"
				"id_class				integer,"
				"id_room				integer,"
				"FOREIGN KEY (id_class) REFERENCES Class(id),"
				"FOREIGN KEY (id_room) REFERENCES Room(id)"
			")");
const char * const INSERT_TABLE_CLASS_ROOM =
			("INSERT INTO ClassRoom(score, id_class, id_room) VALUES (?,?,?)");
const char * const LASTID_TABLE_CLASS_ROOM =
			("SELECT id FROM ClassRoom WHERE rowid = last_insert_rowid()");
const char * const SELECT_CLASS_ROOM_BY_CLASS_ID =
			("SELECT * FROM ClassRoom WHERE id_class = ?");
const char * const DELETE_CLASS_ROOM_BY_CLASS_ID =
			("DELETE FROM ClassRoom WHERE id_class = ?");
const char * const DELETE_CLASS_ROOM_BY_ROOM_ID =
			("DELETE FROM ClassRoom WHERE id_room = ?");
const char * const DELETE_CLASS_ROOM_BY_SCHOOL_ID =
			("DELETE FROM ClassRoom WHERE EXISTS ("
				"SELECT id FROM Class WHERE Class.id = ClassRoom.id_class "
					"AND Class.id_school=?"
			")");

const char * const CREATE_TABLE_SUBJECT =
			("CREATE TABLE IF NOT EXISTS Subject("
				"id						integer primary key autoincrement not null,"
				"name					text,"
				"short_name				text,"
				"id_school				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_SUBJECT =
			("INSERT INTO Subject(name, short_name, id_school) VALUES (?,?,?)");
const char * const LASTID_TABLE_SUBJECT =
			("SELECT id FROM Subject where rowid = last_insert_rowid()");
const char * const SELECT_SUBJECT_BY_SCHOOL_ID =
			("SELECT * FROM Subject WHERE id_school = ?");
const char * const DELETE_SUBJECT_BY_ID =
			("DELETE FROM Subject WHERE id=?");
const char * const DELETE_SUBJECT_BY_SCHOOL_ID =
			("DELETE FROM Subject WHERE id_school=?");

const char * const CREATE_TABLE_SUBJECT_GROUP =
			("CREATE TABLE IF NOT EXISTS SubjectGroup("
				"id 					integer primary key autoincrement not null,"
				"name					text,"
				"id_school				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_SUBJECT_GROUP =
 			("INSERT INTO SubjectGroup(name,id_school) VALUES (?,?)");
const char * const LASTID_TABLE_SUBJECT_GROUP =
			("SELECT id FROM SubjectGroup WHERE rowid = last_insert_rowid()");
const char * const SELECT_TABLE_SUBJECT_GROUP_BY_SCHOOL_ID =
			("SELECT * FROM SubjectGroup WHERE id_school = ?");
const char * const DELETE_SUBJECT_GROUP_BY_SCHOOL_ID =
			("DELETE FROM SubjectGroup WHERE id_school=?");
const char * const DELETE_SUBJECT_GROUP_BY_ID =
			("DELETE FROM SubjectGroup WHERE id=?");

const char * const CREATE_TABLE_SUBJECT_IN_GROUP =
 			("CREATE TABLE IF NOT EXISTS SubjectInGroup("
				"id						integer primary key autoincrement not null,"
				"id_subject				integer,"
				"id_group				integer,"
				"FOREIGN KEY (id_subject) REFERENCES Subject(id),"
				"FOREIGN KEY (id_group)   REFERENCES SubjectGroup(id)"
			")");
const char * const INSERT_TABLE_SUBJECT_IN_GROUP =
			("INSERT INTO SubjectInGroup(id_subject, id_group) VALUES (?,?)");
const char * const LASTID_TABLE_SUBJECT_IN_GROUP =
			("SELECT id FROM SubjectInGroup where rowid=last_insert_rowid()");
const char * const SELECT_TABLE_SUBJECT_IN_GROUP_BY_SUBJECT_ID =
			("SELECT * FROM SubjectInGroup WHERE id_subject=?");
const char * const DELETE_SUBJECT_IN_GROUP_BY_SUBJECT_ID =
			("DELETE FROM SubjectInGroup WHERE id_subject = ?");
const char * const DELETE_SUBJECT_IN_GROUP_BY_GROUP_ID =
			("DELETE FROM SubjectInGroup WHERE id_group = ?");
const char * const DELETE_FROM_SUBJECT_IN_GROUP_BY_SCHOOL_ID =
			("DELETE FROM SubjectInGroup WHERE EXISTS("
				"SELECT Subject WHERE SubjectInGroup.id_subject = Subject.id "
					"AND Subject.id_school=?"
			")");

const char * const CREATE_TABLE_CLASS_SUBJECT =
			("CREATE TABLE IF NOT EXISTS ClassSubject("
				"id 					integer primary key autoincrement not null,"
				"class_id				integer,"
				"subject_id				integer,"
				"amount					integer,"
				"max_per_per_day		integer,"
				"FOREIGN KEY (class_id) REFERENCES Class(id),"
				"FOREIGN KEY (subject_id) REFERENCES Subject(id)"
			")");
const char * const INSERT_TABLE_CLASS_SUBJECT =
			("INSERT INTO ClassSubject(class_id, subject_id, amount, max_per_per_day) VALUES (?,?,?,?)");
const char * const LASTID_CLASS_SUBJECT =
			("SELECT id FROM ClassSubject where rowid = last_insert_rowid()");
const char * const SELECT_CLASS_SUBJECT_BY_CLASS_ID =
			("SELECT * FROM ClassSubject WHERE class_id = ?");
const char * const DELETE_CLASS_SUBJECT_BY_CLASS_ID =
			("DELETE FROM ClassSubject WHERE class_id = ?");
const char * const DELETE_CLASS_SUBJECT_BY_SUBJECT_ID =
			("DELETE FROM ClassSubject WHERE subject_id = ?");
const char * const DELETE_CLASS_SUBJECT_BY_SCHOOL_ID =
			("DELETE FROM ClassSubject WHERE EXISTS ("
				"SELECT id FROM Class WHERE Class.id=ClassSubject.id_class "
					"AND Class.id_school=?"
			")");

const char * const CREATE_TABLE_CLASS_SUBJECT_GROUP =
			("CREATE TABLE IF NOT EXISTS ClassSubjectGroup("
				"id						integer primary key autoincrement not null,"
				"id_class				integer,"
				"id_group				integer,"
				"max_per_day			integer,"
				"FOREIGN KEY (id_class) REFERENCES Class(id),"
				"FOREIGN KEY (id_group) REFERENCES SubjectGroup(id)"
			")");
const char * const INSERT_TABLE_CLASS_SUBJECT_GROUP =
			("INSERT INTO ClassSubjectGroup(id_class, id_group, max_per_day) VALUES (?,?,?)");
const char * const LASTID_CLASS_SUBJECT_GROUP =
			("SELECT id FROM ClassSubjectGroup WHERE rowid = last_insert_rowid()");
const char * const SELECT_CLASS_SUBJECT_GROUP_BY_CLASS_ID =
			("SELECT * FROM ClassSubjectGroup WHERE id_class=?");
const char * const SELECT_CLASS_SUBJECT_GROUP_BY_GROUP_ID =
			("SELECT * FROM ClassSubjectGroup WHERE id_group=?");
const char * const DELETE_CLASS_SUBJECT_GROUP_BY_CLASS_ID =
			("DELETE FROM ClassSubjectGroup WHERE id_class=?");
const char * const DELETE_CLASS_SUBJECT_GROUP_BY_GROUP_ID =
			("DELETE FROM ClassSubjectGroup WHERE id_group=?");
const char * const DELETE_CLASS_SUBJECT_GROUP_BY_SCHOOL_ID =
			("DELETE FROM ClassSubjectGroup WHERE EXISTS ("
				"SELECT Class WHERE Class.id = ClassSubjectGroup.id_class "
					"AND Class.id_school = ?"
			")");

const char * const CREATE_TABLE_TEACHER =
			("CREATE TABLE IF NOT EXISTS Teacher("
				"id 					integer primary key autoincrement not null,"
				"name					text,"
				"short_name				text,"
				"max_days				integer,"
				"max_per_day			integer,"
				"max_per_class_per_day	integer,"
				"max_per				integer,"
				"num_per_planning		integer,"
				"id_school				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_TEACHER =
			("INSERT INTO Teacher VALUES (?,?,?,?,?,?,?,?,?)");
const char * const LASTID_TABLE_TEACHER =
			("SELECT id FROM Teacher where rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_BY_SCHOOL_ID =
			("SELECT * FROM Teacher WHERE id_school = ?");
const char * const DELETE_TEACHER_BY_ID =
			("DELETE FROM Teacher WHERE id=?");
const char * const DELETE_TEACHER_BY_SCHOOL_ID =
			("DELETE FROM Teacher WHERE id_school=?");

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
const char * const DELETE_TEACHER_DAY_BY_TEACHER_ID =
			("DELETE FROM TeacherDay WHERE id_teacher = ?");
const char * const DELETE_TEACHER_DAY_BY_SCHOOL_ID =
			("DELETE FROM TeacherDay WHERE EXISTS ("
				"SELECT id Teacher WHERE Teacher.id=TeacherDay.id_teacher "
					"AND Teacher.id_school=?"
			")");

const char * const CREATE_TABLE_TEACHES =
			("CREATE TABLE IF NOT EXISTS Teaches("
				"id 					integer primary key autoincrement not null,"
				"id_teacher				integer,"
				"id_subject				integer,"
				"score  				integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_subject) REFERENCES Subject(id)"
			")");
const char * const INSERT_TABLE_TEACHES =
			("INSERT INTO Teaches(id, id_teacher, id_subject, score) VALUES (?,?,?,?)");
const char * const LASTID_TABLE_TEACHES =
			("SELECT id FROM Teaches where rowid = last_insert_rowid()");
const char * const SELECT_TEACHES_BY_SCHOOL_ID =
			("SELECT * FROM Teaches INNER JOIN"
			 	" Teacher ON Teacher.id = Teaches.id_teacher"
				" WHERE Teacher.id_school=?"
			);
const char * const DELETE_TEACHES_BY_SUBJECT_ID =
			("DELETE FROM Teaches WHERE id_subject = ?");
const char * const DELETE_TEACHES_BY_TEACHER_ID =
			("DELETE FROM Teaches WHERE id_teacher = ?");
const char * const DELETE_TEACHES_BY_SCHOOL_ID =
			("DELETE FROM Teaches WHERE EXISTS ("
				"SELECT id FROM Teacher WHERE Teaches.id_teacher = Teacher.id "
					"AND Teacher.id_school=?"
			")");

const char * const CREATE_TABLE_TEACHER_SUBORDINATION =
			("CREATE TABLE IF NOT EXISTS TeacherSubordination("
				"id						integer primary key autoincrement not null,"
				"id_sub					integer,"
				"id_sup					integer,"
				"FOREIGN KEY (id_sub) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_sup) REFERENCES Teacher(id)"
			")");
const char * const INSERT_TABLE_TEACHER_SUBORDINATION =
			("INSERT INTO TeacherSubordination(id_sub, id_sup) VALUES (?,?)");
const char * const LASTID_TEACHER_SUBORDINATION =
			("SELECT id FROM TeacherSubordination where rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_SUBORDINATION_BY_SUP_ID =
			("SELECT * FROM TeacherSubordination WHERE id_sup=?");
const char * const DELETE_TEACHER_SUBORDINATION_BY_SUP_ID =
			("DELETE FROM TeacherSubordination WHERE id_sup = ?");
const char * const DELETE_TEACHER_SUBORDINATION_BY_SUB_ID =
			("DELETE FROM TeacherSubordination WHERE id_sub = ?");
const char * const DELETE_TEACHER_SUBORDINATION_BY_SCHOOL_ID =
			/* Selecting id_sup is sufficient and equivalent to selecting id_sub*/
			("DELETE FROM TeacherSubordination WHERE EXISTS("
				"SELECT Teacher WHERE Teacher.id = TeacherSubordination.id_sup "
					"AND Teacher.id_school=? "
			")");

const char * const CREATE_TABLE_TEACHER_ATTENDANCE =
			("CREATE TABLE IF NOT EXISTS TeacherAttendance("
				"id						integer primary key autoincrement not null,"
				"id_teacher				integer,"
				"id_period				integer,"
				"id_att_type			integer,"
				"score					integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id)"
				"FOREIGN KEY (id_att_type) REFERENCES AttendanceType(id)"
			")");
const char * const INSERT_TABLE_TEACHER_ATTENDANCE =
			("INSERT INTO TeacherAttendance(id_teacher, id_period, id_att_type, score) VALUES (?,?,?,?)");
const char * const LASTID_TEACHER_ATTENDANCE =
			("SELECT id FROM TeacherAttendance where rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_ATTENDANCE_BY_TEACHER_ID =
			("SELECT * FROM TeacherAttendance WHERE id_teacher=?");
const char * const DELETE_TEACHER_ATTENDANCE_BY_TEACHER_ID =
			("DELETE FROM TeacherAttendance WHERE id_teacher = ?");
const char * const DELETE_TEACHER_ATTENDANCE_BY_SCHOOL_ID =
			("DELETE FROM TeacherAtteandance WHERE EXISTS ("
				"SELECT Teacher WHERE Teacher.id = TeacherAttendance.id_teacher "
					"AND Teacher.id_school=?"
			")");

const char * const CREATE_TABLE_TEACHER_ROOM =
			("CREATE TABLE IF NOT EXISTS TeacherRoom("
				"id						integer primary key autoincrement not null,"
				"score					integer,"
				"id_teacher				integer,"
				"id_room				integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_room) REFERENCES Room(id)"
			")");
const char * const INSERT_TABLE_TEACHER_ROOM =
			("INSERT INTO TeacherRoom(score, id_teacher, id_room) VALUES (?,?,?)");
const char * const LASTID_TABLE_TEACHER_ROOM =
			("SELECT id FROM TeacherRoom WHERE rowid = last_insert_rowid()");
const char * const SELECT_TABLE_TEACHER_ROOM_BY_TEACHER_ID =
			("SELECT * from TeacherRoom WHERE id_teacher = ?");
const char * const DELETE_TEACHER_ROOM_BY_TEACHER_ID =
			("DELETE FROM TeacherRoom WHERE id_teacher=?");

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
				"id_teaches				integer,"
				"id_feature				integer,"
				"min_score				integer,"
				"score  				integer,"
				"FOREIGN KEY (id_teaches) REFERENCES Teaches(id),"
				"FOREIGN KEY (id_feature) REFERENCES Feature(id)"
			")");
const char * const INSERT_TABLE_DEMAND =
			("INSERT INTO Demand VALUES (?,?,?)");
const char * const LASTID_DEMAND =
			("SELECT id FROM Demand where rowid = last_insert_rowid()");
const char * const SELECT_DEMAND_BY_TEACHES_ID =
			("SELECT * FROM Demand WHERE id_teaches=?");
const char * const DELETE_DEMAND_BY_TEACHES_ID =
 			("DELETE FROM Demand WHERE id_teaches=?");
const char * const DELETE_DEMAND_BY_FEATURE_ID =
 			("DELETE FROM Demand WHERE id_feature=?");
const char * const DELETE_DEMAND_BY_ID =
 			("DELETE FROM Demand WHERE id=?");
const char * const DELETE_DEMAND_BY_SUBJECT_ID =
			("DELETE FROM Demand WHERE EXISTS ("
				"SELECT id from Teaches WHERE Teaches.id = Demand.id_teaches "
					"AND Teaches.id_subject = ?"
			")");
const char * const DELETE_DEMAND_BY_TEACHER_ID =
			("DELETE FROM Demand WHERE EXISTS ("
				"SELECT id FROM Teaches WHERE Teaches.id = Demand.id_teaches "
					"AND Teaches.id_teacher = ?"
			")");
const char * const DELETE_DEMAND_BY_SCHOOL_ID =
			("DELETE FROM Demand WHERE EXISTS ("
				"SELECT id FROM Feature WHERE Feature.id = Demand.id_feature "
					"AND Feature.id_school=?"
			")");

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
const char * const DELETE_TEACHES_PERIOD_PREFERENCE_BY_SUBJECT_ID =
			("DELETE FROM TeachesPeriodPreference WHERE EXISTS ("
				"SELECT id from Teaches WHERE Teaches.id = TeachesPeriodPreference.id_teaches "
					"AND Teaches.id_subject = ?"
			")");
const char * const DELETE_TEACHES_PERIOD_PREFERENCE_BY_TEACHER_ID =
			("DELETE FROM TeachesPeriodPreference WHERE EXISTS ("
				"SELECT id FROM Teaches WHERE Teaches.id = TeachesPeriodPreference.id_teaches "
					"AND Teaches.id_teacher = ?"
			")");
const char * const DELETE_TEACHES_PERIOD_PREFERENCE_BY_SCHOOL_ID =
			("DELETE FROM TeachesPeriodPreference WHERE EXISTS ("
				"SELECT id FROM Period WHERE Period.id = TeachesPeriodPreference.id_period "
					"AND Period.id_school = ?"
			")");

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
const char * const DELETE_TEACHES_TWIN_PREFERENCE_BY_SUBJECT_ID =
			("DELETE FROM TeachesTwinPreference WHERE EXISTS ("
				"SELECT id from Teaches WHERE Teaches.id = TeachesTwinPreference.id_teaches "
					"AND Teaches.id_subject = ?"
			")");
const char * const DELETE_TEACHES_TWIN_PREFERENCE_BY_TEACHER_ID =
			("DELETE FROM TeachesTwinPreference WHERE EXISTS ("
				"SELECT id FROM Teaches WHERE Teaches.id = TeachesTwinPreference.id_teaches "
					"AND Teaches.id_teacher = ?"
			")");
const char * const DELETE_TEACHES_TWIN_PREFERENCE_BY_SCHOOL_ID =
			("DELETE FROM TeachesTwinPreference WHERE EXISTS ("
				"SELECT id FROM Teaches WHERE Teaches.id = TeachesTwinPreference.id_teaches "
					"AND EXISTS("
						"SELECT id FROM Teacher WHERE Teaches.id_teacher = Teacher.id "
							"AND Teacher.id_school = ?"
					")"
			")");

const char * const CREATE_TABLE_SOLUTION =
			("CREATE TABLE IF NOT EXISTS Solution("
				"id						integer primary key,"
				"gen_date				text," /* Format to be defined */
				"name					text,"
				"desc					text,"
				"id_school				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_SOLUTION =
			("INSERT INTO Solution VALUES (?,?,?,?,?)");
const char * const LASTID_TABLE_SOLUTION =
			("SELECT id FROM Solution WHERE rowid = last_insert_rowid()");
const char * const SELECT_SOLUTION_BY_SCHOOL_ID =
			("SELECT * FROM SOLUTION WHERE id_school=?");
const char * const DELETE_SOLUTION_BY_SCHOOL_ID =
			("DELETE FROM SOLUTION WHERE id_school=?");

const char * const CREATE_TABLE_MEETING =
			("CREATE TABLE IF NOT EXISTS Meeting("
				"id						integer primary key,"
				"id_class				integer,"
				"id_subject				integer,"
				"id_school				integer,"
				"FOREIGN KEY (id_class) 	REFERENCES Class(id),"
				"FOREIGN KEY (id_subject)	REFERENCES Subject(id),"
				"FOREIGN KEY (id_school)	REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_MEETING =
			("INSERT INTO Meeting(id_class, id_subject, id_school) VALUES (?,?,?)");
const char * const LASTID_TABLE_MEETING =
			("SELECT id FROM Meeting where rowid = last_insert_rowid()");
const char * const SELECT_MEETING_BY_SCHOOL_ID =
			("SELECT * FROM Meeting WHERE id_school=?");
const char * const DELETE_MEETING_BY_SUBJECT_ID =
			("DELETE FROM Meeting WHERE id_subject = ?");
const char * const DELETE_MEETING_BY_CLASS_ID =
			("DELETE FROM Meeting WHERE id_class = ?");
const char * const DELETE_MEETING_BY_SCHOOL_ID =
			("DELETE FROM Meeting WHERE id_school = ?");

const char * const CREATE_TABLE_POSSIBLE_TEACHER =
			("CREATE TABLE IF NOT EXISTS PossibleTeacher("
				"id integer primary key autoincrement not null,"
				"score integer,"
				"id_meeting integer,"
				"id_teacher integer,"
				"FOREIGN KEY (id_meeting) REFERENCES Meeting(id),"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id)"
			")");
const char * const INSERT_TABLE_POSSIBLE_TEACHER =
			("INSERT INTO PossibleTeacher(score, id_meeting, id_teacher) VALUES (?,?,?)");
const char * const LASTID_TABLE_POSSIBLE_TEACHER =
			("SELECT id FROM PossibleTeacher WHERE rowid=last_insert_rowid()");
const char * const SELECT_TABLE_POSSIBLE_TEACHER_BY_MEETING_ID =
			("SELECT * FROM PossibleTeacher WHERE id_meeting=?");
const char * const DELETE_POSSIBLE_TEACHER_BY_MEETING_ID =
			("DELETE FROM PossibleTeacher WHERE id_meeting=?");
const char * const DELETE_POSSIBLE_TEACHER_BY_SCHOOL_ID =
			("DELETE FROM PossibleTeacher WHERE EXISTS("
				"SELECT id FROM Meeting WHERE Meeting.id = PossibleTeacher.id_meeting "
					"AND Meeting.id_school=?"
			")");

const char * const CREATE_TABLE_POSSIBLE_ROOM =
			("CREATE TABLE IF NOT EXISTS PossibleRoom("
				"id integer primary key autoincrement not null,"
				"score integer,"
				"id_meeting integer,"
				"id_room integer,"
				"FOREIGN KEY (id_meeting) REFERENCES Meeting(id),"
				"FOREIGN KEY (id_room) REFERENCES Room(id)"
			")");
const char * const INSERT_TABLE_POSSIBLE_ROOM =
			("INSERT INTO PossibleRoom(score, id_meeting, id_room) VALUES (?,?,?)");
const char * const LASTID_TABLE_POSSIBLE_ROOM =
			("SELECT id FROM PossibleRoom WHERE rowid = last_insert_rowid()");
const char * const SELECT_TABLE_POSSIBLE_ROOM_BY_MEETING_ID =
			("SELECT * FROM PossibleRoom WHERE id_meeting = ?");
const char * const DELETE_POSSIBLE_ROOM_BY_MEETING_ID =
			("DELETE FROM PossibleRoom WHERE id_meeting = ?");
const char * const DELETE_POSSIBLE_ROOM_BY_SCHOOL_ID =
			("DELETE FROM PossibleRoom WHERE EXISTS("
				"SELECT id FROM Meeting WHERE Meeting.id = PossibleRoom.id_meeting "
					"AND Meeting.id_school=?"
			")");

const char * const CREATE_TABLE_POSSIBLE_PERIOD =
			("CREATE TABLE IF NOT EXISTS PossiblePeriod("
				"id integer primary key autoincrement not null,"
				"score integer,"
				"id_meeting integer,"
				"id_period integer,"
				"FOREIGN KEY (id_meeting) REFERENCES Meeting(id),"
				"FOREIGN KEY (id_period)  REFERENCES Period(id)"
			")");
const char * const INSERT_TABLE_POSSIBLE_PERIOD =
			("INSERT INTO PossiblePeriod(score, id_meeting, id_period) VALUES (?,?,?)");
const char * const LASTID_TABLE_POSSIBLE_PERIOD =
			("SELECT id FROM PossiblePeriod WHERE rowid = last_insert_rowid()");
const char * const SELECT_TABLE_POSSIBLE_PERIOD_BY_MEETING_ID =
			("SELECT * FROM PossiblePeriod WHERE id_meeting = ?");
const char * const DELETE_POSSIBLE_PERIOD_BY_MEETING_ID =
			("DELETE FROM PossiblePeriod WHERE id_meeting = ?");
const char * const DELETE_POSSIBLE_PERIOD_BY_SCHOOL_ID =
			("DELETE FROM PossiblePeriod WHERE EXISTS("
				"SELECT id FROM Meeting WHERE Meeting.id = PossiblePeriod.id_meeting "
					"AND Meeting.id_school=?"
			")");

const char * const CREATE_TABLE_MEETING_SOLUTION =
			("CREATE TABLE IF NOT EXISTS MeetingSolution("
				"id						integer primary key,"
				"id_meeting				integer,"
				"id_solution			integer,"
				"id_period				integer,"
				"id_teacher				integer,"
				"id_room				integer,"
				"FOREIGN KEY (id_solution) 	REFERENCES Solution(id),"
				"FOREIGN KEY (id_period) 	REFERENCES Period(id),"
				"FOREIGN KEY (id_teacher) 	REFERENCES Teacher(id),"
				"FOREIGN KEY (id_room) 		REFERENCES Room(id)"
			")");
const char * const INSERT_TABLE_MEETING_SOLUTION =
			("INSERT INTO MeetingSolution(id_meeting, id_solution, id_period, id_teacher, id_room) VALUES (?,?,?,?,?)");
const char * const LASTID_TABLE_MEETING_SOLUTION =
			("SELECT id FROM MeetingSolution where rowid = last_insert_rowid()");
const char * const SELECT_MEETING_SOLUTION_BY_SOLUTION_ID =
			("SELECT * FROM MeetingSolution WHERE id_solution=?");
const char * const UNSET_MEETING_SOLUTION_ROOM_BY_ROOM_ID =
			("UPDATE MeetingSolution SET id_room=null where id_room=?");
const char * const UNSET_MEETING_TEACHER_BY_TEACHER_ID =
			("UPDATE MeetingSolution SET id_teacher=null WHERE id_teacher = ?");
const char * const DELETE_MEETING_SOLUTION_BY_SOLUTION_ID =
			("DELETE FROM MeetingSolution WHERE id_solution = ?");
const char * const DELETE_MEETING_SOLUTION_BY_SCHOOL_ID =
			("DELETE FROM MeetingSolution WHERE EXISTS("
				"SELECT id_school FROM Meeting WHERE Meeting.id = MeetingSolution.id "
					"AND Meeting.id_school=?"
			")");

/**
 * Creates a table based on one of the strings above.
 *
 * @param console_out: Buffer where warnings and messages are printed;
 * @param db: The database in which the data is created;
 * @param tablename: Table name. Printed to console_out;
 *
 * return: SQLite error code in case of failure.
 **/
static int create_table(FILE * console_out, sqlite3* db,const char * const tablename, const char * const sql){
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
* Tries to recreate every table. Ignores if table already exists.
*/
bool init_all_tables(FILE * console_out, sqlite3 * db){
	return create_table(console_out,db,"School", CREATE_TABLE_SCHOOL)?1:
			create_table(console_out,db,"DailyPeriod", CREATE_TABLE_DAILY_PERIOD)?1:
			create_table(console_out,db,"Day", CREATE_TABLE_DAY)?1:
			create_table(console_out,db,"Period", CREATE_TABLE_PERIOD)?1:
			create_table(console_out,db,"AttendanceType", CREATE_TABLE_ATTENDANCE_TYPE)?1:
			create_table(console_out,db,"RoomAvalibility", CREATE_TABLE_ROOM_AVALIBILITY)?1:
			create_table(console_out,db,"Feature", CREATE_TABLE_FEATURE)?1:
			create_table(console_out,db,"Room", CREATE_TABLE_ROOM)?1:
			create_table(console_out,db,"RoomFeature", CREATE_TABLE_ROOM_FEATURE)?1:
			create_table(console_out,db,"Class", CREATE_TABLE_CLASS)?1:
			create_table(console_out,db,"ClassAttendance", CREATE_TABLE_CLASS_ATTENDANCE)?1:
			create_table(console_out,db,"ClassSubordination", CREATE_TABLE_CLASS_SUBORDINATION)?1:
			create_table(console_out,db,"ClassRoom", CREATE_TABLE_CLASS_ROOM)?1:
			create_table(console_out,db,"Subject", CREATE_TABLE_SUBJECT)?1:
			create_table(console_out,db,"SubjectGroup", CREATE_TABLE_SUBJECT_GROUP)?1:
			create_table(console_out,db,"SubjectInGroup", CREATE_TABLE_SUBJECT_IN_GROUP)?1:
			create_table(console_out,db,"ClassSubject", CREATE_TABLE_CLASS_SUBJECT)?1:
			create_table(console_out,db,"ClassSubjectGroup", CREATE_TABLE_CLASS_SUBJECT_GROUP)?1:
			create_table(console_out,db,"Teacher", CREATE_TABLE_TEACHER)?1:
			create_table(console_out,db,"TeacherDay",CREATE_TABLE_TEACHER_DAY)?1:
			create_table(console_out,db,"Teaches", CREATE_TABLE_TEACHES)?1:
			create_table(console_out,db,"TeacherSubordination", CREATE_TABLE_TEACHER_SUBORDINATION)?1:
			create_table(console_out,db,"TeacherAttendance", CREATE_TABLE_TEACHER_ATTENDANCE)?1:
			create_table(console_out,db,"TeacherRoom", CREATE_TABLE_TEACHER_ROOM)?1:
			create_table(console_out,db,"Demand", CREATE_TABLE_DEMAND)?1:
			create_table(console_out,db,"TeachesPeriodPreference", CREATE_TABLE_TEACHES_PERIOD_PREFERENCE)?1:
			create_table(console_out,db,"TeachesTwinPreference", CREATE_TABLE_TEACHES_TWIN_PREFERENCE)?1:
			create_table(console_out,db,"Solution", CREATE_TABLE_SOLUTION)?1:
			create_table(console_out,db,"Meeting", CREATE_TABLE_MEETING)?1:
			create_table(console_out,db,"PossibleRoom", CREATE_TABLE_POSSIBLE_ROOM)?1:
			create_table(console_out,db,"PossiblePeriod", CREATE_TABLE_POSSIBLE_PERIOD)?1:
			create_table(console_out,db,"PossibleTeacher", CREATE_TABLE_POSSIBLE_TEACHER)?1:
			create_table(console_out,db,"MeetingSolution", CREATE_TABLE_MEETING_SOLUTION)?1:0;
}

/**
 * SQLite Callback to get the id of a newly inserted row.
 *
 * Parameters related to SQL.
 * @param id_field_ptr: the pointer at which the id data will be written
 */
static int get_id_callback(void* id_field_ptr,int no_columns,char** text_columns,char** name_columns){
	if(no_columns == 1 && id_field_ptr != NULL){
		*((int *)id_field_ptr) = strtol(text_columns[0], NULL,10);
		return 0;
	}
	printf("CALLBACK FUNCTION MISUSE!");
	return -1;

}

static bool insert_room_relation(FILE * console_out, sqlite3* db, const char * const table_insert, int obj_id, int * room_scores, School * school){
	int i = 0, errc;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, table_insert, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		printf("Could not insert room relation");
		return false;
	}
	while(errc == SQLITE_OK && i < school->n_rooms && room_scores[i] >= 0){
		sqlite3_bind_int(stmt,1, room_scores[i]);
		sqlite3_bind_int(stmt,2, obj_id);
		sqlite3_bind_int(stmt,3, school->rooms[i].id);

		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		++i;
	}
	return true;
}

static bool insert_attendance(FILE * console_out, sqlite3* db, const char* const table_insert, int * period_ids, int * period_scores, int obj_id, School * school){
	int i = 0;
	int errc = 0;
	sqlite3_stmt * stmt;

	LMH_ASSERT(school != NULL && period_scores != NULL && period_ids != NULL && table_insert != NULL && db != NULL);

	errc = sqlite3_prepare(db, table_insert, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		errc = SQLITE_DONE;
		for(i = 0; (i < school->n_periods) && (period_scores[i] >= 0) && (errc == SQLITE_DONE); ++i){
			sqlite3_bind_int(stmt,1, obj_id);
			sqlite3_bind_int(stmt,2, period_ids[i]);
			sqlite3_bind_int(stmt,3, 1);
			sqlite3_bind_int(stmt,4, period_scores[i]);
			errc = sqlite3_step(stmt);
			sqlite3_reset(stmt);
		}
	}
	if(errc != SQLITE_DONE){
		printf("Error inserting attendance.%d %s\n", errc, sqlite3_errmsg(db));
	}
	return true;
}

int insert_class_sub(FILE * console_out, sqlite3 * db, int sup_id, int sub_id){
	sqlite3_stmt * stmt;
	int errc, id=-1;

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_CLASS_SUBORDINATION, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "could not prepare insert class sub. %d %s", errc, sqlite3_errmsg(db));
		return -1;
	}
	sqlite3_bind_int(stmt,1,sub_id);
	sqlite3_bind_int(stmt,2,sup_id);
	errc = sqlite3_step(stmt);
	if(errc != SQLITE_DONE){
		fprintf(console_out, "could not step insert class sub. %d %s", errc, sqlite3_errmsg(db));
		return -1;
	}
	errc = sqlite3_exec(db, LASTID_TEACHER_SUBORDINATION, get_id_callback, &id, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "could not lastid insert class sub. %d %s", errc, sqlite3_errmsg(db));
	}
	return id;
}

int insert_teacher_sub(FILE * console_out, sqlite3 * db, int sup_id, int sub_id){
	sqlite3_stmt * stmt;
	int errc, id = -1;

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_TEACHER_SUBORDINATION, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "could not prepare insert teacher sub. %d %s", errc, sqlite3_errmsg(db));
		return -1;
	}
	sqlite3_bind_int(stmt,1,sub_id);
	sqlite3_bind_int(stmt,2,sup_id);
	errc = sqlite3_step(stmt);
	if(errc != SQLITE_DONE){
		fprintf(console_out, "could not step insert teacher sub. %d %s", errc, sqlite3_errmsg(db));
		return -1;
	}
	errc = sqlite3_exec(db, LASTID_TEACHER_SUBORDINATION, get_id_callback, &id, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "could not lastid insert teacher sub. %d %s", errc, sqlite3_errmsg(db));
	}
	return id;
}

/* TODO test. */
int insert_teacher(FILE * console_out, sqlite3 * db, Teacher * teacher, School * school){
	sqlite3_stmt * stmt;
	int errc, i;

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

	if(errc != SQLITE_DONE){
		if(console_out){
			fprintf(console_out, "Insert Teacher Errc %d %s\n", errc, sqlite3_errmsg(db));
		}
		return -1;
	}
	sqlite3_finalize(stmt);

	errc = sqlite3_exec(db, LASTID_TABLE_TEACHER, get_id_callback, &(teacher->id), NULL);
	if(errc != SQLITE_OK){
		if(console_out){
			fprintf(console_out, "Insert Teacher Lastid Errc %d %s\n", errc, sqlite3_errmsg(db));
		}
		return -1;
	}

	insert_attendance(console_out, db, INSERT_TABLE_TEACHER_ATTENDANCE, school->period_ids, teacher->periods, teacher->id, school);

	if(teacher->teaches != NULL){
		for(i = 0;  (teacher->teaches[i] != NULL) && (teacher->teaches[i]->subject != NULL); ++i){
			insert_teaches(console_out, db, teacher->teaches[i], school);
		}
	}

	if(teacher->subordinates != NULL){
		for(i = 0; i < school->n_teachers; ++i){
			if(teacher->subordinates[i]){
				insert_teacher_sub(console_out, db, teacher->id, school->teachers[i].id);
			}
		}
	}

	if(teacher->rooms != NULL){
		insert_room_relation(console_out, db, INSERT_TABLE_TEACHER_ROOM, teacher->id, teacher->rooms, school);
	}

	return teacher->id;
}

/* TODO test */
int insert_teaches(FILE * console_out, sqlite3* db, Teaches * t, School * school){
	int errc = 0;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_TEACHES, -1, &stmt, NULL);

	if(errc != SQLITE_OK){
		fprintf(console_out, "Couldn't insert Teaches %d %s\n", errc, sqlite3_errmsg(db));
		return -1;
	}

	sqlite3_bind_int(stmt,2,t->teacher->id);
	sqlite3_bind_int(stmt,3,t->subject->id);
	sqlite3_bind_int(stmt,4,t->score);

	errc = sqlite3_step(stmt);
	errc |= sqlite3_exec(db, LASTID_TABLE_TEACHES, get_id_callback, &(t->id), NULL);
	if(errc != SQLITE_DONE){
		fprintf(console_out, "Could not insert teaches. %s\n", sqlite3_errmsg(db));
		return -1;
	}
	sqlite3_finalize(stmt);

	return t->id;
}

/* TODO test. */
int insert_class(FILE * console_out, sqlite3 * db, Class * class, School * school){
	sqlite3_stmt * stmt;
	int errc, i;

	LMH_ASSERT(db != NULL && class != NULL && school != NULL);

	errc = sqlite3_prepare(db, INSERT_TABLE_CLASS, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		if(console_out){
			fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
		}
		return -1;
	}
	sqlite3_bind_text(stmt, 1, class->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, class->short_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 3, class->size);
	sqlite3_bind_int(stmt, 4, class->abstract);
	sqlite3_bind_int(stmt, 5, class->can_have_free_periods_flag);
	sqlite3_bind_int(stmt, 6, school->daily_period_ids[class->maximal_entry_period]);
	sqlite3_bind_int(stmt, 7, school->daily_period_ids[class->minimal_exit_period]);
	sqlite3_bind_int(stmt, 8, school->id);

	errc = sqlite3_step(stmt);
	errc |= sqlite3_exec(db, LASTID_TABLE_CLASS, get_id_callback, &(class->id), NULL);

	if(errc != SQLITE_DONE){
		if(console_out){
			fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
		}
		return -1;
	}
	sqlite3_finalize(stmt);

	insert_attendance(console_out, db, INSERT_TABLE_CLASS_ATTENDANCE, school->period_ids, class->periods, class->id, school);

	if(class->needs != NULL){
		errc = 	sqlite3_prepare_v2(db,INSERT_TABLE_CLASS_SUBJECT, -1, &stmt, NULL);
		if(errc == SQLITE_OK){
			for(i = 0;class->needs[i].subject != NULL; ++i){
				sqlite3_bind_int(stmt,1, class->id);
				sqlite3_bind_int(stmt,2, class->needs[i].subject->id);
				sqlite3_bind_int(stmt,3, class->needs[i].quantity);
				sqlite3_bind_int(stmt,4, -1);
				sqlite3_step(stmt);
				sqlite3_reset(stmt);
			}
			sqlite3_finalize(stmt);
		} else {
			fprintf(console_out, "Could not insert ClassSubject %s\n", sqlite3_errmsg(db));
			return -1;
		}
	}

	if(class->subordinates != NULL){
		for(i = 0; i < school->n_classes; ++i){
			if(class->subordinates[i] > 0){
				insert_class_sub(console_out, db, class->id, school->classes[i].id);
			}
		}
	}

	return class->id;
}

/* TODO test. */
int insert_room(FILE * console_out, sqlite3 * db, Room * room, School * school){
	sqlite3_stmt * stmt;
	int errc;

	LMH_ASSERT(db != NULL && room != NULL && school != NULL);
	room->id = -1;

	sqlite3_prepare(db, INSERT_TABLE_ROOM, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, room->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, room->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,  3, room->size);
	sqlite3_bind_int(stmt,  4, school->id);

	errc = sqlite3_step(stmt);
	if(errc != SQLITE_DONE){
		if(console_out){
			fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
		}
		return -1;
	}

	errc |= sqlite3_exec(db, LASTID_TABLE_ROOM, get_id_callback, &(room->id), NULL);

	if(errc != SQLITE_DONE){
		if(console_out){
			fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
		}
		return -1;
	}
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, INSERT_TABLE_ROOM_AVALIBILITY, -1, &stmt, NULL);
	for(int i = 0; i < school->n_periods; ++i){
		sqlite3_bind_int(stmt, 1, room->id);
		sqlite3_bind_int(stmt, 2, school->period_ids[i]);
		sqlite3_bind_int(stmt, 3, room->disponibility[i]);

		sqlite3_step(stmt);
		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);

	errc = sqlite3_prepare(db, INSERT_TABLE_ROOM_FEATURE, -1, &stmt, NULL);
	for(int i = 0; i < school->n_features && room->room_features && room->room_features[i] >= 0; ++i){
		if(room->room_features[i] != 0){
			sqlite3_bind_int(stmt,1, room->id);
			sqlite3_bind_int(stmt,2, school->feature_ids[i]);
			sqlite3_bind_int(stmt,3, room->room_features[i]);
			errc = sqlite3_step(stmt);
			sqlite3_reset(stmt);
		}
	}
	sqlite3_finalize(stmt);

	return room->id;
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

int insert_subject_group(FILE * console_out,sqlite3 * db, School * school, char * group_name){
	int errc, id;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare(db, INSERT_TABLE_SUBJECT_GROUP, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out,"Could not prepare insert subject group. %d %s\n", errc, sqlite3_errmsg(db));
		return -1;
	}
	sqlite3_bind_text(stmt,1, group_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,2, school->id);
	errc = sqlite3_step(stmt);
	if(errc != SQLITE_DONE){
		fprintf(console_out,"Could not step insert subject group. %d %s\n", errc, sqlite3_errmsg(db));
		return -1;
	}
	sqlite3_finalize(stmt);
	errc = sqlite3_exec(db, LASTID_TABLE_SUBJECT_GROUP, get_id_callback, &id, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out,"Could not lastid insert subject group. %d %s\n", errc, sqlite3_errmsg(db));
		return -1;
	}
	return id;
}

int insert_subject_in_group(FILE * console_out,sqlite3 * db, int subj_id, int group_id){
	int errc, id;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare(db, INSERT_TABLE_SUBJECT_IN_GROUP, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out,"Could not prepare insert subjectingroup. %s\n", sqlite3_errmsg(db));
		return -1;
	}

	sqlite3_bind_int(stmt,1,subj_id);
	sqlite3_bind_int(stmt,2,group_id);
	errc = sqlite3_step(stmt);

	if(errc != SQLITE_DONE){
		fprintf(console_out,"Could not step insert subjectingroup. %s\n", sqlite3_errmsg(db));
		return -1;
	}
	sqlite3_finalize(stmt);
	errc = sqlite3_exec(db, LASTID_TABLE_SUBJECT_IN_GROUP, get_id_callback, &id, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out,"Could not lastid insert subjectingroup. %s\n", sqlite3_errmsg(db));
		return -1;
	}
	return id;
}

/* TODO test */
int insert_subject(FILE * console_out, sqlite3* db, Subject * subject, School * school){
	int errc = 0, i = 0;
	sqlite3_stmt * stmt = NULL;

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_SUBJECT, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		sqlite3_bind_text(stmt,1, subject->name, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt,2, subject->short_name, -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt,3, school->id);

		errc = sqlite3_step(stmt);
		if(errc != SQLITE_DONE){
			fprintf(console_out,"Could not insert subject. %s\n", sqlite3_errmsg(db));
			return -1;
		}
		errc = sqlite3_exec(db, LASTID_TABLE_SUBJECT, get_id_callback, &subject->id, NULL);
		if(errc != SQLITE_OK){
			fprintf(console_out,"Could not lastid subject. %s\n", sqlite3_errmsg(db));
			return -1;
		}
		sqlite3_finalize(stmt);

		errc = sqlite3_prepare(db, INSERT_TABLE_SUBJECT_IN_GROUP, -1, &stmt, NULL);
		if(errc != SQLITE_OK){
			fprintf(console_out,"Could not insert subjectingroup. %s\n", sqlite3_errmsg(db));
			return -1;
		}
		for(i = 0; i < school->n_subjects && subject->in_groups && subject->in_groups[i] >= 0 ; ++i){
			if(subject->in_groups[i] > 0){
				sqlite3_bind_int(stmt,1,subject->id);
				sqlite3_bind_int(stmt,2,school->subject_group_ids[i]);
				errc = sqlite3_step(stmt);

				if(errc != SQLITE_DONE){
					fprintf(console_out,"Could not insert subjectingroup. %s\n", sqlite3_errmsg(db));
					return -1;
				}
				sqlite3_reset(stmt);
			}
		}

		sqlite3_finalize(stmt);
	} else {
		fprintf(console_out,"Could not insert subject. %s\n", sqlite3_errmsg(db));
	}
	return subject->id;
}

/* TODO test */
int insert_all_meetings(FILE * console_out, sqlite3* db, School * school){
	int i,j, errc;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_MEETING, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		for(i = 0; i < school->n_meetings; ++i){
			sqlite3_bind_int(stmt,1, school->meetings[i].m_class->id);
			sqlite3_bind_int(stmt,2, school->meetings[i].subj->id);
			sqlite3_bind_int(stmt,3, school->id);

			errc = sqlite3_step(stmt);
			sqlite3_reset(stmt);
			if(errc != SQLITE_OK){
				printf("could not insert meeting . %d %s\n", errc, sqlite3_errmsg(db));
			}
		}
		sqlite3_finalize(stmt);
	} else {
		printf("Was not ok to insert all meetings. %d %s\n", errc, sqlite3_errmsg(db));
		return -1;
	}

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_POSSIBLE_PERIOD, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		for(i = 0; i < school->n_meetings; ++i){
			if(school->meetings[i].possible_periods != NULL){
				for(j = 0; j < school->n_periods && school->meetings[i].possible_periods[j] >= 0; ++j){
					sqlite3_bind_int(stmt,1, school->meetings[i].possible_periods[j]);
					sqlite3_bind_int(stmt,2, school->meetings[i].id);
					sqlite3_bind_int(stmt,3, school->period_ids[j]);
					errc = sqlite3_step(stmt);
					sqlite3_reset(stmt);
					if(errc != SQLITE_OK){
						printf("could not insert possible period. %d %s\n", errc, sqlite3_errmsg(db));
					}
				}
			}
		}
		sqlite3_finalize(stmt);
	} else {
		printf("Was not ok to insert meeting possible period. %d %s\n", errc, sqlite3_errmsg(db));
	}

	errc = sqlite3_prepare(db, INSERT_TABLE_POSSIBLE_TEACHER, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		for(i = 0; i < school->n_meetings; ++i){
			if(school->meetings[i].possible_teachers != NULL){
				for(j = 0; j < school->n_teachers && school->meetings[i].possible_teachers[j] >= 0; ++j){
					sqlite3_bind_int(stmt,1, school->meetings[i].possible_teachers[j]);
					sqlite3_bind_int(stmt,2, school->meetings[i].id);
					sqlite3_bind_int(stmt,3, school->teachers[j].id);
					errc = sqlite3_step(stmt);
					sqlite3_reset(stmt);
					if(errc != SQLITE_OK){
						printf("could not insert possible teacher. %d %s\n", errc, sqlite3_errmsg(db));
					}
				}
			}
		}
		sqlite3_finalize(stmt);
	} else {
		printf("Was not ok to insert meeting possible teacher. %d %s\n", errc, sqlite3_errmsg(db));
	}

	errc = sqlite3_prepare(db, INSERT_TABLE_POSSIBLE_ROOM, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		for(i = 0; i < school->n_meetings; ++i){
			if(school->meetings[i].possible_rooms != NULL){
				for(j = 0; j < school->n_rooms && school->meetings[i].possible_rooms[j] >= 0; ++j){
					sqlite3_bind_int(stmt,1, school->meetings[i].possible_rooms[j]);
					sqlite3_bind_int(stmt,2, school->meetings[i].id);
					sqlite3_bind_int(stmt,3, school->rooms[j].id);
					errc = sqlite3_step(stmt);
					sqlite3_reset(stmt);
					if(errc != SQLITE_OK){
						printf("could not insert possible room. %d %s\n", errc, sqlite3_errmsg(db));
					}
				}
			}
		}
		sqlite3_finalize(stmt);
	} else {
		printf("Was not ok to insert meeting possible room. %d %s\n", errc, sqlite3_errmsg(db));
	}
}

/* TODO test. */
int insert_solution(FILE * console_out, sqlite3 * db, School * school, Solution * sol){
	int i, errc = 0;
	sqlite3_stmt * stmt;
	printf("Inserting solution.\n");

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_SOLUTION, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		sqlite3_bind_text(stmt,2, "?", -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt,3,sol->name, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt,4,sol->desc, -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt,5,school->id);

		errc = sqlite3_step(stmt);
		if(errc != SQLITE_DONE){
			fprintf(console_out, "Could not insert solution. %d %s", errc, sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return -1;
		}

		errc = sqlite3_exec(db, LASTID_TABLE_SOLUTION, get_id_callback, &(sol->id), NULL);
		if(errc != SQLITE_OK){
			fprintf(console_out, "Could not fetch lastid solution. %d %s", errc, sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return -1;
		}

	} else {
		fprintf(console_out, "Could not prepare insert solution. %d %s", errc, sqlite3_errmsg(db));
		return -1;
	}

	errc = sqlite3_prepare(db, INSERT_TABLE_MEETING_SOLUTION, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "Could not insert all meetings in the solution. %d %s", errc, sqlite3_errmsg(db));
		return -1;
	}
	for(i = 0; sol->meetings[i].m_class != NULL; ++i){
		sqlite3_bind_int(stmt, 1, school->meetings[i].id);
		sqlite3_bind_int(stmt, 2, sol->id);
		sqlite3_bind_int(stmt, 3, school->period_ids[ sol->meetings[i].period ]);
		sqlite3_bind_int(stmt, 4, sol->meetings[i].teacher->id);
		sqlite3_bind_int(stmt, 5, sol->meetings[i].room->id);

		errc = sqlite3_step(stmt);
		if(errc != SQLITE_DONE){
			if(console_out){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			}
			break;
		}

		errc |= sqlite3_exec(db, LASTID_TABLE_MEETING, get_id_callback, &(sol->meetings[i].id), NULL);

		if(errc != SQLITE_DONE){
			if(console_out){
				fprintf(console_out, "Errc %d %s\n", errc, sqlite3_errmsg(db));
			}
			break;
		}
		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
	return sol->id;
}

/* TODO test. */
static bool insert_all_teaches(FILE * console_out, sqlite3 * db, School * school){
	int i = 0;
	for(i = 0; i < school->n_teaches; ++i){
		insert_teaches(console_out, db, &school->teaches[i], school);
	}
	return true;
}

/* TODO test. */
static bool insert_all_teachers(FILE * console_out, sqlite3 * db, School * school){
	int i;
	printf("Insert all teachers.\n");
	for(i = 0; i < school->n_teachers; ++i){
		insert_teacher(console_out,db,&(school->teachers[i]), school);
	}
	return false;
}

/* TODO test */
static bool insert_all_rooms(FILE * console_out, sqlite3 * db, School * school){
	int i = 0;
	for(i = 0; i < school->n_rooms; ++i){
		insert_room(console_out, db, &school->rooms[i], school);
	}
	return true;
}

/* TODO test. */
static bool insert_all_classes(FILE * console_out, sqlite3 * db, School * school){
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
static bool insert_all_features(FILE * console_out, sqlite3* db, School * school){
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
static bool insert_all_periods(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i;
	int errc;

	printf("Inserting all periods.");

	school->period_ids = calloc(school->n_periods, sizeof((*school->period_ids)));
	errc = sqlite3_prepare(db, INSERT_TABLE_PERIOD, -1, &stmt, NULL);
	if(errc != SQLITE_OK) {
		printf("Something inserting all periods\n");
		return false;
	}
	for(i = 0; i < school->n_periods; i++){
		if(school->period_names != NULL){
			sqlite3_bind_text(stmt,2, school->period_names[i], -1, SQLITE_TRANSIENT);
		} else {
			sqlite3_bind_null(stmt,2);
		}
		sqlite3_bind_int(stmt,3, school->periods[i]);
		sqlite3_bind_int(stmt,4, school->day_ids[i / school->n_periods_per_day	]);
		sqlite3_bind_int(stmt,5, school->daily_period_ids[i % school->n_periods_per_day] );
		sqlite3_bind_int(stmt,6, school->id);
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
static bool insert_all_daily_periods(FILE * console_out, sqlite3* db, School * school){
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
static bool insert_all_days(FILE * console_out, sqlite3* db, School * school){
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
		school->id = -1;
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
		if(school->rooms != NULL){
			insert_all_rooms(console_out, db, school);
		}
		if(school->classes != NULL){
			insert_all_classes(console_out, db, school);
		}
		if(school->teachers != NULL){
			insert_all_teachers(console_out, db, school);
		}
		if(school->meetings != NULL){
			insert_all_meetings(console_out, db, school);
		}
		/* Double insertion. */
		// if(school->teaches != NULL){
		// 	insert_all_teaches(console_out, db, school);
		// }
		if(school->solutions != NULL){
			for(int i = 0; i < school->n_solutions; ++i){
				insert_solution(console_out, db, school, &school->solutions[i]);
			}
		}
	}
	return school->id;
}

/* ------------------------------------------------------------------------- *
 * ------------------------------------------------------------------------- */

static char * copy_sqlite_string(sqlite3_stmt * stmt, int col_i){
	char * str;
	sqlite3_column_text(stmt,col_i); /* Defines the type to utf8 */
	int sz = (sqlite3_column_bytes(stmt,col_i));
	str = calloc(sz + 1, sizeof(char));
	strncpy(str,(const char * )sqlite3_column_text(stmt,col_i),sz);
	return str;
}

static int * select_attendance(FILE * console_out, sqlite3* db, const char * const sql, int id, School * school){
	int i = 0, errc = 0, i_per = 0;
	sqlite3_stmt * stmt;
	int * arr = NULL;

	int id_period;

	errc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if(errc == SQLITE_OK){
		sqlite3_bind_int(stmt,1,id);
		errc = sqlite3_step(stmt);

		arr = calloc(school->n_periods + 1, sizeof(int));
		while(errc == SQLITE_ROW){
			id_period = sqlite3_column_int(stmt,2);
			for(i_per = 0; i_per < school->n_periods; ++i_per){
				if(school->period_ids[i_per] == id_period){
					arr[i_per] = sqlite3_column_int(stmt,4);
					break;
				}
			}
			i++;
			errc = sqlite3_step(stmt);
		}
		arr[school->n_periods] = -1;
	} else {
		fprintf(console_out, "Couldn't select attendance.\n");
	}

	return arr;

}

static int * select_room_availibility(FILE * console_out, sqlite3* db, int room_id, School * school){
	int i = 0, errc = 0, i_per = 0;
	sqlite3_stmt * stmt;
	int * arr = NULL;
	int id_period;

	errc = sqlite3_prepare_v2(db, SELECT_ROOM_AVAILIBILITY_BY_ROOM_ID, -1, &stmt, NULL);

	if(errc == SQLITE_OK){
		sqlite3_bind_int(stmt,1,room_id);
		errc = sqlite3_step(stmt);

		arr = calloc(school->n_periods + 1, sizeof(int));
		while(errc == SQLITE_ROW){
			id_period = sqlite3_column_int(stmt,2);
			for(i_per = 0; i_per < school->n_periods; ++i_per){
				if(school->period_ids[i_per] == id_period){
					arr[i_per] = sqlite3_column_int(stmt,3);
					break;
				}
			}
			i++;
			errc = sqlite3_step(stmt);
		}
		arr[school->n_periods] = -1;
	} else {
		fprintf(console_out, "Couldn't select attendance.\n");
	}
	return arr;
}

/*
 * Returns a null-terminated list of strings, all freeable.
 * In case of error, returns null
 */
char** select_all_school_names(FILE * console_out, sqlite3* db, int ** ids){
	int i = 0, errc, alloc_sz;
	sqlite3_stmt * stmt;
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
			if(ids != NULL){
				(*ids)[i] = sqlite3_column_int(stmt,0);
			}
			strings[i] = copy_sqlite_string(stmt,1);

			i++;
			if(i >= alloc_sz){
				alloc_sz += 10;
				strings = realloc(strings, (alloc_sz)*sizeof(char*));
				if(*ids != NULL){
					*ids = realloc(*ids, alloc_sz*sizeof(int));
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

static int select_all_meetings_by_solution_id(FILE * console_out, sqlite3* db, School * school, Solution * sol){
	int i = 0, errc, id, j;
	sqlite3_stmt* stmt;
	errc = sqlite3_prepare_v2(db, SELECT_MEETING_SOLUTION_BY_SOLUTION_ID, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		sqlite3_bind_int(stmt,1, sol->id);
		errc = sqlite3_step(stmt);
		/* Double check because we can't alloc if there aren't any */
		if(errc == SQLITE_ROW){
			sol->meetings = calloc(11, sizeof(Meeting));
			while(errc == SQLITE_ROW){
				sol->meetings[i].id = sqlite3_column_int(stmt,0);

				id = sqlite3_column_int(stmt,2);
				for(j = 0; j < school->n_periods; ++j){
					if(school->period_ids[j] == id){
						sol->meetings[i].period = j;
						break;
					}
				}

				id = sqlite3_column_int(stmt,3);
				for(j = 0; j < school->n_classes; ++j){
					if(school->classes[j].id == id){
						sol->meetings[i].m_class = &(school->classes[j]);
						break;
					}
				}

				id = sqlite3_column_int(stmt,4);
				for(j = 0; j < school->n_teachers; ++j){
					if(school->teachers[j].id == id){
						sol->meetings[i].teacher = &(school->teachers[j]);
						break;
					}
				}

				id = sqlite3_column_int(stmt,5);
				for(j = 0; j < school->n_subjects; ++j) {
					if(school->subjects[j].id == id){
						sol->meetings[i].subj = &(school->subjects[j]);
						break;
					}
				}

				id = sqlite3_column_int(stmt,6);
				for(j = 0; j < school->n_rooms; ++j){
					if(school->rooms[j].id == id){
						sol->meetings[i].room = &(school->rooms[j]);
						break;
					}
				}

				errc = sqlite3_step(stmt);
				++i;
				if(i % 10 == 0){
					sol->meetings = realloc(sol->meetings, (i + 11)*sizeof(Meeting));
				}
			}
			sol->n_meetings = i;
		}
	} else {
		fprintf(console_out, "Could not select all meetings. %s\n", sqlite3_errmsg(db));
	}
	return i;
}

static int select_all_solutions(FILE * console_out, sqlite3* db, School * school){
	int errc, i = 0;
	sqlite3_stmt * stmt= NULL;

	LMH_ASSERT(school != NULL && db != NULL);

	fprintf(console_out,"Selecting all solutions\n");

	errc = sqlite3_prepare_v2(db, SELECT_SOLUTION_BY_SCHOOL_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "Could not prepare select solutions. %d %s\n", errc, sqlite3_errmsg(db));
		return -1;
	}
	sqlite3_bind_int(stmt,1,school->id);
	errc = sqlite3_step(stmt);

	if(errc == SQLITE_ROW){
		school->solutions = calloc(11, sizeof(Solution));
		while(errc == SQLITE_ROW){
			school->solutions[i].id = sqlite3_column_int(stmt,0);
			school->solutions[i].name = copy_sqlite_string(stmt,2);
			school->solutions[i].desc = copy_sqlite_string(stmt,3);

			++i;
			errc = sqlite3_step(stmt);

			if(i % 10 == 0){
				school->solutions = realloc(school->solutions, (i + 11)*sizeof(Solution));
			}
		}
		school->n_solutions = i;
		for(i = 0; i < school->n_solutions; ++i){
			select_all_meetings_by_solution_id(console_out, db, school, &school->solutions[i]);
		}
	}
	return school->n_solutions;
}

static int select_all_meetings(FILE * console_out, sqlite3* db, School * school){
	int errc, i = 0;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, SELECT_MEETING_BY_SCHOOL_ID, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		sqlite3_bind_int(stmt, 1, school->id);
		errc = sqlite3_step(stmt);

		if(errc == SQLITE_ROW){
			school->meetings = calloc(11, sizeof(Meeting));
			while(errc == SQLITE_ROW){
				school->meetings[i].m_class = find_class_by_id(school, sqlite3_column_int(stmt,1));
				school->meetings[i].subj = find_subject_by_id(school, sqlite3_column_int(stmt,2));
				sqlite3_step(stmt);
				sqlite3_reset(stmt);
				++i;
				if(i % 10 == 0){
					school->meetings = realloc(school->meetings, (i + 11)*sizeof(Meeting));
				}
			}
		}
		school->n_meetings = i;
	} else {
		fprintf(console_out, "Was not ok to select all meetings\n");
	}
}

static bool select_all_room_features_by_room_id(FILE * console_out, sqlite3* db, Room * room, School * school){
	int errc = 0, id_feature, j;
	sqlite3_stmt * stmt = NULL;

	LMH_ASSERT(room != NULL && school != NULL && db != NULL);

	fprintf(console_out, "Selecting room features by room id.\n");

	errc = sqlite3_prepare_v2(db, SELECT_ROOM_FEATURE_BY_ROOM_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out,"Could not select room features. %s\n", sqlite3_errmsg(db));
		return false;
	}
	sqlite3_bind_int(stmt, 1, room->id);
	errc = sqlite3_step(stmt);
	if(errc == SQLITE_ROW){
		room->room_features = calloc(school->n_features + 1, sizeof(int));
		while(errc == SQLITE_ROW){
			id_feature = sqlite3_column_int(stmt,2);
			bool found_correspondence = false;
			for(j = 0; j < school->n_features; ++j){
				if(school->feature_ids[j] == id_feature){
					found_correspondence = true;
					room->room_features[j] = sqlite3_column_int(stmt,3);
					break;
				}
			}
			room->room_features[school->n_features] = -1;
			if(!found_correspondence){
				fprintf(console_out, "Could not find feature id.\n");
			}
			errc = sqlite3_step(stmt);
		}
	}
	return true;
}

/* TODO test. */
static Room * select_all_rooms_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_rooms, School * school){
	int i = 0, errc = 0, alloc_sz = 0;
	Room * rooms = NULL;
	sqlite3_stmt * stmt = NULL;

	errc = sqlite3_prepare_v2(db, SELECT_ROOM_BY_SCHOOL_ID, -1, &stmt, NULL);

	fprintf(console_out,"Selecting all rooms.\n");
	if(errc != SQLITE_OK){
		fprintf(console_out,"Could not select rooms.\n");
	} else {
		sqlite3_bind_int(stmt,1, id);
		errc = sqlite3_step(stmt);

		alloc_sz = 10;
		rooms = calloc(alloc_sz + 1, sizeof(Room));
		i = 0;
		while(errc == SQLITE_ROW){
			if(i >= alloc_sz -1){
				alloc_sz += 10;
				rooms = realloc(rooms, (1+ alloc_sz)*sizeof(Room));
			}

			rooms[i].id = sqlite3_column_int(stmt,0);

			rooms[i].name = calloc( sqlite3_column_bytes(stmt,1) + 1, sizeof(char));
			strncpy(rooms[i].name, (const char *)sqlite3_column_text(stmt,1), sqlite3_column_bytes(stmt,1));


			rooms[i].short_name = calloc( sqlite3_column_bytes(stmt,2) + 1, sizeof(char));
			strncpy(rooms[i].short_name, (const char *)sqlite3_column_text(stmt,2), sqlite3_column_bytes(stmt,2));

			rooms[i].size = sqlite3_column_int(stmt,3);

			errc = sqlite3_step(stmt);

			rooms[i].disponibility = select_room_availibility(console_out, db, rooms[i].id, school);

			select_all_room_features_by_room_id(console_out, db, &rooms[i], school);
			++i;
		}
		sqlite3_finalize(stmt);

	}
	if(n_rooms != NULL){
		*n_rooms = i;
	}
	return rooms;
}

static int * select_teacher_subordinates_by_teacher_id(FILE * console_out, sqlite3 * db, Teacher * t, School * school){
	sqlite3_stmt * stmt;
	int errc, i,j, sub_id;

	errc = sqlite3_prepare_v2(db, SELECT_TEACHER_SUBORDINATION_BY_SUP_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "Could not prepare select teacher sub. %d %s \n", errc, sqlite3_errmsg(db));
		return NULL;
	}
	sqlite3_bind_int(stmt,1,t->id);
	errc = sqlite3_step(stmt);
	if(errc != SQLITE_DONE && errc != SQLITE_ROW){
		fprintf(console_out, "Could not select teacher sub. %d %s \n", errc, sqlite3_errmsg(db));
		return NULL;
	}
	if(errc == SQLITE_ROW) {
		t->subordinates = calloc(1+ school->n_teachers, sizeof(int));

		while(errc == SQLITE_ROW){
			sub_id = sqlite3_column_int(stmt,1);
			for(j = 0; j < school->n_teachers; ++j){
				if(sub_id == school->teachers[j].id){
					t->subordinates[j] = 1;
					break;
				}
			}
			errc = sqlite3_step(stmt);
			++i;
		}
	}
	return t->subordinates;
}

/* TODO test. */
static Teacher * select_all_teachers_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_teachers, School * school){
	int i = 0, errc = 0, n = 0;
	Teacher * teachers;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, SELECT_TEACHER_BY_SCHOOL_ID, -1, &stmt, NULL);

	if(errc != SQLITE_OK){
		fprintf(console_out, "Could not select all teachers.\n");
	} else {
		sqlite3_bind_int(stmt,1,id);
		errc= sqlite3_step(stmt);
		teachers = calloc(11, sizeof(Teacher));
		while(errc == SQLITE_ROW){
			if(i % 10 == 0 && i > 0){
				teachers = realloc(teachers,(i + 11)*sizeof(Teacher));
			}
			teachers[i].id = sqlite3_column_int(stmt,0);
			teachers[i].name = copy_sqlite_string(stmt,1);
			teachers[i].short_name = copy_sqlite_string(stmt,2);
			teachers[i].max_days = sqlite3_column_int(stmt,3);
			teachers[i].max_meetings_per_day = sqlite3_column_int(stmt,4);
			teachers[i].max_meetings_per_class_per_day = sqlite3_column_int(stmt,5);
			teachers[i].max_meetings = sqlite3_column_int(stmt,6);
			teachers[i].num_planning_periods = sqlite3_column_int(stmt,7);
			teachers[i].periods = select_attendance(console_out, db, SELECT_TEACHER_ATTENDANCE_BY_TEACHER_ID, teachers[i].id, school);

			errc = sqlite3_step(stmt);
			++i;
		}
	}
	school->teachers = teachers;
	n = i;
	if(n_teachers != NULL){
		*n_teachers = n;
	}
	for(i = 0; i < n; ++i){
		teachers[i].subordinates = select_teacher_subordinates_by_teacher_id(console_out, db, &teachers[i], school);
	}
	return teachers;
}

bool select_all_class_subject_by_class_id(FILE * console_out, sqlite3* db, Class * class, School * school){
	int i = 0, errc = 0, subj_id, i_subj = 0, alloc_sz = 0;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, SELECT_CLASS_SUBJECT_BY_CLASS_ID, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		sqlite3_bind_int(stmt,1, class->id);
		errc = sqlite3_step(stmt);

		class->needs = calloc(alloc_sz + 1, sizeof(Class));
		while(errc == SQLITE_ROW){
			subj_id = sqlite3_column_int(stmt,2);
			bool found = false;
			for(i_subj = 0; i_subj < school->n_subjects; ++i_subj){
				if(school->subjects[i_subj].id == subj_id){
					found = true;
					class->needs[i].subject = &(school->subjects[i_subj]);
					class->needs[i].quantity = sqlite3_column_int(stmt,3);

					printf("Found it sir.\n");
				}
			}
			if(! found){
				printf("did not find it sr\n");
			}

			++i;
			errc = sqlite3_step(stmt);
			if(i >= alloc_sz){
				alloc_sz += 10;
				class->needs = realloc(class->needs, (alloc_sz+1)*sizeof(SubjectQuantity));
			}
		}
		if(i > 0){
			class->needs[i].subject = NULL;
		}
		return true;
	} else {
		fprintf(console_out, "Could not select class subjects. %s\n", sqlite3_errmsg(db));
	}
	return false;
}

static int * select_all_class_subordination_by_class_id(FILE * console_out, sqlite3* db, Class * class, School * school){
	int errc, i, subid;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, SELECT_CLASS_SUBORDINATION_BY_SUP_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "Could not prepare select classub. %d %s", errc, sqlite3_errmsg(db));
		return NULL;
	}
	sqlite3_bind_int(stmt,1, class->id);
	errc = sqlite3_step(stmt);
	if(errc == SQLITE_ROW){
		class->subordinates = calloc(school->n_classes + 1, sizeof(int));
		class->subordinates[school->n_classes] = -1;
		while(errc == SQLITE_ROW){
			subid = sqlite3_column_int(stmt,1);
			printf("Searching for subid %d \n", subid);
			for(i = 0; i < school->n_classes; ++i){
				printf("Comparing to subid %d \n", school->classes[i].id);
				if(subid == school->classes[i].id){
					printf("%s Subordinates %s.\n", class->name, school->classes[i].name);
					class->subordinates[i] = 1;
				}
			}

			errc = sqlite3_step(stmt);
		}
	}
	return class->subordinates;
}

/* TODO test. */
static Class * select_all_classes_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_classes, School * school){
	int i = 0, n, j, alloc_sz = 0, errc;
	sqlite3_stmt * stmt;
	Class * classes = NULL;

	errc = sqlite3_prepare_v2(db, SELECT_CLASS_BY_SCHOOL_ID, -1, &stmt, NULL);

	fprintf(console_out, "Selecting all classes.\n");
	if(errc != SQLITE_OK){
		fprintf(console_out, "Could not select all classes.");
	} else {
		sqlite3_bind_int(stmt,1,id);
		errc = sqlite3_step(stmt);

		alloc_sz = 10;
		classes = calloc(alloc_sz, sizeof(Class));

		while(errc == SQLITE_ROW){
			if(i >= alloc_sz){
				alloc_sz += 10;
				classes = realloc(classes, alloc_sz * sizeof(Class));
			}

			classes[i].id = sqlite3_column_int(stmt,0);
			classes[i].name = copy_sqlite_string(stmt,1);
			classes[i].short_name = copy_sqlite_string(stmt,2);
			classes[i].size = sqlite3_column_int(stmt,3);
			classes[i].abstract = sqlite3_column_int(stmt,4);
			classes[i].can_have_free_periods_flag = sqlite3_column_int(stmt,5);
			/* Id, temporarily */
			classes[i].maximal_entry_period = sqlite3_column_int(stmt,6);
			for(j = 0; j < school->n_periods_per_day; ++j){
				if(school->daily_period_ids[j] == classes[i].maximal_entry_period){
					classes[i].maximal_entry_period = j;
					break;
				}
			}
			classes[i].minimal_exit_period = sqlite3_column_int(stmt,7);
			for(j = 0; j < school->n_periods_per_day; ++j){
				if(school->daily_period_ids[j] == classes[i].minimal_exit_period){
					classes[i].minimal_exit_period = j;
					break;
				}
			}
			classes[i].periods = select_attendance(console_out, db, SELECT_CLASS_ATTENDANCE_BY_CLASS_ID, classes[i].id, school);

			select_all_class_subject_by_class_id(console_out, db, &classes[i], school);

			++i;
			errc = sqlite3_step(stmt);

		}
	}
	school->classes = classes;
	n = i;
	if(n_classes != NULL){
		*n_classes = i;
	}

	for(i = 0; i < n; ++i){
		select_all_class_subordination_by_class_id(console_out, db, &classes[i], school);
	}
	return classes;
}

/* TODO test. */
static Teaches * select_all_teaches_by_school_id(FILE * console_out, sqlite3 * db, int id, int * n_teaches, School * school){
	int i = 0, j, subject_id, teacher_id, i_teacher = 0, i_teaches, errc = 0, i_tt;
	sqlite3_stmt * stmt;
	Teaches * teaches = NULL;
	/* First saving all in school.teaches */
	errc = sqlite3_prepare_v2(db, SELECT_TEACHES_BY_SCHOOL_ID, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		sqlite3_bind_int(stmt,1, id);
		errc = sqlite3_step(stmt);

		teaches = calloc(11, sizeof(Teaches));
		while(errc == SQLITE_ROW){
			teaches[i].id = sqlite3_column_int(stmt,0);

			teacher_id = sqlite3_column_int(stmt,1);
			for(j = 0; j < school->n_teachers; ++j){
				if(school->teachers[j].id == teacher_id){
					teaches[i].teacher = &(school->teachers[j]);

				}
			}

			subject_id = sqlite3_column_int(stmt,2);
			for(j = 0; (j < school->n_subjects) && (school->subjects != NULL) && (school->subjects[j].name != NULL); ++j){
				if(school->subjects[j].id == subject_id){
					teaches[i].subject = &(school->subjects[j]);
				}
			}

			teaches[i].score = sqlite3_column_int(stmt,3);
			errc = sqlite3_step(stmt);
			++i;
			if(i % 10 == 0){
				teaches = realloc(teaches, (i + 11)*sizeof(Teaches));
			}
		}
	} else {
		printf("Couldn't select all teaches. %s\n", sqlite3_errmsg(db));
	}
	/* Then storing at each teacher */
	for(i_teacher = 0; i_teacher < school->n_teachers; ++i_teacher){
		int teaches_ctr = 0;
		/* Count how many to store */
		for(i_teaches = 0; i_teaches < school->n_teaches; ++i_teaches){
			if(school->teaches[i_teaches].teacher->id == school->teachers[i_teacher].id){
				teaches_ctr++;
			}
		}
		/* Alloc and store at school.teacher[i_teacher].teaches[...] */
		school->teachers[i_teacher].teaches = calloc(1 + teaches_ctr, sizeof(Teaches *));
		i_tt = 0;
		for(i_teaches = 0; i_teaches < school->n_teaches; ++i_teaches){
			if(school->teachers[i_teacher].id == school->teaches[i_teaches].teacher->id){
				school->teachers[i_teacher].teaches[i_tt] = &(school->teaches[i_teaches]);
				++i_tt;
				break;
			}
		}

	}


	if(n_teaches != NULL){
		*n_teaches = i;
	}
	return teaches;
}

/* TODO test */
static bool select_all_features_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_features, School * school){
	int i = 0, errc = 0;
	sqlite3_stmt * stmt;

	fprintf(console_out, "Selecting all features.\n");

	errc = sqlite3_prepare(db, SELECT_FEATURE_BY_SCHOOL_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "Couldn't select all features");
	} else {
		sqlite3_bind_int(stmt,1,id);
		errc = sqlite3_step(stmt);

		school->feature_names = calloc(11, sizeof(char*));
		school->feature_ids = calloc(11, sizeof(int));

		while(errc == SQLITE_ROW){
			school->feature_ids[i] = sqlite3_column_int(stmt,0);
			school->feature_names[i] = copy_sqlite_string(stmt,1);
			printf("name of the %d th feature is %s\n", i, school->feature_names[i]);
			errc = sqlite3_step(stmt);
			++i;
			if(i % 10 == 0){
				school->feature_names = realloc(school->feature_names, (school->n_features + 11) * sizeof(char*));
				school->feature_ids = realloc(school->feature_ids, (school->n_features + 11) * sizeof(int));
			}
		}
		if(n_features != NULL){
			*n_features = i;
		}

		return true;
	}
	return false;
}

/* TODO test */
static bool select_all_subjects_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_subjects, School * school){
	int i = 0, n, errc = 0;
	sqlite3_stmt * stmt;

	fprintf(console_out, "Selecting all subjects.\n");

	errc = sqlite3_prepare(db, SELECT_SUBJECT_BY_SCHOOL_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "Couldn't select all subjects %d %s\n", errc, sqlite3_errmsg(db));
	} else {
		sqlite3_bind_int(stmt,1,id);
		errc = sqlite3_step(stmt);

		school->subjects = calloc(11, sizeof(Subject));

		while(errc == SQLITE_ROW){
			school->subjects[i].id = sqlite3_column_int(stmt,0);
			school->subjects[i].name = copy_sqlite_string(stmt,1);
			school->subjects[i].short_name = copy_sqlite_string(stmt,2);
			errc = sqlite3_step(stmt);
			++i;
			if(i % 10 == 0 ){
				school->subjects = realloc(school->subjects, (11 + i)*sizeof(Subject));
			}
		}
		n = i;
		if(n_subjects != NULL){
			*n_subjects = i;
		}
		sqlite3_finalize(stmt);

		errc = sqlite3_prepare(db, SELECT_TABLE_SUBJECT_IN_GROUP_BY_SUBJECT_ID, -1, &stmt, NULL);
		if(errc == SQLITE_OK){
			for(i = 0; i < n; ++i){
				sqlite3_bind_int(stmt, 1, school->subjects[i].id);
				errc = sqlite3_step(stmt);
				if(errc == SQLITE_ROW){
					school->subjects[i].in_groups = calloc(school->n_subjects + 1, sizeof(int));
					school->subjects[i].in_groups[school->n_subjects]  = -1;
				}
				while(errc == SQLITE_ROW){
					bool found = false;
					int id_group = sqlite3_column_int(stmt,2);
					for(int j = 0; j < school->n_subject_groups; ++j){
						if(id_group == school->subject_group_ids[j]){
							found = true;
							/* TODO FIXME is there any purpouse of scoring? Maybe a bool suffices. */
							school->subjects[i].in_groups[j] = 1;
							break;
						}
					}
					if(!found){
						fprintf(console_out, "Did not found subject_in_group groupid %d\n", id_group);
					}
					errc = sqlite3_step(stmt);
				}
				if(errc != SQLITE_DONE){
					fprintf(console_out, "Could not select subject in group. %d %s", errc, sqlite3_errmsg(db));
				}
				sqlite3_reset(stmt);
			}
		} else {
			fprintf(console_out, "Could not preapare select subject in group. %d %s", errc, sqlite3_errmsg(db));
		}

		return true;
	}
	return false;
}

/* TODO test */
static bool select_all_days_by_school_id(FILE * console_out, sqlite3 * db, int id, int * n_days, School * school){
	int i = 0, errc;
	sqlite3_stmt * stmt;

	fprintf(console_out, "Selecting all days.\n");

	errc = sqlite3_prepare_v2(db, SELECT_DAY_BY_SCHOOL_ID, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		sqlite3_bind_int(stmt,1, id);
		errc = sqlite3_step(stmt);
		school->day_names = calloc(11, sizeof(char*));
		school->day_ids = calloc(11, sizeof(int));
		while(errc == SQLITE_ROW){
			school->day_ids[i] = sqlite3_column_int(stmt,0);
			school->day_names[i] = copy_sqlite_string(stmt,1);
			errc = sqlite3_step(stmt);
			++i;
			if(i % 10 == 0){
				school->day_names = realloc(school->day_names, (i + 11)*sizeof(char*));
				school->day_ids = realloc(school->day_ids, (i + 11)*sizeof(int));
			}
		}
		if(n_days != NULL){
			*n_days = i;
		}
		sqlite3_finalize(stmt);
		return true;
	} else {
		fprintf(console_out, "Could'nt load days.\n");
		return false;
	}
}

/* TODO test */
static bool select_all_daily_periods_by_school_id(FILE * console_out, sqlite3 * db, int id, int * n_daily_periods, School * school){
	int i = 0, errc;
	sqlite3_stmt * stmt;

	fprintf(console_out, "Selecting all daily periods.\n");

	errc = sqlite3_prepare_v2(db, SELECT_DAILY_PERIOD_BY_SCHOOL_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "Could'nt load daily periods.\n");
	} else {
		sqlite3_bind_int(stmt,1, id);
		errc = sqlite3_step(stmt);

		school->daily_period_names = calloc(11, sizeof(char*));
		school->daily_period_ids = calloc(11, sizeof(int));
		while(errc == SQLITE_ROW){
			school->daily_period_ids[i] = sqlite3_column_int(stmt,0);
			school->daily_period_names[i] = copy_sqlite_string(stmt,1);
			errc = sqlite3_step(stmt);
			++i;
			if(i % 10 == 0){
				school->daily_period_names = realloc(school->daily_period_names,(i+11)*sizeof(char*));
				school->daily_period_ids = realloc(school->daily_period_ids, (i + 11)*sizeof(int));
			}
		}
		if( n_daily_periods != NULL) {
			*n_daily_periods = i;
		}
		sqlite3_finalize(stmt);
		return true;
	}
	return false;
}

/* TODO test */
static bool select_all_periods_by_school_id(FILE * console_out, sqlite3 * db, int id, int * n_periods, School * school){
	int i = 0, errc, per_i, day_per_i, day_i;
	sqlite3_stmt * stmt;

	fprintf(console_out, "Selecting all periods.\n");

	errc = sqlite3_prepare_v2(db, SELECT_PERIOD_BY_SCHOOL_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "Couldn't load periods.\n");
	} else {
		school->n_periods = school->n_periods_per_day * school->n_days;

		school->period_names = calloc(school->n_periods + 1, sizeof(char*));
		school->period_ids = calloc(school->n_periods + 1, sizeof(int));
		school->period_names[school->n_periods] = NULL;
		school->period_ids[school->n_periods] = -1;
		school->periods = calloc(school->n_periods, sizeof(bool));

		sqlite3_bind_int(stmt,1, id);
		errc = sqlite3_step(stmt);
		while(errc == SQLITE_ROW){
			day_i = get_day_index_by_id(school, sqlite3_column_int(stmt,3));
			day_per_i = get_daily_period_index_by_id(school, sqlite3_column_int(stmt,4));
			if(day_per_i != -1 && day_i != -1){
				per_i = day_i * school->n_periods_per_day + day_per_i;
				school->period_ids[per_i] = sqlite3_column_int(stmt,0);
				school->period_names[per_i] = copy_sqlite_string(stmt,1);
				school->periods[per_i] = sqlite3_column_int(stmt,2) > 0;
			} else {
				fprintf(console_out, "Could not find periods' parents: %d %d \n", day_i, day_per_i);
			}
			errc = sqlite3_step(stmt);
			++i;

		}
		sqlite3_finalize(stmt);
		return true;
	}
	return false;
}

static void link_all_teaches(School * school){
	int i, j, n;
	for(i = 0; i < school->n_teachers; ++i){
		n = 0;
		for(j = 0; j < school->n_teaches; ++j){
			if(school->teaches[j].teacher->id == school->teachers[i].id){
				++n;
			}
		}
		school->teachers[i].teaches = calloc(n+1, sizeof(Teaches*));

		n = 0;
		for(j = 0; j < school->n_teaches; ++j){
			if(school->teaches[j].teacher->id == school->teachers[i].id){
				school->teachers[i].teaches[n] = & school->teaches[j];
				++n;
			}
		}
	}
}

int select_all_subject_groups_by_school_id(FILE * console_out, sqlite3 * db, int id, int * n_groups, School * school){
	int i = 0, errc;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, SELECT_TABLE_SUBJECT_GROUP_BY_SCHOOL_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out, "Could not prepare select subject group names. %d %s\n", errc, sqlite3_errmsg(db));
		return 0;
	}
	sqlite3_bind_int(stmt, 1, id);
	errc = sqlite3_step(stmt);
	if(errc == SQLITE_ROW){
		school->subject_group_ids = calloc(11, sizeof(int));
		school->subject_group_names = calloc(11, sizeof(char*));

		while(errc == SQLITE_ROW){
			school->subject_group_ids[i] = sqlite3_column_int(stmt,0);
			school->subject_group_names[i] = copy_sqlite_string(stmt,1);

			++i;
			errc = sqlite3_step(stmt);
			if(i % 10 == 0){
				school->subject_group_ids = realloc(school->subject_group_ids, (i+11)* sizeof(int));
				school->subject_group_names = realloc(school->subject_group_ids, (i+11)* sizeof(char*));
			}
		}
	}
	if(errc != SQLITE_DONE) {
		fprintf(console_out, "Could not step select subject group names %d %s\n", errc, sqlite3_errmsg(db));
	}
	*n_groups = i;
	return i;
}

/* TODO more or less tested. */
School * select_school_by_id(FILE * console_out, sqlite3* db, int id){
	int errc;
	sqlite3_stmt * stmt;
	School * school = NULL;

	fprintf(console_out,"Selecting school by id.\n");

	errc = sqlite3_prepare_v2(db, SELECT_SCHOOL_BY_ID, -1, &stmt, NULL);
	if(errc != SQLITE_OK){
		fprintf(console_out,"Failed to prepare statement.");
		return NULL;
	}
	sqlite3_bind_int(stmt,1,id);
	errc = sqlite3_step(stmt);

	if( errc == SQLITE_ROW ){
		school = calloc(1, sizeof(School));

		school->id = id;
		school->name = copy_sqlite_string(stmt,1);
		school->n_periods_per_day = sqlite3_column_int(stmt,2);
		school->n_days = sqlite3_column_int(stmt,3);
		school->max_meetings_teacher_per_day = sqlite3_column_int(stmt,4);
		school->max_meetings_teacher_per_week = sqlite3_column_int(stmt,5);

		select_all_days_by_school_id(stdout, db, id, &(school->n_days), school);
		select_all_daily_periods_by_school_id(stdout, db, id, &(school->n_periods_per_day), school);
		select_all_periods_by_school_id(stdout, db, id, &(school->n_periods), school);
		select_all_features_by_school_id(stdout, db, id, &(school->n_features), school);
		select_all_subject_groups_by_school_id(stdout, db, id, &(school->n_subject_groups), school);
		select_all_subjects_by_school_id(stdout,db,id,&(school->n_subjects), school);
		school->rooms = select_all_rooms_by_school_id(stdout, db, id, &(school->n_rooms), school);
		school->classes = select_all_classes_by_school_id(stdout, db, id, &(school->n_classes), school);
		school->teachers = select_all_teachers_by_school_id(stdout, db, id, &(school->n_teachers), school);
		school->teaches = select_all_teaches_by_school_id(stdout, db, id, &(school->n_teaches), school);
		link_all_teaches(school);
		select_all_meetings(stdout,db, school);
		select_all_solutions(stdout, db, school);
	}
	sqlite3_finalize(stmt);

	return school;
}

static bool exec_and_check(sqlite3 * db, const char * const sql, int id){
	sqlite3_stmt * stmt;
	int errc = 0;
	bool retc = true;

	errc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		sqlite3_bind_int(stmt,1,id);
		errc =sqlite3_step(stmt);
		if(errc != SQLITE_DONE){
			printf("Could not execute. %d %s.\n", errc, sqlite3_errmsg(db));
			retc = false;
		}
		sqlite3_finalize(stmt);
	} else {
		printf("Could not execute. %d %s.\n", errc, sqlite3_errmsg(db));
		retc = false;
	}

	return retc;
}

bool save_backup(sqlite3* memory_db, const char * const filename){
	sqlite3 * file_db;
	sqlite3 * from_db;
	sqlite3 * to_db;
	sqlite3_backup * backup;
	int errc;
	bool retc = false;

	errc = sqlite3_open(filename, &file_db);

	if(errc == SQLITE_OK){
		from_db = memory_db;
		to_db = file_db;

		backup = sqlite3_backup_init(to_db, "main", from_db, "main");
		if( backup ) {
			sqlite3_backup_step(backup, -1);
			sqlite3_backup_finish(backup);
			retc = true;
		} else {
			printf("wtf\n");
		}
		errc = sqlite3_errcode(to_db);
	} else {
		printf("Não foi possível abrir o arquivo. %d %s\n", errc, sqlite3_errmsg(file_db));
	}
	sqlite3_close(file_db);

	return retc;
}

bool load_backup(sqlite3* memory_db, const char * const filename){
	sqlite3 * file_db;
	sqlite3_backup * backup;
	bool retc = false;
	int errc;

	errc = sqlite3_open(filename, &file_db);

	if(errc == SQLITE_OK){
		backup = sqlite3_backup_init(memory_db, "main", file_db, "main");
		if( backup ) {
			sqlite3_backup_step(backup, -1);
			sqlite3_backup_finish(backup);
			retc = true;
		} else {
			printf("wtf\n");
		}
		errc = sqlite3_errcode(memory_db);
	} else {
		printf("Não foi possível abrir o arquivo. %d %s\n", errc, sqlite3_errmsg(file_db));
	}
	sqlite3_close(file_db);

	return retc;
}

bool remove_feature(FILE * console_out, sqlite3* db, int id){
	return  !exec_and_check(db, DELETE_ROOM_FEATURE_BY_FEATURE_ID, id)?false:
			!exec_and_check(db, DELETE_DEMAND_BY_FEATURE_ID, id)?false:
			!exec_and_check(db, DELETE_FEATURE_BY_ID, id)?false:
			true;
}

bool remove_room(FILE * console_out, sqlite3* db, int id){
	return  !exec_and_check(db, DELETE_ROOM_BY_ID, id)?false:
			!exec_and_check(db, DELETE_ROOM_FEATURE_BY_ROOM_ID, id)?false:
			!exec_and_check(db, DELETE_ROOM_AVAILIBILITY_BY_ROOM_ID, id)?false:
			!exec_and_check(db, UNSET_MEETING_SOLUTION_ROOM_BY_ROOM_ID, id)?false:
			true;
}

bool remove_subject(FILE * console_out, sqlite3* db, int id){
	return  !exec_and_check(db,DELETE_SUBJECT_IN_GROUP_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_MEETING_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_TEACHES_TWIN_PREFERENCE_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_TEACHES_PERIOD_PREFERENCE_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_CLASS_SUBJECT_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_DEMAND_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_TEACHES_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_SUBJECT_BY_ID, id)?false:
			true;
}

bool remove_subject_group(FILE * console_out, sqlite3* db, int id){
	return  !exec_and_check(db, DELETE_CLASS_SUBJECT_GROUP_BY_GROUP_ID, id)?false:
			!exec_and_check(db, DELETE_SUBJECT_IN_GROUP_BY_GROUP_ID, id)?false:
			!exec_and_check(db, DELETE_SUBJECT_GROUP_BY_SCHOOL_ID, id)?false:true;
}

bool remove_teacher(FILE * console_out, sqlite3* db, int id) {
	return  !exec_and_check(db, DELETE_TEACHES_TWIN_PREFERENCE_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_PERIOD_PREFERENCE_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_DEMAND_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_ATTENDANCE_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_SUBORDINATION_BY_SUB_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_SUBORDINATION_BY_SUP_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_DAY_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_BY_ID, id)?false:
			true;
			// If we are to logically delete, then this is not needed. TODO FIXME
			// !exec_and_check(db, UNSET_MEETING_SOLUTION_TEACHER_BY_TEACHER_ID, id)?false:
}

bool remove_class(FILE * console_out, sqlite3* db, int id) {
	return  !exec_and_check(db, DELETE_MEETING_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBJECT_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBORDINATION_BY_SUB_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBORDINATION_BY_SUP_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_ATTENDANCE_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_BY_ID, id)?false:
			true;
}

bool remove_school(FILE * console_out, sqlite3 * db, int id){
	return	!exec_and_check(db, DELETE_POSSIBLE_PERIOD_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_POSSIBLE_ROOM_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_POSSIBLE_TEACHER_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_MEETING_SOLUTION_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_MEETING_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_SOLUTION_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_TWIN_PREFERENCE_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_PERIOD_PREFERENCE_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_DEMAND_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_ATTENDANCE_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_SUBORDINATION_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_DAY_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBJECT_GROUP_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBJECT_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_SUBJECT_GROUP_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_SUBJECT_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_ATTENDANCE_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_ROOM_AVAILIBILITY_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_ROOM_FEATURE_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_ROOM_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_FEATURE_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_PERIOD_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_DAY_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_DAILY_PERIOD_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_SCHOOL_BY_ID, id)?false:
			true;
}

// bool remove_teaches(FILE * console_out, sqlite3* db, int id);
// bool remove_meeting(FILE * console_out, sqlite3* db, int id);
