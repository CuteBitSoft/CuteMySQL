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
	delete tablesFolderMenu;
	delete viewsFolderMenu;
	delete proceduresFolderMenu;
	delete functionsFolderMenu;
	delete triggersFolderMenu;
	delete eventsFolderMenu;

	delete viewMenu;
	delete procedureMenu;
	delete functionMenu;
	delete triggerMenu;
	delete eventMenu;
	
	menu = nullptr;
	tablesFolderMenu = nullptr;
	viewsFolderMenu = nullptr;
	proceduresFolderMenu = nullptr;
	functionsFolderMenu = nullptr;
	triggersFolderMenu = nullptr;
	eventsFolderMenu = nullptr;

	viewMenu = nullptr;
	procedureMenu = nullptr;
	functionMenu = nullptr;
	triggerMenu = nullptr;
	eventMenu = nullptr;
}

void DatabaseMenuDelegate::setTreeView(wxTreeCtrl* treeView)
{
	this->treeView = treeView;
}

void DatabaseMenuDelegate::createMenus()
{
	imgdir = ResourceUtil::getProductImagesDir() + "/database/menu";
	createDatabaseMenu();

	createTablesFolderMenu();
	createViewsFolderMenu();
	createProceduresFolderMenu();
	createFunctionsFolderMenu();
	createTriggersFolderMenu();
	createEventsFolderMenu();

	createViewMenu();
	createProcedureMenu();
	createFunctionMenu();
	createTriggerMenu();
	createEventMenu();
}

void DatabaseMenuDelegate::popDatabaseMenu(int x, int y)
{
	treeView->PopupMenu(menu, x, y);
}

void DatabaseMenuDelegate::popTablesFolderMenu(int x, int y)
{
	treeView->PopupMenu(tablesFolderMenu, x, y);
}

void DatabaseMenuDelegate::popViewsFolderMenu(int x, int y)
{
	treeView->PopupMenu(viewsFolderMenu, x, y);
}

void DatabaseMenuDelegate::popProceduresFolderMenu(int x, int y)
{
	treeView->PopupMenu(proceduresFolderMenu, x, y);
}

void DatabaseMenuDelegate::popFunctionsFolderMenu(int x, int y)
{
	treeView->PopupMenu(functionsFolderMenu, x, y);
}

void DatabaseMenuDelegate::popTriggersFolderMenu(int x, int y)
{
	treeView->PopupMenu(triggersFolderMenu, x, y);
}

void DatabaseMenuDelegate::popEventsFolderMenu(int x, int y)
{
	treeView->PopupMenu(eventsFolderMenu, x, y);
}

void DatabaseMenuDelegate::popViewMenu(int x, int y)
{
	treeView->PopupMenu(viewMenu, x, y);
}

void DatabaseMenuDelegate::popProcedureMenu(int x, int y)
{
	treeView->PopupMenu(procedureMenu, x, y);
}

void DatabaseMenuDelegate::popFunctionMenu(int x, int y)
{
	treeView->PopupMenu(functionMenu, x, y);
}

void DatabaseMenuDelegate::popTriggerMenu(int x, int y)
{
	treeView->PopupMenu(triggerMenu, x, y);
}

void DatabaseMenuDelegate::popEventMenu(int x, int y)
{
	treeView->PopupMenu(eventMenu, x, y);
}

