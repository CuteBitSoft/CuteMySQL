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
 * @file   LeftTreeView.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-21
 *********************************************************************/
#include "LeftTreeView.h"
#include <cassert>
#include "common/Config.h"
#include "common/AppContext.h"
#include "common/MsgClientData.h"
#include "utils/ResourceUtil.h"
#include "core/common/Lang.h"
#include "core/entity/Entity.h"
#include "ui/common/data/QTreeItemData.h"
#include "ui/common/msgbox/QAnimateBox.h"
#include "ui/common/msgbox/QConfirmBox.h"
#include "ui/dialog/connect/ConnectDialog.h"
#include "ui/dialog/database/DatabaseDialog.h"

BEGIN_EVENT_TABLE(LeftTreeView, wxPanel)
	EVT_SIZE(LeftTreeView::OnSize)
	EVT_PAINT(LeftTreeView::OnPaint)
	EVT_TREE_ITEM_EXPANDED(Config::DATABASE_TREEVIEW_ID, OnTreeItemExpended)
	EVT_TREE_SEL_CHANGED(Config::DATABASE_TREEVIEW_ID, OnTreeItemSelChanged)
	EVT_BUTTON(Config::DATABASE_CONNECT_BUTTON_ID, OnClickConnectButton)
	EVT_BUTTON(Config::DATABASE_CREATE_BUTTON_ID, OnClickCreateButton)
	EVT_BUTTON(Config::DATABASE_DELETE_BUTTON_ID, OnClickDeleteButton)
	EVT_COMBOBOX(Config::TREEVIEW_SELECTED_DB_COMBOBOX_ID, OnSelectedDbCombobox)
	// Handle the message
	EVT_NOTITY_MESSAGE_HANDLE(Config::MSG_CONNECTION_CONNECTED_ID, OnHandleConnectionConnected)
	EVT_NOTITY_MESSAGE_HANDLE(Config::MSG_ADD_DATABASE_ID, OnHandleAddDatabase)
END_EVENT_TABLE()

LeftTreeView::LeftTreeView():QPanel()
{
	AppContext::getInstance()->subscribe(this, Config::MSG_CONNECTION_CONNECTED_ID);
	AppContext::getInstance()->subscribe(this, Config::MSG_ADD_DATABASE_ID);
	leftTreeDelegate = LeftTreeDelegate::getInstance(this);
	leftTopbarDelegate = LeftTopbarDelegate::getInstance(this);
}

LeftTreeView::~LeftTreeView()
{
	AppContext::getInstance()->unsubscribe(this, Config::MSG_CONNECTION_CONNECTED_ID);
	AppContext::getInstance()->unsubscribe(this, Config::MSG_ADD_DATABASE_ID);
	
	LeftTreeDelegate::destroyInstance();
	leftTreeDelegate = nullptr;	

	LeftTopbarDelegate::destroyInstance();
	leftTopbarDelegate = nullptr;
}

void LeftTreeView::init()
{
}

void LeftTreeView::createControls()
{
	QPanel::createControls();
	createButtons();
	createComboBox();
	createTreeView(); 
	topSizer->Add(treeView, 1, wxALIGN_CENTRE_HORIZONTAL | wxALIGN_TOP, 10);
}

