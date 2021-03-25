#include "gui.hpp"
#include <wx/notebook.h>
#include <wx/richtext/richtextctrl.h>

ManualWindow::ManualWindow(Application * owner) : wxFrame(nullptr, wxID_ANY, owner->m_lang->str_minerva_school_timetables, wxPoint(30,30), wxSize(490,600)){
	m_owner = owner;

	#ifdef __WXMSW__
		SetIcon(wxICON(aaaaaaaa));
	#endif

	SetFont(*m_owner->m_text_font);
	m_notebook = new wxNotebook(this, wxID_ANY);

	m_notebook->AddPage(PopulateSchoolWindow(), wxT("Geral"));
	m_notebook->AddPage(PopulateRoomsWindow(), wxT("Salas"));
	m_notebook->AddPage(PopulateSubjectsWindow(), wxT("Disciplinas"));
	m_notebook->AddPage(PopulateTeachersWindow(), wxT("Professores"));
	m_notebook->AddPage(PopulateClassesWindow(), wxT("Turmas"));
	// m_notebook->AddPage(PopulateLecturesWindow(), wxT("Aulas"));
	m_notebook->AddPage(PopulateTimetableWindow(), wxT("Horário"));
	m_notebook->AddPage(PopulateSoftwareWindow(), wxT("Software"));

	this->Refresh();
}

wxWindow * ManualWindow::PopulateSchoolWindow(){
	wxRichTextCtrl * text = new wxRichTextCtrl(m_notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxRE_READONLY);

	text->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	text->BeginBold();
	text->WriteText(wxT("Por Onde Começar\n"));
	text->EndBold();
	text->BeginAlignment(wxTEXT_ALIGNMENT_LEFT );
	text->WriteText(wxT("Agradecemos por você estar testando este programa. Todo feedback é importante para nós.\n\n"
				"A criação de uma nova escola pode ser feita da tela inicial no botão CRIAR. Os dados dela poderão ser alterados "
				"posteriormente no menu DETALHES DA ESCOLA. Para editar valores, primeiro clique no botão EDITAR abaixo, altere e aperte em SALVAR.\n\n"
				"Dias podem ter nomes, como 'Seg', 'Ter' ou 'Qua', assim como os períodos diários, ex: 10:00, 11:00, 12:00, etc.\n\n"

				"Para desfazer alguma ação feita, pode-se clicar no botão DESFAZER na parte inferior da tela. Para cada mudança que deve ser salva, "
				"clique no botão SALVAR, ao lado. Para refazer uma ação desfeita, pode-se apertar no botão REFAZER."
			));
	return text;
}

wxWindow * ManualWindow::PopulateRoomsWindow(){
	wxRichTextCtrl * text = new wxRichTextCtrl(m_notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxRE_READONLY);
	text->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	text->BeginBold();
	text->WriteText(wxT("Salas e Características\n"));
	text->EndBold();
	text->BeginAlignment(wxTEXT_ALIGNMENT_LEFT );
	text->WriteText(wxT("A primeira coisa a se fazer, agora que já temos a escola, é inserir as salas.\n\n "
				"Salas têm capacicades medidas em número de alunos. Então uma turma de 30 alunos não poderia ter aulas em uma sala de capacidade 20."
				"\n\n Elas podem ser removidas ou editadas no menu LISTAR SALAS, e adicionadas no menu ADICIONAR SALA."
			));
	return text;
}

wxWindow * ManualWindow::PopulateSubjectsWindow(){
	wxRichTextCtrl * text = new wxRichTextCtrl(m_notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxRE_READONLY);
	text->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	text->BeginBold();
	text->WriteText(wxT("Disciplinas e seus Grupos\n"));
	text->EndBold();
	text->BeginAlignment(wxTEXT_ALIGNMENT_LEFT );
	text->WriteText(wxT("Disciplinas .\n\n "
				"É com disciplinas que dizemos ao programa quais aulas uma turma precisa, e com quais professores essas aulas podem acontecer. "
				"Para editá-las ou removê-las, podemos ir ao menu LISTAR DISCIPLINAS. Adicionamos mais disciplinas no menu Adicionar Disciplina.\n\n"
				"Grupos de disciplinas podem ser usados quando uma turma tem um limite de aulas em uma área do conhecimento (ex. Exatas) por dia."
			));
	return text;
}

