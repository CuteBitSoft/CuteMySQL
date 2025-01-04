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
#include "utils/StringUtil.h"

UserTableList UserTableRepository::getAll(uint64_t connectId, const std::string& schema)
{
	assert(connectId > 0 && !schema.empty());
    UserTableList result;

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

UserTableList UserTableRepository::getAllDetailList(uint64_t connectId, const std::string& schema)
{
	assert(connectId > 0 && !schema.empty());
	UserTableList result;

	try {
		sql::SQLString sql = "SELECT * FROM `information_schema`.`TABLES` WHERE `TABLE_SCHEMA`=?";
		if (StringUtil::tolower(schema) == "information_schema") {
			sql.append(" AND `TABLE_TYPE`='SYSTEM VIEW'");
		} else {
			sql.append(" AND `TABLE_TYPE`='BASE TABLE'");
		}
		auto connect = getUserConnect(connectId);
		std::unique_ptr<sql::PreparedStatement> stmt(connect->prepareStatement(sql));
		stmt->setString(1, schema);
		std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery());
		while (resultSet->next()) {
			UserTable item = toUserTableDetail(resultSet.get());
			result.push_back(item);
		}
		stmt->close();
		resultSet->close();

		return result;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to getAllDetailList, code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}

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
	result.schema = StringUtil::converFromUtf8(rs->getString("TABLE_SCHEM").asStdString());
	result.name = StringUtil::converFromUtf8(rs->getString("TABLE_NAME").asStdString());
	result.tblName = result.name;
	result.type = StringUtil::converFromUtf8(rs->getString("TABLE_TYPE").asStdString());
	result.comment = StringUtil::converFromUtf8(rs->getString("REMARKS").asStdString());
	
	return result;
}

UserTable UserTableRepository::toUserTableDetail(sql::ResultSet* rs)
{
	UserTable result;
	result.catalog = rs->getString("TABLE_CATALOG").asStdString();
	result.schema = StringUtil::converFromUtf8(rs->getString("TABLE_SCHEMA").asStdString());
	result.name = StringUtil::converFromUtf8(rs->getString("TABLE_NAME").asStdString());
	result.tblName = result.name;
	result.type = StringUtil::converFromUtf8(rs->getString("TABLE_TYPE").asStdString());
	result.engine = rs->isNull("ENGINE") ? "" : rs->getString("ENGINE").asStdString();
	result.version = rs->isNull("VERSION") ? 0 : rs->getInt64("VERSION");
	result.autoIncVal = rs->isNull("AUTO_INCREMENT") ? 0 : rs->getUInt64("AUTO_INCREMENT");
	result.dataLength = rs->isNull("DATA_LENGTH") ? 0 : rs->getUInt64("DATA_LENGTH");
	result.rowFormat = rs->isNull("ROW_FORMAT") ? "" : rs->getString("ROW_FORMAT").asStdString();
	result.collaction = rs->isNull("TABLE_COLLATION") ? "" : rs->getString("TABLE_COLLATION").asStdString();
	result.options = rs->isNull("CREATE_OPTIONS") ? "" : rs->getString("CREATE_OPTIONS").asStdString();
	result.rows = rs->isNull("TABLE_ROWS") ? 0 : rs->getUInt64("TABLE_ROWS");
	result.createTime = rs->isNull("CREATE_TIME") ? "" : rs->getString("CREATE_TIME").asStdString();
	result.updateTime = rs->isNull("UPDATE_TIME") ? "" : rs->getString("UPDATE_TIME").asStdString();
	result.checkTime = rs->isNull("CHECK_TIME") ? "" : rs->getString("CHECK_TIME").asStdString();
	result.comment = StringUtil::converFromUtf8(rs->getString("TABLE_COMMENT").asStdString());
	
	return result;
}
