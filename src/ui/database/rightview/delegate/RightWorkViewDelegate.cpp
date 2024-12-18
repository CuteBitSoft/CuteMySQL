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

void RightWorkViewDelegate::setup(QNotebook* tabView, std::vector<QueryPage*>& queryPagePtrs, std::vector<TableStructurePage*>& tablePagePtrs)
{
	this->tabView = tabView;
	this->queryPagePtrs = &queryPagePtrs;
	this->tablePagePtrs = &tablePagePtrs;
}

/**
 * create first query page, then add it to tabView.
 * 
 */
void RightWorkViewDelegate::createFirstQueryPage()
{
	QueryPage* firstQueryPage = new QueryPage();
	firstQueryPage->setup(QUERY_DATA);
	firstQueryPage->Create(tabView, wxID_ANY, wxDefaultPosition, {200, 200}, wxCLIP_CHILDREN | wxNO_BORDER);
	queryPagePtrs->push_back(firstQueryPage);
	auto title = S("query-editor");
	tabView->AddPage(firstQueryPage, StringUtil::blkToTail(title), true, 0); 
}

HistoryPage* RightWorkViewDelegate::createHistoryPage()
{
	HistoryPage* historyPage = new HistoryPage();
	historyPage->Create(tabView, wxID_ANY, wxDefaultPosition, {200, 200}, wxCLIP_CHILDREN | wxNO_BORDER);
	auto title = S("history");
	tabView->AddPage(historyPage, StringUtil::blkToTail(title), false, 1); 
	return historyPage;
}
