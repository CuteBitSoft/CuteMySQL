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
#include "common/AppContext.h"
#include "utils/ResourceUtil.h"
#include "core/common/Lang.h"
#include "core/entity/Entity.h"
#include "core/entity/Enum.h"
#include "ui/common/data/QTreeItemData.h"
#include "ui/common/msgbox/QAnimateBox.h"
#include "ui/common/msgbox/QConfirmBox.h"
#include "ui/dialog/duplicate/connection/DuplicateConnectionDialog.h"
#include "ui/dialog/duplicate/database/DuplicateDatabaseDialog.h"
#include "ui/dialog/duplicate/table/DuplicateTableDialog.h"
#include "ui/dialog/duplicate/object/DuplicateObjectDialog.h"

const std::unordered_map<std::string, TreeObjectType> LeftTreeDelegate::objectTypeMap{
	{"DATABASE", TreeObjectType::SCHEMA},
	{"TABLE", TreeObjectType::TABLE},
	{"VIEW", TreeObjectType::VIEW},
	{"PROCEDURE", TreeObjectType::STORE_PROCEDURE},
	{"FUNCTION", TreeObjectType::FUNCTION},
	{"TRIGGER", TreeObjectType::TRIGGER},
	{"EVENT", TreeObjectType::EVENT}
};
LeftTreeDelegate::~LeftTreeDelegate()
{
	ConnectService::destroyInstance();
	connectService = nullptr;

	DatabaseService::destroyInstance();
	databaseService = nullptr;

	MetadataService::destroyInstance();
	metadataService = nullptr;
}

/**
 * load item for treeView.
 * 
 * @param treeView
 * @param connectId - selected connection
 * @param schema - selected schema
 */
