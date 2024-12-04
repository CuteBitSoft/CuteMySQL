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

 * @file   DatabaseService.cpp
 * @brief  The Sqlite Databases management
 * 
 * @author Xuehan Qin
 * @date   2023-05-19
 *********************************************************************/
#include "DatabaseService.h"
#include "utils/DateUtil.h"
#include "utils/Log.h"
#include "core/common/exception/QRuntimeException.h"

UserDbList DatabaseService::getAllUserDbs(uint64_t connectId)
{
	return getRepository()->getAllByConnectId(connectId);
}

bool DatabaseService::hasUserDb(uint64_t connectId, const std::string & schema)
{
	assert(connectId > 0);
	auto userDbList = getAllUserDbs(connectId);
	for (auto& item : userDbList) {
		if (item.name == schema) {
			return true;
		}
	}
	return false;
}

UserDb DatabaseService::getUserDb(uint64_t connectId, const std::string & schema)
{
	assert(connectId > 0);
	auto userDbList = getAllUserDbs(connectId);
	for (auto& item : userDbList) {
		if (item.name == schema) {
			return item;
		}
	}
	return UserDb();
}


/**
 * remove user db , and close the connect.
 * 
 * @param userDbId
 * @param isDeleteFile
 */
void DatabaseService::removeUserDb(uint64_t connectId, const std::string & schema)
{
	assert(connectId > 0);
	
	// 2) Remove from system db.
	getRepository()->remove(connectId, schema);

	// 3) Close from connects.
	// databaseUserRepository->closeUserConnect(userDbId);
}

void DatabaseService::createUserDb(const UserDb& userDb)
{
	assert(userDb.connectId > 0 && !userDb.name.empty());

	getRepository()->create(userDb);
}

/**
 * Copy database from fromUserDbId to toDbPath.
 * 
 * @param fromUserDbId - source database dbId
 * @param toDbPath - target database path
 * @return target database db id 
 */
bool DatabaseService::copyUserDb(uint64_t fromConnectId, const std::string & fromSchema, uint64_t toConnectId, const std::string & toSchema)
{
	return false;
}
