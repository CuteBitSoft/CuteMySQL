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
 * @file   UserSchemaObjectRepository.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-25
 *********************************************************************/
#include "UserSchemaObjectRepository.h"
#include <list>
#include "utils/StringUtil.h"

UserRoutineList UserSchemaObjectRepository::getAllByObjectType(uint64_t connectId, const std::string& schema, const std::string & objectType)
{
    UserRoutineList result;

    if (connectId <= 0 || schema.empty()) {
		return result;
	}
	
	try {
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getSchemaObjects(catalog, schema, objectType));
		while (resultSet->next()) {			
			UserRoutine item = toUserRoutine(resultSet.get());
			result.push_back(item);
		}
		resultSet->close();
		return result;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to getAll(),code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}

    return result;
}

UserTriggerList UserSchemaObjectRepository::getAllDetailTriggers(uint64_t connectId, const std::string& schema)
{
	assert(connectId > 0 && !schema.empty());
	UserTriggerList result;

	try {
		sql::SQLString sql = "SELECT * FROM `information_schema`.`TRIGGERS` WHERE `TRIGGER_SCHEMA`=?";
		auto connect = getUserConnect(connectId);
		std::unique_ptr<sql::PreparedStatement> stmt(connect->prepareStatement(sql));
		stmt->setString(1, schema);
		std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery());
		while (resultSet->next()) {
			UserRoutine item = toUserTriggerDetail(resultSet.get());
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

bool UserSchemaObjectRepository::remove(uint64_t connectId, const std::string& schema, const std::string& name, const std::string& objectType)
{
	assert(connectId > 0 && !schema.empty() && !name.empty());
	try {
		//sql
		sql::SQLString sql = fmt::format("DROP {} IF EXISTS `{}`", objectType, name);
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

UserRoutine UserSchemaObjectRepository::toUserRoutine(sql::ResultSet* rs)
{
	UserRoutine result;
	result.catalog = rs->getString("CATALOG").asStdString();
	result.schema = rs->getString("SCHEMA").asStdString();
	result.name = StringUtil::converFromUtf8(rs->getString("NAME").asStdString());
	result.ddl = StringUtil::converFromUtf8(rs->getString("DDL").asStdString());
	result.objectType = rs->getString("OBJECT_TYPE");
	
	return result;
}

UserTrigger UserSchemaObjectRepository::toUserTriggerDetail(sql::ResultSet* rs)
{
	UserTrigger result;
	result.catalog = rs->getString("TRIGGER_CATALOG").asStdString();
	result.schema = rs->getString("TRIGGER_SCHEMA").asStdString();
	result.name = StringUtil::converFromUtf8(rs->getString("TRIGGER_NAME").asStdString());
	result.ddl = StringUtil::converFromUtf8(rs->getString("ACTION_STATEMENT").asStdString());
	result.createTime = rs->getString("CREATED").asStdString();
	result.actionSchema = rs->getString("EVENT_OBJECT_SCHEMA").asStdString();
	result.actionTable = rs->getString("EVENT_OBJECT_TABLE").asStdString();
	result.actionTiming = rs->getString("ACTION_TIMING").asStdString();
	return result;
}
