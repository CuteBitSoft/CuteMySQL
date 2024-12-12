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
 * @file   DuplicateTableDialogDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-30
 *********************************************************************/
#include "DuplicateTableDialogDelegate.h"
#include "ui/common/data/QClientData.h"
#include "core/entity/Entity.h"
#include "core/entity/Enum.h"

DuplicateTableDialogDelegate::DuplicateTableDialogDelegate():QDialogDelegate()
{
	
}

void DuplicateTableDialogDelegate::loadForTreeListCtrl(wxTreeListCtrl* treeListCtrl)
{
	if (!treeListCtrl->HasImages()) {
		auto imgdir = ResourceUtil::getProductImagesDir();

		wxImageList* images = new wxImageList(16, 16);
	
		wxString imgpath1 = imgdir + _("/dialog/duplicate/tree/objects-folder.bmp");
		wxString imgpath2 = imgdir + _("/dialog/duplicate/tree/table.bmp");
		wxString imgpath3 = imgdir + _("/dialog/duplicate/tree/view.bmp");
		wxString imgpath4 = imgdir + _("/dialog/duplicate/tree/procedure.bmp");
		wxString imgpath5 = imgdir + _("/dialog/duplicate/tree/function.bmp");
		wxString imgpath6 = imgdir + _("/dialog/duplicate/tree/trigger.bmp");
		wxString imgpath7 = imgdir + _("/dialog/duplicate/tree/event.bmp");
		images->Add(wxBitmap(imgpath1, wxBITMAP_TYPE_BMP)); // 0 - objects folder
		images->Add(wxBitmap(imgpath2, wxBITMAP_TYPE_BMP)); // 1 - table
		images->Add(wxBitmap(imgpath3, wxBITMAP_TYPE_BMP)); // 2 - view
		images->Add(wxBitmap(imgpath4, wxBITMAP_TYPE_BMP)); // 3 - procedure
		images->Add(wxBitmap(imgpath5, wxBITMAP_TYPE_BMP)); // 4 - function
		images->Add(wxBitmap(imgpath6, wxBITMAP_TYPE_BMP)); // 5 - trigger
		images->Add(wxBitmap(imgpath7, wxBITMAP_TYPE_BMP)); // 6 - event
		treeListCtrl->AssignImageList(images); 
	}

	if (!treeListCtrl->GetColumnCount()) {
		
		treeListCtrl->AppendColumn(wxEmptyString,
                       wxCOL_WIDTH_AUTOSIZE,
                       wxALIGN_LEFT,
                       wxCOL_RESIZABLE | wxCOL_SORTABLE);

		treeListCtrl->AppendColumn(S("source-objects"),
                       wxCOL_WIDTH_DEFAULT,
                       wxALIGN_LEFT,
                       wxCOL_RESIZABLE | wxCOL_SORTABLE);
	}
	
	auto connectId = databaseSupplier->runtimeUserConnect->id;
	auto& item = *(databaseSupplier->runtimeUserDb);
	auto& schema = databaseSupplier->runtimeUserDb->name;

	auto rootItem = treeListCtrl->GetRootItem();
	
	auto tblsFolderData = new QClientData<UserDb>(TreeObjectType::TABLES_FOLDER, new UserDb(item));
	auto tblsFolderItem = treeListCtrl->AppendItem(rootItem, S("tables"), 0, 0, tblsFolderData);

	auto viewsFolderData = new QClientData<UserDb>(TreeObjectType::VIEWS_FOLDER, new UserDb(item));
	auto viewsFolderItem = treeListCtrl->AppendItem(rootItem, S("views"), 0, 0, viewsFolderData);

	auto routinesData = new QClientData<UserDb>(TreeObjectType::ROUTINES, new UserDb(item));
	auto routinesItem = treeListCtrl->AppendItem(rootItem, S("store-procedures-and-functions"), 3, 3, routinesData);

	auto triggersFolderData = new QClientData<UserDb>(TreeObjectType::TRIGGERS_FOLDER, new UserDb(item));
	auto triggersFolderItem = treeListCtrl->AppendItem(rootItem, S("triggers"), 5, 5, triggersFolderData);

	auto eventsFolderData = new QClientData<UserDb>(TreeObjectType::EVENTS_FOLDER, new UserDb(item));
	auto eventsFolderItem = treeListCtrl->AppendItem(rootItem, S("events"), 6, 6, eventsFolderData);

	loadTablesForDatabase(treeListCtrl, tblsFolderItem, connectId, schema);
	loadViewsForDatabase(treeListCtrl, viewsFolderItem, connectId, schema);
	

	treeListCtrl->Expand(rootItem);
	treeListCtrl->CheckItem(rootItem, wxCHK_CHECKED);
	treeListCtrl->CheckItemRecursively(rootItem, wxCHK_CHECKED);
}

