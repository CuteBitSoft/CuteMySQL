/*****************************************************************//**
 * Copyright 2024 Xuehan Qin 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and

 * limitations under the License.

 * @file   DatabaseService.h
 * @brief  The Sqlite Databases management class
 * 
 * @author Xuehan Qin
 * @date   2023-05-19
 *********************************************************************/
#pragma once
#include "core/common/service/BaseService.h"
#include "core/repository/db/UserDbRepository.h"

class DatabaseService : public BaseService<DatabaseService, UserDbRepository>
{
public:
	// user db operations
	UserDbList getAllUserDbs(uint64_t connectId);
	bool hasUserDb(uint64_t connectId, const std::string & schema);
	UserDb getUserDb(uint64_t connectId, const std::string & schema);
	void removeUserDb(uint64_t connectId, const std::string & schema);
	void createUserDb(const UserDb& userDb);
	bool copyUserDb(uint64_t fromConnectId, const std::string & fromSchema, uint64_t toConnectId, const std::string & toSchema);

	std::vector<std::string> getSysFunctionStrings(uint64_t connectId, const std::string& schema, bool upcase);
private:

};
