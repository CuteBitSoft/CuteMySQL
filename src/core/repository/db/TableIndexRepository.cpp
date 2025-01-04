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
 * @file   TableIndexRepository.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-25
 *********************************************************************/
#include "TableIndexRepository.h"
#include <list>
#include "utils/StringUtil.h"

IndexInfoList TableIndexRepository::getAll(uint64_t connectId, const std::string& schema, const std::string & tableName)
{
    IndexInfoList result;

    if (connectId <= 0 || schema.empty()) {
		return result;
	}
	
	try {
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getIndexInfo(catalog, schema, tableName, false, true));
		while (resultSet->next()) {
			
			IndexInfo item = toIndexInfo(resultSet.get());
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

bool TableIndexRepository::remove(uint64_t connectId, const std::string& schema, const std::string& tableName, const std::string& indexName)
{
	assert(connectId > 0 && !schema.empty() && !tableName.empty() && !indexName.empty());
	
	try {
		sql::SQLString sql = fmt::format("ALTER TABLE `{}` DROP INDEX `{}`", tableName, indexName);
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

IndexInfo TableIndexRepository::toIndexInfo(sql::ResultSet* rs)
{
	IndexInfo result;
	result.catalog = rs->getString("TABLE_CAT").asStdString();
	result.schema = StringUtil::converFromUtf8(rs->getString("TABLE_SCHEM").asStdString());
	result.table = StringUtil::converFromUtf8(rs->getString("TABLE_NAME").asStdString());
	result.name = StringUtil::converFromUtf8(rs->getString("INDEX_NAME").asStdString());
	result.type = rs->getString("TYPE").asStdString();
	result.columns = StringUtil::converFromUtf8(rs->getString("COLUMN_NAME").asStdString());
	result.un = rs->getString("NON_UNIQUE") == "0";
	result.pk = result.name == "PRIMARY"; 
	
	return result;
}
