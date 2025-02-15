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

 * @file   QueryPageSupplier.h
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-10-30
 *********************************************************************/
#pragma once
#include <wx/window.h>
#include "core/entity/Entity.h"
#include "ui/database/rightview/common/QPageSupplier.h"

class QueryPageSupplier : public QPageSupplier<QueryPageSupplier> {
public:
	QueryPageSupplier();
	~QueryPageSupplier();
	
	static const std::vector<std::string> sqlTags;
	static const std::list<std::tuple<int, std::string, std::string>> pragmas;

	// sql statements
	std::vector<std::string> sqlVector;

	// Using semicolons to separate a SQL statement becomes a member variable sqlVector
	void splitToSqlVector(std::string sql);

	// tables
	UserTableStrings & getCacheUserTableStrings(uint64_t connectId, const std::string & schema);
	void setCacheUserTableStrings(uint64_t connectId, const std::string & schema, UserTableStrings & tblStrs);

	// table columns
	Columns & getCacheTableColumns(uint64_t connectId, const std::string & schema,  const std::string & tblName);
	void setCacheTableColumns(uint64_t connectId,   const std::string & schema,  const std::string & tblName, const Columns & columns);

	std::string & getCacheUseSql() { return cacheUseSql; }
	void setCacheUseSql(const std::string & val) { cacheUseSql = val; }
	void clearCacheUseSql() { cacheUseSql.clear(); }

	
	wxWindow* getActiveResultTabPageHwnd() const { return activeResultTabPageHwnd; }
	void setActiveResultTabPageHwnd(wxWindow* val) { activeResultTabPageHwnd = val; }
private:
	// template params: first - connectid, second - schema , third - table strings 
	std::map<std::pair<uint64_t, std::string>, UserTableStrings> cacheUserTableMap;
	
	// template params:  first - connectId, second - schema.tblName, third - columns strings
	std::map<std::pair<uint64_t, std::string>, Columns> cacheTableColumnsMap;

	// 
	std::string cacheUseSql;

	// The HWND of active page in ResultTabView
	wxWindow * activeResultTabPageHwnd = nullptr;
};
