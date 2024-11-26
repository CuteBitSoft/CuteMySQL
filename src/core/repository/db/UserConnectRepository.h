﻿/*****************************************************************//**
 * Copyright 2024 Xuehan Qin 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and

 * limitations under the License.

 * @file   DbRepository.h
 * @brief  The table db repository
 * 
 * @author Xuehan Qin
 * @date   2023-05-19
 *********************************************************************/
#pragma once
#include <string>
#include "core/entity/Entity.h"
#include "core/common/repository/BaseUserRepository.h"

class UserConnectRepository : public BaseUserRepository<UserConnectRepository>
{
public:
	UserConnectRepository() {};
	~UserConnectRepository() {};

	uint64_t create(const UserConnect & item);
	void update(const UserConnect & item);
	void updateSort(uint64_t id, uint64_t sort);
	bool remove(uint64_t id);
	UserConnect getById(uint64_t id);
	UserConnectList  getAll();
private:
	void queryBind(SQLite::QSqlStatement &query, const UserConnect &item, bool isUpdate = false);
};
