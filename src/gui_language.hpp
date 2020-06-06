#ifndef GUI_LANGUAGE
#define GUI_LANGUAGE


/* Language per se */
typedef struct Language {
	const char * const str_hello_world;
	const char * const str_language;
	const char * const str_settings;
	const char * const str_save;
	const char * const str_back;
} Language;

const Language LANG_POR = {
	.str_hello_world = "Olá, mundo!",
	.str_language = "Linguagem",
	.str_settings = "Configurações",
	.str_save = "Salvar",
	.str_back = "Voltar"
};

const Language LANG_ENG = {
	.str_hello_world = "Hello, world!",
	.str_language = "Language",
	.str_settings = "Settings",
	.str_save = "Save",
	.str_back = "Back"
};

const Language LANG_SPA = {
	.str_hello_world = "¡Hola, mundo!",
	.str_language = "Lenguaje",
	.str_settings = "Ajustes",
	.str_save = "Salvar",
	.str_back = "(?)"
};

const Language LANG_DEU = {
	.str_hello_world = "Hallo, Welt!",
	.str_language = "Sprache",
	.str_settings = "Einsteillungen",
	.str_back = "(?)"
};

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
