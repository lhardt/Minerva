#include "gui.hpp"
#include <wx/grid.h>

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

void PosIntGridTable::SetRowLabelValue(int row, const wxString & str){
	row_labels.insert(row_labels.begin()+row,str);
}

void PosIntGridTable::SetColLabelValue(int col, const wxString & str){
	col_labels.insert(col_labels.begin()+col, str);
}

bool PosIntGridTable::AppendCols(size_t n_new_cols){
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

bool PosIntGridTable::AppendRows(size_t n_new_rows){
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
	row_labels = wxVector<wxString>();
	col_labels = wxVector<wxString>();
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

ChoiceGridTable::ChoiceGridTable(int n_rows, int n_cols) : wxGridTableBase() {
	this->n_cols = n_cols;
	this->n_rows = n_rows;
	if(n_rows > 0 && n_cols > 0){
		values = (int*)calloc(n_rows * n_cols, sizeof(int));
	}
	row_labels = wxVector<wxString>();
	col_labels = wxVector<wxString>();
	value_labels = wxVector<wxString>();
	value_colors = wxVector<wxColor>();
}

wxString ChoiceGridTable::GetRowLabelValue(int row){
	printf("Getting row %d label, with rowlabelssize %d\n", row, row_labels.size());
	if(row < row_labels.size()){
		return row_labels[row];
	} else {
		return wxString::Format("%s %d", default_row_label, row + 1);
	}
}

wxString ChoiceGridTable::GetColLabelValue(int col){
	if(col >= 0 && col < col_labels.size()){
		return col_labels[col];
	} else {
		return wxString::Format("%s %d", default_col_label, col + 1);
	}
}

void ChoiceGridTable::SetRowLabelValue(int row, const wxString & str){
	if(row_labels.size() <= row){
		row_labels.resize(row+1);
	}
	row_labels[row] = str;
}

void ChoiceGridTable::SetColLabelValue(int col, const wxString & str){
	if(col_labels.size() <= col){
		col_labels.resize(col+1);
	}
	col_labels[col] = str;
}

bool ChoiceGridTable::AppendCols(size_t n_new_cols){
	int old_n_cols = n_cols;
	n_cols += (int)n_new_cols;
	if(n_cols > 0 && n_rows > 0){
		if(values == NULL){
			values = (int*)calloc(n_rows * n_cols, sizeof(int));
		} else {
			values = (int*)realloc(values, (n_rows * n_cols)*sizeof(int));
			for(int i = old_n_cols; i < n_cols * n_rows; ++i){
				values[i] = 0;
			}
		}
	}
	wxGridTableMessage pop(this, wxGRIDTABLE_NOTIFY_COLS_APPENDED, n_new_cols);
	wxGrid * view = GetView();
	view->BeginBatch();
	view->ProcessTableMessage(pop);
	view->ForceRefresh();
	view->EndBatch();

	printf("Appended colx\n");
	return true;
}

bool ChoiceGridTable::AppendRows(size_t n_new_rows){
	int old_n_rows = n_rows;
	n_rows += (int)n_new_rows;
	if(n_cols > 0 && n_rows > 0){
		if(values == NULL){
			values = (int*)calloc(n_rows * n_cols, sizeof(int));
		} else {
			values = (int*)realloc(values, (n_rows * n_cols)*sizeof(int));
			// TODO! Transpose values from the old to the new. it's not exactly trivial
			for(int i = 0; i < n_cols * n_rows; ++i){
				values[i] = 0;
			}
		}
	}
	wxGrid * view = GetView();
	view->BeginBatch();
	wxGridTableMessage pop(this, wxGRIDTABLE_NOTIFY_ROWS_APPENDED, n_new_rows);
	view->ProcessTableMessage(pop);
	// view->ForceRefresh();
	view->EndBatch();

	printf("Appended rows. n rows is now %d\n", n_rows);
	return true;
}
ChoiceGridTable::~ChoiceGridTable(){
	free(values);
}

int ChoiceGridTable::GetNumberRows(){
	return n_rows;
}
int ChoiceGridTable::GetNumberCols(){
	return n_cols;
}
bool ChoiceGridTable::IsEmptyCell( int row, int col ){
	return (values[col * n_rows + row] < 0);
}
wxString ChoiceGridTable::GetValue( int row, int col ){
	if(row < n_rows && col < n_cols){
		int val = values[col * n_rows + row];
		if(val >= 0 && val < value_labels.size()){
			return value_labels[val];
		} else if(val == -1){
			return wxT("");
		} else {
			return wxString::Format("??? (%d)", val);
		}
	} else {
		return wxT("");
	}
}
int ChoiceGridTable::GetState( int row, int col ){
	if(row >= 0 && col >= 0 && row < n_rows && col < n_cols){
		return values[col * n_rows + row];
	}
	return -1;
}
void ChoiceGridTable::SetValue( int row, int col, const wxString& value ){
	// long long_val = -1L;
	// int int_val = -1;
	// if(value.ToLong(&long_val)){
	// 	int_val = (int) long_val;
	// 	if(row >= n_rows || col >= n_cols){
	// 		int max_rows = (row + 1)>n_rows?(row+1):n_rows;
	// 		int max_cols = (col + 1)>n_cols?(col+1):n_cols;
	// 		if(values == NULL){
	// 			values = (int*)calloc(max_rows * max_cols, sizeof(int));
	// 		} else {
	// 			values = (int*)realloc(values, (max_rows * max_cols)*sizeof(int));
	// 		}
	// 	}
	// 	if(int_val >= 0){
	// 		printf("Setting attr\n");
	// 		values[col * n_rows + row] = int_val;
	// 		wxGridCellAttr * attr = GetAttr(row,col, wxGridCellAttr::Cell);
	// 		attr->SetBackgroundColour( wxColor(244,244,0) );
	// 		SetAttr(attr, row, col);
	// 	}
	// }
}

void ChoiceGridTable::AddState(wxString name, wxColor color){
	value_labels.push_back(name);
	value_colors.push_back(color);
}

void ChoiceGridTable::SetNextState(int i_row, int i_col){
	int i = i_col * n_rows + i_row;
	values[i] = (values[i] + 1) % value_labels.size();

	// wxGridCellAttr * attr = GetAttr(i_row,i_col, wxGridCellAttr::Cell);
	// attr->SetBackgroundColour( wxColor(244,244,0) );
	// SetAttr(attr, i_row, i_col);
}

wxGridCellAttr * ChoiceGridTable::GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind){
	int value = values[col * n_rows + row];
	wxColor text_color  = wxColor(0,0,0);
	wxColor back_color;
	if(value >= 0 && value_colors.size() > value){
		back_color  = value_colors[value];
	} else if(value == -1){
		back_color = wxColor(240,240,240);
	} else {
		back_color = wxColor(255,0,0);
	}
	wxFont font = GetView()->GetFont();
	return new wxGridCellAttr(text_color, back_color, font, wxAlignment::wxALIGN_CENTRE, wxAlignment::wxALIGN_CENTRE );
}

void ChoiceGridTable::SetState(int i_row, int i_col, int state){
	values[i_col * n_rows + i_row] = state;
}

void ChoiceGridTable::SetDefaultRowLabel(wxString lbl){
	default_row_label = lbl;
}

void ChoiceGridTable::SetDefaultColumnLabel(wxString lbl){
	default_col_label = lbl;
}

ChoiceGrid::ChoiceGrid(Application * owner, wxWindow * parent, wxWindowID id, wxPoint position, wxSize size) : wxGrid(parent,id,position,size), m_owner(owner){
	Bind(wxEVT_GRID_CELL_LEFT_CLICK, &ChoiceGrid::OnLeftClick, this);
	Bind(wxEVT_GRID_LABEL_LEFT_CLICK, &ChoiceGrid::OnHeaderLeftClick, this);
	SetDefaultRenderer(new NoSelectGridCellRenderer);
	SetLabelBackgroundColour( wxColor(255,255,255) );
	// SetSelectionMode(0);
	SetTable(new ChoiceGridTable(1,1));
	EnableEditing(false);
}
ChoiceGrid::~ChoiceGrid(){

}

void ChoiceGrid::GridRemake(int n_cols, int n_rows){
	unsigned int i = 0,j = 0;
	int old_n_rows = GetNumberRows();
	int old_n_cols = GetNumberCols();

	// printf("Setting row label 0 as %s\n", copy_wx_string(GetTable()->GetRowLabelValue(0)));
	if(n_cols > 0 && n_rows > 0){
		// SetRowLabel(0, GetTable()->GetRowLabelValue(0));
		// SetColLabel(0, GetTable()->GetColLabelValue(0));
	}


	if(n_rows > old_n_rows){
		AppendRows(n_rows - old_n_rows);
	} else if(n_rows < old_n_rows) {
		DeleteRows(n_rows, old_n_rows - n_rows);
	}

	if(n_cols > old_n_cols){
		AppendCols(n_cols - old_n_cols);
	} else if(n_cols < old_n_cols){
		DeleteCols(n_cols, old_n_cols - n_cols);
	}
	// m_immutable_cell_text = wxT("");
	// m_immutable_cell_color = wxColor(200, 200, 200);

	/* the FORs below don't catch the (0,0) because we need to generate the table with 1,1 before */
	// if(m_row_names.size() > 0){
	// 	SetRowLabelValue(i, m_row_names[i]);
	// } else {
	// 	SetRowLabelValue(0, m_basic_row_name + wxString::Format(" %d",i + 1));
	// }
	// if(m_col_names.size() > 0){
	// 	SetColLabelValue(j, m_col_names[j]);
	// } else {
	// 	SetColLabelValue(j, m_basic_col_name + wxString::Format(" %d",j + 1));
	// }
	if(GetCellState(0,0) == -2){
		SetCellState(0,0,0);
	}

	// if(n_rows > old_n_rows){
	// 	for(i = old_n_rows; i < n_rows; ++i){
	// 		// if(m_row_names.size() <= i){
	// 		// 	SetRowLabelValue(i, m_basic_row_name + wxString::Format(" %d",i + 1));
	// 		// } else {
	// 		// 	SetRowLabelValue(i, m_row_names[i]);
	// 		// }
	// 		for(j = 0; j < old_n_cols; ++j){
	// 			SetCellState(i,j,0);
	// 			SetReadOnly(i,j,true);
	// 		}
	// 	}
	// }
	// if(n_cols > old_n_cols){
	// 	for(j = old_n_cols; j < n_cols; ++j){
	// 		// if(m_col_names.size() <= j){
	// 		// 	SetColLabelValue(j, m_basic_col_name + wxString::Format(" %d",j + 1));
	// 		// } else {
	// 		// 	SetColLabelValue(j, m_col_names[j]);
	// 		// }
	// 		for(i = 0; i < n_rows; ++i){
	// 			SetCellState(i,j,0);
	// 			SetReadOnly(i,j,true);
	// 		}
	// 	}
	// }
	Refresh();
	AutoSize();
}


void ChoiceGrid::SetColActiveCellsState(int i_col, int state) {
	int i_row = 0;
	for(i_row = 0; i_row < GetNumberRows(); ++i_row){
		if(GetCellState(i_row, i_col) != -1){
			SetCellState(i_row, i_col, state);
		}
	}
}

void ChoiceGrid::SetRowActiveCellsState(int i_row, int state) {
	int i_col = 0;
	for(i_col = 0; i_col < GetNumberCols(); ++i_col){
		if(GetCellState(i_row, i_col) != -1){
			SetCellState(i_row, i_col, state);
		}
	}
}

void ChoiceGrid::SetCanUserClick(bool can){
	m_can_user_click = can;
}


void ChoiceGrid::OnHeaderLeftClick(wxGridEvent & evt){
	int evt_col = evt.GetCol(), evt_row = evt.GetRow();

	// if(GetNumberRows() > 0 && GetNumberCols() > 0 &&  m_can_user_click ){ // && m_value_names.size() > 0){
	// 	if(evt_col == -1 && evt_row == -1){
	// 		/* Loop until it finds one active cell with a state */
	// 		int last_state = -1;
	// 		for(int i_row = 0; last_state == -1 && i_row < GetNumberRows(); ++i_row){
	// 			for(int i_col = 0; last_state == -1 && i_col < GetNumberCols(); ++i_col){
	// 				last_state = GetCellState(i_row, i_col);
	// 			}
	// 		}
	// 		if(last_state >= 0){
	// 			SetAllActiveCellsState((last_state + 1) % m_value_names.size());
	// 		} /* Otherwise all cells are disabled */
	// 	} else if(evt_col == -1){
	// 		/* Loop until it finds one active cell with a state */
	// 		int last_state = -1;
	// 		for(int i_col = 0; last_state == -1 && i_col < GetNumberCols(); ++i_col){
	// 			last_state = GetCellState(evt_row,i_col);
	// 		}
	// 		if(last_state != -1){
	// 			SetRowActiveCellsState(evt_row, (last_state + 1) % m_value_names.size());
	// 		}
	// 	} else if(evt_row == -1){
	// 		/* Loop until it finds one active cell with a state */
	// 		int last_state = -1;
	// 		for(int i_row = 0; last_state == -1 && i_row < GetNumberRows(); ++i_row){
	// 			last_state = GetCellState(i_row,evt_col);
	// 		}
	// 		if(last_state != -1){
	// 			SetColActiveCellsState(evt_col,(last_state + 1) % m_value_names.size());
	// 		}
	// 	}
	// }
}

void ChoiceGrid::OnLeftClick(wxGridEvent & evt){
	int i = 0;
	int evt_col = evt.GetCol(), evt_row = evt.GetRow();

	if(GetNumberRows() > 0 && GetNumberCols() > 0 &&  m_can_user_click ){
		SetCellNextState(evt_row, evt_col);
		evt.Skip();
		Refresh();
	}
}

void ChoiceGrid::SetCellImmutable(int i, int j){
	SetCellValue(i,j, m_immutable_cell_text);
	SetCellBackgroundColour(i,j,m_immutable_cell_color);
}

void ChoiceGrid::SetCellState(int i_row, int i_col, int state){
	if(i_col < GetNumberCols() && i_row < GetNumberRows()){
		((ChoiceGridTable*)GetTable())->SetState(i_row, i_col, state);
	}
	Refresh();
}

void ChoiceGrid::SetCellNextState(int i_row, int i_col){
	if(i_col < GetNumberCols() && i_row < GetNumberRows()){
		((ChoiceGridTable*)GetTable())->SetNextState(i_row, i_col);
	}
	Refresh();
}


void ChoiceGrid::SetAllCellsState(int state){
	/* Inneficiently elegant. Refactor before production */
	for(int i = 0; i < GetNumberRows(); ++i){
		for(int j = 0; j < GetNumberCols(); ++j){
			SetCellState(i,j,state);
		}
	}
}

void ChoiceGrid::SetAllActiveCellsState(int state){
	/* Inneficiently elegant. Refactor before production */
	for(int i = 0; i < GetNumberRows(); ++i){
		for(int j = 0; j < GetNumberCols(); ++j){
			if(GetCellState(i,j) != -1){
				SetCellState(i,j,state);
			}
		}
	}
}

int ChoiceGrid::GetCellState(int i_row, int i_col){
	return ((ChoiceGridTable*)GetTable())->GetState(i_row,i_col);
}

int  ChoiceGrid::AddState(wxString name, wxColor color){
	((ChoiceGridTable*)GetTable())->AddState(name,color);
}

void ChoiceGrid::SetColLabel(int i_col, wxString name){
	ChoiceGridTable * table = (ChoiceGridTable *) GetTable();
	table->SetColLabelValue(i_col, name);
}
void ChoiceGrid::SetRowLabel(int i_row, wxString name){
	ChoiceGridTable * table = (ChoiceGridTable *) GetTable();
	table->SetRowLabelValue(i_row, name);
}

void ChoiceGrid::SetDefaultColumnLabel(wxString lbl){
	ChoiceGridTable * table = (ChoiceGridTable *) GetTable();
	table->SetDefaultColumnLabel(lbl);
}

void ChoiceGrid::SetDefaultRowLabel(wxString lbl){
	ChoiceGridTable * table = (ChoiceGridTable *) GetTable();
	table->SetDefaultRowLabel(lbl);
}
