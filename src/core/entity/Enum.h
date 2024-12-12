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
 * @file   Enum.h
 * @brief  Public Enum type
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-23
 *********************************************************************/
#pragma once

/**
 * Database LeftTreeView use object for wxTreeCtrl.ClientData.
 */
typedef enum {
	ROOT,
	CONNECTION,
	SCHEMA,
	TABLES_FOLDER,
	TABLE,
	TABLE_COLUMNS_FOLDER,
	TABLE_COLUMN,
	TABLE_INDEXES_FOLDER,
	TABLE_INDEX,
	VIEWS_FOLDER,
	VIEW,
	TRIGGERS_FOLDER,
	TRIGGER,
	STORE_PROCEDURE_FOLDER,
	STORE_PROCEDURE,
	FUNCTIONS_FOLDER,
	FUNCTION,
	EVENTS_FOLDER,
	EVENT,
	LOADING,
	ROUTINES
} TreeObjectType;


typedef enum {
	CONNECT_CREATE,
	CONNECT_MANAGE
} ConnectType;


typedef enum {
	DATABASE_CREATE,
	DATABASE_ALTER
} DatabaseType;


typedef enum {
	DUPLICATE_VIEW,
	DUPLICATE_STORE_PROCEDURE,
	DUPLICATE_FUNCTION,
	DUPLICATE_TRIGGER,
	DUPLICATE_EVENT,
} DuplicateObjectType;