void DatabaseMenuDelegate::createDatabaseMenu()
{
	if (menu) {
		return;
	}
	// main menu
	menu = new QMenu();
	

	menu->appendItem(Config::DATABASE_OPEN_MENU_ID, S("database-open"), imgdir + "/open-database.ico");
	menu->appendItem(Config::DATABASE_DELETE_MENU_ID, S("database-delete"), imgdir + "/delete-database.ico");
	menu->appendItem(Config::DATABASE_DUPLICATE_MENU_ID, S("database-duplicate"), imgdir + "/duplicate.ico");
	menu->appendItem(Config::DATABASE_REFRESH_MENU_ID, S("database-refresh"), imgdir + "/refresh.ico");
	
	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menu->appendItem(Config::DATABASE_EXPORT_AS_SQL_MENU_ID, S("database-export-as-sql"), imgdir + "/export-as-sql.ico");
	menu->appendItem(Config::DATABASE_IMPORT_FROM_SQL_MENU_ID, S("database-import-from-sql"), imgdir + "/import-from-sql.ico");
	
	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	// Sub Menu
	newSubMenu = new QMenu();
	newSubMenu->appendItem(Config::DATABASE_NEW_TABLE_MENU_ID, S("database-new-table"), imgdir + "/create-table.ico");
	newSubMenu->appendItem(Config::DATABASE_NEW_VIEW_MENU_ID, S("database-new-view"), imgdir + "/create-view.ico");
	newSubMenu->appendItem(Config::DATABASE_NEW_PROCEDURE_MENU_ID, S("database-new-procedure"), imgdir + "/create-procedure.ico");
	newSubMenu->appendItem(Config::DATABASE_NEW_FUNCTION_MENU_ID, S("database-new-function"), imgdir + "/create-function.ico");
	newSubMenu->appendItem(Config::DATABASE_NEW_TRIGGER_MENU_ID, S("database-new-trigger"), imgdir + "/create-trigger.ico");
	newSubMenu->appendItem(Config::DATABASE_NEW_EVENT_MENU_ID, S("database-new-event"), imgdir + "/create-event.ico");


	auto newMenuItem = menu->Append(Config::DATABASE_NEW_MENU_ID, S("database-new"), newSubMenu);
	newMenuItem->SetBackgroundColour(bkgColor);
	newMenuItem->SetTextColour(textColor);

	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menu->appendItem(Config::DATABASE_PROPERTIES_MENU_ID, S("properties"), imgdir + "/properties.ico");
}

void DatabaseMenuDelegate::createTablesFolderMenu()
{
	if (tablesFolderMenu) {
		return;
	}
	// main menu
	tablesFolderMenu = new QMenu();

	tablesFolderMenu->appendItem(Config::SHOW_TABLES_MENU_ID, S("show-tables"), imgdir + "/show-objects.ico");
	tablesFolderMenu->appendItem(Config::DATABASE_NEW_TABLE_MENU_ID, S("table-create"), imgdir + "/create-table.ico");

	tablesFolderMenu->AppendSeparator()->SetBackgroundColour(bkgColor);

	tablesFolderMenu->appendItem(Config::DATABASE_EXPORT_AS_SQL_MENU_ID, S("database-export-as-sql"), imgdir + "/export-as-sql.ico");
	tablesFolderMenu->appendItem(Config::DATABASE_IMPORT_FROM_SQL_MENU_ID, S("database-import-from-sql"), imgdir + "/import-from-sql.ico");
}

void DatabaseMenuDelegate::createViewsFolderMenu()
{
	if (viewsFolderMenu) {
		return;
	}
	// main menu
	viewsFolderMenu = new QMenu();

	viewsFolderMenu->appendItem(Config::SHOW_VIEWS_MENU_ID, S("show-views"), imgdir + "/show-objects.ico");
	viewsFolderMenu->appendItem(Config::DATABASE_NEW_VIEW_MENU_ID, S("create-view"), imgdir + "/create-view.ico"); 

	viewsFolderMenu->AppendSeparator()->SetBackgroundColour(bkgColor);

	viewsFolderMenu->appendItem(Config::DATABASE_EXPORT_AS_SQL_MENU_ID, S("database-export-as-sql"), imgdir + "/export-as-sql.ico");
	viewsFolderMenu->appendItem(Config::DATABASE_IMPORT_FROM_SQL_MENU_ID, S("database-import-from-sql"), imgdir + "/import-from-sql.ico");
}

