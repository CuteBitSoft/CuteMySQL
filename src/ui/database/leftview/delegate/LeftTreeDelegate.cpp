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
 * @file   LeftTreeDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-23
 *********************************************************************/
#include "LeftTreeDelegate.h"
#include <wx/msgdlg.h>
#include "utils/ResourceUtil.h"
#include "core/common/Lang.h"
#include "core/entity/Entity.h"
#include "core/entity/Enum.h"
#include "ui/common/data/QTreeItemData.h"

LeftTreeDelegate::~LeftTreeDelegate()
{
	ConnectService::destroyInstance();
	connectService = nullptr;

	DatabaseService::destroyInstance();
	databaseService = nullptr;

	MetadataService::destroyInstance();
	metadataService = nullptr;
}

void LeftTreeDelegate::loadForLeftTree(wxTreeCtrl* treeView, uint64_t connectId)
{
	if (!treeView->HasImages()) {
		auto imgdir = ResourceUtil::getProductImagesDir();

		wxImageList* images = new wxImageList(16, 16);
	
		wxString imgpath1 = imgdir + _("/database/tree/folder.bmp");
		wxString imgpath2 = imgdir + _("/database/tree/connection.bmp");
		wxString imgpath3 = imgdir + _("/database/tree/database.bmp");
		wxString imgpath4 = imgdir + _("/database/tree/objects-folder.bmp");
		wxString imgpath5 = imgdir + _("/database/tree/table.bmp");
		wxString imgpath6 = imgdir + _("/database/tree/view.bmp");
		wxString imgpath7 = imgdir + _("/database/tree/procedure.bmp");
		wxString imgpath8 = imgdir + _("/database/tree/function.bmp");
		wxString imgpath9 = imgdir + _("/database/tree/trigger.bmp");
		wxString imgpath10 = imgdir + _("/database/tree/event.bmp");
		wxString imgpath11 = imgdir + _("/database/tree/loading.bmp");
		wxString imgpath12 = imgdir + _("/database/tree/column.bmp");
		wxString imgpath13 = imgdir + _("/database/tree/index.bmp");
		wxString imgpath14 = imgdir + _("/database/tree/primary-key.bmp");
		images->Add(wxBitmap(imgpath1, wxBITMAP_TYPE_BMP)); // 0 - folder	
		images->Add(wxBitmap(imgpath2, wxBITMAP_TYPE_BMP)); // 1 - connection
		images->Add(wxBitmap(imgpath3, wxBITMAP_TYPE_BMP)); // 2 - database
		images->Add(wxBitmap(imgpath4, wxBITMAP_TYPE_BMP)); // 3 - objects folder
		images->Add(wxBitmap(imgpath5, wxBITMAP_TYPE_BMP)); // 4 - table
		images->Add(wxBitmap(imgpath6, wxBITMAP_TYPE_BMP)); // 5 - view
		images->Add(wxBitmap(imgpath7, wxBITMAP_TYPE_BMP)); // 6 - procedure
		images->Add(wxBitmap(imgpath8, wxBITMAP_TYPE_BMP)); // 7 - function
		images->Add(wxBitmap(imgpath9, wxBITMAP_TYPE_BMP)); // 8 - trigger
		images->Add(wxBitmap(imgpath10, wxBITMAP_TYPE_BMP)); // 9 - event
		images->Add(wxBitmap(imgpath11, wxBITMAP_TYPE_BMP)); // 10 - loading
		images->Add(wxBitmap(imgpath12, wxBITMAP_TYPE_BMP)); // 11 - column
		images->Add(wxBitmap(imgpath13, wxBITMAP_TYPE_BMP)); // 12 - index
		images->Add(wxBitmap(imgpath14, wxBITMAP_TYPE_BMP)); // 13 - primary key
		treeView->AssignImageList(images); 
	}
	
	auto rootId = treeView->GetRootItem();
	if (!rootId.IsOk()) {
		QTreeItemData<UserConnect>* data = new QTreeItemData<UserConnect>(0, nullptr, TreeObjectType::ROOT);
		rootId = treeView->AddRoot(S("connect-section-text"), 0, 0, data);
	} else {
		treeView->DeleteChildren(rootId);
	}
	
	auto userConnectList = connectService->getAllUserConnects();
	for (auto& item : userConnectList) {
		QTreeItemData<UserConnect>* data = new QTreeItemData<UserConnect>(item.id, new UserConnect(item), TreeObjectType::CONNECTION);
		std::string connectName = item.name;
		connectName.append(" [").append(item.userName).append("@").append(item.host).append("]");
		auto itemId = treeView->AppendItem(rootId, connectName, 1, 1, data);
		if (connectId == item.id) {
			treeView->SelectItem(itemId);
			loadDbsForConnection(treeView, itemId, connectId);
			treeView->Expand(itemId);
		} else {
			// lazy load connection
			loadingForItem(treeView, itemId);
		}
	}
	
	if (!connectId) {
		wxTreeItemIdValue cookie;
		auto firstItemId = treeView->GetFirstChild(rootId, cookie);
		if (firstItemId.IsOk()) {
			treeView->SelectItem(firstItemId);
		}
	}
	
	treeView->Expand(rootId);
}

