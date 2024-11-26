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
 * @file   DatabaseListItem.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-01
 *********************************************************************/
#include "ConnectListItem.h"

#include <wx/dcclient.h>
#include "core/common/Lang.h"
#include "common/Config.h"
#include "utils/EntityUtil.h"
#include "utils/ResourceUtil.h"
#include "utils/Log.h"
#include "common/AppContext.h"

BEGIN_EVENT_TABLE(ConnectListItem, wxPanel)
	EVT_SHOW(ConnectListItem::OnShow)
	EVT_SIZE(ConnectListItem::OnSize)
	EVT_PAINT(ConnectListItem::OnPaint)
	EVT_ENTER_WINDOW(ConnectListItem::OnMouseMove)
	EVT_LEAVE_WINDOW(ConnectListItem::OnMouseLeave)
	EVT_LEFT_DOWN(ConnectListItem::OnLButtonDown)
	EVT_LEFT_DCLICK(ConnectListItem::OnLeftDoubleClick)
END_EVENT_TABLE()

ConnectListItem::ConnectListItem(UserConnect & _userConnect)
{
	this->userConnect = EntityUtil::copy(_userConnect);

	bkgBrush.SetColour(bkgColor);
	bkgHoverBrush.SetColour(bkgHoverColor);
	nameFont = FTB("panel-list-header-size", true);
	textFont = FT("panel-list-item-size");

	SetBackgroundColour(bkgColor);
}

uint64_t ConnectListItem::getUserConnectId()
{
	return userConnect.id;
}


void ConnectListItem::setChecked(bool val)
{
	checked = val;
	Refresh();
	wxColour color = checked ? bkgHoverColor : bkgColor;
	userLabel->SetBackgroundColour(color);
	hostLabel->SetBackgroundColour(color);
}

void ConnectListItem::createOrShowUI()
{
	wxRect clientRect = GetClientRect();
	createOrShowUserImage(clientRect);
	createOrShowUserLabel(clientRect);

	createOrShowHostImage(clientRect);
	createOrShowHostLabel(clientRect);
}

void ConnectListItem::createOrShowUserImage(const wxRect& clientRect)
{
	int x = 20, y = 40, w = 14, h = 14;
	wxRect rect(x, y, x + w, y + h);
	if (!userImage) {
		Q_INFO("createOrShowHostImage...");
		std::string imgDir = ResourceUtil::getStdProductImagesDir();
		std::string imgPath = imgDir + "/home/list/user.bmp"; 
		
		userImage = new wxBitmapButton(this, wxID_ANY, wxBitmap(imgPath, wxBITMAP_TYPE_BMP), { x,y }, {w, h}, wxCLIP_CHILDREN);
		userImage->SetBackgroundColour(bkgColor);
		return;
	} else {
		userImage->Move(rect.GetPosition());
	}
}


void ConnectListItem::createOrShowHostImage(const wxRect &clientRect)
{
	
	int x = 20, y = 70, w = 14, h = 14;
	wxRect rect(x, y, x + w, y + h);
	if (!hostImage) {
		Q_INFO("createOrShowHostImage...");
		std::string imgDir = ResourceUtil::getStdProductImagesDir();
		std::string imgPath = imgDir + "/home/list/host.bmp"; 
		
		hostImage = new wxBitmapButton(this, wxID_ANY, wxBitmap(imgPath, wxBITMAP_TYPE_BMP), { x,y }, {w, h}, wxCLIP_CHILDREN);
		hostImage->SetBackgroundColour(bkgColor);
		return;
	} else {
		hostImage->Move(rect.GetPosition());
	}
}

void ConnectListItem::createOrShowUserLabel(const wxRect &clientRect)
{
	wxRect rect(40, 40, clientRect.GetWidth() - 50, 16);
	userLabel = createOrShowLabel(userLabel, Config::CONNECT_ITEM_USER_LABEL_ID, userConnect.userName, rect, clientRect, wxCLIP_CHILDREN);
}

void ConnectListItem::createOrShowHostLabel(const wxRect &clientRect)
{
	wxRect rect(40, 70, clientRect.GetWidth() - 50, 16); 
	std::string hoststr = userConnect.host;
	hoststr.append(":").append(std::to_string(userConnect.port));
	hostLabel = createOrShowLabel(hostLabel, Config::CONNECT_ITEM_HOST_LABEL_ID, hoststr, rect, clientRect, wxCLIP_CHILDREN);
}

wxStaticText * ConnectListItem::createOrShowLabel(wxStaticText * win, uint32_t id, std::string text, const wxRect & rect, const wxRect &clientRect, uint32_t exStyle)
{
	Q_INFO("createOrShowLabel.text:{}", text);
	if (!win) {
		win = new wxStaticText(this, id, text, rect.GetPosition(), rect.GetSize());
		win->SetBackgroundColour(bkgColor);
		return win;
	} else  {
		win->Move(rect.GetPosition());
	}
	return win;
}

void ConnectListItem::OnSize(wxSizeEvent &event)
{
	createOrShowUI();
}

void ConnectListItem::OnShow(wxShowEvent &event)
{
	createOrShowUI();
}

void ConnectListItem::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);

	wxBrush oldBrush = dc.GetBrush();
	wxColour bColour = checked ? bkgHoverColor : bkgColor;
	wxBrush brush(bColour);
	wxRect clientRect = GetClientRect();
	dc.SetBrush(brush);
	dc.DrawRectangle(clientRect);

	int x = 10, y = 10, w = DATABASE_LIST_ITEM_WIDTH - 20, h = 40;
	wxRect rect(x, y, w, h);
	wxColour oldColor = dc.GetTextForeground();
	wxFont oldFont = dc.GetFont();
	wxPen oldPen = dc.GetPen();

	dc.SetFont(textFont);
	dc.SetTextForeground(textColor);
	std::string text = userConnect.name;
	dc.DrawLabel(text, rect, wxALIGN_LEFT | wxALIGN_TOP);

	dc.SetTextForeground(oldColor);
	dc.SetFont(oldFont);
	dc.SetPen(oldPen);
	dc.SetBrush(oldBrush);
}


void ConnectListItem::OnMouseMove(wxMouseEvent& event)
{
	SetBackgroundColour(bkgHoverColor);
	hostLabel->SetBackgroundColour(bkgHoverColor);
	
}

void ConnectListItem::OnMouseLeave(wxMouseEvent& event)
{
	SetBackgroundColour(bkgColor);
	hostLabel->SetBackgroundColour(bkgColor);
}


void ConnectListItem::OnLButtonDown(wxMouseEvent& event)
{
	AppContext::getInstance()->dispatch(Config::MSG_CONNECT_LIST_ITEM_CHECKED_ID, GetId());
}

void ConnectListItem::OnLeftDoubleClick(wxMouseEvent& event)
{
	AppContext::getInstance()->dispatch(Config::MSG_CONNECTION_CONNECTED_ID, userConnect.id);
}

