#include "gui.hpp"

ChoiceGrid::ChoiceGrid(wxWindow * parent, wxWindowID id, wxPoint position, wxSize size) : wxGrid(parent,id,position,size){
	Bind(wxEVT_GRID_CELL_LEFT_CLICK, &ChoiceGrid::OnLeftClick, this);

	CreateGrid(1,1);
	HideColLabels();
	HideRowLabels();
}
ChoiceGrid::~ChoiceGrid(){

}

void ChoiceGrid::GridRemake(int n_cols, int n_rows){
	unsigned int i = 0,j = 0;

	++n_cols; ++n_rows;

	int old_n_rows = GetNumberRows();
	int old_n_cols = GetNumberCols();

	EnableEditing(false);

	m_immutable_cell_text = wxT("");
	m_immutable_cell_color = wxColor(200, 200, 200);

	SetCellBackgroundColour(0,0,wxColor(200,200,200));
	if(n_rows > old_n_rows){
		AppendRows(n_rows - old_n_rows);
		for(i = old_n_rows; i < n_rows; ++i){

			if(m_row_names.size() <= i){
				SetCellValue(i,0, m_basic_row_name + wxString::Format(" %d",i));
			} else {
				SetCellValue(i,0, m_row_names[j]);
			}
			for(j = 1; j < old_n_cols; ++j){
				if(m_background_colors.size() > 0){
					SetCellBackgroundColour(i, j, m_background_colors.at(0));
				}
				if(m_value_names.size() > 0){
					SetCellValue(i, j, m_value_names.at(0));
				}
				SetReadOnly(i,j,true);
			}
		}
	} else if(n_rows < old_n_rows) {
		DeleteRows(n_rows, old_n_rows - n_rows);
	}

	if(n_cols > old_n_cols){
		AppendCols(n_cols - old_n_cols);
		for(j = old_n_cols; j < n_cols; ++j){
			if(m_col_names.size() <= j){
				SetCellValue(0, j, m_basic_col_name + wxString::Format(" %d",j));
			} else {
				SetCellValue(0, j, m_col_names[j]);
			}

			for(i = 1; i < n_rows; ++i){
				if(m_background_colors.size() > 0){
					SetCellBackgroundColour(i, j, m_background_colors.at(0));
				}
				if(m_value_names.size() > 0){
					SetCellValue(i, j, m_value_names.at(0));
				}
				SetReadOnly(i,j,true);
			}
		}
	} else if(n_cols < old_n_cols){
		DeleteCols(n_cols, old_n_cols - n_cols);
	}

	m_n_cols = n_cols -1;
	m_n_rows = n_rows -1;

	Refresh();
}

void ChoiceGrid::SetPossibleValues(wxVector<wxString> values){
	m_value_names = values;
}

void ChoiceGrid::SetBackgroundColors(wxVector<wxColour> values){
	m_background_colors = values;
}

void ChoiceGrid::OnLeftClick(wxGridEvent & evt){
	int i = 0;

	if(m_value_names.size() > 0){
		if(evt.GetCol() > 0 && evt.GetRow() > 0){
			wxString evt_str = GetCellValue(evt.GetRow(), evt.GetCol());
			for( i = 0; i < m_value_names.size(); ++i){
				if(evt_str.IsSameAs(m_value_names.at(i))){
					break;
				}
			}
			if(i != m_value_names.size()){
				// Loop to zero if necessary -- goes to zero if not found
				i = (i + 1)%(m_value_names.size());
				if(m_background_colors.size() > i){
					SetCellBackgroundColour(evt.GetRow(), evt.GetCol(), m_background_colors.at(i));
				} else {
					SetCellBackgroundColour(evt.GetRow(), evt.GetCol(), wxColor(255,255,255));
				}
				SetCellValue(evt.GetRow(), evt.GetCol(), m_value_names.at(i));
					evt.Skip();
				Refresh();
			}
		}
	} else {
		printf("No possible values. \n");
	}

}

void ChoiceGrid::SetCellImmutable(int i, int j){
	SetCellValue(i,j, m_immutable_cell_text);
	SetCellBackgroundColour(i,j,m_immutable_cell_color);
}
