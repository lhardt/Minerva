#ifndef GUI_LANGUAGE
#define GUI_LANGUAGE


/* Language per se */
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
	const wchar_t * const str_school_data;
	/* Create & view school */
	const wchar_t * const str_create_school;
	const wchar_t * const str_school_name;
	const wchar_t * const str_number_of_days_per_cycle;
	const wchar_t * const str_number_of_periods_per_day;
	const wchar_t * const str_in_what_periods_the_school_is_open;
	const wchar_t * const str_create_school_manual_entry;
	/* Header names */
	const wchar_t * const str_school;
	const wchar_t * const str_rooms;
	const wchar_t * const str_subjects;
	const wchar_t * const str_teachers;
	const wchar_t * const str_classes;
	const wchar_t * const str_lectures;
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
	const wchar_t * const str_list_rooms;
	const wchar_t * const str_list_subjects;
	const wchar_t * const str_list_subject_groups;
	const wchar_t * const str_list_teachers;
	const wchar_t * const str_list_teacher_groups;
	const wchar_t * const str_list_classes;
	const wchar_t * const str_list_class_groups;
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

} Language;

const extern Language LANG_POR;

const extern Language LANG_ENG;

const extern Language LANG_SPA;

const extern Language LANG_DEU;

/* IDs of buttons. */

#define LH_ID_PADDING 10000

#define LHID_OF(i) (LH_ID_PADDING + (i))

#define LHN_SAVE_AS 				1
#define LHN_SAVE_AND_CLOSE 			2
#define LHN_CLOSE_WITHOUT_SAVE 		3
#define LHN_SCHOOL_DATA				4
#define LHN_OPEN_SCHOOL_MANUAL		5

#define LHN_SEE_FEATURES			6
#define LHN_SEE_ROOMS				7
#define LHN_ADD_FEATURE				8
#define LHN_ADD_ROOM				9
#define LHN_ROOM_PERIOD_PREF 		10
#define LHN_CHECK_ALL_ROOMS			11
#define LHN_OPEN_ROOMS_MANUAL		12

#define LHN_SEE_SUBJECTS			13
#define LHN_SEE_SUBJECT_GROUPS		14
#define LHN_ADD_SUBJECT				15
#define LHN_ADD_SUBJECT_GROUP		16
#define LHN_SUBJECT_PERIOD_PREF		17
#define LHN_SUBJECT_ROOM_PREF		19
#define LHN_CHECK_ALL_SUBJECTS		20
#define LHN_OPEN_SUBJECTS_MANUAL	21

#define LHN_SEE_TEACHERS			22
#define LHN_SEE_TEACHER_GROUPS		23
#define LHN_ADD_TEACHER				24
#define LHN_ADD_TEACHER_GROUP		25
#define LHN_TEACHER_SUBJECT_PREF	26
#define LHN_TEACHER_PERIOD_PREF		27
#define LHN_TEACHER_TWIN_PREF		28
#define LHN_TEACHER_ROOM_PREF		29
#define LHN_CHECK_ALL_TEACHERS		30
#define LHN_OPEN_TEACHERS_MANUAL	31

#define LHN_SEE_CLASSES				32
#define LHN_SEE_CLASS_GROUPS		33
#define LHN_ADD_CLASS				34
#define LHN_ADD_CLASS_GROUP			35
#define LHN_CLASS_PERIOD_PREF		36
#define LHN_CLASS_TEACHER_PREF		37
#define LHN_CLASS_ROOM_PREF			39
#define LHN_CHECK_ALL_CLASSES		40
#define LHN_OPEN_CLASSES_MANUAL 	41

#define LHN_SEE_LECTURES			42
#define LHN_ADD_LECTURES			43
#define LHN_LECTURE_PERIOD_PREF		44
#define LHN_LECTURE_TEACHER_PREF	45
#define LHN_LECTURE_TWIN_PREF		46
#define LHN_LECTURE_ROOM_PREF		47
#define LHN_CHECK_ALL_LECTURES		48
#define LHN_OPEN_LECTURES_MAUAL 	49

#define LHN_SEE_TIMETABLE			50
#define LHN_EXPORT_TIMETABLE		51
#define LHN_GENERATE_TIMETABLE		52
#define LHN_CREATE_TIMETABLE		53
#define LHN_OPEN_TIMETABLE_MANUAL	54



#endif /* GUI_LANGUAGE */
