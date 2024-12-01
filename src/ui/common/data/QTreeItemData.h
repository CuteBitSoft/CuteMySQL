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
#include "QClientData.h"
#include "core/entity/Enum.h"

template <typename T>
class QTreeItemData : public wxTreeItemData, public QClientData<T>
{
public:
	QTreeItemData();
	~QTreeItemData();
	QTreeItemData(uint16_t _dataId);
	QTreeItemData(uint16_t _dataId, T* _dataPtr, TreeObjectType _type = TreeObjectType::ROOT);

	int getType() { return type; }
	void setType(int val) { type = val; }
private:
	TreeObjectType type;
};

template <typename T>
QTreeItemData<T>::QTreeItemData() : wxTreeItemData(), QClientData<T>()
{
}

template <typename T>
QTreeItemData<T>::~QTreeItemData()
{
	
}

template <typename T>
QTreeItemData<T>::QTreeItemData(uint16_t _dataId) : wxTreeItemData(), QClientData<T>(_dataId)
{
}

template <typename T>
QTreeItemData<T>::QTreeItemData(uint16_t _dataId, T* _dataPtr, TreeObjectType _type)
	: wxTreeItemData(), QClientData<T>(_dataId, _dataPtr), type(_type)
{
}
