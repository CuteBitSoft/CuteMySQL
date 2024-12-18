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
 * @file   RightWorkView.h
 * @brief  Right work area for showing data and showing objects
 * 
 * @Class Tree  RightWorkView
 *                 |->QNoteBook(notebook)
 *                         |-> QueryPage
 *                         |      |-> CHorSplitterWindow
 *                         |            |-> QueryPageEditor <- QHelpEdit <- QHelpPage
 *                         |            |      |                 |-> QSqlEdit (Scintilla)
 *                         |            |      |-> SqlLogDialog
 *                         |            |               |-> SqlLogList
 *                         |            |                      |->SqlLogListItem
 *                         |            |-> ResultTabView
 *                         |                    |-> QTabView
 *                         |                          |-> ResultListPage
 *                         |                          |-> ResultInfoPage
 *                         |                          |-> ResultTableDataPage
 *                         |                          |-> TablePropertiesPage
 *                         |-> TableStructurePage
 *                         |       |-> TableTabView
 *                         |              |->QTabView(tabView)
 *                         |                    |->TableColumnsPage
 *                         |                    |->TableIndexesPage
 *                         |                    |->TableForeignPage
 *                         |-> HistoryPage
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-21
 *********************************************************************/
#pragma once
#include <vector>
#include <wx/imaglist.h>
#include <wx/splitter.h>
#include "ui/common/panel/QPanel.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/database/rightview/panel/RightMainPenel.h"
#include "ui/database/rightview/panel/RightBarPenel.h"

class RightWorkView : public QPanel<DatabaseSupplier>
{
	DECLARE_EVENT_TABLE()
public:
	RightWorkView();
	~RightWorkView();
private:
	wxString imgdir;
	// layout 
	wxBoxSizer * buttonsHoriLayout; // top layout for buttons
	wxBoxSizer * splliterHoriLayout; // center layout for notebook

	// execute sql buttons
	wxBitmapButton * execSqlButton;
	wxBitmapButton * execAllButton;
	wxBitmapButton * explainSqlButton;

	// query, history and objects buttons
	wxBitmapButton * queryButton;
	wxBitmapButton * objectsButton;
	wxBitmapButton * historyButton;

	// save buttons
	wxBitmapButton * saveButton;
	wxBitmapButton * saveAllButton;

	wxSplitterWindow* splitter;
	RightMainPenel* rightMainPanel;
	RightBarPenel* rightBarPanel;

	virtual void init();
	virtual void createControls();
	void createLayouts();
	void createButtons();
	void createSplit();
	void createExecButtons();
	void createQueryButtons();
	void createSaveButtons();

	void createSplitter();
	void createRightMainPenel();
	void createRightBarPenel();

};