void LeftTreeDelegate::expendedForLeftTree(wxTreeCtrl* treeView, wxTreeItemId& itemId)
{
	if (!itemId.IsOk()) {
		return;
	}
	
	// Image id
	int nImage = treeView->GetItemImage(itemId);
	if (nImage == 1) { // 1 - connection
		auto data = (QTreeItemData<UserConnect>*)treeView->GetItemData(itemId);
		auto connectId = data->getDataPtr()->id;
		expendedConnectionItem(treeView, itemId, connectId);
	}else if (nImage == 4) { // 4 - table
		auto data = (QTreeItemData<UserTable>*)treeView->GetItemData(itemId);
		auto connectId = data->getDataId();
		auto userTable = data->getDataPtr();
		expendedTableItem(treeView, itemId, connectId, userTable);
	} else if (nImage == 3) { // 3 - objects folder
		wxTreeItemIdValue cookie;
		auto firstChildId = treeView->GetFirstChild(itemId, cookie);

		int nFirstImage = treeView->GetItemImage(firstChildId);
		if (nFirstImage != 10) { // 10 - loading
			return;
		}
		// delete loading item
		treeView->Delete(firstChildId);

		auto data = (QTreeItemData<long> *)treeView->GetItemData(itemId);
		if (data->getType() == TreeObjectType::TABLES_FOLDER) { // TABLE
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadTablesForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} else if (data->getType() == TreeObjectType::VIEWS_FOLDER) { // VIEW
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadViewsForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} else if (data->getType() == TreeObjectType::STORE_PROCEDURE_FOLDER) { // STORE PROCEDURE
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadProceduresForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} else if (data->getType() == TreeObjectType::FUNCTIONS_FOLDER) { // FUNCTION
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadFunctionsForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} else if (data->getType() == TreeObjectType::TRIGGERS_FOLDER) { // TRIGGER
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadTriggersForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} else if (data->getType() == TreeObjectType::EVENTS_FOLDER) { // EVENTS
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadEventsForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} 
	}
}

/**
 * Find parent connection item of selected item in the treeView, then return the UserConnect ptr
 * 
 * @param treeView
 * @return 
 */
UserConnect* LeftTreeDelegate::getSelectedConnectItemData(wxTreeCtrl* treeView)
{
	if (!treeView) {
		return nullptr;
	}
	auto selItemId = treeView->GetSelection();
	if (selItemId == treeView->GetRootItem()) {
		return nullptr;
	}
	auto data = (QTreeItemData<int> *) treeView->GetItemData(selItemId);
	auto pitemId = selItemId;
	while (data->getType() != TreeObjectType::CONNECTION) {
		pitemId = treeView->GetItemParent(pitemId);
		if (!pitemId.IsOk()) {
			return nullptr;
		}
		data = (QTreeItemData<int> *) treeView->GetItemData(pitemId);
	}
	auto findData = reinterpret_cast<QTreeItemData<UserConnect> *>(data);
	return findData->getDataPtr();
}

