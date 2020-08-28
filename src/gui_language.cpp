/*
 * Project Minerva.
 *   gui_language.cpp - translation tables for the UI.
 *
 * (C) 2019-2020 Léo Hardt <leom.hardt@gmail.com>.
 *
 * This program is free software. See LICENSE.
 *
 * This module contains structures related to
 * decision trees for school timetabling.
 */
#include "gui_language.hpp"

const Language LANG_POR = {
	.str_minerva_school_timetables = L"Horário Escolar Minerva",
	.str_minerva_welcome_description = L"Comece agora a criar o horário de sua escola gratuitamente.\nSobre qual escola estamos falando hoje?",
	.str_system_config = L"Configurações do Sistema",
	.str_language = L"Linguagem",
	.str_settings = L"Configurações",
	.str_save = L"Salvar",
	.str_save_as = L"Salvar como",
	.str_close_and_save = L"Sair e salvar",
	.str_close_without_saving = L"Sair sem salvar",
	.str_name = L"Nome",
	.str_edit = L"Editar",
	.str_size = L"Tamanho",
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
	.str_school_details=L"Detalhes da Escola",
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
	.str_periods=L"Períodos",
	.str_features=L"Características",
	.str_rooms=L"Salas",
	.str_subjects=L"Disciplinas",
	.str_teachers=L"Professores",
	.str_classes=L"Turmas",
	.str_lectures=L"Aulas",
	.str_events=L"Eventos",
	.str_timetable=L"Horário",
	/* Subheader names */
	.str_file=L"Arquivo",
	.str_data=L"Dados",
	.str_view=L"Visualização",
	.str_add=L"Cadastrar",
	.str_preferences=L"Preferências",
	.str_check=L"Conferência",
	/* Listing */
	.str_list_features=L"Listar Características",
	.str_list_of_features=L"Lista de Características",
	.str_list_rooms=L"Listar Salas",
	.str_list_of_rooms=L"Lista de Salas",
	.str_list_subjects=L"Listar Disciplinas",
	.str_list_of_subjects=L"Lista de Disciplinas",
	.str_list_subject_groups=L"Listar Grupos de Disciplinas",
	.str_list_of_subject_groups=L"Lista de Grupos de Disciplinas",
	.str_list_teachers=L"Listar Professores",
	.str_list_of_teachers=L"Lista de Professores",
	.str_list_teacher_groups=L"Listar Grupos de Professores",
	.str_list_of_teacher_groups=L"Lista de Grupos de Professores",
	.str_list_classes=L"Listar Turmas",
	.str_list_of_classes=L"Lista de Turmas",
	.str_list_class_groups=L"Listar Grupos de Turmas",
	.str_list_of_class_groups=L"Lista de Grupos de Turmas",
	.str_list_lectures=L"",
	.str_list_of_lectures=L"Lista de Aulas",
	.str_list_planning_times=L"Listar Horários de Planejamento",
	.str_list_of_planning_times=L"Lista de Horários de Planejamento",
	/* Adding */
	.str_add_feature=L"Adicionar Característica",
	.str_add_room=L"Adicionar Sala",
	.str_add_subject=L"Adicionar Disciplina",
	.str_add_subject_group=L"Adicionar Grupo de Disciplinas",
	.str_add_teacher=L"Adicionar Professor",
	.str_add_teacher_group=L"Adicionar Grupo de Professores",
	.str_add_class=L"Adicionar Turma",
	.str_add_class_group=L"Adicionar Grupo de Turmas",
	/* Possible Preferences */
	.str_period_preference=L"Preferência por Período",
	.str_subject_preference=L"Preferência por Disciplina",
	.str_teacher_preference=L"Preferência por Professor",
	.str_class_preference=L"Preferência por Turma",
	.str_twin_preference=L"Preferência por Geminação",
	.str_room_preference=L"Preferência por Sala",
	/* Checking */
	.str_check_all=L"Conferir tudo",
	/* General-Purpouse / Group Related */
	.str_add_classes_to_the_group=L"Adicionar Turmas ao Grupo",
	.str_add_class_to_the_group=L"Adicionar Turma",
	.str_group_name=L"Nome do Grupo",
	.str_remove_all=L"Remover Todos",
	.str_remove=L"Remover",
	.str_add_group=L"Adicionar Grupo",

	.str_adj__open = L"Aberta",
	.str_adj__closed = L"Fechada",

	.str_day = L"Dia",
	.str_period = L"Período",
	.str_delete = L"Deletar",

	.str_are_you_sure = L"Tem certeza?",
	.str_school_deletion_popup_text = L"Quer mesmo apagar essa escola e todos os dados inseridos?",

	.str_error=L"Erro",
	.str_could_not_open_db_error=L"Não foi possível abrir o banco de dados.",

	.str_yes=L"Sim",
	.str_no=L"Não",

	.str_fill_the_form_correctly=L"Preencha os campos corretamente",
	.str_success=L"Sucesso",
	.str_could_not_insert_on_db=L"Não foi possível inserir as informações no banco de dados",

	.str_list_lectures_by_class=L"Listar aulas por turma",
	.str_add_lectures_by_class=L"Adicionar aulas por turma",
	.str_see_timetable=L"Ver grade horária",
	.str_export_timetable=L"Exportar grade horária",
	.str_generate_timetable=L"Gerar horário",
	.str_create_timetable_manually=L"Criar horário manualmente",

	.str_class_name=L"Nome da turma",
	.str_class_size=L"Tamanho da turma",
	.str_class_entry_period=L"Período de entrada da turma",
	.str_class_exit_period=L"Período de saída da turma",
	.str_class_availibility=L"Disponibilidade da turma",
	.str_class_subjects=L"Disciplinas da Turma",
	.str_class_can_have_free_periods=L"Essa turma pode ter períodos livres",
	.str_class_available=L"Disponível",
	.str_class_unavailable=L"Indisponível",
	.str_room_size_in_students=L"Tamanho da sala, em alunos",
	.str_room_availibility=L"Disponibilidade da Sala",
	.str_room_add_features_with_scores=L"Adicione características à sala com suas \"notas\"",

	.str_add_subjects_to_the_group=L"Adicionar disciplinas ao grupo",

	.str_add_teachers_to_the_group=L"Adicione Professores ao Grupo",
	.str_teacher_availibility=L"Disponibilidade do Professor",
	.str_teacher_available=L"Disponível",
	.str_teacher_unavailable=L"Indisponível",
	.str_teacher_teaches=L"O professor leciona",

	.str_choose_a_timetable=L"Escolha uma Grade",
	.str_timetable__none=L"Nenhuma",
	.str_class__none=L"Nenhuma",
	.str_teacher__none=L"Nenhum",
	.str_subject__none=L"Nenhuma",
	.str_room__none=L"Nenhuma",
	.str_room=L"Sala",
	.str_lecture_desc=L"Descrição da Aula",
	.str_detail=L"Detalhar",

	.str_desc_timetable=L"Ver Grade",
	.str_class=L"Turma",
	.str_teacher=L"Professor",
	.str_subject=L"Disciplina",

	.str_generated_timetables=L"Grades Geradas",
	.str_generate_timetable_text=L"Aqui poderemos gerar novas grades horárias. Pode demorar um pouco, fique tranquilo.",
	.str_last_generated_timetables=L"Últimas Grades Geradas",
	.str_generate=L"Gerar",
	.str_generating=L"Gerando",
	.str_could_not_generate=L"Não foi possível gerar.",
	.str_close=L"Fechar",
	.str_undo=L"Desfazer",
	.str_redo=L"Refazer",
	.str_cancel=L"Cancelar",
	.str_confirm_close_without_saving=L"Deseja salvar antes de sair?",
	.str_subjects_in_the_group=L"Disciplinas no grupo",
	.str_max_number_of_days=L"N° máx de dias",
	.str_max_number_of_periods=L"N° máx de períodos",
	.str_max_number_of_periods_per_day=L"N° máx. de períodos por dia",
	.str_number_of_planning_periods=L"N° de períodos de planejamento",
	.str_teachers_teach=L"Os professores ensinam",

	.str_group_available=L"Disponível",
	.str_group_unavailable=L"Indisponível",
	.str_group_availibility=L"Disponibilidade",
	.str_group_members=L"Membros do Grupo",

	.str_score_0=L"Indisponível",
	.str_score_1=L"Em Último Caso",
	.str_score_2=L"Indiferente",
	.str_score_3=L"Prefere",
	.str_score_4=L"Adoraria",

	.str_duplicate=L"Duplicar",
	.str_lorem=L"Lorem ipsum dolor sit amet consecteur adiscipling elit.",
	.str_amount=L"Quantidade",
	.str_belongs=L"Pertence",
	.str_does_not_belong=L"Não Pertence",
	.str_to_the_group=L"Ao Grupo",
	.str_teaches=L"Ensina",
	.str_day_names=L"Nomes dos Dias",
	.str_daily_period_names=L"Nomes dos Períodos Diários",
	.str_period_names=L"Nomes dos Períodos",
	.str_statistics=L"Estatísticas",
	.str_number_of_classes=L"Número de Turmas",
	.str_number_of_teachers=L"Número de Professores",
	.str_number_of_rooms=L"Número de Salas",
	.str_number_of_subjects=L"Número de Disciplinas",
	.str_number_of_subject_groups=L"Número de Grupos de Disciplinas",
	.str_basic_data=L"Dados Básicos",
	.str_actions=L"Ações",
	.str_free_periods=L"Períodos Livres",
	.str_entry_period=L"Período de Entrada",
	.str_exit_period=L"Período de Saída",
	.str_active=L"Ativo",
	.str_availability=L"Disponibilidade",
	.str_subject_groups=L"Grupos de Disciplinas",
	.str_data_to_fix=L"Dados a Fixar",
	.str_occurence=L"Ocorrência",
	.str_needs_room=L"Precisa de Sala",
	.str_days=L"Dias",
	.str_planning_time_twinning=L"Geminação / Planejamento",
	.str_lecture_rooms=L"Salas / Aula",
	.str_planning_rooms=L"Salas / Planejamento",
	.str_teacher_groups=L"Grupos de Professores",
	.str_list_of_teachers_and_their_groups=L"Lista de Professores e seus Grupos",
	.str_list_teachers_and_their_groups=L"Listar Professores e seus Grupos",
	.str_group=L"Grupo",
	.str_dependency=L"Dependência",
	.str_list_of_classes_and_their_groups=L"Listar Turmas e seus Grupos",
	.str_list_classes_and_their_groups=L"Lista de Turmas e seus Grupos",
	.str_is_superclass=L"É turma composta",
};