void LeftTreeView::createButtons()
{
	wxString imgdir = ResourceUtil::getProductImagesDir();
	wxString normalPath, currentPath, pressedPath, focusPath, disabledPath;
	normalPath = imgdir + "/database/button/connect-button-normal.bmp";
	currentPath = imgdir + "/database/button/connect-button-current.bmp";
	pressedPath = imgdir + "/database/button/connect-button-pressed.bmp";
	focusPath = imgdir + "/database/button/connect-button-pressed.bmp";
	wxBitmap connectBitmap(normalPath, wxBITMAP_TYPE_BMP);
	connectButton = new wxBitmapButton(this, Config::DATABASE_CONNECT_BUTTON_ID, connectBitmap, { 1, 10 }, { 20, 20 }, wxCLIP_CHILDREN);
	connectButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	connectButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	connectButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	connectButton->SetToolTip(S("create-connection"));

	normalPath = imgdir + "/database/button/new-button-normal.bmp";
	currentPath = imgdir + "/database/button/new-button-current.bmp";
	pressedPath = imgdir + "/database/button/new-button-pressed.bmp";
	focusPath = imgdir + "/database/button/new-button-pressed.bmp";
	disabledPath = imgdir + "/database/button/new-button-disabled.bmp";
	wxBitmap newBitmap(normalPath, wxBITMAP_TYPE_BMP);
	newButton = new wxBitmapButton(this, Config::DATABASE_CREATE_BUTTON_ID, newBitmap, {1 + 20 + 5, 10}, { 20, 20 }, wxCLIP_CHILDREN);
	newButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	newButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	newButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	newButton->SetBitmapDisabled(wxBitmap(disabledPath, wxBITMAP_TYPE_BMP));
	newButton->SetToolTip(S("create-database"));
	
	normalPath = imgdir + "/database/button/del-button-normal.bmp";
	currentPath = imgdir + "/database/button/del-button-current.bmp";
	pressedPath = imgdir + "/database/button/del-button-pressed.bmp";
	focusPath = imgdir + "/database/button/del-button-pressed.bmp";
	disabledPath = imgdir + "/database/button/del-button-disabled.bmp";
	wxBitmap delBitmap(normalPath, wxBITMAP_TYPE_BMP);
	delButton = new wxBitmapButton(this, Config::DATABASE_DELETE_BUTTON_ID, delBitmap, {1 + (20 + 5) * 2, 10}, { 20, 20 }, wxCLIP_CHILDREN);
	delButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	delButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	delButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	delButton->SetBitmapDisabled(wxBitmap(disabledPath, wxBITMAP_TYPE_BMP));
	delButton->SetToolTip(S("delete-selected-item"));

	normalPath = imgdir + "/database/button/dup-button-normal.bmp";
	currentPath = imgdir + "/database/button/dup-button-current.bmp";
	pressedPath = imgdir + "/database/button/dup-button-pressed.bmp";
	focusPath = imgdir + "/database/button/dup-button-pressed.bmp";
	disabledPath = imgdir + "/database/button/dup-button-disabled.bmp";
	wxBitmap dupBitmap(normalPath, wxBITMAP_TYPE_BMP);
	dupButton = new wxBitmapButton(this, Config::DATABASE_DUP_BUTTON_ID, dupBitmap, {1 + (20 + 5) * 3, 10}, { 20, 20 }, wxCLIP_CHILDREN);
	dupButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	dupButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	dupButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	dupButton->SetBitmapDisabled(wxBitmap(disabledPath, wxBITMAP_TYPE_BMP));
	dupButton->SetToolTip(S("duplicate-selected-item"));
	
	normalPath = imgdir + "/database/button/refresh-button-normal.bmp";
	currentPath = imgdir + "/database/button/refresh-button-current.bmp";
	pressedPath = imgdir + "/database/button/refresh-button-pressed.bmp";
	focusPath = imgdir + "/database/button/refresh-button-pressed.bmp";
	wxBitmap refreshBitmap(normalPath, wxBITMAP_TYPE_BMP);
	refreshButton = new wxBitmapButton(this, Config::DATABASE_REFRESH_BUTTON_ID, refreshBitmap, {1 + (20 + 5) * 4, 10}, { 20, 20 }, wxCLIP_CHILDREN);
	refreshButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	refreshButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	refreshButton->SetBitmapDisabled(wxBitmap(disabledPath, wxBITMAP_TYPE_BMP));
	refreshButton->SetToolTip(S("refresh"));
	
}

void LeftTreeView::createComboBox()
{
	auto size = GetClientSize();
	selectedDbComboBox = new wxBitmapComboBox(this, Config::TREEVIEW_SELECTED_DB_COMBOBOX_ID, wxEmptyString, {size.GetWidth() - 5 - 120, 8}, {120, 24}, wxArrayString(), wxCB_READONLY | wxNO_BORDER);
	selectedDbComboBox->SetBackgroundColour(bkgColor);
	selectedDbComboBox->SetForegroundColour(textColor);
}

void LeftTreeView::createTreeView()
{
	treeView = new wxTreeCtrl(this, Config::DATABASE_TREEVIEW_ID, { 1, 40 }, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_NO_LINES | wxTR_SINGLE | wxNO_BORDER | wxCLIP_CHILDREN);
	treeView->SetBackgroundColour(bkgColor);
	treeView->SetForegroundColour(textColor);

	loadTreeView(treeView);
}

