#include "gui.hpp"
#include <wx/wupdlock.h>
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
	wxGrid * view = GetView();
	view->BeginBatch();
	wxGridTableMessage pop(this, wxGRIDTABLE_NOTIFY_COLS_APPENDED, n_new_cols);
	view->ProcessTableMessage(pop);
	view->EndBatch();
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
	wxGrid * view = GetView();
	view->BeginBatch();
	wxGridTableMessage pop(this, wxGRIDTABLE_NOTIFY_ROWS_APPENDED, n_new_rows);
	view->ProcessTableMessage(pop);
	view->EndBatch();
	return true;
}


bool PosIntGridTable::DeleteCols(size_t pos, size_t n_del_cols) {
	n_cols -= n_del_cols;

	wxGrid * view = GetView();
	view->BeginBatch();
	wxGridTableMessage pop(this, wxGRIDTABLE_NOTIFY_COLS_DELETED, n_cols, n_del_cols);
	view->ProcessTableMessage(pop);
	view->EndBatch();
}
bool PosIntGridTable::DeleteRows(size_t pos, size_t n_del_rows) {
	n_rows -= n_del_rows;
	for(int i_col = 0; i_col < n_cols; ++i_col){
		for(int i_row = 0; i_row < n_rows; ++i_row){
			values[i_col * n_rows + i_row] = values[i_col * n_rows + (i_row + n_del_rows * i_col)];
		}
	}
	wxGrid * view = GetView();
	view->BeginBatch();
	wxGridTableMessage pop(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED, n_rows, n_del_rows);
	view->ProcessTableMessage(pop);
	view->EndBatch();
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
		SetCellValue(row, col, int_val);
	}
}
void PosIntGridTable::SetCellValue(int row, int col, int value){
	if(row >= n_rows || col >= n_cols){
		int max_rows = (row + 1)>n_rows?(row+1):n_rows;
		int max_cols = (col + 1)>n_cols?(col+1):n_cols;
		if(values == NULL){
			values = (int*)calloc(max_rows * max_cols, sizeof(int));
		} else {
			values = (int*)realloc(values, (max_rows * max_cols)*sizeof(int));
		}
	}
	if(value >= 0){
		values[col * n_rows + row] = value;
	}
}

ChoiceGridTable::ChoiceGridTable(int n_rows, int n_cols) : wxGridTableBase() {
	this->n_cols = n_cols;
	this->n_rows = n_rows;
	if(n_rows > 0 && n_cols > 0){
		values = (int*)calloc(n_rows * n_cols, sizeof(int));
	} else {
		values = NULL;
	}
	row_labels = wxVector<wxString>();
	col_labels = wxVector<wxString>();
	value_labels = wxVector<wxString>();
	value_colors = wxVector<wxColor>();
}

wxString ChoiceGridTable::GetRowLabelValue(int row){
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
			for(int i = old_n_cols * n_rows; i < n_cols * n_rows; ++i){
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
			// Transpose values from the old to the new array of values.
			// No need to do these for's when we are callocing -- there would be no values anyway
			for(int i_col = n_cols -1; i_col >= 0; --i_col){
				for(int i_row = old_n_rows -1; i_row >= 0; --i_row){
					values[(i_col * old_n_rows) + (n_new_rows * i_col) + i_row] = values[i_col * old_n_rows + i_row];
				}
			}
			for(int i_col = 0; i_col < n_cols; ++i_col){
				for(int i_row = old_n_rows; i_row < n_rows; ++i_row){
					values[i_col * n_rows + i_row] = 0;
				}
			}
		}
	}
	wxGrid * view = GetView();
	view->BeginBatch();
	wxGridTableMessage pop(this, wxGRIDTABLE_NOTIFY_ROWS_APPENDED, n_new_rows);
	view->ProcessTableMessage(pop);
	view->EndBatch();
	return true;
}

