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
			std::list<sql::SQLString>({"VIEW"}) : std::list<sql::SQLString>({ "TABLE"});
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getTables(catalog, schema, "%", types));
		bool isOk = resultSet->first();
		while (resultSet->next()) {
			if (resultSet->getString(2) != schema || 
				(schema != "information_schema" && resultSet->getString(2) == "VIEW")) {
				continue;
			}
			UserTable item = toUserTable(resultSet.get());
			result.push_back(item);
			isOk = resultSet->next();
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

UserTable UserTableRepository::toUserTable(sql::ResultSet* rs)
{
	UserTable result;
	result.catalog = rs->getString(1).asStdString();
	result.schema = rs->getString(2).asStdString();
	result.name = rs->getString(3).asStdString();
	result.tblName = rs->getString(3).asStdString();
	result.type = rs->getString(4).asStdString();
	result.comment = rs->getString(5).asStdString();
	
	return result;
}
