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
				"UNIQUE (name)"
			")");
const char * const INSERT_TABLE_SCHOOL =
			("INSERT INTO School(name, num_per_per_day, num_days) values(?,?,?)");
const char * const UPDATE_TABLE_SCHOOL =
			("UPDATE School SET (name, num_per_per_day, num_days) = (?,?,?) WHERE id=?");
const char * const UPDATE_SCHOOL_NAME =
			("UPDATE School SET name = ? WHERE id=?");
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
				"id 					integer primary key autoincrement not null,"
				"name					text,"
				"per_index				integer,"
				"id_school				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id),"
				"UNIQUE (per_index, id_school)"
			")");
const char * const UPSERT_TABLE_DAILY_PERIOD =
			("INSERT INTO DailyPeriod(name, per_index, id_school) VALUES (?1,?2,?3) "
			 "ON CONFLICT (per_index, id_school) DO UPDATE SET name =?1");
const char * const UPDATE_DAILY_PERIOD_NAME =
			("UPDATE DailyPeriod SET name = ? WHERE id=?");
const char * const LASTID_TABLE_DAILY_PERIOD =
			("SELECT id FROM DailyPeriod where rowid = last_insert_rowid()");
const char * const SELECT_DAILY_PERIOD_BY_SCHOOL_ID =
			("SELECT * FROM DailyPeriod WHERE id_school=? ORDER BY per_index");
const char * const DELETE_DAILY_PERIOD_BY_SCHOOL_ID =
			("DELETE FROM DailyPeriod WHERE id_school=?");

const char * const CREATE_TABLE_DAY =
			("CREATE TABLE IF NOT EXISTS Day("
				"id 					integer primary key autoincrement not null,"
				"name					text,"
				"day_index				integer,"
				"id_school				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id),"
				"UNIQUE (day_index, id_school)"
			")");
const char * const UPSERT_TABLE_DAY =
			("INSERT INTO Day(name, day_index, id_school) VALUES (?1,?2,?3) "
			 "ON CONFLICT (day_index, id_school) DO UPDATE SET name=?1");
const char * const UPDATE_DAY_NAME =
			("UPDATE Day SET (name) = (?) WHERE id=?");
const char * const LASTID_TABLE_DAY =
			("SELECT id FROM Day where rowid = last_insert_rowid()");
const char * const SELECT_DAY_BY_SCHOOL_ID =
		("SELECT * FROM Day WHERE id_school = ? ORDER BY day_index");
const char * const DELETE_DAY_BY_SCHOOL_ID =
		("DELETE FROM Day WHERE id_school = ?");

const char * const CREATE_TABLE_PERIOD =
			("CREATE TABLE IF NOT EXISTS Period("
				"id 					integer primary key autoincrement not null,"
				"name					text,"
				"school_operates_flag   integer,"
				"day_id					integer,"
				"daily_period_id		integer,"
				"id_school				integer,"
				"FOREIGN KEY (day_id) REFERENCES Day(id),"
				"FOREIGN KEY (daily_period_id) REFERENCES DailyPeriod(id),"
				"FOREIGN KEY (id_school) REFERENCES School(id),"
				"UNIQUE (day_id, daily_period_id)"
			")");
const char * const UPSERT_TABLE_PERIOD =
			("INSERT INTO Period(name, school_operates_flag, day_id, daily_period_id, id_school) VALUES (?1,?2,?3,?4,?5) "
			 "ON CONFLICT (day_id, daily_period_id) DO UPDATE SET (name, school_operates_flag, day_id, daily_period_id, id_school)=(?1,?2,?3,?4,?5)");
const char * const UPDATE_PERIOD_NAME =
			("UPDATE Period SET name = ? WHERE id=?");
const char * const UPDATE_PERIOD_OPERATES =
			("UPDATE Period SET school_operates_flag = ? WHERE id=?");
const char * const LASTID_TABLE_PERIOD =
			("SELECT id FROM Period where rowid = last_insert_rowid()");
const char * const SELECT_PERIOD_BY_SCHOOL_ID =
		("SELECT * FROM Period WHERE id_school = ?");
const char * const DELETE_PERIOD_BY_SCHOOL_ID =
		("DELETE FROM Period WHERE id_school = ?");

const char * const CREATE_TABLE_ROOM =
			("CREATE TABLE IF NOT EXISTS Room("
				"id 					integer primary key autoincrement not null,"
				"name 					text,"
				"short_name 			text,"
				"size		 			integer,"
				"active 				integer,"
				"id_school 				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id),"
				"UNIQUE (name, id_school)"
			")");
const char * const INSERT_TABLE_ROOM =
			("INSERT INTO Room(id, name, short_name, size, active, id_school) VALUES(?,?,?,?,?,?)");
const char * const UPDATE_TABLE_ROOM =
			("UPDATE Room SET (name, short_name, size, active, id_school) = (?,?,?,?,?) WHERE id=?");
const char * const LASTID_TABLE_ROOM =
			("SELECT id FROM Room where rowid = last_insert_rowid()");
const char * const SELECT_ROOM_BY_SCHOOL_ID =
			("SELECT * FROM Room WHERE id_school = ?");
const char * const DELETE_ROOM_BY_ID =
			("DELETE FROM Room WHERE id=?");
const char * const DELETE_ROOM_BY_SCHOOL_ID =
			("DELETE FROM Room WHERE id_school=?");

const char * const CREATE_TABLE_ROOM_AVAILABILITY =
			("CREATE TABLE IF NOT EXISTS RoomAvailability("
				"id						integer primary key autoincrement not null,"
				"id_room				integer,"
				"id_period				integer,"
				"score					integer,"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id),"
				"UNIQUE (id_room, id_period)"
			")");
const char * const UPSERT_TABLE_ROOM_AVAILABILITY =
			("INSERT INTO RoomAvailability(id_room, id_period, score) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_room, id_period) DO UPDATE SET score = ?3");
const char * const LASTID_TABLE_ROOM_AVAILABILITY =
			("SELECT id FROM RoomAvailability where rowid = last_insert_rowid()");
const char * const SELECT_ROOM_AVAILABILITY_BY_ROOM_ID =
			("SELECT * FROM RoomAvailability WHERE id_room=?");
const char * const DELETE_ROOM_AVAILABILITY_BY_ROOM_ID =
			("DELETE FROM RoomAvailability WHERE id_room = ?");
const char * const DELETE_ROOM_AVAILABILITY_BY_SCHOOL_ID =
			("DELETE FROM RoomAvailability WHERE EXISTS("
				"SELECT id FROM Room WHERE Room.id = RoomAvailability.id_room "
					"AND Room.id_school=?"
			")");

const char * const CREATE_TABLE_CLASS =
			("CREATE TABLE IF NOT EXISTS Class("
				"id 					integer primary key autoincrement not null,"
				"name 					text,"
				"short_name 			text,"
				"size 					integer,"
				"free_periods_flag 		integer,"
				"fixed_entry_per_id		integer,"
				"fixed_exit_per_id		integer,"
				"active					integer,"
				"id_school  			integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id),"
				"FOREIGN KEY (fixed_entry_per_id) REFERENCES Period(id),"
				"FOREIGN KEY (fixed_exit_per_id) REFERENCES Period(id),"
				"UNIQUE (name, id_school)"
			")");
const char * const INSERT_TABLE_CLASS =
			("INSERT INTO Class(id, name, short_name, size, free_periods_flag, fixed_entry_per_id, fixed_exit_per_id, active, id_school) VALUES (?,?,?,?,?,?,?,?,?)");
const char * const UPDATE_TABLE_CLASS =
			("UPDATE Class SET (name, short_name, size, free_periods_flag, fixed_entry_per_id, fixed_exit_per_id, active, id_school) = (?,?,?,?,?,?,?,?) WHERE id=?");
const char * const LASTID_TABLE_CLASS =
			("SELECT id FROM Class where rowid = last_insert_rowid()");
const char * const SELECT_CLASS_BY_SCHOOL_ID =
			("SELECT * FROM Class WHERE id_school=? ORDER BY id");
const char * const DELETE_CLASS_BY_ID =
			("DELETE FROM Class WHERE id=?");
const char * const DELETE_CLASS_BY_SCHOOL_ID =
			("DELETE FROM Class WHERE id_school=?");

const char * const CREATE_TABLE_CLASS_ATTENDANCE =
			("CREATE TABLE IF NOT EXISTS ClassAttendance("
				"id 					integer primary key autoincrement not null,"
				"id_class				integer,"
				"id_period				integer,"
				"score      			integer,"
				"FOREIGN KEY (id_class) REFERENCES Class(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id),"
				"UNIQUE (id_class, id_period)"
			")");
const char * const UPSERT_TABLE_CLASS_ATTENDANCE =
			("INSERT INTO ClassAttendance(id_class, id_period, score) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_class, id_period) DO UPDATE SET (score) = (?3)");
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
				"id_sup					integer,"
				"id_sub					integer,"
				"FOREIGN KEY (id_sub) REFERENCES Class(id),"
				"FOREIGN KEY (id_sup) REFERENCES Class(id),"
				"UNIQUE (id_sup, id_sub)"
			")");
const char * const INSERT_TABLE_CLASS_SUBORDINATION =
			("INSERT INTO ClassSubordination(id_sup, id_sub) VALUES(?,?)");
/* NOTE: there is no reason to update this table. Only delete and add accordingly */
const char * const LASTID_TABLE_CLASS_SUBORDINATION =
			("SELECT id FROM ClassSubordination where rowid = last_insert_rowid()");
const char * const SELECT_CLASS_SUBORDINATION_BY_SUP_ID =
			("SELECT * FROM ClassSubordination WHERE id_sup=?");
const char * const DELETE_CLASS_SUBORDINATION_BY_SUP_ID =
			("DELETE FROM ClassSubordination WHERE id_sup = ?");
const char * const DELETE_CLASS_SUBORDINATION_BY_SUB_ID =
			("DELETE FROM ClassSubordination WHERE id_sub = ?");
const char * const DELETE_CLASS_SUBORDINATION_BY_SCHOOL_ID =
			/* Deleting by supid is enough, and equivalent to by subid */
			("DELETE FROM ClassSubordination WHERE EXISTS ("
				"SELECT id FROM Class WHERE Class.id=ClassSubordination.id_sup "
					"AND Class.id_school = ?"
			")");

const char * const CREATE_TABLE_CLASS_ROOM =
			("CREATE TABLE IF NOT EXISTS ClassRoom("
				"id						integer primary key autoincrement not null,"
				"id_class				integer,"
				"id_room				integer,"
				"score					integer,"
				"FOREIGN KEY (id_class) REFERENCES Class(id),"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"UNIQUE (id_class, id_room)"
			")");
const char * const UPSERT_TABLE_CLASS_ROOM =
			("INSERT INTO ClassRoom(id_class, id_room, score) VALUES (?1,?2,?3) "
			 " ON CONFLICT (id_class, id_room) DO UPDATE SET score=?3");
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
				"FOREIGN KEY (id_school) REFERENCES School(id),"
				"UNIQUE (name, id_school)"
			")");
const char * const INSERT_TABLE_SUBJECT =
			("INSERT INTO Subject(id, name, short_name, id_school) VALUES (?,?,?,?)");
const char * const UPDATE_TABLE_SUBJECT =
			("UPDATE Subject SET (name, short_name, id_school) = (?,?,?) WHERE id=?");
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
				"FOREIGN KEY (id_school) REFERENCES School(id),"
				"UNIQUE (name, id_school)"
			")");
const char * const INSERT_TABLE_SUBJECT_GROUP =
 			("INSERT INTO SubjectGroup(id,name,id_school) VALUES (?,?,?)");
const char * const UPDATE_TABLE_SUBJECT_GROUP =
			("UPDATE SubjectGroup SET (name, id_school) = (?,?) WHERE id=?");
const char * const UPDATE_SUBJECT_GROUP_NAME =
			("UPDATE SubjectGroup SET name = ? WHERE id=?");
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
				"FOREIGN KEY (id_group)   REFERENCES SubjectGroup(id),"
				"UNIQUE (id_subject, id_group)"
			")");
const char * const INSERT_TABLE_SUBJECT_IN_GROUP =
			("INSERT OR IGNORE INTO SubjectInGroup(id_subject, id_group) VALUES (?,?)");
/* NOTE: there is no reason to update this table. Only delete and add accordingly */
const char * const LASTID_TABLE_SUBJECT_IN_GROUP =
			("SELECT id FROM SubjectInGroup where rowid=last_insert_rowid()");
const char * const SELECT_TABLE_SUBJECT_IN_GROUP_BY_SUBJECT_ID =
			("SELECT * FROM SubjectInGroup WHERE id_subject=?");
const char * const DELETE_SUBJECT_IN_GROUP_BY_SUBJECT_ID_GROUP_ID =
			("DELETE FROM SubjectInGroup WHERE id_subject = ? AND id_group = ? ");
const char * const DELETE_SUBJECT_IN_GROUP_BY_SUBJECT_ID =
			("DELETE FROM SubjectInGroup WHERE id_subject = ?");
const char * const DELETE_SUBJECT_IN_GROUP_BY_GROUP_ID =
			("DELETE FROM SubjectInGroup WHERE id_group = ?");
const char * const DELETE_SUBJECT_IN_GROUP_BY_SCHOOL_ID =
			("DELETE FROM SubjectInGroup WHERE EXISTS("
				"SELECT id FROM Subject WHERE SubjectInGroup.id_subject = Subject.id "
					"AND Subject.id_school=?"
			")");

const char * const CREATE_TABLE_CLASS_SUBJECT =
			("CREATE TABLE IF NOT EXISTS ClassSubject("
				"id 					integer primary key autoincrement not null,"
				"amount					integer,"
				"max_per_per_day		integer,"
				"id_class				integer,"
				"id_subject				integer,"
				"FOREIGN KEY (id_class) REFERENCES Class(id),"
				"FOREIGN KEY (id_subject) REFERENCES Subject(id),"
				"UNIQUE (id_class, id_subject)"
			")");
const char * const UPSERT_TABLE_CLASS_SUBJECT =
			("INSERT INTO ClassSubject(amount, max_per_per_day, id_class, id_subject) VALUES (?1,?2,?3,?4) "
			 "ON CONFLICT (id_class, id_subject) DO UPDATE SET (amount, max_per_per_day, id_class, id_subject) = (?1,?2,?3,?4)");
const char * const LASTID_TABLE_CLASS_SUBJECT =
			("SELECT id FROM ClassSubject where rowid = last_insert_rowid()");
const char * const SELECT_CLASS_SUBJECT_BY_CLASS_ID =
			("SELECT * FROM ClassSubject WHERE id_class = ? ORDER BY id_class");
const char * const SELECT_CLASS_SUBJECT_BY_SCHOOL_ID =
			("SELECT * FROM ClassSubject INNER JOIN Class ON Class.id = ClassSubject.id_class "
				"AND Class.id_school = ? ORDER BY id_class");
const char * const DELETE_CLASS_SUBJECT_BY_CLASS_ID =
			("DELETE FROM ClassSubject WHERE id_class = ?");
const char * const DELETE_CLASS_SUBJECT_BY_SUBJECT_ID =
			("DELETE FROM ClassSubject WHERE id_subject = ?");
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
				"FOREIGN KEY (id_group) REFERENCES SubjectGroup(id),"
				"UNIQUE (id_class, id_group)"
			")");
const char * const UPSERT_TABLE_CLASS_SUBJECT_GROUP =
			("INSERT INTO ClassSubjectGroup(id_class, id_group, max_per_day) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_class, id_group) DO UPDATE SET max_per_day=?3");
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
				"SELECT id FROM Class WHERE Class.id = ClassSubjectGroup.id_class "
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
				"planning_needs_room	integer,"
				"num_per_planning		integer,"
				"active					integer,"
				"id_school				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id),"
				"UNIQUE (name, id_school)"
			")");
const char * const INSERT_TABLE_TEACHER =
			("INSERT INTO Teacher (id, name, short_name, max_days, max_per_day, max_per_class_per_day, max_per, planning_needs_room, num_per_planning, active, id_school) VALUES (?,?,?,?,?,?,?,?,?,?,?)");
const char * const UPDATE_TABLE_TEACHER =
			("UPDATE Teacher SET (name, short_name, max_days, max_per_day, max_per_class_per_day, max_per, planning_needs_room, num_per_planning, active, id_school) = (?,?,?,?,?,?,?,?,?,?) "
			 " WHERE id=?");
