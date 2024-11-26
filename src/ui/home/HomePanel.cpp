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
 * @file   HomePanel.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-10-30
 *********************************************************************/

#include "HomePanel.h"
#include "core/common/Lang.h"
#include "utils/ResourceUtil.h"
#include "utils/Log.h"
#include "ui/dialog/connect/ConnectDialog.h"
#include "common/AppContext.h"
#include "common/MsgClientData.h"

BEGIN_EVENT_TABLE(HomePanel, wxPanel)
	EVT_SHOW(HomePanel::OnShow)
	EVT_SIZE(HomePanel::OnSize)
	EVT_PAINT(HomePanel::OnPaint)
	EVT_BUTTON(Config::HOME_CREATE_CONNECT_BUTTON_ID, HomePanel::OnClickCreateConnectButton)
	EVT_BUTTON(Config::HOME_MANAGE_CONNECT_BUTTON_ID, HomePanel::OnClickManageConnectButton)
	// Handle the messge
	EVT_NOTITY_MESSAGE_HANDLE(Config::MSG_CONNECTION_CONNECTED_ID, OnHandleConnectionConnected)
	EVT_NOTITY_MESSAGE_HANDLE(Config::MSG_CONNECTION_REMOVE_ID, OnHandleConnectionRemove)
	EVT_NOTITY_MESSAGE_HANDLE(Config::MSG_CONNECTION_MOVE_ID, OnHandleConnectionMove)
	EVT_NOTITY_MESSAGE_HANDLE(Config::MSG_CONNECT_LIST_ITEM_CHECKED_ID, OnHandleConnectListItemChecked)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(HomePanel, wxPanel)

HomePanel::HomePanel()
{
	AppContext::getInstance()->subscribe(this, Config::MSG_CONNECTION_CONNECTED_ID);
	AppContext::getInstance()->subscribe(this, Config::MSG_CONNECTION_REMOVE_ID);
	AppContext::getInstance()->subscribe(this, Config::MSG_CONNECTION_MOVE_ID);
	AppContext::getInstance()->subscribe(this, Config::MSG_CONNECT_LIST_ITEM_CHECKED_ID);
}

HomePanel::~HomePanel()
{
	SettingService::destroyInstance();
	if (settingService) {
		settingService = nullptr;
	}

	ConnectService::destroyInstance();
	if (connectService) {
		connectService = nullptr;
	}

	clearConnectListItemPtrs();
	AppContext::getInstance()->unsubscribe(this, Config::MSG_CONNECTION_CONNECTED_ID);
	AppContext::getInstance()->unsubscribe(this, Config::MSG_CONNECTION_REMOVE_ID);
	AppContext::getInstance()->unsubscribe(this, Config::MSG_CONNECTION_MOVE_ID);
	AppContext::getInstance()->unsubscribe(this, Config::MSG_CONNECT_LIST_ITEM_CHECKED_ID);

}

void HomePanel::drawHomeText(wxPaintDC & pdc, const wxRect& clientRect)
{
	int x = clientRect.GetLeft(), y = 70, w = clientRect.GetWidth(), h = 48;
	wxRect rect(x, y, w, h);
	//int oldMode = mpdc.SetBkMode(TRANSPARENT);
	
	wxColour oldTextColour = pdc.GetTextBackground();
	wxColour textColour(223, 225, 229, 213);
	pdc.SetTextForeground(textColour);
	
	// welcome
	wxFont oldFont = pdc.GetFont();
	wxFont textFont(wxSize(0, 48), wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,  wxFONTWEIGHT_NORMAL, false, _("Microsoft Yahei UI"));
	pdc.SetFont(textFont);
	wxCoord width, height;
	std::string welcome = S("welcome-text");
	pdc.GetTextExtent(welcome, &width, &height);
	wxString text(welcome);
	// pdc.DrawText(welcome, {(clientRect.GetWidth() - width) / 2, 70});
	pdc.DrawLabel(welcome, rect, wxALIGN_CENTER);

	// home text 
	textColour = { 188, 190, 196, 181 };
	pdc.SetTextForeground(textColour);	
	textFont.SetPixelSize(wxSize(0, 18));
	pdc.SetFont(textFont);

	std::string homeText = S("home-text1");
	pdc.GetTextExtent(homeText, &width, &height);
	wxPoint pt = { (clientRect.GetWidth() - width) / 2, 70 + 48 + 70 };
	// pdc.DrawText(homeText, pt);
	pdc.DrawLabel(homeText, {pt.x, pt.y, width, height}, wxALIGN_CENTER);

	homeText = S("home-text2");
	pdc.GetTextExtent(homeText, &width, &height);
	pt  = { (clientRect.GetWidth() - width) / 2, 70 + 48 + 70 + 18 + 20 };
	// pdc.DrawText(homeText, pt);
	pdc.DrawLabel(homeText, {pt.x, pt.y, width, height}, wxALIGN_CENTER);

	homeText = S("home-text3");
	pdc.GetTextExtent(homeText, &width, &height);
	pt = { (clientRect.GetWidth() - width) / 2, 70 + 48 + 70 + (18 + 20) * 2 };
	// pdc.DrawText(homeText, pt);
	pdc.DrawLabel(homeText, {pt.x, pt.y, width, height}, wxALIGN_CENTER);

	homeText = S("home-text4");
	pdc.GetTextExtent(homeText, &width, &height);
	pt = { (clientRect.GetWidth() - width) / 2, 70 + 48 + 70 + (18 + 20) * 3 };
	// pdc.DrawText(homeText, pt);
	pdc.DrawLabel(homeText, {pt.x, pt.y, width, height}, wxALIGN_CENTER);
	
	pdc.SetTextForeground(oldTextColour);
	pdc.SetFont(oldFont);
}