UserDb* LeftTreeDelegate::getSelectedDbItemData(wxTreeCtrl* treeView)
{
	if (!treeView) {
		return nullptr;
	}
	auto selItemId = treeView->GetSelection();
	if (selItemId == treeView->GetRootItem()) {
		return nullptr;
	}
	auto data = (QTreeItemData<int> *) treeView->GetItemData(selItemId);
	auto pitemId = selItemId;
	while (data->getType() != TreeObjectType::SCHEMA) {
		pitemId = treeView->GetItemParent(pitemId);
		if (!pitemId.IsOk()) {
			return nullptr;
		}
		data = (QTreeItemData<int> *) treeView->GetItemData(pitemId);
	}
	auto findData = reinterpret_cast<QTreeItemData<UserDb> *>(data);
	return findData->getDataPtr();
}

void LeftTreeDelegate::expendedConnectionItem(wxTreeCtrl* treeView, wxTreeItemId& itemId, uint64_t connectId)
{
	wxTreeItemIdValue cookie;
	auto firstChildId = treeView->GetFirstChild(itemId, cookie);

	int nImage = treeView->GetItemImage(firstChildId);
	if (nImage != 10) { // image: 10 - loading
		return;
	}
	treeView->Delete(firstChildId);
	loadDbsForConnection(treeView, itemId, connectId);
}

void LeftTreeDelegate::expendedTableItem(wxTreeCtrl* treeView, wxTreeItemId& itemId, uint64_t connectId, UserTable* userTable)
{
	wxTreeItemIdValue cookie;
	auto firstChildId = treeView->GetFirstChild(itemId, cookie);

	int nImage = treeView->GetItemImage(firstChildId);
	if (nImage != 10) { // image: 10 - loading
		return;
	}
	treeView->Delete(firstChildId);

	// Table - column folder
	auto columsFolderData = new QTreeItemData<UserTable>(connectId, new UserTable(*userTable), TreeObjectType::TABLE_COLUMNS_FOLDER);
	auto columnsFolderItemId = treeView->AppendItem(itemId, S("columns"), 3, 3, columsFolderData);
	// Table - column folder
	auto indexesFolderData = new QTreeItemData<UserTable>(connectId, new UserTable(*userTable), TreeObjectType::TABLE_INDEXES_FOLDER);
	auto indexesFolderItemId = treeView->AppendItem(itemId, S("indexes"), 3, 3, indexesFolderData);

	
	// Table - folder - loading
	loadColomnsForTable(treeView, columnsFolderItemId, connectId, userTable->schema, userTable->tblName);
	loadIndexesForTable(treeView, indexesFolderItemId, connectId, userTable->schema, userTable->tblName);
}

void LeftTreeDelegate::loadingForItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!itemId.IsOk()) {
		return;
	}

	wxTreeItemIdValue cookie;
	wxTreeItemId firstChildItemId = treeView->GetFirstChild(itemId, cookie);
	if (firstChildItemId.IsOk()) {
		return;
	}

	QTreeItemData<int>* data = new QTreeItemData<int>(0, 0, TreeObjectType::LOADING);
	treeView->AppendItem(itemId, S("loading"), 10, 10, data);
}

/**
 * Lazy load for folder.
 */
void LeftTreeDelegate::loadingForFolder(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId)
{
	if (!folderItemId.IsOk() || !connectId) {
		return;
	}

	wxTreeItemIdValue cookie;
	wxTreeItemId firstChildItemId = treeView->GetFirstChild(folderItemId, cookie);
	if (firstChildItemId.IsOk()) {
		return;
	}

	auto data = new QTreeItemData<long>(connectId, 0, TreeObjectType::LOADING);
	treeView->AppendItem(folderItemId, S("loading"), 10, 10, data);
}