void LeftTreeDelegate::loadForLeftTree(wxTreeCtrl* treeView, uint64_t connectId /*= 0 */, const std::string & schema /* = "" */, bool allowSelect/* = true */)
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
	
	UserConnectList userConnectList;
	try {
		userConnectList = connectService->getAllUserConnects();
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	for (auto& item : userConnectList) {
		QTreeItemData<UserConnect>* data = new QTreeItemData<UserConnect>(item.id, new UserConnect(item), TreeObjectType::CONNECTION);
		std::string connectName = item.name;
		connectName.append(" [").append(item.userName).append("@").append(item.host).append("]");
		auto itemId = treeView->AppendItem(rootId, connectName, 1, 1, data);
		if (connectId == item.id) {
			loadDbsForConnection(treeView, itemId, connectId, schema);
			treeView->Expand(itemId);
		} else {
			// lazy load connection
			loadingForItem(treeView, itemId);
		}
	}
	
	if (!connectId && allowSelect) {
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
		if (data->getType() == TreeObjectType::TABLES_FOLDER) { // TABLE folder
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadTablesForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} else if (data->getType() == TreeObjectType::VIEWS_FOLDER) { // VIEW folder
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadViewsForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} else if (data->getType() == TreeObjectType::STORE_PROCEDURES_FOLDER) { // STORE PROCEDURE folder
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadProceduresForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} else if (data->getType() == TreeObjectType::FUNCTIONS_FOLDER) { // FUNCTION folder
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadFunctionsForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} else if (data->getType() == TreeObjectType::TRIGGERS_FOLDER) { // TRIGGER folder
			auto userDbData = (QTreeItemData<UserDb> *)data;
			auto connectId = userDbData->getDataId();
			auto userDbPtr = userDbData->getDataPtr();
			loadTriggersForDatabase(treeView, itemId, connectId, userDbPtr->name);
		} else if (data->getType() == TreeObjectType::EVENTS_FOLDER) { // EVENTS folder
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
	if (data->getType() != TreeObjectType::SCHEMA) {
		return nullptr;
	}
	auto findData = reinterpret_cast<QTreeItemData<UserDb> *>(data);
	return findData->getDataPtr();
}

UserTable* LeftTreeDelegate::getSelectedTableItemData(wxTreeCtrl* treeView)
{
	auto selItemId = treeView->GetSelection();
	if (selItemId == treeView->GetRootItem()) {
		return nullptr;
	}
	auto data = (QTreeItemData<int> *) treeView->GetItemData(selItemId);
	auto pitemId = selItemId;
	while (data->getType() != TreeObjectType::TABLE) {
		pitemId = treeView->GetItemParent(pitemId);
		if (!pitemId.IsOk()) {
			return nullptr;
		}
		data = (QTreeItemData<int> *) treeView->GetItemData(pitemId);
	}
	if (data->getType() != TreeObjectType::TABLE) {
		return nullptr;
	}
	auto findData = reinterpret_cast<QTreeItemData<UserTable> *>(data);
	return findData->getDataPtr();
}

UserView* LeftTreeDelegate::getSelectedViewItemData(wxTreeCtrl* treeView)
{
	auto selItemId = treeView->GetSelection();
	if (selItemId == treeView->GetRootItem()) {
		return nullptr;
	}
	auto data = (QTreeItemData<int> *) treeView->GetItemData(selItemId);
	auto pitemId = selItemId;
	while (data->getType() != TreeObjectType::VIEW) {
		pitemId = treeView->GetItemParent(pitemId);
		if (!pitemId.IsOk()) {
			return nullptr;
		}
		data = (QTreeItemData<int> *) treeView->GetItemData(pitemId);
	}
	if (data->getType() != TreeObjectType::VIEW) {
		return nullptr;
	}
	auto findData = reinterpret_cast<QTreeItemData<UserView> *>(data);
	return findData->getDataPtr();
}

UserRoutine* LeftTreeDelegate::getSelectedRoutineItemData(wxTreeCtrl* treeView)
{
	auto selItemId = treeView->GetSelection();
	if (selItemId == treeView->GetRootItem()) {
		return nullptr;
	}
	auto data = (QTreeItemData<int> *) treeView->GetItemData(selItemId);
	auto pitemId = selItemId;
	while (data->getType() != TreeObjectType::STORE_PROCEDURE 
		&& data->getType() != TreeObjectType::FUNCTION) {
		pitemId = treeView->GetItemParent(pitemId);
		if (!pitemId.IsOk()) {
			return nullptr;
		}
		data = (QTreeItemData<int> *) treeView->GetItemData(pitemId);
	}
	if (data->getType() != TreeObjectType::STORE_PROCEDURE 
		&& data->getType() != TreeObjectType::FUNCTION) {
		return nullptr;
	}
	auto findData = reinterpret_cast<QTreeItemData<UserRoutine> *>(data);
	return findData->getDataPtr();
}

UserTrigger* LeftTreeDelegate::getSelectedTriggerItemData(wxTreeCtrl* treeView)
{
	auto selItemId = treeView->GetSelection();
	if (selItemId == treeView->GetRootItem()) {
		return nullptr;
	}
	auto data = (QTreeItemData<int> *) treeView->GetItemData(selItemId);
	auto pitemId = selItemId;
	while (data->getType() != TreeObjectType::TRIGGER) {
		pitemId = treeView->GetItemParent(pitemId);
		if (!pitemId.IsOk()) {
			return nullptr;
		}
		data = (QTreeItemData<int> *) treeView->GetItemData(pitemId);
	}
	if (data->getType() != TreeObjectType::TRIGGER) {
		return nullptr;
	}
	auto findData = reinterpret_cast<QTreeItemData<UserTrigger> *>(data);
	return findData->getDataPtr();
}

UserEvent* LeftTreeDelegate::getSelectedEventItemData(wxTreeCtrl* treeView)
{
	auto selItemId = treeView->GetSelection();
	if (selItemId == treeView->GetRootItem()) {
		return nullptr;
	}
	auto data = (QTreeItemData<int> *) treeView->GetItemData(selItemId);
	auto pitemId = selItemId;
	while (data->getType() != TreeObjectType::EVENT) {
		pitemId = treeView->GetItemParent(pitemId);
		if (!pitemId.IsOk()) {
			return nullptr;
		}
		data = (QTreeItemData<int> *) treeView->GetItemData(pitemId);
	}
	if (data->getType() != TreeObjectType::EVENT) {
		return nullptr;
	}
	auto findData = reinterpret_cast<QTreeItemData<UserEvent> *>(data);
	return findData->getDataPtr();
}

bool LeftTreeDelegate::removeForLeftTree(wxTreeCtrl* treeView)
{
	if (!treeView) {
		return false;
	}
	auto selItemId = treeView->GetSelection();
	if (selItemId == treeView->GetRootItem()) {
		return false;
	}

	auto data = (QTreeItemData<int> *) treeView->GetItemData(selItemId);
	if (!data) {
		return false;
	}

	if (data->getType() == TreeObjectType::TABLES_FOLDER 
		|| data->getType() == TreeObjectType::VIEWS_FOLDER
		|| data->getType() == TreeObjectType::STORE_PROCEDURES_FOLDER
		|| data->getType() == TreeObjectType::FUNCTIONS_FOLDER
		|| data->getType() == TreeObjectType::TRIGGERS_FOLDER
		|| data->getType() == TreeObjectType::EVENTS_FOLDER
		|| data->getType() == TreeObjectType::TABLE_COLUMNS_FOLDER
		|| data->getType() == TreeObjectType::TABLE_INDEXES_FOLDER) {
		QAnimateBox::warning(S("folder-cannot-delete"));
		return false;
	}

	if (QConfirmBox::confirm(S("remove-item-confirm")) != wxID_OK) {
		return false;
	}

	if (data->getType() == TreeObjectType::CONNECTION) {
		return removeConnectionItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::SCHEMA) {
		return removeDatabaseItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::TABLE) {
		return removeTableItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::VIEW) {
		return removeViewItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::STORE_PROCEDURE) {
		return removeProcedureItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::FUNCTION) {
		return removeFunctionItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::TRIGGER) {
		return removeTriggerItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::EVENT) {
		return removeEventItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::TABLE_COLUMN) {
		return removeTableColumnItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::TABLE_INDEX) {
		return removeTableIndexItem(treeView, selItemId);
	}
	return false;
}

bool LeftTreeDelegate::duplicateForLeftTree(wxTreeCtrl* treeView)
{
	if (!treeView) {
		return false;
	}
	auto selItemId = treeView->GetSelection();
	if (selItemId == treeView->GetRootItem()) {
		return false;
	}

	auto data = (QTreeItemData<int> *) treeView->GetItemData(selItemId);
	if (!data) {
		return false;
	}

	if (data->getType() == TreeObjectType::TABLES_FOLDER
		|| data->getType() == TreeObjectType::VIEWS_FOLDER
		|| data->getType() == TreeObjectType::STORE_PROCEDURES_FOLDER
		|| data->getType() == TreeObjectType::FUNCTIONS_FOLDER
		|| data->getType() == TreeObjectType::TRIGGERS_FOLDER
		|| data->getType() == TreeObjectType::EVENTS_FOLDER
		|| data->getType() == TreeObjectType::TABLE_COLUMNS_FOLDER
		|| data->getType() == TreeObjectType::TABLE_INDEXES_FOLDER) {
		QAnimateBox::warning(S("folder-cannot-duplicate"));
		return false;
	}

	if (data->getType() == TreeObjectType::CONNECTION) {
		return duplicateConnectionItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::SCHEMA) {
		return duplicateDatabaseItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::TABLE) {
		return duplicateTableItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::VIEW) {
		return duplicateViewItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::STORE_PROCEDURE) {
		return duplicateProcedureItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::FUNCTION) {
		return duplicateFunctionItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::TRIGGER) {
		return duplicateTriggerItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::EVENT) {
		return duplicateEventItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::TABLE_COLUMN) {
		return duplicateTableColumnItem(treeView, selItemId);
	} else if (data->getType() == TreeObjectType::TABLE_INDEX) {
		return duplicateTableIndexItem(treeView, selItemId);
	}
	return false;
}

/**
 * Prepare handleUserDb and handleUserObject data for refresh.
 * 
 * @param treeView
 */
void LeftTreeDelegate::beforeFreshForLeftTree(wxTreeCtrl* treeView)
{
	// 1. Prepare handleDb
	auto selItemId = treeView->GetSelection();
	if (!selItemId.IsOk() || selItemId == treeView->GetRootItem()) {
		return;
	}

	auto data = reinterpret_cast<QTreeItemData<int>*>(treeView->GetItemData(selItemId));
	if (data == nullptr || supplier->runtimeUserDb == nullptr) {
		return;
	}
	supplier->handleUserDb = *supplier->runtimeUserDb;
	if (data->getType() == TreeObjectType::SCHEMA) {		
		supplier->handleUserObject = UserObject();
	} else if (data->getType() == TreeObjectType::TABLE 
		|| data->getType() == TreeObjectType::TABLE_COLUMNS_FOLDER
		|| data->getType() == TreeObjectType::TABLE_COLUMN
		|| data->getType() == TreeObjectType::TABLE_INDEXES_FOLDER
		|| data->getType() == TreeObjectType::TABLE_INDEX
		) {
		supplier->handleUserObject = *supplier->runtimeUserTable;
	} else if (data->getType() == TreeObjectType::VIEW) {
		supplier->handleUserObject = *supplier->runtimeUserView;
		supplier->handleUserObject.type = "VIEW";
	} else if (data->getType() == TreeObjectType::STORE_PROCEDURE) {
		supplier->handleUserObject.schema = supplier->runtimeUserRoutine->schema;
		supplier->handleUserObject.name = supplier->runtimeUserRoutine->name;
		supplier->handleUserObject.type = "PROCEDURE";
	} else if (data->getType() == TreeObjectType::FUNCTION) {
		supplier->handleUserObject.schema = supplier->runtimeUserRoutine->schema;
		supplier->handleUserObject.name = supplier->runtimeUserRoutine->name;
		supplier->handleUserObject.type = "FUNCTION";
	} else if (data->getType() == TreeObjectType::TRIGGER) {
		supplier->handleUserObject.schema = supplier->runtimeUserTrigger->schema;
		supplier->handleUserObject.name = supplier->runtimeUserTrigger->name;
		supplier->handleUserObject.type = "TRIGGER";
	} else if (data->getType() == TreeObjectType::EVENT) {
		supplier->handleUserObject.schema = supplier->runtimeUserEvent->schema;
		supplier->handleUserObject.name = supplier->runtimeUserEvent->name;
		supplier->handleUserObject.type = "EVENT";
	} else if (data->getType() == TreeObjectType::TABLES_FOLDER) {
		supplier->handleUserObject.schema = supplier->runtimeUserDb->name;
		supplier->handleUserObject.name = "";
		supplier->handleUserObject.type = "TABLE";
	} else if (data->getType() == TreeObjectType::VIEWS_FOLDER) {
		supplier->handleUserObject.schema = supplier->runtimeUserDb->name;
		supplier->handleUserObject.name = "";
		supplier->handleUserObject.type = "VIEW";
	} else if (data->getType() == TreeObjectType::STORE_PROCEDURES_FOLDER) {
		supplier->handleUserObject.schema = supplier->runtimeUserDb->name;
		supplier->handleUserObject.name = "";
		supplier->handleUserObject.type = "PROCEDURE";
	} else if (data->getType() == TreeObjectType::FUNCTIONS_FOLDER) {
		supplier->handleUserObject.schema = supplier->runtimeUserDb->name;
		supplier->handleUserObject.name = "";
		supplier->handleUserObject.type = "FUNCTION";
	} else if (data->getType() == TreeObjectType::TRIGGERS_FOLDER) {
		supplier->handleUserObject.schema = supplier->runtimeUserDb->name;
		supplier->handleUserObject.name = "";
		supplier->handleUserObject.type = "TRIGGER";
	} else if (data->getType() == TreeObjectType::EVENTS_FOLDER) {
		supplier->handleUserObject.schema = supplier->runtimeUserDb->name;
		supplier->handleUserObject.name = "";
		supplier->handleUserObject.type = "EVENT";
	}
}

void LeftTreeDelegate::refreshConnectItemsForLeftTree(wxTreeCtrl* treeView, uint64_t connectId, const std::string& schema)
{
	// 1.Find the connection item
	auto connectItemId = findConnectItemFromRootItem(treeView, connectId);
	if (!connectItemId.IsOk()) {
		return;
	}

	// 2.Expend the connection item and load databases for the connectItemId
	expendedConnectionItem(treeView, connectItemId, connectId);

	// 3.Find the database item and load object
	auto dbItemId = findDbItemFromConnectionItem(treeView, connectItemId, schema);
	
	if (!dbItemId.IsOk()) {
		return;
	}

	// 4.select the database item
	treeView->SelectItem(dbItemId);
}

/**
 * .
 * 
 * @param treeView
 * @param connectId
 * @param schema
 * @param findSelData - Data of will be selected item 
 *					params: findSelData.type - find type(TreeObjectType::TABLE/VIEW/TRIGGER/STORE_PROCEDURE/FUNCTION/EVENT);   
 *							 findSelData.dataPtr - find object name(such as tableName/viewName/triggerName...)
 */
void LeftTreeDelegate::refreshDbItemsForLeftTree(wxTreeCtrl* treeView, uint64_t connectId, const std::string& schema, const QTreeItemData<std::string>& findSelData)
{
	// 1.Find the connection item
	auto connectItemId = findConnectItemFromRootItem(treeView, connectId);
	if (!connectItemId.IsOk()) {
		return;
	}
	// 2.Expend the connection item and load databases for the connectItemId
	expendedConnectionItem(treeView, connectItemId, connectId);

	// 3.Find the database item and load object
	auto dbItemId = findDbItemFromConnectionItem(treeView, connectItemId, schema);
	
	if (!dbItemId.IsOk()) {
		return;
	}
	
	// 4.Expend database item
	auto folderType = objectTypeToFolderType((TreeObjectType)findSelData.getType());

	// 5.select object item
	auto folderItemId = expendedDbItem(treeView, dbItemId, folderType);	
	selectDbObjectItemFromFolder(treeView, folderItemId, findSelData);
	return ;
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

/**
 * Expend the database item.
 * 
 * @param treeView
 * @param itemId
 * @param findFolderType - tree item type for selected
 * @return folder item if matching  findData.type
 */
wxTreeItemId LeftTreeDelegate::expendedDbItem(wxTreeCtrl* treeView, wxTreeItemId& itemId, const TreeObjectType &findFolderType)
{
	if (!itemId.IsOk()) {
		return wxTreeItemId();
	}

	wxTreeItemIdValue cookie;
	auto objectId = treeView->GetFirstChild(itemId, cookie);

	//auto folderType = objectTypeToFolderType((TreeObjectType)findData.getType());
	auto folderItem = findFolderItemFromDbItem(treeView, itemId, findFolderType);
	if (!folderItem.IsOk()) {
		return wxTreeItemId();
	}
	// Delete all children items
	treeView->DeleteChildren(folderItem);

	auto data = reinterpret_cast<QTreeItemData<UserDb>*>(treeView->GetItemData(itemId));
	if (findFolderType == TreeObjectType::TABLES_FOLDER) {
		loadTablesForDatabase(treeView, folderItem, data->getDataPtr()->connectId, data->getDataPtr()->name);
	} else if (findFolderType == TreeObjectType::VIEWS_FOLDER) {
		loadViewsForDatabase(treeView, folderItem, data->getDataPtr()->connectId, data->getDataPtr()->name);
	} else if (findFolderType == TreeObjectType::STORE_PROCEDURES_FOLDER) {
		loadProceduresForDatabase(treeView, folderItem, data->getDataPtr()->connectId, data->getDataPtr()->name);
	} else if (findFolderType == TreeObjectType::FUNCTIONS_FOLDER) {
		loadFunctionsForDatabase(treeView, folderItem, data->getDataPtr()->connectId, data->getDataPtr()->name);
	} else if (findFolderType == TreeObjectType::TRIGGERS_FOLDER) {
		loadTriggersForDatabase(treeView, folderItem, data->getDataPtr()->connectId, data->getDataPtr()->name);
	} else if (findFolderType == TreeObjectType::EVENTS_FOLDER) {
		loadEventsForDatabase(treeView, folderItem, data->getDataPtr()->connectId, data->getDataPtr()->name);
	}
	
	return folderItem;
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
 * Databases(Schemas) of connection node.
 * 
 * @param treeView
 * @param treeItemId
 * @param connectId
 * @param schema - selected schema
 */
void LeftTreeDelegate::loadDbsForConnection(wxTreeCtrl* treeView, const wxTreeItemId & connectItemId, uint64_t connectId, const std::string & schema /* = "" */)
{
	if (!connectItemId.IsOk() || !connectId) {
		return;
	}

	try {
		UserDbList userDbList = databaseService->getAllUserDbs(connectId);
		wxTreeItemId selDbItemId;
		for (auto& item : userDbList) {
			QTreeItemData<UserDb>* data = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::SCHEMA);
			auto dbItemId = treeView->AppendItem(connectItemId, item.name, 2, 2, data);
			
			auto tblsFolderData = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::TABLES_FOLDER);
			auto tblsFolderItemId = treeView->AppendItem(dbItemId, S("tables"), 3, 3, tblsFolderData);

			auto viewsFolderData = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::VIEWS_FOLDER);
			auto viewsFolderItemId = treeView->AppendItem(dbItemId, S("views"), 3, 3, viewsFolderData);

			auto storeProcsFolderData = new QTreeItemData<UserDb>(connectId, new UserDb(item), TreeObjectType::STORE_PROCEDURES_FOLDER);
			auto storeProcsFolderItemId = treeView->AppendItem(dbItemId, S("store-procedures"), 3, 3, storeProcsFolderData);

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

			if (!schema.empty() && schema == item.name) {
				selDbItemId = dbItemId;
			}
		}
		if (selDbItemId.IsOk()) {
			treeView->SelectItem(selDbItemId);
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


bool LeftTreeDelegate::removeConnectionItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		QAnimateBox::error(ERR("200004"));
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserConnect> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		QAnimateBox::error(ERR("200005"));
		return false;
	}

	try {
		connectService->removeUserConnect(data->getDataId());
		treeView->Delete(itemId);
		return true;
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}	
	return false;
}

bool LeftTreeDelegate::removeDatabaseItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserDb> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->name == "information_schema"
		|| data->getDataPtr()->name == "mysql"
		|| data->getDataPtr()->name == "performance_schema"
		|| data->getDataPtr()->name == "sys") {
		QAnimateBox::error(S("system-schema-cannot-delete"));
		return false;
	}

	try {
		databaseService->removeUserDb(data->getDataId(), data->getDataPtr()->name);
		treeView->Delete(itemId);

		return true;
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	return false;
}

bool LeftTreeDelegate::removeTableItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserTable> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->schema == "information_schema"
		|| data->getDataPtr()->schema == "mysql"
		|| data->getDataPtr()->schema == "performance_schema"
		|| data->getDataPtr()->schema == "sys") {
		QAnimateBox::error(S("system-object-cannot-delete"));
		return false;
	}

	try {
		metadataService->removeUserTable(data->getDataId(), data->getDataPtr()->schema, data->getDataPtr()->name);
		treeView->Delete(itemId);

		return true;
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	return false;
}