void HomePanel::loadWindow()
{
	if (!isNeedReload) {
		return;
	}
	isNeedReload = false;

	loadUserConnectList();
}

void HomePanel::loadUserConnectList()
{
	clearConnectListItemPtrs();

	wxRect clientRect = GetClientRect();
	userConnectList = connectService->getAllUserConnects();
	size_t dbLen = userConnectList.size();
	Q_INFO("userDbList.size():{}", dbLen);

	int maxWidth = clientRect.GetWidth() - 40;
	int nMax = maxWidth / DATABASE_LIST_ITEM_WIDTH; // max item size

	wxRect & lastRect = dbSectionRect;
	int x = 20, y = lastRect.GetBottom() + 20,
		w = DATABASE_LIST_ITEM_WIDTH, h = DATABASE_LIST_ITEM_HEIGHT;
	wxRect rect(x, y, w, h);
	for (size_t i = 0; i < dbLen; i++) {
		auto & userConnect = userConnectList.at(i);
		ConnectListItem * item = new ConnectListItem(userConnect);
		if (i > 0) {
			if (rect.GetRight() + w > clientRect.GetRight()) {
				rect.SetX(x);
				rect.SetY(rect.GetY() + h + 10);
				rect.SetRight(rect.GetX() + w);
				rect.SetBottom(rect.GetY() + h);
			} else {
				rect.Offset(w + 10, 0);
			}			
		}
		createUserConnectListItem(*item, Config::DB_LIST_ITEM_ID_START + i, rect, clientRect);
	}
}

void HomePanel::clearConnectListItemPtrs()
{
	
	if (connectListItemPtrs.empty()) {
		return;
	}
	int n = static_cast<int>(connectListItemPtrs.size());
	for (int i = n - 1; i >= 0; i--) {
		auto ptr = connectListItemPtrs.at(i);
		if (ptr && ptr->GetSize().GetWidth()) {
			ptr->Destroy();
			ptr = nullptr;
		}else if (ptr) {
			delete ptr;
			ptr = nullptr;
		}
	}
	connectListItemPtrs.clear();
}

void HomePanel::ressizeUserConnectListItems(wxRect& clientRect)
{
	if (clientRect.GetWidth() < DATABASE_LIST_ITEM_WIDTH) {
		return;
	}
	wxRect & lastRect = dbSectionRect;
	int x = 20, y = lastRect.GetBottom() + 20,
		w = DATABASE_LIST_ITEM_WIDTH, h = DATABASE_LIST_ITEM_HEIGHT;
	int dbLen = static_cast<int>(connectListItemPtrs.size());
	wxRect rect(x, y, w, h);
	for (int i = 0; i < dbLen; i++) {
		ConnectListItem * item = connectListItemPtrs.at(i);
		if (i > 0) {
			if (rect.GetRight() + w > clientRect.GetRight()) {
				rect.SetX(x);
				rect.SetY(rect.GetY() + h + 10);
				rect.SetRight(rect.GetX() + w);
				rect.SetBottom(rect.GetY() + h);
			} else {
				rect.Offset(w + 10, 0);
			}			
		}
		item->Move(rect.GetPosition());
	}
}


void HomePanel::createUserConnectListItem(ConnectListItem& win, uint32_t id, wxRect& rect, wxRect& clientRect)
{		
	win.Create(this, id, rect.GetPosition(), rect.GetSize(), wxCLIP_CHILDREN);
	win.SetBackgroundColour(wxColour(213, 213, 213));
	connectListItemPtrs.push_back(&win);
	return;	
}

void HomePanel::drawConnectSection(wxPaintDC& pdc, const wxRect& clientRect)
{
	wxColour oldTextColour = pdc.GetTextBackground();
	wxFont oldFont = pdc.GetFont();

	wxColour textColour(223, 225, 229, 213);
	pdc.SetTextForeground(textColour);

	secionFont = wxFont(wxSize(0, 20), wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, _("Microsoft Yahei UI"));
	pdc.SetFont(secionFont);

	std::string sectionText = S("connect-section-text");
	getConnectSectionRect(clientRect, sectionText);
	// pdc.DrawText(sectionText, dbSectionRect.GetPosition());
	pdc.DrawLabel(sectionText, {dbSectionRect.x, dbSectionRect.y, dbSectionRect.GetWidth(), dbSectionRect.GetHeight()}, wxALIGN_LEFT);

	pdc.SetTextForeground(oldTextColour);
	pdc.SetFont(oldFont);
}

