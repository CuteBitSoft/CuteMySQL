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

 * @file   LeftPanel.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2024-10-29
 *********************************************************************/

#include "LeftPanel.h"
#include <wx/dcclient.h>
#include <utils/ResourceUtil.h>
#include <common/AppContext.h>
#include <utils/Log.h>

BEGIN_EVENT_TABLE(LeftPanel, wxPanel)
	EVT_SHOW(LeftPanel::OnShow)
	EVT_SIZE(LeftPanel::OnSize)
	EVT_TOGGLEBUTTON(Config::HOME_BUTTON_ID, OnClickButton)
	EVT_TOGGLEBUTTON(Config::DATABASE_BUTTON_ID, OnClickButton)
	EVT_TOGGLEBUTTON(Config::ANALYSIS_BUTTON_ID, OnClickButton)
	EVT_TOGGLEBUTTON(Config::SETTING_BUTTON_ID, OnClickButton)

	// Handle the messge
END_EVENT_TABLE()

LeftPanel::LeftPanel() : wxPanel()
{
	resDirPath = ResourceUtil::getResourceDir();
	AppContext::getInstance()->subscribe(this, Config::MSG_CONNECTION_CONNECTED_ID);
}

LeftPanel::~LeftPanel()
{
	AppContext::getInstance()->unsubscribe(this, Config::MSG_CONNECTION_CONNECTED_ID);
}

void LeftPanel::selectButtonId(Config::FrmButtonId buttonId)
{
	this->selButtonId = buttonId;

	for (auto item : buttonMap) {
		if (item.first == this->selButtonId) {
			item.second->SetValue(true);
		}else if (item.second->GetValue()) {
			// 原来选中，才更改，不然无需改变状态
			item.second->SetValue(false);
		}
	}
}

Config::FrmButtonId LeftPanel::getSelButtonId()
{
	return selButtonId;
}

void LeftPanel::createHomeButton()
{
	wxString normalPath = resDirPath + "/images/home-button-normal.bmp";
	wxString currentPath = resDirPath + "/images/home-button-current.bmp"; 
	wxString pressedPath = resDirPath + "/images/home-button-pressed.bmp"; 
	
	wxBitmapBundle btmBundle = wxBitmapBundle::FromBitmap(wxBitmap(normalPath, wxBITMAP_TYPE_BMP));
	homeButton.Create(this, Config::HOME_BUTTON_ID, btmBundle, wxPoint(5, 25), wxSize(32, 32), wxCLIP_CHILDREN);
	
	homeButton.SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	homeButton.SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	homeButton.SetBackgroundColour(0x2b2d30);
	homeButton.SetToolTip(_("主页"));
	//homeButton.Freeze();
}

void LeftPanel::createDatabaseButton()
{
	wxString normalPath = resDirPath + "/images/database-button-normal.bmp";
	wxString currentPath = resDirPath + "/images/database-button-current.bmp"; 
	wxString pressedPath = resDirPath + "/images/database-button-pressed.bmp";
	
	wxBitmapBundle btmBundle = wxBitmapBundle::FromBitmap(wxBitmap(normalPath, wxBITMAP_TYPE_BMP));
	databaseButton.Create(this, Config::DATABASE_BUTTON_ID, btmBundle, wxPoint(5, 25 + 32 + 15), wxSize(32, 32), wxCLIP_CHILDREN);
	databaseButton.SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	databaseButton.SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	databaseButton.SetBackgroundColour(0x2b2d30);
	databaseButton.SetToolTip(_("数据库"));
	//databaseButton.Freeze();
}

void LeftPanel::createAnalysisButton()
{
	wxString normalPath = resDirPath + "/images/analysis-button-normal.bmp";
	wxString currentPath = resDirPath + "/images/analysis-button-current.bmp"; 
	wxString pressedPath = resDirPath + "/images/analysis-button-pressed.bmp";
	
	wxBitmapBundle btmBundle = wxBitmapBundle::FromBitmap(wxBitmap(normalPath, wxBITMAP_TYPE_BMP));
	analysisButton.Create(this, Config::ANALYSIS_BUTTON_ID, btmBundle, wxPoint(5, 25 + 2 * (32 + 15)), wxSize(32, 32), wxCLIP_CHILDREN);
	analysisButton.SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	analysisButton.SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	analysisButton.SetBackgroundColour(0x2b2d30);
	analysisButton.SetToolTip(_("分析"));
	//analysisButton.Freeze();
}

void LeftPanel::createSettingButton()
{
	wxString normalPath = resDirPath + "/images/setting-button-normal.bmp";
	wxString currentPath = resDirPath + "/images/setting-button-current.bmp"; 
	wxString pressedPath = resDirPath + "/images/setting-button-pressed.bmp";
	
	wxBitmapBundle btmBundle = wxBitmapBundle::FromBitmap(wxBitmap(normalPath, wxBITMAP_TYPE_BMP));

	const wxRect clientRect = GetClientRect();
	settingButton.Create(this, Config::SETTING_BUTTON_ID, btmBundle, wxPoint(5, clientRect.GetHeight() - 32 - 15), wxSize(32, 32), wxCLIP_CHILDREN);
	settingButton.SetBitmapCurrent(wxBitmap(currentPath, wxBITMAP_TYPE_BMP));
	settingButton.SetBitmapPressed(wxBitmap(pressedPath, wxBITMAP_TYPE_BMP));
	settingButton.SetBackgroundColour(0x2b2d30);
	settingButton.SetToolTip(_("配置"));
	//settingButton.Freeze();
}

void LeftPanel::OnShow(wxShowEvent& event)
{
	createHomeButton();
	createDatabaseButton();
	createAnalysisButton();
	createSettingButton();

	buttonMap[Config::HOME_BUTTON_ID] = &homeButton;		
	buttonMap[Config::DATABASE_BUTTON_ID] = &databaseButton;
	buttonMap[Config::ANALYSIS_BUTTON_ID] = &analysisButton;
	//buttonMap[Config::VIEW_BUTTON_ID] = &viewButton;
	buttonMap[Config::SETTING_BUTTON_ID] = &settingButton;
}

void LeftPanel::OnSize(wxSizeEvent& event)
{
	const wxRect clientRect = GetClientRect();
	settingButton.Move(wxPoint(5, clientRect.GetHeight() - 32 - 15));
}

void LeftPanel::OnClickButton(wxCommandEvent& event)
{
	Q_DEBUG("Click leftpanel.button:{}", event.GetId());
	wxCommandEvent newEvent(wxEVT_BUTTON, event.GetId());
	newEvent.SetEventObject(AppContext::getInstance()->getHomeViewWindow());
	HandleWindowEvent(newEvent);
}
