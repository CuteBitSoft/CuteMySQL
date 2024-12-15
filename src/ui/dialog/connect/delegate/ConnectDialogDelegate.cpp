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
 * @file   ConnectDialogDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-13
 *********************************************************************/
#include "ConnectDialogDelegate.h"
#include <stdlib.h>
#include <malloc.h>
#include <algorithm>
#include "utils/ResourceUtil.h"
#include "core/common/Lang.h"
#include "core/entity/Entity.h"
#include "ui/common/data/QTreeItemData.h"

void ConnectDialogDelegate::loadTreeCtrl(wxTreeCtrl* leftTree)
{
	auto imgdir = ResourceUtil::getProductImagesDir();

	wxImageList* images = new wxImageList(16, 16);
	
	wxString imgpath1 = imgdir + _("/home/dialog/folder.bmp");
	wxString imgpath2 = imgdir + _("/home/dialog/connection.bmp");
	images->Add(wxBitmap(imgpath1, wxBITMAP_TYPE_BMP)); // 0 - folder	
	images->Add(wxBitmap(imgpath2, wxBITMAP_TYPE_BMP)); // 1 - connection
	leftTree->AssignImageList(images);

	auto rootId = leftTree->AddRoot(S("connect-section-text"), 0, 0);
	auto userConnectList = connectService->getAllUserConnects();

	wxTreeItemId selItemId;
	for (auto& item : userConnectList) {
		QTreeItemData<UserConnect>* data = new QTreeItemData<UserConnect>(item.id, new UserConnect(item));
		auto itemId = leftTree->AppendItem(rootId, item.name, 1, 1, data);
		if (item.id == supplier->selConnectId) {
			selItemId = itemId;
		}
	}
	
	leftTree->Expand(rootId);

	if (selItemId.IsOk()) {
		leftTree->SelectItem(selItemId);
	} else {
		wxTreeItemIdValue cookie;
		auto firstItemId = leftTree->GetFirstChild(rootId, cookie);
		if (firstItemId.IsOk()) {
			leftTree->SelectItem(firstItemId);
		}
	}
	
}

void ConnectDialogDelegate::createForTreeCtrl(wxTreeCtrl* leftTree, const std::string & defaultName)
{
	auto rootId = leftTree->GetRootItem();

	UserConnect* ptr = new UserConnect();
	ptr->name = defaultName.empty() ? S("new-connect-name") : defaultName;
	ptr->sort = getMaxSortFromTreeCtrl(leftTree) + 1;
	QTreeItemData<UserConnect>* data = new QTreeItemData<UserConnect>(0, ptr);
	
	auto newId = leftTree->AppendItem(rootId, ptr->name, 1, 1, data);
	leftTree->SelectItem(newId, true);
	supplier->runtimeUserConnect = ptr;
}

void ConnectDialogDelegate::removeForTreeCtrl(wxTreeCtrl* leftTree)
{
	auto rootId = leftTree->GetRootItem();
	auto selItemId = leftTree->GetSelection();
	if (!selItemId.IsOk() || rootId == selItemId) {
		return;
	}

	int answer = wxMessageBox(S("remove-connection-confirm"), S("confirm"),
					wxYES_NO | wxCANCEL, view);
	if (answer != wxYES) {
		return;
	}

	if (supplier->runtimeUserConnect->id) {
		try {
			connectService->removeUserConnect(supplier->runtimeUserConnect->id);
		} catch (QRuntimeException& ex) {
			wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
			msgbox.ShowModal();
			return;
		}		
	}
	
	leftTree->Delete(selItemId);
}

void ConnectDialogDelegate::duplicateForTreeCtrl(wxTreeCtrl* leftTree)
{
	auto rootId = leftTree->GetRootItem();
	auto selItemId = leftTree->GetSelection();

	if (!selItemId.IsOk() || rootId == selItemId) {
		return;
	}

	auto selItemData = (QTreeItemData<UserConnect>*)leftTree->GetItemData(selItemId);

	UserConnect* ptr = new UserConnect(*selItemData->getDataPtr());
	ptr->id = 0;
	ptr->sort = getMaxSortFromTreeCtrl(leftTree) + 1;
	QTreeItemData<UserConnect>* data = new QTreeItemData<UserConnect>(0, ptr);
	leftTree->AppendItem(rootId, ptr->name, 1, 1, data);
}

void ConnectDialogDelegate::moveUpForTreeCtrl(wxTreeCtrl* leftTree)
{
	auto rootId = leftTree->GetRootItem();
	auto selItemId = leftTree->GetSelection();
	auto prevItemId = leftTree->GetPrevSibling(selItemId);

	if (!selItemId.IsOk() || !prevItemId.IsOk() || rootId == selItemId) {
		return;
	}
	swapTreeCtrlItem(leftTree, selItemId, prevItemId);

	// save the changed data to user_connect table of SQLite
	saveTreeCtrlItemData(leftTree, selItemId);
	saveTreeCtrlItemData(leftTree, prevItemId);
}

void ConnectDialogDelegate::moveDownForTreeCtrl(wxTreeCtrl* leftTree)
{
	auto rootId = leftTree->GetRootItem();
	auto selItemId = leftTree->GetSelection();
	auto nextItemId = leftTree->GetNextSibling(selItemId);

	if (!selItemId.IsOk() || !nextItemId.IsOk() || rootId == selItemId) {
		return;
	}
	swapTreeCtrlItem(leftTree, selItemId, nextItemId);

	// save the changed data to user_connect table of SQLite
	saveTreeCtrlItemData(leftTree, selItemId);
	saveTreeCtrlItemData(leftTree, nextItemId);
}