wxWindow * ManualWindow::PopulateTeachersWindow(){
	wxRichTextCtrl * text = new wxRichTextCtrl(m_notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxRE_READONLY);
	text->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	text->BeginBold();
	text->WriteText(wxT("Professores e seus Grupos\n"));
	text->EndBold();
	text->BeginAlignment(wxTEXT_ALIGNMENT_LEFT );
	text->WriteText(wxT("Professores têm duas características importantíssimas: as disciplinas das quais dão aula e suas disponibilidades.\n\n "
		"Um professor pode ensinar mais de uma disciplina, e pode definir em quais dias, perídos e salas ele pode frequentar (no menu LISTAR PROFESSORES)."
		"Além disso, se professores dão aulas em conjunto (como nos casos de História e Geografia), com os dois professores ao mesmo tempo, "
		"deve ser criado um grupo com esses professores. O grupo, então, é quem ensina a disciplina Historia E Geografia."
	));
	return text;
}

wxWindow * ManualWindow::PopulateClassesWindow(){
	wxRichTextCtrl * text = new wxRichTextCtrl(m_notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxRE_READONLY);
	text->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	text->BeginBold();
	text->WriteText(wxT("Turmas e seus Grupos\n"));
	text->EndBold();
	text->BeginAlignment(wxTEXT_ALIGNMENT_LEFT );
	text->WriteText(wxT("As turmas são o último componente obrigatório para a geração do horário.\n\n "
			"Deve-se informar os períodos em que a turma frequenta a escola, bem como quantos períodos de quais disciplinas a turma necessita."
			" (por exemplo, uma turma pode precisar de 7 períodos de Matemática e 2 de Física, e não frequenar a escola às Sextas-feiras). "
			"Um grupo de turmas pode ser feito, por exemplo, para que tenham aulas juntas. Se as turmas 7ºA e 7°B se juntam na aula "
			"de educação física, cria-se um grupo \"7°\", contendo a 7A e a 7B, que assiste a essas aulas."
		));
	return text;
}

wxWindow * ManualWindow::PopulateLecturesWindow(){
	wxRichTextCtrl * text = new wxRichTextCtrl(m_notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxRE_READONLY);
	text->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	text->BeginBold();
	text->WriteText(wxT("Turmas e seus Grupos\n"));
	text->EndBold();
	text->BeginAlignment(wxTEXT_ALIGNMENT_LEFT );
	text->WriteText(wxT("Repare que todas as aulas já são adicionadas na criação das turmas. Esta seção se trata de ajustes finos nas aulas.\n\n"
			"Para cada aula (encontro entre turma e um professor em um período), há uma lista de possibilidades de períodos, de professores e de salas "
			"(por exemplo, uma turma pode precisar de 7 períodos de Matemática e 2 de Física, e não frequenar a escola às Sextas-feiras). "
			"Um grupo de turmas pode ser feito, por exemplo, para que tenham aulas juntas. Se as turmas 7ºA e 7°B se juntam na aula "
			"de educação física, cria-se um grupo \"7°\", contendo a 7A e a 7B, que assiste a essas aulas."
		));
	return text;
}

wxWindow * ManualWindow::PopulateTimetableWindow(){
	wxRichTextCtrl * text = new wxRichTextCtrl(m_notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxRE_READONLY);
	text->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	text->BeginBold();
	text->WriteText(wxT("Geração Final do Horário\n"));
	text->EndBold();
	text->BeginAlignment(wxTEXT_ALIGNMENT_LEFT );
	text->WriteText(wxT("Se tudo correu bem até aqui, deve ser possível gerar o horário, na aba com este nome.\n\n"
			"Tendo sido gerado com sucesso, o horário estará disponível para visualização. Se, por algum acaso, o horário for ruim, "
			"será possível deletá-lo para ser gerado novamente. Note que ao gerar o horário duas vezes, o resultado será exatamente o "
			"mesmo. Ele só será criado de forma diferente se as configurações na aba \"Aula\" ou em qualquer aba anterior forem mudadas. "
		));
	return text;
}

wxWindow * ManualWindow::PopulateSoftwareWindow(){
	wxRichTextCtrl * text = new wxRichTextCtrl(m_notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxRE_READONLY);
	text->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	text->BeginBold();
	text->WriteText(wxT("Sobre o presente Software\n"));
	text->EndBold();
	text->BeginAlignment(wxTEXT_ALIGNMENT_LEFT );
	text->WriteText(wxT("O presente software é parte de um Trabalho de Conclusão de Curso, por Léo Hardt, no IFRS-Canoas.\n\n"
				"Para compor este programa, foram utilizadas diferentes bibliotecas livres. São elas: SQLite3, wxWidgets. "
			));
	return text;
}

ManualWindow::~ManualWindow(){
	m_owner->m_window_manual = NULL;
}
