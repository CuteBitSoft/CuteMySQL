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
	void loadForLeftTree(wxTreeCtrl * treeView, uint64_t connectId = 0, const std::string & schema = "");
	void expendedForLeftTree(wxTreeCtrl * treeView, wxTreeItemId &itemId);
	void expendedConnectionItem(wxTreeCtrl* treeView, wxTreeItemId& itemId, uint64_t connectId);

	UserConnect* getSelectedConnectItemData(wxTreeCtrl* treeView);
	UserDb* getSelectedDbItemData(wxTreeCtrl* treeView);

	bool removeForLeftTree(wxTreeCtrl* treeView);
	bool duplicateForLeftTree(wxTreeCtrl* treeView);
private:
	ConnectService * connectService = ConnectService::getInstance();
	DatabaseService * databaseService = DatabaseService::getInstance();
	MetadataService * metadataService = MetadataService::getInstance();

	// For Connection
	void loadDbsForConnection(wxTreeCtrl * treeView, const wxTreeItemId & connectItemId, uint64_t connectId, const std::string & schema = "");
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

	// loading for lazy load
	void loadingForItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	void loadingForFolder(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId);

	// remove item
	bool removeConnectionItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	bool removeDatabaseItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	bool removeTableItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	bool removeViewItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	bool removeProcedureItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	bool removeFunctionItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	bool removeTriggerItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	bool removeEventItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	bool removeTableColumnItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);
	bool removeTableIndexItem(wxTreeCtrl * treeView, const wxTreeItemId & itemId);

	// duplicate item
	bool duplicateConnectionItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId);
	bool duplicateDatabaseItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId);
	bool duplicateTableItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId);
	bool duplicateViewItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId);
	bool duplicateProcedureItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId);
	bool duplicateFunctionItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId);
	bool duplicateTriggerItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId);
	bool duplicateEventItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId);
	bool duplicateTableColumnItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId);
	bool duplicateTableIndexItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId);
};

