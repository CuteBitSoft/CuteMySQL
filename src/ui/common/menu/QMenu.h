#pragma once
#include <wx/menu.h>
#include <wx/ownerdrw.h>
class QMenu : public wxMenu , public  wxOwnerDrawn
{
public:
	QMenu();
	~QMenu();
	bool ProcessEvent(wxEvent & event) wxOVERRIDE;
	void OnPaint(wxMenuEvent& event);

	virtual wxString GetName() const;
	virtual bool OnMeasureItem(size_t *width, size_t *height);
	virtual bool OnDrawItem(wxDC& dc, const wxRect& rc,
                            wxODAction act, wxODStatus stat) wxOVERRIDE;

	void* menuBrush;
};

