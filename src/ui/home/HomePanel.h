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
 * @file   HomePanel.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-10-30
 *********************************************************************/
#pragma once
#include <wx/panel.h>
#include <wx/bmpbuttn.h>
#include <wx/dcclient.h>

#include "common/Config.h"
#include "core/entity/Entity.h"
#include "ui/home/list/ConnectListItem.h"
#include "core/service/system/SettingService.h"
#include "core/service/db/ConnectService.h"
#include "common/event/MsgDispatcherEvent.h"
#include "ui/common/panel/QPanel.h"
#include "ui/common/supplier/EmptySupplier.h"

class HomePanel : public QPanel<EmptySupplier>
{
	DECLARE_DYNAMIC_CLASS(HomePanel)
	DECLARE_EVENT_TABLE()
public:
	const Config::PanelId panelId = Config::HOME_PANEL;
	bool isNeedReload = true;

	HomePanel();
	~HomePanel();
private:	
	wxBitmapButton * createConnectButton = nullptr;
	wxBitmapButton * modConnectButton = nullptr;

	wxRect dbSectionRect;
	wxFont secionFont;

	UserConnectList userConnectList;
	std::vector<ConnectListItem *> connectListItemPtrs;

	SettingService * settingService = SettingService::getInstance();
	ConnectService * connectService = ConnectService::getInstance();

	void loadWindow();
	void loadUserConnectList();
	void clearConnectListItemPtrs();

	void ressizeUserConnectListItems(wxRect & clientRect);
	void createUserConnectListItem(ConnectListItem & win, uint32_t id, wxRect & rect, wxRect & clientRect);	

	void drawHomeText(wxPaintDC & pdc, const wxRect & clientRect);
	void drawConnectSection(wxPaintDC & pdc, const wxRect & clientRect);
	void getConnectSectionRect(const wxRect& clientRect, const std::string &sectionText);

	void createOrShowConnectButtons(const wxRect& clientRect);

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnShow(wxShowEvent& event);
	void OnClickCreateConnectButton(wxCommandEvent& event);
	void OnClickManageConnectButton(wxCommandEvent& event);

	// handle message that it has been sent from ConnectDialog
	void OnHandleConnectionConnected(MsgDispatcherEvent &event);
	void OnHandleConnectionRemove(MsgDispatcherEvent &event);
	void OnHandleConnectionMove(MsgDispatcherEvent &event);
	void OnHandleConnectListItemChecked(MsgDispatcherEvent &event);
};

