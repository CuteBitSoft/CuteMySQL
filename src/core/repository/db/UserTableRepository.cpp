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
 * @file   UserTableRepository.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-25
 *********************************************************************/
#include "UserTableRepository.h"
#include <list>

UserTableList UserTableRepository::getAll(uint64_t connectId, const std::string& schema)
{
    UserTableList result;

    if (connectId <= 0 || schema.empty()) {
		return result;
	}
	
	try {
		std::list<sql::SQLString> types = (schema == "information_schema") ? 
			std::list<sql::SQLString>({"TEMPORARY TABLE", "VIEW"}) : std::list<sql::SQLString>({"TABLE", "SYSTEM TABLE"});
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getTables(catalog, schema, "%", types));
		while (resultSet->next()) {
			UserTable item = toUserTable(resultSet.get());
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

bool UserTableRepository::remove(uint64_t connectId, const std::string& schema, const std::string& name)
{
	assert(connectId > 0 && !schema.empty() && !name.empty());
	try {
		sql::SQLString sql = fmt::format("DROP TABLE IF EXISTS `{}`", name);
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

bool UserTableRepository::has(uint64_t connectId, const std::string& schema, const std::string& name)
{
	if (connectId <= 0 || schema.empty()) {
		return false;
	}
	
	try {
		std::list<sql::SQLString> types = (schema == "information_schema") ? 
			std::list<sql::SQLString>({"TEMPORARY TABLE", "VIEW"}) : std::list<sql::SQLString>({"TABLE", "SYSTEM TABLE"});
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getTables(catalog, schema, name, types));
		bool hasFound = false;
		while (resultSet->next()) {
			UserTable item = toUserTable(resultSet.get());
			if (item.name == name) {
				hasFound = true;
				break;
			}
		}
		resultSet->close();
		return hasFound;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to has,code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}

    return false;
}

UserTable UserTableRepository::get(uint64_t connectId, const std::string& schema, const std::string& name)
{
	UserTable result;
	if (connectId <= 0 || schema.empty()) {
		return result;
	}
	
	try {
		std::list<sql::SQLString> types = (schema == "information_schema") ? 
			std::list<sql::SQLString>({"TEMPORARY TABLE", "VIEW"}) : std::list<sql::SQLString>({"TABLE", "SYSTEM TABLE"});
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getTables(catalog, schema, name, types));
		while (resultSet->next()) {
			UserTable item = toUserTable(resultSet.get());
			if (item.name == name) {
				result = item;
				break;
			}
		}
		resultSet->close();
		return result;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to has,code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}

    return result;
}

UserTable UserTableRepository::toUserTable(sql::ResultSet* rs)
{
	UserTable result;
	result.catalog = rs->getString("TABLE_CAT").asStdString();
	result.schema = rs->getString("TABLE_SCHEM").asStdString();
	result.name = rs->getString("TABLE_NAME").asStdString();
	result.tblName = result.name;
	result.type = rs->getString("TABLE_TYPE").asStdString();
	result.comment = rs->getString("REMARKS").asStdString();
	
	return result;
}
