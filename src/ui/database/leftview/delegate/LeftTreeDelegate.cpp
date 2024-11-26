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
			loadingForConnection(treeView, itemId, connectId);
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
	}
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

void LeftTreeDelegate::loadingForConnection(wxTreeCtrl* treeView, const wxTreeItemId& connectItemId, uint64_t connectId /*= 0*/)
{
	if (!connectItemId.IsOk() || !connectId) {
		return;
	}

	wxTreeItemIdValue cookie;
	wxTreeItemId firstChildItemId = treeView->GetFirstChild(connectItemId, cookie);
	if (firstChildItemId.IsOk()) {
		return;
	}

	QTreeItemData<int>* data = new QTreeItemData<int>(0, 0, TreeObjectType::LOADING);
	treeView->AppendItem(connectItemId, S("loading"), 10, 10, data);
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

			QTreeItemData<int>* tblsFolderData = new QTreeItemData<int>(connectId, 0, TreeObjectType::TABLES_FOLDER);
			auto tblsFolderItemId = treeView->AppendItem(dbItemId, S("tables"), 3, 3, tblsFolderData);

			QTreeItemData<int>* viewsFolderData = new QTreeItemData<int>(connectId, 0, TreeObjectType::VIEWS_FOLDER);
			auto viewsFolderItemId = treeView->AppendItem(dbItemId, S("views"), 3, 3, viewsFolderData);

			QTreeItemData<int>* storeProcsFolderData = new QTreeItemData<int>(connectId, 0, TreeObjectType::STORE_PROCEDURE_FOLDER);
			auto storeProcsFolderItemId = treeView->AppendItem(dbItemId, S("store-processes"), 3, 3, storeProcsFolderData);

			QTreeItemData<int>* funsFolderData = new QTreeItemData<int>(connectId, 0, TreeObjectType::FUNCTIONS_FOLDER);
			auto funsFolderItemId = treeView->AppendItem(dbItemId, S("functions"), 3, 3, funsFolderData);

			QTreeItemData<int>* triggersFolderData = new QTreeItemData<int>(connectId, 0, TreeObjectType::TRIGGERS_FOLDER);
			auto triggersFolderItemId = treeView->AppendItem(dbItemId, S("triggers"), 3, 3, triggersFolderData);

			QTreeItemData<int>* eventsFolderData = new QTreeItemData<int>(connectId, 0, TreeObjectType::EVENTS_FOLDER);
			auto eventsFolderItemId = treeView->AppendItem(dbItemId, S("events"), 3, 3, eventsFolderData);

			// load tables
			loadTablesForDatabase(treeView, tblsFolderItemId, connectId, item.name);
			// load views
			loadViewsForDatabase(treeView, viewsFolderItemId, connectId, item.name);
			// load procedures
			loadProceduresForDatabase(treeView, storeProcsFolderItemId, connectId, item.name);
			// load functions
			loadFunctionsForDatabase(treeView, funsFolderItemId, connectId, item.name);
			// load triggers
			loadTriggersForDatabase(treeView, triggersFolderItemId, connectId, item.name);
			// load events
			loadEventsForDatabase(treeView, eventsFolderItemId, connectId, item.name);
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
			QTreeItemData<UserTable>* data = new QTreeItemData<UserTable>(connectId, new UserTable(item), TreeObjectType::TABLE);
			treeView->AppendItem(folderItemId, item.name, 4, 4, data); 
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
