#include "gui.hpp"
extern "C"{
	#include "util.h"
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

	wxString col_name = m_owner->m_lang->str_periods;
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
	m_grid->SetEditable(false);
	m_cancel_btn->Show(false);

	m_edit_btn->Bind(wxEVT_BUTTON, &PosIntGridPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &PosIntGridPane::OnCancelButtonClicked, this);
}


void 	PosIntGridPane::OnEditButtonClicked(wxCommandEvent &evt){
	if(m_grid->IsEditable()){
		printf("Save clicked\n");
		m_cancel_btn->Show(false);
		m_grid->SetEditable(false);
		m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
	} else {
		printf("Edit clicked\n");
		m_cancel_btn->Show(true);
		m_grid->SetEditable(true);
		m_edit_btn->SetLabel(m_owner->m_lang->str_save);
	}
	FitInside();
}
void	PosIntGridPane::OnCancelButtonClicked(wxCommandEvent &evt){
	printf("Cancel clicked\n");
	m_cancel_btn->Show(false);
	m_grid->SetEditable(false);
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
}

PosIntGridPane::~PosIntGridPane(){

}
