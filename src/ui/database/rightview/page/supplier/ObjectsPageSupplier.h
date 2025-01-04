/*****************************************************************//**
 * Copyright 2023 Xuehan Qin 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and

 * limitations under the License.

 * @file   ObjectsPageSupplier.h
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-10-30
 *********************************************************************/
#pragma once
#include <wx/window.h>
#include "core/entity/Entity.h"
#include "ui/database/rightview/common/QPageSupplier.h"
#include "core/service/db/DatabaseService.h"
#include "core/service/db/MetadataService.h"
#include "core/service/db/ConnectService.h"

class ObjectsPageSupplier : public QPageSupplier<ObjectsPageSupplier> {
public:
	ObjectsPageSupplier();
	~ObjectsPageSupplier();
	
	const std::vector<std::pair<wxString, int>> & getColumns() const;

	ObjectsPageType runtimeObjectType;

	UserConnect getUserConnect();
	UserTableList getUserTables();
	UserViewList getUserViews();
	UserProcedureList getUserProcedures();
	UserFunctionList getUserFunctions();
	UserTriggerList getUserTriggers();
	UserEventList getUserEvents();
private:
	// objects columns
	static std::vector<std::pair<wxString, int>> tableColumns, viewColumns, routineColumns, triggerColumns, eventColumns;

	MetadataService* metadataService = MetadataService::getInstance();
	DatabaseService* databaseService = DatabaseService::getInstance();
	ConnectService* connectService = ConnectService::getInstance();
};