const char * const LASTID_TABLE_TEACHER =
			("SELECT id FROM Teacher where rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_BY_SCHOOL_ID =
			("SELECT * FROM Teacher WHERE id_school = ? ORDER BY id");
const char * const DELETE_TEACHER_BY_ID =
			("DELETE FROM Teacher WHERE id=?");
const char * const DELETE_TEACHER_BY_SCHOOL_ID =
			("DELETE FROM Teacher WHERE id_school=?");

const char * const CREATE_TABLE_TEACHER_DAY =
			("CREATE TABLE IF NOT EXISTS TeacherDay("
				"id						integer primary key autoincrement not null,"
				"id_teacher				integer,"
				"id_day					integer,"
				"max_periods			integer,"
				"score					integer,"
				"FOREIGN KEY (id_teacher) references Teacher(id),"
				"FOREIGN KEY (id_day) references Day(id),"
				"UNIQUE (id_teacher, id_day)"
			")");
const char * const UPSERT_TABLE_TEACHER_DAY =
			("INSERT INTO TeacherDay(id_teacher, id_day, max_periods, score) VALUES (?1, ?2, ?3, ?4) "
			 "ON CONFLICT (id_teacher, id_day) DO UPDATE SET (max_periods, score) = (?3,?4)");
const char * const UPDATE_TEACHER_DAY_MAX_PER =
			("UPDATE TeacherDay SET max_periods = ?3 WHERE (id_teacher=?1) AND (id_day=?2)");
const char * const LASTID_TABLE_TEACHER_DAY =
			("SELECT id from TeacherDay where rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_DAY_BY_TEACHER_ID =
			("SELECT * FROM TeacherDay WHERE id_teacher = ?");
const char * const DELETE_TEACHER_DAY_BY_TEACHER_ID =
			("DELETE FROM TeacherDay WHERE id_teacher = ?");
const char * const DELETE_TEACHER_DAY_BY_SCHOOL_ID =
			("DELETE FROM TeacherDay WHERE EXISTS ("
				"SELECT id FROM Teacher WHERE Teacher.id=TeacherDay.id_teacher "
					"AND Teacher.id_school=?"
			")");

const char * const CREATE_TABLE_TEACHES =
			("CREATE TABLE IF NOT EXISTS Teaches("
				"id 					integer primary key autoincrement not null,"
				"score  				integer,"
				"max_per_day			integer,"
				"max_per_class_per_day	integer,"
				"id_teacher				integer,"
				"id_subject				integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_subject) REFERENCES Subject(id),"
				"UNIQUE (id_subject, id_teacher)"
			")");
const char * const UPSERT_TABLE_TEACHES =
			("INSERT INTO Teaches(score, max_per_day, max_per_class_per_day, id_teacher, id_subject) VALUES (?1,?2,?3,?4,?5) "
			 "ON CONFLICT (id_subject, id_teacher) DO UPDATE SET (score, max_per_day, max_per_class_per_day)=(?1,?2,?3)");
const char * const LASTID_TABLE_TEACHES =
			("SELECT id FROM Teaches where rowid = last_insert_rowid()");
const char * const SELECT_TEACHES_BY_SCHOOL_ID =
			("SELECT * FROM Teaches INNER JOIN"
			 	" Teacher ON Teacher.id = Teaches.id_teacher"
				" WHERE Teacher.id_school=? ORDER BY id_teacher"
			);
const char * const DELETE_TEACHES_BY_SUBJECT_ID =
			("DELETE FROM Teaches WHERE id_subject = ?");
const char * const DELETE_TEACHES_BY_ID =
			("DELETE FROM Teaches WHERE id = ?");
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
				"id_sup					integer,"
				"id_sub					integer,"
				"FOREIGN KEY (id_sub) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_sup) REFERENCES Teacher(id),"
				"UNIQUE (id_sub, id_sup)"
			")");
const char * const INSERT_TABLE_TEACHER_SUBORDINATION =
			("INSERT INTO TeacherSubordination(id_sup, id_sub) VALUES (?,?)");
/* NOTE: there is no reason to update this table. Only delete and add accordingly */
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
				"SELECT id FROM Teacher WHERE Teacher.id = TeacherSubordination.id_sup "
					"AND Teacher.id_school=? "
			")");

const char * const CREATE_TABLE_TEACHER_ATTENDANCE =
			("CREATE TABLE IF NOT EXISTS TeacherAttendance("
				"id						integer primary key autoincrement not null,"
				"id_teacher				integer,"
				"id_period				integer,"
				"score_lecture			integer,"
				"score_planning			integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id),"
				"UNIQUE (id_teacher, id_period)"
			")");
const char * const UPSERT_TABLE_TEACHER_ATTENDANCE =
			("INSERT INTO TeacherAttendance(id_teacher, id_period, score_lecture, score_planning) VALUES (?1,?2,?3,?4) "
			 "ON CONFLICT (id_teacher, id_period) DO UPDATE SET (score_lecture, score_planning) = (?3,?4)");
const char * const LASTID_TEACHER_ATTENDANCE =
			("SELECT id FROM TeacherAttendance where rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_ATTENDANCE_BY_TEACHER_ID =
			("SELECT * FROM TeacherAttendance WHERE id_teacher=?");
const char * const DELETE_TEACHER_ATTENDANCE_BY_TEACHER_ID =
			("DELETE FROM TeacherAttendance WHERE id_teacher = ?");
const char * const DELETE_TEACHER_ATTENDANCE_BY_SCHOOL_ID =
			("DELETE FROM TeacherAttendance WHERE EXISTS ("
				"SELECT id FROM Teacher WHERE Teacher.id = TeacherAttendance.id_teacher "
					"AND Teacher.id_school = ?"
			")");

const char * const CREATE_TABLE_TEACHER_TWIN_PREFERENCE =
			("CREATE TABLE IF NOT EXISTS TeacherTwinPreference("
				"id						integer primary key autoincrement not null,"
				"twin_val				integer,"
				"score_planning			integer,"
				"id_teacher				integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"UNIQUE (id_teacher, twin_val)"
			")");
const char * const UPSERT_TABLE_TEACHER_TWIN_PREFERENCE =
			("INSERT INTO TeacherTwinPreference(twin_val, score_planning, id_teacher) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_teacher, twin_val) DO UPDATE SET score_planning=?2");
const char * const LASTID_TEACHER_TWIN_PREFERENCE =
			("SELECT id FROM TeacherTwinPreference WHERE rowid = last_insert_rowid()");
const char * const SELECT_TEACHER_TWIN_PREFERENCE_BY_TEACHER_ID =
			("SELECT * FROM TeacherTwinPreference WHERE id_teacher=?");
const char * const DELETE_TEACHER_TWIN_PREFERENCE_BY_TEACHER_ID =
			("DELETE FROM TeacherTwinPreference WHERE id_teacher=?");
const char * const DELETE_TEACHER_TWIN_PREFERENCE_BY_SCHOOL_ID =
			("DELETE FROM TeacherTwinPreference WHERE EXISTS ("
				"SELECT id FROM Teacher WHERE Teacher.id = TeacherTwinPreference.id_teacher "
					"AND Teacher.id_school=?"
			")");

const char * const CREATE_TABLE_TEACHER_ROOM =
			("CREATE TABLE IF NOT EXISTS TeacherRoom("
				"id						integer primary key autoincrement not null,"
				"id_teacher				integer,"
				"id_room				integer,"
				"score_lecture			integer,"
				"score_planning			integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"UNIQUE (id_teacher, id_room)"
			")");
const char * const UPSERT_TABLE_TEACHER_ROOM =
			("INSERT INTO TeacherRoom(id_teacher, id_room, score_lecture, score_planning) VALUES (?1,?2,?3,?4) "
			 "ON CONFLICT (id_teacher, id_room) DO UPDATE SET (score_lecture, score_planning)=(?3,?4)");
const char * const UPSERT_TEACHER_SCORE_LECTURE =
			("INSERT INTO TeacherRoom(id_teacher, id_room, score_lecture) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_teacher, id_room) DO UPDATE SET (score_lecture)=(?3)");
const char * const LASTID_TABLE_TEACHER_ROOM =
			("SELECT id FROM TeacherRoom WHERE rowid = last_insert_rowid()");
const char * const SELECT_TABLE_TEACHER_ROOM_BY_TEACHER_ID =
			("SELECT * from TeacherRoom WHERE id_teacher = ?");
const char * const DELETE_TEACHER_ROOM_BY_TEACHER_ID =
			("DELETE FROM TeacherRoom WHERE id_teacher=?");
const char * const DELETE_TEACHER_ROOM_BY_ROOM_ID =
			("DELETE FROM TeacherRoom WHERE id_room=?");
const char * const DELETE_TEACHER_ROOM_BY_SCHOOL_ID =
			("DELETE FROM TeacherRoom WHERE EXISTS("
				"SELECT id FROM Teacher WHERE Teacher.id = TeacherRoom.id_teacher "
					"AND Teacher.id_school=?"
			")");

const char * const CREATE_TABLE_TEACHES_ROOM =
			("CREATE TABLE IF NOT EXISTS TeachesRoom("
				"id						integer primary key autoincrement not null,"
				"id_teaches				integer,"
				"id_room				integer,"
				"score					integer,"
				"FOREIGN KEY (id_teaches) REFERENCES Teaches(id),"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"UNIQUE (id_teaches, id_room)"
			")");
const char * const UPSERT_TABLE_TEACHES_ROOM =
			("INSERT INTO TeachesRoom(id_teaches, id_room, score) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_teaches, id_room) DO UPDATE SET score = ?3");
const char * const INSERT_TABLE_TEACHES_ROOM =
			("INSERT INTO TeachesRoom(id_teaches, id_room, score) VALUES (?,?,?)");
const char * const UPDATE_TABLE_TEACHES_ROOM =
			("UPDATE TeachesRoom SET (id_teaches, id_room, score) = (?1,?2,?3) WHERE id_teaches=?1 AND id_room=?2");
const char * const LASTID_TABLE_TEACHES_ROOM =
			("SELECT id FROM TeachesRoom WHERE rowid = last_insert_rowid()");
const char * const SELECT_TABLE_TEACHES_ROOM_BY_TEACHES_ID =
			("SELECT * FROM TeachesRoom WHERE id_teaches=?");
const char * const DELETE_TEACHES_ROOM_BY_ROOM_ID =
			("DELETE FROM TeachesRoom WHERE id_room=?");
const char * const DELETE_TEACHES_ROOM_BY_TEACHER_ID =
			("DELETE FROM TeachesRoom WHERE EXISTS ("
				"SELECT id FROM Teaches WHERE Teaches.id = TeachesRoom.id_teaches "
					"AND Teaches.id_teacher=?"
			")");
const char * const DELETE_TEACHES_ROOM_BY_TEACHES_ID =
			("DELETE FROM TeachesRoom WHERE id_room=?");
const char * const DELETE_TEACHES_ROOM_BY_SUBJECT_ID =
			("DELETE FROM TeachesRoom WHERE EXISTS("
				"SELECT id FROM Teaches WHERE Teaches.id = TeachesRoom.id_teaches "
					"AND Teaches.id_subject=?"
			")");
const char * const DELETE_TEACHES_ROOM_BY_SCHOOL_ID =
			("DELETE FROM TeachesRoom WHERE EXISTS("
				"SELECT id FROM Room WHERE Room.id = TeachesRoom.id_room "
					"AND Room.id_school=?"
			")");

const char * const CREATE_TABLE_TEACHES_PERIOD =
			("CREATE TABLE IF NOT EXISTS TeachesPeriod("
				"id 					integer primary key autoincrement not null,"
				"id_teaches				integer,"
				"id_period				integer,"
				"score					integer,"
				"FOREIGN KEY (id_teaches) REFERENCES Teaches(id),"
				"FOREIGN KEY (id_period)  REFERENCES Period(id),"
				"UNIQUE (id_teaches, id_period)"
			")");
const char * const UPSERT_TABLE_TEACHES_PERIOD =
			("INSERT INTO TeachesPeriod(id_teaches, id_period, score) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_teaches, id_period) DO UPDATE SET (score) = (?3)");
const char * const LASTID_TEACHES_PERIOD =
			("SELECT id FROM TeachesPeriod where rowid = last_insert_rowid()");
const char * const SELECT_TEACHES_PERIOD_BY_TEACHES_ID =
			("SELECT * FROM TeachesPeriod WHERE id_teaches=?");
const char * const DELETE_TEACHES_PERIOD_BY_TEACHES_ID =
			("DELETE FROM TeachesPeriod WHERE id_teaches=?");
const char * const DELETE_TEACHES_PERIOD_BY_SUBJECT_ID =
			("DELETE FROM TeachesPeriod WHERE EXISTS ("
				"SELECT id from Teaches WHERE Teaches.id = TeachesPeriod.id_teaches "
					"AND Teaches.id_subject = ?"
			")");
const char * const DELETE_TEACHES_PERIOD_BY_TEACHER_ID =
			("DELETE FROM TeachesPeriod WHERE EXISTS ("
				"SELECT id FROM Teaches WHERE Teaches.id = TeachesPeriod.id_teaches "
					"AND Teaches.id_teacher = ?"
			")");
const char * const DELETE_TEACHES_PERIOD_BY_SCHOOL_ID =
			("DELETE FROM TeachesPeriod WHERE EXISTS ("
				"SELECT id FROM Period WHERE Period.id = TeachesPeriod.id_period "
					"AND Period.id_school = ?"
			")");

const char * const CREATE_TABLE_TEACHES_TWIN_PREFERENCE =
			("CREATE TABLE IF NOT EXISTS TeachesTwinPreference("
				"id 					integer primary key autoincrement not null,"
				"twin_val				integer,"
				"score					integer,"
				"id_teaches				integer,"
				"FOREIGN KEY (id_teaches) REFERENCES Teaches(id),"
				"UNIQUE (id_teaches, twin_val)"
			")");
const char * const UPSERT_TABLE_TEACHES_TWIN_PREFERENCE =
			("INSERT INTO TeachesTwinPreference (twin_val, score, id_teaches) values (?1,?2,?3) "
			 "ON CONFLICT (id_teaches, twin_val) DO UPDATE SET score=?2");
const char * const LASTID_TEACHES_TWIN_PREFERENCE =
			("SELECT id FROM TeachesTwinPreference where rowid = last_insert_rowid()");
const char * const SELECT_TEACHES_TWIN_PREFERENCE_BY_TEACHES_ID =
			("SELECT * FROM TeachesTwinPreference WHERE id_teaches=?");
const char * const DELETE_TEACHES_TWIN_PREFERENCE_BY_TEACHES_ID =
			("DELETE FROM TeachesTwinPreference WHERE id_teaches=?");
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
				"id						integer primary key autoincrement not null,"
				"gen_date				text," /* Format to be defined */
				"name					text,"
				"desc					text,"
				"id_school				integer,"
				"FOREIGN KEY (id_school) REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_SOLUTION =
			("INSERT INTO Solution(id, gen_date, name, desc, id_school) VALUES (?, ?,?,?,?)");
const char * const UPDATE_TABLE_SOLUTION =
			("UPDATE Solution SET (gen_date, name, desc, id_school) = (?,?,?,?) WHERE id = ?");
const char * const LASTID_TABLE_SOLUTION =
			("SELECT id FROM Solution WHERE rowid = last_insert_rowid()");
const char * const SELECT_SOLUTION_BY_SCHOOL_ID =
			("SELECT * FROM SOLUTION WHERE id_school=? ORDER BY id");
const char * const DELETE_SOLUTION_BY_ID =
			("DELETE FROM SOLUTION WHERE id=?");
const char * const DELETE_SOLUTION_BY_SCHOOL_ID =
			("DELETE FROM SOLUTION WHERE id_school=?");

const char * const CREATE_TABLE_LECTURE =
			("CREATE TABLE IF NOT EXISTS Lecture("
				"id						integer primary key autoincrement not null,"
				"id_classsubject		integer,"
				"id_school				integer,"
				"FOREIGN KEY (id_classsubject) REFERENCES ClassSubject(id),"
				"FOREIGN KEY (id_school)  REFERENCES School(id)"
			")");
const char * const INSERT_TABLE_LECTURE =
			("INSERT INTO Lecture(id_classsubject, id_school) VALUES (?,?)");
/* NOTE: there is no reason to update this table. Only delete and add accordingly */
const char * const LASTID_TABLE_LECTURE =
			("SELECT id FROM Lecture where rowid = last_insert_rowid()");
const char * const SELECT_LECTURE_BY_SCHOOL_ID =
			("SELECT * FROM Lecture WHERE id_school=?");
const char * const DELETE_LECTURE_BY_SUBJECT_ID =
			("DELETE FROM Lecture WHERE EXISTS ("
				"SELECT id FROM ClassSubject WHERE ClassSubject.id = Lecture.id_classsubject "
					"AND ClassSubject.id_subject=?"
			")");
const char * const DELETE_LECTURE_BY_CLASS_ID =
			("DELETE FROM Lecture WHERE EXISTS ("
				"SELECT id FROM ClassSubject WHERE ClassSubject.id = Lecture.id_classsubject "
					"AND ClassSubject.id_class=?"
			")");
const char * const DELETE_LECTURE_BY_SCHOOL_ID =
			("DELETE FROM Lecture WHERE id_school = ?");

const char * const CREATE_TABLE_POSSIBLE_TEACHER =
			("CREATE TABLE IF NOT EXISTS PossibleTeacher("
				"id integer primary key autoincrement not null,"
				"id_classsubject integer,"
				"id_teacher integer,"
				"score integer,"
				"FOREIGN KEY (id_classsubject) REFERENCES ClassSubject(id),"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id),"
				"UNIQUE (id_classsubject, id_teacher)"
			")");
