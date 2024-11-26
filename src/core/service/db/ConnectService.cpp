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
 * @file   ConnectService.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-25
 *********************************************************************/
#include "ConnectService.h"
#include "utils/Log.h"

UserConnectList ConnectService::getAllUserConnects()
{
	return getRepository()->getAll();
}

UserConnect ConnectService::getUserConnect(uint64_t userConnectId)
{
	return getRepository()->getById(userConnectId);
}

uint64_t ConnectService::createUserConnect(const UserConnect& userConnect)
{
	auto connectId = getRepository()->create(userConnect);
	if (userConnect.sort == 0) {
		getRepository()->updateSort(connectId, connectId);
	}
	
	return connectId;
}

uint64_t ConnectService::updateUserConnect(const UserConnect& userConnect)
{
	getRepository()->update(userConnect);
	return userConnect.id;
}

void ConnectService::updateUserConnectSort(uint64_t userConnectId, uint64_t sort)
{
	getRepository()->updateSort(userConnectId, sort);
}

void ConnectService::removeUserConnect(uint64_t userConnectId)
{
	getRepository()->remove(userConnectId);
}

void ConnectService::testConnect(int64_t userConnectId)
{
	getRepository()->testUserConnect(userConnectId);
}

void ConnectService::connect(int64_t userConnectId)
{
	getRepository()->getUserConnect(userConnectId);
}
