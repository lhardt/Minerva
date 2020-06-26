#include "gui_language.hpp"


const Language LANG_POR = {
	.str_minerva_school_timetables = L"Horário Escolar Minerva",
	.str_minerva_welcome_description = L"Comece agora a criar seu próprio horário escolar.\nSobre qual escola estamos falando hoje?",
	.str_system_config = L"Configurações do Sistema",
	.str_language = L"Linguagem",
	.str_settings = L"Configurações",
	.str_save = L"Salvar",
	.str_save_as = L"Salvar como",
	.str_close_and_save = L"Sair e salvar",
	.str_close_without_saving = L"Sair sem salvar",
	.str_back = L"Voltar",
	.str_font_sz= L"Tamanho da Fonte",
	.str_small_font=L"Pequena",
	.str_medium_font=L"Média",
	.str_large_font=L"Grande",
	.str_open=L"Abrir",
	.str_help=L"Ajuda",
	.str_create=L"Criar",
	.str_copyright_notice=L"Copyright (C) Léo Hardt 2020. Licença a ser definida.",
	.str_open_manual=L"Abrir Manual",
	.str_school_data=L"Dados da Escola",
	/* Create & view school */
	.str_create_school=L"Cadastrar Escola",
	.str_school_name=L"Nome da Escola",
	.str_number_of_days_per_cycle=L"Número de Dias por Ciclo",
	.str_number_of_periods_per_day=L"Número de Períodos por dia",
	.str_in_what_periods_the_school_is_open=L"Em que períodos a Escola está Aberta?",
	.str_create_school_manual_entry=L"Seja bem vindo ao software Minerva. Agradecemos por testá-lo e pedimos o favor que se encontrares algum erro, "
		"faça a gentileza de entrar em contato.\n\n"
		"Em caso de dúvidas, o software dispõe de um manual, esclarecendo o uso de suas funções.\n\n"
		"Para cadastrar uma escola, preencha os campos ao lado. Pense em cada \"escola\" aqui como uma entidade que "
		"precisa de um cronograma diferente. Esse nome será usado na tela inicial e no horário. Recomendamos um nome "
		"curto, como \"Escola Dom Pedro - Noite.\"\n\n"
		"O tamanho do ciclo é tamanho do horário, em dias. Geralmente são cinco dias. Se a escola só funcionar "
		"em três dias da semana, são três dias no ciclo. Se o horário será bisemanal, são 10 dias. E assim por diante. "
		"Caso tenha dificuldade para decidir, pense \"quantos dias demora para o horário voltar ao início?\".\n\n"
		"Por último, clique em qualquer período em que a escola não estiver aberta para fazer com que ela esteja fechada. Algumas escolas, por "
		"exemplo, ficam fechadas no 5º período da sexta-feira. Para fazer com que a escola volte a estar aberta, clique novamente.",
	/* Header names */
	.str_school=L"Escola",
	.str_rooms=L"Salas",
	.str_subjects=L"Disciplinas",
	.str_teachers=L"Professores",
	.str_classes=L"Turmas",
	.str_lectures=L"Aulas",
	.str_timetable=L"Horário",
	/* Subheader names */
	.str_file=L"Arquivo",
	.str_data=L"Dados",
	.str_view=L"Visualização",
	.str_add=L"Cadastro",
	.str_preferences=L"Preferências",
	.str_check=L"Conferência",
	/* Listing */
	.str_list_features=L"Listar Características",
	.str_list_rooms=L"Listar Salas",
	.str_list_subjects=L"Listar Disciplinas",
	.str_list_subject_groups=L"Listar Grupos de Disciplinas",
	.str_list_teachers=L"Listar Professores",
	.str_list_teacher_groups=L"Listar Grupos de Professores",
	.str_list_classes=L"Listar Turmas",
	.str_list_class_groups=L"Listar Grupos de Turmas",
	/* Adding */
	.str_add_feature=L"Adicionar Característica",
	.str_add_room=L"Adicionar Salas",
	.str_add_subject=L"Adicionar Disciplina",
	.str_add_subject_group=L"Adicionar Grupo de Disciplinas",
	.str_add_teacher=L"Adicionar Professor",
	.str_add_teacher_group=L"Adicionar Grupo de Professores",
	.str_add_class=L"Adicionar Turma",
	.str_add_class_group=L"Adicionar Grupo de Turmas",
	/* Possible Preferences */
	.str_period_preference=L"Preferência por Período",
	.str_subject_preference=L"Preferência por Disciplina",
	.str_class_preference=L"Preferência por Turma",
	.str_twin_preference=L"Preferência por Geminação",
	.str_room_preference=L"Preferência por Sala",
	/* Checking */
	.str_check_all=L"Conferir tudo"
};

