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
 * @file   QTreeItemData.h
 * @brief  TreeCtrl data item class
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-14
 *********************************************************************/
#pragma once
#include <wx/treebase.h>
#include "core/entity/Enum.h"

template <typename T>
class QTreeItemData : public wxTreeItemData
{
public:
	QTreeItemData();
	~QTreeItemData();
	QTreeItemData(uint16_t _dataId);
	QTreeItemData(uint16_t _dataId, T* _dataPtr, TreeObjectType _type = TreeObjectType::ROOT);

	uint64_t getDataId() { return dataId; }
	void setDataId(uint16_t _dataId) { dataId = _dataId; }

	T* getDataPtr() { return dataPtr; }
	void setDataPtr(T* _dataPtr) { dataPtr = _dataPtr; }

	int getType() { return type; }
	void setType(int val) { type = val; }
private:
	uint16_t dataId;
	T * dataPtr;
	TreeObjectType type;
};

template <typename T>
QTreeItemData<T>::QTreeItemData() : wxTreeItemData(), dataId(0), dataPtr(nullptr)
{
}

template <typename T>
QTreeItemData<T>::~QTreeItemData()
{
	if (dataPtr) {
		delete dataPtr;
		dataPtr = nullptr;
	}
}

template <typename T>
QTreeItemData<T>::QTreeItemData(uint16_t _dataId) : wxTreeItemData(), dataId(_dataId), dataPtr(nullptr)
{
}

template <typename T>
QTreeItemData<T>::QTreeItemData(uint16_t _dataId, T* _dataPtr, TreeObjectType _type)
	: wxTreeItemData(), dataId(_dataId), dataPtr(_dataPtr), type(_type)
{
}
