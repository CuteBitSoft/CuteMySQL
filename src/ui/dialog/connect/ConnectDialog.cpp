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
 * @file   ConnectDialog.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-07
 *********************************************************************/

#include "ConnectDialog.h"
#include <wx/imaglist.h>
#include "utils/ResourceUtil.h"
#include "core/common/Lang.h"
#include "common/AppContext.h"
#include "common/MsgClientData.h"


BEGIN_EVENT_TABLE(ConnectDialog, wxDialog)
	EVT_TREE_SEL_CHANGED(Config::CONNECT_DIALOG_LEFT_TREE_ID, OnLeftTreeSelChange)

	EVT_BUTTON(Config::CONNECT_TEST_BUTTON_ID, OnTestButtonClick)
	EVT_BUTTON(Config::CONNECT_CONFIRM_BUTTON_ID, OnConnectButtonClick)
	EVT_BUTTON(Config::CONNECT_CANCEL_BUTTON_ID, OnCancelButtonClick)

	EVT_BUTTON(Config::CONNECT_NEW_BUTTON_ID, OnNewButtonClick)
	EVT_BUTTON(Config::CONNECT_DEL_BUTTON_ID, OnDelButtonClick)
	EVT_BUTTON(Config::CONNECT_DUP_BUTTON_ID, OnDupButtonClick)
	EVT_BUTTON(Config::CONNECT_UP_BUTTON_ID, OnUpButtonClick)
	EVT_BUTTON(Config::CONNECT_DOWN_BUTTON_ID, OnDownButtonClick)

	// Handle the message
	EVT_NOTITY_MESSAGE_HANDLE(Config::MSG_CONNECTION_CREATE_ID, OnHandleConnectionCreate)
	EVT_NOTITY_MESSAGE_HANDLE(Config::MSG_CONNECTION_CHANGE_NAME_ID, OnHandleConnectionChangeName)
END_EVENT_TABLE()

ConnectDialog::ConnectDialog(ConnectType type, uint64_t selConnectId) 
	: QDialog(), connectType(type)
{
	// Subscribe to Config::MSG_CONNECTION_CHANGE_NAME_ID message
	AppContext::getInstance()->subscribe(this, Config::MSG_CONNECTION_CREATE_ID);
	AppContext::getInstance()->subscribe(this, Config::MSG_CONNECTION_CHANGE_NAME_ID);

	supplier->selConnectId = selConnectId;
}

ConnectDialog::~ConnectDialog()
{
	// Unsubscribe to Config::MSG_CONNECTION_CHANGE_NAME_ID message
	AppContext::getInstance()->unsubscribe(this, Config::MSG_CONNECTION_CREATE_ID);
	AppContext::getInstance()->unsubscribe(this, Config::MSG_CONNECTION_CHANGE_NAME_ID);
}
void ConnectDialog::init()
{
	QDialog::init();
}

