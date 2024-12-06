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
 * @file   DuplicateDatabaseDialogDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-30
 *********************************************************************/
#include "DuplicateDatabaseDialogDelegate.h"
#include "ui/common/data/QClientData.h"

void DuplicateDatabaseDialogDelegate::loadForTreeListCtrl(wxTreeListCtrl* treeListCtrl)
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
	
	auto tblsFolderData = new QClientData<UserDb>(connectId, new UserDb(item));
	auto tblsFolderItem = treeListCtrl->AppendItem(rootItem, S("tables"), 0, 0, tblsFolderData);

	auto viewsFolderData = new QClientData<UserDb>(connectId, new UserDb(item));
	auto viewsFolderItem = treeListCtrl->AppendItem(rootItem, S("views"), 0, 0, viewsFolderData);

	auto storeProcsFolderData = new QClientData<UserDb>(connectId, new UserDb(item));
	auto storeProcsFolderItem = treeListCtrl->AppendItem(rootItem, S("store-processes"), 0, 0, storeProcsFolderData);

	auto funsFolderData = new QClientData<UserDb>(connectId, new UserDb(item));
	auto funsFolderItem = treeListCtrl->AppendItem(rootItem, S("functions"), 0, 0, funsFolderData);

	auto triggersFolderData = new QClientData<UserDb>(connectId, new UserDb(item));
	auto triggersFolderItem = treeListCtrl->AppendItem(rootItem, S("triggers"), 0, 0, triggersFolderData);

	auto eventsFolderData = new QClientData<UserDb>(connectId, new UserDb(item));
	auto eventsFolderItem = treeListCtrl->AppendItem(rootItem, S("events"), 0, 0, eventsFolderData);

	loadTablesForDatabase(treeListCtrl, tblsFolderItem, connectId, schema);
	loadViewsForDatabase(treeListCtrl, viewsFolderItem, connectId, schema);
	loadProceduresForDatabase(treeListCtrl, storeProcsFolderItem, connectId, schema);
	loadFunctionsForDatabase(treeListCtrl, funsFolderItem, connectId, schema);
	loadTriggersForDatabase(treeListCtrl, triggersFolderItem, connectId, schema);
	loadEventsForDatabase(treeListCtrl, eventsFolderItem, connectId, schema);

	treeListCtrl->Expand(rootItem);
	treeListCtrl->CheckItem(rootItem, wxCHK_CHECKED);
	treeListCtrl->CheckItemRecursively(rootItem, wxCHK_CHECKED);
}



void DuplicateDatabaseDialogDelegate::loadTablesForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem& folderItem, uint64_t connectId, const std::string& schema)
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

void DuplicateDatabaseDialogDelegate::loadViewsForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem& folderItem, uint64_t connectId, const std::string& schema)
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

void DuplicateDatabaseDialogDelegate::loadProceduresForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem& folderItem, uint64_t connectId, const std::string& schema)
{
	if (!folderItem.IsOk() || !connectId || schema.empty()) {
		return;
	}
	
	try {
		UserProcedureList list = metadataService->getUserProcedures(connectId, schema);
		for (auto& item : list) {
			QClientData<UserProcedure>* data = new QClientData<UserProcedure>(connectId, new UserProcedure(item));
			treeListCtrl->AppendItem(folderItem, item.name, 3, 3, data); 
		}
		treeListCtrl->Expand(folderItem);
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
}


void DuplicateDatabaseDialogDelegate::loadFunctionsForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem& folderItem, uint64_t connectId, const std::string& schema)
{
	if (!folderItem.IsOk() || !connectId || schema.empty()) {
		return;
	}
	
	try {
		UserFunctionList list = metadataService->getUserFunctions(connectId, schema);
		for (auto& item : list) {
			QClientData<UserFunction>* data = new QClientData<UserFunction>(connectId, new UserFunction(item));
			treeListCtrl->AppendItem(folderItem, item.name, 4, 4, data); 
		}
		treeListCtrl->Expand(folderItem);
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
}

void DuplicateDatabaseDialogDelegate::loadTriggersForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem& folderItem, uint64_t connectId, const std::string& schema)
{
	if (!folderItem.IsOk() || !connectId || schema.empty()) {
		return;
	}
	
	try {
		UserTriggerList list = metadataService->getUserTriggers(connectId, schema);
		for (auto& item : list) {
			QClientData<UserTrigger>* data = new QClientData<UserTrigger>(connectId, new UserTrigger(item));
			treeListCtrl->AppendItem(folderItem, item.name, 5, 5, data); 
		}
		treeListCtrl->Expand(folderItem);
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
}

void DuplicateDatabaseDialogDelegate::loadEventsForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem& folderItem, uint64_t connectId, const std::string& schema)
{
	if (!folderItem.IsOk() || !connectId || schema.empty()) {
		return;
	}

	try {
		UserEventList list = metadataService->getUserEvents(connectId, schema);
		for (auto& item : list) {
			QClientData<UserEvent>* data = new QClientData<UserEvent>(connectId, new UserEvent(item));
			treeListCtrl->AppendItem(folderItem, item.name, 6, 6, data); 
		}
		treeListCtrl->Expand(folderItem);
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
}
