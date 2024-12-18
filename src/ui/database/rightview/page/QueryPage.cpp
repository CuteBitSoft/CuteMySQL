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

void QueryPage::setup(PageOperateType operateType, const std::string& content, const std::string& tplPath)
{
	supplier->setRuntimeUserDbId(databaseSupplier->getRuntimeUserDbId());
	supplier->setOperateType(operateType);
	if (operateType == TABLE_DATA) {
		// persistent store the three runtime data		
		supplier->setRuntimeTblName(databaseSupplier->getRuntimeTblName());
		supplier->setRuntimeSchema(databaseSupplier->getRuntimeSchema());
	}
	
	this->tplPath = tplPath;
	this->content = content;
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
	queryEditor = new QueryPageEditor();
	queryEditor->Create(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxCLIP_CHILDREN);
}

void QueryPage::createResultTabView()
{
	resultTabView = new ResultTabView();
	resultTabView->Create(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxCLIP_CHILDREN);
}
