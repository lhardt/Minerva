#include "gui.hpp"
#include <wx/timer.h>
#include <wx/hyperlink.h>

// class Notification : public wxPanel {
// public:
// 	Notification(wxWindow * parent, wxWindowID id = wxID_ANY, wxString text, wxPoint position = wxDefaultPosition, wxSize size = wxDefaultSize);
// 	Notification(wxWindow * parent, wxWindowID id = wxID_ANY, wxString text,  wxString action_str, wxPoint position = wxDefaultPosition, wxSize size = wxDefaultSize);
//
// 	wxHyperlinkCtrl * GetAction();
// private:
// 	wxStaticText * m_text;
// 	wxHyperlinkCtrl * action;
// 	/* Just to prevent skipping of the event. */
// 	void OnHyperlikAction(wxHyperlinkEvent &);
// };

Notification::Notification(Application * owner, wxWindow * parent, wxWindowID id, wxString text, wxPoint position , wxSize size) : wxPanel(parent, id, position, size){
	SetBackgroundColour(wxColour(0x32,0x32,0x32));

	wxSizer * sz = new wxBoxSizer(wxHORIZONTAL);
	m_text = new wxStaticText(this, wxID_ANY, text, wxDefaultPosition, wxDefaultSize);
	m_text->SetForegroundColour(wxColour(255,255,255));

	sz->Add(m_text, 1, wxALL, 8);
	SetSizer(sz);
}

Notification::Notification(Application * owner, wxWindow * parent, wxWindowID id, wxString text, wxString action_text, wxPoint position , wxSize size) : wxPanel(parent, id, position, size){
	SetBackgroundColour(wxColour(0x32,0x32,0x32));

	wxSizer * sz = new wxBoxSizer(wxHORIZONTAL);
	m_text = new wxStaticText(this, wxID_ANY, text, wxDefaultPosition, wxDefaultSize);
	m_action = new wxHyperlinkCtrl(this, wxID_ANY, action_text, wxT(""), wxDefaultPosition, wxDefaultSize);
	m_text->SetForegroundColour(wxColour(255,255,255));
	m_action->SetFont(*owner->m_bold_text_font);
	m_action->SetNormalColour(wxColour(0x89,0x03,0x03));
	m_action->SetHoverColour(wxColour(255,255,255));

	/* The action is taller, therefore we don't need vertical margin here. */
	sz->Add(m_text, 0, wxLEFT |wxALIGN_CENTER_VERTICAL, 20);
	sz->AddStretchSpacer();
	sz->Add(m_action, 0, wxALL |wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(sz);

	m_action->Bind(wxEVT_HYPERLINK, &Notification::OnHyperlikAction, this);
	m_timer = new wxTimer();
	m_timer->Bind(wxEVT_TIMER, &Notification::OnCloseTimer, this);
}

void Notification::Start(int ms){
	m_timer->Start(ms);
}

wxHyperlinkCtrl* Notification::GetAction(){
	return m_action;
}

wxTimer* Notification::GetTimer(){
	return m_timer;
}

void Notification::OnCloseTimer(wxTimerEvent &){
	Close();
}

void Notification::Close(){
	Destroy();
}


void Notification::OnHyperlikAction(wxHyperlinkEvent &){
	printf("Aktion!\n");
	Close();
}
