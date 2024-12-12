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

 * @file   DbRepository.cpp
 * @brief  The table db repository
 * 
 * @author Xuehan Qin
 * @date   2023-05-19
 *********************************************************************/
#include "UserDbRepository.h"
#include <memory>
#include <mysql/jdbc.h>
#include <spdlog/fmt/fmt.h>
#include "utils/Log.h"
#include "core/common/exception/QRuntimeException.h"
#include "core/common/driver/sqlite/QSqlColumn.h"

uint64_t UserDbRepository::create(const UserDb & item)
{
	assert(item.connectId && !item.name.empty());
	//sql
	std::string sql = " ";
	try {
		sql::SQLString sql = fmt::format("CREATE DATABASE IF NOT EXISTS `{}` CHARACTER SET={} COLLATE ={}", item.name, item.charset, item.collation);
		auto connect = getUserConnect(item.connectId);
		std::unique_ptr<sql::Statement> stmt(connect->createStatement()); 
		
		stmt->execute(sql);
		stmt->close();
	}
	catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to create, code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}
	return 0;
}


bool UserDbRepository::remove(uint64_t connectId, const std::string& schema)
{
	assert(connectId > 0 && !schema.empty());
	//sql
	std::string sql = " ";
	try {
		sql::SQLString sql = fmt::format("DROP DATABASE IF EXISTS `{}`", schema);
		auto connect = getUserConnect(connectId);
		std::unique_ptr<sql::Statement> stmt(connect->createStatement());

		stmt->execute(sql);
		stmt->close();
		return true;
	}
	catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to create, code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}

	return false;
}

UserDbList UserDbRepository::getAllByConnectId(uint64_t connectId)
{
	if (connectId <= 0) {
		return UserDbList();
	}
	UserDbList result;
	
	try {
		std::unique_ptr<sql::ResultSet> resultSet(getUserConnect(connectId)->getMetaData()->getSchemas());
		while (resultSet->next()) {
			UserDb item = toUserDb(connectId, resultSet.get());
			result.push_back(item);
		}
		resultSet.get()->close();
		return result;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to getAllByConnectId(),code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}
}


/**
 * convert to entity.
 * 
 * @param query
 * @return 
 */
UserDb UserDbRepository::toUserDb(uint64_t connectId, sql::ResultSet * rs)
{
	UserDb item;
	if (rs->wasNull()) {
		return item;
	}
	item.connectId = connectId;
	item.name = rs->getString(1).asStdString();
	item.catalog = rs->getString(2).asStdString();
	return item;
}