bool LeftTreeDelegate::removeViewItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserView> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->schema == "information_schema"
		|| data->getDataPtr()->schema == "mysql"
		|| data->getDataPtr()->schema == "performance_schema"
		|| data->getDataPtr()->schema == "sys") {
		QAnimateBox::error(S("system-object-cannot-delete"));
		return false;
	}

	try {
		metadataService->removeUserView(data->getDataId(), data->getDataPtr()->schema, data->getDataPtr()->name);
		treeView->Delete(itemId);

		return true;
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	return false;
}

bool LeftTreeDelegate::removeProcedureItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserProcedure> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->schema == "information_schema"
		|| data->getDataPtr()->schema == "mysql"
		|| data->getDataPtr()->schema == "performance_schema"
		|| data->getDataPtr()->schema == "sys") {
		QAnimateBox::error(S("system-object-cannot-delete"));
		return false;
	}

	try {
		metadataService->removeUserProcedure(data->getDataId(), data->getDataPtr()->schema, data->getDataPtr()->name);
		treeView->Delete(itemId);

		return true;
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	return false;
}

bool LeftTreeDelegate::removeFunctionItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserFunction> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->schema == "information_schema"
		|| data->getDataPtr()->schema == "mysql"
		|| data->getDataPtr()->schema == "performance_schema"
		|| data->getDataPtr()->schema == "sys") {
		QAnimateBox::error(S("system-object-cannot-delete"));
		return false;
	}

	try {
		metadataService->removeUserFunction(data->getDataId(), data->getDataPtr()->schema, data->getDataPtr()->name);
		treeView->Delete(itemId);

		return true;
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	return false;
}

