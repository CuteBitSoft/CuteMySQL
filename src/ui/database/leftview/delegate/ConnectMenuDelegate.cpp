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
 * @file   ConnectMenuDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-15
 *********************************************************************/
#include "ConnectMenuDelegate.h"
#include <cassert>
#include <wx/msgdlg.h>
#include "ui/common/data/QClientData.h"
#include "ui/common/msgbox/QAnimateBox.h"
#include "core/common/Lang.h"

ConnectMenuDelegate::~ConnectMenuDelegate()
{
	DatabaseService::destroyInstance();
	databaseService = nullptr;

	delete menu;
	menu = nullptr;
}

void ConnectMenuDelegate::setTreeView(wxTreeCtrl* treeView)
{
	this->treeView = treeView;
}

void ConnectMenuDelegate::createMenu()
{
	if (menu) {
		return;
	}
	// main menu
	menu = new QMenu();
	
	auto imgdir = ResourceUtil::getProductImagesDir();

	wxMenuItem * menuItem = new wxMenuItem(0, Config::CONNECTION_REFRESH_MENU_ID, S("connection-refresh"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap1(imgdir + "/database/menu/refresh.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap1));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::CONNECTION_MANAGE_MENU_ID, S("connection-manage"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap2(imgdir + "/database/menu/manage-connection.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap2));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::CONNECTION_CREATE_MENU_ID, S("connection-create"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap3(imgdir + "/database/menu/create-connection.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap3));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::CONNECTION_DELETE_MENU_ID, S("connection-delete"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap4(imgdir + "/database/menu/delete.ico", wxBITMAP_TYPE_ICO); 
	menuItem->SetBitmap(wxBitmapBundle(bitmap4));
	menu->Append(menuItem);

	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menuItem = new wxMenuItem(0, Config::DATABASE_CREATE_MENU_ID, S("database-create"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap5(imgdir + "/database/menu/create-database.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap5));
	menu->Append(menuItem);

	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menuItem = new wxMenuItem(0, Config::CONNECTION_IMPORT_FROM_SQL_MENU_ID, S("connection-import-from-sql"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap6(imgdir + "/database/menu/import-from-sql.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap6));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::CONNECTION_EXPORT_AS_SQL_MENU_ID, S("connection-export-as-sql"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap7(imgdir + "/database/menu/export-as-sql.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap7));
	menu->Append(menuItem);
}

void ConnectMenuDelegate::popMenu(int x, int y)
{
	treeView->PopupMenu(menu, x, y);
}
