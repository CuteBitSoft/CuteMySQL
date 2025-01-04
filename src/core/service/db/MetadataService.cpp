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
 * @file   MetadataService.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-25
 *********************************************************************/
#include "MetadataService.h"

MetadataService::~MetadataService()
{
    UserViewRepository::destroyInstance();
    userViewRepository = nullptr;

    UserRoutineRepository::destroyInstance();
    userRoutineRepository = nullptr;

    UserSchemaObjectRepository::destroyInstance();
    userSchemaObjectRepository = nullptr;

    UserEventRepository::destroyInstance();
     userEventRepository = nullptr;

    TableColumnRepository::destroyInstance();
     tableColumnRepository = nullptr;

    TableIndexRepository::destroyInstance();
	tableIndexRepository = nullptr;

    CharsetRepository::destroyInstance();
    charsetRepository = nullptr;

    CollationRepository::destroyInstance();
    collationRepository = nullptr;
}

UserTableList MetadataService::getUserTables(uint64_t connectId, const std::string& schema)
{
    return getRepository()->getAll(connectId, schema);
}

UserTableList MetadataService::getUserViews(uint64_t connectId, const std::string& schema)
{
    return userViewRepository->getAll(connectId, schema);
}

UserProcedureList MetadataService::getUserProcedures(uint64_t connectId, const std::string& schema)
{
    return userRoutineRepository->getAllByType(connectId, schema, RoutineType::ROUTINE_PROCEDURE);
}

UserFunctionList MetadataService::getUserFunctions(uint64_t connectId, const std::string& schema)
{
    return userRoutineRepository->getAllByType(connectId, schema, RoutineType::ROUTINE_FUNCTION);
}

UserTriggerList MetadataService::getUserTriggers(uint64_t connectId, const std::string& schema)
{
    auto list = userSchemaObjectRepository->getAllByObjectType(connectId, schema, "trigger");
    for (auto& item : list) {
        auto npos = item.name.find_last_of('.');
        if (npos == std::string::npos) {
            continue;
        }

        item.name = item.name.substr(npos + 1);
    }
    return list;
}

UserEventList MetadataService::getUserEvents(uint64_t connectId, const std::string& schema)
{
    return userEventRepository->getAll(connectId, schema);
}

UserTableList MetadataService::getDetailUserTables(uint64_t connectId, const std::string& schema)
{
    return getRepository()->getAllDetailList(connectId, schema);
}

UserViewList MetadataService::getDetailUserViews(uint64_t connectId, const std::string& schema)
{
     return userViewRepository->getAllDetailList(connectId, schema);
}

UserProcedureList MetadataService::getDetailUserProcedures(uint64_t connectId, const std::string& schema)
{
    return userRoutineRepository->getAllDetailListByType(connectId, schema, RoutineType::ROUTINE_PROCEDURE);
}

UserFunctionList MetadataService::getDetailUserFunctions(uint64_t connectId, const std::string& schema)
{
    return userRoutineRepository->getAllDetailListByType(connectId, schema, RoutineType::ROUTINE_FUNCTION);
}

UserTriggerList MetadataService::getDetailUserTriggers(uint64_t connectId, const std::string& schema)
{
    return userSchemaObjectRepository->getAllDetailTriggers(connectId, schema);
}

UserEventList MetadataService::getDetailUserEvents(uint64_t connectId, const std::string& schema)
{
    return userEventRepository->getAllDetailList(connectId, schema);
}

ColumnInfoList MetadataService::getColumnsOfUserTable(uint64_t connectId, const std::string& schema, const std::string& tableName)
{
    return tableColumnRepository->getAll(connectId, schema, tableName);
}

IndexInfoList MetadataService::getIndexesOfUserTable(uint64_t connectId, const std::string& schema, const std::string& tableName)
{
    return tableIndexRepository->getAll(connectId, schema, tableName);
}

CharsetInfoList MetadataService::getCharsets(uint64_t connectId)
{
    return charsetRepository->getAll(connectId);
}

