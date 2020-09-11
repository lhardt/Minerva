#include "gui.hpp"
extern "C"{
	#include "util.h"
}

ScoreGridPane::ScoreGridPane(Application * owner,
							 wxWindow * parent,
							 wxWindowID id,
							 wxPoint pos,
							 wxSize size) :  wxScrolledWindow(parent, id, pos, size){
	int i;
	SetBackgroundColour(wxColour(240,240,240));
	this->m_owner = owner;

	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_save_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_save);
	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	m_grid = new ChoiceGrid(m_owner, this);

	m_grid->SetCanUserClick(false);

	wxBoxSizer * sz = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * btsz = new wxBoxSizer(wxHORIZONTAL);

	btsz->Add(m_edit_btn, 0, wxLEFT, 10);
	btsz->Add(m_cancel_btn, 0, wxLEFT, 10);
	btsz->Add(m_save_btn, 0, wxLEFT, 10);
	sz->Add(m_grid, 0, wxALL, 10);
	sz->Add(btsz, 1, wxLEFT | wxRIGHT | wxBOTTOM, 10);

	SetSizerAndFit(sz);
	m_cancel_btn->Show(false);
	m_save_btn->Show(false);

	m_edit_btn->Bind(wxEVT_BUTTON, &ScoreGridPane::OnEditButtonClicked, this);
	m_save_btn->Bind(wxEVT_BUTTON, &ScoreGridPane::OnSaveButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &ScoreGridPane::OnCancelButtonClicked, this);
}

void ScoreGridPane::SetEditing(bool editing){
	m_cancel_btn->Show(editing);
	m_save_btn->Show(editing);
	m_edit_btn->Show(!editing);
	m_grid->SetCanUserClick(editing);
	FitInside();
}

ChoiceGrid * ScoreGridPane::GetGrid(){
	return m_grid;
}

void 	ScoreGridPane::OnSaveButtonClicked(wxCommandEvent &evt){
	SetEditing(false);
}

void 	ScoreGridPane::OnEditButtonClicked(wxCommandEvent &evt){
	SetEditing(true);
}

void	ScoreGridPane::OnCancelButtonClicked(wxCommandEvent &evt){
	SetEditing(false);
}

wxButton * ScoreGridPane::GetCancelButton(){
	return m_cancel_btn;
}

wxButton * ScoreGridPane::GetEditButton(){
	return m_edit_btn;
}

wxButton * ScoreGridPane::GetSaveButton(){
	return m_save_btn;
}

ScoreGridPane::~ScoreGridPane(){

}

PosIntGridPane::PosIntGridPane(Application * owner,
				 wxWindow * parent,
				 wxWindowID id,
				 wxPoint pos,
				 wxSize size,
				 wxString column_name,
				 wxVector<wxString> row_names) :  wxScrolledWindow(parent, id, pos, size), m_owner(owner){
 	int i;
 	SetBackgroundColour(wxColour(245,245,245));

 	SetFont(*m_owner->m_text_font);

 	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_save_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_save);
 	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	m_grid = new wxGrid(this, wxID_ANY);
	PosIntGridTable * grid_table = new PosIntGridTable(row_names.size(),1);

	grid_table->SetColLabelValue(0, column_name);
	for(i = 0; i < row_names.size(); ++i){
		grid_table->SetRowLabelValue(i, row_names[i]);
	}
	m_grid->SetTable(grid_table, true);
	m_grid->AutoSizeColumn(0, true);

	wxBoxSizer * sz = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * btsz = new wxBoxSizer(wxHORIZONTAL);

	btsz->Add(m_edit_btn, 0, wxLEFT, 10);
	btsz->Add(m_cancel_btn, 0, wxLEFT, 10);
	btsz->Add(m_save_btn, 0, wxLEFT, 10);
	sz->Add(m_grid, 0, wxALL, 10);
	sz->Add(btsz, 1, wxLEFT | wxRIGHT | wxBOTTOM, 10);

	SetSizerAndFit(sz);
	m_grid->EnableEditing(false);
	m_cancel_btn->Show(false);
	m_save_btn->Show(false);

	m_edit_btn->Bind(wxEVT_BUTTON, &PosIntGridPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &PosIntGridPane::OnCancelButtonClicked, this);
}

