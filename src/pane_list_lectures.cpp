#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

ListLecturesPane::ListLecturesPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i;
	this->m_owner = owner;
	School * school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxStaticText * title = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_list_lectures_by_class);
	wxStaticText * class_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_class);
	wxStaticText * subject_label = new wxStaticText(this, wxID_ANY, m_owner->m_lang->str_subject);
	wxStaticText * occurence_label = new wxStaticText(this, wxID_ANY, wxT("Ocasião"));

	class_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	subject_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));
	occurence_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(200,30));

	title->SetFont(*m_owner->m_bold_text_font);
	class_label->SetFont(*m_owner->m_small_font);
	subject_label->SetFont(*m_owner->m_small_font);
	occurence_label->SetFont(*m_owner->m_small_font);

	wxSizer * wrapper = new wxBoxSizer(wxVERTICAL);
	wxSizer * sizer   = new wxBoxSizer(wxVERTICAL);
	wxSizer * content_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * left_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * right_sz = new wxBoxSizer(wxVERTICAL);

	left_sz->Add(class_label, 0, wxBOTTOM, 5);
	left_sz->Add(class_choice, 0, wxBOTTOM, 10);
	left_sz->Add(subject_label, 0, wxBOTTOM, 5);
	left_sz->Add(subject_choice, 0, wxBOTTOM, 10);
	left_sz->Add(occurence_label, 0, wxBOTTOM, 5);
	left_sz->Add(occurence_choice, 0, wxBOTTOM, 10);

	content_sz->Add(left_sz, 0, wxRIGHT, 10);
	content_sz->Add(right_sz);

	sizer->Add(title, 0, wxBOTTOM, 10);
	sizer->Add(content_sz);

	wrapper->Add(sizer, 0, wxALL, 15);

	this->SetSizer(wrapper);
	Layout();

	for(i = 0; i < school->n_classes; ++i){
		class_choice->Insert(wxString::FromUTF8(school->classes[i].name), i, new IntClientData(i));
	}

	class_choice->Bind(wxEVT_CHOICE, &ListLecturesPane::OnClassSelectionChanged, this);

}

void ListLecturesPane::OnClassSelectionChanged(wxCommandEvent & ev){
	int n_inserted = 0, i = 0, i_class;
	School * school = m_owner->m_school;
	printf("event\n");
	subject_choice->Clear();
	if(class_choice->GetSelection() != wxNOT_FOUND){
		i_class = ((IntClientData*)class_choice->GetClientObject(class_choice->GetSelection()))->m_value;
		for(i = 0; i < school->n_assignments; ++i){
			if(school->assignments[i].m_class->id == school->classes[i_class].id){
				subject_choice->Insert(wxString::FromUTF8(school->assignments[i].subject->name), n_inserted);
				++n_inserted;
			}
			class_choice->Insert(wxString::FromUTF8(school->classes[i].name), i, new IntClientData(i));
		}
	}
}

ListLecturesPane::~ListLecturesPane(){

}
