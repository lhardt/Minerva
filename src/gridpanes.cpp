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
	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	m_grid = new ChoiceGrid(m_owner, this);

	m_grid->SetCanUserClick(false);

	wxBoxSizer * sz = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * btsz = new wxBoxSizer(wxHORIZONTAL);

	btsz->AddStretchSpacer();
	btsz->Add(m_edit_btn, 0, wxLEFT, 10);
	btsz->Add(m_cancel_btn, 0, wxLEFT, 10);
	sz->Add(m_grid, 0, wxALL, 10);
	sz->Add(btsz, 1, wxLEFT | wxRIGHT | wxBOTTOM, 10);

	SetSizerAndFit(sz);
	m_cancel_btn->Show(false);

	m_edit_btn->Bind(wxEVT_BUTTON, &ScoreGridPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &ScoreGridPane::OnCancelButtonClicked, this);
}

ChoiceGrid * ScoreGridPane::GetGrid(){
	return m_grid;
}

void 	ScoreGridPane::OnEditButtonClicked(wxCommandEvent &evt){
	if(m_grid->m_can_user_click){
		printf("Save clicked\n");
		m_cancel_btn->Show(false);
		m_grid->SetCanUserClick(false);
		m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	} else {
		printf("Edit clicked\n");
		m_cancel_btn->Show(true);
		m_grid->SetCanUserClick(true);
		m_edit_btn->SetLabel(m_owner->m_lang->str_save);
	}
	FitInside();
}
void	ScoreGridPane::OnCancelButtonClicked(wxCommandEvent &evt){
	printf("Cancel clicked\n");
	m_cancel_btn->Show(false);
	m_grid->SetCanUserClick(false);
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
}


wxButton * ScoreGridPane::GetCancelButton(){
	return m_cancel_btn;
}
wxButton * ScoreGridPane::GetEditButton(){
	return m_edit_btn;
}
int		 * ScoreGridPane::GetValues(){
	School * school = m_owner->m_school;
	int * values = (int*) calloc(school->n_periods + 1, sizeof(int));
	values[school->n_periods] = -1;

	for(int i = 0; i < school->n_periods; ++i){
		values[i] = (m_grid->GetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day))==m_owner->m_lang->str_class_available? 1:0);
	}

	return values;
}

//  TODO FIXME
void 	   ScoreGridPane::SetValues(int * values){
	School * school = m_owner->m_school;
	for(int i = 0; i < school->n_periods; ++i){
		if(school->periods[i]){
			m_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
					values[i] > 0?m_owner->m_lang->str_class_available:m_owner->m_lang->str_class_unavailable);
			m_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
					(values[i] > 0?wxColor(200,200,255):wxColor(255,200,200)));
		}
		m_grid->SetReadOnly(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), true);
	}
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

	btsz->AddStretchSpacer();
	btsz->Add(m_edit_btn, 0, wxLEFT, 10);
	btsz->Add(m_cancel_btn, 0, wxLEFT, 10);
	sz->Add(m_grid, 0, wxALL, 10);
	sz->Add(btsz, 1, wxLEFT | wxRIGHT | wxBOTTOM, 10);

	SetSizerAndFit(sz);
	m_grid->EnableEditing(false);
	m_cancel_btn->Show(false);

	m_edit_btn->Bind(wxEVT_BUTTON, &PosIntGridPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &PosIntGridPane::OnCancelButtonClicked, this);
}

wxGrid * PosIntGridPane::GetGrid(){
	return m_grid;
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


void 	PosIntGridPane::OnEditButtonClicked(wxCommandEvent &evt){
	if(m_grid->IsEditable()){
		printf("Save clicked\n");
		m_cancel_btn->Show(false);
		m_grid->EnableEditing(false);
		m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	} else {
		printf("Edit clicked\n");
		m_cancel_btn->Show(true);
		m_grid->EnableEditing(true);
		m_edit_btn->SetLabel(m_owner->m_lang->str_save);
	}
	FitInside();
}
void	PosIntGridPane::OnCancelButtonClicked(wxCommandEvent &evt){
	printf("Cancel clicked\n");
	m_cancel_btn->Show(false);
	m_grid->EnableEditing(false);
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
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
	sz->Add(m_grid, 0, wxALL, 10);
	sz->Add(btsz, 1, wxLEFT | wxRIGHT | wxBOTTOM, 10);

	SetSizerAndFit(sz);
	m_grid->EnableEditing(false);
	m_cancel_btn->Show(false);

	m_edit_btn->Bind(wxEVT_BUTTON, &StringGridPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &StringGridPane::OnCancelButtonClicked, this);
}

wxGrid  * StringGridPane::GetGrid(){
	return m_grid;
}


void 	StringGridPane::OnEditButtonClicked(wxCommandEvent &evt){
	if(m_grid->IsEditable()){
		printf("Save clicked\n");
		m_cancel_btn->Show(false);
		m_grid->EnableEditing(false);
		m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	} else {
		printf("Edit clicked\n");
		m_cancel_btn->Show(true);
		m_grid->EnableEditing(true);
		m_edit_btn->SetLabel(m_owner->m_lang->str_save);
	}
	FitInside();
}

void	StringGridPane::OnCancelButtonClicked(wxCommandEvent &evt){
	printf("Cancel clicked\n");
	m_cancel_btn->Show(false);
	m_grid->EnableEditing(false);
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
}

StringGridPane::~StringGridPane(){

}
