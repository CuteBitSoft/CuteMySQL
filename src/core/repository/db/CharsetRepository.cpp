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
 * @file   CharsetRepository.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-01
 *********************************************************************/
#include "CharsetRepository.h"

CharsetInfoList CharsetRepository::getAll(uint64_t connectId)
{
	CharsetInfoList result;

	try {
		sql::SQLString sql = "SHOW CHARACTER SET";
		auto connect = getUserConnect(connectId);
		std::unique_ptr<sql::Statement> stmt(connect->createStatement());
		std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery(sql));
		while (resultSet->next()) {
			CharsetInfo item = toCharsetInfo(resultSet.get());
			result.push_back(item);
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

    return result;
}

CharsetInfo CharsetRepository::toCharsetInfo(sql::ResultSet* rs)
{
	CharsetInfo result;
	result.name = rs->getString("Charset");
	result.defaultCollation = rs->getString("Default collation");
	result.description = rs->getString("Description");
	result.maxLen = rs->getInt("Maxlen");
	return result;
}
