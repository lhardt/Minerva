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
	m_notebook->AddPage(PopulateLecturesWindow(), wxT("Aulas"));
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
	text->WriteText(wxT("Gostaríamos mais uma vez de estar testando essa novidade. Todo feedback nos é importante.\n\n"
				"Para usar este programa, é importante notar que todos os dados são salvos automaticamente. "
				"No entanto, ainda não criamos uma função de desfazer. Há uma variedade de outras funções que não operam ainda.\n\n"
				"Por ser uma versão preliminar, o programa que você está usando serve mais para ser julgado (incluindo dar dicas, fazer reclamações, etc) "
				"do que realmente para fazer o horário. Ainda assim, a geração do horário está disponível e, a princípio, deve dar um resultado válido."
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
				"Algumas salas podem ser diferentes das outras e é aí que entram as características. Por exemplo, se algumas salas são laboratório, "
				"outras são ginásio, vale a pena inserir essas características no sistema. Elas serão úteis mais tarde, na inserção de aulas."
				"Uma aula pode exigir uma característica (como um professor de química poderia exigir um laboratório)."
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
	text->WriteText(wxT("Disciplinas, no papel, são coisas muito simples: somente seu nome.\n\n "
				"No entanto, alguns professores podem preferir dar aula de química só nos laboratórios, ou só nos primeiros períodos. "
				"Futuramente, essas configurações poderão ser feitas mais adiante, na aba \"Aulas\", pois cada professor "
				"poderia decidir sobre se usará um laboratório ou não. \n\n"
			));
	return text;
}

wxWindow * ManualWindow::PopulateTeachersWindow(){
	wxRichTextCtrl * text = new wxRichTextCtrl(m_notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxRE_READONLY);
	text->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	text->BeginBold();
	text->WriteText(wxT("Disciplinas e seus Grupos\n"));
	text->EndBold();
	text->BeginAlignment(wxTEXT_ALIGNMENT_LEFT );
	text->WriteText(wxT("Professores têm duas características importantíssimas: as disciplinas das quais dão aula e suas disponibilidades.\n\n "
		"Poderá haver preferência por parte do professor por uma disciplina, mesmo que ele possa ensinar mais de uma"
		" (por exemplo, um professor que pode dar aula de Matemática e de Física, mas que tenha mais experiência com Física). "
		"Além disso, se professores dão aulas em conjunto (como nos casos de História e Geografia), com os dois professores ao mesmo tempo, "
		"deve ser criado um grupo com esses professores. O grupo, então, é quem ensina a \"disciplina HistoGeo\"."
	));
	return text;
}

wxWindow * ManualWindow::PopulateClassesWindow(){
	wxRichTextCtrl * text = new wxRichTextCtrl(m_notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxRE_READONLY);
	text->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	text->BeginBold();
	text->WriteText(wxT("Turmas e seus Grupos\n"));
	text->EndBold();
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
	text->WriteText(wxT("O presente software é parte de um Trabalho de Conclusão de Curso, por Léo Hardt, no IFRS-Canoas.\n\n"
				"Ele será aprimorado subsequentemente, tendo em vista as novas melhorias que serão necessárias com seu uso. "
				"Em sua versão atual, é apenas um protótipo. A licença de uso e de distribuição será definida mais tarde.\n\n"
				"Para compor este programa, foram utilizadas diferentes bibliotecas livres. São elas: SQLite3, wxWidgets. "
				// TODO TODO
			));
	return text;
}

ManualWindow::~ManualWindow(){
	m_owner->m_window_manual = NULL;
}
