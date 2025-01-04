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
 * @file   RightWorkViewDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-16
 *********************************************************************/
#include "RightWorkViewDelegate.h"
#include "utils/StringUtil.h"
#include "core/common/Lang.h"
#include "core/entity/Entity.h"

void RightWorkViewDelegate::setup(QNotebook* tabView, std::vector<QueryPage*>& queryPagePtrs, std::vector<TableStructurePage*>& tablePagePtrs)
{
	this->tabView = tabView;
	this->historyPage = nullptr;
	this->objectsPage = nullptr;
	this->queryPagePtrs = &queryPagePtrs;
	this->tablePagePtrs = &tablePagePtrs;
}

/**
 * create first query page, then add it to tabView.
 * 
 */
void RightWorkViewDelegate::createFirstQueryPage()
{
	QueryPage* firstQueryPage = new QueryPage(QUERY_DATA);
	firstQueryPage->Create(tabView, wxID_ANY, wxDefaultPosition, {200, 200}, wxCLIP_CHILDREN | wxNO_BORDER);
	queryPagePtrs->push_back(firstQueryPage);
	auto title = S("query-editor");
	tabView->AddPage(firstQueryPage, StringUtil::blkToTail(title), true, 0); 
}

HistoryPage* RightWorkViewDelegate::createHistoryPage()
{
	historyPage = new HistoryPage();
	historyPage->Create(tabView, wxID_ANY, wxDefaultPosition, {200, 200}, wxCLIP_CHILDREN | wxNO_BORDER);
	auto title = S("history");
	tabView->AddPage(historyPage, StringUtil::blkToTail(title), false, 1); 
	return historyPage;
}

void RightWorkViewDelegate::execSelectedSql()
{
	int nPage = tabView->GetSelection();
	if (nPage < 0) {
		return;
	}

	wxWindow * activeWindow = tabView->GetPage(nPage);
	for (auto pagePtr : *queryPagePtrs) {
		if (pagePtr && activeWindow == pagePtr) {
			pagePtr->execAndShow(true);
		}
	}
}

void RightWorkViewDelegate::openObjectsPage(TreeObjectType treeObjectType)
{
	// find the objects page in the tabView
	bool found = false;
	int nSelPage = tabView->GetSelection();
	size_t pageCount = tabView->GetPageCount();
	for (size_t i = 0; i < pageCount; ++i) {
		wxWindow* pagePtr = tabView->GetPage(i);
		if (pagePtr->IsKindOf(CLASSINFO(ObjectsPage))) {
			found = true;
			objectsPage = (ObjectsPage*)pagePtr;
			if (nSelPage != static_cast<int>(i)) {
				tabView->SetSelection(i);
			}
		}
	}

	if (found) {
		doChangeObjectsPage(treeObjectType);
		return ;
	}

	// If Objects page not found, create a new.
	doCreateObjectsPage(treeObjectType);
}

void RightWorkViewDelegate::changeObjectsPage(TreeObjectType treeObjectType)
{
	// find the objects page in the tabView
	bool found = false;
	int nSelPage = tabView->GetSelection();
	size_t pageCount = tabView->GetPageCount();
	for (size_t i = 0; i < pageCount; ++i) {
		wxWindow* pagePtr = tabView->GetPage(i);
		if (pagePtr->IsKindOf(CLASSINFO(ObjectsPage))) {
			objectsPage = (ObjectsPage*)pagePtr;
			if (nSelPage == static_cast<int>(i)) {
				found = true;
			}
		}
	}

	if (!found) {
		return ;
	}

	doChangeObjectsPage(treeObjectType);
}


void RightWorkViewDelegate::doCreateObjectsPage(TreeObjectType treeObjectType)
{
	ObjectsPageType pageType;
	switch (treeObjectType)
	{
	case TreeObjectType::SCHEMA:
	case TreeObjectType::TABLES_FOLDER:
		pageType = TABLE_OBJECTS;
		break;
	case TreeObjectType::VIEWS_FOLDER:
		pageType = VIEW_OBJECTS;
		break;
	case TreeObjectType::STORE_PROCEDURES_FOLDER:
		pageType = PROCEDURE_OBJECTS;
		break;
	case TreeObjectType::FUNCTIONS_FOLDER:
		pageType = FUNCTION_OBJECTS;
		break;
	case TreeObjectType::TRIGGERS_FOLDER:
		pageType = TRIGGER_OBJECTS;
		break;
	case TreeObjectType::EVENTS_FOLDER:
		pageType = EVENT_OBJECTS;
		break;
	default:
		return;
	}
	
	objectsPage = new ObjectsPage(pageType);
	objectsPage->Create(tabView, wxID_ANY, wxDefaultPosition, {200, 200}, wxCLIP_CHILDREN | wxNO_BORDER);
	auto title = S("objects");
	tabView->InsertPage(0, objectsPage, StringUtil::blkToTail(title), true, 2); // image id: 2 - objects
}


void RightWorkViewDelegate::doChangeObjectsPage(TreeObjectType treeObjectType)
{
	assert(objectsPage);

	ObjectsPageType pageType;
	switch (treeObjectType)
	{
	case TreeObjectType::SCHEMA:
	case TreeObjectType::TABLES_FOLDER:
		pageType = TABLE_OBJECTS;
		break;
	case TreeObjectType::VIEWS_FOLDER:
		pageType = VIEW_OBJECTS;
		break;
	case TreeObjectType::STORE_PROCEDURES_FOLDER:
		pageType = PROCEDURE_OBJECTS;
		break;
	case TreeObjectType::FUNCTIONS_FOLDER:
		pageType = FUNCTION_OBJECTS;
		break;
	case TreeObjectType::TRIGGERS_FOLDER:
		pageType = TRIGGER_OBJECTS;
		break;
	case TreeObjectType::EVENTS_FOLDER:
		pageType = EVENT_OBJECTS;
		break;
	default:
		return;
	}

	objectsPage->refreshListView(pageType);
}