bool LeftTreeDelegate::removeTriggerItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserTrigger> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	try {
		metadataService->removeUserTrigger(data->getDataId(), data->getDataPtr()->schema, data->getDataPtr()->name);
		treeView->Delete(itemId);

		return true;
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	return false;
}

bool LeftTreeDelegate::removeEventItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserEvent> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->schema == "information_schema"
		|| data->getDataPtr()->schema == "mysql"
		|| data->getDataPtr()->schema == "performance_schema"
		|| data->getDataPtr()->schema == "sys") {
		QAnimateBox::error(S("system-object-cannot-delete"));
		return false;
	}

	try {
		metadataService->removeUserEvent(data->getDataId(), data->getDataPtr()->schema, data->getDataPtr()->name);
		treeView->Delete(itemId);

		return true;
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	return false;
}

bool LeftTreeDelegate::removeTableColumnItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<ColumnInfo> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->schema == "information_schema"
		|| data->getDataPtr()->schema == "mysql"
		|| data->getDataPtr()->schema == "performance_schema"
		|| data->getDataPtr()->schema == "sys") {
		QAnimateBox::error(S("system-object-cannot-delete"));
		return false;
	}

	try {
		metadataService->removeTableColumn(data->getDataId(), data->getDataPtr()->schema, data->getDataPtr()->table, data->getDataPtr()->name);
		treeView->Delete(itemId);

		return true;
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	return false;
}

