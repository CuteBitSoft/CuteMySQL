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
 * @file   QueryPage.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-16
 *********************************************************************/

#include "QueryPage.h"
#include "common/Config.h"
#include "core/common/Lang.h"
#include "utils/SqlUtil.h"
#include "common/AppContext.h"

QueryPage::QueryPage(PageOperateType operateType, const std::string& content, const std::string& tplPath)
	: QTabPage<EmptySupplier>()
{
	init();
	setup(operateType, content, tplPath);
}

QueryPage::~QueryPage()
{
	delete mysupplier;
	mysupplier = nullptr;

	ExecutorService::destroyInstance();
	executorService = nullptr;
}

void QueryPage::setup(PageOperateType operateType, const std::string& content, const std::string& tplPath)
{
	mysupplier->setOperateType(operateType);
	// copy data from databaseSupplier(singleton for LeftTreeView)
	mysupplier->setRuntimeUserConnectId(databaseSupplier->runtimeUserConnect->id);	
	mysupplier->setRuntimeSchema(databaseSupplier->getRuntimeSchema());		
		
	if (operateType == TABLE_DATA && !databaseSupplier->getRuntimeTblName().empty()) {
		// persistent store the three runtime data		
		supplier->setRuntimeTblName(databaseSupplier->getRuntimeTblName());
	} else {
		supplier->setRuntimeTblName("");
	}
	
	this->tplPath = tplPath;
	this->content = content;
}

void QueryPage::execAndShow(bool select)
{
	std::string sqls;
	if (select) {
		sqls = queryEditor->getSelText().ToStdString();
		if (sqls.empty()) {
			sqls = queryEditor->getText().ToStdString();
		}
	}
	else {
		sqls = queryEditor->getText();
	}

	if (sqls.empty()) {
		QAnimateBox::warning(S("no-sql-statement"));
		queryEditor->focus();
		return;
	}
	if (!mysupplier->getRuntimeUserConnectId() || mysupplier->getRuntimeSchema().empty()) {
		QAnimateBox::warning(S("no-select-connection"));
		queryEditor->SetFocus();
		return;
	}
	resultTabView->clearMessage();
	if (mysupplier->getOperateType() == QUERY_DATA || mysupplier->getOperateType() == TABLE_DATA) {
		mysupplier->splitToSqlVector(sqls);
		std::vector<std::string>& sqlVector = mysupplier->sqlVector;
		int n = static_cast<int>(sqlVector.size());
		int nSelectSqlCount = 0, nNotSelectSqlCount = 0;

		// BEGIN a save point 
		std::string spSql;
		bool hasBeginTransaction = StringUtil::startWith(sqls, "BEGIN;", true);
		if (!hasBeginTransaction) {
			spSql = "BEGIN;";
			executorService->executeSql(
				mysupplier->getRuntimeUserConnectId(), 
				mysupplier->getRuntimeSchema(), 
				spSql);
		}

		bool hasError = false;
		for (int i = 0; i < n; i++) {
			auto sql = sqlVector.at(i);
			if (SqlUtil::isSelectSql(sql) || SqlUtil::isPragmaStmt(sql, true)) {
				resultTabView->addResultToListPage(sql, nSelectSqlCount + 1);
				nSelectSqlCount++;
			} else {
				nNotSelectSqlCount++;
				bool ret = resultTabView->execSqlToInfoPage(sql);
				if (!ret) {
					hasError = true;
					spSql = "ROLLBACK;"; // ROLLBACK
					executorService->executeSql(mysupplier->getRuntimeUserConnectId(), mysupplier->getRuntimeSchema(), spSql);
					break;
				}
			}
		}

		if (nSelectSqlCount) {
			// if the count of ptrs has more than nSelectSqlCount in ResultTabView object, Get gid of them.
			resultTabView->removeResultListPageFrom(nSelectSqlCount);
			resultTabView->setActivePage(0);
		}
		else if (nNotSelectSqlCount) {
			resultTabView->activeResultInfoPage();
		}
		if (!hasError) {
			bool hasCommitTransaction = StringUtil::endWith(sqls, "COMMIT;", true);
			if (!hasCommitTransaction) {
				spSql = "COMMIT;"; // COMMIT TRANSACTION
				executorService->executeSql(mysupplier->getRuntimeUserConnectId(), mysupplier->getRuntimeSchema(), spSql);
			}

			if (!nSelectSqlCount || nNotSelectSqlCount) {
				QAnimateBox::success(S("execute-sql-success"));
			}
		}
	}
	else {
		bool ret = resultTabView->execSqlToInfoPage(sqls);
		if (ret) {
			QAnimateBox::success(S("execute-sql-success"));
			if (mysupplier->getOperateType() != QUERY_DATA && mysupplier->getOperateType() != TABLE_DATA) {
				//Send message to refresh database when creating a table or altering a table , wParam = NULL, lParam=NULL 
				AppContext::getInstance()->dispatch(Config::MSG_LEFTVIEW_REFRESH_DATABASE_ID);
			}
		}
	}
}

void QueryPage::init()
{
	mysupplier = new QueryPageSupplier();
}

void QueryPage::createControls()
{
	createSplitter();
	createQueryEditor();
	createResultTabView();
	splitter->SplitHorizontally(queryEditor, resultTabView, 300);
}

void QueryPage::createSplitter()
{
	splitter = new wxSplitterWindow();
	splitter->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN | wxNO_BORDER);
	splitter->SetMinimumPaneSize(250); 
	topSizer->Add(splitter, 1, wxEXPAND);
}

void QueryPage::createQueryEditor()
{
	queryEditor = new QueryPageEditor(mysupplier);
	queryEditor->Create(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxCLIP_CHILDREN);
}

void QueryPage::createResultTabView()
{
	resultTabView = new ResultTabView(mysupplier);
	resultTabView->Create(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxCLIP_CHILDREN);
}
