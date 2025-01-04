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

ResultTabView::ResultTabView(QueryPageSupplier * supplier) : QPanel(), imageList(16, 16, true, 1)
{
	this->mysupplier = supplier;
	init();
}

void ResultTabView::clearMessage()
{
	// to do...
}

ResultListPage * ResultTabView::addResultToListPage(const std::string& sql, int tabNo)
{
	ResultListPage * resultListPagePtr = nullptr;
	int n = static_cast<int>(resultListPagePtrs.size());
	if (tabNo-1 < n) {
		resultListPagePtr = resultListPagePtrs.at(tabNo -1);
		resultListPagePtr->setup(mysupplier, sql);
		resultListPagePtr->loadListView();
	} else {
		resultListPagePtr = new ResultListPage(mysupplier, sql);
		resultListPagePtr->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN | wxNO_BORDER );
		std::string pageTitle = S("result-list").append(" ").append(std::to_string(tabNo));
		int nInsert = static_cast<int>(resultListPagePtrs.size());
		tabView->InsertPage(nInsert, resultListPagePtr, pageTitle.c_str(), 0, 0);
		resultListPagePtrs.push_back(resultListPagePtr);
	}
	return resultListPagePtr;
}

void ResultTabView::setActivePage(int nQueryPage)
{

}

bool ResultTabView::execSqlToInfoPage(const std::string& sql)
{
	return false;
}

void ResultTabView::removeResultListPageFrom(int nQueryPage)
{

}

void ResultTabView::activeResultInfoPage()
{

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
	createOrShowResultTableDataPage();
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

void ResultTabView::createOrShowResultTableDataPage()
{
	auto page = new ResultListPage(mysupplier, "");
	page->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN | wxNO_BORDER );
	tabView->AddPage(page, S("result"), true, 0);
	resultListPagePtrs.push_back(page);
}
