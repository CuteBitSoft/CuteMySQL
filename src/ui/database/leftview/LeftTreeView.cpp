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
#include "common/Config.h"
#include "utils/ResourceUtil.h"
#include "core/common/Lang.h"
#include "core/entity/Entity.h"
#include "common/AppContext.h"
#include "common/MsgClientData.h"

BEGIN_EVENT_TABLE(LeftTreeView, wxPanel)
	EVT_SIZE(LeftTreeView::OnSize)
	EVT_PAINT(LeftTreeView::OnPaint)
	EVT_TREE_ITEM_EXPANDED(Config::DATABASE_TREEVIEW_ID, OnTreeItemExpended)
	// Handle the messge
	EVT_NOTITY_MESSAGE_HANDLE(Config::MSG_CONNECTION_CONNECTED_ID, OnHandleConnectionConnected)
END_EVENT_TABLE()

LeftTreeView::LeftTreeView():QPanel()
{
	AppContext::getInstance()->subscribe(this, Config::MSG_CONNECTION_CONNECTED_ID);
	delegate = LeftTreeDelegate::getInstance(this);
}

LeftTreeView::~LeftTreeView()
{
	AppContext::getInstance()->unsubscribe(this, Config::MSG_CONNECTION_CONNECTED_ID);
	
	LeftTreeDelegate::destroyInstance();
	delegate = nullptr;	
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
	wxString normalPath = imgdir + "/database/button/new-button-normal.bmp";
	wxString currentPath = imgdir + "/database/button/new-button-current.bmp";
	wxString pressedPath = imgdir + "/database/button/new-button-pressed.bmp";
	wxString focusPath = imgdir + "/database/button/new-button-pressed.bmp";
	wxBitmap newBitmap(normalPath, wxBITMAP_TYPE_BMP);
	newButton = new wxBitmapButton(this, Config::DATABASE_CREATE_BUTTON_ID, newBitmap, {1, 10}, { 20, 20 }, wxCLIP_CHILDREN);
	newButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	newButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	newButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	
	normalPath = imgdir + "/database/button/del-button-normal.bmp";
	currentPath = imgdir + "/database/button/del-button-current.bmp";
	pressedPath = imgdir + "/database/button/del-button-pressed.bmp";
	focusPath = imgdir + "/database/button/del-button-pressed.bmp";
	wxBitmap delBitmap(normalPath, wxBITMAP_TYPE_BMP);
	delButton = new wxBitmapButton(this, Config::DATABASE_DELETE_BUTTON_ID, delBitmap, {1 + 20 + 5, 10}, { 20, 20 }, wxCLIP_CHILDREN);
	delButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	delButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	delButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));

	normalPath = imgdir + "/database/button/dup-button-normal.bmp";
	currentPath = imgdir + "/database/button/dup-button-current.bmp";
	pressedPath = imgdir + "/database/button/dup-button-pressed.bmp";
	focusPath = imgdir + "/database/button/dup-button-pressed.bmp";
	wxBitmap dupBitmap(normalPath, wxBITMAP_TYPE_BMP);
	dupButton = new wxBitmapButton(this, Config::DATABASE_DUP_BUTTON_ID, dupBitmap, {1 + (20 + 5) * 2, 10}, { 20, 20 }, wxCLIP_CHILDREN);
	dupButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	dupButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	dupButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	
	normalPath = imgdir + "/database/button/refresh-button-normal.bmp";
	currentPath = imgdir + "/database/button/refresh-button-current.bmp";
	pressedPath = imgdir + "/database/button/refresh-button-pressed.bmp";
	focusPath = imgdir + "/database/button/refresh-button-pressed.bmp";
	wxBitmap refreshBitmap(normalPath, wxBITMAP_TYPE_BMP);
	refreshButton = new wxBitmapButton(this, Config::DATABASE_REFRESH_BUTTON_ID, refreshBitmap, {1 + (20 + 5) * 3, 10}, { 20, 20 }, wxCLIP_CHILDREN);
	refreshButton->SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	refreshButton->SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	refreshButton->SetBitmapFocus(wxBitmap(focusPath, wxBITMAP_TYPE_BMP));
	
}

void LeftTreeView::createComboBox()
{
	auto size = GetClientSize();
	selectedDbComboBox = new wxBitmapComboBox(this, Config::TREEVIEW_SELECTED_DB_COMBOBOX_ID, wxEmptyString, {size.GetWidth() - 10 - 100, 10}, {100, 20});
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
	selectedDbComboBox->SetPosition({ size.GetWidth() - 10 - 100, 10 });
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
	delegate->loadForLeftTree(treeView);
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

	delegate->expendedForLeftTree(treeView, itemId);
}

void LeftTreeView::OnHandleConnectionConnected(MsgDispatcherEvent& event)
{
	auto clientData = (MsgClientData *)event.GetClientData();
	auto connectId = clientData->getDataPtr();

	delegate->loadForLeftTree(treeView, connectId);
}