const char * const UPSERT_TABLE_POSSIBLE_TEACHER =
			("INSERT INTO PossibleTeacher(id_classsubject, id_teacher, score) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_classsubject, id_teacher) DO UPDATE SET score=?3");
const char * const LASTID_TABLE_POSSIBLE_TEACHER =
			("SELECT id FROM PossibleTeacher WHERE rowid=last_insert_rowid()");
const char * const SELECT_TABLE_POSSIBLE_TEACHER_BY_CLASS_SUBJECT_ID =
			("SELECT * FROM PossibleTeacher WHERE id_classsubject=?");
const char * const DELETE_POSSIBLE_TEACHER_BY_CLASS_ID =
			("DELETE FROM PossibleTeacher WHERE EXISTS ("
				"SELECT id FROM ClassSubject WHERE ClassSubject.id = PossibleTeacher.id_classsubject "
					"AND ClassSubject.id_class=?"
			")");
const char * const DELETE_POSSIBLE_TEACHER_BY_SUBJECT_ID =
			("DELETE FROM PossibleTeacher WHERE EXISTS("
				"SELECT id FROM ClassSubject WHERE ClassSubject.id = PossibleTeacher.id_classsubject "
					"AND ClassSubject.id_subject = ?"
			")");
const char * const DELETE_POSSIBLE_TEACHER_BY_TEACHER_ID =
			("DELETE FROM PossibleTeacher WHERE id_teacher=?");
const char * const DELETE_POSSIBLE_TEACHER_BY_SCHOOL_ID =
			("DELETE FROM PossibleTeacher WHERE EXISTS("
				"SELECT id FROM Teacher WHERE Teacher.id = PossibleTeacher.id_teacher "
					"AND Teacher.id_school=?"
			")");

const char * const CREATE_TABLE_LECTURE_POSSIBLE_ROOM =
			("CREATE TABLE IF NOT EXISTS LecturePossibleRoom("
				"id integer primary key autoincrement not null,"
				"id_lecture integer,"
				"id_room integer,"
				"score integer,"
				"FOREIGN KEY (id_lecture) REFERENCES Lecture(id),"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"UNIQUE (id_lecture, id_room)"
			")");
const char * const UPSERT_TABLE_LECTURE_POSSIBLE_ROOM =
			("INSERT INTO LecturePossibleRoom(id_lecture, id_room, score) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_lecture, id_room) DO UPDATE SET score=?3");
const char * const LASTID_TABLE_LECTURE_POSSIBLE_ROOM =
			("SELECT id FROM LecturePossibleRoom WHERE rowid = last_insert_rowid()");
const char * const SELECT_TABLE_LECTURE_POSSIBLE_ROOM_BY_LECTURE_ID =
			("SELECT * FROM LecturePossibleRoom WHERE id_lecture = ?");
const char * const DELETE_LECTURE_POSSIBLE_ROOM_BY_LECTURE_ID =
			("DELETE FROM LecturePossibleRoom WHERE id_lecture = ?");
const char * const DELETE_LECTURE_POSSIBLE_ROOM_BY_ROOM_ID =
			("DELETE FROM LecturePossibleRoom WHERE id_room = ?");
const char * const DELETE_LECTURE_POSSIBLE_ROOM_BY_CLASS_ID =
			("DELETE FROM LecturePossibleRoom WHERE EXISTS ("
				"SELECT id FROM Lecture WHERE Lecture.id = LecturePossibleRoom.id_lecture "
					"AND EXISTS("
						"SELECT id FROM ClassSubject WHERE ClassSubject.id = Lecture.id_classsubject "
							"AND ClassSubject.id_class=?"
					")"
			")");
const char * const DELETE_LECTURE_POSSIBLE_ROOM_BY_SUBJECT_ID =
			("DELETE FROM LecturePossibleRoom WHERE EXISTS ("
				"SELECT id FROM Lecture WHERE Lecture.id = LecturePossibleRoom.id_lecture "
					"AND EXISTS ("
						"SELECT id FROM ClassSubject WHERE ClassSubject.id = Lecture.id_classsubject "
							"AND ClassSubject.id_subject=?"
					")"
			")");
const char * const DELETE_LECTURE_POSSIBLE_ROOM_BY_SCHOOL_ID =
			("DELETE FROM LecturePossibleRoom WHERE EXISTS("
				"SELECT id FROM Lecture WHERE Lecture.id = LecturePossibleRoom.id_lecture "
					"AND Lecture.id_school=?"
			")");

const char * const CREATE_TABLE_LECTURE_POSSIBLE_PERIOD =
			("CREATE TABLE IF NOT EXISTS LecturePossiblePeriod("
				"id integer primary key autoincrement not null,"
				"id_lecture integer,"
				"id_period integer,"
				"score integer,"
				"FOREIGN KEY (id_lecture) REFERENCES Lecture(id),"
				"FOREIGN KEY (id_period)  REFERENCES Period(id),"
				"UNIQUE (id_lecture, id_period)"
			")");
const char * const UPSERT_TABLE_LECTURE_POSSIBLE_PERIOD =
			("INSERT INTO LecturePossiblePeriod(id_lecture, id_period, score) VALUES (?1,?2,?3) "
		     "ON CONFLICT (id_lecture, id_period) DO UPDATE SET score=?3");
const char * const LASTID_TABLE_LECTURE_POSSIBLE_PERIOD =
			("SELECT id FROM LecturePossiblePeriod WHERE rowid = last_insert_rowid()");
const char * const SELECT_TABLE_LECTURE_POSSIBLE_PERIOD_BY_LECTURE_ID =
			("SELECT * FROM LecturePossiblePeriod WHERE id_lecture = ?");
const char * const DELETE_LECTURE_POSSIBLE_PERIOD_BY_LECTURE_ID =
			("DELETE FROM LecturePossiblePeriod WHERE id_lecture = ?");
const char * const DELETE_LECTURE_POSSIBLE_PERIOD_BY_CLASS_ID =
			("DELETE FROM LecturePossiblePeriod WHERE EXISTS ("
				"SELECT id FROM Lecture WHERE Lecture.id = id_lecture "
					"AND EXISTS ("
						"SELECT id FROM ClassSubject WHERE ClassSubject.id = Lecture.id_classsubject "
							"AND ClassSubject.id_class = ?"
					")"
			")");
const char * const DELETE_LECTURE_POSSIBLE_PERIOD_BY_SUBJECT_ID =
			("DELETE FROM LecturePossiblePeriod WHERE EXISTS ("
				"SELECT id FROM Lecture WHERE Lecture.id = LecturePossiblePeriod.id_lecture "
					"AND EXISTS ("
						"SELECT id FROM ClassSubject WHERE ClassSubject.id = Lecture.id_classsubject "
							"AND Classsubject.id_subject = ?"
					")"
			")");
const char * const DELETE_LECTURE_POSSIBLE_PERIOD_BY_SCHOOL_ID =
			("DELETE FROM LecturePossiblePeriod WHERE EXISTS("
				"SELECT id FROM Lecture WHERE Lecture.id = LecturePossiblePeriod.id_lecture "
					"AND Lecture.id_school=?"
			")");

const char * const CREATE_TABLE_LECTURE_SOLUTION =
			("CREATE TABLE IF NOT EXISTS LectureSolution("
				"id						integer primary key,"
				"id_lecture				integer,"
				"id_solution			integer,"
				"id_period				integer,"
				"id_teacher				integer,"
				"id_room				integer,"
				"id_class				integer,"
				"id_subject				integer,"
				"FOREIGN KEY (id_lecture) 	REFERENCES Lecture(id),"
				"FOREIGN KEY (id_solution) 	REFERENCES Solution(id),"
				"FOREIGN KEY (id_period) 	REFERENCES Period(id),"
				"FOREIGN KEY (id_teacher) 	REFERENCES Teacher(id),"
				"FOREIGN KEY (id_room) 		REFERENCES Room(id),"
				"FOREIGN KEY (id_class) 	REFERENCES Class(id),"
				"FOREIGN KEY (id_subject)	REFERENCES Subject(id),"
				"UNIQUE (id_lecture, id_solution)"
			")");
const char * const UPSERT_TABLE_LECTURE_SOLUTION = /* And are we really going to use the update part? */
			("INSERT INTO LectureSolution(id_lecture, id_solution, id_period, id_teacher, id_room, id_class, id_subject) VALUES (?1,?2,?3,?4,?5,?6,?7) "
			 "ON CONFLICT (id_lecture, id_solution) DO UPDATE SET (id_period, id_teacher, id_room) = (?3,?4,?5)");
const char * const LASTID_TABLE_LECTURE_SOLUTION =
			("SELECT id FROM LectureSolution where rowid = last_insert_rowid()");
const char * const SELECT_LECTURE_SOLUTION_BY_SOLUTION_ID =
			("SELECT * FROM LectureSolution WHERE id_solution=?");
const char * const UNSET_LECTURE_SOLUTION_ROOM_BY_ROOM_ID =
			("UPDATE LectureSolution SET id_room=null where id_room=?");
const char * const UNSET_LECTURE_TEACHER_BY_TEACHER_ID =
			("UPDATE LectureSolution SET id_teacher=null WHERE id_teacher = ?");
const char * const DELETE_LECTURE_SOLUTION_BY_SUBJECT_ID =
			("DELETE FROM LectureSolution WHERE EXISTS ("
				"SELECT id FROM Lecture WHERE Lecture.id = LectureSolution.id_lecture "
					"AND EXISTS ("
						"SELECT id FROM ClassSubject WHERE ClassSubject.id = Lecture.id_classsubject "
							"AND ClassSubject.id_subject=?"
					")"
			")");
const char * const DELETE_LECTURE_SOLUTION_BY_SOLUTION_ID =
			("DELETE FROM LectureSolution WHERE id_solution = ?");
const char * const DELETE_LECTURE_SOLUTION_BY_CLASS_ID =
			("DELETE FROM LectureSolution WHERE EXISTS("
				"SELECT id FROM Lecture WHERE Lecture.id = LectureSolution.id_lecture "
					"AND EXISTS("
						"SELECT id FROM ClassSubject WHERE ClassSubject.id = Lecture.id_classsubject "
							"AND ClassSubject.id_class=?"
					")"
			")");
const char * const DELETE_LECTURE_SOLUTION_BY_SCHOOL_ID =
			("DELETE FROM LectureSolution WHERE EXISTS("
				"SELECT id_school FROM Lecture WHERE Lecture.id = LectureSolution.id "
					"AND Lecture.id_school=?"
			")");

const char * const CREATE_TABLE_PLANNING =
			("CREATE TABLE IF NOT EXISTS Planning("
				"id						integer primary key autoincrement not null,"
				"id_teacher				integer,"
				"FOREIGN KEY (id_teacher) REFERENCES Teacher(id)"
			")");
const char * const INSERT_TABLE_PLANNING =
			("INSERT INTO Planning (id_teacher) VALUES (?)");
const char * const LASTID_TABLE_PLANNING =
			("SELECT id FROM Planning WHERE rowid = last_insert_rowid()");
/* NOTE: there is no reason to update this table. Only delete and add accordingly */
const char * const SELECT_PLANNING_BY_TEACHER_ID =
			("SELECT * FROM Planning WHERE id_teacher = ?");
const char * const DELETE_PLANNING_BY_TEACHER_ID =
			("DELETE FROM Planning WHERE id_teacher = ?");
const char * const DELETE_PLANNING_BY_SCHOOL_ID =
			("DELETE FROM Planning WHERE EXISTS  ("
				"SELECT id FROM Teacher WHERE Teacher.id = Planning.id_teacher "
					"AND Teacher.id_school=?"
			")");

const char * const CREATE_TABLE_PLANNING_PERIOD =
			("CREATE TABLE IF NOT EXISTS PlanningPeriod("
				"id						integer primary key autoincrement not null,"
				"id_planning			integer,"
				"id_period				integer,"
				"score					integer,"
				"FOREIGN KEY (id_planning) REFERENCES Planning(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id),"
				"UNIQUE (id_planning, id_period)"
			")");
const char * const UPSERT_TABLE_PLANNING_PERIOD =
			("INSERT INTO PlanningPeriod(id_planning, id_period, score) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_planning, id_period) DO UPDATE SET score=?3");
const char * const LASTID_TABLE_PLANNING_PERIOD =
			("SELECT id FROM PlanningPeriod WHERE rowid = last_insert_rowid()");
const char * const SELECT_TABLE_PLANNING_PERIOD =
			("SELECT * FROM PlanningPeriod");
const char * const DELETE_PLANNING_PERIOD_BY_PLANNING_ID =
			("DELETE FROM PlanningPeriod WHERE id_planning = ?");
const char * const DELETE_PLANNING_PERIOD_BY_TEACHER_ID =
			("DELETE FROM PlanningPeriod WHERE EXISTS("
				"SELECT id FROM Planning WHERE PlanningPeriod.id_planning = Planning.id "
					"AND Planning.id_teacher = ?"
			")");
const char * const DELETE_PLANNING_PERIOD_BY_SCHOOL_ID =
			("DELETE FROM PlanningPeriod WHERE EXISTS("
				"SELECT id FROM Period WHERE PlanningPeriod.id_period = Period.id "
					"AND Period.id_school = ?"
			")");

const char * const CREATE_TABLE_PLANNING_ROOM =
			("CREATE TABLE IF NOT EXISTS PlanningRoom("
				"id 					integer primary key autoincrement not null,"
				"id_planning			integer,"
				"id_room				integer,"
				"score					integer,"
				"FOREIGN KEY (id_planning) REFERENCES Planning(id),"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"UNIQUE (id_planning, id_room)"
			")");
const char * const UPSERT_TABLE_PLANNING_ROOM =
			("INSERT INTO PlanningRoom(id_planning, id_room, score) VALUES (?1,?2,?3) "
			 "ON CONFLICT (id_planning, id_room) DO UPDATE SET score=?3");
const char * const LASTID_TABLE_PLANNING_ROOM =
			("SELECT id FROM PlanningRoom WHERE rowid = last_insert_rowid()");
const char * const SELECT_PLANNING_ROOM_BY_PLANNING_ID =
			("SELECT * FROM PlanningRoom WHERE id_planning=?");
const char * const DELETE_PLANNING_ROOM_BY_PLANNING_ID =
			("DELETE FROM PlanningRoom WHERE id_planning=?");
const char * const DELETE_PLANNING_ROOM_BY_TEACHER_ID =
			("DELETE FROM PlanningRoom WHERE EXISTS ("
				"SELECT id FROM Planning WHERE Planning.id = PlanningRoom.id_planning "
					"AND Planning.id_teacher = ?"
			")");
const char * const DELETE_PLANNING_ROOM_BY_ROOM_ID =
			("DELETE FROM PlanningRoom WHERE id_room=?");
const char * const DELETE_PLANNING_ROOM_BY_SCHOOL_ID =
			("DELETE FROM PlanningRoom WHERE EXISTS ("
				"SELECT id FROM Room WHERE Room.id = PlanningRoom.id_room "
					"AND Room.id_school=?"
			")");

const char * const CREATE_TABLE_PLANNING_SOLUTION =
			("CREATE TABLE IF NOT EXISTS PlanningSolution("
				"id							integer primary key not null,"
				"id_planning				integer,"
				"id_solution				integer,"
				"id_period					integer,"
				"id_room					integer,"
				"FOREIGN KEY (id_planning) REFERENCES Planning(id),"
				"FOREIGN KEY (id_solution) REFERENCES Solution(id),"
				"FOREIGN KEY (id_period) REFERENCES Period(id),"
				"FOREIGN KEY (id_room) REFERENCES Room(id),"
				"UNIQUE (id_planning, id_solution)"
			")");
const char * const UPSERT_TABLE_PLANNING_SOLUTION =
			("INSERT INTO PlanningSolution(id_planning, id_solution, id_period, id_room) VALUES (?1,?2,?3,?4) "
			 "ON CONFLICT (id_planning, id_solution) DO UPDATE SET (id_period, id_room) = (?3,?4)");
const char * const LASTID_TABLE_PLANNING_SOLUTION =
			("SELECT id FROM PlanningSolution WHERE rowid = last_insert_rowid()");
const char * const SELECT_PLANNING_SOLUTION_BY_SOLUTION_ID =
			("SELECT * FROM PlanningSolution WHERE id_solution = ?");
const char * const DELETE_PLANNING_SOLUTION_BY_SOLUTION_ID =
			("DELETE FROM PlanningSolution WHERE id_solution = ?");
const char * const DELETE_PLANNING_SOLUTION_BY_TEACHER_ID =
			("DELETE FROM PlanningSolution WHERE EXISTS("
				"SELECT id FROM Planning WHERE Planning.id = PlanningSolution.id_planning "
					"AND Planning.id_teacher=?"
			")");
const char * const UNSET_PLANNING_SOLUTION_ROOM_ID =
			("UPDATE PlanningSolution SET id_room=null WHERE id_room=?");
