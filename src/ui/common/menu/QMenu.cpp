#include "QMenu.h"
#ifdef __WXMSW__
#include <windows.h>
#endif
QMenu::QMenu() : wxMenu()
{
#ifdef __WXMSW__
	menuBrush = ::CreateSolidBrush(RGB(43, 45,48));
	MENUINFO mi;
	mi.cbSize = sizeof(MENUINFO);
	mi.fMask = MIM_BACKGROUND | MIM_STYLE; 
	mi.hbrBack = (HBRUSH)menuBrush;
	mi.dwStyle = MNS_CHECKORBMP;
	HMENU hMenu = GetHMenu();
	::SetMenuInfo(hMenu, &mi);
#endif
}

QMenu::~QMenu()
{
#ifdef __WXMSW__
	::DeleteObject(menuBrush);
#endif
	
}

wxString QMenu::GetName() const
{
	return wxEmptyString;
}

bool QMenu::OnMeasureItem(size_t* width, size_t* height)
{
	return false;
}

bool QMenu::OnDrawItem(wxDC& dc, const wxRect& rc,
	wxODAction act, wxODStatus stat)
{
	return false;
}
bool QMenu::ProcessEvent(wxEvent& event)
{
	int i = 0;
	
	if (event.GetEventType() == wxEVT_MENU_HIGHLIGHT) {
		// todo...
	}

	if (event.GetEventType() == wxEVT_MENU_CLOSE) {
		// todo...
	}

	if (event.GetEventType() == wxEVT_MENU_OPEN) {
		// todo...
	}
	
	return wxMenu::ProcessEvent(event);
}
