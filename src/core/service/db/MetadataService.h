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
 * @file   TableService.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-24
 *********************************************************************/
#pragma once
#include "core/common/service/BaseService.h"
#include "core/entity/Enum.h"
#include "core/repository/db/UserTableRepository.h"
#include "core/repository/db/UserViewRepository.h"
#include "core/repository/db/UserSchemaObjectRepository.h"
#include "core/repository/db/UserRoutineRepository.h"
#include "core/repository/db/UserEventRepository.h"
#include "core/repository/db/TableColumnRepository.h"
#include "core/repository/db/TableIndexRepository.h"
#include "core/repository/db/CharsetRepository.h"
#include "core/repository/db/CollationRepository.h"

class MetadataService : public BaseService<MetadataService, UserTableRepository>
{
public:
	~MetadataService();
	// get object list 
	UserTableList getUserTables(uint64_t connectId, const std::string& schema);
	UserViewList getUserViews(uint64_t connectId, const std::string& schema);	
	UserProcedureList getUserProcedures(uint64_t connectId, const std::string& schema);
	UserFunctionList getUserFunctions(uint64_t connectId, const std::string& schema);
	UserTriggerList getUserTriggers(uint64_t connectId, const std::string& schema);
	UserEventList getUserEvents(uint64_t connectId, const std::string& schema);

	UserTableList getDetailUserTables(uint64_t connectId, const std::string& schema);
	UserViewList getDetailUserViews(uint64_t connectId, const std::string& schema);
	UserProcedureList getDetailUserProcedures(uint64_t connectId, const std::string& schema);
	UserFunctionList getDetailUserFunctions(uint64_t connectId, const std::string& schema);
	UserTriggerList getDetailUserTriggers(uint64_t connectId, const std::string& schema);
	UserEventList getDetailUserEvents(uint64_t connectId, const std::string& schema);

	ColumnInfoList getColumnsOfUserTable(uint64_t connectId, const std::string& schema, const std::string& tableName);
	IndexInfoList getIndexesOfUserTable(uint64_t connectId, const std::string& schema, const std::string& tableName);

	CharsetInfoList getCharsets(uint64_t connectId);
	CollationInfoList getCollations(uint64_t connectId, const std::string& charset);

	// get one object
	UserTable getUserTable(uint64_t connectId, const std::string& schema, const std::string& name);
	UserView getUserView(uint64_t connectId, const std::string& schema, const std::string& name);

	// get object DDL
	std::string getUserObjectDDL(uint64_t connectId, const std::string& schema, const std::string& name, const std::string & objectType);

	// remove
	bool removeUserTable(uint64_t connectId, const std::string& schema, const std::string& tableName);
	bool removeUserView(uint64_t connectId, const std::string& schema, const std::string& viewName);
	bool removeUserProcedure(uint64_t connectId, const std::string& schema, const std::string& procedureName);
	bool removeUserFunction(uint64_t connectId, const std::string& schema, const std::string& functionName);
	bool removeUserTrigger(uint64_t connectId, const std::string& schema, const std::string& triggerName);
	bool removeUserEvent(uint64_t connectId, const std::string& schema, const std::string& eventName);
	bool removeTableColumn(uint64_t connectId, const std::string& schema, const std::string& tableName, const std::string& columnName);
	bool removeTableIndex(uint64_t connectId, const std::string& schema, const std::string& tableName, const std::string& indexName);

	bool hasUserTable(uint64_t connectId, const std::string& schema, const std::string& tableName);
	bool hasUserObject(uint64_t connectId, const std::string& schema, const std::string& name, const std::string &objectType);

	// -- Get object strings 
	// +- user table strings
	UserTableStrings getUserTableStrings(uint64_t connectId, const std::string & schema);
	Columns getUserColumnStrings(uint64_t connectId, const std::string & schema, const std::string & tblName);
private:
	UserViewRepository* userViewRepository = UserViewRepository::getInstance();
	UserRoutineRepository* userRoutineRepository = UserRoutineRepository::getInstance();
	UserSchemaObjectRepository* userSchemaObjectRepository = UserSchemaObjectRepository::getInstance();
	UserEventRepository* userEventRepository = UserEventRepository::getInstance();
	TableColumnRepository* tableColumnRepository = TableColumnRepository::getInstance();
	TableIndexRepository* tableIndexRepository = TableIndexRepository::getInstance();
	CharsetRepository* charsetRepository = CharsetRepository::getInstance();
	CollationRepository* collationRepository = CollationRepository::getInstance();
};

