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

 * @file   ResultListPageDelegate.h
 * @brief  data list view adapter class of ResultListPage, get the data and showListView;
 * 
 * @author Xuehan Qin
 * @date   2023-05-28
 *********************************************************************/
#pragma once
#include <cstdint>
#include <string>
#include <list>
#include <tuple>
#include <mysql/jdbc.h>
#include <wx/listctrl.h>
#include "ui/common/delegate/QDelegate.h"
#include "ui/common/listview/QListView.h"
#include "ui/database/rightview/page/supplier/QueryPageSupplier.h"
#include "core/service/db/ExecutorService.h"
#include "core/service/db/DatabaseService.h"
#include "core/service/db/MetadataService.h"

/**
 * Define FilterTuple and DataFilters
 * that is the query condition vector of the filters that used in ResultFilterDialog to restore the filter condition.
 * the tuple param means 0-and/or, 1-column, 2-operator, 3-value
 */
typedef std::tuple<std::string, std::string, std::string, std::string> FilterTuple;
typedef std::vector<FilterTuple> DataFilters;

// query result type 
typedef enum {
	QUERY_SQL_RESULT,
	QUERY_TABLE_DATA
} ResultType;

#define TABLE_DATA_SETTING_PREFIX "table-data-"
class ResultListPageDelegate : public QDelegate<ResultListPageDelegate, QueryPageSupplier, QListView>
{
public:
	ResultListPageDelegate(wxWindow * parentWin, QListView * listView, QueryPageSupplier * supplier, ResultType resultType = QUERY_SQL_RESULT);
	~ResultListPageDelegate();

	int loadListView(uint64_t connectId, const std::string & schema, std::string & sql);
	
	// Add filters for result list
	int loadFilterListView();

	bool sortListView(int iItem);

	// virtual list data load
	//LRESULT fillDataInListViewSubItem(NMLVDISPINFO * pLvdi);

	RowItem getFirstSelectdRowItem();
	int getFirstSelectdIndex();
	DataList getSelectedDatas();
	int getSelectedItemCount();
	
	uint64_t getRuntimeUserDbId() { return runtimeUserConnectId; }
	void setRuntimeUserConnectId(uint64_t connectId);

	const UserTableStrings & getRuntimeTables();
	void setRuntimeTables(const UserTableStrings & val);
	const Columns & getRuntimeColumns() ;
	void setRuntimeColumns(const Columns & columns);
	const DataList & getRuntimeDatas();
	void setRuntimeDatas(const DataList & dataList);

	void addListViewChangeVal(SubItemValue &subItemVal);

	ColumnInfoList getRuntimeColumnInfos(const std::string & schema, const std::string & tblName);
	UserTable getRuntimeUserTable(const std::string & schema, const std::string & tblName);
	Columns getRuntimeValidFilterColumns();

	const DataFilters & getRuntimeFilters();
	void setRuntimeFilters(DataFilters & filters);
	void clearRuntimeFilters();
	bool isRuntimeFiltersEmpty();

	// copy data
	void copyAllRowsToClipboard();
	void copySelRowsToClipboard();
	void copyAllRowsAsSql();
	void copySelRowsAsSql();

	// system sys_init table key prefix
	void setSettingPrefix(std::string & prefix);

	void changeRuntimeDatasItem(int iItem, int iSubItem, std::string & origText, std::string & newText);
	void invalidateSubItem(int iItem, int iSubItem);

	// create/copy a new row operation
	void createNewRow();
	void copyNewRow();
	// save/delete/cancel
	bool save();
	bool remove(bool confirm=true);
	bool cancel();

	int removeRowFromDb(int nSelItem, RowItem & rowItem);

	bool isDirty();

	// query result info
	ResultInfo & getRuntimeResultInfo();
	void sendExecSqlMessage(ResultInfo & resultInfo, bool isWait = false);	
	std::string & getRuntimeSql() { return runtimeSql; }
private:
	ExecutorService * executorService = ExecutorService::getInstance();
	DatabaseService * databaseService = DatabaseService::getInstance();
	MetadataService * metadataService = MetadataService::getInstance();
	
	wxColour rowBkgColor1, rowBkgColor2, textColor;
	// the runtime variables
	uint64_t runtimeUserConnectId = 0;
	std::string originSql;
	std::string runtimeSql;

	UserTableStrings runtimeTables;
	Columns runtimeColumns;
	DataList runtimeDatas;   // runtime data(s) for showing list view
	std::vector<int> runtimeNewRows; // runtimeDatas index for create or copy a new row
	ResultInfo runtimeResultInfo;

	DataFilters runtimeFilters;

	ResultType resultType;
	std::string settingPrefix;

	void loadRuntimeTables(uint64_t connectId, const std::string & schema, const std::string & sql);
	void loadRuntimeHeader(sql::ResultSet * resultSet);
	void clearHeaderSorted(int notSelItem = -1);
	int loadRuntimeData(sql::ResultSet * resultSet);
	void displayRuntimeData();
	int loadRuntimeDataToList(sql::ResultSet * resultSet);	
	void loadLimitParams(LimitParams & limitParams);

	bool getIsChecked(int iItem);

	std::string buildRungtimeSqlWithFilters();

	//save datas
	bool saveChangeVals();
	bool saveNewRows();
	bool restoreChangeVals();
	void resetRuntimeResultInfo();
	void sortRuntimeDatas(int index, bool isDown);
};
