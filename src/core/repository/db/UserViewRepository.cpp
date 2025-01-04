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
#include "utils/StringUtil.h"

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

UserViewList UserViewRepository::getAllDetailList(uint64_t connectId, const std::string& schema)
{
	assert(connectId > 0 && !schema.empty());
	UserViewList result;

	try {
		sql::SQLString sql = "SELECT * FROM `information_schema`.`TABLES` WHERE `TABLE_SCHEMA`=? AND `TABLE_TYPE`='VIEW'";
		
		auto connect = getUserConnect(connectId);
		std::unique_ptr<sql::PreparedStatement> stmt(connect->prepareStatement(sql));
		stmt->setString(1, schema);
		std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery());
		while (resultSet->next()) {
			UserView item = toUserViewDetail(resultSet.get());
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
	return getObjectDDL(connectId, schema, name, "VIEW");
}

UserView UserViewRepository::toUserView(sql::ResultSet* rs)
{
	UserView result;
	result.catalog = rs->getString("TABLE_CAT").asStdString();
	result.schema = StringUtil::converFromUtf8(rs->getString("TABLE_SCHEM").asStdString());
	result.name = StringUtil::converFromUtf8(rs->getString("TABLE_NAME").asStdString());
	result.tblName = result.name;
	result.type = rs->getString("TABLE_TYPE").asStdString();
	result.comment = StringUtil::converFromUtf8(rs->getString("REMARKS").asStdString());
	
	return result;
}

UserView UserViewRepository::toUserViewDetail(sql::ResultSet* rs)
{
	UserView result;
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