const Language LANG_ENG = {
	.str_minerva_school_timetables = L"Minerva School Timetables",
	.str_minerva_welcome_description = L"Start creating your own timetable now.\nAbout what school are we talking today?",
	.str_system_config = L"System Settings",
	.str_language = L"Language",
	.str_settings = L"Settings",
	.str_save = L"Save",
	.str_save_as = L"Save as",
	.str_close_and_save = L"Close and save",
	.str_close_without_saving = L"Close without saving",
	.str_back = L"Back",
	.str_font_sz= L"Font size",
	.str_small_font=L"Small",
	.str_medium_font=L"Medium",
	.str_large_font=L"Large",
	.str_open=L"Open",
	.str_help=L"Help",
	.str_create=L"Create",
	.str_copyright_notice=L"Copyright (C) Léo Hardt 2020. License to be defined.",
	.str_open_manual=L"Open Manual",
	.str_school_data=L"School data",
	/* Create & view school */
	.str_create_school=L"Create school",
	.str_school_name=L"School name",
	.str_number_of_days_per_cycle=L"Number of days per cycle",
	.str_number_of_periods_per_day=L"Number of periods per day",
	.str_in_what_periods_the_school_is_open=L"In what periods is the school open?",
	.str_create_school_manual_entry=L"Welcome to the Minerva software. We thank you for testing it and ask that if you find any error, "
		"please come into contact.\n\n"
		"When in doubt, this program has a manual, explaining the usage of its functions.\n\n"
		"To create a school, fill the form at the side. See each \"school\" as an entity that requieres a different timetable. "
		"Its name will be shown in the initial screen and on the timetable. We recommend a short name, such as "
		" \"Oak St School - Morning\"\n\n"
		"The cycle size is the size of the timetable, in days. Normally there are five days. If the school only operates "
		"three days a week, it's three days in the cycle. If the timetable is bi-weekly, then 10 days are needed. And so on. "
		"If you are having difficulty to decide, think \"how many days does it take for the timetable to repeat itself?\".\n\n"
		"At last, click in any period in which the school is not open to mark it as closed. Some schools, for example, "
		"don't open at the last period of friday. To mark the period as open, just click again.",
	/* Header names */
	.str_school=L"School",
	.str_rooms=L"Rooms",
	.str_subjects=L"Subjects",
	.str_teachers=L"Teachers",
	.str_classes=L"Classes",
	.str_lectures=L"Lectures",
	.str_timetable=L"Timetable",
	/* Subheader names */
	.str_file=L"File",
	.str_data=L"Data",
	.str_view=L"View",
	.str_add=L"Add",
	.str_preferences=L"Preferences",
	.str_check=L"Check",
	/* Listing */
	.str_list_features=L"List features",
	.str_list_rooms=L"List rooms",
	.str_list_subjects=L"List subjects",
	.str_list_subject_groups=L"List subject groups",
	.str_list_teachers=L"List teachers",
	.str_list_teacher_groups=L"List teacher Groups",
	.str_list_classes=L"List classes",
	.str_list_class_groups=L"List class groups",
	/* Adding */
	.str_add_feature=L"Add feature",
	.str_add_room=L"Add room",
	.str_add_subject=L"Add subject",
	.str_add_subject_group=L"Add subject group",
	.str_add_teacher=L"Add teacher",
	.str_add_teacher_group=L"Add teacher group",
	.str_add_class=L"Add class",
	.str_add_class_group=L"Add class group",
	/* Possible Preferences */
	.str_period_preference=L"Period Preference",
	.str_subject_preference=L"Subject Preference",
	.str_class_preference=L"Class Preference",
	.str_twin_preference=L"Double Period Preference",
	.str_room_preference=L"Room Preference",
	/* Checking */
	.str_check_all=L"Check all"
};

const Language LANG_SPA = {
	.str_minerva_school_timetables = L"Horario Escolar Minerva",
	.str_minerva_welcome_description = L"Cria ahora tu próprio horario escolar.\nSobre cual escuela falamos hoy?",
	.str_system_config = L"Ajustes del Sistema",
	.str_language = L"Lenguaje",
	.str_settings = L"Ajustes",
	.str_save = L"Guardar",
	.str_save_as = L"Guardar como",
	.str_close_and_save = L"Guardar y salir",
	.str_close_without_saving = L"Guardar sin salir",
	.str_back = L"(?)",
	.str_font_sz= L"Tamaño de Fuente",
	.str_small_font=L"Pequeña",
	.str_medium_font=L"Mediana",
	.str_large_font=L"Grande",
	.str_open=L"Abrir",
	.str_help=L"Ayuda",
	.str_create=L"Crear",
	.str_copyright_notice=L"Copyright (C) Léo Hardt 2020. Licencia a ser definida.",
	.str_open_manual=L"Abrir el manual",
	.str_school_data=L"Datos de la escuela",
	/* Create & view school */
	.str_create_school=L"Crear escuela",
	.str_school_name=L"Nombre de la escuela",
	.str_number_of_days_per_cycle=L"Número de días en un ciclo",
	.str_number_of_periods_per_day=L"Número de períodos por día ",
	.str_in_what_periods_the_school_is_open=L"En que períodos la escuela está abierta?",
	.str_create_school_manual_entry=L"Texto Grande; Texto Grande; Texto Grande; Texto Grande; Texto Grande; ",
	/* Header names */
	.str_school=L"Escuela",
	.str_rooms=L"Salas",
	.str_subjects=L"Asignaturas",
	.str_teachers=L"Maestros",
	.str_classes=L"Turmas",
	.str_lectures=L"Lección",
	.str_timetable=L"Horario",
	/* Subheader names */
	.str_file=L"Arquivo",
	.str_data=L"Datos",
	.str_view=L"Ver",
	.str_add=L"Añadir",
	.str_preferences=L"Preferencias",
	.str_check=L"Confirmar",
	/* Listing */
	.str_list_features=L"Listar Características",
	.str_list_rooms=L"Listar Salas",
	.str_list_subjects=L"Listar Asignaturas",
	.str_list_subject_groups=L"Listar Grupos de Asignaturas",
	.str_list_teachers=L"Listar Maestros",
	.str_list_teacher_groups=L"Listar Grupos de Maestros",
	.str_list_classes=L"Listar Clases",
	.str_list_class_groups=L"Listar Grupos de Clases",
	/* Adding */
	.str_add_feature=L"Añadir Característica",
	.str_add_room=L"Añadir Salas",
	.str_add_subject=L"Añadir Asignaturas",
	.str_add_subject_group=L"Añadir Grupo de Asignaturas",
	.str_add_teacher=L"Añadir Maestro",
	.str_add_teacher_group=L"Añadir Grupo de Maestros",
	.str_add_class=L"Añadir Clase",
	.str_add_class_group=L"Añadir Grupo de Clases",
	/* Possible Preferences */
	.str_period_preference=L"?",
	.str_subject_preference=L"?",
	.str_class_preference=L"?",
	.str_twin_preference=L"?",
	.str_room_preference=L"?",
	/* Checking */
	.str_check_all=L"?"
};

