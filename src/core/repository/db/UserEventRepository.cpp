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

UserEventList UserEventRepository::getAll(uint64_t connectId, const std::string& schema)
{
    UserEventList result;

    if (connectId <= 0 || schema.empty()) {
		return result;
	}
	
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

UserEvent UserEventRepository::toUserEvent(sql::ResultSet* rs)
{
	UserEvent result;
	result.schema = rs->getString("Db").asStdString();
	result.name = rs->getString("Name").asStdString();
	result.definer = rs->getString("Definer").asStdString();
	result.timeZone = rs->getString("Time zone").asStdString();
	result.type = rs->getString("Type").asStdString();
	result.excuteAt = rs->getString("Execute at").asStdString();
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
