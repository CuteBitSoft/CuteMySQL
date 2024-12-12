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
    return userSchemaObjectRepository->getAllByObjectType(connectId, schema, "trigger");
}

UserEventList MetadataService::getUserEvents(uint64_t connectId, const std::string& schema)
{
    return userEventRepository->getAll(connectId, schema);
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

UserView MetadataService::getUserView(uint64_t connectId, const std::string& schema, const std::string& name)
{
    return userViewRepository->get(connectId, schema, name);
}

std::string MetadataService::getUserViewDDL(uint64_t connectId, const std::string& schema, const std::string& name)
{
    return userViewRepository->getDDL(connectId, schema, name);
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

bool MetadataService::hasUserObject(uint64_t connectId, const std::string& schema, const DuplicateObjectType& dupObjectType, const std::string& tableName)
{
    return false;
}
