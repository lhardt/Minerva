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
	m_grid = new ChoiceGrid(this);

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
		values[i] = (m_grid->GetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day))==m_owner->m_lang->str_class_availible? 1:0);
	}

	return values;
}

void 	   ScoreGridPane::SetValues(int * values){
	School * school = m_owner->m_school;
	for(int i = 0; i < school->n_periods; ++i){
		if(school->periods[i]){
			m_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
					values[i] > 0?m_owner->m_lang->str_class_availible:m_owner->m_lang->str_class_unavailible);
			m_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
					(values[i] > 0?wxColor(200,200,255):wxColor(255,200,200)));
		}
		m_grid->SetReadOnly(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day), true);
	}

}

ScoreGridPane::~ScoreGridPane(){

}
