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

 * @file   HomePanel.h
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2024-10-29
 *********************************************************************/

#pragma once
#include <unordered_map>
#include <wx/window.h>
#include <ui/ConnectSupplier.h>
#include <ui/LeftPanel.h>
#include <ui/home/HomePanel.h>
#include <ui/database/DatabasePanel.h>
#include <ui/analysis/AnalysisPanel.h>
#include <ui/setting/SettingPanel.h>

#include "common/event/MsgDispatcherEvent.h"

class MainView : public wxWindow
{
	DECLARE_EVENT_TABLE()
public:
	MainView();
	~MainView();
private:
	 ConnectSupplier * supplier = ConnectSupplier::getInstance();

	bool isLeftPanelCreated = false;

	LeftPanel * leftPanel = nullptr;

	HomePanel * homePanel = nullptr;
	DatabasePanel * databasePanel = nullptr;
	AnalysisPanel * analysisPanel = nullptr;
	SettingPanel  * settingPanel = nullptr;

	// all panel sets, use to for_each, hide or show
	std::unordered_map<Config::PanelId, wxPanel *> panels;

	// left button id and panelId relationship map
	std::unordered_map<Config::FrmButtonId, Config::PanelId> buttonPanelRelations;

	// selected buttonId and selected panelId
	Config::FrmButtonId selectedButtonId = Config::UNSED_BUTTON_ID;
	Config::PanelId selectedPannelId = static_cast<Config::PanelId>(0);

	void createOrShowUI();
	void createOrShowLeftPanel(const wxRect & clientRect);
	void initPanel(Config::PanelId panelId, wxPanel * panel, const wxRect & clientRect);
	void resizePanel(Config::PanelId panelId, wxPanel * panel, const wxRect & clientRect);
	
	// change Panel(by buttonId)
	wxPanel* changePanelByButtonId(Config::FrmButtonId selButtonId);

	// change Panel(by panelId)
	wxPanel* changePanelByPanelId(Config::PanelId panelId);

	void OnShow(wxShowEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

	void OnClickLeftPanelButtons(wxCommandEvent& event);
	void OnHandleConnectionConnected(MsgDispatcherEvent& event);
};

