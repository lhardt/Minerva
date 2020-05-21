#include "gui.hpp"

ListTeacherGroupsPane::ListTeacherGroupsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));
}

ListTeacherGroupsPane::~ListTeacherGroupsPane(){

}
