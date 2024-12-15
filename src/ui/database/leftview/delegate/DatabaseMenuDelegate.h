
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
 * @file   DatabaseMenuDelegate.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-28
 *********************************************************************/
#include <wx/bmpcbox.h>
#include <wx/treectrl.h>
#include "ui/common/delegate/QDelegate.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/common/menu/QMenu.h"
#include "core/service/db/DatabaseService.h"
class DatabaseMenuDelegate : public QDelegate<DatabaseMenuDelegate, DatabaseSupplier>
{
public:
	~DatabaseMenuDelegate();

	void setTreeView(wxTreeCtrl * treeView);
	void createMenus();
	
	void popDatabaseMenu(int x, int y);
	void popTablesFolderMenu(int x, int y);
	void popViewsFolderMenu(int x, int y);
	void popProceduresFolderMenu(int x, int y);
	void popFunctionsFolderMenu(int x, int y);
	void popTriggersFolderMenu(int x, int y);
	void popEventsFolderMenu(int x, int y);

	void popViewMenu(int x, int y);
	void popProcedureMenu(int x, int y);
	void popFunctionMenu(int x, int y);
	void popTriggerMenu(int x, int y);
	void popEventMenu(int x, int y);
private:
	wxTreeCtrl * treeView;
	wxString imgdir;
	QMenu* menu = nullptr;
	QMenu* newSubMenu = nullptr;

	QMenu* tablesFolderMenu = nullptr;
	QMenu* viewsFolderMenu = nullptr;
	QMenu* proceduresFolderMenu = nullptr;
	QMenu* functionsFolderMenu = nullptr;
	QMenu* triggersFolderMenu = nullptr;
	QMenu* eventsFolderMenu = nullptr;

	QMenu* tableMenu = nullptr;
	QMenu* viewMenu = nullptr;
	QMenu* procedureMenu = nullptr;
	QMenu* functionMenu = nullptr;
	QMenu* triggerMenu = nullptr;
	QMenu* eventMenu = nullptr;

	wxColour bkgColor{ 43, 45, 48, 43 };
	wxColour textColor{223, 225, 229, 213};

	DatabaseService * databaseService = DatabaseService::getInstance();
	//Database menu
	void createDatabaseMenu();
	// Folder menus
	void createTablesFolderMenu();
	void createViewsFolderMenu();
	void createProceduresFolderMenu();
	void createFunctionsFolderMenu();
	void createTriggersFolderMenu();
	void createEventsFolderMenu();

	// Object menus
	void createViewMenu();
	void createProcedureMenu();
	void createFunctionMenu();
	void createTriggerMenu();
	void createEventMenu();
};