const Language LANG_ENG = {
	.str_minerva_school_timetables = L"Minerva School Timetables",
	.str_minerva_welcome_description = L"Start creating the timetable of your school free of charge.\nAbout what school are we talking today?",
	.str_system_config = L"System Settings",
	.str_language = L"Language",
	.str_settings = L"Settings",
	.str_save = L"Save",
	.str_save_as = L"Save as",
	.str_close_and_save = L"Close and save",
	.str_close_without_saving = L"Close without saving",
	.str_name = L"Name",
	.str_edit = L"Edit",
	.str_size = L"Size",
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
	.str_school_details=L"School Details",
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
	.str_periods=L"Periods",
	.str_features=L"Features",
	.str_rooms=L"Rooms",
	.str_subjects=L"Subjects",
	.str_teachers=L"Teachers",
	.str_classes=L"Classes",
	.str_lectures=L"Lectures",
	.str_events=L"Events",
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
	.str_list_of_features=L"List of features",
	.str_list_rooms=L"List rooms",
	.str_list_of_rooms=L"List of rooms",
	.str_list_subjects=L"List subjects",
	.str_list_of_subjects=L"List of subjects",
	.str_list_subject_groups=L"List subject groups",
	.str_list_of_subject_groups=L"List of subject groups",
	.str_list_teachers=L"List Teachers",
	.str_list_of_teachers=L"List of Teachers",
	.str_list_teacher_groups=L"List Teacher Groups",
	.str_list_of_teacher_groups=L"List of Teacher Groups",
	.str_list_classes=L"List classes",
	.str_list_of_classes=L"List of classes",
	.str_list_class_groups=L"List class groups",
	.str_list_of_class_groups=L"List of class groups",
	.str_list_lectures=L"",
	.str_list_of_lectures=L"List of Lectures",
	.str_list_planning_times=L"List Planning Times",
	.str_list_of_planning_times=L"Lista of Planning Times",
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
	.str_teacher_preference=L"Teacher Preference",
	.str_class_preference=L"Class Preference",
	.str_twin_preference=L"Double Period Preference",
	.str_room_preference=L"Room Preference",
	/* Checking */
	.str_check_all=L"Check all",
	/* General-Purpouse / Group Related */
	.str_add_classes_to_the_group=L"Add Classes to the group",
	.str_add_class_to_the_group=L"Add class",
	.str_group_name=L"Group name",
	.str_remove_all=L"Remove All",
	.str_remove=L"Remove",
	.str_add_group=L"Add Group",

	.str_adj__open = L"Open",
	.str_adj__closed = L"Closed",

	.str_day = L"Day",
	.str_period = L"Period",
	.str_delete = L"Delete",

	.str_are_you_sure = L"Are you sure?",
	.str_school_deletion_popup_text = L"Do you really want to delete this school and all its data?",

	.str_error=L"Error",
	.str_could_not_open_db_error=L"Could not open the database file.",

	.str_yes=L"Yes",
	.str_no=L"No",

	.str_fill_the_form_correctly=L"Fill the form correctly",
	.str_success=L"Success",
	.str_could_not_insert_on_db=L"Could not insert the data onto the database.",

	.str_list_lectures_by_class=L"List lectures by class",
	.str_add_lectures_by_class=L"Add lectures by class",
	.str_see_timetable=L"See timetable",
	.str_export_timetable=L"Export timetable",
	.str_generate_timetable=L"Generate timetable",
	.str_create_timetable_manually=L"Create timetable manually",

	.str_class_name=L"Class' name",
	.str_class_size=L"Class' size",
	.str_class_entry_period=L"Class' entry period",
	.str_class_exit_period=L"Class' exit period",
	.str_class_availibility=L"Class' availibility",
	.str_class_subjects=L"Class' subjects",
	.str_class_can_have_free_periods=L"This class can have free periods",
	.str_class_available=L"Available",
	.str_class_unavailable=L"Unavailable",
	.str_room_size_in_students=L"Room size, in students",
	.str_room_availibility=L"Room availibility",
	.str_room_add_features_with_scores=L"Add features to the class with their \"scores\"",
	.str_add_subjects_to_the_group=L"Add subjects to the group",

	.str_add_teachers_to_the_group=L"Add teachers to the group",
	.str_teacher_availibility=L"Teacher Availibility",
	.str_teacher_available=L"Available",
	.str_teacher_unavailable=L"Unavailable",
	.str_teacher_teaches=L"The teacher teaches",

	.str_choose_a_timetable=L"Choose a timetable",
	.str_timetable__none=L"None",
	.str_class__none=L"None",
	.str_teacher__none=L"None",
	.str_subject__none=L"None",
	.str_room__none=L"None",
	.str_room=L"None",
	.str_lecture_desc=L"Lecture Description",
	.str_detail=L"Detail",

	.str_desc_timetable=L"Describe Timetable",
	.str_class=L"Class",
	.str_teacher=L"Teacher",
	.str_subject=L"Subject",

	.str_generated_timetables=L"Generated timetables",
	.str_generate_timetable_text=L"Here we can generate new timetables. It can take a while.",
	.str_last_generated_timetables=L"Last generated timetables",
	.str_generate=L"Generate",
	.str_generating=L"Generating",
	.str_could_not_generate=L"Could not generate timetable.",
	.str_close=L"Close",
	.str_undo=L"Undo",
	.str_redo=L"Redo",
	.str_cancel=L"Cancel",
	.str_confirm_close_without_saving=L"Do you want to save before leaving?",
	.str_subjects_in_the_group=L"Subjects in the group",
	.str_max_number_of_days=L"Max number of days",
	.str_max_number_of_periods=L"Max number of periods",
	.str_max_number_of_periods_per_day=L"Max number of periods per day",
	.str_number_of_planning_periods=L"Number of planning periods",
	.str_teachers_teach=L"The teachers teach",

	.str_group_available=L"Available",
	.str_group_unavailable=L"Unavailable",
	.str_group_availibility=L"Availibility",
	.str_group_members=L"Group members",
	.str_score_0=L"Unavailable",
	.str_score_1=L"Last-Case",
	.str_score_2=L"Indifferent",
	.str_score_3=L"Prefers",
	.str_score_4=L"Would love to",
	.str_duplicate=L"Duplicar",
	.str_lorem=L"Lorem ipsum dolor sit amet consecteur adiscipling elit.",
	.str_amount=L"Amount",
	.str_belongs=L"Belongs",
	.str_does_not_belong=L"Does not belong",
	.str_to_the_group=L"To the group",
	.str_teaches=L"Teaches",
	.str_day_names=L"Day names",
	.str_daily_period_names=L"Daily period names",
	.str_period_names=L"Period names",
	.str_statistics=L"Statistics",
	.str_number_of_classes=L"Number of classes",
	.str_number_of_teachers=L"Number of teachers",
	.str_number_of_rooms=L"Number of rooms",
	.str_number_of_subjects=L"Number of subjects",
	.str_number_of_subject_groups=L"Number of subject groups",
	.str_basic_data=L"Basic Data",
	.str_actions=L"Actions",
	.str_free_periods=L"Free Periods",
	.str_entry_period=L"Entry Period",
	.str_exit_period=L"Exit Period",
	.str_active=L"Active",
	.str_availability=L"Availability",
	.str_subject_groups=L"Subject Groups",
	.str_data_to_fix=L"Data to Set",
	.str_occurence=L"Occurence",
	.str_needs_room=L"Needs Room",
	.str_days=L"Days",
	.str_planning_time_twinning=L"Twinning / Planning",
	.str_lecture_rooms=L"Rooms / Lectures",
	.str_planning_rooms=L"Rooms / Planning",
	.str_teacher_groups=L"Teacher Groups",
	.str_list_of_teachers_and_their_groups=L"List of Teachers and their Groups",
	.str_list_teachers_and_their_groups=L"List Teachers and their Groups",
	.str_group=L"Group",
	.str_dependency=L"Dependency",
	.str_list_of_classes_and_their_groups=L"List of Classes and their Groups",
	.str_list_classes_and_their_groups=L"List Classes and their Groups",
	.str_is_superclass=L"Is composite class",
};

