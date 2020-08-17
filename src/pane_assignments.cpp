#include "gui.hpp"
extern "C"{
	#include "util.h"
}
#include <iostream>

class PosIntGridTable : public wxGridTableBase{
public:
	PosIntGridTable(int n_rows, int n_cols) : wxGridTableBase(){
		this->n_cols = n_cols;
		this->n_rows = n_rows;
		values = (int*)calloc(n_cols * n_cols, sizeof(int));

		row_labels = wxVector<wxString>(n_rows);
		col_labels = wxVector<wxString>(n_cols);
	}
	~PosIntGridTable(){
		free(values);
	}

	virtual int GetNumberRows(){
		return n_rows;
	}
	virtual int GetNumberCols(){
		return n_cols;
	}
	virtual bool IsEmptyCell( int row, int col ){
		return (values[col * n_rows + row] < 0);
	}
	virtual wxString GetValue( int row, int col ){
		if(row < n_rows && col < n_cols){
			return wxString::Format("%d", values[col * n_rows + row]);
		} else {
			return wxT("");
		}
	}
	virtual void SetValue( int row, int col, const wxString& value ){
		long long_val = -1L;
		int int_val = -1;
		if(value.ToLong(&long_val)){
			int_val = (int) long_val;
			if(row >= n_rows || col >= n_cols){
				int max_rows = (row + 1)>n_rows?(row+1):n_rows;
				int max_cols = (col + 1)>n_cols?(col+1):n_cols;
				if(values == NULL){
					values = (int*)calloc(max_rows * max_cols, sizeof(int));
				} else {
					values = (int*)realloc(values, (max_rows * max_cols)*sizeof(int));
				}
			}
			if(int_val >= 0){
				values[col * n_rows + row] = int_val;
			}
		}
	}

	virtual bool AppendRows(unsigned long n_new_rows){
		n_rows += n_new_rows;
		if(n_cols > 0 && n_rows > 0){
			if(values == NULL){
				values = (int*)calloc(n_rows * n_cols, sizeof(int));
			} else {
				values = (int*)realloc(values, (n_rows * n_cols)*sizeof(int));
			}
		}
		return true;
	}


	virtual bool AppendCols(unsigned long n_new_cols){
		n_cols += (int)n_new_cols;
		if(n_cols > 0 && n_rows > 0){
			if(values == NULL){
				values = (int*)calloc(n_rows * n_cols, sizeof(int));
			} else {
				values = (int*)realloc(values, (n_rows * n_cols)*sizeof(int));
			}
		}
		return true;
	}

	virtual void SetColLabelValue(int col, wxString & str){
		col_labels.insert(col_labels.begin()+col, str);
	}

	virtual void SetRowLabelValue(int row, wxString & str){
		row_labels.insert(row_labels.begin()+row,str);
	}

	virtual wxString GetColLabelValue(int col){
		return col_labels[col];
	}

	virtual wxString GetRowLabelValue(int row){
		return row_labels[row];
	}
private:
	int n_rows;
	int n_cols;
	int * values;
	wxVector<wxString> col_labels;
	wxVector<wxString> row_labels;
};

AssignmentsPane::AssignmentsPane(Application * owner,
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
	m_grid = new wxGrid(this, wxID_ANY);
	PosIntGridTable * grid_table = new PosIntGridTable(school->n_subjects,1);

	wxString col_name = m_owner->m_lang->str_periods;
	grid_table->SetColLabelValue(0, col_name);
	for(i = 0; i < school->n_subjects; ++i){
		wxString row_name = wxString::FromUTF8(m_owner->m_school->subjects[i].name);
		grid_table->SetRowLabelValue(i, row_name);
	}
	m_grid->SetTable(grid_table, true);

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

	m_edit_btn->Bind(wxEVT_BUTTON, &AssignmentsPane::OnEditButtonClicked, this);
	m_cancel_btn->Bind(wxEVT_BUTTON, &AssignmentsPane::OnCancelButtonClicked, this);
}


void 	AssignmentsPane::OnEditButtonClicked(wxCommandEvent &evt){
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
void	AssignmentsPane::OnCancelButtonClicked(wxCommandEvent &evt){
	printf("Cancel clicked\n");
	m_cancel_btn->Show(false);
	m_grid->SetEditable(false);
	m_edit_btn->SetLabel(m_owner->m_lang->str_edit);
}

AssignmentsPane::~AssignmentsPane(){

}
