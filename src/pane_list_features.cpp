#include "gui.hpp"

ListFeaturesPane::ListFeaturesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Lista de Características"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	m_features_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));

	wxArrayString list;
	for(i = 0; i < school->n_features; ++i){
		list.Add(wxString::FromUTF8(school->feature_names[i]));
	}
	if(school->n_features > 0){
		m_features_list->InsertItems(list,0);
	}

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(m_features_list, 1, wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();
}

ListFeaturesPane::~ListFeaturesPane(){

}
