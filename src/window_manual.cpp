#include "gui.hpp"
#include <wx/notebook.h>

ManualWindow::ManualWindow(Application * owner) : wxFrame(nullptr, wxID_ANY, wxString::FromUTF8("Horário Escolar Minerva - Manual"), wxPoint(30,30), wxSize(490,600)){
	m_owner = owner;

	m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	m_notebook->InsertPage(0, PopulateSchoolWindow(), wxT("Geral."));
	m_notebook->InsertPage(1, PopulateRoomsWindow(), wxT("Salas."));
	m_notebook->InsertPage(2, new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize), wxT("Disciplinas."));
	m_notebook->InsertPage(3, new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize), wxT("Professores."));
	m_notebook->InsertPage(4, new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize), wxT("Turmas."));
	m_notebook->InsertPage(5, new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize), wxT("Aulas."));
	m_notebook->InsertPage(6, new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize), wxT("Horário."));

	PopulateSchoolWindow();

	this->Refresh();
}

wxWindow * ManualWindow::PopulateSchoolWindow(){

	wxScrolledWindow * man_win = new wxScrolledWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);

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

	wxSizer * textsz = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(man_win, wxID_ANY, wxT("Salas e Características\n"), wxDefaultPosition, wxSize(300,30));
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

ManualWindow::~ManualWindow(){
	m_owner->m_window_manual = NULL;
}
