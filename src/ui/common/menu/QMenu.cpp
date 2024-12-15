#include "QMenu.h"
#ifdef __WXMSW__
#include <windows.h>
#endif
QMenu::QMenu() : wxMenu(), bkgColor{ 43, 45, 48, 43 }, textColor{ 223, 225, 229, 213 }
{
#ifdef __WXMSW__
	menuBrush = ::CreateSolidBrush(RGB(bkgColor.GetRed(), bkgColor.GetGreen(), bkgColor.GetBlue()));
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

void QMenu::appendItem(int itemId, const wxString& itemLabel, const wxString& iconPath)
{
	wxMenuItem* menuItem = new wxMenuItem(0, itemId, itemLabel);
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap(iconPath, wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap));
	wxMenu::Append(menuItem);
}

/*
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
*/
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
