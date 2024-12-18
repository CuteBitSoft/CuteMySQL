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
 * @file   RightWorkView.cpp
 * @brief  
 * @Class Tree  RightWorkView
 *				   |->RightMainPanel
 *                 |     |->QTabView(tabView)
 *                 |             |-> QueryPage
 *                 |             |      |-> CHorSplitterWindow
 *                 |             |            |-> QueryPageEditor <- QHelpEdit <- QHelpPage
 *                 |             |            |      |                 |-> QSqlEdit (Scintilla)
 *                 |             |            |      |-> SqlLogDialog
 *                 |             |            |               |-> SqlLogList
 *                 |             |            |                      |->SqlLogListItem
 *                 |             |            |-> ResultTabView
 *                 |             |                    |-> QTabView
 *                 |             |                          |-> ResultListPage
 *                 |             |                          |-> ResultInfoPage
 *                 |             |                          |-> ResultTableDataPage
 *                 |             |                          |-> TablePropertiesPage
 *                 |             |-> TableStructurePage
 *                 |             |       |-> TableTabView
 *                 |             |              |->QTabView(tabView)
 *                 |             |                    |->TableColumnsPage
 *                 |             |                    |->TableIndexesPage
 *                 |             |                    |->TableForeignPage
 *                 |             |-> HistoryPage
 *                 |
 *                 |->RightBarView(wxNotebook)
 *                       |->QTabView(tabView)
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-21
 *********************************************************************/
#include "RightWorkView.h"
#include "common/Config.h"
#include "utils/ResourceUtil.h"
#include "core/common/Lang.h"

BEGIN_EVENT_TABLE(RightWorkView, wxPanel)
END_EVENT_TABLE()

RightWorkView::RightWorkView():QPanel()
{
	init();
}

RightWorkView::~RightWorkView()
{
	
}

void RightWorkView::init()
{
	bkgColor = wxColour(30, 31, 34, 30);

}

void RightWorkView::createControls()
{
	QPanel::createControls();
	// layouts
	createLayouts();
	// buttons
	createButtons();
	// splitter
	createSplitter();
	// right main panel
	createRightMainPenel();
	// right bar panel
	createRightBarPenel();

	wxSize size = GetClientSize();
	splitter->SplitVertically(rightMainPanel, rightBarPanel, size.GetWidth() - 200);
}

void RightWorkView::createLayouts()
{
	topSizer->AddSpacer(5);
	buttonsHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(buttonsHoriLayout, 0, wxALIGN_LEFT | wxALIGN_TOP);

	topSizer->AddSpacer(5);

	splliterHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(splliterHoriLayout, 1, wxEXPAND);
}

void RightWorkView::createButtons()
{
	imgdir = ResourceUtil::getProductImagesDir() + "/database/button/right/";
	createExecButtons();
	createQueryButtons();
	createSaveButtons();
}

