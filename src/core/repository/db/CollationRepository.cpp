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
 * @file   CollationRepository.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-01
 *********************************************************************/
#include "CollationRepository.h"

CollationInfoList CollationRepository::getAll(uint64_t connectId, const std::string& charset)
{
	CollationInfoList result;

    try {
		sql::SQLString sql = "SHOW COLLATION WHERE Charset = ?";
		auto connect = getUserConnect(connectId);
		std::unique_ptr<sql::PreparedStatement> stmt(connect->prepareStatement(sql));
		// bind value
		stmt->setString(1, charset);
		std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery());
		while (resultSet->next()) {
			CollationInfo item = toCollationInfo(resultSet.get());
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

CollationInfo CollationRepository::toCollationInfo(sql::ResultSet* rs)
{
	CollationInfo result;
	result.id = rs->getInt("Id");
	result.name = rs->getString("Collation");
	result.charset = rs->getString("Charset");
	result.defaultVal = rs->getString("Default");
	result.compiled = rs->getString("Compiled");
	result.sortLen = rs->getInt("Sortlen");

	return result;
}
