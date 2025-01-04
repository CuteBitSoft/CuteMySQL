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
 * @file   UserSqlExecutorRepository.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-01
 *********************************************************************/
#include "UserSqlExecutorRepository.h"
#include <cassert>
#include <memory>
#include "utils/ThreadUtil.h"

sql::ResultSet * UserSqlExecutorRepository::executeQuery(uint64_t connectId, const std::string& schema, const std::string& sql)
{
    assert(connectId > 0  && !sql.empty());
	try {
		beginPerfTime(); // for performance - begin
		auto connect = getUserConnect(connectId);
		if (!schema.empty()) {
			connect->setSchema(schema);
		}
		
		std::unique_ptr<sql::Statement> stmt(connect->createStatement());
		//stmt->execute("set names 'utf8'; ");
		sql::ResultSet * resultSet = stmt->executeQuery(sql);
		stmt->close();
		endPerfTime(); // for performance - end
		return resultSet;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Repository Error, code:{}, error:{}", code, ex.what());
		endPerfTime(); // for performance - end
		throw ex;
	}
}


bool UserSqlExecutorRepository::execute(uint64_t connectId, const std::string& schema, const std::string& sql)
{
    assert(connectId > 0  && !sql.empty());
	try {
		beginPerfTime(); // for performance - begin
		auto connect = getUserConnect(connectId);
		if (!schema.empty()) {
			connect->setSchema(schema);
		}
		
		std::unique_ptr<sql::Statement> stmt(connect->createStatement());

		bool ret = stmt->execute(sql);
		stmt->close();
		endPerfTime(); // for performance - end
		return true;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Repository Error, code:{}, error:{}", code, ex.what());
		endPerfTime(); // for performance - end
		throw ex;
	}

	return false;
}



const PerfTime& UserSqlExecutorRepository::getPerfTime() const
{
	uint32_t threadId = ThreadUtil::currentThreadId();
	auto iter = perfTimes.find(threadId);
	assert(iter != perfTimes.end());

	return perfTimes.at(threadId);
}

void UserSqlExecutorRepository::endPerfTime()
{
	uint32_t threadId = ThreadUtil::currentThreadId();
	auto iter = perfTimes.find(threadId);
	if (iter == perfTimes.end()) {
		PerfTime perfTime;
		perfTime.begin = std::chrono::high_resolution_clock::now();	
		perfTimes[threadId] = perfTime;
	}

	auto& ptime = perfTimes[threadId];
	ptime.end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(ptime.end - ptime.begin);
	ptime.elapsedMicroSeconds = elapsed.count();
}

void UserSqlExecutorRepository::beginPerfTime()
{
	uint32_t threadId = ThreadUtil::currentThreadId();
	auto iter = perfTimes.find(threadId);
	if (iter == perfTimes.end()) {
		PerfTime perfTime;
		perfTime.begin = std::chrono::high_resolution_clock::now();
		perfTimes[threadId] = perfTime;
	} else {
		iter->second.begin = std::chrono::high_resolution_clock::now();
	}

}