const Language LANG_SPA = {
	.str_minerva_school_timetables = L"Horario Escolar Minerva",
	.str_minerva_welcome_description = L"Cría ahora el horario de su escuela gratuitamente.\nSobre cual escuela falamos hoy?",
	.str_system_config = L"Ajustes del Sistema",
	.str_language = L"Lenguaje",
	.str_settings = L"Ajustes",
	.str_save = L"Guardar",
	.str_save_as = L"Guardar como",
	.str_close_and_save = L"Guardar y salir",
	.str_close_without_saving = L"Guardar sin salir",
	.str_name = L"Nombre",
	.str_edit = L"Editar",
	.str_size = L"Tamaño",
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
	.str_school_details=L"Datos de la escuela",
	/* Create & view school */
	.str_create_school=L"Crear escuela",
	.str_school_name=L"Nombre de la escuela",
	.str_number_of_days_per_cycle=L"Número de días en un ciclo",
	.str_number_of_periods_per_day=L"Número de períodos por día ",
	.str_in_what_periods_the_school_is_open=L"En que períodos la escuela está abierta?",
	.str_create_school_manual_entry=L"Texto Grande; Texto Grande; Texto Grande; Texto Grande; Texto Grande; ",
	/* Header names */
	.str_school=L"Escuela",
	.str_periods=L"Períodos",
	.str_features=L"Características",
	.str_rooms=L"Salas",
	.str_subjects=L"Asignaturas",
	.str_teachers=L"Maestros",
	.str_classes=L"Turmas",
	.str_lectures=L"Lecciónes",
	.str_events=L"",
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
	.str_list_of_features=L"Lista de Características",
	.str_list_rooms=L"Listar Salas",
	.str_list_of_rooms=L"Lista de Salas",
	.str_list_subjects=L"Listar Asignaturas",
	.str_list_of_subjects=L"Lista de Asignaturas",
	.str_list_subject_groups=L"Listar Grupos de Asignaturas",
	.str_list_of_subject_groups=L"Lista de Grupos de Asignaturas",
	.str_list_teachers=L"Listar Maestros",
	.str_list_of_teachers=L"Lista de Maestros",
	.str_list_teacher_groups=L"Listar Grupos de Maestros",
	.str_list_of_teacher_groups=L"Lista de Grupos de Maestros",
	.str_list_classes=L"Listar Clases",
	.str_list_of_classes=L"Lista de Clases",
	.str_list_class_groups=L"Listar Grupos de Clases",
	.str_list_of_class_groups=L"Lista de Grupos de Clases",
	.str_list_lectures=L"Listar Lecciones",
	.str_list_of_lectures=L"Lista de Lecciones",
	.str_list_planning_times=L" ",
	.str_list_of_planning_times=L" ",
	/* Adding */
	.str_add_feature=L"Añadir Característica",
	.str_add_room=L"Añadir Sala",
	.str_add_subject=L"Añadir Asignaturas",
	.str_add_subject_group=L"Añadir Grupo de Asignaturas",
	.str_add_teacher=L"Añadir Maestro",
	.str_add_teacher_group=L"Añadir Grupo de Maestros",
	.str_add_class=L"Añadir Clase",
	.str_add_class_group=L"Añadir Grupo de Clases",
	/* Possible Preferences */
	.str_period_preference=L"?",
	.str_subject_preference=L"?",
	.str_teacher_preference=L"?",
	.str_class_preference=L"?",
	.str_twin_preference=L"?",
	.str_room_preference=L"?",
	/* Checking */
	.str_check_all=L"?",

	/* General-Purpouse / Group Related */
	.str_add_classes_to_the_group=L"Añadir Clases",
	.str_add_class_to_the_group=L"Añadir Clase",
	.str_group_name=L"Nombre del grupo",
	.str_remove_all=L"Remove Todos",
	.str_remove=L"Remover",
	.str_add_group=L"Añadir Grupo",

	.str_adj__open = L"Abierta",
	.str_adj__closed = L"Cerrada",

	.str_day = L"Día",
	.str_period = L"Período",
	.str_delete = L"Apagar",

	.str_are_you_sure = L"???",
	.str_school_deletion_popup_text = L"???",

	.str_error=L"Error",
	.str_could_not_open_db_error=L"???",

	.str_yes=L"Sí",
	.str_no=L"No",

	.str_fill_the_form_correctly=L"???",
	.str_success=L"???",
	.str_could_not_insert_on_db=L"???",

	.str_list_lectures_by_class=L"",
	.str_add_lectures_by_class=L"",
	.str_see_timetable=L"",
	.str_export_timetable=L"",
	.str_generate_timetable=L"",
	.str_create_timetable_manually=L"",

	.str_class_name=L"",
	.str_class_size=L"",
	.str_class_entry_period=L"",
	.str_class_exit_period=L"",
	.str_class_availibility=L"",
	.str_class_subjects=L"",
	.str_class_can_have_free_periods=L"",
	.str_class_available=L"",
	.str_class_unavailable=L"",
	.str_room_size_in_students=L"",
	.str_room_availibility=L"",
	.str_room_add_features_with_scores=L"",
	.str_add_subjects_to_the_group=L"",

	.str_add_teachers_to_the_group=L"",
	.str_teacher_availibility=L"",
	.str_teacher_available=L"",
	.str_teacher_unavailable=L"",
	.str_teacher_teaches=L"",

	.str_choose_a_timetable=L"",
	.str_timetable__none=L"",
	.str_class__none=L"",
	.str_teacher__none=L"",
	.str_subject__none=L"",
	.str_room__none=L"",
	.str_room=L"",
	.str_lecture_desc=L"",
	.str_detail=L"",
	.str_desc_timetable=L"",
	.str_class=L"",
	.str_teacher=L"",
	.str_subject=L"",

	.str_generated_timetables=L"",
	.str_generate_timetable_text=L"",
	.str_last_generated_timetables=L"",
	.str_generate=L"",
	.str_generating=L"",
	.str_could_not_generate=L"",
	.str_close=L"",
	.str_undo=L"",
	.str_redo=L"",
	.str_cancel=L"",
	.str_confirm_close_without_saving=L"",
	.str_subjects_in_the_group=L"",
	.str_max_number_of_days=L"",
	.str_max_number_of_periods=L"",
	.str_max_number_of_periods_per_day=L"",
	.str_number_of_planning_periods=L"",
	.str_teachers_teach=L"",

	.str_group_available=L" ",
	.str_group_unavailable=L" ",
	.str_group_availibility=L" ",
	.str_group_members=L" ",
	.str_score_0=L" ",
	.str_score_1=L" ",
	.str_score_2=L" ",
	.str_score_3=L" ",
	.str_score_4=L" ",
	.str_duplicate=L" ",
	.str_lorem=L"Lorem ipsum dolor sit amet consecteur adiscipling elit.",
	.str_amount=L"?",
	.str_belongs=L"?",
	.str_does_not_belong=L"?",
	.str_to_the_group=L"?",
	.str_teaches=L"?",
	.str_day_names=L"?",
	.str_daily_period_names=L"?",
	.str_period_names=L"?",
	.str_statistics=L"?",
	.str_number_of_classes=L"?",
	.str_number_of_teachers=L"?",
	.str_number_of_rooms=L"?",
	.str_number_of_subjects=L"?",
	.str_number_of_subject_groups=L"?",
	.str_basic_data=L"?",
	.str_actions=L"?",
	.str_free_periods=L"?",
	.str_entry_period=L"?",
	.str_exit_period=L"?",
	.str_active=L"?",
	.str_availability=L"?",
	.str_subject_groups=L"?",
	.str_data_to_fix=L"?",
	.str_occurence=L"?",
	.str_needs_room=L"?",
	.str_days=L"?",
	.str_planning_time_twinning=L"?",
	.str_lecture_rooms=L"?",
	.str_planning_rooms=L"?",
	.str_teacher_groups=L"?",
	.str_list_of_teachers_and_their_groups=L"?",
	.str_list_teachers_and_their_groups=L"",
	.str_group=L"?",
	.str_dependency=L"?",
	.str_list_of_classes_and_their_groups=L"?",
	.str_list_classes_and_their_groups=L"?",
	.str_is_superclass=L"?",
};

