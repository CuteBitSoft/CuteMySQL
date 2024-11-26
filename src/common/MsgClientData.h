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
 * @file   MsgClientData.h
 * @brief  Client data for MsgDispatcher 
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-15
 *********************************************************************/
#pragma once
#include <wx/clntdata.h>

class MsgClientData : public wxClientData 
{
public:
	MsgClientData() : wxClientData(), msgId(0),dataPtr(0) {}
	~MsgClientData() {};

	MsgClientData(uint64_t _msgId) : wxClientData(), msgId(_msgId), dataPtr(0) {}
	MsgClientData(uint64_t _msgId, uint64_t _dataPtr) : wxClientData(), msgId(_msgId), dataPtr(_dataPtr) {}

	void setMsgId(uint64_t msgId) { this->msgId = msgId; }
	uint64_t getMsgId() { return msgId; }

	void setDataPtr(uint64_t dataPtr) { this->dataPtr = dataPtr; }
	uint64_t getDataPtr() { return dataPtr; }
private:
	uint64_t msgId;
	uint64_t dataPtr;
};
