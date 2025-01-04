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

 * @file   ObjectsPageSupplier.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-10-30
 *********************************************************************/
#include "ObjectsPageSupplier.h"
#include "utils/StringUtil.h"
#include "utils/ResourceUtil.h"
#include "core/common/Lang.h"

std::vector<std::pair<wxString, int>> ObjectsPageSupplier::tableColumns;
std::vector<std::pair<wxString, int>> ObjectsPageSupplier::viewColumns;
std::vector<std::pair<wxString, int>> ObjectsPageSupplier::routineColumns;
std::vector<std::pair<wxString, int>> ObjectsPageSupplier::triggerColumns;
std::vector<std::pair<wxString, int>> ObjectsPageSupplier::eventColumns;

ObjectsPageSupplier::ObjectsPageSupplier()
{
	tableColumns = { 
		{S("name"), 200}, 
		{S("auto-inc-val"), 100}, 
		{S("create-time"), 150}, 
		{S("data-length"), 100}, 
		{S("engine"), 100}, 
		{S("rows"), 100}, 
		{S("comment"), 300} 
	};

	viewColumns = {
		{S("name"), 200}, 
		{S("allow-alter"), 100},
		{S("create-time"), 150},
		{S("last-altered"), 150}
	};

	routineColumns = {
		{S("name"), 200}, 
		{S("create-time"), 150}, 
		{S("last-altered"), 150},
		{S("comment"), 300}
	};

	triggerColumns = {
		{S("name"), 200}, 
		{S("action-schema"), 100},
		{S("action-table"), 100},
		{S("action-timing"), 100},
		{S("create-time"), 150}, 
		{S("last-altered"), 150}
	};

	eventColumns = {
		{S("name"), 200}, 
		{S("event-type"), 150}, 
		{S("status"), 150}, 
		{S("execute-at"), 150},
		{S("last-executed"), 150},
		{S("create-time"), 150}, 
		{S("last-altered"), 150}, 
		
	};
}


ObjectsPageSupplier::~ObjectsPageSupplier()
{
}

const std::vector<std::pair<wxString, int>>& ObjectsPageSupplier::getColumns() const
{
	switch (runtimeObjectType)
	{
	case VIEW_OBJECTS:
		return viewColumns;
	case PROCEDURE_OBJECTS:
	case FUNCTION_OBJECTS:
		return routineColumns;
	case TRIGGER_OBJECTS:
		return triggerColumns;
	case EVENT_OBJECTS:
		return eventColumns;
	case TABLE_OBJECTS:
	default:
		return tableColumns;
	}
}

UserConnect ObjectsPageSupplier::getUserConnect()
{
	return connectService->getUserConnect(runtimeUserConnectId);
}

UserTableList ObjectsPageSupplier::getUserTables()
{
	return metadataService->getDetailUserTables(runtimeUserConnectId, runtimeSchema);
}

UserViewList ObjectsPageSupplier::getUserViews()
{
	return metadataService->getDetailUserViews(runtimeUserConnectId, runtimeSchema);
}

UserProcedureList ObjectsPageSupplier::getUserProcedures()
{
	return metadataService->getDetailUserProcedures(runtimeUserConnectId, runtimeSchema);
}

UserFunctionList ObjectsPageSupplier::getUserFunctions()
{
	return metadataService->getDetailUserFunctions(runtimeUserConnectId, runtimeSchema);
}

UserTriggerList ObjectsPageSupplier::getUserTriggers()
{
	return metadataService->getDetailUserTriggers(runtimeUserConnectId, runtimeSchema);
}

UserEventList ObjectsPageSupplier::getUserEvents()
{
	return metadataService->getDetailUserEvents(runtimeUserConnectId, runtimeSchema);
}