void RightWorkView::createSplit()
{
	wxBitmap bitmap(imgdir + "split.png", wxBITMAP_TYPE_PNG);
	wxStaticBitmap * split = new wxStaticBitmap(this, wxID_ANY, wxBitmapBundle(bitmap), wxDefaultPosition, { 20, 20 }, wxCLIP_CHILDREN | wxNO_BORDER);
	buttonsHoriLayout->Add(split, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
}

void RightWorkView::createExecButtons()
{
	buttonsHoriLayout->AddSpacer(10);
	
	wxBitmap normalBitmap(imgdir + "exec-sql-button-normal.png", wxBITMAP_TYPE_PNG);
	wxBitmap pressedBitmap(imgdir + "exec-sql-button-pressed.png", wxBITMAP_TYPE_PNG);
	execSqlButton = new wxBitmapButton(this, Config::DATABASE_EXEC_SQL_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, {22, 22}, wxCLIP_CHILDREN | wxNO_BORDER);
	execSqlButton->SetBackgroundColour(bkgColor);
	execSqlButton->SetBitmapPressed(pressedBitmap);
	execSqlButton->SetBitmapFocus(pressedBitmap);
	execSqlButton->SetToolTip(SNT("exec-select-sql"));
	buttonsHoriLayout->Add(execSqlButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	buttonsHoriLayout->AddSpacer(10);

	normalBitmap = wxBitmap(imgdir + "exec-all-button-normal.png", wxBITMAP_TYPE_PNG);
	pressedBitmap = wxBitmap(imgdir + "exec-all-button-pressed.png", wxBITMAP_TYPE_PNG);
	execAllButton = new wxBitmapButton(this, Config::DATABASE_EXEC_ALL_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, {22, 22}, wxCLIP_CHILDREN | wxNO_BORDER);
	execAllButton->SetBackgroundColour(bkgColor);
	execAllButton->SetBitmapPressed(pressedBitmap);
	execAllButton->SetBitmapFocus(pressedBitmap);
	execAllButton->SetToolTip(SNT("exec-all-sql"));
	buttonsHoriLayout->Add(execAllButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	buttonsHoriLayout->AddSpacer(10);

	normalBitmap = wxBitmap(imgdir + "explain-button-normal.png", wxBITMAP_TYPE_PNG);
	pressedBitmap = wxBitmap(imgdir + "explain-button-pressed.png", wxBITMAP_TYPE_PNG);
	explainSqlButton = new wxBitmapButton(this, Config::DATABASE_EXPLAIN_SQL_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, {22, 22}, wxCLIP_CHILDREN | wxNO_BORDER);
	explainSqlButton->SetBackgroundColour(bkgColor);
	explainSqlButton->SetBitmapPressed(pressedBitmap);
	explainSqlButton->SetBitmapFocus(pressedBitmap);
	explainSqlButton->SetToolTip(SNT("explain-select-sql"));
	buttonsHoriLayout->Add(explainSqlButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
}

void RightWorkView::createQueryButtons()
{
	createSplit();
	wxBitmap normalBitmap(imgdir + "query-button-normal.png", wxBITMAP_TYPE_PNG);
	wxBitmap pressedBitmap(imgdir + "query-button-pressed.png", wxBITMAP_TYPE_PNG);
	queryButton = new wxBitmapButton(this, Config::DATABASE_QUERY_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, {22, 22}, wxCLIP_CHILDREN | wxNO_BORDER);
	queryButton->SetBackgroundColour(bkgColor);
	queryButton->SetBitmapPressed(pressedBitmap);
	queryButton->SetBitmapFocus(pressedBitmap);
	queryButton->SetToolTip(SNT("new-query"));
	buttonsHoriLayout->Add(queryButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	normalBitmap = wxBitmap(imgdir + "objects-button-normal.png", wxBITMAP_TYPE_PNG);
	pressedBitmap = wxBitmap(imgdir + "objects-button-pressed.png", wxBITMAP_TYPE_PNG);
	objectsButton = new wxBitmapButton(this, Config::DATABASE_OBJECTS_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, {22, 22}, wxCLIP_CHILDREN | wxNO_BORDER);
	objectsButton->SetBackgroundColour(bkgColor);
	objectsButton->SetBitmapPressed(pressedBitmap);
	objectsButton->SetBitmapFocus(pressedBitmap);
	objectsButton->SetToolTip(SNT("objects"));
	buttonsHoriLayout->Add(objectsButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	normalBitmap = wxBitmap(imgdir + "history-button-normal.png", wxBITMAP_TYPE_PNG);
	pressedBitmap = wxBitmap(imgdir + "history-button-pressed.png", wxBITMAP_TYPE_PNG);
	historyButton = new wxBitmapButton(this, Config::DATABASE_HISTORY_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, {22, 22}, wxCLIP_CHILDREN | wxNO_BORDER);
	historyButton->SetBackgroundColour(bkgColor);
	historyButton->SetBitmapPressed(pressedBitmap);
	historyButton->SetBitmapFocus(pressedBitmap); 
	historyButton->SetToolTip(SNT("history"));
	buttonsHoriLayout->Add(historyButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
}

void RightWorkView::createSaveButtons()
{
	createSplit();
	wxBitmap normalBitmap(imgdir + "save-button-normal.png", wxBITMAP_TYPE_PNG);
	wxBitmap pressedBitmap(imgdir + "save-button-pressed.png", wxBITMAP_TYPE_PNG);
	saveButton = new wxBitmapButton(this, Config::DATABASE_SAVE_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, {22, 22}, wxCLIP_CHILDREN | wxNO_BORDER);
	saveButton->SetBackgroundColour(bkgColor);
	saveButton->SetBitmapPressed(pressedBitmap);
	saveButton->SetBitmapFocus(pressedBitmap);
	saveButton->SetToolTip(SNT("save-current"));
	buttonsHoriLayout->Add(saveButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	normalBitmap = wxBitmap(imgdir + "save-all-button-normal.png", wxBITMAP_TYPE_PNG);
	pressedBitmap = wxBitmap(imgdir + "save-all-button-pressed.png", wxBITMAP_TYPE_PNG);
	saveAllButton = new wxBitmapButton(this, Config::DATABASE_SAVE_ALL_BUTTON_ID, wxBitmapBundle(normalBitmap), wxDefaultPosition, {22, 22}, wxCLIP_CHILDREN | wxNO_BORDER);
	saveAllButton->SetBackgroundColour(bkgColor);
	saveAllButton->SetBitmapPressed(pressedBitmap);
	saveAllButton->SetBitmapFocus(pressedBitmap);
	saveAllButton->SetToolTip(SNT("save-all"));
	buttonsHoriLayout->Add(saveAllButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
}

void RightWorkView::createSplitter()
{
	wxSize clientSize = GetClientSize();
	splitter = new wxSplitterWindow();
	splitter->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN | wxNO_BORDER);
	splitter->SetMinimumPaneSize(250); 
	splliterHoriLayout->Add(splitter, 1, wxEXPAND);
}

void RightWorkView::createRightMainPenel()
{
	rightMainPanel = new RightMainPenel();
	rightMainPanel->Create(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN | wxNO_BORDER);
	
}

void RightWorkView::createRightBarPenel()
{
	rightBarPanel = new RightBarPenel();
	rightBarPanel->Create(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN | wxNO_BORDER);
}