/**
 * Databases(schemas) of connection node.
 * 
 * @param treeView
 * @param treeItemId
 * @param connectId
 */
void LeftTreeDelegate::loadDbsForConnection(wxTreeCtrl* treeView, const wxTreeItemId & connectItemId, uint64_t connectId)
{
	if (!connectItemId.IsOk() || !connectId) {
		return;
	}

	try {
		UserDbList userDbList = databaseService->getAllUserDbs(connectId);
		for (auto& item : userDbList) {
			QTreeItemData<UserDb>* data = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::SCHEMA);
			auto dbItemId = treeView->AppendItem(connectItemId, item.name, 2, 2, data);

			auto tblsFolderData = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::TABLES_FOLDER);
			auto tblsFolderItemId = treeView->AppendItem(dbItemId, S("tables"), 3, 3, tblsFolderData);

			auto viewsFolderData = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::VIEWS_FOLDER);
			auto viewsFolderItemId = treeView->AppendItem(dbItemId, S("views"), 3, 3, viewsFolderData);

			auto storeProcsFolderData = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::STORE_PROCEDURE_FOLDER);
			auto storeProcsFolderItemId = treeView->AppendItem(dbItemId, S("store-processes"), 3, 3, storeProcsFolderData);

			auto funsFolderData = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::FUNCTIONS_FOLDER);
			auto funsFolderItemId = treeView->AppendItem(dbItemId, S("functions"), 3, 3, funsFolderData);

			auto triggersFolderData = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::TRIGGERS_FOLDER);
			auto triggersFolderItemId = treeView->AppendItem(dbItemId, S("triggers"), 3, 3, triggersFolderData);

			auto eventsFolderData = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::EVENTS_FOLDER);
			auto eventsFolderItemId = treeView->AppendItem(dbItemId, S("events"), 3, 3, eventsFolderData);

			// Add a loading... child item for folder.
			loadingForFolder(treeView, tblsFolderItemId, connectId);
			loadingForFolder(treeView, viewsFolderItemId, connectId);
			loadingForFolder(treeView, storeProcsFolderItemId, connectId);
			loadingForFolder(treeView, funsFolderItemId, connectId);
			loadingForFolder(treeView, triggersFolderItemId, connectId);
			loadingForFolder(treeView, eventsFolderItemId, connectId);
		}
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}
	
}

void LeftTreeDelegate::loadTablesForDatabase(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId, const std::string& schema)
{
	if (!folderItemId.IsOk() || !connectId || schema.empty()) {
		return;
	}
	
	try {
		UserTableList list = metadataService->getUserTables(connectId, schema);
		for (auto& item : list) {
			// Table item
			auto data = new QTreeItemData<UserTable>(connectId, new UserTable(item), TreeObjectType::TABLE);
			auto tableItemId = treeView->AppendItem(folderItemId, item.name, 4, 4, data); 
			// lazy load table
			loadingForItem(treeView, tableItemId);
		}
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}
}

void LeftTreeDelegate::loadViewsForDatabase(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId, const std::string& schema)
{
	if (!folderItemId.IsOk() || !connectId || schema.empty()) {
		return;
	}
	
	try {
		UserViewList list = metadataService->getUserViews(connectId, schema);
		for (auto& item : list) {
			QTreeItemData<UserView>* data = new QTreeItemData<UserView>(connectId, new UserView(item), TreeObjectType::VIEW);
			treeView->AppendItem(folderItemId, item.name, 5, 5, data); 
		}
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}
}

void LeftTreeDelegate::loadProceduresForDatabase(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId, const std::string& schema)
{
	if (!folderItemId.IsOk() || !connectId || schema.empty()) {
		return;
	}
	
	try {
		UserProcedureList list = metadataService->getUserProcedures(connectId, schema);
		for (auto& item : list) {
			QTreeItemData<UserProcedure>* data = new QTreeItemData<UserProcedure>(connectId, new UserProcedure(item), TreeObjectType::STORE_PROCEDURE);
			treeView->AppendItem(folderItemId, item.name, 6, 6, data); 
		}
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}
}


