#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

RoomPeriodPrefPane::RoomPeriodPrefPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;

	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * m_title = new wxStaticText(this, wxID_ANY, wxT("Sala: Preferência Por Períodos"), wxDefaultPosition, wxSize(400,30));
	m_title->SetFont(*m_owner->m_page_title_font);

	wxArrayString room_names;
	for(i = 0; i < school->n_rooms; ++i){
		room_names.push_back(wxString::FromUTF8(school->rooms[i].name));
	}
	m_room_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30), room_names);
	wxStaticText * room_choice_label = new wxStaticText(this, wxID_ANY, wxT("A qual sala está se referindo?"), wxDefaultPosition, wxSize(200,20));

	wxButton * save_btn = new wxButton(this, wxID_ANY, wxT("Salvar"), wxDefaultPosition, wxSize(200,30));
	wxButton * reset_btn = new wxButton(this, wxID_ANY, wxT("Limpar"), wxDefaultPosition, wxSize(200,30));

	wxSizer * button_sz = new wxBoxSizer(wxHORIZONTAL);
	button_sz->Add(save_btn, 0, wxRIGHT, 10);
	button_sz->Add(reset_btn, 0, wxRIGHT, 10);

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_title, 0, wxALL, 15);
	sizer->Add(room_choice_label, 0, wxLEFT | wxRIGHT, 15);
	sizer->Add(m_room_choice, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);
	sizer->AddStretchSpacer();
	sizer->Add(button_sz, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);

	wxSizer * buffer_sz = new wxBoxSizer(wxVERTICAL);
	buffer_sz->Add(sizer, 1, wxEXPAND | wxALL, 15);
	this->SetSizerAndFit(buffer_sz);
}
RoomPeriodPrefPane::~RoomPeriodPrefPane(){

}