bool LeftTreeDelegate::removeTableIndexItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<IndexInfo> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->schema == "information_schema"
		|| data->getDataPtr()->schema == "mysql"
		|| data->getDataPtr()->schema == "performance_schema"
		|| data->getDataPtr()->schema == "sys") {
		QAnimateBox::error(S("system-object-cannot-delete"));
		return false;
	}

	try {
		metadataService->removeTableIndex(data->getDataId(), data->getDataPtr()->schema, data->getDataPtr()->table, data->getDataPtr()->name);
		treeView->Delete(itemId);

		return true;
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	return false;
}

bool LeftTreeDelegate::duplicateConnectionItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserConnect> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	DuplicateConnectionDialog dialog;
	dialog.Create(AppContext::getInstance()->getMainFrmWindow(), wxID_ANY, S("duplicate-connection"), wxDefaultPosition, wxDefaultSize);
	return dialog.ShowModal() == wxID_OK;
}

bool LeftTreeDelegate::duplicateDatabaseItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserDb> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->name == "information_schema"
		|| data->getDataPtr()->name == "mysql"
		|| data->getDataPtr()->name == "performance_schema"
		|| data->getDataPtr()->name == "sys") {
		QAnimateBox::error(S("system-schema-cannot-duplicate"));
		return false;
	}

	DuplicateDatabaseDialog dialog;
	dialog.Create(AppContext::getInstance()->getMainFrmWindow(), wxID_ANY, S("duplicate-database"), wxDefaultPosition, wxDefaultSize);
	return dialog.ShowModal() == wxID_OK;
}

