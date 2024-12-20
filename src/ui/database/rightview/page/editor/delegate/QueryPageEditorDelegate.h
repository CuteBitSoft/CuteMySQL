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
 * @file   QueryPageEditorDelegate.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-19
 *********************************************************************/
#pragma once
#include "ui/common/delegate/QDelegate.h"
#include "ui/database/rightview/page/supplier/QueryPageSupplier.h"
#include "core/service/db/DatabaseService.h"
#include "core/service/db/MetadataService.h"

class QueryPageEditorDelegate : public QDelegate<QueryPageEditorDelegate>
{
public:
	QueryPageEditorDelegate(wxWindow * editor, QueryPageSupplier * supplier);

	virtual std::vector<std::string> getTags(const std::string& line, const std::string& preline, const std::string& word, size_t curPosInLine);
	virtual std::vector<std::string>& getCacheUserTableStrings(uint64_t connectId, const std::string & schema);
	
private:
	QueryPageSupplier* mysupplier;

	MetadataService* metadataService = MetadataService::getInstance();
	DatabaseService* databaseService = DatabaseService::getInstance();

	// For auto complete
	std::vector<std::string> getSelectTags(const std::string& upline, const std::string& upPreline, const std::string& upword, size_t curPosInLine);

	std::vector<std::string> getUpdateTags(const std::string& upline, const std::string& upPreline, const std::string& upword, size_t curPosInLine);
	Columns& getCacheTableColumns(uint64_t connectId, const std::string & schema, const std::string& tblName);
};