void HomePanel::getConnectSectionRect(const wxRect& clientRect, const std::string& sectionText)
{
	wxClientDC dc(this);
	wxFont oldFont = dc.GetFont();

	secionFont = wxFont(wxSize(0, 20), wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, _("Microsoft Yahei UI"));
	dc.SetFont(secionFont);
	int width, height;
	dc.GetTextExtent(sectionText, &width, &height);
	dbSectionRect = {20, 70 + 48 + 70 + (18 + 20) * 4 + 40, width, height};

	dc.SetFont(oldFont);
}

void HomePanel::createOrShowConnectButtons(const wxRect& clientRect)
{
	Q_INFO("HomePanel::createOrShowDbButtons...");
	getConnectSectionRect(clientRect, S("connect-section-text"));
	int x = dbSectionRect.GetRight() + 20,
		y = dbSectionRect.GetTop() + 7,
		w = 16, h = 16;
	wxRect rect(x, y, w, h);
	std::string imgDir = ResourceUtil::getStdProductImagesDir();

	wxColour bgkColour(30, 31, 34, 30);
	std::string normalImagePath, hoverImagePath, pressedImagePath;
	if (!createConnectButton) {
		normalImagePath = imgDir + "/home/button/add-connect-button-normal.bmp";
		hoverImagePath = imgDir + "/home/button/add-connect-button-hover.bmp";
		pressedImagePath = imgDir + "/home/button/add-connect-button-pressed.bmp";
		createConnectButton = new wxBitmapButton(this, Config::HOME_CREATE_CONNECT_BUTTON_ID, wxBitmapBundle(wxBitmap(normalImagePath, wxBITMAP_TYPE_BMP)), rect.GetPosition(), { w, h });
		createConnectButton->SetBackgroundColour(bgkColour);
		createConnectButton->SetBitmapCurrent(wxBitmapBundle(wxBitmap(hoverImagePath, wxBITMAP_TYPE_BMP)));
		createConnectButton->SetBitmapPressed(wxBitmapBundle(wxBitmap(pressedImagePath, wxBITMAP_TYPE_BMP)));
		createConnectButton->SetToolTip(S("create-connection"));
	}
	
	rect = {rect.GetLeft() + w + 10, y, w, h};
	if (!modConnectButton) {
		normalImagePath = imgDir + "/home/button/mod-connect-button-normal.bmp";
		hoverImagePath = imgDir + "/home/button/mod-connect-button-hover.bmp";
		pressedImagePath = imgDir + "/home/button/mod-connect-button-pressed.bmp";
		modConnectButton= new wxBitmapButton(this, Config::HOME_MANAGE_CONNECT_BUTTON_ID, wxBitmapBundle(wxBitmap(normalImagePath, wxBITMAP_TYPE_BMP)), rect.GetPosition(), {w, h});		
		modConnectButton->SetBackgroundColour(bgkColour);
		modConnectButton->SetBitmapCurrent(wxBitmapBundle(wxBitmap(hoverImagePath, wxBITMAP_TYPE_BMP)));
		modConnectButton->SetBitmapPressed(wxBitmapBundle(wxBitmap(pressedImagePath, wxBITMAP_TYPE_BMP)));
		modConnectButton->SetToolTip(S("manage-connections"));
		modConnectButton->Raise();
	}
	
}

void HomePanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	
	wxRect clientRect = GetClientRect();
	drawHomeText(dc, clientRect);
	drawConnectSection(dc, clientRect);
}

void HomePanel::OnSize(wxSizeEvent& event)
{
	RefreshRect({ 0, 0, event.GetSize().GetWidth(), 450});

	wxRect clientRect = GetClientRect();
	ressizeUserConnectListItems(clientRect);
}

void HomePanel::OnShow(wxShowEvent& event)
{
	createOrShowConnectButtons(GetClientRect());
	loadWindow();
}

void HomePanel::OnClickCreateConnectButton(wxCommandEvent& event)
{
	ConnectDialog connectDialog(Config::CONNECT_CREATE);
	connectDialog.Create(this, -1, S("create-connection"));
	connectDialog.ShowModal();
}

void HomePanel::OnClickManageConnectButton(wxCommandEvent& event)
{
	ConnectDialog connectDialog(Config::CONNECT_MANAGE);
	connectDialog.Create(this, -1, S("create-connection"));
	connectDialog.ShowModal();
}

void HomePanel::OnHandleConnectionConnected(MsgDispatcherEvent& event)
{
	loadUserConnectList();
}

void HomePanel::OnHandleConnectionRemove(MsgDispatcherEvent& event)
{
	loadUserConnectList();
}

void HomePanel::OnHandleConnectionMove(MsgDispatcherEvent& event)
{
	loadUserConnectList();
}

void HomePanel::OnHandleConnectListItemChecked(MsgDispatcherEvent& event)
{
	auto clientData = (MsgClientData *)event.GetClientData();
	wxWindowID checkedId = (wxWindowID)clientData->getDataPtr();

	for (auto itemPtr : connectListItemPtrs) {
		itemPtr->setChecked(itemPtr->GetId() == checkedId);
	}
}
