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
 * @file   TableMenuDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-28
 *********************************************************************/
#include "TableMenuDelegate.h"
#include <cassert>
#include <wx/msgdlg.h>
#include "ui/common/data/QClientData.h"
#include "ui/common/msgbox/QAnimateBox.h"
#include "core/common/Lang.h"

TableMenuDelegate::~TableMenuDelegate()
{
	DatabaseService::destroyInstance();
	databaseService = nullptr;

	delete menu;
	delete columnsMenu;
	delete indexesMenu;

	menu = nullptr;
	indexesMenu = nullptr;
}

void TableMenuDelegate::setTreeView(wxTreeCtrl* treeView)
{

	this->treeView = treeView;
}

void TableMenuDelegate::createMenus()
{
	imgdir = ResourceUtil::getProductImagesDir() + "/database/menu";
	
	createTableMenu();
	createColumnsMenu();
	createIndexesMenu();
}
void TableMenuDelegate::createTableMenu()
{
	if (menu) {
		return;
	}
	// main menu
	menu = new QMenu();

	menu->appendItem(Config::TABLE_OPEN_MENU_ID, S("table-open"), imgdir + "/open-table.ico");
	menu->appendItem(Config::TABLE_CREATE_MENU_ID, S("table-create"), imgdir + "/create-table.ico");
	menu->appendItem(Config::TABLE_ALTER_MENU_ID, S("table-alter"), imgdir + "/alter-table.ico");
	menu->appendItem(Config::TABLE_RENAME_MENU_ID, S("table-rename"), imgdir + "/rename-table.ico");
	menu->appendItem(Config::TABLE_TRUNCATE_MENU_ID, S("table-truncate"), imgdir + "/truncate-table.ico");
	menu->appendItem(Config::TABLE_DROP_MENU_ID, S("table-drop"), imgdir + "/drop-table.ico");
	menu->appendItem(Config::TABLE_COPY_MENU_ID, S("table-copy-as"), imgdir + "/copy.ico");
	menu->appendItem(Config::TABLE_SHARDING_MENU_ID, S("table-sharding-as"), imgdir + "/sharding.ico");

	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menu->appendItem(Config::TABLE_EXPORT_MENU_ID, S("table-export"), imgdir + "/export-as-sql.ico");
	menu->appendItem(Config::TABLE_IMPORT_SQL_MENU_ID, S("table-import-sql"), imgdir + "/import-from-sql.ico");
	menu->appendItem(Config::TABLE_IMPORT_CSV_MENU_ID, S("table-import-csv"), imgdir + "/import-from-csv.ico");
	
	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menu->appendItem(Config::TABLE_MANAGE_INDEX_MENU_ID, S("table-manage-index"), imgdir + "/manage-index.ico");
	menu->appendItem(Config::TABLE_MANAGE_FOREIGNKEY_MENU_ID, S("table-manage-foreign-key"), imgdir + "/manage-foreign-key.ico");
	
	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menu->appendItem(Config::TABLE_PROPERTIES_MENU_ID, S("properties"), imgdir + "/properties.ico");
}

void TableMenuDelegate::createColumnsMenu()
{
	if (columnsMenu) {
		return;
	}
	// main menu
	columnsMenu = new QMenu();

	columnsMenu->appendItem(Config::TABLE_MANAGE_COLUMNS_MENU_ID, S("table-manage-columns"), imgdir + "/manager-columns.ico");
	columnsMenu->appendItem(Config::TABLE_DROP_COLUMN_MENU_ID, S("table-drop-column"), imgdir + "/drop-column.ico");
}

void TableMenuDelegate::createIndexesMenu()
{
	if (indexesMenu) {
		return;
	}
	// main menu
	indexesMenu = new QMenu();

	indexesMenu->appendItem(Config::TABLE_MANAGE_INDEX_MENU_ID, S("table-manage-indexes"), imgdir + "/manage-index.ico");
	indexesMenu->appendItem(Config::TABLE_DROP_INDEX_MENU_ID, S("table-drop-index"), imgdir + "/drop-index.ico");
}	

void TableMenuDelegate::popMenu(int x, int y)
{
	treeView->PopupMenu(menu, x, y);
}

void TableMenuDelegate::popColumnsMenu(int x, int y, bool isColumnItem)
{
	columnsMenu->Enable(Config::TABLE_DROP_COLUMN_MENU_ID, isColumnItem);
	treeView->PopupMenu(columnsMenu, x, y);
}

void TableMenuDelegate::popIndexesMenu(int x, int y, bool isIndexItem)
{
	indexesMenu->Enable(Config::TABLE_DROP_INDEX_MENU_ID, isIndexItem);
	treeView->PopupMenu(indexesMenu, x, y);
}