bool LeftTreeDelegate::duplicateTableItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserTable> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->schema == "information_schema"
		|| data->getDataPtr()->schema == "mysql"
		|| data->getDataPtr()->schema == "performance_schema"
		|| data->getDataPtr()->schema == "sys") {
		QAnimateBox::error(S("system-object-cannot-duplicate"));
		return false;
	}
	DuplicateTableDialog dialog;
	dialog.Create(AppContext::getInstance()->getMainFrmWindow(), wxID_ANY, S("duplicate-table"), wxDefaultPosition, wxDefaultSize);
	return dialog.ShowModal() == wxID_OK;
}

bool LeftTreeDelegate::duplicateViewItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	return duplicateObjectItem(treeView, itemId, DuplicateObjectType::DUPLICATE_VIEW);
}

bool LeftTreeDelegate::duplicateProcedureItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	return duplicateObjectItem(treeView, itemId, DuplicateObjectType::DUPLICATE_STORE_PROCEDURE);
}

bool LeftTreeDelegate::duplicateFunctionItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	return duplicateObjectItem(treeView, itemId, DuplicateObjectType::DUPLICATE_FUNCTION);
}

bool LeftTreeDelegate::duplicateTriggerItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	return duplicateObjectItem(treeView, itemId, DuplicateObjectType::DUPLICATE_TRIGGER);
}