bool ChoiceGridTable::DeleteRows(size_t pos, size_t n_del_rows){
	// TODO: maybe use pos, but in the current use of this program,
	// 		 it is irrelevant and simply easier to use the last rows.
	/* No need to shrink/realloc */
	n_rows -= n_del_rows;
	for(int i_col = 0; i_col < n_cols; ++i_col){
		for(int i_row = 0; i_row < n_rows; ++i_row){
			values[i_col * n_rows + i_row] = values[i_col * n_rows + (i_row + n_del_rows * i_col)];
		}
	}
	wxGrid * view = GetView();
	view->BeginBatch();
	wxGridTableMessage pop(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED, n_rows, n_del_rows);
	view->ProcessTableMessage(pop);
	view->EndBatch();
	return true;
}
bool ChoiceGridTable::DeleteCols(size_t pos, size_t n_del_cols){
	// TODO: maybe use pos, but in the current use of this program,
	// 		 it is irrelevant and simply easier to use the last cols.
	/* No need to shrink/realloc */
	n_cols -= n_del_cols;

	wxGrid * view = GetView();
	view->BeginBatch();
	wxGridTableMessage pop(this, wxGRIDTABLE_NOTIFY_COLS_DELETED, n_cols, n_del_cols);
	view->ProcessTableMessage(pop);
	view->EndBatch();
	return true;
	return true;
}

void ChoiceGridTable::SetColNextState(int col){
	int state = -1;
	// Finds a state to 'calculate' the next.
	for(int i = 0; state == CELL_STATE_LOCKED && i < n_rows; ++i){
		state = values[col * n_rows + i];
	}
	// No need to set if there are no settable cells
	if(state == CELL_STATE_LOCKED) return;
	// Sets all to $state+1, except for those marked as -1.
	state = (state + 1) % value_labels.size();

	for(int i = 0; i < n_rows; ++i){
		values[col * n_rows + i] = values[col * n_rows + i] == CELL_STATE_LOCKED ? CELL_STATE_LOCKED : state;
	}
}
void ChoiceGridTable::SetRowNextState(int row){
	int state = CELL_STATE_LOCKED;
	// Finds a state to 'calculate' the next.
	for(int i = row; state == CELL_STATE_LOCKED && i < n_rows * n_cols; i += n_rows){
		state = values[i];
	}
	// i_col * n_rows + i_row
	// No need to set if there are no settable cells
	if(state == CELL_STATE_LOCKED) return;
	// Sets all to $state+1, except for those marked as -1.
	state = (state + 1) % value_labels.size();
	for(int i = row; i < n_rows * n_cols; i += n_rows){
		values[i] = values[i] == CELL_STATE_LOCKED ? CELL_STATE_LOCKED : state;
	}
}
void ChoiceGridTable::SetTableNextState(){
	int state = -1;
	// Finds a state to 'calculate' the next.
	for(int i = 0; state == -1 && i < n_rows; ++i){
		for(int j = 0; state == -1 && j < n_cols; ++j){
			state = values[i * n_cols + j];
		}
	}
	// No need to set if there are no settable cells
	if(state == -1) return;
	// Sets all to $state+1, except for those marked as -1.
	state = (state + 1) % value_labels.size();
	for(int i = 0; i < n_rows * n_cols; ++i){
		values[i] = (values[i] == CELL_STATE_LOCKED)? CELL_STATE_LOCKED : state;
	}
}
void ChoiceGridTable::SetTableState(int state){
	for(int i = 0; i < n_rows * n_cols; ++i){
		values[i] = state;
	}

}
void ChoiceGridTable::SetTableActiveState(int state){
	for(int i = 0; i < n_rows * n_cols; ++i){
		values[i] = (values[i] == CELL_STATE_LOCKED)? CELL_STATE_LOCKED : state;
	}

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
		} else if(val == CELL_STATE_LOCKED){
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
	return CELL_STATE_LOCKED;
}
void ChoiceGridTable::SetValue( int row, int col, const wxString& value ){
	printf("SetValue used in ChoiceGridTable...\n");
}

/* Returns the code of that created state. */
int ChoiceGridTable::AddState(wxString name, wxColor color){
	value_labels.push_back(name);
	value_colors.push_back(color);
	return value_labels.size() - 1;
}