void LeftTreeDelegate::loadFunctionsForDatabase(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId, const std::string& schema)
{
	if (!folderItemId.IsOk() || !connectId || schema.empty()) {
		return;
	}
	
	try {
		UserFunctionList list = metadataService->getUserFunctions(connectId, schema);
		for (auto& item : list) {
			QTreeItemData<UserFunction>* data = new QTreeItemData<UserFunction>(connectId, new UserFunction(item), TreeObjectType::FUNCTION);
			treeView->AppendItem(folderItemId, item.name, 7, 7, data); 
		}
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}
}

void LeftTreeDelegate::loadTriggersForDatabase(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId, const std::string& schema)
{
	if (!folderItemId.IsOk() || !connectId || schema.empty()) {
		return;
	}
	
	try {
		UserTriggerList list = metadataService->getUserTriggers(connectId, schema);
		for (auto& item : list) {
			QTreeItemData<UserTrigger>* data = new QTreeItemData<UserTrigger>(connectId, new UserTrigger(item), TreeObjectType::TRIGGER);
			treeView->AppendItem(folderItemId, item.name, 8, 8, data); 
		}
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}
}

void LeftTreeDelegate::loadEventsForDatabase(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId, const std::string& schema)
{
	if (!folderItemId.IsOk() || !connectId || schema.empty()) {
		return;
	}

	
	try {
		UserEventList list = metadataService->getUserEvents(connectId, schema);
		for (auto& item : list) {
			QTreeItemData<UserEvent>* data = new QTreeItemData<UserEvent>(connectId, new UserEvent(item), TreeObjectType::EVENT);
			treeView->AppendItem(folderItemId, item.name, 9, 9, data); 
		}
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}
}

void LeftTreeDelegate::loadColomnsForTable(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId, const std::string& schema, const std::string tableName)
{
	if (!folderItemId.IsOk() || !connectId || schema.empty() || tableName.empty()) {
		return;
	}
	
	try {
		ColumnInfoList list = metadataService->getColumnsOfUserTable(connectId, schema, tableName);
		for (auto& item : list) {
			QTreeItemData<ColumnInfo>* data = new QTreeItemData<ColumnInfo>(connectId, new ColumnInfo(item), TreeObjectType::TABLE_COLUMN);
			std::string columnName = item.name;
			columnName.append(" [")
				.append(item.type)
				.append(item.size ? "(" + std::to_string(item.size) +")" : "")
				.append(item.un ? " UNSIGNED" : "")
				.append(", ")
				.append(item.isNullable ? "NULL" : "NOT NULL")
				.append("]");
			treeView->AppendItem(folderItemId, columnName, 11, 11, data);
		}
		if (!list.empty()) {
			treeView->Expand(folderItemId);
		}
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK | wxCENTRE | wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}
}

void LeftTreeDelegate::loadIndexesForTable(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, uint64_t connectId, const std::string& schema, const std::string tableName)
{
	if (!folderItemId.IsOk() || !connectId || schema.empty() || tableName.empty()) {
		return;
	}
	
	try {
		auto list = metadataService->getIndexesOfUserTable(connectId, schema, tableName); 
		for (auto& item : list) {
			auto data = new QTreeItemData<IndexInfo>(connectId, new IndexInfo(item), TreeObjectType::TABLE_INDEX);
			std::string indexName = item.name;
			indexName.append(" (").append(item.columns).append(")");
			if (item.name != "PRIMARY" && item.un) {
				indexName.append(", ").append("UNIQUE");
			}
			int nImage = item.pk ? 13 : 12;
			treeView->AppendItem(folderItemId, indexName, nImage, nImage, data);
		}
		if (!list.empty()) {
			treeView->Expand(folderItemId);
		}
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK | wxCENTRE | wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}
}