const char * const DELETE_PLANNING_SOLUTION_BY_SCHOOL_ID =
			("DELETE FROM PlanningSolution WHERE EXISTS ("
				"SELECT id FROM Room WHERE Room.id = PlanningSolution.id_room "
					"AND Room.id_school=?"
			")");
/*
 * Table                 | Insert | Select | Update | Delete
 * ----------------------|--------|--------|--------|----------------
 * School                |        |        |        |
 * DailyPeriod           |        |        |        |
 * Day                   |        |        |        |
 * Period                |        |        |        |
 * Room                  |        |        |        |
 * RoomAvailability      |        |        |        |
 * Class                 |        |        |        |
 * ClassAttendance       |        |        |        |
 * ClassSubordination    |        |        |        |
 * ClassRoom             |        |        |        |
 * Subject               |        |        |        |
 * SubjectGroup          |        |        |        |
 * SubjectInGroup        |        |        |        |
 * ClassSubject          |        |        |        |
 * ClassSubjectGroup     |        |        |        |
 * Teacher               |        |        |        |
 * TeacherDay            |        |        |        |
 * Teaches               |        |        |        |
 * TeacherSubordination  |        |        |        |
 * TeacherAttendance     |        |        |        |
 * TeacherTwinPreference |        |        |        |
 * TeacherRoom           |        |        |        |
 * TeachesRoom           |        |        |        |
 * TeachesPeriod         |        |        |        |
 * TeachesTwinPreference |        |        |        |
 * Solution              |        |        |        |
 * Lecture               |        |        |        |
 * PossibleTeacher       |        |        |        |
 * LecturePossibleRoom   |        |        |        |
 * LecturePossiblePeriod |        |        |        |
 * LectureSolution       |        |        |        |
 * Planning              |        |        |        |
 * PlanningPeriod        |        |        |        |
 * PlanningRoom          |        |        |        |
 * PlanningSolution      |        |        |        |
 * ----------------------|--------|--------|--------|----------------
 */

/*********************************************************/
/*                   UTILITY FUNCTIONS                   */
/*********************************************************/

static bool exec_and_check(sqlite3 * db, const char * const sql, int id){
	sqlite3_stmt * stmt;
	int errc = 0;
	bool retc = true;

	errc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		sqlite3_bind_int(stmt,1,id);
		errc = sqlite3_step(stmt);
		if(errc != SQLITE_DONE){
			printf("Could not execute SQL %s. %d %s.\n", sql, errc, sqlite3_errmsg(db));
			retc = false;
		}
		sqlite3_finalize(stmt);
	} else {
		printf("Could not execute SQL %s. %d %s.\n", sql, errc, sqlite3_errmsg(db));
		retc = false;
	}

	return retc;
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


static char * copy_sqlite_string(sqlite3_stmt * stmt, int col_i){
	char * str;
	sqlite3_column_text(stmt,col_i); /* Defines the type to utf8 */
	int sz = (sqlite3_column_bytes(stmt,col_i));
	str = calloc(sz + 1, sizeof(char));
	strncpy(str, (const char *)sqlite3_column_text(stmt,col_i), sz);
	return str;
}

#define CERTIFY_ERRC_SQLITE_OK(ret_val) do{ \
			if(errc != SQLITE_OK){ \
				fprintf(console_out, "SQLite errc wasn't OK at function %s line %d. %d %s\n", __func__, __LINE__, errc, sqlite3_errmsg(db)); \
				return ret_val; \
			} \
		}while(0)

#define CERTIFY_ERRC_SQLITE_ROW_OR_DONE(ret_val) do{ \
			if(errc != SQLITE_ROW && errc != SQLITE_DONE){ \
				fprintf(console_out, "SQLite errc wasn't ROW/DONE at function %s line %d. %d %s\n", __func__, __LINE__, errc, sqlite3_errmsg(db)); \
				return ret_val; \
			} \
		}while(0)

#define CERTIFY_ERRC_SQLITE_DONE(ret_val) do{ \
			if(errc != SQLITE_DONE){ \
				fprintf(console_out, "SQLite errc wasn't ROW/DONE at function %s line %d. %d %s\n", __func__, __LINE__, errc, sqlite3_errmsg(db)); \
				return ret_val; \
			} \
		}while(0)

/*********************************************************/
/*                 CREATE TABLE FUNCTIONS                */
/*********************************************************/

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
	return
	create_table(console_out,db,"School", CREATE_TABLE_SCHOOL)?1:
	create_table(console_out,db,"DailyPeriod", CREATE_TABLE_DAILY_PERIOD)?1:
	create_table(console_out,db,"Day", CREATE_TABLE_DAY)?1:
	create_table(console_out,db,"Period", CREATE_TABLE_PERIOD)?1:
	create_table(console_out,db,"Room", CREATE_TABLE_ROOM)?1:
	create_table(console_out,db,"RoomAvailability", CREATE_TABLE_ROOM_AVAILABILITY)?1:
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
	create_table(console_out,db,"TeacherTwinPreference", CREATE_TABLE_TEACHER_TWIN_PREFERENCE)?1:
	create_table(console_out,db,"TeacherRoom", CREATE_TABLE_TEACHER_ROOM)?1:
	create_table(console_out,db,"TeachesRoom", CREATE_TABLE_TEACHES_ROOM)?1:
	create_table(console_out,db,"TeachesPeriod", CREATE_TABLE_TEACHES_PERIOD)?1:
	create_table(console_out,db,"TeachesTwinPreference", CREATE_TABLE_TEACHES_TWIN_PREFERENCE)?1:
	create_table(console_out,db,"Solution", CREATE_TABLE_SOLUTION)?1:
	create_table(console_out,db,"Lecture", CREATE_TABLE_LECTURE)?1:
	create_table(console_out,db,"PossibleTeacher", CREATE_TABLE_POSSIBLE_TEACHER)?1:
	create_table(console_out,db,"LecturePossibleRoom", CREATE_TABLE_LECTURE_POSSIBLE_ROOM)?1:
	create_table(console_out,db,"LecturePossiblePeriod", CREATE_TABLE_LECTURE_POSSIBLE_PERIOD)?1:
	create_table(console_out,db,"LectureSolution", CREATE_TABLE_LECTURE_SOLUTION)?1:
	create_table(console_out,db,"Planning", CREATE_TABLE_PLANNING)?1:
	create_table(console_out,db,"PlanningPeriod", CREATE_TABLE_PLANNING_PERIOD)?1:
	create_table(console_out,db,"PlanningRoom", CREATE_TABLE_PLANNING_ROOM)?1:
	create_table(console_out,db,"PlanningSolution", CREATE_TABLE_PLANNING_SOLUTION)?1:0;
}

/*********************************************************/
/*                   INSERT  FUNCTIONS                   */
/*********************************************************/

/**
 * General Insert/Update function for tables: ClassAttendance, RoomAvailability, TeachesPeriod, PlanningPeriod and LecturePossiblePeriod.
 * Note that it does not work for TeacherAttendance, because it has two scores.
 * All these tables were deliberately made with the same structure (id, objid, perid, score);
 */
static bool insert_or_update_period_scores(FILE * console_out, sqlite3 * db, const char * const sql, int obj_id, const int * const scores, const School * const school){
	int i = 0, errc;
	sqlite3_stmt * stmt;

	LMH_ASSERT(scores != NULL && db != NULL && sql != NULL);

	errc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	while(i < school->n_periods && scores[i] >= 0){
		sqlite3_bind_int(stmt,1, obj_id);
		sqlite3_bind_int(stmt,2, school->period_ids[i]);
		sqlite3_bind_int(stmt,3, scores[i]);

		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		++i;
		CERTIFY_ERRC_SQLITE_ROW_OR_DONE(false);
	}
	sqlite3_finalize(stmt);
	return true;
}

/**
 * General Insert/Update function for tables: LecturePossibleRoom, ClassRoom, TeachesRoom, and PlanningRoom.
 * Note that it does not work for TeacherRoom, because it has two scores.
 * All these tables were deliberately made with the same structure (id, objid, roomid, score);
 */
