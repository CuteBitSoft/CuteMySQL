#pragma once
#include <wx/menu.h>
#include <wx/ownerdrw.h>
class QMenu : public wxMenu 
	/*, public  wxOwnerDrawn*/
{
public:
	QMenu();
	~QMenu();
	void appendItem(int itemId, const wxString & itemLabel, const wxString & bitmapPath);
	bool ProcessEvent(wxEvent & event) wxOVERRIDE;
	/*
	virtual wxString GetName() const;
	virtual bool OnMeasureItem(size_t *width, size_t *height);
	virtual bool OnDrawItem(wxDC& dc, const wxRect& rc,
                            wxODAction act, wxODStatus stat) wxOVERRIDE;
	*/
private:
	wxColour bkgColor; // { 43, 45, 48, 43 };
	wxColour textColor; // { 223, 225, 229, 213 };

	void* menuBrush;
};

