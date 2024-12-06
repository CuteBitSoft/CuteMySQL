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
 * @file   QClientData.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-28
 *********************************************************************/
#pragma once

template<typename T>
class QClientData : public wxClientData {
public :
	QClientData();
	~QClientData();

	QClientData(uint16_t _dataId);
	QClientData(uint16_t _dataId, T* _dataPtr);


	uint64_t getDataId() { return dataId; }
	void setDataId(uint64_t _dataId) { dataId = _dataId; }

	T* getDataPtr() { return dataPtr; }
	void setDataPtr(T* _dataPtr) { dataPtr = _dataPtr; }
protected:
	uint64_t dataId;
	T* dataPtr;
};

template<typename T>
QClientData<T>::QClientData(uint16_t _dataId, T* _dataPtr) : wxClientData(), dataId(_dataId), dataPtr(_dataPtr)
{

}

template<typename T>
QClientData<T>::QClientData(uint16_t _dataId): wxClientData(), dataId(_dataId), dataPtr(nullptr)
{

}

template<typename T>
QClientData<T>::~QClientData()
{
	if (dataPtr) {
		delete dataPtr;
		dataPtr = nullptr;
	}
}

template<typename T>
QClientData<T>::QClientData():wxClientData(), dataId(0), dataPtr(nullptr)
{

}
