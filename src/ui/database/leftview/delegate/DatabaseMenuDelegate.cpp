/*****************************************************************//**
 * Copyright 2024 Xuehan Qin (qinxuehan2018@gmail.com) 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * @file   DatabaseMenuDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-28
 *********************************************************************/
#include "DatabaseMenuDelegate.h"
#include <cassert>
#include <wx/msgdlg.h>
#include "ui/common/data/QClientData.h"
#include "ui/common/msgbox/QAnimateBox.h"
#include "core/common/Lang.h"

DatabaseMenuDelegate::~DatabaseMenuDelegate()
{
	DatabaseService::destroyInstance();
	databaseService = nullptr;

	delete menu;
	menu = nullptr;
}

void DatabaseMenuDelegate::setTreeView(wxTreeCtrl* treeView)
{
	this->treeView = treeView;
}

void DatabaseMenuDelegate::createMenu()
{
	if (menu) {
		return;
	}
	// main menu
	menu = new QMenu();
	
	auto imgdir = ResourceUtil::getProductImagesDir();

	wxMenuItem * menuItem = new wxMenuItem(0, Config::DATABASE_OPEN_MENU_ID, S("database-open"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap2(imgdir + "/database/menu/open-database.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap2));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::DATABASE_DELETE_MENU_ID, S("database-delete"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap3(imgdir + "/database/menu/delete-database.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap3));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::DATABASE_DUPLICATE_MENU_ID, S("database-duplicate"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap4(imgdir + "/database/menu/duplicate.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap4));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::DATABASE_REFRESH_MENU_ID, S("database-refresh"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap5(imgdir + "/database/menu/refresh-database.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap5));
	menu->Append(menuItem);

	auto sepMenuItem2 = menu->AppendSeparator();
	sepMenuItem2->SetBackgroundColour(bkgColor);

	menuItem = new wxMenuItem(0, Config::DATABASE_EXPORT_AS_SQL_MENU_ID, S("database-export-as-sql"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap6(imgdir + "/database/menu/export-as-sql.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap6));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::DATABASE_IMPORT_FROM_SQL_MENU_ID, S("database-import-from-sql"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap7(imgdir + "/database/menu/import-from-sql.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap7));
	menu->Append(menuItem);

	auto sepMenuItem1 = menu->AppendSeparator();
	sepMenuItem1->SetBackgroundColour(bkgColor);

	// Sub Menu
	newSubMenu = new QMenu();
	menuItem = new wxMenuItem(0, Config::DATABASE_NEW_TABLE_MENU_ID, S("database-new-table"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap8(imgdir + "/database/menu/create-table.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap8));
	newSubMenu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::DATABASE_NEW_VIEW_MENU_ID, S("database-new-view"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap9(imgdir + "/database/menu/create-view.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap9));
	newSubMenu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::DATABASE_NEW_PROCEDURE_MENU_ID, S("database-new-procedure"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap10(imgdir + "/database/menu/create-procedure.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap10));
	newSubMenu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::DATABASE_NEW_FUNCTION_MENU_ID, S("database-new-function"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap11(imgdir + "/database/menu/create-function.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap11));
	newSubMenu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::DATABASE_NEW_TRIGGER_MENU_ID, S("database-new-trigger"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap12(imgdir + "/database/menu/create-trigger.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap12));
	newSubMenu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::DATABASE_NEW_EVENT_MENU_ID, S("database-new-event"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap13(imgdir + "/database/menu/create-event.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap12));
	newSubMenu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::DATABASE_CREATE_MENU_ID, S("database-create"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap1(imgdir + "/database/menu/create-database.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap1));
	menu->Append(menuItem);

	auto newMenuItem = menu->Append(Config::DATABASE_NEW_MENU_ID, S("database-new"), newSubMenu);
	newMenuItem->SetBackgroundColour(bkgColor);
	newMenuItem->SetTextColour(textColor);
}

void DatabaseMenuDelegate::popMenu(int x, int y)
{
	treeView->PopupMenu(menu, x, y);
}