void DatabaseMenuDelegate::createProceduresFolderMenu()
{
	if (proceduresFolderMenu) {
		return;
	}
	// main menu
	proceduresFolderMenu = new QMenu();

	proceduresFolderMenu->appendItem(Config::SHOW_PROCEDURES_MENU_ID, S("show-procedures"), imgdir + "/show-objects.ico");
	proceduresFolderMenu->appendItem(Config::DATABASE_NEW_PROCEDURE_MENU_ID, S("create-procedure"), imgdir + "/create-procedure.ico");

	proceduresFolderMenu->AppendSeparator()->SetBackgroundColour(bkgColor);

	proceduresFolderMenu->appendItem(Config::DATABASE_EXPORT_AS_SQL_MENU_ID, S("database-export-as-sql"), imgdir + "/export-as-sql.ico");
	proceduresFolderMenu->appendItem(Config::DATABASE_IMPORT_FROM_SQL_MENU_ID, S("database-import-from-sql"), imgdir + "/import-from-sql.ico");
}

void DatabaseMenuDelegate::createFunctionsFolderMenu()
{
	if (functionsFolderMenu) {
		return;
	}
	// main menu
	functionsFolderMenu = new QMenu();

	functionsFolderMenu->appendItem(Config::SHOW_FUNCTIONS_MENU_ID, S("show-functions"), imgdir + "/show-objects.ico");
	functionsFolderMenu->appendItem(Config::DATABASE_NEW_FUNCTION_MENU_ID, S("create-function"), imgdir + "/create-procedure.ico");

	functionsFolderMenu->AppendSeparator()->SetBackgroundColour(bkgColor);

	functionsFolderMenu->appendItem(Config::DATABASE_EXPORT_AS_SQL_MENU_ID, S("database-export-as-sql"), imgdir + "/export-as-sql.ico");
	functionsFolderMenu->appendItem(Config::DATABASE_IMPORT_FROM_SQL_MENU_ID, S("database-import-from-sql"), imgdir + "/import-from-sql.ico");
}

void DatabaseMenuDelegate::createTriggersFolderMenu()
{
	if (triggersFolderMenu) {
		return;
	}
	// main menu
	triggersFolderMenu = new QMenu();

	triggersFolderMenu->appendItem(Config::SHOW_TRIGGERS_MENU_ID, S("show-triggers"), imgdir + "/show-objects.ico");
	triggersFolderMenu->appendItem(Config::DATABASE_NEW_TRIGGER_MENU_ID, S("create-trigger"), imgdir + "/create-trigger.ico");

	triggersFolderMenu->AppendSeparator()->SetBackgroundColour(bkgColor);

	triggersFolderMenu->appendItem(Config::DATABASE_EXPORT_AS_SQL_MENU_ID, S("database-export-as-sql"), imgdir + "/export-as-sql.ico");
	triggersFolderMenu->appendItem(Config::DATABASE_IMPORT_FROM_SQL_MENU_ID, S("database-import-from-sql"), imgdir + "/import-from-sql.ico");
}

void DatabaseMenuDelegate::createEventsFolderMenu()
{
	if (eventsFolderMenu) {
		return;
	}
	// main menu
	eventsFolderMenu = new QMenu();

	eventsFolderMenu->appendItem(Config::SHOW_EVENTS_MENU_ID, S("show-events"), imgdir + "/show-objects.ico");
	eventsFolderMenu->appendItem(Config::DATABASE_NEW_EVENT_MENU_ID, S("create-event"), imgdir + "/create-event.ico");

	eventsFolderMenu->AppendSeparator()->SetBackgroundColour(bkgColor);

	eventsFolderMenu->appendItem(Config::DATABASE_EXPORT_AS_SQL_MENU_ID, S("database-export-as-sql"), imgdir + "/export-as-sql.ico");
	eventsFolderMenu->appendItem(Config::DATABASE_IMPORT_FROM_SQL_MENU_ID, S("database-import-from-sql"), imgdir + "/import-from-sql.ico");
}

