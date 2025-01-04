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
 * @file   ResultListPage.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-22
 *********************************************************************/
#pragma once
#include "ui/common/panel/QPanel.h"
#include "ui/common/listview/QListView.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/database/rightview/page/supplier/QueryPageSupplier.h"
#include "ui/database/rightview/page/result/page/delegate/ResultListPageDelegate.h"

class ResultListPage : public QPanel<DatabaseSupplier>
{
	DECLARE_EVENT_TABLE()
public:
	ResultListPage(QueryPageSupplier * supplier, const std::string & sql);
	~ResultListPage();
	void setup(QueryPageSupplier * supplier, const std::string & sql);
	void loadListView();

private:
	int rowCount;

	wxString imgdir;
	wxBoxSizer* toolbarHoriLayout;
	wxBoxSizer* toolbarHoriLeftLayout;
	wxBoxSizer* toolbarHoriCenterLayout;
	wxBoxSizer* toolbarHoriRightLayout;

	wxCheckBox * selAllCheckBox;
	wxBitmapButton* exportButton;
	wxBitmapButton* copyButton;
	wxCheckBox * formViewCheckBox;
	wxBitmapButton* filterButton;
	wxBitmapButton* refreshButton;
	wxCheckBox * limitCheckBox;
	wxStaticText* offsetLabel;
	wxTextCtrl* offsetEdit;
	wxStaticText* rowsLabel;
	wxTextCtrl* rowsEdit;

	QListView * listView;
	wxStatusBar* statusBar;

	QueryPageSupplier* mysupplier;
	ResultListPageDelegate* delegate;

	virtual void init();
	virtual void createControls();
	void createLayouts();
	void createToolbarButtons();
	void createToobarLeftButtons();
	void createToolbarMidInputs();
	void createToobarRightButtons();
	void createListView();
	void createStatusBar();
	virtual void initControls();

	// display the result rows and exec time in the status bar
	void displayStatusBarPanels(ResultInfo & resultInfo);
	void displayRuntimeSql();
	void displayDatabase();
	void displayResultRows();
	void displayExecTime(ResultInfo & resultInfo);

	void OnClickCheckBox(wxCommandEvent& event); 
	void OnClickExportButton(wxCommandEvent& event); 
	void OnClickCopyButton(wxCommandEvent& event); 
};