void ConnectDialogDelegate::swapTreeCtrlItem(wxTreeCtrl* leftTree, wxTreeItemId& selItemId, wxTreeItemId& swapItemId)
{
	if (!swapItemId.IsOk()) {
		return;
	}

	auto selItemData = (QTreeItemData<UserConnect>*)leftTree->GetItemData(selItemId);
	UserConnect* selPtr = selItemData->getDataPtr();

	auto swapItemData = (QTreeItemData<UserConnect>*)leftTree->GetItemData(swapItemId);
	UserConnect* swapPtr = swapItemData->getDataPtr();

	std::swap<uint64_t>(selPtr->sort, swapPtr->sort);

	leftTree->SetItemText(selItemId, swapPtr->name);
	leftTree->SetItemText(swapItemId, selPtr->name);
	
	QTreeItemData<UserConnect>* tmpItemData = selItemData;

	leftTree->SetItemData(selItemId, swapItemData);
	leftTree->SetItemData(swapItemId, tmpItemData);

	leftTree->SelectItem(swapItemId);
}

/**
 * Get max sort from leftTree->GetItemData(swapItemId)->getDataPtr()->sort.
 * 
 * @param leftTree
 * @return 
 */
uint64_t ConnectDialogDelegate::getMaxSortFromTreeCtrl(wxTreeCtrl* leftTree)
{
	uint64_t maxSort = 0, sort = 0;
	auto rootId = leftTree->GetRootItem();
	wxTreeItemIdValue cookie;
	auto childItemId = leftTree->GetFirstChild(rootId, cookie);
	while (childItemId.IsOk()) {
		auto itemData = (QTreeItemData<UserConnect>*)leftTree->GetItemData(childItemId);
		UserConnect* selPtr = itemData->getDataPtr();
		if (selPtr == nullptr) {
			break;
		}
		sort = selPtr->sort;
		maxSort = std::max<uint64_t>(maxSort, sort);
		childItemId = leftTree->GetNextSibling(childItemId);
	}

	return maxSort;
}

void ConnectDialogDelegate::saveTreeCtrlItemData(wxTreeCtrl* leftTree, wxTreeItemId& itemId)
{
	auto itemData = (QTreeItemData<UserConnect>*)leftTree->GetItemData(itemId);
	UserConnect* dataPtr = itemData->getDataPtr();
	if (dataPtr == nullptr) {
		return;
	}

	if (!dataPtr->id) {
		dataPtr->id = connectService->createUserConnect(* dataPtr);
	} else {
		connectService->updateUserConnect(* dataPtr);
	}
	
}


/**
 * exchange the data from rightPanel to supplier->runtimeUserConnect.
 * Notice: supplier->runtimeUserConnect is the data pointer of old select item
 * 
 * @param rightPanel
 */
void ConnectDialogDelegate::exchangeDataFromRightPanel(ConnectPanel* rightPanel)
{
	if (supplier->runtimeUserConnect == nullptr) {
		return;
	}
	if (rightPanel == nullptr) {
		return;
	}

	rightPanel->exchangeData();
}

void ConnectDialogDelegate::changeSelItemForTreeCtrl(wxTreeCtrl* leftTree, wxTreeItemId selItemId)
{
	auto data = (QTreeItemData<UserConnect>*)leftTree->GetItemData(selItemId);
	if (data == nullptr) {
		supplier->runtimeUserConnect = nullptr;
		return;
	}
	supplier->runtimeUserConnect = data->getDataPtr();
}

void ConnectDialogDelegate::changeDataForRightPanel(ConnectPanel* rightPanel)
{
	if (rightPanel) {
		rightPanel->loadControls();
	}	
}

void ConnectDialogDelegate::clearDataForRightPanel(ConnectPanel* rightPanel)
{
	if (rightPanel) {
		rightPanel->clearControls();
	}
}

void ConnectDialogDelegate::disableForRightPanel(ConnectPanel* rightPanel)
{
	if (rightPanel) {
		rightPanel->disableControls();
	}
}

void ConnectDialogDelegate::enableForRightPanel(ConnectPanel* rightPanel)
{
	if (rightPanel) {
		rightPanel->enableControls();
	}
}

void ConnectDialogDelegate::testConnect()
{
	auto connectId = saveUserConnect();

	try {
		connectService->testConnect(connectId);
		wxMessageDialog msgbox(view, S("connect-success"), S("notic"), wxOK|wxCENTRE|wxICON_INFORMATION);
		msgbox.ShowModal();
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
		msgbox.ShowModal();
	}
	
}

uint64_t ConnectDialogDelegate::saveUserConnect()
{
	if (!supplier->runtimeUserConnect) {
		return 0;
	}
	uint64_t connectId = supplier->runtimeUserConnect->id;
	try {
		if (supplier->getIsDirty() && !connectId) {
			connectId = connectService->createUserConnect(*supplier->runtimeUserConnect);
			supplier->runtimeUserConnect->id = connectId;
		} else if (supplier->getIsDirty()) {
			connectService->updateUserConnect(*supplier->runtimeUserConnect);
		}

		supplier->setIsDirty(false);
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
		msgbox.ShowModal();
	}
	return connectId;
}

bool ConnectDialogDelegate::connect()
{
	auto connectId = saveUserConnect();

	try {
		connectService->connect(connectId);
		return true;
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK|wxCENTRE|wxICON_ERROR);
		msgbox.ShowModal();
	}
	return false;
}