void LeftTreeView::resizeComboBox()
{
	if (!selectedDbComboBox) {
		return;
	}
	auto size = GetClientSize();
	int maxWidth = 220; 
	int calWidth = size.GetWidth() - 5 - (1 + (20 + 5) * 5); // 
	int w = calWidth > maxWidth ? maxWidth : calWidth;
	selectedDbComboBox->SetSize(w, 24);
	selectedDbComboBox->SetPosition({ size.GetWidth() - 5 - w, 8 });
}

void LeftTreeView::resizeTreeView()
{
	if (!treeView) {
		return;
	}

	wxRect rect = GetClientRect();
	treeView->SetPosition({1, 40 });
	treeView->SetSize({ rect.GetWidth() - 2, rect.GetHeight() - 40});
}

void LeftTreeView::loadTreeView(wxTreeCtrl * treeView)
{
	leftTreeDelegate->loadForLeftTree(treeView);
}

void LeftTreeView::OnSize(wxSizeEvent& event)
{
	resizeComboBox();
	resizeTreeView();	
}


void LeftTreeView::OnPaint(wxPaintEvent& event)
{
	wxSize size = GetClientSize();
	wxPaintDC pdc(this);
	wxColor lineColor(30, 31, 34, 30);
	wxPen oldPen = pdc.GetPen();
	wxPen pen(lineColor, 1);
	pdc.SetPen(pen);
	pdc.DrawLine({ 0,0 }, { 0, size.GetHeight() });

	pdc.SetPen(oldPen);
}


void LeftTreeView::OnTreeItemExpended(wxTreeEvent& event)
{
	auto itemId = event.GetItem();
	leftTreeDelegate->expendedForLeftTree(treeView, itemId);

	if (itemId.IsOk() && itemId != treeView->GetSelection() && itemId != treeView->GetRootItem()) {
		int nImage = treeView->GetItemImage(itemId);
		if (nImage == 1) { // 1 - connection
			auto connData = reinterpret_cast<QTreeItemData<UserConnect> *>(treeView->GetItemData(itemId));
			auto selDbData = leftTreeDelegate->getSelectedDbItemData(treeView);
			if (selDbData && connData && selDbData->connectId != connData->getDataId()) {
				treeView->SelectItem(itemId);
			} else if (!selDbData || !connData) {
				treeView->SelectItem(itemId);
			}
		} else {
			treeView->SelectItem(itemId);
		}
	}
}

void LeftTreeView::OnTreeItemSelChanged(wxTreeEvent& event)
{
	auto selItemId = event.GetItem();
	auto rootId = treeView->GetRootItem();
	if (selItemId == rootId) {
		newButton->Disable();
		delButton->Disable();
		dupButton->Disable();
		return;
	} else {
		newButton->Enable();
		delButton->Enable();
		dupButton->Enable();
	}

	auto data = (QTreeItemData<int> *)treeView->GetItemData(selItemId);
	auto connectId = data->getDataId();
	if (!connectId) {
		return;
	}
	auto selConnectData = leftTreeDelegate->getSelectedConnectItemData(treeView);
	auto selDbData = leftTreeDelegate->getSelectedDbItemData(treeView);
	
	if (supplier->runtimeUserConnect && selConnectData == supplier->runtimeUserConnect) {
		if (selDbData == supplier->runtimeUserDb) {
			return;
		} else {
			supplier->runtimeUserDb = selDbData;
			bool ret = leftTopbarDelegate->selectDbsForComboBox(selectedDbComboBox);
			if (!ret) {
				leftTopbarDelegate->loadDbsForComboBox(selectedDbComboBox);
			}
			return;
		}
	} else if (selDbData != supplier->runtimeUserDb) {
		supplier->runtimeUserDb = selDbData;
	}
	supplier->runtimeUserConnect = selConnectData;
	leftTopbarDelegate->loadDbsForComboBox(selectedDbComboBox);
}

void LeftTreeView::OnHandleConnectionConnected(MsgDispatcherEvent& event)
{
	auto clientData = (MsgClientData *)event.GetClientData();
	auto connectId = clientData->getDataPtr();

	leftTreeDelegate->loadForLeftTree(treeView, connectId);
}

