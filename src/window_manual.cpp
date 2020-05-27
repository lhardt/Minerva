#include "gui.hpp"
#include <wx/notebook.h>

ManualWindow::ManualWindow(Application * owner) : wxFrame(nullptr, wxID_ANY, wxString::FromUTF8("Horário Escolar Minerva - Manual"), wxPoint(30,30), wxSize(490,600)){
	m_owner = owner;

	#ifdef __WXMSW__
		SetIcon(wxICON(aaaaaaaa));
	#endif

	m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	m_notebook->InsertPage(0, PopulateSchoolWindow(), wxT("Geral"));
	m_notebook->InsertPage(1, PopulateRoomsWindow(), wxT("Salas"));
	m_notebook->InsertPage(2, PopulateSubjectsWindow(), wxT("Disciplinas"));
	m_notebook->InsertPage(3, PopulateTeachersWindow(), wxT("Professores"));
	m_notebook->InsertPage(4, PopulateClassesWindow(), wxT("Turmas"));
	m_notebook->InsertPage(5, PopulateLecturesWindow(), wxT("Aulas"));
	m_notebook->InsertPage(6, PopulateTimetableWindow(), wxT("Horário"));
	m_notebook->InsertPage(6, PopulateSoftwareWindow(), wxT("Software"));

	this->Refresh();
}

wxWindow * ManualWindow::PopulateSchoolWindow(){
	wxScrolledWindow * man_win = new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	man_win->SetBackgroundColour(wxColor(255,255,255));

	wxSizer * textsz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(man_win, wxID_ANY, wxT("Por Onde Começar\n"), wxDefaultPosition, wxSize(300,30));
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(title, 0, wxALIGN_CENTER | wxALL, 15);

	wxStaticText * introd = new wxStaticText(man_win, wxID_ANY,
			wxT("Gostaríamos mais uma vez de estar testando essa novidade. Todo feedback nos é importante.\n\n"
				"Para usar este programa, é importante notar que todos os dados são salvos automaticamente. "
				"No entanto, ainda não criamos uma função de desfazer. Há uma variedade de outras funções que não operam ainda.\n\n "
				"Por ser uma versão preliminar, o programa que você está usando serve mais para ser julgado (incluindo dar dicas, fazer reclamações, etc) "
				"do que realmente para fazer o horário. Ainda assim, a geração do horário está disponível e, a princípio, deve dar um resultado válido."
			), wxDefaultPosition, wxSize(450,500));
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(introd, 0, wxALL, 15);

	man_win->SetSizerAndFit(textsz);
	return man_win;
}

wxWindow * ManualWindow::PopulateRoomsWindow(){
	wxScrolledWindow * man_win = new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	man_win->SetBackgroundColour(wxColor(255,255,255));

	wxSizer * textsz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(man_win, wxID_ANY, wxT("Salas e Características\n"), wxDefaultPosition, wxSize(300,30), wxALIGN_CENTRE_HORIZONTAL);
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(title, 0, wxALIGN_CENTER | wxALL, 15);


	wxStaticText * introd = new wxStaticText(man_win, wxID_ANY,
			wxT("A primeira coisa a se fazer, agora que já temos a escola, é inserir as salas.\n\n "
				"Algumas salas podem ser diferentes das outras e é aí que entram as características. Por exemplo, se algumas salas são laboratório, "
				"outras são ginásio, vale a pena inserir essas características no sistema. Elas serão úteis mais tarde, na inserção de aulas."
				"Uma aula pode exigir uma característica (como um professor de química poderia exigir um laboratório)."
			), wxDefaultPosition, wxSize(450,500));
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(introd, 0, wxALL, 15);

	man_win->SetSizerAndFit(textsz);
	return man_win;
}

wxWindow * ManualWindow::PopulateSubjectsWindow(){
	wxScrolledWindow * man_win = new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	man_win->SetBackgroundColour(wxColor(255,255,255));

	wxSizer * textsz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(man_win, wxID_ANY, wxT("Disciplinas e seus Grupos\n"), wxDefaultPosition, wxSize(400,30), wxALIGN_CENTRE_HORIZONTAL);
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(title, 0, wxALIGN_CENTER | wxALL, 15);


	wxStaticText * introd = new wxStaticText(man_win, wxID_ANY,
			wxT("Disciplinas, no papel, são coisas muito simples: somente seu nome.\n\n "
				"No entanto, alguns professores podem preferir dar aula de química só nos laboratórios, ou só nos primeiros períodos. "
				"Futuramente, essas configurações poderão ser feitas mais adiante, na aba \"Aulas\", pois cada professor "
				"poderia decidir sobre se usará um laboratório ou não. \n\n"
			), wxDefaultPosition, wxSize(450,500));
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(introd, 0, wxALL, 15);

	man_win->SetSizerAndFit(textsz);
	return man_win;
}

wxWindow * ManualWindow::PopulateTeachersWindow(){
	wxScrolledWindow * man_win = new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	man_win->SetBackgroundColour(wxColor(255,255,255));

	wxSizer * textsz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(man_win, wxID_ANY, wxT("Professores e seus Grupos\n"), wxDefaultPosition, wxSize(400,30), wxALIGN_CENTRE_HORIZONTAL);
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(title, 0, wxALIGN_CENTER | wxALL, 15);


	wxStaticText * introd = new wxStaticText(man_win, wxID_ANY,
			wxT("Professores têm duas características importantíssimas: as disciplinas das quais dão aula e suas disponibilidades.\n\n "
				"Poderá haver preferência por parte do professor por uma disciplina, mesmo que ele possa ensinar mais de uma"
				" (por exemplo, um professor que pode dar aula de Matemática e de Física, mas que tenha mais experiência com Física). "
				"Além disso, se professores dão aulas em conjunto (como nos casos de História e Geografia), com os dois professores ao mesmo tempo, "
				"deve ser criado um grupo com esses professores. O grupo, então, é quem ensina a \"disciplina HistoGeo\"."
			), wxDefaultPosition, wxSize(450,500));
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(introd, 0, wxALL, 15);

	man_win->SetSizerAndFit(textsz);
	return man_win;
}

