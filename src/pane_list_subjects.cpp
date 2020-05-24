#include "gui.hpp"

extern "C" {
	#include "loader.h"
};

ListSubjectsPane::ListSubjectsPane(Application * owner, wxWindow * parent, wxPoint pos) : wxScrolledWindow(parent, wxID_ANY, pos, wxSize(600,400)){
	int i = 0;
	School * school = NULL;
	this->m_owner = owner;
	school = m_owner->m_school;
	SetBackgroundColour(wxColour(240,240,240));

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	wxSizer * body_sz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer * desc_sz = new wxBoxSizer(wxVERTICAL);
	wxSizer * butn_sz = new wxBoxSizer(wxHORIZONTAL);


	wxStaticText * title = new wxStaticText(this, wxID_ANY, wxT("Lista de Disciplinas"), wxDefaultPosition, wxSize(400,25));
	title->SetFont(*m_owner->m_page_title_font);

	m_subjects_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));

	if(school->n_subjects > 0){
		wxArrayString list;
		for(i = 0; i < school->n_subjects; ++i){
			list.Add(wxString::FromUTF8(school->subjects[i].name));
		}
		m_subjects_list->InsertItems(list,0);
	}

	m_name_text = new wxStaticText(this, wxID_ANY, wxT("Nome:"), wxDefaultPosition, wxSize(300,30));

	wxButton * edit_btn = new wxButton(this, wxID_ANY, wxT("Editar"), wxDefaultPosition, wxSize(200,30));
	wxButton * delete_btn = new wxButton(this, wxID_ANY,wxT("Remover"), wxDefaultPosition, wxSize(200,30));

	edit_btn->Disable();

	butn_sz->Add(edit_btn, 1, wxEXPAND|wxALL, 5);
	butn_sz->Add(delete_btn, 1, wxEXPAND|wxALL,5);

	desc_sz->Add(m_name_text, 0, wxBOTTOM, 5);
	desc_sz->AddStretchSpacer();
	desc_sz->Add(butn_sz, 0, 0);

	body_sz->Add(m_subjects_list, 0, wxEXPAND|wxALL, 15);
	body_sz->Add(desc_sz, 1, wxEXPAND|wxALL, 15);

	sizer->Add(title, 0, wxALL, 15);
	sizer->Add(body_sz, 1, wxALL, 15);

	SetSizerAndFit(sizer);
	SetScrollRate(5,5);
	ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_ALWAYS);
	this->GetSizer()->SetSizeHints(this);
	Layout();

	edit_btn->Bind(wxEVT_BUTTON, &ListSubjectsPane::OnEditButtonClicked, this);
	delete_btn->Bind(wxEVT_BUTTON, &ListSubjectsPane::OnDeleteButtonClicked, this);
	m_subjects_list->Bind(wxEVT_LISTBOX, &ListSubjectsPane::OnSelectionChanged, this);
}

ListSubjectsPane::~ListSubjectsPane(){

}

void ListSubjectsPane::OnEditButtonClicked(wxCommandEvent & ev){
	printf("Not Implemented\n");
}

void ListSubjectsPane::OnDeleteButtonClicked(wxCommandEvent & ev){
	School * school = m_owner->m_school;
	int i,j, del_i;
	bool success;
	if(m_subjects_list->GetSelection() != wxNOT_FOUND){
		del_i = m_subjects_list->GetSelection();

		success = remove_subject(stdout, m_owner->m_database, school->subjects[del_i].id);
		if(success){
			for(i = 0; i < school->n_teaches; ++i){
				/* Push all behind to the front if this is about the deleted. */
				if(school->teaches[i].subject->id == school->subjects[del_i].id){
					for(j = i; j < school->n_teaches; ++j){
						school->teaches[j] = school->teaches[j+1];
					}
					--school->n_teaches;
				}
			}

			if(school->all_meetings != NULL){
				for(i = 0; school->all_meetings[i].m_class != NULL; ++i){
					Meeting * meet = & school->all_meetings[i];
					if(meet->subj->id == school->subjects[del_i].id){
						/* No need to set to null. Will be overwritten anyway. */
						if(meet->possible_periods != NULL){
							free(school->all_meetings[i].possible_periods);
						}
						if(meet->possible_rooms != NULL){
							free(school->all_meetings[i].possible_rooms);
						}
						if(meet->possible_teachers != NULL){
							free(school->all_meetings[i].possible_teachers);
						}

						for(j = i; school->all_meetings[j].m_class != NULL; ++j){
							school->all_meetings[j] = school->all_meetings[j+1];
						}
					}
				}
			}

			for(i = del_i; i < school->n_subjects; ++i){
				school->subjects[i] = school->subjects[i + 1];
			}
			--school->n_subjects;

			printf("Sucesso.\n");
			m_subjects_list->Delete(del_i);
		} else {
			printf("Não foi possível.\n");
		}

	}
	printf("Hello, \n");
}

void ListSubjectsPane::OnSelectionChanged(wxCommandEvent & ev){
	Subject * subject =  nullptr;
	if(m_subjects_list->GetSelection() != wxNOT_FOUND){
		subject = &(m_owner->m_school->subjects[m_subjects_list->GetSelection()]);
		m_name_text->SetLabel(wxString::Format(wxT("Nome: %s"), wxString::FromUTF8(subject->name)));
	} else {
		m_name_text->SetLabel(wxString::FromUTF8("Nome:"));
	}
}