/**
 * Generate the string of the ignore tables from treeListCtrl.
 * Note:Generate string through unchecked tables
 * 
 * @param treeListCtrl
 * @param schema
 * @return ignore table string ,for example, "--ignore-table=schema.table_name1 --ignore-table=schema.table_name2 ..."
 */
std::string DuplicateTableDialogDelegate::getIgnoreTablesStringFromTreeListCtrl(wxTreeListCtrl* treeListCtrl, const std::string& schema)
{
	auto rootItem = treeListCtrl->GetRootItem();
	auto tablesFolderItem = treeListCtrl->GetFirstChild(rootItem);
	if (!tablesFolderItem.IsOk()) {
		return "";
	}

	auto tableItem = treeListCtrl->GetFirstChild(tablesFolderItem);
	if (!tableItem.IsOk()) {
		return "";
	}
	std::string result;
	while (tableItem.IsOk()) {
		if (treeListCtrl->GetCheckedState(tableItem)) {
			tableItem = treeListCtrl->GetNextSibling(tableItem);
			continue;
		}
		auto data = reinterpret_cast<QClientData<UserTable> *>(treeListCtrl->GetItemData(tableItem));
		if (data == nullptr) {
			tableItem = treeListCtrl->GetNextSibling(tableItem);
			continue;
		}
		result.append(" --ignore-table=").append(schema).append(".").append(data->getDataPtr()->name);
		tableItem = treeListCtrl->GetNextSibling(tableItem);
	}
	
	return result;
}

/**
 * Generate the string for objects but exclude tables.
 * 
 * @param treeListCtrl
 * @return 
 */
std::string DuplicateTableDialogDelegate::getObjectsStringFromTreeListCtrl(wxTreeListCtrl* treeListCtrl)
{
	auto rootItem = treeListCtrl->GetRootItem();
	auto tablesFolderItem = treeListCtrl->GetFirstChild(rootItem);
	if (!tablesFolderItem.IsOk()) {
		return "";
	}

	std::string result;
	auto objectItem = treeListCtrl->GetNextSibling(tablesFolderItem);
	while (objectItem.IsOk()) {
		if (!treeListCtrl->GetCheckedState(objectItem)) {
			objectItem = treeListCtrl->GetNextSibling(objectItem);
			continue;
		}
		auto data = reinterpret_cast<QClientData<UserDb> *>(treeListCtrl->GetItemData(objectItem));
		
		switch(data->getDataId()) {
		case TreeObjectType::ROUTINES:
			result.append(" --routines");
			break;
		case TreeObjectType::TRIGGERS_FOLDER:
			result.append(" --triggers");
			break;
		case TreeObjectType::EVENTS_FOLDER:
			result.append(" --events");
			break;
		default:
			break;
		}
		objectItem = treeListCtrl->GetNextSibling(objectItem);
	}
	return result;
}

void DuplicateTableDialogDelegate::loadTablesForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem& folderItem, uint64_t connectId, const std::string& schema)
{
	if (!folderItem.IsOk() || !connectId || schema.empty()) {
		return;
	}
	
	try {
		UserTableList list = metadataService->getUserTables(connectId, schema);
		for (auto& item : list) {
			// Table item
			auto data = new QClientData<UserTable>(connectId, new UserTable(item));
			auto tableItemId = treeListCtrl->AppendItem(folderItem, item.name, 1, 1, data); 			
		}
		treeListCtrl->Expand(folderItem);
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
}

void DuplicateTableDialogDelegate::loadViewsForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem& folderItem, uint64_t connectId, const std::string& schema)
{
	if (!folderItem.IsOk() || !connectId || schema.empty()) {
		return;
	}
	
	try {
		UserViewList list = metadataService->getUserViews(connectId, schema);
		for (auto& item : list) {
			QClientData<UserView>* data = new QClientData<UserView>(connectId, new UserView(item));
			treeListCtrl->AppendItem(folderItem, item.name, 2, 2, data); 
		}
		treeListCtrl->Expand(folderItem);
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
}

