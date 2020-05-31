#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

SubjectRoomPrefPane::SubjectRoomPrefPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	this->m_owner = owner;
	SetBackgroundColour(wxColour(240,240,240));
}
SubjectRoomPrefPane::~SubjectRoomPrefPane(){
	
}