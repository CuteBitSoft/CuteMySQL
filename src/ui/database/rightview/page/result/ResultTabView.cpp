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
 * @file   ResultTabView.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-17
 *********************************************************************/
#include "ResultTabView.h"
#include "utils/ResourceUtil.h"
#include "common/Config.h"
#include "ui/common/notebook/QAuiDefaultTabArt.h"
#include "ui/database/rightview/common/QTabPage.h"
#include "core/common/Lang.h"

ResultTabView::ResultTabView() : QPanel(), imageList(16, 16, true, 1)
{
	init();
}

void ResultTabView::init()
{
	bkgColor = wxColour(30, 31, 34, 30);
	createImageList();
}

void ResultTabView::createControls()
{
	createTabView();
	createInfoPage();
	createResultListPage();
}

void ResultTabView::createImageList()
{
	if (imageList.GetImageCount()) {
		return;
	}

	wxString imgdir = ResourceUtil::getProductImagesDir() + "/database/tab/";
	imageList.Add(wxBitmap(imgdir + "result.ico", wxBITMAP_TYPE_ICO)); // 0 - result page
	imageList.Add(wxBitmap(imgdir + "info.ico", wxBITMAP_TYPE_ICO)); // 1- info page
	imageList.Add(wxBitmap(imgdir + "table-data.ico", wxBITMAP_TYPE_ICO)); // 2 - table data
	imageList.Add(wxBitmap(imgdir + "table-properties.ico", wxBITMAP_TYPE_ICO)); // 3- table properties dirty
	imageList.Add(wxBitmap(imgdir + "table-data-dirty.ico", wxBITMAP_TYPE_ICO)); // 4 - table data dirty
}

void ResultTabView::createTabView()
{
	tabView = new QNotebook();
	wxRect clientRect = GetClientRect();
	long style = wxCLIP_CHILDREN | wxNO_BORDER | wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_CLOSE_ON_ALL_TABS | wxAUI_NB_MIDDLE_CLICK_CLOSE;
	tabView->Create(this, Config::DATABASE_RESULT_TAB_VIEW_ID, wxDefaultPosition, wxDefaultSize, style );
	tabView->SetBackgroundColour(bkgColor);
	tabView->SetForegroundColour(textColor);
	tabView->SetImageList(&imageList);
	tabView->SetArtProvider(new QAuiDefaultTabArt());
	auto tabArt = tabView->GetArtProvider();
	tabArt->SetActiveColour(textColor);
	tabArt->SetColour(bkgColor);
	topSizer->AddSpacer(1);
	topSizer->Add(tabView, 1, wxEXPAND);
	topSizer->AddSpacer(1);
}

void ResultTabView::createInfoPage()
{
	auto page = new QTabPage<DatabaseSupplier>();
	page->Create(this);
	tabView->AddPage(page, S("result-info"), true, 1);
}

void ResultTabView::createResultListPage()
{
	auto page = new QTabPage<DatabaseSupplier>();
	page->Create(this);
	tabView->AddPage(page, S("result"), true, 0);
}