void DatabaseMenuDelegate::createViewMenu()
{
	if (viewMenu) {
		return;
	}
	// main menu
	viewMenu = new QMenu();
	viewMenu->appendItem(Config::DATABASE_NEW_VIEW_MENU_ID, S("create-view"), imgdir + "/create-view.ico");
	viewMenu->appendItem(Config::DATABASE_OPEN_VIEW_MENU_ID, S("open-view"), imgdir + "/open.ico");
	viewMenu->appendItem(Config::DATABASE_ALTER_VIEW_MENU_ID, S("alter-view"), imgdir + "/alter.ico");
	viewMenu->appendItem(Config::DATABASE_DROP_VIEW_MENU_ID, S("drop-view"), imgdir + "/drop.ico");

	viewMenu->AppendSeparator()->SetBackgroundColour(bkgColor);
	viewMenu->appendItem(Config::DATABASE_EXPORT_VIEW_DATA_MENU_ID, S("export-view-data"), imgdir + "/export-as-sql.ico");
	viewMenu->appendItem(Config::DATABASE_SHOW_VIEW_DATA_MENU_ID, S("show-view-data"), imgdir + "/show-data.ico");

}

void DatabaseMenuDelegate::createProcedureMenu()
{
	if (procedureMenu) {
		return;
	}
	// main menu
	procedureMenu = new QMenu();
	procedureMenu->appendItem(Config::DATABASE_NEW_PROCEDURE_MENU_ID, S("create-procedure"), imgdir + "/create-procedure.ico");
	procedureMenu->appendItem(Config::DATABASE_OPEN_PROCEDURE_MENU_ID, S("open-procedure"), imgdir + "/open.ico");
	procedureMenu->appendItem(Config::DATABASE_ALTER_PROCEDURE_MENU_ID, S("alter-procedure"), imgdir + "/alter.ico");
	procedureMenu->appendItem(Config::DATABASE_DROP_PROCEDURE_MENU_ID, S("drop-procedure"), imgdir + "/drop.ico");
}

void DatabaseMenuDelegate::createFunctionMenu()
{
	if (functionMenu) {
		return;
	}
	// main menu
	functionMenu = new QMenu();
	functionMenu->appendItem(Config::DATABASE_NEW_FUNCTION_MENU_ID, S("create-function"), imgdir + "/create-function.ico");
	functionMenu->appendItem(Config::DATABASE_OPEN_FUNCTION_MENU_ID, S("open-function"), imgdir + "/open.ico");
	functionMenu->appendItem(Config::DATABASE_ALTER_FUNCTION_MENU_ID, S("alter-function"), imgdir + "/alter.ico");
	functionMenu->appendItem(Config::DATABASE_DROP_FUNCTION_MENU_ID, S("drop-function"), imgdir + "/drop.ico");
}

void DatabaseMenuDelegate::createTriggerMenu()
{
	if (triggerMenu) {
		return;
	}
	// main menu
	triggerMenu = new QMenu();
	triggerMenu->appendItem(Config::DATABASE_NEW_TRIGGER_MENU_ID, S("create-trigger"), imgdir + "/create-trigger.ico");
	triggerMenu->appendItem(Config::DATABASE_OPEN_TRIGGER_MENU_ID, S("open-trigger"), imgdir + "/open.ico");
	triggerMenu->appendItem(Config::DATABASE_ALTER_TRIGGER_MENU_ID, S("alter-trigger"), imgdir + "/alter.ico");
	triggerMenu->appendItem(Config::DATABASE_DROP_TRIGGER_MENU_ID, S("drop-trigger"), imgdir + "/drop.ico");
}

void DatabaseMenuDelegate::createEventMenu()
{
	if (eventMenu) {
		return;
	}
	// main menu
	eventMenu = new QMenu();
	eventMenu->appendItem(Config::DATABASE_NEW_TRIGGER_MENU_ID, S("create-event"), imgdir + "/create-event.ico");
	eventMenu->appendItem(Config::DATABASE_OPEN_TRIGGER_MENU_ID, S("open-event"), imgdir + "/open.ico");
	eventMenu->appendItem(Config::DATABASE_ALTER_TRIGGER_MENU_ID, S("alter-event"), imgdir + "/alter.ico");
	eventMenu->appendItem(Config::DATABASE_DROP_TRIGGER_MENU_ID, S("drop-event"), imgdir + "/drop.ico");
}