wxWindow * ManualWindow::PopulateClassesWindow(){
	wxScrolledWindow * man_win = new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	man_win->SetBackgroundColour(wxColor(255,255,255));

	wxSizer * textsz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(man_win, wxID_ANY, wxT("Turmas e seus Grupos\n"), wxDefaultPosition, wxSize(400,30), wxALIGN_CENTRE_HORIZONTAL);
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(title, 0, wxALIGN_CENTER | wxALL, 15);

	wxStaticText * introd = new wxStaticText(man_win, wxID_ANY,
			wxT("As turmas são o último componente obrigatório para a geração do horário.\n\n "
				"Deve-se informar os períodos em que a turma frequenta a escola, bem como quantos períodos de quais disciplinas a turma necessita."
				" (por exemplo, uma turma pode precisar de 7 períodos de Matemática e 2 de Física, e não frequenar a escola às Sextas-feiras). "
				"Um grupo de turmas pode ser feito, por exemplo, para que tenham aulas juntas. Se as turmas 7ºA e 7°B se juntam na aula "
				"de educação física, cria-se um grupo \"7°\", contendo a 7A e a 7B, que assiste a essas aulas."
			), wxDefaultPosition, wxSize(450,500));
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(introd, 0, wxALL, 15);

	man_win->SetSizerAndFit(textsz);
	return man_win;
}

wxWindow * ManualWindow::PopulateLecturesWindow(){
	wxScrolledWindow * man_win = new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	man_win->SetBackgroundColour(wxColor(255,255,255));

	wxSizer * textsz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(man_win, wxID_ANY, wxT("Aulas e suas Especificações\n"), wxDefaultPosition, wxSize(400,30), wxALIGN_CENTRE_HORIZONTAL);
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(title, 0, wxALIGN_CENTER | wxALL, 15);

	wxStaticText * introd = new wxStaticText(man_win, wxID_ANY,
			wxT("Repare que todas as aulas já são adicionadas na criação das turmas. Esta seção se trata de ajustes finos nas aulas.\n\n"
				"Para cada aula (encontro entre turma e um professor em um período), há uma lista de possibilidades de períodos, de professores e de salas "
				"(por exemplo, uma turma pode precisar de 7 períodos de Matemática e 2 de Física, e não frequenar a escola às Sextas-feiras). "
				"Um grupo de turmas pode ser feito, por exemplo, para que tenham aulas juntas. Se as turmas 7ºA e 7°B se juntam na aula "
				"de educação física, cria-se um grupo \"7°\", contendo a 7A e a 7B, que assiste a essas aulas."
			), wxDefaultPosition, wxSize(450,500));
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(introd, 0, wxALL, 15);

	man_win->SetSizerAndFit(textsz);
	return man_win;
}


wxWindow * ManualWindow::PopulateTimetableWindow(){
	wxScrolledWindow * man_win = new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	man_win->SetBackgroundColour(wxColor(255,255,255));

	wxSizer * textsz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(man_win, wxID_ANY, wxT("Geração Final do Horário\n"), wxDefaultPosition, wxSize(400,30), wxALIGN_CENTRE_HORIZONTAL);
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(title, 0, wxALIGN_CENTER | wxALL, 15);

	wxStaticText * introd = new wxStaticText(man_win, wxID_ANY,
			wxT("Se tudo correu bem até aqui, deve ser possível gerar o horário, na aba com este nome.\n\n"
				"Tendo sido gerado com sucesso, o horário estará disponível para visualização. Se, por algum acaso, o horário for ruim, "
				"será possível deletá-lo para ser gerado novamente. Note que ao gerar o horário duas vezes, o resultado será exatamente o "
				"mesmo. Ele só será criado de forma diferente se as configurações na aba \"Aula\" ou em qualquer aba anterior forem mudadas. "
			), wxDefaultPosition, wxSize(450,500));
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(introd, 0, wxALL, 15);

	man_win->SetSizerAndFit(textsz);
	return man_win;
}

wxWindow * ManualWindow::PopulateSoftwareWindow(){
	wxScrolledWindow * man_win = new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	man_win->SetBackgroundColour(wxColor(255,255,255));

	wxSizer * textsz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(man_win, wxID_ANY, wxT("Sobre o presente Software\n"), wxDefaultPosition, wxSize(400,30), wxALIGN_CENTRE_HORIZONTAL);
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(title, 0, wxALIGN_CENTER | wxALL, 15);

	wxStaticText * introd = new wxStaticText(man_win, wxID_ANY,
			wxT("O presente software é parte de um Trabalho de Conclusão de Curso, por Léo Hardt, no IFRS-Canoas.\n\n"
				"Ele será aprimorado subsequentemente, tendo em vista as novas melhorias que serão necessárias com seu uso. "
				"Em sua versão atual, é apenas um protótipo. A licença de uso e de distribuição será definida mais tarde."
			), wxDefaultPosition, wxSize(450,500));
	title->SetFont(*m_owner->m_page_title_font);
	textsz->Add(introd, 0, wxALL, 15);

	man_win->SetSizerAndFit(textsz);
	return man_win;
}



ManualWindow::~ManualWindow(){
	m_owner->m_window_manual = NULL;
}