const Language LANG_DEU = {
	.str_minerva_school_timetables = L"Minerva Stundenplan",
	.str_minerva_welcome_description = L"Beginnen Sie zu seine eigene Stundenplan machen.\nÜber welcher Schule sprechen wir heute?",
	.str_system_config = L"Systemeinstellungen",
	.str_language = L"Sprache",
	.str_settings = L"Einstellungen",
	.str_save = L"Speichern",
	.str_save_as = L"Speichern als",
	.str_close_and_save = L"Speichern und Schließen",
	.str_close_without_saving = L"Schließen ohne zu Speichern",
	.str_back = L"(?)",
	.str_font_sz= L"Schriftgröße",
	.str_small_font=L"Kleine",
	.str_medium_font=L"Mittlere",
	.str_large_font=L"Große",
	.str_open=L"Offenen",
	.str_help=L"Hilfe",
	.str_create=L"Erstellen",
	.str_copyright_notice=L"Copyright (C) Léo Hardt 2020. Lizenz definiert werden.",
	.str_open_manual=L"Handbuch offenen",
	.str_school_data=L"Schulendatei",
	/* Create & view school */
	.str_create_school=L"Schule Erstellen",
	.str_school_name=L"Name der Schule",
	.str_number_of_days_per_cycle=L"Anzahl Tage pro Zyklus",
	.str_number_of_periods_per_day=L"Anzahl Zeiträume pro Tag",
	.str_in_what_periods_the_school_is_open=L"In welche Schulstunde ist die Schule geöffenet?",
	.str_create_school_manual_entry=L"Grosses Text; Grosses Text; Grosses Text; Grosses Text; Grosses Text;",
	/* Header names */
	.str_school=L"Schule",
	.str_rooms=L"Klassenzimmer",
	.str_subjects=L"Fache",
	.str_teachers=L"Lehrer",
	.str_classes=L"Klassen",
	.str_lectures=L"Vorträge",
	.str_timetable=L"Zeitplan",
	/* Subheader names */
	.str_file=L"Datei",
	.str_data=L"Daten",
	.str_view=L"Ansehen",
	.str_add=L"Hinzufügen",
	.str_preferences=L"Präferenzen",
	.str_check=L"Prüfen",
	/* Listing */
	.str_list_features=L"Merkmale auflisten",
	.str_list_rooms=L"Klassenzimmer auflisten",
	.str_list_subjects=L"Fache auflisten",
	.str_list_subject_groups=L"Fächergruppen auflisten",
	.str_list_teachers=L"Lehrer auflisten",
	.str_list_teacher_groups=L"Lehrergruppen auflisten",
	.str_list_classes=L"Klassen auflisten",
	.str_list_class_groups=L"Klassengruppen auflisten",
	/* Adding */
	.str_add_feature=L"Merkmal hinzufügen",
	.str_add_room=L"Klassenzimmer hinzufügen",
	.str_add_subject=L"Fach hinzufügen",
	.str_add_subject_group=L"Fächergruppe hinzufügen",
	.str_add_teacher=L"Lehrer hinzufügen",
	.str_add_teacher_group=L"Lehrergruppe hinzufügen",
	.str_add_class=L"Klass hinzufügen",
	.str_add_class_group=L"Klassengruppe hinzufügen",
	/* Possible Preferences */
	.str_period_preference=L"?",
	.str_subject_preference=L"?",
	.str_class_preference=L"?",
	.str_twin_preference=L"?",
	.str_room_preference=L"?",
	/* Checking */
	.str_check_all=L"?"
};
