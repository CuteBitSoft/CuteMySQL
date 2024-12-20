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
 * @file   QueryPage.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-16
 *********************************************************************/
#pragma once
#include <wx/splitter.h>
#include "ui/database/rightview/common/QTabPage.h"
#include "ui/common/supplier/EmptySupplier.h"
#include "ui/database/rightview/page/supplier/QueryPageSupplier.h"
#include "ui/database/rightview/page/editor/QueryPageEditor.h"
#include "ui/database/rightview/page/result/ResultTabView.h"
#include <wx/bmpcbox.h>

class QueryPage : public QTabPage<EmptySupplier>
{
public:
	QueryPage(PageOperateType operateType, const std::string& content = std::string(), const std::string& tplPath = std::string());
	~QueryPage();
	void setup(PageOperateType operateType, const std::string & content = std::string(), const std::string & tplPath = std::string());
private:
	std::string viewName;
	std::string tplPath;
	std::string content;

	QueryPageSupplier* mysupplier;
	bool isSpliterReload = true;

	QueryPageEditor* queryEditor;
	ResultTabView* resultTabView;
	wxSplitterWindow * splitter;

	virtual void init();
	virtual void createControls();
	void createSplitter();
	void createQueryEditor();
	void createResultTabView();
};

