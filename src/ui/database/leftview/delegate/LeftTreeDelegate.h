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
 * @file   LeftTreeDelegate.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-23
 *********************************************************************/
#pragma once
#include <wx/treectrl.h>
#include "ui/common/delegate/QDelegate.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "core/service/db/ConnectService.h"
#include "core/service/db/DatabaseService.h"
#include "core/service/db/MetadataService.h"

class LeftTreeDelegate :  public QDelegate<LeftTreeDelegate, DatabaseSupplier>
{
public:
	~LeftTreeDelegate();
	void loadForLeftTree(wxTreeCtrl * treeView, uint64_t connectId = 0);
	void expendedForLeftTree(wxTreeCtrl * treeView, wxTreeItemId &itemId);
	void expendedConnectionItem(wxTreeCtrl* treeView, wxTreeItemId& itemId, uint64_t connectId);

	UserConnect* getSelectedConnectItemData(wxTreeCtrl* treeView);
	UserDb* getSelectedDbItemData(wxTreeCtrl* treeView);
private:
	ConnectService * connectService = ConnectService::getInstance();
	DatabaseService * databaseService = DatabaseService::getInstance();
	MetadataService * metadataService = MetadataService::getInstance();

	// For Connection
	void loadDbsForConnection(wxTreeCtrl * treeView, const wxTreeItemId & connectItemId, uint64_t connectId = 0);
	// For Database
	void loadTablesForDatabase(wxTreeCtrl * treeView, const wxTreeItemId & folderItemId, uint64_t connectId, const std::string & schema);
	void loadViewsForDatabase(wxTreeCtrl * treeView, const wxTreeItemId & folderItemId, uint64_t connectId, const std::string & schema);
	void loadProceduresForDatabase(wxTreeCtrl * treeView, const wxTreeItemId & folderItemId, uint64_t connectId, const std::string & schema);
	void loadFunctionsForDatabase(wxTreeCtrl * treeView, const wxTreeItemId & folderItemId, uint64_t connectId, const std::string & schema);
	void loadTriggersForDatabase(wxTreeCtrl * treeView, const wxTreeItemId & folderItemId, uint64_t connectId, const std::string & schema);
	void loadEventsForDatabase(wxTreeCtrl * treeView, const wxTreeItemId & folderItemId, uint64_t connectId, const std::string & schema);

	// For Table
	void loadColomnsForTable(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId, const std::string& schema, const std::string tableName);
	void loadIndexesForTable(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId, const std::string& schema, const std::string tableName);

	
	void expendedTableItem(wxTreeCtrl* treeView, wxTreeItemId& itemId, uint64_t connectId, UserTable * userTable);

	void loadingForItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	void loadingForFolder(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId);
};

