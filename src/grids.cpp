#include "gui.hpp"

/* Credits to RemcoM in the wxWidgets forum for the NoSelect renderer. */
class NoSelectGridCellRenderer : public wxGridCellStringRenderer {
public:
	virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected){
		wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, false);
	}
};

wxString PosIntGridTable::GetRowLabelValue(int row){
	return row_labels[row];
}

wxString PosIntGridTable::GetColLabelValue(int col){
	return col_labels[col];
}

void PosIntGridTable::SetRowLabelValue(int row, wxString & str){
	row_labels.insert(row_labels.begin()+row,str);
}

void PosIntGridTable::SetColLabelValue(int col, wxString & str){
	col_labels.insert(col_labels.begin()+col, str);
}

bool PosIntGridTable::AppendCols(unsigned long n_new_cols){
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

bool PosIntGridTable::AppendRows(unsigned long n_new_rows){
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

PosIntGridTable::PosIntGridTable(int n_rows, int n_cols) : wxGridTableBase(){
	this->n_cols = n_cols;
	this->n_rows = n_rows;
	values = (int*)calloc(n_rows * n_cols, sizeof(int));
	row_labels = wxVector<wxString>(n_rows);
	col_labels = wxVector<wxString>(n_cols);
}
PosIntGridTable::~PosIntGridTable(){
	free(values);
}

int PosIntGridTable::GetNumberRows(){
	return n_rows;
}
int PosIntGridTable::GetNumberCols(){
	return n_cols;
}
bool PosIntGridTable::IsEmptyCell( int row, int col ){
	return (values[col * n_rows + row] < 0);
}
wxString PosIntGridTable::GetValue( int row, int col ){
	if(row < n_rows && col < n_cols){
		return wxString::Format("%d", values[col * n_rows + row]);
	} else {
		return wxT("");
	}
}
void PosIntGridTable::SetValue( int row, int col, const wxString& value ){
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


ChoiceGrid::ChoiceGrid(Application * owner, wxWindow * parent, wxWindowID id, wxPoint position, wxSize size) : wxGrid(parent,id,position,size), m_owner(owner){
	Bind(wxEVT_GRID_CELL_LEFT_CLICK, &ChoiceGrid::OnLeftClick, this);
	SetDefaultRenderer(new NoSelectGridCellRenderer);
	CreateGrid(1,1);
	HideColLabels();
	HideRowLabels();

	m_basic_col_name=wxT("");
	m_basic_row_name=wxT("");
	m_row_names=wxVector<wxString>();
	m_col_names=wxVector<wxString>();
	m_value_names=wxVector<wxString>();
	m_background_colors=wxVector<wxColour>();
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

	if(n_rows > old_n_rows){
		AppendRows(n_rows - old_n_rows);
		for(i = old_n_rows; i < n_rows; ++i){

			if(i > 0){
				if(m_row_names.size() < i){
					SetCellValue(i,0, m_basic_row_name + wxString::Format(" %d",i));
				} else {
						SetCellValue(i,0, m_row_names[i-1]);
				}
				SetCellFont(i,0, *m_owner->m_bold_text_font);
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
			if(j > 0){
				if(m_col_names.size() < j){
					SetCellValue(0, j, m_basic_col_name + wxString::Format(" %d",j));
				} else {
					SetCellValue(0, j, m_col_names[j-1]);
				}
				SetCellFont(0,j, *m_owner->m_bold_text_font);
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

void ChoiceGrid::SetCanUserClick(bool can){
	m_can_user_click = can;
}

void ChoiceGrid::OnLeftClick(wxGridEvent & evt){
	int i = 0;

	if(m_can_user_click && m_value_names.size() > 0){
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
	}

}

void ChoiceGrid::SetCellImmutable(int i, int j){
	SetCellValue(i,j, m_immutable_cell_text);
	SetCellBackgroundColour(i,j,m_immutable_cell_color);
}

void ChoiceGrid::SetCellState(int i_row, int i_col, int state){
	if(i_col < GetNumberCols() && i_row < GetNumberRows()){
		if(state == -1){
			SetCellValue(i_row + 1, i_col + 1, m_immutable_cell_text);
			SetCellBackgroundColour(i_row + 1, i_col + 1,m_immutable_cell_color);
		} else if(state < m_value_names.size() && state < m_background_colors.size()){
			SetCellValue(i_row + 1, i_col + 1, m_value_names[state]);
			SetCellBackgroundColour(i_row + 1, i_col + 1, m_background_colors[state]);
		}
	}
}

void ChoiceGrid::SetAllCellsState(int state){
	/* Inneficiently elegant. Refactor before production*/
	for(int i = 0; i < GetNumberRows() -1; ++i){
		for(int j = 0; j < GetNumberCols() -1; ++j){
			SetCellState(i,j,state);
		}
	}
}

int ChoiceGrid::GetCellState(int i_row, int i_col){
	if(i_col < GetNumberCols() && i_row < GetNumberRows()){
		wxColor bgcolor = GetCellBackgroundColour(i_row + 1, i_col + 1);
		for(int i = 0; i < m_background_colors.size(); ++i){
			if(bgcolor == m_background_colors[i]){
				return i;
			}
		}
		/* The immutable might not have been set yet */
		if(bgcolor == m_immutable_cell_color){
			return -1;
		}
		printf("Something is wrong. GetCellState with invalid/blank cell");
	}
	return -1;
}

int  ChoiceGrid::AddState(wxString state_name, wxColor state_value){
	m_value_names.push_back(state_name);
	m_background_colors.push_back(state_value);
}

void ChoiceGrid::SetColName(int i_col, wxString name){
	if(m_col_names.size() <= i_col){
		m_col_names.resize(i_col+1);
	}
	m_col_names[i_col] = name;
	if(GetNumberCols() > 1+i_col){
		SetCellValue(0,1 + i_col, name);
		SetCellFont(0,1 + i_col, *m_owner->m_bold_text_font);
	}
}
void ChoiceGrid::SetRowName(int i_row, wxString name){
	if(m_row_names.size() <= i_row){
		m_row_names.resize(i_row+1);
	}
	m_row_names[i_row] = name;
	if(GetNumberRows() > 1+ i_row){
		SetCellValue(1+i_row,0, name);
		SetCellFont(1+i_row, 0, *m_owner->m_bold_text_font);
	}
}
