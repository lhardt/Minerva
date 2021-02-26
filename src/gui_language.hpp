/*
 * Project Minerva.
 *   gui_language.hpp - translation tables for the UI.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains lists of strings used to translate the program
 * between languages.
 */
#ifndef GUI_LANGUAGE
#define GUI_LANGUAGE

typedef struct Language {
	const wchar_t * const str_minerva_school_timetables;
	const wchar_t * const str_minerva_welcome_description;
	const wchar_t * const str_system_config;
	const wchar_t * const str_language;
	const wchar_t * const str_settings;
	const wchar_t * const str_save;
	const wchar_t * const str_save_as;
	const wchar_t * const str_close_and_save;
	const wchar_t * const str_close_without_saving;
	const wchar_t * const str_name;
	const wchar_t * const str_edit;
	const wchar_t * const str_size;
	const wchar_t * const str_back;
	const wchar_t * const str_font_sz;
	const wchar_t * const str_small_font;
	const wchar_t * const str_medium_font;
	const wchar_t * const str_large_font;
	const wchar_t * const str_open;
	const wchar_t * const str_help;
	const wchar_t * const str_create;
	const wchar_t * const str_copyright_notice;
	const wchar_t * const str_open_manual;
	const wchar_t * const str_school_details;
	/* Create & view school */
	const wchar_t * const str_create_school;
	const wchar_t * const str_school_name;
	const wchar_t * const str_number_of_days_per_cycle;
	const wchar_t * const str_number_of_periods_per_day;
	const wchar_t * const str_in_what_periods_the_school_is_open;
	const wchar_t * const str_create_school_manual_entry;
	/* Header names */
	const wchar_t * const str_school;
	const wchar_t * const str_periods;
	const wchar_t * const str_features;
	const wchar_t * const str_rooms;
	const wchar_t * const str_subjects;
	const wchar_t * const str_teachers;
	const wchar_t * const str_classes;
	const wchar_t * const str_lectures;
	const wchar_t * const str_events;
	const wchar_t * const str_timetable;
	/* Subheader names */
	const wchar_t * const str_file;
	const wchar_t * const str_data;
	const wchar_t * const str_view;
	const wchar_t * const str_add;
	const wchar_t * const str_preferences;
	const wchar_t * const str_check;
	/* Listing */
	const wchar_t * const str_list_features;
	const wchar_t * const str_list_of_features;
	const wchar_t * const str_list_rooms;
	const wchar_t * const str_list_of_rooms;
	const wchar_t * const str_list_subjects;
	const wchar_t * const str_list_of_subjects;
	const wchar_t * const str_list_subject_groups;
	const wchar_t * const str_list_of_subject_groups;
	const wchar_t * const str_list_teachers;
	const wchar_t * const str_list_of_teachers;
	const wchar_t * const str_list_teacher_groups;
	const wchar_t * const str_list_of_teacher_groups;
	const wchar_t * const str_list_classes;
	const wchar_t * const str_list_of_classes;
	const wchar_t * const str_list_class_groups;
	const wchar_t * const str_list_of_class_groups;
	const wchar_t * const str_list_lectures;
	const wchar_t * const str_list_of_lectures;
	const wchar_t * const str_list_planning_times;
	const wchar_t * const str_list_of_planning_times;
	/* Adding */
	const wchar_t * const str_add_feature;
	const wchar_t * const str_add_room;
	const wchar_t * const str_add_subject;
	const wchar_t * const str_add_subject_group;
	const wchar_t * const str_add_teacher;
	const wchar_t * const str_add_teacher_group;
	const wchar_t * const str_add_class;
	const wchar_t * const str_add_class_group;
	/* Possible Preferences */
	const wchar_t * const str_period_preference;
	const wchar_t * const str_subject_preference;
	const wchar_t * const str_teacher_preference;
	const wchar_t * const str_class_preference;
	const wchar_t * const str_twin_preference;
	const wchar_t * const str_room_preference;
	/* Checking */
	const wchar_t * const str_check_all;
	/* General-Purpouse / Group Related */
	const wchar_t * const str_add_classes_to_the_group;
	const wchar_t * const str_add_class_to_the_group;
	const wchar_t * const str_group_name;
	const wchar_t * const str_remove_all;
	const wchar_t * const str_remove;
	const wchar_t * const str_add_group;

	const wchar_t * const str_adj__open;
	const wchar_t * const str_adj__closed;

	const wchar_t * const str_day;
	const wchar_t * const str_period;
	const wchar_t * const str_delete;

	const wchar_t * const str_are_you_sure;
	const wchar_t * const str_school_deletion_popup_text;

	const wchar_t * const str_error;
	const wchar_t * const str_could_not_open_db_error;

	const wchar_t * const str_yes;
	const wchar_t * const str_no;

	const wchar_t * const str_fill_the_form_correctly;
	const wchar_t * const str_success;
	const wchar_t * const str_could_not_insert_on_db;

	const wchar_t * const str_list_lectures_by_class;
	const wchar_t * const str_add_lectures_by_class;
	const wchar_t * const str_see_timetable;
	const wchar_t * const str_export_timetable;
	const wchar_t * const str_generate_timetable;
	const wchar_t * const str_create_timetable_manually;

	const wchar_t * const str_class_name;
	const wchar_t * const str_class_size;
	const wchar_t * const str_class_entry_period;
	const wchar_t * const str_class_exit_period;
	const wchar_t * const str_class_availability;
	const wchar_t * const str_class_subjects;

	const wchar_t * const str_class_can_have_free_periods;
	const wchar_t * const str_class_available;
	const wchar_t * const str_class_unavailable;

	const wchar_t * const str_room_size_in_students;
	const wchar_t * const str_room_availability;
	const wchar_t * const str_room_add_features_with_scores;

	const wchar_t * const str_add_subjects_to_the_group;
	const wchar_t * const str_add_teachers_to_the_group;

	const wchar_t * const str_teacher_availability;
	const wchar_t * const str_teacher_available;
	const wchar_t * const str_teacher_unavailable;
	const wchar_t * const str_teacher_teaches;

	const wchar_t * const str_choose_a_timetable;
	const wchar_t * const str_timetable__none;
	const wchar_t * const str_class__none;
	const wchar_t * const str_teacher__none;
	const wchar_t * const str_subject__none;
	const wchar_t * const str_room__none;
	const wchar_t * const str_room;
	const wchar_t * const str_lecture_desc;
	const wchar_t * const str_detail;

	const wchar_t * const str_desc_timetable;
	const wchar_t * const str_class;
	const wchar_t * const str_teacher;
	const wchar_t * const str_subject;

	const wchar_t * const str_generated_timetables;
	const wchar_t * const str_generate_timetable_text;
	const wchar_t * const str_last_generated_timetables;
	const wchar_t * const str_generate;
	const wchar_t * const str_generating;
	const wchar_t * const str_could_not_generate;
	const wchar_t * const str_close;
	const wchar_t * const str_undo;
	const wchar_t * const str_redo;

	const wchar_t * const str_cancel;
	const wchar_t * const str_confirm_close_without_saving;
	const wchar_t * const str_subjects_in_the_group;

	const wchar_t * const str_max_number_of_days;
	const wchar_t * const str_max_number_of_periods;
	const wchar_t * const str_max_number_of_periods_per_day;
	const wchar_t * const str_number_of_planning_periods;
	const wchar_t * const str_teachers_teach;

	const wchar_t * const str_group_available;
	const wchar_t * const str_group_unavailable;
	const wchar_t * const str_group_availability;
	const wchar_t * const str_group_members;

	const wchar_t * const str_score_0;
	const wchar_t * const str_score_1;
	const wchar_t * const str_score_2;
	const wchar_t * const str_score_3;
	const wchar_t * const str_score_4;

	const wchar_t * const str_duplicate;
	const wchar_t * const str_lorem;
	const wchar_t * const str_amount;
	const wchar_t * const str_belongs;
	const wchar_t * const str_does_not_belong;
	const wchar_t * const str_to_the_group;
	const wchar_t * const str_teaches;

	const wchar_t * const str_day_names;
	const wchar_t * const str_daily_period_names;
	const wchar_t * const str_period_names;
	const wchar_t * const str_statistics;

	const wchar_t * const str_number_of_classes;
	const wchar_t * const str_number_of_teachers;
	const wchar_t * const str_number_of_rooms;
	const wchar_t * const str_number_of_subjects;
	const wchar_t * const str_number_of_subject_groups;
	const wchar_t * const str_basic_data;
	const wchar_t * const str_actions;

	const wchar_t * const str_free_periods;
	const wchar_t * const str_entry_period;
	const wchar_t * const str_exit_period;
	const wchar_t * const str_active;
	const wchar_t * const str_availability;
	const wchar_t * const str_subject_groups;
	const wchar_t * const str_data_to_fix;
	const wchar_t * const str_occurence;
	const wchar_t * const str_needs_room;
	const wchar_t * const str_days;
	const wchar_t * const str_planning_time_twinning;
	const wchar_t * const str_lecture_rooms;
	const wchar_t * const str_planning_rooms;
	const wchar_t * const str_teacher_groups;
	const wchar_t * const str_list_of_teachers_and_their_groups;
	const wchar_t * const str_list_teachers_and_their_groups;
	const wchar_t * const str_group;
	const wchar_t * const str_dependency;
	const wchar_t * const str_list_of_classes_and_their_groups;
	const wchar_t * const str_list_classes_and_their_groups;
	const wchar_t * const str_is_superclass;
	const wchar_t * const str_search;
	const wchar_t * const str_selection;
	const wchar_t * const str_clear;
	const wchar_t * const str_selection_and_search;
	const wchar_t * const str_member;
	const wchar_t * const str_periods_school_open;
	const wchar_t * const str_subjects_required_by_the_class;
	const wchar_t * const str_participation_in_class_groups;
	const wchar_t * const str_max_periods_per_day_subject_group;
	const wchar_t * const str_members;
	const wchar_t * const str_repeated_name_error;

	const wchar_t * const str_planning_periods;
	const wchar_t * const str_lecture_periods;
} Language;

const extern Language LANG_POR;

const extern Language LANG_ENG;

const extern Language LANG_SPA;

const extern Language LANG_DEU;

#endif /* GUI_LANGUAGE */
