/*****************************************************************//**
 * Copyright 2024 Xuehan Qin 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and

 * limitations under the License.

 * @file   MainView.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2024-10-29
 *********************************************************************/
#include "MainView.h"
#include <wx/dcclient.h>
#include <utils/Log.h>
#include "common/Config.h"
#include "common/AppContext.h"
#include "common/MsgClientData.h"

BEGIN_EVENT_TABLE(MainView, wxWindow)
	EVT_SHOW(MainView::OnShow)
	EVT_SIZE(MainView::OnSize)
	EVT_BUTTON(Config::HOME_BUTTON_ID, OnClickLeftPanelButtons)
	EVT_BUTTON(Config::DATABASE_BUTTON_ID, OnClickLeftPanelButtons)
	EVT_BUTTON(Config::ANALYSIS_BUTTON_ID, OnClickLeftPanelButtons)
	EVT_BUTTON(Config::SETTING_BUTTON_ID, OnClickLeftPanelButtons)
	//EVT_PAINT(HomeView::OnPaint)

	// Handle the messge
	EVT_NOTITY_MESSAGE_HANDLE(Config::MSG_CONNECTION_CONNECTED_ID, OnHandleConnectionConnected)
END_EVENT_TABLE()

MainView::MainView() : wxWindow()
{
	//subscribe msg
	AppContext::getInstance()->subscribe(this, Config::MSG_CONNECTION_CONNECTED_ID);
	//左边的按钮ID和panel的id对应关系
	buttonPanelRelations[Config::HOME_BUTTON_ID] = Config::HOME_PANEL;
	buttonPanelRelations[Config::DATABASE_BUTTON_ID] = Config::DATABASE_PANEL;
	buttonPanelRelations[Config::ANALYSIS_BUTTON_ID] = Config::ANALYSIS_PANEL;
	buttonPanelRelations[Config::SETTING_BUTTON_ID] = Config::SETTING_PANEL;
}

MainView::~MainView()
{
	AppContext::getInstance()->unsubscribe(this, Config::MSG_CONNECTION_CONNECTED_ID);

	ConnectSupplier::destroyInstance();
	supplier = nullptr;
}

void MainView::createOrShowUI()
{
	wxRect clientRect = GetClientRect();
	Q_INFO("clientRect:w={},h={}", clientRect.GetWidth(), clientRect.GetHeight());
	createOrShowLeftPanel(clientRect);

	// create or show panel

	resizePanel(Config::HOME_PANEL, homePanel, clientRect);
	resizePanel(Config::DATABASE_PANEL, databasePanel, clientRect);
	resizePanel(Config::ANALYSIS_PANEL, analysisPanel, clientRect);
	resizePanel(Config::SETTING_PANEL, settingPanel, clientRect);
	//std::string initPanel = SettingService::getInstance()->getSysInit("init-panel");
	Config::PanelId nPanel = selectedPannelId == 0 ? Config::HOME_PANEL : selectedPannelId;

	changePanelByPanelId(nPanel);

	return;
}


void MainView::createOrShowLeftPanel(const wxRect& clientRect)
{
	const wxSize size(42, clientRect.GetHeight());
	if (!leftPanel) {
		leftPanel = new LeftPanel();
		isLeftPanelCreated = leftPanel->Create(this, wxID_ANY, wxPoint(0, 0), size, wxCLIP_CHILDREN);
		wxColour colour(0x2b, 0x2d, 0x30, 43);
		leftPanel->SetBackgroundColour(colour);
	} else {
		leftPanel->Move(0, 0);
		leftPanel->SetSize(size);
	}
}

void MainView::initPanel(Config::PanelId panelId, wxPanel * panel, const wxRect& clientRect)
{
	if (!panel) {
		return;
	}

	wxRect rect( 43, 0, clientRect.GetWidth() - 43, clientRect.GetHeight());

	// panel 右边 
	// 控制显示/隐藏
	if (panel->IsKindOf(CLASSINFO(DatabasePanel))) {
		((DatabasePanel *)panel)->Create(this, panelId, rect.GetPosition(), rect.GetSize(), wxSP_BORDER | wxCLIP_CHILDREN);
	} else if (panel->IsKindOf(CLASSINFO(AnalysisPanel))) {
		((AnalysisPanel *)panel)->Create(this, panelId, rect.GetPosition(), rect.GetSize(), wxSP_BORDER | wxCLIP_CHILDREN);
	} else if (panel->IsKindOf(CLASSINFO(SettingPanel))) {
		((SettingPanel *)panel)->Create(this, panelId, rect.GetPosition(), rect.GetSize(), wxSP_BORDER | wxCLIP_CHILDREN);
	} else {
		panel->Create(this, panelId, rect.GetPosition(), rect.GetSize());
	}
	

	wxColour colour(30, 31, 34, 30);
	panel->SetBackgroundColour(colour);
	//panel放置到MAP，便于控制显示/隐藏
	panels[panelId] = panel;

	//控制显示/隐藏
	(selectedButtonId == Config::UNSED_BUTTON_ID && panelId == Config::HOME_PANEL)
		|| (panelId == selectedPannelId)  // selectedPannelId，优先于 selectedButtonId
		//|| (panelId == buttonPanelRelations[selectedButtonId])  // selectedButtonId滞后于selectedPannelId
		? panel->Show() : panel->Hide();
}

