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
 * @file   ObjectsPage.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2025-01-01
 *********************************************************************/
#pragma once
#include <wx/tglbtn.h>
#include <map>
#include "common/Config.h"
#include "core/entity/Entity.h"
#include "core/entity/Enum.h"
#include "ui/common/listview/QListView.h"
#include "ui/database/rightview/common/QTabPage.h"
#include "ui/database/rightview/page/supplier/ObjectsPageSupplier.h"


class ObjectsPage : public QTabPage<ObjectsPageSupplier>
{
	DECLARE_DYNAMIC_CLASS(ObjectsPage)
	DECLARE_EVENT_TABLE()
public:
	ObjectsPage(ObjectsPageType type = TABLE_OBJECTS);
	~ObjectsPage();
	void refreshListView(ObjectsPageType pageType);
private:
	wxBoxSizer* toolbarHoriLayout;

	// toolbar buttons
	wxBitmapButton* refreshButton;
	wxBitmapToggleButton* tableButton;
	wxBitmapToggleButton* viewButton;
	wxBitmapToggleButton* procedureButton;
	wxBitmapToggleButton* functionButton;
	wxBitmapToggleButton* triggerButton;
	wxBitmapToggleButton* eventButton;
	std::vector<wxBitmapToggleButton*> objectButtonPtrs;
	

	wxString imgdir;
	wxImageList* objectsImageList;
	QListView* listView;
	wxStatusBar* statusBar;

	virtual void init();

	virtual void createControls();
	void createLayouts();
	void createToolbarButtons();
	void createImageList();
	void createListView();
	void createStatusBar();

	virtual void loadControls();
	void refreshObjects();

	//events
	void OnClickObjectButton(wxCommandEvent& event);
private:
	const static std::map<ObjectsPageType, Config::ButtonId>  pageButtonMap;
};

