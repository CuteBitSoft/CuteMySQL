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
	imgdir = ResourceUtil::getProductImagesDir() + "/database/menu";
	if (menu) {
		return;
	}
	// main menu
	menu = new QMenu();
	
	menu->appendItem(Config::CONNECTION_REFRESH_MENU_ID, S("connection-refresh"), imgdir + "/refresh.ico");
	menu->appendItem(Config::CONNECTION_MANAGE_MENU_ID, S("connection-manage"), imgdir + "/manage-connection.ico");
	menu->appendItem(Config::CONNECTION_CREATE_MENU_ID, S("connection-create"), imgdir + "/create-connection.ico");
	menu->appendItem(Config::CONNECTION_DELETE_MENU_ID, S("connection-delete"), imgdir + "/delete.ico");
	
	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menu->appendItem(Config::DATABASE_CREATE_MENU_ID, S("database-create"), imgdir + "/create-database.ico");

	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menu->appendItem(Config::CONNECTION_IMPORT_FROM_SQL_MENU_ID, S("connection-import-from-sql"), imgdir + "/import-from-sql.ico");
	menu->appendItem(Config::CONNECTION_EXPORT_AS_SQL_MENU_ID, S("connection-export-as-sql"), imgdir + "/export-as-sql.ico");
	
}

void ConnectMenuDelegate::popMenu(int x, int y)
{
	treeView->PopupMenu(menu, x, y);
}
