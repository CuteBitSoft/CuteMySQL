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
 * @file   DatabasePanel.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-10-31
 *********************************************************************/
#pragma once
#include <wx/panel.h>
#include <wx/splitter.h>
#include <common/Config.h>
#include "ui/common/panel/QPanel.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/database/leftview/LeftTreeView.h"
#include "ui/database/rightview/RightWorkView.h"

class DatabasePanel : public QPanel<DatabaseSupplier>
{
	DECLARE_DYNAMIC_CLASS(DatabasePanel)
	DECLARE_EVENT_TABLE()
public:
	const Config::PanelId panelId = Config::DATABASE_PANEL;
	bool isNeedReload = true;

	DatabasePanel();
	~DatabasePanel();
private:	
	wxSplitterWindow * splitter;
	LeftTreeView* leftTreeView;
	RightWorkView* rightWorkView;

	void init();
	void createControls();
	void loadControls();

	void createSplitter();
	void createLeftTreeView();
	void createRightWorkView();

	void resizeSplitter();
	void OnSize(wxSizeEvent& event);
	
};

