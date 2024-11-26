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

 * @file   LeftPanel.h
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2024-10-29
 *********************************************************************/

#include <unordered_map>
#include <wx/panel.h>
#include <wx/tglbtn.h>
#include <common/Config.h>

class LeftPanel : public wxPanel 
{
	DECLARE_EVENT_TABLE()
public:
	LeftPanel();
	~LeftPanel();

	void selectButtonId(Config::FrmButtonId buttonId);
	Config::FrmButtonId getSelButtonId();
private:
	

	wxString resDirPath;

	// 选中的BUTTONID
	Config::FrmButtonId selButtonId = Config::HOME_BUTTON_ID;

	wxBitmapToggleButton homeButton;
	wxBitmapToggleButton databaseButton;
	wxBitmapToggleButton analysisButton;
	wxBitmapToggleButton viewButton;
	wxBitmapToggleButton settingButton;

	//左边的按钮ID和按钮的对象指针对应关系
	std::unordered_map<Config::FrmButtonId, wxBitmapToggleButton *> buttonMap;

	void createHomeButton();
	void createDatabaseButton();
	void createAnalysisButton();
	void createViewButton();
	void createSettingButton();

	void OnShow(wxShowEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnClickButton(wxCommandEvent& event);
};