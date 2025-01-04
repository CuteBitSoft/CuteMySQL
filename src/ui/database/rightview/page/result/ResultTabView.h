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
 * @file   ResultTabView.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-17
 *********************************************************************/
#pragma once
#include "ui/common/panel/QPanel.h"
#include "ui/common/notebook/QNotebook.h"
#include "ui/database/rightview/page/supplier/QueryPageSupplier.h"
#include "ui/database/rightview/page/result/page/ResultListPage.h"

class ResultTabView : public QPanel<QueryPageSupplier>
{
public:
	ResultTabView(QueryPageSupplier * supplier);

	void clearMessage();
	ResultListPage * addResultToListPage(const std::string & sql, int tabNo);
	void setActivePage(int nQueryPage);
	bool execSqlToInfoPage(const std::string & sql);
	void removeResultListPageFrom(int nQueryPage);
	void activeResultInfoPage();
private:
	wxImageList imageList;
	QNotebook* tabView;
	std::vector<ResultListPage*> resultListPagePtrs;

	QueryPageSupplier* mysupplier;

	virtual void init();
	
	virtual void createControls();

	void createImageList();
	void createTabView();
	void createInfoPage();
	void createOrShowResultTableDataPage();
};

