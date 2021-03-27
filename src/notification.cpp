#include "gui.hpp"
#include <wx/timer.h>
#include <wx/hyperlink.h>

Notification::Notification(Application * owner, wxWindow * parent, wxWindowID id, wxString text, wxPoint position , wxSize size) : wxPanel(parent, id, position, size){
	SetBackgroundColour(wxColour(0x32,0x32,0x32));

	wxSizer * sz = new wxBoxSizer(wxHORIZONTAL);
	m_text = new wxStaticText(this, wxID_ANY, text, wxDefaultPosition, wxDefaultSize);
	m_text->SetForegroundColour(wxColour(255,255,255));

	sz->Add(m_text, 1, wxALL, 8);
	SetSizer(sz);
}

Notification::Notification(Application * owner, wxWindow * parent, wxWindowID id, Action * action, wxPoint position , wxSize size) : wxPanel(parent, id, position, size){
	SetBackgroundColour(wxColour(0x32,0x32,0x32));

	wxSizer * sz = new wxBoxSizer(wxHORIZONTAL);
	m_text = new wxStaticText(this, wxID_ANY, action->Describe(), wxDefaultPosition, wxDefaultSize);
	// m_button = new wxHyperlinkCtrl(this, wxID_ANY, owner->m_lang->str_undo, wxT(""), wxDefaultPosition, wxDefaultSize);
	m_text->SetForegroundColour(wxColour(255,255,255));
	// m_button->SetFont(*owner->m_bold_text_font);
	// m_button->SetNormalColour(wxColour(0x89,0x03,0x03));
	// m_button->SetHoverColour(wxColour(255,255,255));

	m_associated_action = action;
	/* The action is taller, therefore we don't need vertical margin here. */
	sz->Add(m_text, 0, wxALL, 10);
	sz->AddStretchSpacer();
	// sz->Add(m_button, 0, wxALL |wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(sz);

	// m_button->Bind(wxEVT_HYPERLINK, &Notification::OnHyperlinkAction, this);
	m_timer = new wxTimer();
	m_timer->Bind(wxEVT_TIMER, &Notification::OnCloseTimer, this);
}

Notification::Notification(Application * owner, wxWindow * parent, wxWindowID id, wxString text, wxString action_text, wxPoint position , wxSize size) : wxPanel(parent, id, position, size){
	SetBackgroundColour(wxColour(0x32,0x32,0x32));

	wxSizer * sz = new wxBoxSizer(wxHORIZONTAL);
	m_text = new wxStaticText(this, wxID_ANY, text, wxDefaultPosition, wxDefaultSize);
	m_button = new wxHyperlinkCtrl(this, wxID_ANY, action_text, wxT(""), wxDefaultPosition, wxDefaultSize);
	m_text->SetForegroundColour(wxColour(255,255,255));
	m_button->SetFont(*owner->m_bold_text_font);
	m_button->SetNormalColour(wxColour(0x89,0x03,0x03));
	m_button->SetHoverColour(wxColour(255,255,255));

	/* The action is taller, therefore we don't need vertical margin here. */
	sz->Add(m_text, 0, wxLEFT |wxALIGN_CENTER_VERTICAL, 20);
	sz->AddStretchSpacer();
	sz->Add(m_button, 0, wxALL |wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(sz);

	m_button->Bind(wxEVT_HYPERLINK, &Notification::OnHyperlinkAction, this);
	m_timer = new wxTimer();
	m_timer->Bind(wxEVT_TIMER, &Notification::OnCloseTimer, this);
}

void Notification::Start(int ms){
	m_timer->Start(ms);
}

wxHyperlinkCtrl* Notification::GetButton(){
	return m_button;
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


void Notification::OnHyperlinkAction(wxHyperlinkEvent &){
	if(m_associated_action != NULL){
		m_associated_action->Undo();
	}
	Close();
}