void ConnectDialog::createControls()
{
	QDialog::createControls();
	SetSize(800, 600);

	vLayout = new wxBoxSizer(wxVERTICAL); 
	hLayout = new wxBoxSizer(wxHORIZONTAL);
	bLayout = new wxBoxSizer(wxHORIZONTAL);
	vLayout->Add(hLayout, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	vLayout->AddSpacer(5);
	vLayout->Add(bLayout, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 10);

	createLeftTree();	
	hLayout->Add(leftTree, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	createRightPanel();
	hLayout->Add(rightPanel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	createBottomBottons();	
	this->SetSizer(vLayout);
}

void ConnectDialog::loadControls()
{
	// load connections for left tree
	delegate->loadTreeCtrl(leftTree);

	// create new connection
	if (connectType == ConnectType::CONNECT_CREATE || supplier->runtimeUserConnect == nullptr) {
		delegate->createForTreeCtrl(leftTree);
	}
}

void ConnectDialog::createLeftTree()
{
	leftTree = new wxTreeCtrl(this, Config::CONNECT_DIALOG_LEFT_TREE_ID, wxDefaultPosition, wxSize(200, 400), wxTR_HAS_BUTTONS | wxTR_SINGLE);
	leftTree->SetBackgroundColour(bkgColor);
	leftTree->SetForegroundColour(textColor);

	
	
}

void ConnectDialog::createRightPanel()
{
	rightPanel = new ConnectPanel();
	rightPanel->Create(this, -1, wxDefaultPosition, wxSize(500, 400), 0);
}

void ConnectDialog::createBottomBottons()
{
	createBottomLeftBottons();
	createBottomRightBottons();
}

void ConnectDialog::createBottomLeftBottons()
{
	wxBoxSizer* layout = new wxBoxSizer(wxHORIZONTAL);
	bLayout->Add(layout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	wxString imgdir = ResourceUtil::getProductImagesDir();
	wxString normalPath = imgdir + "/home/dialog/new-connect-button-normal.bmp";
	wxString currentPath = imgdir + "/home/dialog/new-connect-button-current.bmp";
	wxString pressedPath = imgdir + "/home/dialog/new-connect-button-pressed.bmp";
	wxString focusPath = imgdir + "/home/dialog/new-connect-button-pressed.bmp";
	wxBitmap newBitmap(normalPath, wxBITMAP_TYPE_BMP);
	newButton = new wxBitmapButton(this, Config::CONNECT_NEW_BUTTON_ID, newBitmap, wxDefaultPosition, {20, 20}, wxCLIP_CHILDREN);
	newButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	newButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	newButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	layout->Add(newButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10); 

	layout->AddSpacer(10);

	normalPath = imgdir + "/home/dialog/del-connect-button-normal.bmp";
	currentPath = imgdir + "/home/dialog/del-connect-button-current.bmp";
	pressedPath = imgdir + "/home/dialog/del-connect-button-pressed.bmp";
	focusPath = imgdir + "/home/dialog/del-connect-button-pressed.bmp";
	wxBitmap delBitmap(normalPath, wxBITMAP_TYPE_BMP);
	delButton = new wxBitmapButton(this, Config::CONNECT_DEL_BUTTON_ID, delBitmap, wxDefaultPosition, { 20, 20 }, wxCLIP_CHILDREN);
	delButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	delButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	delButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	layout->Add(delButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10); 

	layout->AddSpacer(10);

	normalPath = imgdir + "/home/dialog/dup-connect-button-normal.bmp";
	currentPath = imgdir + "/home/dialog/dup-connect-button-current.bmp";
	pressedPath = imgdir + "/home/dialog/dup-connect-button-pressed.bmp";
	focusPath = imgdir + "/home/dialog/dup-connect-button-pressed.bmp";
	wxBitmap dupBitmap(normalPath, wxBITMAP_TYPE_BMP);
	dupButton = new wxBitmapButton(this, Config::CONNECT_DUP_BUTTON_ID, dupBitmap, wxDefaultPosition, { 20, 20 }, wxCLIP_CHILDREN);
	dupButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	dupButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	dupButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	layout->Add(dupButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);

	layout->AddSpacer(10);

	normalPath = imgdir + "/home/dialog/up-connect-button-normal.bmp";
	currentPath = imgdir + "/home/dialog/up-connect-button-current.bmp";
	pressedPath = imgdir + "/home/dialog/up-connect-button-pressed.bmp";
	focusPath = imgdir + "/home/dialog/up-connect-button-pressed.bmp";
	wxBitmap upBitmap(normalPath, wxBITMAP_TYPE_BMP);
	upButton = new wxBitmapButton(this, Config::CONNECT_UP_BUTTON_ID, upBitmap, wxDefaultPosition, { 20, 20 }, wxCLIP_CHILDREN);
	upButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	upButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	upButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	layout->Add(upButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);

	layout->AddSpacer(10);

	normalPath = imgdir + "/home/dialog/down-connect-button-normal.bmp";
	currentPath = imgdir + "/home/dialog/down-connect-button-current.bmp";
	pressedPath = imgdir + "/home/dialog/down-connect-button-pressed.bmp";
	focusPath = imgdir + "/home/dialog/down-connect-button-pressed.bmp";
	wxBitmap downBitmap(normalPath, wxBITMAP_TYPE_BMP);
	downButton = new wxBitmapButton(this, Config::CONNECT_DOWN_BUTTON_ID, downBitmap, wxDefaultPosition, { 20, 20 }, wxCLIP_CHILDREN);
	downButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	downButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	downButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	layout->Add(downButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);
}

void ConnectDialog::createBottomRightBottons()
{
	bLayout->AddSpacer(100);
	wxBoxSizer* layout = new wxBoxSizer(wxHORIZONTAL);
	bLayout->Add(layout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	layout->AddSpacer(20);

	connectButton = new wxButton(this, Config::CONNECT_CONFIRM_BUTTON_ID, S("connect")); 
	layout->Add(connectButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);

	layout->AddSpacer(20);
	cancelButton = new wxButton(this, Config::CONNECT_CANCEL_BUTTON_ID, S("cancel")); 
	layout->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);

	layout->AddSpacer(20);
	testButton = new wxButton(this, Config::CONNECT_TEST_BUTTON_ID, S("test-connect"));
	layout->Add(testButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);
}

void ConnectDialog::OnLeftTreeSelChange(wxTreeEvent& event)
{
	auto oldItemId = event.GetOldItem();
	auto selItemId = event.GetItem();
	if (oldItemId.IsOk()) {
		delegate->exchangeDataFromRightPanel(rightPanel);
	}
	// todo...
	if (selItemId == leftTree->GetRootItem()) {
		supplier->runtimeUserConnect = nullptr;
		delegate->clearDataForRightPanel(rightPanel);

		delegate->disableForRightPanel(rightPanel);

		if (connectButton) connectButton->Disable();
		if (testButton) testButton->Disable();
		return;
	}  else {
		delegate->enableForRightPanel(rightPanel);

		if (connectButton) connectButton->Enable();
		if (testButton) testButton->Enable();
	}
	delegate->changeSelItemForTreeCtrl(leftTree, selItemId);
	delegate->changeDataForRightPanel(rightPanel);
}

void ConnectDialog::OnTestButtonClick(wxCommandEvent& event)
{
	// exchange the data from controls to supplier->runtimeUserConnect
	delegate->exchangeDataFromRightPanel(rightPanel);

	delegate->testConnect();
}

void ConnectDialog::OnCancelButtonClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void ConnectDialog::OnNewButtonClick(wxCommandEvent& event)
{
	delegate->createForTreeCtrl(leftTree);
}

void ConnectDialog::OnDelButtonClick(wxCommandEvent& event)
{
	delegate->removeForTreeCtrl(leftTree);

	AppContext::getInstance()->dispatch(Config::MSG_CONNECTION_REMOVE_ID);
}

void ConnectDialog::OnDupButtonClick(wxCommandEvent& event)
{
	delegate->duplicateForTreeCtrl(leftTree);
}

void ConnectDialog::OnUpButtonClick(wxCommandEvent& event)
{
	delegate->moveUpForTreeCtrl(leftTree); 
	AppContext::getInstance()->dispatch(Config::MSG_CONNECTION_MOVE_ID);
}

void ConnectDialog::OnDownButtonClick(wxCommandEvent& event)
{
	delegate->moveDownForTreeCtrl(leftTree); 
	AppContext::getInstance()->dispatch(Config::MSG_CONNECTION_MOVE_ID);
}

void ConnectDialog::OnConnectButtonClick(wxCommandEvent& event)
{
	// exchange the data from controls to supplier->runtimeUserConnect
	delegate->exchangeDataFromRightPanel(rightPanel);

	if (!delegate->connect()) {
		return ;
	}
	AppContext::getInstance()->dispatch(Config::MSG_CONNECTION_CONNECTED_ID, supplier->runtimeUserConnect->id);
	EndModal(wxID_OK);
}

void ConnectDialog::OnHandleConnectionCreate(MsgDispatcherEvent& event)
{
	auto rootItemId = leftTree->GetRootItem();
	if (leftTree->HasChildren(rootItemId)) {
		return;
	}
	MsgClientData * data = (MsgClientData *)event.GetClientData();
	std::string name = (char *)data->getDataPtr();
	delegate->createForTreeCtrl(leftTree, name);
}

/**
 * Handle the messge: Config::MSG_CONNECTION_CHANGE_NAME_ID.
 * Change the selected item name in the leftTree control
 * 
 * @param event
 */
void ConnectDialog::OnHandleConnectionChangeName(MsgDispatcherEvent& event)
{
	auto rootItemId = leftTree->GetRootItem();
	auto selItemId = leftTree->GetSelection();
	if (!selItemId.IsOk() || rootItemId == selItemId) {
		return;
	}

	auto data = (MsgClientData*)event.GetClientData();
	auto userConnect = (UserConnect*)data->getDataPtr();
	if (userConnect == nullptr) {
		return;
	}
	leftTree->SetItemText(selItemId, userConnect->name);
}
