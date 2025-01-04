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
 * @file   UserEventRepository.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-25
 *********************************************************************/
#include "UserEventRepository.h"
#include <list>
#include <cassert>
#include "utils/StringUtil.h"

UserEventList UserEventRepository::getAll(uint64_t connectId, const std::string& schema)
{
	assert(connectId > 0 && !schema.empty());
    UserEventList result;

	try {
		sql::SQLString sql = "SHOW EVENTS";
		auto connect = getUserConnect(connectId);
		connect->setSchema(schema);
		std::unique_ptr<sql::Statement> stmt(connect->createStatement());
		std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery(sql));
		while (resultSet->next()) {
			UserEvent item = toUserEvent(resultSet.get());
			result.push_back(item);
		}
		resultSet->close();
		stmt->close();
		return result;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to getAll(),code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}

    return result;
}

UserEventList UserEventRepository::getAllDetailList(uint64_t connectId, const std::string& schema)
{
	assert(connectId > 0 && !schema.empty());
    UserEventList result;

	try {
		sql::SQLString sql = "SELECT * FROM `information_schema`.`EVENTS` WHERE `EVENT_SCHEMA`=?";
		auto connect = getUserConnect(connectId);
		std::unique_ptr<sql::PreparedStatement> stmt(connect->prepareStatement(sql));
		stmt->setString(1, schema);
		std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery());
		while (resultSet->next()) {
			UserEvent item = toUserEventDetail(resultSet.get());
			result.push_back(item);
		}
		stmt->close();
		resultSet->close();

		return result;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to getAllDetailListByType, code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}
}

bool UserEventRepository::remove(uint64_t connectId, const std::string& schema, const std::string& name)
{
	assert(connectId > 0 && !schema.empty() && !name.empty());
	
	try {
		sql::SQLString sql = fmt::format("DROP EVENT IF EXISTS `{}`", name);
		auto connect = getUserConnect(connectId);
		connect->setSchema(schema);
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

UserEvent UserEventRepository::toUserEvent(sql::ResultSet* rs)
{
	UserEvent result;
	result.schema = rs->getString("Db").asStdString();
	result.name = StringUtil::converFromUtf8(rs->getString("Name").asStdString());
	result.definer = StringUtil::converFromUtf8(rs->getString("Definer").asStdString());
	result.timeZone = rs->getString("Time zone").asStdString();
	result.type = rs->getString("Type").asStdString();
	result.executeAt = rs->getString("Execute at").asStdString();
	result.intervalValue = rs->getString("Interval value").asStdString();
	result.intervalField = rs->getString("Interval field").asStdString();
	result.starts = rs->getString("Starts").asStdString();
	result.ends = rs->getString("Ends").asStdString();
	result.status = rs->getString("Status").asStdString();
	result.originator = rs->getString("Originator").asStdString();
	result.characterSetClient = rs->getString("character_set_client").asStdString();
	result.collationConnection = rs->getString("collation_connection").asStdString();
	result.dbCollation = rs->getString("Database Collation").asStdString();

	return result;
}

UserEvent UserEventRepository::toUserEventDetail(sql::ResultSet* rs)
{
	UserEvent result;
	result.catalog = rs->getString("EVENT_CATALOG").asStdString();
	result.schema =  StringUtil::converFromUtf8(rs->getString("EVENT_SCHEMA").asStdString());
	result.name = StringUtil::converFromUtf8(rs->getString("EVENT_NAME").asStdString());
	result.definer = StringUtil::converFromUtf8(rs->getString("DEFINER").asStdString());
	result.timeZone = rs->getString("TIME_ZONE").asStdString();
	result.ddl = rs->getString("EVENT_DEFINITION").asStdString();
	result.type = rs->getString("EVENT_TYPE").asStdString();
	result.executeAt = rs->getString("EXECUTE_AT").asStdString();
	result.intervalValue =  rs->isNull("INTERVAL_VALUE") ? "" : rs->getString("INTERVAL_VALUE").asStdString();
	result.intervalField = rs->isNull("INTERVAL_FIELD") ? "" : rs->getString("INTERVAL_FIELD").asStdString();
	result.starts = rs->isNull("STARTS") ? "" : rs->getString("STARTS").asStdString();
	result.ends = rs->isNull("ENDS") ? "" : rs->getString("ENDS").asStdString();
	result.status = rs->isNull("STATUS") ? "" : rs->getString("STATUS").asStdString();
	result.originator = rs->isNull("ORIGINATOR") ? "" : rs->getString("ORIGINATOR").asStdString();
	result.characterSetClient = rs->isNull("CHARACTER_SET_CLIENT") ? "" : rs->getString("CHARACTER_SET_CLIENT").asStdString();
	result.collationConnection = rs->isNull("COLLATION_CONNECTION") ? "" : rs->getString("COLLATION_CONNECTION").asStdString();
	result.dbCollation = rs->isNull("DATABASE_COLLATION") ? "" : rs->getString("DATABASE_COLLATION").asStdString();
	result.createTime = rs->isNull("CREATED") ? "" : rs->getString("CREATED").asStdString();
	result.updateTime = rs->isNull("LAST_ALTERED") ? "" : rs->getString("LAST_ALTERED").asStdString();
	result.lastExecute = rs->isNull("LAST_EXECUTED") ? "" : rs->getString("LAST_EXECUTED").asStdString();

	return result;
}