void MainView::resizePanel(Config::PanelId panelId, wxPanel * panel, const wxRect& clientRect)
{
	if (!panel) {
		return;
	}
	wxRect rect( 43, 0, clientRect.GetWidth() - 43, clientRect.GetHeight());
	rect = { 43, 0, clientRect.GetWidth() - 43, clientRect.GetHeight() };
	panel->Move(rect.GetPosition());
	panel->SetSize(rect.GetSize());

	//控制显示/隐藏
	(selectedButtonId == Config::UNSED_BUTTON_ID && panelId == Config::HOME_PANEL)
		|| (panelId == selectedPannelId)  // selectedPannelId，优先于 selectedButtonId
		//|| (panelId == buttonPanelRelations[selectedButtonId])  // selectedButtonId滞后于selectedPannelId
		? panel->Show() : panel->Hide();
}

wxPanel* MainView::changePanelByButtonId(Config::FrmButtonId selButtonId)
{
	Config::PanelId panelId;
	// 1.先在buttonPanelRelations<buttonId, panelId>关系中找出panelId，再根据panelId定位是哪一个panel
	for (auto item : buttonPanelRelations) {
		if (item.first == selButtonId) {
			panelId = item.second;
			leftPanel->selectButtonId(selButtonId);
			break;
		}
	}
	
	wxPanel * selPanel = nullptr;
	// 2.使用panels<panelId, panels> 根据panelId定位具体是哪一个panel
	for (auto item : panels) {
		if (item.first == panelId) {
			item.second->Show();
			selectedPannelId = panelId; // 选中状态
			selPanel = item.second;
			// SettingService::getInstance()->setSysInit("init-panel",std::to_wstring(panelId));
		} else {
			item.second->Hide();
		}
	}
	return selPanel;
}

wxPanel* MainView::changePanelByPanelId(Config::PanelId panelId)
{
	// 2.使用panels<panelId, panels> 根据panelId定位具体是哪一个panel
	wxPanel* selPanel = nullptr;
	for (auto item : panels) {
		if (item.first == panelId) {
			item.second->Show();
			selectedPannelId = panelId; // 选中状态
			selPanel = item.second;
		}
		else {
			item.second->Hide();
		}
		Q_INFO("panelId:{},panel visible:{}", std::to_string(item.first), std::to_string(item.second->IsShown()));
	}

	// 在{buttonId,panelId}关系MAP里找,找不到isFound=false
	bool isFound = false;
	for (auto item : buttonPanelRelations) {
		if (item.second == selectedPannelId) {
			leftPanel->selectButtonId(item.first);			
			SettingService::getInstance()->setSysInit("init-panel", std::to_string(panelId));
			isFound = true;
			break;
		}
	}

	// 关系MAP里找不到,则手工判断
	return selPanel;
}

void MainView::OnShow(wxShowEvent& event)
{
	Q_INFO("OnShow...");
	homePanel = new HomePanel();
	databasePanel = new DatabasePanel();
	analysisPanel = new AnalysisPanel();
	settingPanel = new SettingPanel();

	auto clientRect = GetClientRect();
	initPanel(Config::HOME_PANEL, homePanel, clientRect);
	initPanel(Config::DATABASE_PANEL, databasePanel, clientRect);
	initPanel(Config::ANALYSIS_PANEL, analysisPanel, clientRect);
	initPanel(Config::SETTING_PANEL, settingPanel, clientRect);
}

void MainView::OnSize(wxSizeEvent& event)
{
	Q_INFO("OnSize...");
	createOrShowUI();
}

void MainView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	const wxRect clientRect = GetClientRect();
	const wxBrush oldBrush = dc.GetBrush();
	const wxPen oldPen = dc.GetPen();
	
	wxColour color;
	color.SetRGB(0x636363);
	wxPen pen(color);
	wxBrush brush(color);
	dc.SetBrush(brush);
	dc.SetPen(pen);
	dc.DrawRectangle(clientRect);

	dc.SetBrush(oldBrush);
	dc.SetPen(oldPen);
}

void MainView::OnClickLeftPanelButtons(wxCommandEvent& event)
{
	Config::FrmButtonId selButtonId = (Config::FrmButtonId)event.GetId();

	changePanelByButtonId(selButtonId);
}

void MainView::OnHandleConnectionConnected(MsgDispatcherEvent& event)
{
	changePanelByButtonId(Config::DATABASE_BUTTON_ID);
}