bool LeftTreeDelegate::duplicateEventItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	return duplicateObjectItem(treeView, itemId, DuplicateObjectType::DUPLICATE_EVENT);
}

bool LeftTreeDelegate::duplicateTableColumnItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	return false;
}

bool LeftTreeDelegate::duplicateTableIndexItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId)
{
	return false;
}

/**
 * Duplicate the object item.(object include:view/store procedure/function/trigger/event)
 * 
 * @param treeView
 * @param itemId
 * @param type - object type ,must be VIEW/STORE_PROCEDURE/FUNCTION/TRIGGER/EVENT
 * @return 
 */
bool LeftTreeDelegate::duplicateObjectItem(wxTreeCtrl* treeView, const wxTreeItemId& itemId, DuplicateObjectType type)
{
	if (!treeView || !itemId.IsOk()) {
		return false;
	}
	auto data = reinterpret_cast<QTreeItemData<UserTable> *>(treeView->GetItemData(itemId));
	if (!data || !data->getDataId()) {
		return false;
	}

	if (data->getDataPtr()->schema == "information_schema"
		|| data->getDataPtr()->schema == "mysql"
		|| data->getDataPtr()->schema == "performance_schema"
		|| data->getDataPtr()->schema == "sys") {
		QAnimateBox::error(S("system-object-cannot-duplicate"));
		return false;
	}
	DuplicateObjectDialog dialog(type);
	dialog.Create(AppContext::getInstance()->getMainFrmWindow(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	return dialog.ShowModal() == wxID_OK;
}

wxTreeItemId LeftTreeDelegate::findConnectItemFromRootItem(wxTreeCtrl* treeView, uint64_t connectId)
{
	auto rootItemId = treeView->GetRootItem();
	wxTreeItemIdValue cookie;
	auto connectItemId = treeView->GetFirstChild(rootItemId, cookie);
	while (connectItemId.IsOk()) {
		auto data = reinterpret_cast<QTreeItemData<UserConnect>*>(treeView->GetItemData(connectItemId));
		if (data == nullptr || data->getDataPtr() == nullptr) {
			return wxTreeItemId();
		}
		// found
		if (data->getDataPtr()->id == connectId) {
			break;
		}
		connectItemId = treeView->GetNextSibling(connectItemId);
	}
	return connectItemId;
}

/**
 * Find children database item from connection item..
 * 
 * @param treeView
 * @param connectItemId
 * @param schema
 * @return 
 */
wxTreeItemId LeftTreeDelegate::findDbItemFromConnectionItem(wxTreeCtrl* treeView, const wxTreeItemId& connectItemId, const std::string& schema)
{
	if (!connectItemId.IsOk()) {
		return wxTreeItemId();
	}
	wxTreeItemIdValue cookie;
	auto dbItemId = treeView->GetFirstChild(connectItemId, cookie);
	while (dbItemId.IsOk()) {
		auto data = reinterpret_cast<QTreeItemData<UserDb>*>(treeView->GetItemData(dbItemId));
		if (data == nullptr || data->getDataPtr() == nullptr) {
			return wxTreeItemId();
		}
		// found
		if (data->getDataPtr()->name == schema) {
			break;
		}
		dbItemId = treeView->GetNextSibling(dbItemId);
	}
	return dbItemId;
}

/**
 * Find children folder item from database item.
 * 
 * @param treeView
 * @param dbItemId
 * @param folderType
 * @return 
 */
wxTreeItemId LeftTreeDelegate::findFolderItemFromDbItem(wxTreeCtrl* treeView, const wxTreeItemId& dbItemId, const TreeObjectType folderType)
{
	if (!dbItemId.IsOk()) {
		return wxTreeItemId();
	}

	wxTreeItemIdValue cookie;
	auto follderItemId = treeView->GetFirstChild(dbItemId, cookie);
	while (follderItemId.IsOk()) {
		auto data = reinterpret_cast<QTreeItemData<UserDb>*>(treeView->GetItemData(follderItemId));
		if (data == nullptr || data->getDataPtr() == nullptr) {
			return wxTreeItemId();
		}
		// found
		if (data->getType() == folderType) {
			break;
		}
		follderItemId = treeView->GetNextSibling(follderItemId);
	}
	return follderItemId;
}

TreeObjectType LeftTreeDelegate::objectTypeToFolderType(const TreeObjectType objectType)
{
	auto folderType = TreeObjectType::TABLES_FOLDER;
	if (objectType == TreeObjectType::TABLE) {
		folderType = TreeObjectType::TABLES_FOLDER;
	} else if (objectType == TreeObjectType::VIEW) {
		folderType = TreeObjectType::VIEWS_FOLDER;
	} else if (objectType == TreeObjectType::STORE_PROCEDURE) {
		folderType = TreeObjectType::STORE_PROCEDURES_FOLDER;
	} else if (objectType == TreeObjectType::FUNCTION) {
		folderType = TreeObjectType::FUNCTIONS_FOLDER;
	} else if (objectType == TreeObjectType::TRIGGER) {
		folderType = TreeObjectType::TRIGGERS_FOLDER;
	} else if (objectType == TreeObjectType::EVENT) {
		folderType = TreeObjectType::EVENTS_FOLDER;
	}

	return folderType;
}

/**
 * Select children object item in the specified database folder.
 * 
 * @param treeView
 * @param folderItemId
 * @param findSelData - Data of will be selected item 
 *						params: findSelData.type - find type(TreeObjectType::TABLE/VIEW/TRIGGER/STORE_PROCEDURE/FUNCTION/EVENT);   
 *								findSelData.dataPtr - find object name(such as tableName/viewName/triggerName...)
 */
void LeftTreeDelegate::selectDbObjectItemFromFolder(wxTreeCtrl* treeView, const wxTreeItemId& folderItemId, const QTreeItemData<std::string>& findSelData)
{
	if (!folderItemId.IsOk()) {
		return;
	}
	if (findSelData.getDataPtr() == nullptr || findSelData.getDataPtr()->empty()) {
		treeView->SelectItem(folderItemId);
		return;
	}

	wxTreeItemIdValue cookie;
	auto itemId = treeView->GetFirstChild(folderItemId, cookie);

	std::string itemName;
	while (itemId.IsOk()) {
		if (findSelData.getType() == TreeObjectType::TABLE) {
			auto itemData = reinterpret_cast<QTreeItemData<UserTable>*>(treeView->GetItemData(itemId));
			itemName = itemData->getDataPtr()->name;
		} else if (findSelData.getType() == TreeObjectType::VIEW) {
			auto itemData = reinterpret_cast<QTreeItemData<UserView>*>(treeView->GetItemData(itemId));
			itemName = itemData->getDataPtr()->name;
		} else if (findSelData.getType() == TreeObjectType::STORE_PROCEDURE 
			|| findSelData.getType() == TreeObjectType::FUNCTION
			|| findSelData.getType() == TreeObjectType::TRIGGER) {
			auto itemData = reinterpret_cast<QTreeItemData<UserRoutine>*>(treeView->GetItemData(itemId));
			itemName = itemData->getDataPtr()->name;
		}else if (findSelData.getType() == TreeObjectType::EVENT) {
			auto itemData = reinterpret_cast<QTreeItemData<UserEvent>*>(treeView->GetItemData(itemId));
			itemName = itemData->getDataPtr()->name;
		} 

		if (itemName == *findSelData.getDataPtr()) {
			break;
		}
		itemId = treeView->GetNextSibling(itemId);
	}

	if (itemId.IsOk()) {
		treeView->SelectItem(itemId);
	}
}