void PosIntGridPane::SetEditing(bool editing){
	m_cancel_btn->Show(editing);
	m_save_btn->Show(editing);
	m_edit_btn->Show(!editing);
	m_grid->EnableEditing(editing);
	FitInside();
}

wxGrid * PosIntGridPane::GetGrid(){
	return m_grid;
}

wxButton * PosIntGridPane::GetSaveButton(){
	return m_save_btn;
}

wxButton * PosIntGridPane::GetCancelButton(){
	return m_cancel_btn;
}

void 	   PosIntGridPane::SetValues(int * values){
	School * school = m_owner->m_school;
	if(values == NULL){
		for(int i = 0; i < m_grid->GetNumberRows(); ++i){
			m_grid->SetCellValue(i, 0, wxString::Format("%d", 0));
		}
	} else {
		for(int i = 0; i < m_grid->GetNumberRows() && values[i] >= 0; ++i){
			m_grid->SetCellValue(i, 0, wxString::Format("%d", values[i]));
		}
	}
}

void 	PosIntGridPane::OnSaveButtonClicked(wxCommandEvent &){
	SetEditing(false);
}

void 	PosIntGridPane::OnEditButtonClicked(wxCommandEvent &evt){
	SetEditing(true);
}

void	PosIntGridPane::OnCancelButtonClicked(wxCommandEvent &evt){
	SetEditing(false);
}

PosIntGridPane::~PosIntGridPane(){

}

StringGridPane::StringGridPane(Application * owner,
				 wxWindow * parent,
				 wxWindowID id,
				 wxPoint pos,
				 wxSize size,
				 wxString column_name,
				 wxVector<wxString> row_names) :  wxScrolledWindow(parent, id, pos, size), m_owner(owner){
 	int i;
 	SetBackgroundColour(wxColour(245,245,245));

 	SetFont(*m_owner->m_text_font);

 	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_save_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_save);
 	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	m_grid = new wxGrid(this, wxID_ANY);
	m_grid->CreateGrid(row_names.size(),1);

	m_grid->SetColLabelValue(0, column_name);
	for(i = 0; i < row_names.size(); ++i){
		m_grid->SetRowLabelValue(i, row_names[i]);
	}
	m_grid->AutoSizeColumn(0, true);

	wxBoxSizer * sz = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * btsz = new wxBoxSizer(wxHORIZONTAL);

	btsz->AddStretchSpacer();
	btsz->Add(m_edit_btn, 0, wxLEFT, 10);
	btsz->Add(m_cancel_btn, 0, wxLEFT, 10);
	btsz->Add(m_save_btn, 0, wxLEFT, 10);
	sz->Add(m_grid, 0, wxALL, 10);
	sz->Add(btsz, 1, wxLEFT | wxRIGHT | wxBOTTOM, 10);

	SetSizerAndFit(sz);
	m_grid->EnableEditing(false);
	m_cancel_btn->Show(false);
	m_save_btn->Show(false);

	m_edit_btn->Bind(wxEVT_BUTTON, &StringGridPane::OnEditButtonClicked, this);
	m_save_btn->Bind(wxEVT_BUTTON, &StringGridPane::OnSaveButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &StringGridPane::OnCancelButtonClicked, this);
}

void StringGridPane::SetEditing(bool editing){
	m_cancel_btn->Show(editing);
	m_save_btn->Show(editing);
	m_edit_btn->Show(!editing);
	m_grid->EnableEditing(editing);
	FitInside();
}

wxGrid  * StringGridPane::GetGrid(){
	return m_grid;
}

wxButton * StringGridPane::GetSaveButton(){
	return m_save_btn;
}

wxButton * StringGridPane::GetCancelButton(){
	return m_cancel_btn;
}

void 	StringGridPane::OnSaveButtonClicked(wxCommandEvent &evt){
	SetEditing(false);
}

void 	StringGridPane::OnEditButtonClicked(wxCommandEvent &evt){
	SetEditing(true);
}

void	StringGridPane::OnCancelButtonClicked(wxCommandEvent &evt){
	SetEditing(false);
}

StringGridPane::~StringGridPane(){

}
