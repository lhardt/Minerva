#include "gui.hpp"
extern "C"{
	#include "util.h"
}

AvailabilityPane::AvailabilityPane(Application * owner,
				 wxWindow * parent,
				 wxWindowID id,
				 wxPoint pos,
				 wxSize size) :  wxScrolledWindow(parent, id, pos, size), m_owner(owner){
	School * school = m_owner->m_school;
	int i;
	SetBackgroundColour(wxColour(240,240,240));

	SetFont(*m_owner->m_text_font);

	m_edit_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_edit);
	m_cancel_btn = new wxButton(this, wxID_ANY, m_owner->m_lang->str_cancel);
	m_grid = new ChoiceGrid(this);

	m_grid->SetCanUserClick(false);

	wxVector<wxString> grid_values = wxVector<wxString>();
	grid_values.push_back(m_owner->m_lang->str_class_availible);
	grid_values.push_back(m_owner->m_lang->str_class_unavailible);
	m_grid->SetPossibleValues(grid_values);

	wxVector<wxColor> grid_colors = wxVector<wxColor>();
	grid_colors.push_back(wxColor(200,200,255));
	grid_colors.push_back(wxColor(255,200,200));
	m_grid->SetBackgroundColors(grid_colors);

	m_grid->m_basic_col_name = m_owner->m_lang->str_day;
	m_grid->m_basic_row_name = m_owner->m_lang->str_period;

	m_grid->GridRemake(school->n_days,school->n_periods_per_day);

	wxBoxSizer * sz = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * btsz = new wxBoxSizer(wxHORIZONTAL);

	btsz->AddStretchSpacer();
	btsz->Add(m_edit_btn, 0, wxLEFT, 10);
	btsz->Add(m_cancel_btn, 0, wxLEFT, 10);
	sz->Add(m_grid, 0, wxALL, 10);
	sz->Add(btsz, 1, wxLEFT | wxRIGHT | wxBOTTOM, 10);

	SetSizerAndFit(sz);
	m_cancel_btn->Show(false);

	m_edit_btn->Bind(wxEVT_BUTTON, &AvailabilityPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &AvailabilityPane::OnCancelButtonClicked, this);

	for(i = 0; i < school->n_periods; ++i){
		if(school->periods[i]){
			m_grid->SetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
					wxT(""));
			m_grid->SetCellBackgroundColour(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day),
					wxColor(255,255,255));
		}
	}

}

void 	AvailabilityPane::OnEditButtonClicked(wxCommandEvent &evt){
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
void	AvailabilityPane::OnCancelButtonClicked(wxCommandEvent &evt){
	printf("Cancel clicked\n");
	m_cancel_btn->Show(false);
	m_grid->SetCanUserClick(false);
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
}


wxButton * AvailabilityPane::GetCancelButton(){
	return m_cancel_btn;
}
wxButton * AvailabilityPane::GetEditButton(){
	return m_edit_btn;
}
int		 * AvailabilityPane::GetValues(){
	School * school = m_owner->m_school;
	int * values = (int*) calloc(school->n_periods + 1, sizeof(int));
	values[school->n_periods] = -1;

	for(int i = 0; i < school->n_periods; ++i){
		values[i] = (m_grid->GetCellValue(1 + (i % school->n_periods_per_day),1 +  (i / school->n_periods_per_day))==m_owner->m_lang->str_class_availible? 1:0);
	}

	return values;
}

void 	   AvailabilityPane::SetValues(int * values){
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

AvailabilityPane::~AvailabilityPane(){

}
