
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
 * @file   TableMenuDelegate.h
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
class TableMenuDelegate : public QDelegate<TableMenuDelegate, DatabaseSupplier>
{
public:
	~TableMenuDelegate();

	void setTreeView(wxTreeCtrl * treeView);
	void createMenus();
	void popMenu(int x, int y);
	void popColumnsMenu(int x, int y, bool isColumnItem);
	void popIndexesMenu(int x, int y, bool isIndexItem);
private:
	wxTreeCtrl * treeView;
	wxString imgdir;

	QMenu* menu = nullptr;
	QMenu* columnsMenu = nullptr;
	QMenu* indexesMenu = nullptr;
	wxColour bkgColor{ 43, 45, 48, 43 };
	wxColour textColor{223, 225, 229, 213};

	DatabaseService * databaseService = DatabaseService::getInstance();

	void createTableMenu();
	void createColumnsMenu();
	void createIndexesMenu();
};

