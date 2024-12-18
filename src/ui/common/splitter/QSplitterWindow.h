#pragma once
#include <wx/splitter.h>
class QSplitterWindow : public wxSplitterWindow
{
	DECLARE_EVENT_TABLE();
public:
	void OnPaint(wxPaintEvent& event);
};

