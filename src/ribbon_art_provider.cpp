#include "gui.hpp"

RibbonArtProvider::RibbonArtProvider(Application * owner, bool set_colour_scheme, const wxFont * font)
			: wxRibbonMetroArtProvider(set_colour_scheme, font), m_owner(owner){

}

RibbonArtProvider::~RibbonArtProvider(){

}

void RibbonArtProvider::DrawTabCtrlBackground(wxDC& dc, wxWindow* wnd, const wxRect& rect){
	wxRibbonMetroArtProvider::DrawTabCtrlBackground(dc, wnd, rect);
	dc.SetFont(m_panel_label_font);

	wxString max_text = "100%";

	wxSize text_sz = dc.GetTextExtent(max_text);
	wxRect icons_rect(rect.x + rect.width - rect.height - text_sz.GetWidth() - 10, rect.y, rect.height, rect.height-1);
	wxRect text_rect(rect.x + rect.width - text_sz.GetWidth() - 5, rect.y + (rect.height - text_sz.GetHeight())/2 + 3, 0, 0);

	// dc.SetFont();
	wxBitmap bmp1 = wxArtProvider::GetBitmap(wxART_INFORMATION ,wxART_OTHER,  wxSize(rect.height-4, rect.height-4));
	wxBitmap bmp2 = wxArtProvider::GetBitmap(wxART_INFORMATION ,wxART_OTHER,  wxSize(rect	.height-4, rect.height-4));

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(wxColor(255,255,0));
	dc.DrawRectangle(icons_rect);
	dc.DrawText(max_text, text_rect.x, text_rect.y);
	// dc.DrawBitmap(bmp2, icons_rect.x+2, icons_rect.y+2);

	icons_rect.x -= text_sz.GetWidth() + 15 + rect.height;
	text_rect.x  -= text_sz.GetWidth() + 15 + rect.height;

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(wxColor(255,0,0));
	dc.DrawRectangle(icons_rect);
	// dc.DrawBitmap(bmp1, icons_rect.x+1, icons_rect.y+1);
	// dc.DrawRectangle(rect.x + rect.width - rect.height - rect.height - 5, rect.height, rect.y, rect.height -1);
	dc.DrawText(max_text, text_rect.x, text_rect.y);

	// dc.DrawText(max_text, rect.x + rect.width - text_sz.GetWidth() - 5, rect.y + (rect.height - text_sz.GetHeight())/2);

}
