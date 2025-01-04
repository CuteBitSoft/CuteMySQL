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
 * @file   TableColumnRepository.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-25
 *********************************************************************/
#include "TableColumnRepository.h"
#include <list>
#include <cassert>
#include "utils/StringUtil.h"

ColumnInfoList TableColumnRepository::getAll(uint64_t connectId, const std::string& schema, const std::string & tableName)
{
	assert(connectId > 0 && !schema.empty() && !tableName.empty());
    ColumnInfoList result;
	try {
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getColumns(catalog, schema, tableName, "%"));
		while (resultSet->next()) {
			
			ColumnInfo item = toColumnInfo(resultSet.get());
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

bool TableColumnRepository::remove(uint64_t connectId, const std::string& schema, const std::string& tableName, const std::string& columnName)
{
	assert(connectId > 0 && !schema.empty() && !tableName.empty() && !columnName.empty());
	
	try {
		sql::SQLString sql = fmt::format("ALTER TABLE `{}` DROP COLUMN `{}`", tableName, columnName);
		auto connect = getUserConnect(connectId);
		connect->setSchema(schema);
		std::unique_ptr<sql::Statement> stmt(connect->createStatement());

		stmt->execute(sql);
		stmt->close();
		return true;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to create, code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}

	return false;
}

ColumnInfo TableColumnRepository::toColumnInfo(sql::ResultSet* rs)
{
	ColumnInfo result;
	result.catalog = rs->getString("TABLE_CAT").asStdString();
	result.schema = StringUtil::converFromUtf8(rs->getString("TABLE_SCHEM").asStdString());
	result.table = StringUtil::converFromUtf8(rs->getString("TABLE_NAME").asStdString());
	result.name = StringUtil::converFromUtf8(rs->getString("COLUMN_NAME").asStdString());
	result.fullType = rs->getString("TYPE_NAME").asStdString();
	result.un = (result.fullType.find("UNSIGNED") != std::string::npos) ? 1 : 0;
	result.type = !result.un ? result.fullType : StringUtil::replace(result.fullType, " UNSIGNED", "");
	result.size = rs->getUInt64("COLUMN_SIZE");
	result.defVal = rs->getString("COLUMN_DEF").asStdString();
	result.isNullable = rs->getString("IS_NULLABLE") == "YES";
	result.remarks = StringUtil::converFromUtf8(rs->getString("REMARKS").asStdString());
	result.ai = rs->getString("IS_AUTOINCREMENT") == "YES";
	
	return result;
}
