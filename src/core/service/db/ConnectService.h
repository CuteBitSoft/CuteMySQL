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
 * @file   ConnectService.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-03
 *********************************************************************/
#pragma once
#include "core/common/service/BaseService.h"
#include "core/repository/db/UserConnectRepository.h"

class ConnectService : public BaseService<ConnectService, UserConnectRepository>
{
public:
	// user db operations
	UserConnectList getAllUserConnects();
	uint64_t hasUserConnect(std::string& host, int port);
	bool hasUserConnect(uint64_t userDbId);
	UserConnect getUserConnect(uint64_t userConnectId);
	uint64_t createUserConnect(const UserConnect& userConnect);
	uint64_t updateUserConnect(const UserConnect& userConnect);
	void updateUserConnectSort(uint64_t userConnectId, uint64_t sort);
	void removeUserConnect(uint64_t userConnectId);
	bool activeUserConnect(uint64_t userConnectId);
	uint64_t copyUserConnect(uint64_t fromUserConnectId, const std::string& toConnectName);

	void testConnect(int64_t userConnectId);
	void connect(int64_t userConnectId);

};

