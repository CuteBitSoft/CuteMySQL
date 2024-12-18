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
 * @file   RightBarPenel.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-17
 *********************************************************************/
#include "RightBarPenel.h"
#include "common/Config.h"
#include "ui/common/notebook/QAuiDefaultTabArt.h"
#include "utils/ResourceUtil.h"
#include "core/common/Lang.h"
#include "ui/database/rightview/common/QTabPage.h"

RightBarPenel::RightBarPenel() : QPanel<DatabaseSupplier>(), imageList(16, 16, true, 11)
{
	init();
}

RightBarPenel::~RightBarPenel()
{
}

void RightBarPenel::init()
{
	bkgColor = wxColour(30, 31, 34, 30);

	//Image list for tabView icons
	createImageList();
}

void RightBarPenel::createControls()
{
	//tabView - QNotebook
	createTabView();
	createSqlTemplatePage();
}


void RightBarPenel::createImageList()
{
	if (imageList.GetImageCount()) {
		return;
	}

	wxString imgdir = ResourceUtil::getProductImagesDir() + "/database/tab/";
	imageList.Add(wxBitmap(imgdir + "query.ico", wxBITMAP_TYPE_ICO)); // 0 - query page
	imageList.Add(wxBitmap(imgdir + "history.ico", wxBITMAP_TYPE_ICO)); // 1- history page
	imageList.Add(wxBitmap(imgdir + "table.ico", wxBITMAP_TYPE_ICO)); // 2- table structure page
	imageList.Add(wxBitmap(imgdir + "view.ico", wxBITMAP_TYPE_ICO)); // 3- view page
	imageList.Add(wxBitmap(imgdir + "trigger.ico", wxBITMAP_TYPE_ICO)); // 4- trigger
	imageList.Add(wxBitmap(imgdir + "table-data.ico", wxBITMAP_TYPE_ICO)); // 5 - table data
	imageList.Add(wxBitmap(imgdir + "table-data-dirty.ico", wxBITMAP_TYPE_ICO)); // 6- table data dirty
	imageList.Add(wxBitmap(imgdir + "table-structure-dirty.ico", wxBITMAP_TYPE_ICO)); // 7- table structure dirty
	imageList.Add(wxBitmap(imgdir + "create-table.ico", wxBITMAP_TYPE_ICO)); // 8 - create table
	imageList.Add(wxBitmap(imgdir + "create-table-dirty.ico", wxBITMAP_TYPE_ICO)); // 9 - create table dirty
	imageList.Add(wxBitmap(imgdir + "query-dirty.ico", wxBITMAP_TYPE_ICO)); // 10 - query dirty
}

void RightBarPenel::createTabView()
{
	tabView = new QNotebook();
	wxRect clientRect = GetClientRect();
	long style = wxCLIP_CHILDREN | wxNO_BORDER | wxAUI_NB_RIGHT | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_CLOSE_ON_ALL_TABS | wxAUI_NB_MIDDLE_CLICK_CLOSE;
	tabView->Create(this, Config::DATABASE_WORK_TAB_VIEW_ID, wxDefaultPosition, wxDefaultSize, wxUSE_AUI | wxAUI_NB_BOTTOM );
	//tabView->Create(this, Config::DATABASE_WORK_TAB_VIEW_ID, wxDefaultPosition, {200,800}, style);
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

void RightBarPenel::createSqlTemplatePage()
{
	auto page = new QTabPage<DatabaseSupplier>();
	page->Create(this);
	tabView->AddPage(page, "SQL Template", true, 3);
}

