#include "gui.hpp"

ListClassGroupsPane::ListClassGroupsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * body_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * butn_sz = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Grupos de Turmas"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	wxArrayString group_names;
	for(i = 0; i < school->n_classes; ++i){
		if(school->classes[i].subordinates != NULL){
			group_names.push_back(wxString::FromUTF8(school->classes[i].name));
		}
	}

	m_groups_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300), group_names);
	m_name_text = new wxStaticText(this, wxID_ANY, wxT("Nome do Grupo: "), wxDefaultPosition, wxSize(400,25));
	m_members_text = new wxStaticText(this, wxID_ANY, wxT("Membros do Grupo: "), wxDefaultPosition, wxSize(400,-1));

	wxStaticText * periods_label = new wxStaticText(this, wxID_ANY, wxT("Períodos em que o grupo inteiro está presente: "), wxDefaultPosition, wxSize(300,30));
	m_periods_grid = new ChoiceGrid(this, wxID_ANY, wxDefaultPosition, wxSize(400,300));

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(wxT("Disponível"));
	grid_values.push_back(wxT("Indisponível"));
	m_periods_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200));
	m_periods_grid->SetBackgroundColors(grid_colors);

	m_periods_grid->m_basic_col_name = wxT("Dia");
	m_periods_grid->m_basic_row_name = wxT("Período");

	m_periods_grid->GridRemake(school->n_days,school->n_periods_per_day);

	wxButton * edit_btn = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,wxT("Remover"), wxDefaultPosition, wxSize(200,30));

	m_err_msg = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,30));

	butn_sz->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	butn_sz->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	desc_sz->Add(m_name_text, 0, wxBOTTOM, 5);
	desc_sz->Add(m_members_text, 0, wxBOTTOM, 5);
	desc_sz->Add(periods_label, 0, wxBOTTOM, 5);
	desc_sz->Add(m_periods_grid, 0, wxBOTTOM, 5);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(butn_sz, 0, 0);

	body_sz->Add(m_groups_list, 0, wxEXPAND|wxALL, 15);
	body_sz->Add(desc_sz, 1, wxEXPAND|wxALL, 15);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(body_sz, 1, wxALL, 15);
	sizer->Add(m_err_msg, 0, wxBOTTOM | wxLEFT | wxRIGHT, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	m_groups_list->Bind(wxEVT_LISTBOX, &ListClassGroupsPane::OnSelectionChanged, this);
	edit_btn->Bind(wxEVT_BUTTON, &ListClassGroupsPane::OnEditButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListClassGroupsPane::OnDeleteButtonClicked, this);
}

void ListClassGroupsPane::OnSelectionChanged(wxCommandEvent & evt){
	int i_select, i_class, i_group = 0, i;
	School * school = m_owner->m_school;
	i_select = m_groups_list->GetSelection();
	if(i_select != wxNOT_FOUND){
		for(i_class = 0; i_class < school->n_classes && i_group <= i_select; ++i_class){
			if(school->classes[i_class].subordinates != NULL){
				++i_group;
			}
		}
		if(i_group == i_select +1){
			--i_class;
			m_name_text->SetLabel(wxT("Nome do Grupo: ") + wxString::FromUTF8(school->classes[i_class].name));

			for(i = 0; i < school->n_classes; ++i){
				if(school->classes[i_class].subordinates[i] > 0){
					m_members_text->SetLabel(m_members_text->GetLabel() + "\n\t" + wxString::FromUTF8(school->classes[i].name));
				}
			}
		}
	}
}

void ListClassGroupsPane::OnEditButtonClicked(wxCommandEvent & evt){

}

void ListClassGroupsPane::OnDeleteButtonClicked(wxCommandEvent & evt){
	int i_class, i_group = 0, i_select = m_groups_list->GetSelection();
	School * school = m_owner->m_school;
	if(i_select != wxNOT_FOUND){
		for(i_class = 0; i_class < school->n_classes && i_group <= i_select; ++i_class){

		}
	}
}


ListClassGroupsPane::~ListClassGroupsPane(){

}