static bool insert_or_update_room_scores(FILE * console_out, sqlite3 * db, const char * const sql, int obj_id, const int * const scores, const School * const school){
	int i = 0, errc;
	sqlite3_stmt * stmt;

	LMH_ASSERT(scores != NULL && db != NULL && sql != NULL);

	errc = sqlite3_prepare_v2(db,sql,-1,&stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	while(scores[i] >= 0 && i < school->n_rooms){
		sqlite3_bind_int(stmt,1, obj_id);
		sqlite3_bind_int(stmt,2, school->rooms[i].id);
		sqlite3_bind_int(stmt,3, scores[i]);

		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		++i;
		CERTIFY_ERRC_SQLITE_ROW_OR_DONE(false);
	}
	sqlite3_finalize(stmt);
	return true;
}

/**
 * Insert/Update function for table TeacherRoom.
 */
static bool insert_or_update_teacher_room_scores(FILE * console_out, sqlite3 * db, const Teacher * teacher, const School * const school){
	int i = 0, errc;
	sqlite3_stmt * stmt;

	LMH_ASSERT(teacher != NULL && db != NULL);

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_TEACHER_ROOM,-1,&stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	while(i < school->n_rooms){
		sqlite3_bind_int(stmt,1, teacher->id);
		sqlite3_bind_int(stmt,2, school->rooms[i].id);
		if(teacher->lecture_room_scores != NULL){
			sqlite3_bind_int(stmt,3, teacher->lecture_room_scores[i]);
		} else {
			sqlite3_bind_int(stmt, 3, 0);
		}
		if(teacher->planning_room_scores != NULL){
			sqlite3_bind_int(stmt,4, teacher->planning_room_scores[i]);
		} else {
			sqlite3_bind_int(stmt, 4, 0);
		}
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		++i;
		CERTIFY_ERRC_SQLITE_ROW_OR_DONE(false);
	}
	sqlite3_finalize(stmt);
	return true;
}

/**
 * Insert/Update function for table PossibleTeacher
 */
static bool insert_or_update_teacher_scores(FILE * console_out, sqlite3 * db, const char * const sql, int obj_id, const int * const scores, const School * const school){
	int i = 0, errc;
	sqlite3_stmt * stmt;

	LMH_ASSERT(scores != NULL && db != NULL && sql != NULL);

	errc = sqlite3_prepare_v2(db,sql,-1,&stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	while(scores[i] >= 0 && i < school->n_teachers){
		sqlite3_bind_int(stmt,1, obj_id);
		sqlite3_bind_int(stmt,2, school->teachers[i].id);
		sqlite3_bind_int(stmt,3, scores[i]);

		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		++i;
		CERTIFY_ERRC_SQLITE_ROW_OR_DONE(false);
	}
	sqlite3_finalize(stmt);
	return true;
}

/**
 * General Insert/Delete function for tables: TeacherSubordination, ClassSubordination.
 * These tables were deliberately made with the same structure (id, idsup, idsub);
 */
static bool insert_or_delete_subordination(FILE * console_out, sqlite3 * db, const char * const sql, int id_sup, int id_sub){
	int errc;
	sqlite3_stmt * stmt;

	LMH_ASSERT(db != NULL && sql != NULL);

	errc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_int(stmt,1,id_sup);
	sqlite3_bind_int(stmt,2,id_sub);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(false);
	sqlite3_finalize(stmt);
	return true;
}

/**
 * Insert/Update function for table TeacherAttendance.
 */
static bool insert_or_update_teacher_period_scores(FILE * console_out, sqlite3 * db, Teacher * teacher, School * school){
	int i, errc;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_TEACHER_ATTENDANCE, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(i = 0; i < school->n_periods; ++i){
		sqlite3_bind_int(stmt,1, teacher->id);
		sqlite3_bind_int(stmt,2, school->period_ids[i]);
		sqlite3_bind_int(stmt,3, teacher->lecture_period_scores[i]);
		sqlite3_bind_int(stmt,4, teacher->planning_period_scores[i]);

		errc = sqlite3_step(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
		sqlite3_reset(stmt);
	}
	return true;
}

/**
 * General Insert/Update function for TeachesTwinPreference and TeacherTwinPreference
 */
static bool insert_or_update_twin_scores(FILE * console_out, sqlite3 * db, const char * const sql, int obj_id, int * twin_scores){
	int i, errc;
	sqlite3_stmt * stmt;

	LMH_ASSERT(db != NULL && obj_id > 0 && twin_scores != NULL);

	errc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(i = 0; twin_scores[i] >= 0; ++i){
		sqlite3_bind_int(stmt,1, i+1);
		sqlite3_bind_int(stmt,2, twin_scores[i]);
		sqlite3_bind_int(stmt,3, obj_id);
		errc = sqlite3_step(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
	return true;
}

/* Deep insert/update. */
bool insert_or_update_teaches(FILE * console_out, sqlite3* db, Teaches * t, School * school){
	int errc = 0;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_TEACHES, -1, &stmt, NULL);

	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_int(stmt,1,t->score);
	sqlite3_bind_int(stmt,2,t->max_per_day);
	sqlite3_bind_int(stmt,3,t->max_per_class_per_day);
	sqlite3_bind_int(stmt,4,t->teacher->id);
	sqlite3_bind_int(stmt,5,t->subject->id);

	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(false);
	sqlite3_finalize(stmt);
	errc = sqlite3_exec(db, LASTID_TABLE_TEACHES, get_id_callback, &(t->id), NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	if(t->room_scores != NULL){
		insert_or_update_room_scores(console_out, db, UPSERT_TABLE_TEACHES_ROOM, t->id, t->room_scores, school);
	}
	if(t->period_scores != NULL){
		insert_or_update_period_scores(console_out, db, UPSERT_TABLE_TEACHES_PERIOD, t->id, t->period_scores, school);
	}
	if(t->twin_scores != NULL){
		insert_or_update_twin_scores(console_out, db, UPSERT_TABLE_TEACHES_TWIN_PREFERENCE, t->id, t->twin_scores);
	}
	/* TODO must insert at all assignments that this is a possible teacher? */
	return true;
}

/**
 * General Insert/Update function for the table TeacherDay.
 */
static bool insert_or_update_teacher_day_scores(FILE * console_out, sqlite3 * db, Teacher * t, School  * school){
	int i, errc;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_TEACHER_DAY, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(i = 0; i < school->n_days; ++i){
		sqlite3_bind_int(stmt,1, t->id);
		sqlite3_bind_int(stmt,2, school->day_ids[i]);
		sqlite3_bind_int(stmt,3, t->day_max_meetings[i]);
		sqlite3_bind_int(stmt,4, t->day_scores[i]);
		errc = sqlite3_step(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
	return true;
}

/* NOTE: Shallow UPDATE. For deeper updates, update each table manually. */
bool update_teacher_basic_data(FILE * console_out, sqlite3 * db, Teacher * teacher, School * school){
	int errc;
	sqlite3_stmt * stmt;
	LMH_ASSERT(db != NULL && teacher != NULL && school != NULL);
	errc = sqlite3_prepare(db, UPDATE_TABLE_TEACHER, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(true);
	sqlite3_bind_text(stmt, 1, teacher->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, teacher->short_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 3, teacher->max_days);
	sqlite3_bind_int(stmt, 4, teacher->max_meetings_per_day);
	sqlite3_bind_int(stmt, 5, teacher->max_meetings_per_class_per_day);
	sqlite3_bind_int(stmt, 6, teacher->max_meetings);
	sqlite3_bind_int(stmt, 7, teacher->planning_needs_room);
	sqlite3_bind_int(stmt, 8, teacher->num_planning_periods);
	sqlite3_bind_int(stmt, 9, teacher->active);
	sqlite3_bind_int(stmt,10, school->id);
	sqlite3_bind_int(stmt,11, teacher->id);

	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(false);
	sqlite3_finalize(stmt);

	return true;
}

/* TODO test. */
int insert_teacher(FILE * console_out, sqlite3 * db, Teacher * teacher, School * school, int optional_id){
	int errc, i = 0;
	sqlite3_stmt * stmt;
	LMH_ASSERT(db != NULL && teacher != NULL && school != NULL);
	errc = sqlite3_prepare(db, INSERT_TABLE_TEACHER, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	if(optional_id != -1){
		sqlite3_bind_int(stmt,1, optional_id);
	}
	sqlite3_bind_text(stmt, 2, teacher->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, teacher->short_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 4, teacher->max_days);
	sqlite3_bind_int(stmt, 5, teacher->max_meetings_per_day);
	sqlite3_bind_int(stmt, 6, teacher->max_meetings_per_class_per_day);
	sqlite3_bind_int(stmt, 7, teacher->max_meetings);
	sqlite3_bind_int(stmt, 8, teacher->planning_needs_room);
	sqlite3_bind_int(stmt, 9, teacher->num_planning_periods);
	sqlite3_bind_int(stmt,10, teacher->active);
	sqlite3_bind_int(stmt,11, school->id);

	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(-1);
	sqlite3_finalize(stmt);

	errc = sqlite3_exec(db, LASTID_TABLE_TEACHER, get_id_callback, &(teacher->id), NULL);
	if(teacher->teaches != NULL){
		for(i = 0;  (teacher->teaches[i] != NULL) && (teacher->teaches[i]->subject != NULL); ++i){
			teacher->teaches[i]->teacher = teacher;
			insert_or_update_teaches(console_out, db, teacher->teaches[i], school);
		}
	}
	if(teacher->subordinates != NULL){
		/* NOTE: Frankly, rebooting it all is unecessarily expensive... but works. */
		exec_and_check(db, DELETE_TEACHER_SUBORDINATION_BY_SUP_ID, teacher->id);
		for(i = 0; i < school->n_teachers; ++i){
			if(teacher->subordinates[i]){
				insert_or_delete_subordination(console_out, db, INSERT_TABLE_TEACHER_SUBORDINATION, teacher->id, school->teachers[i].id);
			}
		}
	}
	if(teacher->lecture_room_scores != NULL || teacher->planning_room_scores != NULL){
		insert_or_update_teacher_room_scores(console_out, db, teacher, school);
	}
	if(teacher->day_max_meetings != NULL || teacher->day_scores != NULL){
		insert_or_update_teacher_day_scores(console_out, db, teacher, school);
	}
	if(teacher->lecture_period_scores != NULL || teacher->planning_period_scores){
		insert_or_update_teacher_period_scores(console_out, db, teacher, school);
	}
	if(teacher->planning_twin_scores != NULL){
		insert_or_update_twin_scores(console_out, db, UPSERT_TABLE_TEACHER_TWIN_PREFERENCE, teacher->id, teacher->planning_twin_scores);
	}
	return teacher->id;
}

bool insert_or_update_assignment(FILE * console_out, sqlite3 * db, Assignment * assignment, School * school){
	int errc;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_CLASS_SUBJECT, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_int(stmt,1, assignment->amount);
	sqlite3_bind_int(stmt,2, assignment->max_per_day);
	sqlite3_bind_int(stmt,3, assignment->m_class->id);
	sqlite3_bind_int(stmt,4, assignment->subject->id);
	errc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	CERTIFY_ERRC_SQLITE_DONE(false);
	sqlite3_exec(db, LASTID_TABLE_CLASS_SUBJECT, get_id_callback, &(assignment->id), NULL);
	return true;
}

bool insert_or_update_class_subject_group(FILE * console_out, sqlite3 * db, Class * class, School * school){
	int i, errc;
	sqlite3_stmt * stmt;

	LMH_ASSERT(db != NULL && class != NULL && school != NULL);

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_CLASS_SUBJECT_GROUP, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(i = 0; i < school->n_subject_groups && class->max_per_day_subject_group[i] >= 0; ++i){
		// TODO: Testar
		sqlite3_bind_int(stmt,1, class->id);
		sqlite3_bind_int(stmt,2, school->subject_group_ids[i]);
		sqlite3_bind_int(stmt,3, class->max_per_day_subject_group[i]);
		errc = sqlite3_step(stmt);
		CERTIFY_ERRC_SQLITE_DONE(stmt);
		// id_class, id_group, max_per_day
	}

	return true;
}

bool update_class_max_per_day_subjectgroup(FILE * console_out, sqlite3 * db, int class_id, int group_id, int max){
	sqlite3_stmt * stmt;
	int errc = sqlite3_prepare(db, UPSERT_TABLE_CLASS_SUBJECT_GROUP, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_int(stmt,1,class_id);
	sqlite3_bind_int(stmt,2,group_id);
	sqlite3_bind_int(stmt,3,max);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(false);
	return true;
}

/* TODO test. */
int insert_class(FILE * console_out, sqlite3 * db, Class * class, School * school, int optional_id){
	sqlite3_stmt * stmt;
	int errc, i;

	LMH_ASSERT(db != NULL && class != NULL && school != NULL);

	errc = sqlite3_prepare(db,INSERT_TABLE_CLASS, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	if(optional_id != -1){
		sqlite3_bind_int(stmt,1,optional_id);
	}
	sqlite3_bind_text(stmt, 2, class->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, class->short_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 4, class->size);
	sqlite3_bind_int(stmt, 5, class->can_have_free_periods_flag);
	sqlite3_bind_int(stmt, 6, class->maximal_entry_period>=0?school->daily_period_ids[class->maximal_entry_period]:0);
	sqlite3_bind_int(stmt, 7, class->minimal_exit_period>=0?school->daily_period_ids[class->minimal_exit_period]:0);
	sqlite3_bind_int(stmt, 8, class->active);
	sqlite3_bind_int(stmt, 9, school->id);

	errc = sqlite3_step(stmt);

	CERTIFY_ERRC_SQLITE_DONE(-1);
	sqlite3_exec(db, LASTID_TABLE_CLASS, get_id_callback, &(class->id), NULL);
	sqlite3_finalize(stmt);

	insert_or_update_period_scores(console_out, db, UPSERT_TABLE_CLASS_ATTENDANCE, class->id, class->period_scores, school);
	if(class->assignments != NULL){
		for(i = 0;class->assignments[i] != NULL; ++i){
			insert_or_update_assignment(console_out, db, class->assignments[i], school);
		}
		printf("Upserted %d assignments\n", i);
	}
	if(class->subordinates != NULL){
		for(i = 0; i < school->n_classes; ++i){
			if(class->subordinates[i] > 0){
				insert_or_delete_subordination(console_out, db, INSERT_TABLE_CLASS_SUBORDINATION, class->id, school->classes[i].id);
			}
		}
	}
	if(class->room_scores != NULL){
		insert_or_update_room_scores(console_out, db, UPSERT_TABLE_CLASS_ROOM, class->id, class->room_scores, school);
	}
	if(class->max_per_day_subject_group != NULL){
		insert_or_update_class_subject_group(console_out, db, class, school);
	}
	return class->id;
}

/* NOTE: Shallow UPDATE. For deeper updates, update each table manually. */
bool update_class_basic_data(FILE * console_out, sqlite3 * db, Class * class, School * school){
	sqlite3_stmt * stmt;
	int errc;

	LMH_ASSERT(db != NULL && class != NULL && school != NULL);

	errc = sqlite3_prepare(db,UPDATE_TABLE_CLASS, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_text(stmt, 1, class->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, class->short_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 3, class->size);
	sqlite3_bind_int(stmt, 4, class->can_have_free_periods_flag);
	sqlite3_bind_int(stmt, 5, class->maximal_entry_period>=0?school->daily_period_ids[class->maximal_entry_period]:0);
	sqlite3_bind_int(stmt, 6, class->minimal_exit_period>=0?school->daily_period_ids[class->minimal_exit_period]:0);
	sqlite3_bind_int(stmt, 7, class->active);
	sqlite3_bind_int(stmt, 8, school->id);
	sqlite3_bind_int(stmt, 9, class->id);

	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(false);
	sqlite3_exec(db, LASTID_TABLE_CLASS, get_id_callback, &(class->id), NULL);
	sqlite3_finalize(stmt);
	return true;
}

/* TODO test. */
int insert_room(FILE * console_out, sqlite3 * db, Room * room, School * school, int optional_id){
	sqlite3_stmt * stmt;
	int errc;

	LMH_ASSERT(db != NULL && room != NULL && school != NULL);
	room->id = -1;

	sqlite3_prepare(db, INSERT_TABLE_ROOM, -1, &stmt, NULL);
	if(optional_id != -1){
		sqlite3_bind_int(stmt,1, optional_id);
	}
	sqlite3_bind_text(stmt, 2, room->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, room->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,  4, room->size);
	sqlite3_bind_int(stmt,  5, room->active);
	sqlite3_bind_int(stmt,  6, school->id);

	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(-1);

	errc = sqlite3_exec(db, LASTID_TABLE_ROOM, get_id_callback, &(room->id), NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);

	sqlite3_finalize(stmt);

	insert_or_update_period_scores(console_out, db, UPSERT_TABLE_ROOM_AVAILABILITY, room->id, room->availability, school);

	return room->id;
}

int insert_subject_group(FILE * console_out,sqlite3 * db, School * school, char * group_name, int optional_id){
	int errc, id;
	sqlite3_stmt * stmt;
	LMH_ASSERT(db != NULL && school != NULL && group_name != NULL);
	errc = sqlite3_prepare(db, INSERT_TABLE_SUBJECT_GROUP, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	if(optional_id != -1){
		sqlite3_bind_int(stmt,1,optional_id);
	} else {
		sqlite3_bind_null(stmt,0);
	}
	sqlite3_bind_text(stmt,2, group_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,3, school->id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(-1);
	sqlite3_finalize(stmt);
	sqlite3_exec(db, LASTID_TABLE_SUBJECT_GROUP, get_id_callback, &id, NULL);
	return id;
}

int insert_subject_in_group(FILE * console_out,sqlite3 * db, int subj_id, int group_id){
	int errc, id;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare(db, INSERT_TABLE_SUBJECT_IN_GROUP, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);

	sqlite3_bind_int(stmt,1,subj_id);
	sqlite3_bind_int(stmt,2,group_id);
	errc = sqlite3_step(stmt);

	CERTIFY_ERRC_SQLITE_DONE(-1);
	sqlite3_finalize(stmt);
	sqlite3_exec(db, LASTID_TABLE_SUBJECT_IN_GROUP, get_id_callback, &id, NULL);
	return id;
}

/* TODO test */
int insert_subject(FILE * console_out, sqlite3* db, Subject * subject, School * school, int optional_id){
	int errc = 0, i = 0;
	sqlite3_stmt * stmt = NULL;

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_SUBJECT, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);

	if(optional_id != -1){
		sqlite3_bind_int(stmt,1,optional_id);
	}
	sqlite3_bind_text(stmt,2, subject->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt,3, subject->short_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,4, school->id);

	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(-1);
	sqlite3_finalize(stmt);
	errc = sqlite3_exec(db, LASTID_TABLE_SUBJECT, get_id_callback, &subject->id, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);

	for(i = 0; i < school->n_subject_groups && subject->in_groups && subject->in_groups[i] >= 0 ; ++i){
		if(subject->in_groups[i] > 0){
			insert_subject_in_group(console_out, db, subject->id, school->subject_group_ids[i]);
		}
	}
	return subject->id;
}

bool insert_meetings_list(FILE * console_out, sqlite3 * db, Meeting * meetings, School * school){
	int i, errc;
	sqlite3_stmt * stmt;
	LMH_ASSERT(db != NULL && meetings != NULL && school != NULL);
	errc = sqlite3_prepare_v2(db, INSERT_TABLE_LECTURE, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	int n_insert = 0;
	for(i = 0; meetings[i].type != meet_NULL; ++i){
		if(meetings[i].type == meet_LECTURE){
			Assignment * asg = find_assignment_by_class_subj_id(school,meetings[i].m_class->id,meetings[i].subject->id);
			sqlite3_bind_int(stmt,1, asg->id);
			sqlite3_bind_int(stmt,2, school->id);
			errc = sqlite3_step(stmt);
			sqlite3_reset(stmt);
			CERTIFY_ERRC_SQLITE_DONE(false);
			errc = sqlite3_exec(db, LASTID_TABLE_LECTURE, get_id_callback, &meetings[i].id, NULL);
			CERTIFY_ERRC_SQLITE_OK(false);
			++n_insert;
		}
	}
	printf("I is %d, n_insert %d\n", i, n_insert);
	sqlite3_finalize(stmt);

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_PLANNING, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	for(i = 0; meetings[i].type != meet_NULL; ++i){
		if(meetings[i].type == meet_PLANNING){
			sqlite3_bind_int(stmt,1, meetings[i].teacher->id);
			errc = sqlite3_step(stmt);
			sqlite3_reset(stmt);
			CERTIFY_ERRC_SQLITE_DONE(false);

			errc = sqlite3_exec(db, LASTID_TABLE_PLANNING, get_id_callback, &meetings[i].id, NULL);
			CERTIFY_ERRC_SQLITE_OK(false);
		}
	}
	sqlite3_finalize(stmt);

	for(i = 0; meetings[i].type != meet_NULL; ++i){
		if(meetings[i].type == meet_LECTURE){
			if (meetings[i].possible_periods != NULL){
				insert_or_update_period_scores(console_out, db, UPSERT_TABLE_LECTURE_POSSIBLE_PERIOD, meetings[i].id, meetings[i].possible_periods, school);
			}
			if(meetings[i].possible_rooms != NULL){
				insert_or_update_room_scores(console_out, db, UPSERT_TABLE_LECTURE_POSSIBLE_ROOM, meetings[i].id, meetings[i].possible_rooms, school);
			}
			if(meetings[i].possible_teachers != NULL){
				insert_or_update_teacher_scores(console_out, db, UPSERT_TABLE_POSSIBLE_TEACHER, meetings[i].id, meetings[i].possible_teachers, school);
			}
		} else if(meetings[i].type == meet_PLANNING){
			if (meetings[i].possible_periods != NULL){
				insert_or_update_period_scores(console_out, db, UPSERT_TABLE_PLANNING_PERIOD, meetings[i].id, meetings[i].possible_periods, school);
			}
			if(meetings[i].possible_rooms != NULL){
				insert_or_update_room_scores(console_out, db, UPSERT_TABLE_PLANNING_ROOM, meetings[i].id, meetings[i].possible_rooms, school);
			}
		}
	}
	return true;
}

/* TODO test */
static bool insert_all_meetings(FILE * console_out, sqlite3* db, School * school){
	int i,errc;
	sqlite3_stmt * stmt;
	fprintf(console_out, "Inserting all meetings.\n");

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_LECTURE, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	for(i = 0; i < school->n_meetings; ++i){
		if(school->meetings[i].type == meet_LECTURE){
			Assignment * asg = find_assignment_by_class_subj_id(school,school->meetings[i].m_class->id,school->meetings[i].subject->id);
			sqlite3_bind_int(stmt,1, asg->id);
			sqlite3_bind_int(stmt,2, school->id);
			errc = sqlite3_step(stmt);
			sqlite3_reset(stmt);
			CERTIFY_ERRC_SQLITE_DONE(false);

			errc = sqlite3_exec(db, LASTID_TABLE_LECTURE, get_id_callback, &school->meetings[i].id, NULL);
			CERTIFY_ERRC_SQLITE_OK(false);
		}
	}
	sqlite3_finalize(stmt);

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_PLANNING, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	for(i = 0; i < school->n_meetings; ++i){
		if(school->meetings[i].type == meet_PLANNING){
			sqlite3_bind_int(stmt,1, school->meetings[i].teacher->id);
			errc = sqlite3_step(stmt);
			sqlite3_reset(stmt);
			CERTIFY_ERRC_SQLITE_DONE(false);

			errc = sqlite3_exec(db, LASTID_TABLE_PLANNING, get_id_callback, &school->meetings[i].id, NULL);
			CERTIFY_ERRC_SQLITE_OK(false);
		}
	}
	sqlite3_finalize(stmt);

	for(i = 0; i < school->n_meetings; ++i){
		if(school->meetings[i].type == meet_LECTURE){
			if (school->meetings[i].possible_periods != NULL){
				insert_or_update_period_scores(console_out, db, UPSERT_TABLE_LECTURE_POSSIBLE_PERIOD, school->meetings[i].id, school->meetings[i].possible_periods, school);
			}
			if(school->meetings[i].possible_rooms != NULL){
				insert_or_update_room_scores(console_out, db, UPSERT_TABLE_LECTURE_POSSIBLE_ROOM, school->meetings[i].id, school->meetings[i].possible_rooms, school);
			}
			if(school->meetings[i].possible_teachers != NULL){
				insert_or_update_teacher_scores(console_out, db, UPSERT_TABLE_POSSIBLE_TEACHER, school->meetings[i].id, school->meetings[i].possible_teachers, school);
			}
		} else if(school->meetings[i].type == meet_PLANNING){
			if (school->meetings[i].possible_periods != NULL){
				insert_or_update_period_scores(console_out, db, UPSERT_TABLE_PLANNING_PERIOD, school->meetings[i].id, school->meetings[i].possible_periods, school);
			}
			if(school->meetings[i].possible_rooms != NULL){
				insert_or_update_room_scores(console_out, db, UPSERT_TABLE_PLANNING_ROOM, school->meetings[i].id, school->meetings[i].possible_rooms, school);
			}
		}
	}
	return true;
}

/* TODO test. */
int insert_solution(FILE * console_out, sqlite3 * db, School * school, Solution * sol, int optional_id){
	int i, errc = 0;
	sqlite3_stmt * stmt;
	fprintf(console_out, "Inserting solution.\n");

	LMH_ASSERT(db != NULL && sol != NULL && school != NULL);
	errc = sqlite3_prepare_v2(db, INSERT_TABLE_SOLUTION, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	if(optional_id != -1){
		sqlite3_bind_int(stmt,1, optional_id);
	}
	sqlite3_bind_text(stmt,2, sol->gen_date, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt,3,sol->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt,4,sol->desc, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,5,school->id);

	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(-1);

	errc = sqlite3_exec(db, LASTID_TABLE_SOLUTION, get_id_callback, &(sol->id), NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	sqlite3_finalize(stmt);

	errc = sqlite3_prepare(db, UPSERT_TABLE_LECTURE_SOLUTION, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	int i_inserted = 0;
	for(i = 0; i < sol->n_meetings; ++i){
		if(sol->meetings[i].type == meet_LECTURE){
			LMH_ASSERT(school->meetings != NULL && school->period_ids != NULL && sol->meetings[i].teacher != NULL && sol->meetings[i].room != NULL);
			sqlite3_bind_int(stmt, 1, school->meetings[i].id);
			sqlite3_bind_int(stmt, 2, sol->id);
			sqlite3_bind_int(stmt, 3, school->period_ids[ sol->meetings[i].period ]);
			sqlite3_bind_int(stmt, 4, sol->meetings[i].teacher->id);
			sqlite3_bind_int(stmt, 5, sol->meetings[i].room->id);
			sqlite3_bind_int(stmt, 6, sol->meetings[i].m_class->id);
			sqlite3_bind_int(stmt, 7, sol->meetings[i].subject->id);

			errc = sqlite3_step(stmt);
			CERTIFY_ERRC_SQLITE_DONE(-1);

			errc = sqlite3_exec(db, LASTID_TABLE_LECTURE, get_id_callback, &(sol->meetings[i].id), NULL);
			CERTIFY_ERRC_SQLITE_OK(-1);
			sqlite3_reset(stmt);
			++i_inserted;
		}
	}
	printf("From n_meetings %d, inserted %d.\n", sol->n_meetings, i_inserted);
	sqlite3_finalize(stmt);
	return sol->id;
}

/* TODO test. */
static bool insert_all_teaches(FILE * console_out, sqlite3 * db, School * school){
	int i = 0;
	bool err = false;
	printf("Inserting all teaches.\n");
	for(i = 0; i < school->n_teaches; ++i){
		err |= insert_or_update_teaches(console_out, db, &school->teaches[i], school);
	}
	return !err;
}

/* TODO test. */
static bool insert_all_teachers(FILE * console_out, sqlite3 * db, School * school){
	int i;
	fprintf(console_out, "Inserting all teachers.\n");
	for(i = 0; i < school->n_teachers; ++i){
		insert_teacher(console_out,db,&(school->teachers[i]), school, -1);
	}
	return false;
}

/* TODO test */
static bool insert_all_rooms(FILE * console_out, sqlite3 * db, School * school){
	int i = 0;
	bool err = false;
	fprintf(console_out,"Inserting all rooms");
	for(i = 0; i < school->n_rooms; ++i){
		err |= insert_room(console_out, db, &school->rooms[i], school, -1);
	}
	return !err;
}

/* TODO test. */
static bool insert_all_classes(FILE * console_out, sqlite3 * db, School * school){
	int i;
	bool err = false;
	fprintf(console_out, "Inserting all classes.\n");
	for(i = 0; i < school->n_classes; i++){
		err |= insert_class(console_out,db, &school->classes[i],school, -1);
	}
	return !err;
}

/* TODO test. */
static bool insert_all_periods(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i;
	int errc;

	printf("Inserting all periods.");

	school->period_ids = calloc(school->n_periods, sizeof((*school->period_ids)));
	errc = sqlite3_prepare(db, UPSERT_TABLE_PERIOD, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);

	for(i = 0; i < school->n_periods; ++i){
		if(school->period_names != NULL){
			sqlite3_bind_text(stmt,1, school->period_names[i], -1, SQLITE_TRANSIENT);
		} else {
			sqlite3_bind_null(stmt,1);
		}
		sqlite3_bind_int(stmt,2, school->periods[i]);
		sqlite3_bind_int(stmt,3, school->day_ids[i / school->n_periods_per_day	]);
		sqlite3_bind_int(stmt,4, school->daily_period_ids[i % school->n_periods_per_day] );
		sqlite3_bind_int(stmt,5, school->id);
		errc = sqlite3_step(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
		errc = sqlite3_exec(db, LASTID_TABLE_PERIOD, get_id_callback, &(school->period_ids[i]), NULL);
		CERTIFY_ERRC_SQLITE_OK(false);
		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
	return true;
}

/* TODO more or less tested. */
static bool insert_all_daily_periods(FILE * console_out, sqlite3* db, School * school){
	sqlite3_stmt * stmt;
	int i,errc;

	fprintf(console_out, "Inserting all daily periods.\n");
	school->daily_period_ids = calloc(school->n_periods_per_day, sizeof((*school->daily_period_ids)));
	sqlite3_prepare(db, UPSERT_TABLE_DAILY_PERIOD, -1, &stmt, NULL);
	for(i = 0; i < school->n_periods_per_day; ++i){
		sqlite3_bind_text(stmt,1, school->daily_period_names[i], -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt,2, i);
		sqlite3_bind_int(stmt,3, school->id);
		errc = sqlite3_step(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
		errc = sqlite3_exec(db, LASTID_TABLE_DAILY_PERIOD, get_id_callback, &(school->daily_period_ids[i]), NULL);
		CERTIFY_ERRC_SQLITE_OK(false);
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
	errc = sqlite3_prepare(db, UPSERT_TABLE_DAY, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(i = 0; i < school->n_days; i++){
		sqlite3_bind_text(stmt,1, school->day_names[i], -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt,2, i);
		sqlite3_bind_int(stmt,3, school->id);
		errc = sqlite3_step(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
		errc = sqlite3_exec(db, LASTID_TABLE_DAY, get_id_callback, &(school->day_ids[i]), NULL);
		CERTIFY_ERRC_SQLITE_OK(false);
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

	school->id = -1;
	if(console_out && (school->name)){
		fprintf(console_out, "Inserting school (%s) into school table.\n", school->name);
	}
	errc = sqlite3_prepare(db, INSERT_TABLE_SCHOOL, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);

	sqlite3_bind_text(stmt,1, school->name, -1,SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,2, school->n_periods_per_day);
	sqlite3_bind_int(stmt,3, school->n_days);

	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(false);
	sqlite3_finalize(stmt);
	errc = sqlite3_exec(db, LASTID_TABLE_SCHOOL, get_id_callback, &(school->id), NULL);
	CERTIFY_ERRC_SQLITE_OK(false);

	if(school->day_names != NULL){
		insert_all_days(console_out, db, school);
	}
	if(school->daily_period_names != NULL){
		insert_all_daily_periods(console_out, db, school);
	}
	if(school->day_names != NULL && school->daily_period_names != NULL){
		insert_all_periods(console_out, db, school);
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
	if(school->solutions != NULL){
		for(int i = 0; i < school->n_solutions; ++i){
			insert_solution(console_out, db, school, &school->solutions[i], -1);
		}
	}

	return school->id;
}

/*********************************************************/
/*                   SELECT  FUNCTIONS                   */
/*********************************************************/

/*
* Returns a null-terminated list of strings, all freeable.
* In case of error, returns null
*/
char** select_all_school_names(FILE * console_out, sqlite3* db, int ** ids){
	int i = 0, errc;
	sqlite3_stmt * stmt;
	char ** strings = NULL;

	errc = sqlite3_prepare_v2(db, SELECT_SCHOOL_NAMES, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);
	errc = sqlite3_step(stmt);
	strings = calloc(11, sizeof(char*));
	if(ids != NULL){
		*ids = calloc(11, sizeof(char *));
	}
	while( errc == SQLITE_ROW ){
		if(ids != NULL){
			(*ids)[i] = sqlite3_column_int(stmt,0);
		}
		strings[i] = copy_sqlite_string(stmt,1);

		++i;
		if(i % 10 == 0){
			strings = realloc(strings, (i+11)*sizeof(char*));
			if(*ids != NULL){
				*ids = realloc(*ids, (i+11)*sizeof(int));
			}
		}
		errc = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
	return strings;
}

static int * select_period_scores(FILE * console_out, sqlite3* db, const char * const sql, int id, School * school){
	int errc = 0, i_per = 0;
	sqlite3_stmt * stmt;
	int * arr = NULL;

	errc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);
	sqlite3_bind_int(stmt,1,id);
	errc = sqlite3_step(stmt);
	arr = calloc(school->n_periods + 1, sizeof(int));
	arr[school->n_periods] = -1;
	while(errc == SQLITE_ROW){
		i_per = get_per_index_by_id(school, sqlite3_column_int(stmt,2));
		arr[i_per] = sqlite3_column_int(stmt,3);
		errc = sqlite3_step(stmt);
	}
	CERTIFY_ERRC_SQLITE_DONE(NULL);
	return arr;
}

static int * select_teacher_scores(FILE * console_out, sqlite3* db, const char * const sql, int id, School * school){
	int i_teacher, errc, *scores=NULL;
	sqlite3_stmt* stmt;

	errc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);
	sqlite3_bind_int(stmt,1,id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(NULL);
	if(errc == SQLITE_ROW){
		scores = calloc(school->n_teachers +1, sizeof(int));
		scores[school->n_teachers] = -1;
		while(errc == SQLITE_ROW){
			i_teacher = get_teacher_index_by_id(school, sqlite3_column_int(stmt,2));
			scores[i_teacher] = sqlite3_column_int(stmt,3);
			errc = sqlite3_step(stmt);
		}
	}
	sqlite3_finalize(stmt);

	return scores;
}

/*
 * General select function for tables LecturePossibleRoom, ClassRoom, TeachesRoom, and PlanningRoom.
 */
static int * select_room_scores(FILE * console_out, sqlite3* db, const char * const sql, int id, School * school){
	int i = 0, errc, *scores = NULL, i_room;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);

	sqlite3_bind_int(stmt,1, id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(NULL);
	scores = calloc(school->n_rooms + 1, sizeof(int));
	scores[school->n_rooms] = -1;
	while(errc == SQLITE_ROW){
		i_room = get_room_index_by_id(school, sqlite3_column_int(stmt,1));
		scores[i_room] = sqlite3_column_int(stmt,2);
		errc = sqlite3_step(stmt);
		++i;
	}
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(NULL);
	sqlite3_finalize(stmt);
	return scores;
}

/*
 * Select function for table TeacherRoom.
 */
static bool select_teacher_room_scores(FILE * console_out, sqlite3* db, Teacher * teacher, School * school){
	int i = 0, errc, i_room;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, SELECT_TABLE_TEACHER_ROOM_BY_TEACHER_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);

	sqlite3_bind_int(stmt,1, teacher->id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(NULL);
	teacher->planning_room_scores = calloc(school->n_rooms+1, sizeof(int));
	teacher->lecture_room_scores = calloc(school->n_rooms+1, sizeof(int));
	while(errc == SQLITE_ROW){
		i_room = get_room_index_by_id(school, sqlite3_column_int(stmt,2));
		LMH_ASSERT(i_room >= 0 && i_room < school->n_rooms);
		teacher->lecture_room_scores[i_room] = sqlite3_column_int(stmt,3);
		teacher->planning_room_scores[i_room] = sqlite3_column_int(stmt,4);
		errc = sqlite3_step(stmt);
		++i;
	}
	teacher->lecture_room_scores[school->n_rooms] = -1;
	teacher->planning_room_scores[school->n_rooms] = -1;
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(false);
	sqlite3_finalize(stmt);
	return true;
}

// TODO select planning
static int select_all_meetings_by_solution_id(FILE * console_out, sqlite3* db, School * school, Solution * sol){
	int i = 0, errc;
	sqlite3_stmt* stmt;
	errc = sqlite3_prepare_v2(db, SELECT_LECTURE_SOLUTION_BY_SOLUTION_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(-1);
	sqlite3_bind_int(stmt,1, sol->id);
	errc = sqlite3_step(stmt);
	/* Double check because we can't alloc if there aren't any */
	if(errc == SQLITE_ROW){
		sol->meetings = calloc(11, sizeof(Meeting));
		while(errc == SQLITE_ROW){
			sol->meetings[i].id = sqlite3_column_int(stmt,0);
			sol->meetings[i].type = meet_LECTURE;
			sol->meetings[i].period = get_per_index_by_id(school, sqlite3_column_int(stmt,3));
			sol->meetings[i].teacher = find_teacher_by_id(school, sqlite3_column_int(stmt,4));
			sol->meetings[i].room = find_room_by_id(school, sqlite3_column_int(stmt, 5));
			sol->meetings[i].m_class = find_class_by_id(school, sqlite3_column_int(stmt,6));
			sol->meetings[i].subject = find_subject_by_id(school, sqlite3_column_int(stmt,7));

			errc = sqlite3_step(stmt);
			++i;
			if(i % 10 == 0){
				sol->meetings = realloc(sol->meetings, (i + 11)*sizeof(Meeting));
			}
		}
		sol->n_meetings = i;
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
			school->solutions[i].gen_date = copy_sqlite_string(stmt,1);
			school->solutions[i].name = copy_sqlite_string(stmt,2);
			school->solutions[i].desc = copy_sqlite_string(stmt,3);

			++i;
			errc = sqlite3_step(stmt);

			if(i % 10 == 0){
				school->solutions = realloc(school->solutions, (i + 11)*sizeof(Solution));
			}
		}
		sqlite3_finalize(stmt);
		school->n_solutions = i;
		for(i = 0; i < school->n_solutions; ++i){
			select_all_meetings_by_solution_id(console_out, db, school, &school->solutions[i]);
		}
	}
	return school->n_solutions;
}

// TODO select planning
static bool select_all_meetings(FILE * console_out, sqlite3* db, School * school){
	int errc, i = 0;
	sqlite3_stmt * stmt;
	errc = sqlite3_prepare_v2(db, SELECT_LECTURE_BY_SCHOOL_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_int(stmt, 1, school->id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(false);
	if(errc == SQLITE_ROW){
		school->meetings = calloc(11, sizeof(Meeting));
		while(errc == SQLITE_ROW){
			school->meetings[i].type = meet_LECTURE;
			school->meetings[i].id = sqlite3_column_int(stmt,0);
			school->meetings[i].possible_teachers = NULL;
			school->meetings[i].possible_rooms = NULL;
			school->meetings[i].possible_periods = NULL;
			Assignment * asg = find_assignment_by_id(school, sqlite3_column_int(stmt,1));
			if(asg != NULL){
				school->meetings[i].m_class = asg->m_class;
				school->meetings[i].subject = asg->subject;
				if(asg->possible_teachers){
					school->meetings[i].possible_teachers = int_list_copy(asg->possible_teachers);
				}
			} else {
				printf("Did not found assignment by id in select_all_meetings.\n");
				school->meetings[i].m_class = NULL;
				school->meetings[i].subject = NULL;
			}
			errc = sqlite3_step(stmt);
			++i;
			if(i % 10 == 0){
				school->meetings = realloc(school->meetings, (i + 11)*sizeof(Meeting));
			}
		}
	}
	sqlite3_finalize(stmt);
	school->n_meetings = i;

	for(i = 0; i < school->n_meetings; ++i){
		school->meetings[i].possible_rooms = select_room_scores(console_out, db, SELECT_TABLE_LECTURE_POSSIBLE_ROOM_BY_LECTURE_ID, school->meetings[i].id, school);
		school->meetings[i].possible_periods = select_period_scores(console_out, db, SELECT_TABLE_LECTURE_POSSIBLE_PERIOD_BY_LECTURE_ID, school->meetings[i].id, school);
	}
	return true;
}

/* TODO test. */
static Room * select_all_rooms_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_rooms, School * school){
	int i = 0, errc = 0;
	Room * rooms = NULL;
	sqlite3_stmt * stmt = NULL;

	fprintf(console_out,"Selecting all rooms.\n");
	errc = sqlite3_prepare_v2(db, SELECT_ROOM_BY_SCHOOL_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);
	sqlite3_bind_int(stmt,1, id);
	errc = sqlite3_step(stmt);
	rooms = calloc(11, sizeof(Room));
	while(errc == SQLITE_ROW){
		rooms[i].id = sqlite3_column_int(stmt,0);
		rooms[i].name = copy_sqlite_string(stmt,1);
		rooms[i].short_name = copy_sqlite_string(stmt,2);
		rooms[i].size = sqlite3_column_int(stmt,3);
		rooms[i].active = sqlite3_column_int(stmt,4) > 0;
		errc = sqlite3_step(stmt);
		rooms[i].availability = select_period_scores(console_out, db, SELECT_ROOM_AVAILABILITY_BY_ROOM_ID, rooms[i].id, school);
		++i;
		if(i % 10 == 0){
			rooms = realloc(rooms, (11+i)*sizeof(Room));
		}
	}
	sqlite3_finalize(stmt);
	if(n_rooms != NULL){
		*n_rooms = i;
	}
	return rooms;
}

static int * select_class_subordinates(FILE * console_out, sqlite3 * db, Class * c, School * school){
	sqlite3_stmt * stmt;
	int errc,i=0,i_sub;
	errc = sqlite3_prepare_v2(db, SELECT_CLASS_SUBORDINATION_BY_SUP_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);
	sqlite3_bind_int(stmt,1,c->id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(NULL);
	if(errc == SQLITE_ROW) {
		c->subordinates = calloc(1+ school->n_teachers, sizeof(int));
		while(errc == SQLITE_ROW){
			i_sub = get_class_index_by_id(school, sqlite3_column_int(stmt,1));
			c->subordinates[i_sub] = 1;
			errc = sqlite3_step(stmt);
			++i;
		}
	}
	return c->subordinates;
}

static int * select_teacher_subordinates_by_teacher_id(FILE * console_out, sqlite3 * db, Teacher * t, School * school){
	sqlite3_stmt * stmt;
	int errc,i=0,i_sub;

	errc = sqlite3_prepare_v2(db, SELECT_TEACHER_SUBORDINATION_BY_SUP_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);
	sqlite3_bind_int(stmt,1,t->id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(NULL);
	if(errc == SQLITE_ROW) {
		t->subordinates = calloc(1+ school->n_teachers, sizeof(int));
		while(errc == SQLITE_ROW){
			i_sub = get_teacher_index_by_id(school, sqlite3_column_int(stmt,2));
			t->subordinates[i_sub] = 1;
			errc = sqlite3_step(stmt);
			++i;
		}
	} else {
		t->subordinates = NULL;
	}
	return t->subordinates;
}

static bool select_teacher_attendance_by_teacher_id(FILE * console_out, sqlite3* db, Teacher * teacher, School *school){
	int i=0, per_i, errc;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, SELECT_TEACHER_ATTENDANCE_BY_TEACHER_ID,-1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_int(stmt,1,teacher->id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(false);
	if(errc == SQLITE_ROW){
		teacher->lecture_period_scores = calloc(1+school->n_periods, sizeof(int));
		teacher->planning_period_scores = calloc(1+school->n_periods, sizeof(int));
		teacher->lecture_period_scores[school->n_periods] =-1;
		teacher->planning_period_scores[school->n_periods] =-1;
		while(errc == SQLITE_ROW){
			per_i = get_per_index_by_id(school, sqlite3_column_int(stmt,2));
			teacher->lecture_period_scores[per_i] = sqlite3_column_int(stmt,3);
			teacher->planning_period_scores[per_i] = sqlite3_column_int(stmt,4);
			errc = sqlite3_step(stmt);
			++i;
		}
	}
	return true;
}

static bool select_teacher_day_by_teacher_id(FILE * console_out, sqlite3 * db, Teacher * teacher, School * school){
	int i=0, errc;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, SELECT_TEACHER_DAY_BY_TEACHER_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_int(stmt,1, teacher->id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(false);
	teacher->day_max_meetings = calloc(1 + school->n_days, sizeof(int));
	teacher->day_scores = calloc(1 + school->n_days, sizeof(int));
	// if(errc == SQLITE_ROW){
	while(errc == SQLITE_ROW){
		teacher->day_max_meetings[i] = sqlite3_column_int(stmt,3);
		teacher->day_scores[i] = sqlite3_column_int(stmt,4);
		++i;
		errc = sqlite3_step(stmt);
		CERTIFY_ERRC_SQLITE_ROW_OR_DONE(false);
	}
	teacher->day_max_meetings[school->n_days] = -1;
	teacher->day_scores[school->n_days] = -1;
	// }
	sqlite3_finalize(stmt);
	return true;
}

static int* select_twin_scores(FILE * console_out, sqlite3* db, const char * const sql, int id, School * school){
	int errc, *scores = NULL;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_int(stmt,1,id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(false);
	if(errc == SQLITE_ROW){
		scores = calloc(school->n_periods_per_day+1, sizeof(int));
		scores[school->n_periods_per_day] = -1;
		while(errc == SQLITE_ROW){
			int twin_i = sqlite3_column_int(stmt,1) -1;
			scores[twin_i] = sqlite3_column_int(stmt,2);
			errc = sqlite3_step(stmt);
		}
	}
	return scores;
}

/* TODO test. */
static Teacher * select_all_teachers_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_teachers, School * school){
	int i = 0, errc = 0, n = 0;
	Teacher * teachers;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, SELECT_TEACHER_BY_SCHOOL_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);
	sqlite3_bind_int(stmt,1,id);
	errc= sqlite3_step(stmt);
	teachers = calloc(11, sizeof(Teacher));
	while(errc == SQLITE_ROW){
		teachers[i].id = sqlite3_column_int(stmt,0);
		teachers[i].name = copy_sqlite_string(stmt,1);
		teachers[i].short_name = copy_sqlite_string(stmt,2);
		teachers[i].max_days = sqlite3_column_int(stmt,3);
		teachers[i].max_meetings_per_day = sqlite3_column_int(stmt,4);
		teachers[i].max_meetings_per_class_per_day = sqlite3_column_int(stmt,5);
		teachers[i].max_meetings = sqlite3_column_int(stmt,6);
		teachers[i].planning_needs_room = sqlite3_column_int(stmt,7);
		teachers[i].num_planning_periods = sqlite3_column_int(stmt,8);
		teachers[i].active = sqlite3_column_int(stmt,9);
		teachers[i].teaches = NULL;
		teachers[i].day_scores = NULL;
		teachers[i].day_max_meetings = NULL;
		errc = sqlite3_step(stmt);
		++i;
		if(i % 10 == 0){
			teachers = realloc(teachers,(i + 11)*sizeof(Teacher));
		}
	}
	sqlite3_finalize(stmt);
	school->teachers = teachers;
	n = i;
	if(n_teachers != NULL){
		*n_teachers = n;
	}
	for(i = 0; i < n; ++i){
		teachers[i].subordinates = select_teacher_subordinates_by_teacher_id(console_out, db, &teachers[i], school);
		select_teacher_attendance_by_teacher_id(console_out, db, &teachers[i], school);
		select_teacher_room_scores(console_out, db, &teachers[i], school);
		select_teacher_day_by_teacher_id(console_out, db, &teachers[i], school);
		teachers[i].planning_twin_scores = select_twin_scores(console_out, db, SELECT_TEACHER_TWIN_PREFERENCE_BY_TEACHER_ID, teachers[i].id, school);
	}
	return teachers;
}

static Assignment * select_all_assignments_by_school_id(FILE * console_out, sqlite3* db, int id_school, int * n_assignments, School * school){
	int errc, i = 0, n;
	sqlite3_stmt * stmt;
	Assignment * assignments = NULL;
	errc = sqlite3_prepare_v2(db, SELECT_CLASS_SUBJECT_BY_SCHOOL_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);
	sqlite3_bind_int(stmt,1, id_school);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(NULL);
	if(errc == SQLITE_ROW){
		assignments = calloc(11, sizeof(Assignment));
		while(errc == SQLITE_ROW){
			assignments[i].id 		   = sqlite3_column_int(stmt,0);
			assignments[i].amount 	   = sqlite3_column_int(stmt,1);
			assignments[i].max_per_day = sqlite3_column_int(stmt,2);
			assignments[i].m_class     = find_class_by_id(school, sqlite3_column_int(stmt,3));
			assignments[i].subject     = find_subject_by_id(school, sqlite3_column_int(stmt,4));
			++i;
			errc = sqlite3_step(stmt);
			if(i % 10 == 0){
				assignments = realloc(assignments,(i + 11)*sizeof(Assignment));
			}
		}
		assignments[i].id = -1;
		assignments[i].amount = 0;
		assignments[i].max_per_day = 0;
		assignments[i].m_class = NULL;
		assignments[i].subject = NULL;
	}
	sqlite3_finalize(stmt);

	n = i;
	if(n_assignments != NULL){
		*n_assignments = n;
	}
	for(i = 0; i < n; ++i){
		assignments[i].possible_teachers = select_teacher_scores(console_out, db, SELECT_TABLE_POSSIBLE_TEACHER_BY_CLASS_SUBJECT_ID, assignments[i].id, school);
	}
	return assignments;
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
			for(i = 0; i < school->n_classes; ++i){
				if(subid == school->classes[i].id){
					class->subordinates[i] = 1;
				}
			}

			errc = sqlite3_step(stmt);
		}
	}
	return class->subordinates;
}

static int * select_class_subject_group(FILE * console_out, sqlite3* db, int class_id, School * school){
	int errc, i=0, i_group=0, *max_per_day = NULL;
	sqlite3_stmt * stmt;

	errc = sqlite3_prepare_v2(db, SELECT_CLASS_SUBJECT_GROUP_BY_CLASS_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);
	sqlite3_bind_int(stmt,1,class_id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(NULL);
	if(errc == SQLITE_ROW){
		max_per_day = calloc(school->n_subject_groups, sizeof(int));
		max_per_day[school->n_subject_groups] = -1;
		while(errc == SQLITE_ROW){
			i_group = get_subject_group_index_by_id(school, sqlite3_column_int(stmt,2));
			max_per_day[i_group] = sqlite3_column_int(stmt,3);
			errc = sqlite3_step(stmt);
			++i;
		}
	}
	return max_per_day;
}

/* TODO test. */
static Class * select_all_classes_by_school_id(FILE * console_out, sqlite3* db, int id, int * n_classes, School * school){
	int i = 0, n, errc;
	sqlite3_stmt * stmt;
	Class * classes = NULL;

	errc = sqlite3_prepare_v2(db, SELECT_CLASS_BY_SCHOOL_ID, -1, &stmt, NULL);

	fprintf(console_out, "Selecting all classes.\n");
	if(errc != SQLITE_OK){
		fprintf(console_out, "Could not select all classes.");
	} else {
		sqlite3_bind_int(stmt,1,id);
		errc = sqlite3_step(stmt);

		classes = calloc(11, sizeof(Class));
		while(errc == SQLITE_ROW){
			classes[i].id = sqlite3_column_int(stmt,0);
			classes[i].name = copy_sqlite_string(stmt,1);
			classes[i].short_name = copy_sqlite_string(stmt,2);
			classes[i].size = sqlite3_column_int(stmt,3);
			classes[i].can_have_free_periods_flag = sqlite3_column_int(stmt,4);
			classes[i].active = sqlite3_column_int(stmt,7);
			classes[i].maximal_entry_period = get_daily_period_index_by_id(school, sqlite3_column_int(stmt,5));
			classes[i].minimal_exit_period = get_daily_period_index_by_id(school, sqlite3_column_int(stmt,6));
			classes[i].period_scores = NULL;
			classes[i].room_scores = NULL;
			classes[i].subordinates = NULL;
			classes[i].max_per_day_subject_group = NULL;
			classes[i].assignments = NULL;
			++i;
			errc = sqlite3_step(stmt);
			if(i % 10 == 0){
				classes = realloc(classes, (i+11) * sizeof(Class));
			}
		}
	}
	school->classes = classes;
	n = i;
	if(n_classes != NULL){
		*n_classes = i;
	}

	for(i = 0; i < n; ++i){
		classes[i].period_scores = select_period_scores(console_out, db, SELECT_CLASS_ATTENDANCE_BY_CLASS_ID, classes[i].id, school);
		classes[i].room_scores = select_room_scores(console_out, db, SELECT_CLASS_ROOM_BY_CLASS_ID, classes[i].id, school);
		select_all_class_subordination_by_class_id(console_out, db, &classes[i], school);
		classes[i].max_per_day_subject_group = select_class_subject_group(console_out, db, classes[i].id,school);
		/* The assignments are linked later, in link_assignments. Otherwise we would
		 * have a conflict of precedence (Classes need assignments and vice-versa)
		 */
		 classes[i].assignments = NULL;
	}
	return classes;
}

/* TODO test. */
static Teaches * select_all_teaches_by_school_id(FILE * console_out, sqlite3 * db, int id, int * n_teaches, School * school){
	int i = 0, n, errc = 0;
	sqlite3_stmt * stmt;
	Teaches * teaches = NULL;

	errc = sqlite3_prepare_v2(db, SELECT_TEACHES_BY_SCHOOL_ID, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(NULL);
	sqlite3_bind_int(stmt,1, id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_ROW_OR_DONE(NULL);
	if(errc == SQLITE_ROW){
		teaches = calloc(11, sizeof(Teaches));
		while(errc == SQLITE_ROW){
			teaches[i].id = sqlite3_column_int(stmt,0);
			teaches[i].score = sqlite3_column_int(stmt,1);
			teaches[i].max_per_day = sqlite3_column_int(stmt,2);
			teaches[i].max_per_class_per_day = sqlite3_column_int(stmt,3);
			teaches[i].teacher = find_teacher_by_id(school, sqlite3_column_int(stmt,4));
			teaches[i].subject = find_subject_by_id(school, sqlite3_column_int(stmt,5));
			errc = sqlite3_step(stmt);
			++i;
			if(i % 10 == 0){
				teaches = realloc(teaches, (i + 11)*sizeof(Teaches));
			}
		}
		sqlite3_finalize(stmt);
	}
	n = i;
	if(n_teaches != NULL){
		*n_teaches = n;
	}
	for(i = 0; i < n; ++i){
		teaches[i].period_scores = select_period_scores(console_out, db, SELECT_TEACHES_PERIOD_BY_TEACHES_ID, teaches[i].id, school);
		teaches[i].room_scores = select_room_scores(console_out, db, SELECT_TABLE_TEACHES_ROOM_BY_TEACHES_ID, teaches[i].id, school);
		teaches[i].twin_scores = select_twin_scores(console_out, db, SELECT_TEACHES_TWIN_PREFERENCE_BY_TEACHES_ID, teaches[i].id, school);
	}
	return teaches;
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
				school->subjects[i].in_groups = calloc(school->n_subjects + 1, sizeof(int));
				school->subjects[i].in_groups[school->n_subjects]  = -1;
				while(errc == SQLITE_ROW){
					int i_group = get_subject_group_index_by_id(school, sqlite3_column_int(stmt,2));
					school->subjects[i].in_groups[i_group]  = 1;
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
	int i = 0, errc, n_alloc, i_day;
	sqlite3_stmt * stmt;

	fprintf(console_out, "Selecting all days.\n");

	errc = sqlite3_prepare_v2(db, SELECT_DAY_BY_SCHOOL_ID, -1, &stmt, NULL);
	if(errc == SQLITE_OK){
		sqlite3_bind_int(stmt,1, id);
		errc = sqlite3_step(stmt);
		school->day_names = calloc(11, sizeof(char*));
		school->day_ids = calloc(11, sizeof(int));
		n_alloc = 11;
		while(errc == SQLITE_ROW){
			i_day = sqlite3_column_int(stmt,2);
			if(i_day > n_alloc){
				school->day_names = realloc(school->day_names, (i_day + 1)*sizeof(char*));
				school->day_ids = realloc(school->day_ids, (i_day + 1)*sizeof(int));
				n_alloc = i_day + 1;
			}
			school->day_ids[i_day] = sqlite3_column_int(stmt,0);
			school->day_names[i_day] = copy_sqlite_string(stmt,1);
			errc = sqlite3_step(stmt);
			++i;
		}
		LMH_ASSERT(i == i_day + 1);
		school->day_names[i] = NULL;
		school->day_ids[i] = -1;

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
		school->periods = calloc(school->n_periods + 1, sizeof(int));
		school->periods[school->n_periods] = -1;
		sqlite3_bind_int(stmt,1, id);
		errc = sqlite3_step(stmt);
		while(errc == SQLITE_ROW){
			day_i = get_day_index_by_id(school, sqlite3_column_int(stmt,3));
			day_per_i = get_daily_period_index_by_id(school, sqlite3_column_int(stmt,4));
			if(day_per_i != -1 && day_i != -1){
				per_i = day_i * school->n_periods_per_day + day_per_i;
				school->period_ids[per_i] = sqlite3_column_int(stmt,0);
				school->period_names[per_i] = copy_sqlite_string(stmt,1);
				school->periods[per_i] = sqlite3_column_int(stmt,2);
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

static void link_all_assignments(School * school){
	int i, j, n;
	for(i = 0; i < school->n_classes; ++i){
		n = 0;
		for(j = 0; j < school->n_assignments; ++j){
			if(school->assignments[j].m_class->id == school->classes[i].id){
				++n;
			}
		}
		school->classes[i].assignments = calloc(n+1, sizeof(Assignment*));
		n = 0;
		for(j = 0; j < school->n_assignments; ++j){
			if(school->assignments[j].m_class->id == school->classes[i].id){
				school->classes[i].assignments[n] = & school->assignments[j];
				++n;
			}
		}
		school->classes[i].assignments[n] = NULL;
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
		school->n_assignments = 0;
		select_all_days_by_school_id(stdout, db, id, &(school->n_days), school);
		select_all_daily_periods_by_school_id(stdout, db, id, &(school->n_periods_per_day), school);
		select_all_periods_by_school_id(stdout, db, id, &(school->n_periods), school);
		select_all_subject_groups_by_school_id(stdout, db, id, &(school->n_subject_groups), school);
		select_all_subjects_by_school_id(stdout,db,id,&(school->n_subjects), school);
		school->rooms = select_all_rooms_by_school_id(stdout, db, id, &(school->n_rooms), school);
		school->classes = select_all_classes_by_school_id(stdout, db, id, &(school->n_classes), school);
		school->teachers = select_all_teachers_by_school_id(stdout, db, id, &(school->n_teachers), school);
		school->teaches = select_all_teaches_by_school_id(stdout, db, id, &(school->n_teaches), school);
		school->assignments = select_all_assignments_by_school_id(stdout, db, id, &(school->n_assignments), school);
		link_all_teaches(school);
		link_all_assignments(school);
		select_all_meetings(stdout,db, school);
		select_all_solutions(stdout, db, school);
	}
	sqlite3_finalize(stmt);

	return school;
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

/*********************************************************/
/*                   REMOVE  FUNCTIONS                   */
/*********************************************************/
/* NOTE for logical deletes, try updates; 				 */

bool remove_solution(FILE * console_out, sqlite3* db, int id){
	return  !exec_and_check(db, DELETE_PLANNING_SOLUTION_BY_SOLUTION_ID, id)? false:
			!exec_and_check(db, DELETE_LECTURE_SOLUTION_BY_SOLUTION_ID, id)? false:
			!exec_and_check(db, DELETE_SOLUTION_BY_ID, id)? false:
			true;
}

bool remove_room(FILE * console_out, sqlite3* db, int id){
	return	!exec_and_check(db, UNSET_LECTURE_SOLUTION_ROOM_BY_ROOM_ID, id)?false:
			!exec_and_check(db, UNSET_PLANNING_SOLUTION_ROOM_ID, id)?false:
			!exec_and_check(db, DELETE_PLANNING_ROOM_BY_ROOM_ID, id)?false:
			!exec_and_check(db, DELETE_LECTURE_POSSIBLE_ROOM_BY_ROOM_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_ROOM_BY_ROOM_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_ROOM_BY_ROOM_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_ROOM_BY_ROOM_ID, id)?false:
			!exec_and_check(db, DELETE_ROOM_AVAILABILITY_BY_ROOM_ID, id)?false:
			!exec_and_check(db, DELETE_ROOM_BY_ID, id)?false:
			true;
}

bool remove_subject(FILE * console_out, sqlite3* db, int id){
	return  !exec_and_check(db,DELETE_LECTURE_SOLUTION_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_LECTURE_POSSIBLE_PERIOD_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_LECTURE_POSSIBLE_ROOM_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_POSSIBLE_TEACHER_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_LECTURE_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_TEACHES_TWIN_PREFERENCE_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_TEACHES_PERIOD_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_TEACHES_ROOM_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_TEACHES_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_CLASS_SUBJECT_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_SUBJECT_IN_GROUP_BY_SUBJECT_ID, id)?false:
			!exec_and_check(db,DELETE_SUBJECT_BY_ID, id)?false:
			true;
}

bool remove_subject_group(FILE * console_out, sqlite3* db, int id){
	return  !exec_and_check(db, DELETE_CLASS_SUBJECT_GROUP_BY_GROUP_ID, id)?false:
			!exec_and_check(db, DELETE_SUBJECT_IN_GROUP_BY_GROUP_ID, id)?false:
			!exec_and_check(db, DELETE_SUBJECT_GROUP_BY_ID, id)?false:
			true;
}

bool remove_teacher(FILE * console_out, sqlite3* db, int id) {
	return	!exec_and_check(db, DELETE_PLANNING_SOLUTION_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_PLANNING_ROOM_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_PLANNING_PERIOD_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_PLANNING_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, UNSET_LECTURE_TEACHER_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_POSSIBLE_TEACHER_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_TWIN_PREFERENCE_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_PERIOD_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_ROOM_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_ROOM_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_TWIN_PREFERENCE_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_ATTENDANCE_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_SUBORDINATION_BY_SUB_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_SUBORDINATION_BY_SUP_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_DAY_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_BY_ID, id)?false:
			true;
			// !exec_and_check(db, UNSET_MEETING_SOLUTION_TEACHER_BY_TEACHER_ID, id)?false:
}

/* TODO Check. */
bool remove_teaches(FILE * console_out, sqlite3* db, int id){
	return	!exec_and_check(db, DELETE_TEACHES_PERIOD_BY_TEACHES_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_TWIN_PREFERENCE_BY_TEACHES_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_PERIOD_BY_TEACHES_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_ROOM_BY_TEACHES_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_BY_ID, id)?false:
			true;
}

bool remove_class(FILE * console_out, sqlite3* db, int id) {
	return  !exec_and_check(db, DELETE_LECTURE_SOLUTION_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_LECTURE_POSSIBLE_PERIOD_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_LECTURE_POSSIBLE_ROOM_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_POSSIBLE_TEACHER_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_LECTURE_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBJECT_GROUP_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBJECT_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_ROOM_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBORDINATION_BY_SUB_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBORDINATION_BY_SUP_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_ATTENDANCE_BY_CLASS_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_BY_ID, id)?false:
			true;
}

bool remove_school(FILE * console_out, sqlite3 * db, int id){
	return	!exec_and_check(db, DELETE_PLANNING_SOLUTION_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_PLANNING_ROOM_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_PLANNING_PERIOD_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_PLANNING_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_LECTURE_SOLUTION_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_LECTURE_POSSIBLE_PERIOD_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_LECTURE_POSSIBLE_ROOM_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_POSSIBLE_TEACHER_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_LECTURE_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_SOLUTION_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_TWIN_PREFERENCE_BY_TEACHER_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_PERIOD_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_ROOM_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_ROOM_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_TWIN_PREFERENCE_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_ATTENDANCE_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_SUBORDINATION_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHES_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_DAY_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_TEACHER_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBJECT_GROUP_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBJECT_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_SUBJECT_IN_GROUP_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_SUBJECT_GROUP_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_SUBJECT_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_ROOM_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_SUBORDINATION_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_ATTENDANCE_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_CLASS_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_ROOM_AVAILABILITY_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_ROOM_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_PERIOD_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_DAY_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_DAILY_PERIOD_BY_SCHOOL_ID, id)?false:
			!exec_and_check(db, DELETE_SCHOOL_BY_ID, id)?false:
			true;
}

/*********************************************************/
/*                 EXTERNAL ABSTRACTIONS                 */
/*********************************************************/

bool update_school_name(FILE * console_out, sqlite3 * db, int id, char * name){
	sqlite3_stmt * stmt;
	int errc = sqlite3_prepare_v2(db, UPDATE_SCHOOL_NAME, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, id);
	errc = sqlite3_step(stmt);
	return (errc == SQLITE_DONE);
}

bool update_school_period_scores(FILE * console_out, sqlite3 * db, int n_periods, int * period_ids, int * scores){
	sqlite3_stmt * stmt;
	int errc = sqlite3_prepare_v2(db, UPDATE_PERIOD_OPERATES, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < n_periods; ++i){
		sqlite3_bind_int(stmt,1, scores[i]);
		sqlite3_bind_int(stmt,2, period_ids[i]);
		printf("Updating period_id %d with score %d\n", period_ids[i], scores[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	return true;
}

bool update_day_names(FILE * console_out, sqlite3 * db, int n_days, int * ids, char ** names) {
	sqlite3_stmt * stmt;
	int errc = sqlite3_prepare_v2(db, UPDATE_DAY_NAME, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < n_days; ++i){
		sqlite3_bind_text(stmt, 1, names[i], -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 2, ids[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	return true;
}

bool update_daily_period_names(FILE * console_out, sqlite3 * db, int n_periods, int * ids, char ** names) {
	sqlite3_stmt * stmt;
	int errc = sqlite3_prepare_v2(db, UPDATE_DAILY_PERIOD_NAME, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < n_periods; ++i){
		sqlite3_bind_text(stmt, 1, names[i], -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 2, ids[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	return true;
}

bool update_period_names(FILE * console_out, sqlite3 * db, int n_periods, int * ids, char ** names){
	sqlite3_stmt * stmt;
	int errc = sqlite3_prepare_v2(db, UPDATE_PERIOD_NAME, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < n_periods; ++i){
		sqlite3_bind_text(stmt, 1, names[i], -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 2, ids[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	return true;
}

bool update_room_basic_data(FILE * console_out, sqlite3* db, int room_id, Room * room, School * school){
	sqlite3_stmt * stmt;
	int errc = sqlite3_prepare_v2(db, UPDATE_TABLE_ROOM, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_text(stmt,1, room->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt,2, room->short_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,3, room->size);
	sqlite3_bind_int(stmt,4, room->active);
	sqlite3_bind_int(stmt,5, school->id);
	sqlite3_bind_int(stmt,6, room_id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(stmt);
	sqlite3_finalize(stmt);
	return true;
}

bool update_room_availability(FILE * console_out, sqlite3* db, int room_id, int * availability, School * school){
	return insert_or_update_period_scores(console_out, db, UPSERT_TABLE_ROOM_AVAILABILITY, room_id, availability, school);
}

bool update_subject_basic_data(FILE * console_out, sqlite3 * db, int subj_id, Subject * subject, School * school){
	sqlite3_stmt * stmt;
	int errc = sqlite3_prepare_v2(db, UPDATE_TABLE_SUBJECT, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_text(stmt, 1, subject->name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, subject->short_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 3, school->id);
	sqlite3_bind_int(stmt, 4, subj_id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(false);
	sqlite3_finalize(stmt);
	return true;
}

bool update_subject_group_name(FILE * console_out, sqlite3 * db, int sgr_id, char * new_name){
	LMH_ASSERT(db != NULL && sgr_id > 0 && new_name != NULL);
	sqlite3_stmt * stmt;
	int errc = sqlite3_prepare_v2(db, UPDATE_SUBJECT_GROUP_NAME, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, sgr_id);
	errc = sqlite3_step(stmt);
	CERTIFY_ERRC_SQLITE_DONE(stmt);
	sqlite3_finalize(stmt);
	return true;
}

bool update_subject_group_members(FILE * console_out, sqlite3 * db, int sgr_id, int * members, School * school){
	LMH_ASSERT(console_out != NULL && db != NULL && members != NULL && school != NULL && sgr_id > 0);
	sqlite3_stmt * stmt_insert, * stmt_remove;
	int errc;

	errc = sqlite3_prepare_v2(db, INSERT_TABLE_SUBJECT_IN_GROUP, -1, &stmt_insert, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	errc = sqlite3_prepare_v2(db, DELETE_SUBJECT_IN_GROUP_BY_SUBJECT_ID_GROUP_ID, -1, &stmt_remove, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < school->n_subjects; ++i){
		sqlite3_stmt * to_exec = (members[i] > 0)?(stmt_insert):(stmt_remove);
		sqlite3_bind_int(to_exec,1, school->subjects[i].id);
		sqlite3_bind_int(to_exec,2, sgr_id);
		errc = sqlite3_step(to_exec);
		sqlite3_reset(to_exec);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	sqlite3_finalize(stmt_insert);
	sqlite3_finalize(stmt_remove);
	return true;
}

bool update_room_teacher_score(FILE * console_out, sqlite3 * db, int room_id, int * plan_scr, int * lec_scr,  School * school){
	LMH_ASSERT(console_out != NULL && db != NULL && plan_scr != NULL && lec_scr != NULL && school != NULL && room_id > 0);
	sqlite3_stmt * stmt;
	int errc;

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_TEACHER_ROOM, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < school->n_teachers; ++i){
		sqlite3_bind_int(stmt, 1, school->teachers[i].id);
		sqlite3_bind_int(stmt, 2, room_id);
		sqlite3_bind_int(stmt, 3, lec_scr[i]);
		sqlite3_bind_int(stmt, 4, plan_scr[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	sqlite3_finalize(stmt);
	return true;
}

bool update_room_class_score(FILE * console_out, sqlite3 * db, int room_id, int * scores,  School * school){
	LMH_ASSERT(console_out != NULL && db != NULL && scores != NULL && school != NULL && room_id > 0);
	sqlite3_stmt * stmt;
	int errc;

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_CLASS_ROOM, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < school->n_classes; ++i){
		sqlite3_bind_int(stmt, 1, school->classes[i].id);
		sqlite3_bind_int(stmt, 2, room_id);
		sqlite3_bind_int(stmt, 3, scores[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	sqlite3_finalize(stmt);
	return true;
}

bool update_room_teaches_score(FILE * console_out, sqlite3 * db, int room_id, int * scores, School * school){
	LMH_ASSERT(console_out != NULL && db != NULL && scores != NULL && school != NULL && room_id > 0);
	sqlite3_stmt * stmt;
	int errc;

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_TEACHES_ROOM, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < school->n_teaches; ++i){
		sqlite3_bind_int(stmt, 1, school->teaches[i].id);
		sqlite3_bind_int(stmt, 2, room_id);
		sqlite3_bind_int(stmt, 3, scores[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	sqlite3_finalize(stmt);
	return true;
}

bool update_room_meeting_score(FILE * console_out, sqlite3 * db, int room_id, int * scores, School * school){
	LMH_ASSERT(console_out != NULL && db != NULL && scores != NULL && school != NULL && room_id > 0);
	sqlite3_stmt * stmt_lec, * stmt_plan;
	int errc;

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_LECTURE_POSSIBLE_ROOM, -1, &stmt_lec, NULL);
	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_PLANNING_ROOM, -1, &stmt_plan, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < school->n_meetings; ++i){
		sqlite3_stmt * stmt = school->meetings[i].type == meet_LECTURE ? stmt_lec :
								school->meetings[i].type == meet_PLANNING ? stmt_plan :
								NULL;
		LMH_ASSERT(stmt != NULL);
		sqlite3_bind_int(stmt, 1, school->teaches[i].id);
		sqlite3_bind_int(stmt, 2, room_id);
		sqlite3_bind_int(stmt, 3, scores[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);

	}
	sqlite3_finalize(stmt_plan);
	sqlite3_finalize(stmt_lec);
	return true;
}
bool update_teacher_assignment_score(FILE * console_out, sqlite3 * db, int teacher_id, int * scores, School * school){
	LMH_ASSERT(console_out != NULL && db != NULL && scores != NULL && school != NULL && teacher_id > 0);
	sqlite3_stmt * stmt;
	int errc;

	errc = sqlite3_prepare_v2(db, UPSERT_TABLE_POSSIBLE_TEACHER, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < school->n_assignments; ++i){
		//id_classsubject, id_teacher, score
		sqlite3_bind_int(stmt, 1, school->assignments[i].id);
		sqlite3_bind_int(stmt, 2, teacher_id);
		sqlite3_bind_int(stmt, 3, scores[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	sqlite3_finalize(stmt);
	return true;
}
// Seems to be dead code.
// bool update_teacher_meeting_fixation(FILE * console_out, sqlite3 * db, int teacher_id, bool * fixed, School * school){
// 	LMH_ASSERT(console_out != NULL && db != NULL && fixed != NULL && school != NULL && teacher_id > 0);
// 	sqlite3_stmt * stmt;
// 	int errc;
//
// 	errc = sqlite3_prepare_v2(db, UPDATE_CLASSSUBJECT_TEACHER_BY_LECTURE_ID, -1, &stmt, NULL);
// 	CERTIFY_ERRC_SQLITE_OK(false);
//
//  	for(int i = 0; i < school->n_meetings; ++i){
// 		if(school->meetings[i].type == meet_LECTURE){
// 			sqlite3_bind_int(stmt, 1, school->meetings[i].id);
// 			if(fixed[i]){
// 				sqlite3_bind_int(stmt, 2, teacher_id);
// 			} else {
// 				sqlite3_bind_null(stmt,2);
// 			}
// 			errc = sqlite3_step(stmt);
// 			CERTIFY_ERRC_SQLITE_DONE(false);
// 		}
// 	}
// 	sqlite3_finalize(stmt);
// 	return true;
// }

bool update_teacher_day_max_per(FILE * console_out, sqlite3 * db, int id_teacher, int * max_per, School * school){
	LMH_ASSERT(console_out != NULL && db != NULL && max_per != NULL && school != NULL && id_teacher > 0);
	sqlite3_stmt * stmt;
	int errc;

	errc = sqlite3_prepare_v2(db, UPDATE_TEACHER_DAY_MAX_PER, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < school->n_days; ++i){
		sqlite3_bind_int(stmt, 1, id_teacher);
		sqlite3_bind_int(stmt, 2, school->day_ids[i]);
		sqlite3_bind_int(stmt, 3, max_per[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	sqlite3_finalize(stmt);
	return true;
}

bool update_teacher_twin_preference(FILE * console_out, sqlite3 * db, int id_teacher, int * twinning){
	return insert_or_update_twin_scores(console_out, db, UPSERT_TABLE_TEACHER_TWIN_PREFERENCE, id_teacher, twinning);
}


bool update_teacher_lecture_room_preference(FILE * console_out, sqlite3 * db, int id_teacher, int * scores, School * school){
	LMH_ASSERT(console_out != NULL && db != NULL && scores != NULL && school != NULL && id_teacher > 0);
	sqlite3_stmt * stmt;
	int errc;

	errc = sqlite3_prepare_v2(db, UPSERT_TEACHER_SCORE_LECTURE, -1, &stmt, NULL);
	CERTIFY_ERRC_SQLITE_OK(false);
	for(int i = 0; i < school->n_rooms; ++i){
		sqlite3_bind_int(stmt, 1, id_teacher);
		sqlite3_bind_int(stmt, 2, school->rooms[i].id);
		sqlite3_bind_int(stmt, 3, scores[i]);
		errc = sqlite3_step(stmt);
		sqlite3_reset(stmt);
		CERTIFY_ERRC_SQLITE_DONE(false);
	}
	sqlite3_finalize(stmt);
	return true;

}
