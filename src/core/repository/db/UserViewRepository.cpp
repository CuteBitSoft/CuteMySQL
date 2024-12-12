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
 * @file   UserViewRepository.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-25
 *********************************************************************/
#include "UserViewRepository.h"
#include <list>

UserViewList UserViewRepository::getAll(uint64_t connectId, const std::string& schema)
{
    assert(connectId > 0 && !schema.empty());
	UserViewList result;
	try {
		std::list<sql::SQLString> types{"VIEW"};
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getTables(catalog, schema, "%", types));
		while (resultSet->next()) {
			if (schema == "information_schema" && resultSet->getString("TABLE_TYPE") == "VIEW") {
				continue;
			}
			UserView item = toUserView(resultSet.get());
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

bool UserViewRepository::remove(uint64_t connectId, const std::string& schema, const std::string& name)
{
	assert(connectId > 0 && !schema.empty() && !name.empty());
	
	try {
		sql::SQLString sql = fmt::format("DROP VIEW IF EXISTS `{}`", name);
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

UserView UserViewRepository::get(uint64_t connectId, const std::string& schema, const std::string& name)
{
	assert(connectId > 0 && !schema.empty() && !name.empty());
	UserView result;
	
	try {
		std::list<sql::SQLString> types{"VIEW"};
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getTables(catalog, schema, name, types));
		if (resultSet->next()) {
			result = toUserView(resultSet.get());
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

std::string UserViewRepository::getDDL(uint64_t connectId, const std::string& schema, const std::string& name)
{
	assert(connectId > 0 && !schema.empty() && !name.empty());
	std::string result;
	try {
		sql::SQLString sql = "SHOW CREATE VIEW ";
		sql.append(" `").append(name).append("`");
		auto connect = getUserConnect(connectId);
		connect->setSchema(schema);
		std::unique_ptr<sql::Statement> stmt(connect->createStatement());
		std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery(sql));
		if (resultSet->next()) {
			result = resultSet->getString("Create View").asStdString();
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
}

UserView UserViewRepository::toUserView(sql::ResultSet* rs)
{
	UserView result;
	result.catalog = rs->getString("TABLE_CAT").asStdString();
	result.schema = rs->getString("TABLE_SCHEM").asStdString();
	result.name = rs->getString("TABLE_NAME").asStdString();
	result.tblName = result.name;
	result.type = rs->getString("TABLE_TYPE").asStdString();
	result.comment = rs->getString("REMARKS").asStdString();
	
	return result;
}
