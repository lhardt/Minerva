#ifndef GUI_LANGUAGE
#define GUI_LANGUAGE


/* Language per se */
typedef struct Language {
	const wchar_t * const str_system_config;
	const wchar_t * const str_language;
	const wchar_t * const str_settings;
	const wchar_t * const str_save;
	const wchar_t * const str_back;
} Language;

const Language LANG_POR = {
	.str_system_config = L"Configurações do Sistema",
	.str_language = L"Linguagem",
	.str_settings = L"Configurações",
	.str_save = L"Salvar",
	.str_back = L"Voltar"
};

const Language LANG_ENG = {
	.str_system_config = L"System Settings",
	.str_language = L"Language",
	.str_settings = L"Settings",
	.str_save = L"Save",
	.str_back = L"Back"
};

const Language LANG_SPA = {
	.str_system_config = L"Ajustes del Sistema",
	.str_language = L"Lenguaje",
	.str_settings = L"Ajustes",
	.str_save = L"Salvar",
	.str_back = L"(?)"
};

const Language LANG_DEU = {
	.str_system_config = L"Systemeinstellungen",
	.str_language = L"Sprache",
	.str_settings = L"Einsteillungen",
	.str_back = L"(?)"
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
