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
 * @file   ObjectsPageTableDelegate.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2025-01-03
 *********************************************************************/
#pragma once
#include "BaseObjectsDelegate.h"
#include "ui/database/rightview/page/supplier/ObjectsPageSupplier.h"
#include "ui/common/listview/QListView.h"

class ObjectsPageTableDelegate : public BaseObjectsDelegate<ObjectsPageTableDelegate>
{
public:
	virtual void loadObjects();
private:	
	virtual void createMenu();
};