void LeftTreeView::OnHandleAddDatabase(MsgDispatcherEvent& event)
{
	auto clientData = (MsgClientData*)event.GetClientData();
	auto connectId = clientData->getDataPtr();

	leftTreeDelegate->loadForLeftTree(treeView, connectId, supplier->handleUserDb.name);
}

void LeftTreeView::OnClickConnectButton(wxCommandEvent& event)
{
	ConnectDialog connectDialog(ConnectType::CONNECT_CREATE);
	connectDialog.Create(AppContext::getInstance()->getMainFrmWindow(), -1, S("create-connection"));
	connectDialog.ShowModal();
}

void LeftTreeView::OnClickCreateButton(wxCommandEvent& event)
{
	if (!supplier->runtimeUserConnect) {
		return;
	}
	DatabaseDialog databaseDialog(DatabaseType::DATABASE_CREATE);
	databaseDialog.Create(AppContext::getInstance()->getMainFrmWindow(), -1, S("create-database-caption"));
	databaseDialog.ShowModal();
}

void LeftTreeView::OnClickDeleteButton(wxCommandEvent& event)
{
	if (!supplier->runtimeUserConnect) {
		return;
	}

	auto selItemId = treeView->GetSelection();
	if (!selItemId.IsOk() || selItemId == treeView->GetRootItem()) {
		QAnimateBox::notice(S("not-choose-treeitem"));
		return;
	}
	
	if (leftTreeDelegate->removeForLeftTree(treeView)) {
		leftTopbarDelegate->loadDbsForComboBox(selectedDbComboBox);
	}	
}
/**
 * When selectedDbComboBox changing its selected item, changing the selected item of treeView to match the database in the same time.
 * Note: This event will be triggered the EVT_TREE_SEL_CHANGED event that calling LeftTreeView::OnTreeItemSelChanged
 * 
 * @param event
 */
void LeftTreeView::OnSelectedDbCombobox(wxCommandEvent& event)
{
	assert(supplier->runtimeUserConnect != nullptr);
	int nSelItem = selectedDbComboBox->GetSelection();
	auto data = reinterpret_cast<QClientData<UserDb> *>(selectedDbComboBox->GetClientObject(nSelItem));
	if (supplier->runtimeUserDb && data
		&& supplier->runtimeUserDb->connectId == data->getDataPtr()->connectId
		&& supplier->runtimeUserDb->name == data->getDataPtr()->name) {
		return;
	}
	// Find the connection of treeView using supplier->runtimeUserConnect
	auto treeRootId = treeView->GetRootItem();
	if (!treeRootId.IsOk() || !treeView->HasChildren(treeRootId)) {
		return;
	}
	wxTreeItemIdValue cookie;
	auto connectItemId = treeView->GetFirstChild(treeRootId, cookie);
	while (connectItemId.IsOk()) {
		auto connectItemData = reinterpret_cast<QTreeItemData<UserConnect> *>(treeView->GetItemData(connectItemId));
		wxString connectItemStr = treeView->GetItemText(connectItemId);
		// matching connection
		if (connectItemData && supplier->runtimeUserConnect->id == connectItemData->getDataPtr()->id) {
			break;
		}
		connectItemId = treeView->GetNextSibling(connectItemId);
	}
	
	if (!connectItemId.IsOk()) {
		return;
	}

	if (!treeView->IsExpanded(connectItemId)) {
		leftTreeDelegate->expendedConnectionItem(treeView, connectItemId, supplier->runtimeUserConnect->id);
		treeView->Expand(connectItemId);
	}

	// Find the database item of treeView using data = selectedDbComboBox->getClientObject(nSelItem)
	wxTreeItemIdValue cookie2;
	auto dbItemId = treeView->GetFirstChild(connectItemId, cookie2);
	while (dbItemId.IsOk()) {
		auto dbItemData = reinterpret_cast<QTreeItemData<UserDb> *>(treeView->GetItemData(dbItemId));
		// matching database
		if (dbItemData->getDataPtr()->connectId == data->getDataPtr()->connectId 
			&& dbItemData->getDataPtr()->name == data->getDataPtr()->name) {
			treeView->SelectItem(dbItemId);
			return;
		}

		dbItemId = treeView->GetNextSibling(dbItemId); 
	}
}
