#include "gui.hpp"

#include <wx/richtooltip.h>
#include <wx/tooltip.h>
#include <wx/event.h>

HoverToolTip::HoverToolTip(wxWindow * parent, wxWindowID  id, wxString txt, wxBitmap bmp, wxPoint pos, wxSize sz)
 			: wxStaticBitmap(parent, id, bmp, pos, sz){
	m_text = txt;
	Bind(wxEVT_MOTION, &HoverToolTip::OnHover, this);
}


void HoverToolTip::OnHover(wxMouseEvent& evt){
	SetToolTip(m_text);
	GetToolTip()->Enable(true);
}

HoverToolTip::~HoverToolTip(){

}
