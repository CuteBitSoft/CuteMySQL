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

UserProcedureList UserSchemaObjectRepository::getAllByObjectType(uint64_t connectId, const std::string& schema, const std::string & objectType)
{
    UserProcedureList result;

    if (connectId <= 0 || schema.empty()) {
		return result;
	}
	
	try {
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		std::unique_ptr<sql::ResultSet> resultSet(connect->getMetaData()->getSchemaObjects(catalog, schema, objectType));
		while (resultSet->next()) {			
			UserProcedure item = toUserProcedure(resultSet.get());
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

UserProcedure UserSchemaObjectRepository::toUserProcedure(sql::ResultSet* rs)
{
	UserProcedure result;
	result.catalog = rs->getString("CATALOG").asStdString();
	result.schema = rs->getString("SCHEMA").asStdString();
	result.name = rs->getString("NAME").asStdString();
	result.ddl = rs->getInt("DDL");
	result.objectType = rs->getString("OBJECT_TYPE");
	
	return result;
}