CollationInfoList MetadataService::getCollations(uint64_t connectId, const std::string& charset)
{
    return collationRepository->getAll(connectId, charset);
}

UserTable MetadataService::getUserTable(uint64_t connectId, const std::string& schema, const std::string& name)
{
    return getRepository()->get(connectId, schema, name);
}

UserView MetadataService::getUserView(uint64_t connectId, const std::string& schema, const std::string& name)
{
    return userViewRepository->get(connectId, schema, name);
}

/**
 * Get object DDL.
 * 
 * @param connectId - connection id from sqlite.user_connect.id
 * @param schema - database name 
 * @param name - object name
 * @param objectType - object type : "DATABASE", "TABLE", "VIEW", "PROCEDURE", "FUNCTION", "TRIGGER", "EVENT"
 * @return DDL string
 */
std::string MetadataService::getUserObjectDDL(uint64_t connectId, const std::string& schema, const std::string& name, const std::string& objectType)
{
    return getRepository()->getObjectDDL(connectId, schema, name, objectType);
}

bool MetadataService::removeUserTable(uint64_t connectId, const std::string& schema, const std::string& tableName)
{
    return getRepository()->remove(connectId, schema, tableName);
}

bool MetadataService::removeUserView(uint64_t connectId, const std::string& schema, const std::string& viewName)
{
    return userViewRepository->remove(connectId, schema, viewName);
}

bool MetadataService::removeUserProcedure(uint64_t connectId, const std::string& schema, const std::string& procedureName)
{
    return userRoutineRepository->remove(connectId, schema, procedureName, RoutineType::ROUTINE_PROCEDURE);
}

bool MetadataService::removeUserFunction(uint64_t connectId, const std::string& schema, const std::string& functionName)
{
    return userRoutineRepository->remove(connectId, schema, functionName, RoutineType::ROUTINE_FUNCTION);
}

bool MetadataService::removeUserTrigger(uint64_t connectId, const std::string& schema, const std::string& triggerName)
{
    return userSchemaObjectRepository->remove(connectId, schema, triggerName, "TRIGGER");
}

bool MetadataService::removeUserEvent(uint64_t connectId, const std::string& schema, const std::string& eventName)
{
    return userEventRepository->remove(connectId, schema, eventName);
}

bool MetadataService::removeTableColumn(uint64_t connectId, const std::string& schema, const std::string& tableName, const std::string& columnName)
{
    return tableColumnRepository->remove(connectId, schema, tableName, columnName);
}

bool MetadataService::removeTableIndex(uint64_t connectId, const std::string& schema, const std::string& tableName, const std::string& indexName)
{
    return tableIndexRepository->remove(connectId, schema, tableName, indexName);
}

bool MetadataService::hasUserTable(uint64_t connectId, const std::string& schema, const std::string& tableName)
{
    return getRepository()->has(connectId, schema, tableName);
}

bool MetadataService::hasUserObject(uint64_t connectId, const std::string& schema, const std::string& name, const std::string &objectType)
{
    return getRepository()->hasObject(connectId, schema, name, objectType);
}

UserTableStrings MetadataService::getUserTableStrings(uint64_t connectId, const std::string& schema)
{
    assert(connectId > 0);
	UserTableStrings result;
     if (schema.empty()) {
        return result;
    }
	UserTableList userTableList = getRepository()->getAll(connectId, schema);
	for (auto userTable : userTableList) {
		result.push_back(userTable.name);
	}
	return result;
}

Columns MetadataService::getUserColumnStrings(uint64_t connectId, const std::string& schema, const std::string& tblName)
{
    assert(connectId > 0 && !schema.empty() && !tblName.empty());
	ColumnInfoList columnInfoList = tableColumnRepository->getAll(connectId, schema, tblName);
	Columns result;
	for (auto & item : columnInfoList) {
		result.push_back(item.name);
	}
	return result;
}
