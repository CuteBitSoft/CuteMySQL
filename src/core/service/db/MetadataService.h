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
#include "core/repository/db/UserTableRepository.h"
#include "core/repository/db/UserViewRepository.h"
#include "core/repository/db/UserSchemaObjectRepository.h"
#include "core/repository/db/UserRoutineRepository.h"
#include "core/repository/db/UserEventRepository.h"
#include "core/repository/db/TableColumnRepository.h"
#include "core/repository/db/TableIndexRepository.h"

class MetadataService : public BaseService<MetadataService, UserTableRepository>
{
public:
	~MetadataService();
	UserTableList getUserTables(uint64_t connectId, const std::string& schema);
	UserViewList getUserViews(uint64_t connectId, const std::string& schema);
	UserProcedureList getUserProcedures(uint64_t connectId, const std::string& schema);
	UserFunctionList getUserFunctions(uint64_t connectId, const std::string& schema);
	UserTriggerList getUserTriggers(uint64_t connectId, const std::string& schema);
	UserEventList getUserEvents(uint64_t connectId, const std::string& schema);

	ColumnInfoList getColumnsOfUserTable(uint64_t connectId, const std::string& schema, const std::string& tableName);
	IndexInfoList getIndexesOfUserTable(uint64_t connectId, const std::string& schema, const std::string& tableName);
private:
	UserViewRepository* userViewRepository = UserViewRepository::getInstance();
	UserRoutineRepository* userRoutineRepository = UserRoutineRepository::getInstance();
	UserSchemaObjectRepository* userSchemaObjectRepository = UserSchemaObjectRepository::getInstance();
	UserEventRepository* userEventRepository = UserEventRepository::getInstance();
	TableColumnRepository* tableColumnRepository = TableColumnRepository::getInstance();
	TableIndexRepository* tableIndexRepository = TableIndexRepository::getInstance();
};

