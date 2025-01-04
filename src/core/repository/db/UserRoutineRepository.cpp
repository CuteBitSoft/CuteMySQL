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
 * @file   UserRoutineRepository.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-25
 *********************************************************************/
#include "UserRoutineRepository.h"
#include <list>
#include <cassert>
#include <spdlog/fmt/fmt.h>

UserRoutineList UserRoutineRepository::getAllByType(uint64_t connectId, const std::string& schema, RoutineType type)
{
	assert(connectId && !schema.empty());
    UserRoutineList result;	
	try {
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getProcedures(catalog, schema, "%"));
		
		while (resultSet->next()) {
			if (resultSet->getInt("PROCEDURE_TYPE") != type) {
				continue;
			}
			UserRoutine item = toUserRoutine(resultSet.get());
			result.push_back(item);
		}
		resultSet->close();
		return result;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}

    return result;
}

UserRoutineList UserRoutineRepository::getAllDetailListByType(uint64_t connectId, const std::string& schema, RoutineType type)
{
	assert(connectId > 0 && !schema.empty());
	UserRoutineList result;

	try {
		sql::SQLString sql = "SELECT * FROM `information_schema`.`ROUTINES` WHERE `ROUTINE_SCHEMA`=?";
		if (type == RoutineType::ROUTINE_PROCEDURE) {
			sql.append(" AND `ROUTINE_TYPE`='PROCEDURE'");
		} else if (type == RoutineType::ROUTINE_FUNCTION) {
			sql.append(" AND `ROUTINE_TYPE`='FUNCTION'");
		}
		auto connect = getUserConnect(connectId);
		std::unique_ptr<sql::PreparedStatement> stmt(connect->prepareStatement(sql));
		stmt->setString(1, schema);
		std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery());
		while (resultSet->next()) {
			UserRoutine item = toUserRoutineDetail(resultSet.get());
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

bool UserRoutineRepository::remove(uint64_t connectId, const std::string& schema, const std::string& name, RoutineType type)
{
	assert(connectId > 0 && !schema.empty() && !name.empty());
	
	std::string routineType;
	if (type == RoutineType::ROUTINE_PROCEDURE) {
		routineType = "PROCEDURE";
	} else if (type == RoutineType::ROUTINE_FUNCTION) {
		routineType = "FUNCTION";
	} else {
		return false;
	}
	try {
		
		sql::SQLString sql = fmt::format("DROP {} IF EXISTS `{}`", routineType, name);
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
		Q_ERROR("code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}

	return false;
}

std::vector<std::string> UserRoutineRepository::getSysFunctions(uint64_t connectId)
{
	std::vector<std::string> result;

	try {
		
		sql::SQLString sql = "SHOW FUNCTION STATUS";
		auto connect = getUserConnect(connectId);
		std::unique_ptr<sql::Statement> stmt(connect->createStatement());
		std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery(sql));
		while (resultSet->next()) {
			result.push_back(resultSet->getString("Name").asStdString());
		}
		stmt->close();
		resultSet->close();
	}
	catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}
	return result;
}

UserRoutine UserRoutineRepository::toUserRoutine(sql::ResultSet* rs)
{
	UserRoutine result;
	result.catalog = rs->getString("PROCEDURE_CAT").asStdString();
	result.schema = StringUtil::converFromUtf8(rs->getString("PROCEDURE_SCHEM").asStdString());
	result.name = StringUtil::converFromUtf8(rs->getString("PROCEDURE_NAME").asStdString());
	result.objectType = "routine";
	result.remarks = StringUtil::converFromUtf8(rs->getString("REMARKS").asStdString());
	result.type = rs->getInt("PROCEDURE_TYPE");
	
	return result;
}

UserRoutine UserRoutineRepository::toUserRoutineDetail(sql::ResultSet* rs)
{
	UserRoutine result;

	result.catalog = rs->getString("ROUTINE_CATALOG").asStdString();
	result.schema = StringUtil::converFromUtf8(rs->getString("ROUTINE_SCHEMA").asStdString());
	result.name = StringUtil::converFromUtf8(rs->getString("ROUTINE_NAME").asStdString());
	result.objectType = rs->getString("ROUTINE_TYPE").asStdString();
	result.type = result.objectType == "PROCEDURE" ? 0 : 1;
	result.remarks = StringUtil::converFromUtf8(rs->getString("ROUTINE_COMMENT").asStdString());
	result.ddl = StringUtil::converFromUtf8(rs->getString("ROUTINE_DEFINITION").asStdString());
	result.createTime = rs->getString("CREATED").asStdString();
	result.updateTime = rs->getString("LAST_ALTERED").asStdString();
	return result;
}