const Language LANG_DEU = {
	.str_minerva_school_timetables = L"Minerva Stundenplan",
	.str_minerva_welcome_description = L"Beginnen Sie kostenlos, den Stundenplan deiner Schule zu erstellen.\nÜber welcher Schule sprechen wir heute?",
	.str_system_config = L"Systemeinstellungen",
	.str_language = L"Sprache",
	.str_settings = L"Einstellungen",
	.str_save = L"Speichern",
	.str_save_as = L"Speichern als",
	.str_close_and_save = L"Speichern und Schließen",
	.str_close_without_saving = L"Schließen ohne zu Speichern",
	.str_name = L"Name",
	.str_edit = L"Bearbeiten",
	.str_size = L"Große",
	.str_back = L"Zurück",
	.str_font_sz= L"Schriftgröße",
	.str_small_font=L"Kleine",
	.str_medium_font=L"Mittlere",
	.str_large_font=L"Große",
	.str_open=L"Offenen",
	.str_help=L"Hilfe",
	.str_create=L"Erstellen",
	.str_copyright_notice=L"Copyright (C) Léo Hardt 2020. Lizenz definiert werden.",
	.str_open_manual=L"Handbuch offenen",
	.str_school_details=L"Schulendatei",
	/* Create & view school */
	.str_create_school=L"Schule Erstellen",
	.str_school_name=L"Name der Schule",
	.str_number_of_days_per_cycle=L"Anzahl Tage pro Zyklus",
	.str_number_of_periods_per_day=L"Anzahl Zeiträume pro Tag",
	.str_in_what_periods_the_school_is_open=L"In welche Schulstunde ist die Schule geöffenet?",
	.str_create_school_manual_entry=L"Grosses Text; Grosses Text; Grosses Text; Grosses Text; Grosses Text;",
	/* Header names */
	.str_school=L"Schule",
	.str_periods=L"Periods",
	.str_features=L"Merkmale",
	.str_rooms=L"Klassenzimmer",
	.str_subjects=L"Fache",
	.str_teachers=L"Lehrer",
	.str_classes=L"Klassen",
	.str_lectures=L"Vorträge",
	.str_events=L"?",
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
	.str_list_of_features=L"Merkmaleliste",
	.str_list_rooms=L"Klassenzimmer auflisten",
	.str_list_of_rooms=L"Klassenzimmerliste",
	.str_list_subjects=L"Fache auflisten",
	.str_list_of_subjects=L"Fachliste",
	.str_list_subject_groups=L"Fächergruppen auflisten",
	.str_list_of_subject_groups=L"Fächergruppenliste",
	.str_list_teachers=L"Lehrer auflisten",
	.str_list_of_teachers=L"Lehrerliste",
	.str_list_teacher_groups=L"Lehrergruppen auflisten",
	.str_list_of_teacher_groups=L"Lehrergruppenliste",
	.str_list_classes=L"Klassen auflisten",
	.str_list_of_classes=L"Klassenliste",
	.str_list_class_groups=L"Klassengruppen auflisten",
	.str_list_of_class_groups=L"Klassengruppenlisten",
	.str_list_lectures=L"Vorträge auflisten",
	.str_list_of_lectures=L"Vortragsliste",
	/* Isn't 'List of lectures' translated in the same way as 'a cup of tea' (eine tasse tee?) */
	.str_list_planning_times=L"?",
	.str_list_of_planning_times=L"?",
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
	.str_teacher_preference=L"?",
	.str_class_preference=L"?",
	.str_twin_preference=L"?",
	.str_room_preference=L"?",
	/* Checking */
	.str_check_all=L"Alles prüfen",
	/* General-Purpouse / Group Related */
	.str_add_classes_to_the_group=L"Klasse zur Gruppe hinzufügen",
	.str_add_class_to_the_group=L"Klasse hinzufügen",
	.str_group_name=L"Gruppenname",
	.str_remove_all=L"Alles Entfernen",
	.str_remove=L"Entfernen",
	.str_add_group=L"Gruppe hinzufügen",

	.str_adj__open = L"Geoffenet",
	.str_adj__closed = L"Geschlosst",

	.str_day = L"Tag",
	.str_period = L"Period",
	.str_delete = L"Löschen",

	.str_are_you_sure = L"Sind Sie sicher?",
	.str_school_deletion_popup_text = L"??",

	.str_error=L"Fehler",
	.str_could_not_open_db_error=L"???.",

	.str_yes=L"Ja",
	.str_no=L"Nein",

	.str_fill_the_form_correctly=L"???",
	.str_success=L"???",
	.str_could_not_insert_on_db=L"???",

	.str_list_lectures_by_class=L"",
	.str_add_lectures_by_class=L"",
	.str_see_timetable=L"Zeitplan sehen",
	.str_export_timetable=L"Zeitplan ??",
	.str_generate_timetable=L"Zeitpaln Erstellend",
	.str_create_timetable_manually=L"",

	.str_class_name=L"Klassenname",
	.str_class_size=L"Klassengroße",
	.str_class_entry_period=L"",
	.str_class_exit_period=L"",
	.str_class_availibility=L"Verfügbarkeit",
	.str_class_subjects=L"Fächer der Klasse",
	.str_class_can_have_free_periods=L"",
	.str_class_available=L"Verfügbar",
	.str_class_unavailable=L"Nicht Verfügbar",
	.str_room_size_in_students=L"",
	.str_room_availibility=L"Verfügbarkeit",
	.str_room_add_features_with_scores=L"",
	.str_add_subjects_to_the_group=L"",
	.str_add_teachers_to_the_group=L"",
	.str_teacher_availibility=L"Verfügbarkeit",
	.str_teacher_available=L"Verfügbar",
	.str_teacher_unavailable=L"Nicht Verfügbar",
	.str_teacher_teaches=L"Der Lehrer lehrt",
	.str_choose_a_timetable=L"",
	.str_timetable__none=L"Kein",
	.str_class__none=L"Keine",
	.str_teacher__none=L"Kein",
	.str_subject__none=L"Kein",
	.str_room__none=L"Kein",
	.str_room=L"Klassenzimmer",
	.str_lecture_desc=L"Vortrag detaillieren",
	.str_detail=L"Einzelheit",
	.str_desc_timetable=L"Zeitplan detaillieren",
	.str_class=L"Klass",
	.str_teacher=L"Lehrer",
	.str_subject=L"Fach",
	.str_generated_timetables=L"Erstellt Zeitpläne",
	.str_generate_timetable_text=L"",
	.str_last_generated_timetables=L"Letzte erstellt Zeitpläne",
	.str_generate=L"Erstellen",
	.str_generating=L"Erstellend",
	.str_could_not_generate=L"Konnte es nicht zu erstellen",
	.str_close=L"Schließen",
	.str_undo=L"Rückgängig",
	.str_redo=L"Wiederholen",
	.str_cancel=L"Abbrechen",
	.str_confirm_close_without_saving=L"Bestätige ohne zu speichern schließen",
	.str_subjects_in_the_group=L"Fäche in der Gruppe",
	.str_max_number_of_days=L"Die maximal Anzahl von Tage",
	.str_max_number_of_periods=L"Die maximal Anzahl von Zeiträume",
	.str_max_number_of_periods_per_day=L"Die maximal Anzahl von Zeiträume pro Tag",
	.str_number_of_planning_periods=L"Die Anzahl von Klassenplanungszeiträume",
	.str_teachers_teach=L"Die Lehrer lehren",

	.str_group_available=L"?",
	.str_group_unavailable=L"?",
	.str_group_availibility=L"?",
	.str_group_members=L"?",

	.str_score_0=L" ",
	.str_score_1=L" ",
	.str_score_2=L" ",
	.str_score_3=L" ",
	.str_score_4=L" ",
	.str_duplicate=L" ",

	.str_lorem=L"Lorem ipsum dolor sit amet consecteur adiscipling elit.",
	.str_amount=L"?",
	.str_belongs=L"?",
	.str_does_not_belong=L"?",
	.str_to_the_group=L"?",
	.str_teaches=L"?",
	.str_day_names=L"?",
	.str_daily_period_names=L"?",
	.str_period_names=L"?",
	.str_statistics=L"?",
	.str_number_of_classes=L"?",
	.str_number_of_teachers=L"?",
	.str_number_of_rooms=L"?",
	.str_number_of_subjects=L"?",
	.str_number_of_subject_groups=L"?",
	.str_basic_data=L"?",
	.str_actions=L"?",
	.str_free_periods=L"?",
	.str_entry_period=L"?",
	.str_exit_period=L"?",
	.str_active=L"?",
	.str_availability=L"?",
	.str_subject_groups=L"?",
	.str_data_to_fix=L"",
	.str_occurence=L"",
	.str_needs_room=L"",
	.str_days=L"Tage",
	.str_planning_time_twinning=L"?",
	.str_lecture_rooms=L"?",
	.str_planning_rooms=L"?",
	.str_teacher_groups=L"?",
	.str_list_of_teachers_and_their_groups=L"?",
	.str_list_teachers_and_their_groups=L"?",
	.str_group=L"?",
	.str_dependency=L"?",
	.str_list_of_classes_and_their_groups=L"?",
	.str_list_classes_and_their_groups=L"?",
	.str_is_superclass=L"?",
};
