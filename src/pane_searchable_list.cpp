#include "gui.hpp"

SearchableListPane::SearchableListPane(Application * owner, wxWindow * parent, wxWindowID id, wxPoint pos, wxSize sz)
				: wxPanel(parent, id, pos, sz), m_owner(owner), m_item_ids(), m_item_names() {
	SetBackgroundColour(wxColour(250,250,250));

	wxStaticText * search_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_search);
	wxStaticText * selection_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_selection);

	search_label->SetFont(*m_owner->m_small_font);
	selection_label->SetFont(*m_owner->m_small_font);

	m_searchbox = new wxTextCtrl(this, wxID_ANY, wxT(""),wxDefaultPosition, wxSize(200,-1));
	m_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(200,200), 0, NULL, wxLB_SORT);
	wxButton * clear_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_clear, wxDefaultPosition, wxSize(200,30));

	m_list->Bind(wxEVT_LISTBOX, &SearchableListPane::OnListBoxClick, this);
	m_searchbox->Bind(wxEVT_TEXT, &SearchableListPane::OnSearchBoxEdit, this);
	clear_btn->Bind(wxEVT_BUTTON, &SearchableListPane::OnClearButtonClicked, this);

	wxSizer * sizer = new wxStaticBoxSizer(wxVERTICAL, this, m_owner->m_lang->str_selection_and_search);
	sizer->Add(search_label, 0, wxLEFT | wxRIGHT | wxTOP, 10);
	sizer->Add(m_searchbox, 0, wxLEFT | wxRIGHT, 10);
	sizer->Add(selection_label, 0, wxLEFT | wxRIGHT | wxTOP, 10);
	sizer->Add(m_list, 1,  wxLEFT | wxRIGHT | wxBOTTOM, 10);
	sizer->Add(clear_btn, 0,  wxLEFT | wxRIGHT | wxBOTTOM, 10);
	SetSizer(sizer);
}

void SearchableListPane::Clear(){
	m_searchbox->Clear();
	m_item_ids.clear();
	m_item_names.clear();
	m_list->Clear();
}

void SearchableListPane::EditItem(int id, wxString new_name){
	int i;
	for(i = 0; i < m_item_ids.size(); ++i){
		if(m_item_ids[i] == id){
			m_item_names[i] = new_name;
			RefilterElements();
			break;
		}
	}
}

void SearchableListPane::OnClearButtonClicked(wxCommandEvent &){
	m_searchbox->Clear();
	RefilterElements(); /* It sometimes does not call wxEVT_TEXT on Clear() */
}

void SearchableListPane::AddItem(int id, wxString name) {
	m_item_ids.push_back(id);
	m_item_names.push_back(name);
	RefilterElements();
}

void SearchableListPane::RemoveItem(int id) {
	int i;
	for(i = 0; i < m_item_ids.size(); ++i){
		if(m_item_ids[i] == id){
			m_item_ids.erase(m_item_ids.begin() + i);
			m_item_names.erase(m_item_names.begin() + i);
			RefilterElements();
			break;
		}
	}
}

void SearchableListPane::RefilterElements(){
	int i;
	wxString search_str = m_searchbox->GetValue();
	m_list->Clear();
	for(i = 0; i < m_item_ids.size(); ++i){
		if(m_item_names[i].StartsWith(search_str)){
			m_list->Append(m_item_names[i], new IntClientData(m_item_ids[i]));
		}
	}
}

wxListBox * SearchableListPane::GetList() {
	return m_list;
}

wxTextCtrl * SearchableListPane::GetSearchBox(){
	return m_searchbox;
}

void SearchableListPane::OnSearchBoxEdit(wxCommandEvent &) {
	RefilterElements();
}

void SearchableListPane::OnListBoxClick(wxCommandEvent &) {
	RefilterElements();
}