void ChoiceGridTable::SetNextState(int i_row, int i_col){
	int i = i_col * n_rows + i_row;
	if(values[i] != CELL_STATE_LOCKED){
		values[i] = (values[i] + 1) % value_labels.size();
	}
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

int  ChoiceGridTable::GetNumberStates(){
	return value_labels.size();
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
	Bind(wxEVT_GRID_SELECT_CELL, &ChoiceGrid::BlankFunction, this);
	Bind(wxEVT_GRID_SELECT_CELL, &ChoiceGrid::BlankFunction, this);
	SetDefaultRenderer(new NoSelectGridCellRenderer);
	SetLabelBackgroundColour( wxColor(255,255,255) );
	// SetSelectionMode(0);
	SetTable(new ChoiceGridTable(1,1));
	EnableEditing(false);

	m_cell_width = 0;
}
ChoiceGrid::~ChoiceGrid(){

}

void ChoiceGrid::BlankFunction(wxGridEvent &){ }

void ChoiceGrid::GridRemake(int n_cols, int n_rows){
	int old_n_rows = GetNumberRows();
	int old_n_cols = GetNumberCols();

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
	Refresh();
	SetDefaultColSize(m_cell_width, true);
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
	if(m_can_user_click){
		int evt_col = evt.GetCol(), evt_row = evt.GetRow();
		ChoiceGridTable * table = (ChoiceGridTable *) GetTable();
		if(evt_col != -1){
			table->SetColNextState(evt_col);
		} else if(evt_row != -1){
			table->SetRowNextState(evt_row);
		} else {
			table->SetTableNextState();
		}
		ForceRefresh();
	}
}

void ChoiceGrid::OnLeftClick(wxGridEvent & evt){
	int evt_col = evt.GetCol(), evt_row = evt.GetRow();

	if(GetNumberRows() > 0 && GetNumberCols() > 0 &&  m_can_user_click ){
		SetCellNextState(evt_row, evt_col);
		evt.Skip();
		ForceRefresh();
	}
}

void ChoiceGrid::SetCellLocked(int i, int j){
	((ChoiceGridTable *)GetTable())->SetState(i, j, CELL_STATE_LOCKED);
	ForceRefresh();
}

void ChoiceGrid::SetCellState(int i_row, int i_col, int state){
	if(i_col < GetNumberCols() && i_row < GetNumberRows()){
		((ChoiceGridTable*)GetTable())->SetState(i_row, i_col, state);
	}
	ForceRefresh();
}

void ChoiceGrid::SetCellNextState(int i_row, int i_col){
	if(i_col < GetNumberCols() && i_row < GetNumberRows()){
		((ChoiceGridTable*)GetTable())->SetNextState(i_row, i_col);
	}
	ForceRefresh();
}


void ChoiceGrid::SetAllCellsState(int state){
	((ChoiceGridTable*)GetTable())->SetTableState(state);
	wxWindowUpdateLocker noUpdates(this);
	ForceRefresh();
}

void ChoiceGrid::SetAllActiveCellsState(int state){
	((ChoiceGridTable*)GetTable())->SetTableActiveState(state);
	ForceRefresh();
}

int ChoiceGrid::GetCellState(int i_row, int i_col){
	return ((ChoiceGridTable*)GetTable())->GetState(i_row,i_col);
}

int  ChoiceGrid::AddState(wxString name, wxColor color){
	int ret_val = ((ChoiceGridTable*)GetTable())->AddState(name,color);
	int n_states = ((ChoiceGridTable*)GetTable())->GetNumberStates();
	// cell 0,0 is used as ruler here
	int dummy_old_state = GetCellState(0,0);
	// Cosidering the added state size.
	SetCellState(0,0,n_states-1);
	AutoSizeColumn(0);
	int width = GetColSize(0);
	m_cell_width = (m_cell_width > width)? m_cell_width : width;
	if(m_cell_width > 0){
		SetDefaultColSize(m_cell_width, true);
	}
	SetCellState(0,0,dummy_old_state);
	return ret_val;
}

void ChoiceGrid::SetColLabel(int i_col, wxString name){
	ChoiceGridTable * table = (ChoiceGridTable *) GetTable();
	table->SetColLabelValue(i_col, name);
	AutoSizeColLabelSize(i_col);
	int width = GetColSize(i_col);
	m_cell_width = (m_cell_width > width)? m_cell_width : width;
	if(width < m_cell_width){
		SetColSize(i_col, m_cell_width);
	}
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
