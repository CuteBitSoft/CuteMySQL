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
 * @file   UserRoutineRepository.h
 * @brief  User routine (procedure/function) repository of msyql database
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-25
 *********************************************************************/
#pragma once
#include "core/common/repository/BaseUserRepository.h"
#include "core/entity/Entity.h"
class UserRoutineRepository : public BaseUserRepository<UserRoutineRepository>
{
public:
	UserRoutineList getAllByType(uint64_t connectId, const std::string& schema, RoutineType type = RoutineType::ROUTINE_PROCEDURE);
	UserRoutineList getAllDetailListByType(uint64_t connectId, const std::string& schema, RoutineType type = RoutineType::ROUTINE_PROCEDURE);
	bool remove(uint64_t connectId, const std::string& schema, const std::string& name, RoutineType type = RoutineType::ROUTINE_PROCEDURE);

	std::vector<std::string> getSysFunctions(uint64_t connectId);
private:
	UserRoutine toUserRoutine(sql::ResultSet* rs);
	UserRoutine toUserRoutineDetail(sql::ResultSet* rs);
};

