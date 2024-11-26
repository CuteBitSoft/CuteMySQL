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
 * @file   RightWorkView.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-21
 *********************************************************************/
#pragma once
#include "ui/common/panel/QPanel.h"
#include "ui/database/supplier/DatabaseSupplier.h"
class RightWorkView : public QPanel<DatabaseSupplier>
{
	DECLARE_EVENT_TABLE()
public:
	RightWorkView();
	~RightWorkView();
private:
	virtual void init();
	virtual void createControls();
};

