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
    if (userViewRepository) {
        userViewRepository = nullptr;
    }

    UserRoutineRepository::destroyInstance();
    if (userRoutineRepository) {
        userRoutineRepository = nullptr;
    }

    UserSchemaObjectRepository::destroyInstance();
    if (userSchemaObjectRepository) {
        userSchemaObjectRepository = nullptr;
    }

    UserEventRepository::destroyInstance();
    if (userEventRepository) {
        userEventRepository = nullptr;
    }
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
