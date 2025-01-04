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
 * @file   ResultListPageDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-22
 *********************************************************************/
#include "ResultListPageDelegate.h"
#include <algorithm>
#include <Strsafe.h>
#include <CommCtrl.h>
#include "common/AppContext.h"
#include "core/common/Lang.h"
#include "core/common/exception/QSqlExecuteException.h"
#include "core/service/system/SettingService.h"
#include "ui/common/msgbox/QAnimateBox.h"
#include "ui/common/msgbox/QConfirmBox.h"
#include "utils/SqlUtil.h"
#include "utils/ClipboardUtil.h"
#include "utils/PerformUtil.h"
#include "utils/SavePointUtil.h"

ResultListPageDelegate::ResultListPageDelegate(wxWindow* parentWin, QListView* listView, QueryPageSupplier* supplier, ResultType resultType)
	: rowBkgColor1(30, 31, 34, 30), rowBkgColor2(46, 46, 46, 43), textColor(223, 225, 229, 213)
{
	this->parentWin = parentWin;
	this->view = listView;
	this->resultType = resultType;
	this->supplier = supplier;
	if (this->resultType == QUERY_TABLE_DATA) {
		settingPrefix = TABLE_DATA_SETTING_PREFIX;
	}
}

ResultListPageDelegate::~ResultListPageDelegate()
{

}

int ResultListPageDelegate::loadListView(uint64_t connectId, const std::string & schema, std::string & sql)
{
	view->DeleteAllColumns();
	view->DeleteAllItems();
	runtimeTables.clear();
	runtimeDatas.clear();
	runtimeColumns.clear();
	runtimeFilters.clear();
	runtimeNewRows.clear();
	resetRuntimeResultInfo();

	runtimeUserConnectId = connectId;
	originSql = sql;
	runtimeSql = sql;

	if (sql.empty()) {
		return 0;
	}

	if (!SqlUtil::isPragmaStmt(originSql, false) && !SqlUtil::hasLimitClause(originSql)) {
		LimitParams limitParams;
		loadLimitParams(limitParams);
		if (limitParams.checked) {
			runtimeSql.append(" LIMIT ")
				.append(std::to_string(limitParams.offset))
				.append(",")
				.append(std::to_string(limitParams.rows));
		}		
	}
	runtimeResultInfo.connectId = connectId;
	runtimeResultInfo.schema = schema;
	runtimeResultInfo.sql = runtimeSql;
	auto bt = PerformUtil::begin();
	try {		
		std::unique_ptr<sql::ResultSet> resultSet(executorService->executeQuerySql(connectId, schema, runtimeSql));
		runtimeResultInfo.execTime = PerformUtil::elapsedMs(executorService->getPerfTime());
		loadRuntimeTables(connectId, schema, runtimeSql); 
		loadRuntimeHeader(resultSet.get());
		auto bt2 = PerformUtil::begin();
		int effectRows = loadRuntimeData(resultSet.get());
		
		runtimeResultInfo.effectRows = effectRows;	
		runtimeResultInfo.transferTime = PerformUtil::end(bt2);
		displayRuntimeData();
		//view->changeAllItemsCheckState();
		return runtimeResultInfo.effectRows;
	} catch (sql::SQLException &ex) {
		std::string _err = ex.what();
		Q_ERROR("query db has error:{}, msg:{}", ex.getErrorCode(), _err);		
		runtimeResultInfo.code = ex.getErrorCode();
		runtimeResultInfo.msg = _err;
		runtimeResultInfo.execTime = PerformUtil::end(bt);
		runtimeResultInfo.transferTime = PerformUtil::end(bt);
		throw QSqlExecuteException(std::to_string(ex.getErrorCode()), _err, runtimeSql);
	}
	
	return 0;
}

//int ResultListPageDelegate::loadFilterListView()
//{
//	view->DeleteAllItems();
//	view->clearChangeVals();
//	view->destroySubItemElems();
//	runtimeTables.clear();
//	runtimeDatas.clear();
//	runtimeNewRows.clear();
//	resetRuntimeResultInfo();
//
//	if (originSql.empty()) {
//		return 0;
//	}
//	runtimeSql = buildRungtimeSqlWithFilters();
//	if (runtimeSql.empty()) {
//		return 0;
//	}
//	runtimeResultInfo.sql = runtimeSql;
//	runtimeResultInfo.connectId = runtimeUserDbId;
//	auto bt = PerformUtil::begin();
//	try {		
//		QSqlStatement query = sqlService->tryExecuteSql(runtimeUserDbId, runtimeSql);
//		runtimeResultInfo.execTime = PerformUtil::end(bt);
//
//		loadRuntimeTables(runtimeUserDbId, runtimeSql); 		
//		clearHeaderSorted();
//		runtimeResultInfo.effectRows = loadRuntimeData(query);
//		runtimeResultInfo.transferTime = PerformUtil::end(bt);
//		view->changeAllItemsCheckState();
//		return runtimeResultInfo.effectRows;
//	} catch (SQLite::QSqlException &ex) {
//		std::string _err = ex.getErrorStr();
//		Q_ERROR("query db has error:{}, msg:{}", ex.getErrorCode(), _err);
//		runtimeResultInfo.code = ex.getErrorCode();
//		runtimeResultInfo.msg = _err;
//		runtimeResultInfo.execTime = PerformUtil::end(bt);
//		runtimeResultInfo.transferTime = PerformUtil::end(bt);
//		throw QSqlExecuteException(std::to_string(ex.getErrorCode()), _err, runtimeSql);
//	}
//	return 0;
//}


//bool ResultListPageDelegate::sortListView(int iSelItem)
//{
//	// 1. save the changes first
//	if (isDirty()) {
//		if (QMessageBox::confirm(parentHwnd, S("save-if-data-has-changed"), S("save"), S("unsort")) == Config::CUSTOMER_FORM_YES_BUTTON_ID) {
//			save();
//		} else {
//			return false;
//		}
//	}
//
//	// 2. Reset the HDF_SORTDOWN/HDF_SORTUP of other header items	
//	clearHeaderSorted(iSelItem);
//
//	// 3. sort the select header item;
//	auto headerCtrl = view->GetHeader();
//	HDITEMW headerItem;	
//	memset(&headerItem, 0, sizeof(headerItem));
//	headerItem.mask = HDI_FORMAT;
//	headerCtrl.GetItem(iSelItem, &headerItem);
//	bool down = headerItem.fmt & HDF_SORTDOWN;
//	bool up = headerItem.fmt & HDF_SORTUP;
//	if (!down && !up) {
//		headerItem.fmt |= HDF_SORTDOWN;
//		down = true;
//	}else if (down) {
//		headerItem.fmt ^= HDF_SORTDOWN;
//		headerItem.fmt |= HDF_SORTUP;
//		down = !down;
//	} else if (up) {
//		headerItem.fmt ^= HDF_SORTUP;
//		headerItem.fmt |= HDF_SORTDOWN;	
//		down = !down;
//	}
//	headerCtrl.SetItem(iSelItem, &headerItem);
//		
//	// 4.sort the runtime datas	
//	sortRuntimeDatas(iSelItem - 1, down);
//
//	view->Invalidate(true);
//	return true;
//}

/**
 * Fill the item and subitem data value of list view .
 * 
 * @param pLvdi
 * @return 
 */
//LRESULT ResultListPageDelegate::fillDataInListViewSubItem(NMLVDISPINFO * pLvdi)
//{
//	auto iItem = pLvdi->item.iItem;
//	if (-1 == iItem)
//		return 0;
//
//	auto count = static_cast<int>(runtimeDatas.size());
//	if (!count || count <= iItem)
//		return 0;
//
//	if (iItem == count - 1) {
//		iItem = count - 1;
//	}
//
//	// set checked/unchecked image in the first column	
//	if (pLvdi->item.iSubItem == 0 && pLvdi->item.mask & LVIF_TEXT) {
//		pLvdi->item.mask = LVIF_IMAGE;
//		if (getIsChecked(pLvdi->item.iItem)) {
//			pLvdi->item.iImage = 1;
//		} else {
//			pLvdi->item.iImage = 0;
//		}
//		
//		return 0;
//	}
//
//	auto iter = runtimeDatas.begin();
//	for (int i = 1; i < iItem + 1; i++) {
//		iter++;
//	}
//	RowItem & rowItem = *iter;
//	if (pLvdi->item.iSubItem > 0 && (pLvdi->item.mask & LVIF_TEXT)) {
//		std::string val;
//		if (resultType == QUERY_TABLE_DATA || runtimeColumns.at(0) == "_ct_sqlite_rowid") { // ROW_ID
//			val = rowItem.at(pLvdi->item.iSubItem);
//		} else {
//			val = rowItem.at(pLvdi->item.iSubItem - 1);
//		}
//		StringCchCopy(pLvdi->item.pszText, pLvdi->item.cchTextMax, val.c_str());
//	}
//
//	return 0;
//}

/**
 * load the query table from sql statement in runtime.
 * 
 * @param connectId the select db id
 * @param sql
 */
void ResultListPageDelegate::loadRuntimeTables(uint64_t connectId, const std::string & schema, const std::string & sql)
{
	ATLASSERT(!sql.empty());
	runtimeTables.clear();
	if (!SqlUtil::isSelectSql(sql)) {
		return ;
	}
	UserTableStrings allTables = metadataService->getUserTableStrings(connectId, schema);
	runtimeTables = SqlUtil::getTablesFromSelectSql(sql, allTables);
}

void ResultListPageDelegate::loadRuntimeHeader(sql::ResultSet * query)
{
	view->DeleteAllColumns();
	int n = query->getMetaData()->getColumnCount();
	for (int i = 0; i < n; i++) {
		std::string columnName = query->getMetaData()->getColumnName(i+1);
		if (!columnName.empty() && columnName != "_ct_sqlite_rowid") {
			int colIdx = view->GetColumnCount();
			wxListItem item;
			item.SetId(colIdx);
			item.SetText(columnName);
			item.SetWidth(100);
			item.SetBackgroundColour(rowBkgColor1);
			item.SetTextColour(textColor);
			view->InsertColumn(i, item);
		}
		
		runtimeColumns.push_back(columnName);
	}
}

/**
 * Clear the header sorted but not selected item.
 * 
 * @param notSelItem - not selected item.
 */
//void ResultListPageDelegate::clearHeaderSorted(int notSelItem)
//{
//	auto headerCtrl = view->GetHeader();
//	int n = headerCtrl.GetItemCount();
//	HDITEMW headerItem;
//	for (int i = 1; i < n; ++i) {
//		if (i == notSelItem) {
//			continue;
//		}
//		memset(&headerItem, 0, sizeof(headerItem));
//		headerItem.mask = HDI_FORMAT;
//		headerCtrl.GetItem(i, &headerItem);
//		if (headerItem.fmt & HDF_SORTDOWN) {
//			headerItem.fmt ^= HDF_SORTDOWN;
//			headerCtrl.SetItem(i, &headerItem);
//		} else if (headerItem.fmt & HDF_SORTUP) {
//			headerItem.fmt ^= HDF_SORTUP;
//			headerCtrl.SetItem(i, &headerItem);
//		}
//	}
//}

int ResultListPageDelegate::loadRuntimeData(sql::ResultSet * resultSet)
{
	int n = static_cast<int>(runtimeColumns.size());
	while (resultSet->next()) {
		RowItem rowItem;
		for (int i = 0; i < n; i++) {
			std::string columnVal = resultSet->isNull(i + 1) ? "< NULL >" : 
				StringUtil::converFromUtf8( resultSet->getString(i + 1).asStdString());

			//std::string columnVal = resultSet->isNull(i + 1) ? "< NULL >" : resultSet->getString(i + 1).asStdString();
			rowItem.push_back(columnVal);
		}
		runtimeDatas.push_back(rowItem);
	}
	int nRow = static_cast<int>(runtimeDatas.size());
	// trigger CListViewCtrl message LVN_GETDISPINFO to parent HWND, it will call this->fillListViewItemData(NMLVDISPINFO * pLvdi)
	//view->SetDataList(&runtimeDatas);
	
	return nRow;
}

void ResultListPageDelegate::displayRuntimeData()
{
	int nRows = static_cast<int>(runtimeDatas.size());
	int nCols = static_cast<int>(runtimeColumns.size());

	
	int row = 0;
	for (auto iter = runtimeDatas.begin(); iter != runtimeDatas.end(); iter++) {
		RowItem & rowItem = *iter;
		for (int i = 0; i < nCols; i++) {
			std::string columnVal = rowItem.at(i);
			if (i == 0) {
				wxListItem listItem;
				listItem.SetColumn(i);
				listItem.SetId(row);
				listItem.SetText(columnVal);
				listItem.SetAlign(wxLIST_FORMAT_LEFT);
				listItem.SetBackgroundColour(row % 2 ? rowBkgColor1 : rowBkgColor2);
				listItem.SetTextColour(textColor);
				view->InsertItem(listItem);
			} else {
				view->SetItem(row, i, columnVal);
			}
		}
		++row;
	}
}

int ResultListPageDelegate::loadRuntimeDataToList(sql::ResultSet* resultSet)
{
	int n = static_cast<int>(runtimeColumns.size());
	int row = 0;
	while (resultSet->next()) {
		RowItem rowItem;	
		
		for (int i = 0; i < n; i++) {
			std::string columnVal = resultSet->isNull(i + 1) ? "< NULL >" : 
				StringUtil::converFromUtf8( resultSet->getString(i + 1).asStdString());
			rowItem.push_back(columnVal);

			if (i == 0) {
				wxListItem listItem;
				listItem.SetColumn(i);
				listItem.SetId(runtimeDatas.size());
				listItem.SetText(columnVal);
				listItem.SetAlign(wxLIST_FORMAT_LEFT);
				listItem.SetBackgroundColour(row % 2 ? rowBkgColor1 : rowBkgColor2);
				listItem.SetTextColour(textColor);
				view->InsertItem(listItem);
			} else {
				view->SetItem(runtimeDatas.size(), i, columnVal);
			}
		}
		++row;
		runtimeDatas.push_back(rowItem);
	}
	int nRow = static_cast<int>(runtimeDatas.size());
	// trigger CListViewCtrl message LVN_GETDISPINFO to parent HWND, it will call this->fillListViewItemData(NMLVDISPINFO * pLvdi)
	// virtual controls for listView lazy load the data
	// view->SetItemCount(nRow);
	
	return nRow;
}

void ResultListPageDelegate::loadLimitParams(LimitParams & limitParams)
{
	std::string limitChecked = SettingService::getInstance()->getSysInit(settingPrefix + "limit-checked");
	std::string offset = SettingService::getInstance()->getSysInit(settingPrefix + "limit-offset");
	std::string rows = SettingService::getInstance()->getSysInit(settingPrefix + "limit-rows");
	offset = StringUtil::isDigit(offset) ? offset : "0";
	rows = StringUtil::isDigit(rows) ? rows : "1000";
	limitParams.checked = limitChecked == "true";
	limitParams.offset = std::stoi(offset);
	limitParams.rows = std::stoi(rows);
}

/**
 * if the row of index=iItem is selected.
 * 
 * @param iItem the row index
 * @return 
 */
bool ResultListPageDelegate::getIsChecked(int iItem)
{
	if (view->GetSelectedItemCount() == 0) {
		return false;
	}
	int nSelItem = view->GetNextItem(-1,  wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED); // 向下搜索选中的项 -1表示先找出第一个
	
	while (nSelItem != -1) {
		if (nSelItem == iItem) {
			return true;
		}
		nSelItem = view->GetNextItem(nSelItem, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED); // 继续往下搜索选中项
	}

	return false;
}

/**
 * Buid runtime sql statement with origin sql statement.
 * 
 * @return new runtime sql
 */
std::string ResultListPageDelegate::buildRungtimeSqlWithFilters()
{
	if (runtimeFilters.empty() || originSql.empty()) {
		return originSql;
	}
	std::string newSql;
	std::string whereClause = SqlUtil::getWhereClause(originSql);
	std::string fourthClause = SqlUtil::getFourthClause(originSql);

	std::string condition;
	int n = static_cast<int>(runtimeFilters.size());
	
	for (int i = 0; i < n; i++) {
		auto tuple = runtimeFilters.at(i);
		auto connect = std::get<0>(tuple);// connect such as "and/or"
		auto column = std::get<1>(tuple);// column
		auto operater =std::get<2>(tuple);// operator such as "=,>,<..."
		auto val = StringUtil::escapeSql(std::get<3>(tuple));// value

		// ignore if column is empty or operater is empty
		if (column.empty() || operater.empty()) {
			continue;
		}
		condition.append(connect).append(" ") 
			.append(column).append(" ") 
			.append(operater).append(" '") // 
			.append(val).append("' "); 
	}

	std::string newWhereClause = whereClause;
	if (whereClause.empty() && !condition.empty()) {		
		if (fourthClause.empty()) {
			// for example : select * from analysis_hair_inspection;
			newWhereClause.append(" WHERE ").append(condition);
		}else {
			// for example : select * from analysis_hair_inspection order by uid;
			newWhereClause.append(" WHERE ").append(condition).append(" ").append(fourthClause);
		}
		
	}else if (!whereClause.empty() && !condition.empty()) {
		// for example : select * from analysis_hair_inspection where id=1 order by uid;
		newWhereClause.append(" AND (").append(condition).append(")");
	}

	if (!whereClause.empty()) {
		newSql = StringUtil::replace(originSql, whereClause, newWhereClause);
	} else {
		// for example : select * from analysis_hair_inspection
		if (fourthClause.empty() && !newWhereClause.empty()) {
			newSql.assign(originSql).append(" ").append(newWhereClause);
		}else if (!fourthClause.empty()){
			// for example : select * from analysis_hair_inspection order by uid;
			newSql = StringUtil::replace(originSql, fourthClause, newWhereClause);
		}else {
			newSql = originSql;
		}		
	}

	if (!SqlUtil::hasLimitClause(originSql)) {
		LimitParams limitParams;
		loadLimitParams(limitParams);
		if (limitParams.checked) {
			newSql.append(" LIMIT ").append(std::to_string(limitParams.rows))
				.append(" OFFSET ").append(std::to_string(limitParams.offset));
		}		
	}
	
	return newSql;
}

RowItem ResultListPageDelegate::getFirstSelectdRowItem()
{
	if (!view->GetSelectedItemCount()) {
		return RowItem();
	}
	
	int nSelItem = -1;
	if ((nSelItem = view->GetNextItem(nSelItem, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1) {
		DataList::iterator itor = runtimeDatas.begin();		
		for (int i = 0; i < nSelItem && itor != runtimeDatas.end(); i++) {
			itor++;
		}
		return *itor;
	}
	return RowItem();
}

int ResultListPageDelegate::getFirstSelectdIndex()
{
	if (!view->GetSelectedItemCount()) {
		return -1;
	}

	int nSelItem = -1;
	return  view->GetNextItem(nSelItem, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

DataList ResultListPageDelegate::getSelectedDatas()
{
	if (!view->GetSelectedItemCount()) {
		return DataList();
	}
	DataList result;
	int nSelItem = -1;
	while ((nSelItem = view->GetNextItem(nSelItem, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1) {
		DataList::iterator itor = runtimeDatas.begin();
		for (int i = 0; i < nSelItem && itor != runtimeDatas.end(); i++) {
			itor++;
		}
		if (itor != runtimeDatas.end()) {
			result.push_back(*itor);
		}
		
	}
	return result;
}

int ResultListPageDelegate::getSelectedItemCount()
{
	return view->GetSelectedItemCount();
}

void ResultListPageDelegate::setRuntimeUserConnectId(uint64_t connectId)
{
	runtimeUserConnectId = connectId;
}

const UserTableStrings & ResultListPageDelegate::getRuntimeTables()
{
	return runtimeTables;
}

void ResultListPageDelegate::setRuntimeTables(const UserTableStrings & val)
{
	runtimeTables = val;
}

const Columns & ResultListPageDelegate::getRuntimeColumns()
{
	return runtimeColumns;
}

void ResultListPageDelegate::setRuntimeColumns(const Columns & columns)
{
	runtimeColumns = columns;
}

const DataList & ResultListPageDelegate::getRuntimeDatas()
{
	return runtimeDatas;
}

void ResultListPageDelegate::setRuntimeDatas(const DataList & dataList)
{
	runtimeDatas = dataList;
}

/**
 * add subItemVal to variable changeVals of QListView.
 * 
 * @param subItemVal
 */
void ResultListPageDelegate::addListViewChangeVal(SubItemValue &subItemVal)
{
	// view->setChangeVal(subItemVal);
}

ColumnInfoList ResultListPageDelegate::getRuntimeColumnInfos(const std::string & schema, const std::string & tblName)
{
	assert(runtimeUserConnectId && !schema.empty() && !tblName.empty());
	return metadataService->getColumnsOfUserTable(runtimeUserConnectId, schema, tblName);
}

UserTable ResultListPageDelegate::getRuntimeUserTable(const std::string & schema, const std::string & tblName)
{
	assert(runtimeUserConnectId && !tblName.empty());
	return metadataService->getUserTable(runtimeUserConnectId, schema, tblName);
}

/**
 * Get the runtime valid query columns for filter the result list.
 * 
 * @return valid query columns
 */
Columns ResultListPageDelegate::getRuntimeValidFilterColumns()
{
	if (runtimeTables.empty() || runtimeColumns.empty()) {
		return Columns();
	}
	Columns validColums;

	std::string schema = supplier->getRuntimeSchema();
	for (auto & tblName : runtimeTables) {
		ColumnInfoList columnInfos = getRuntimeColumnInfos(schema, tblName);
		for (auto & column : runtimeColumns) {
			auto iter = std::find_if(columnInfos.begin(), columnInfos.end(), [&column](ColumnInfo & columnInfo) {
				return column == columnInfo.name;
			});
			if (iter != columnInfos.end()) {
				validColums.push_back(column);
			}
		}
	}

	return validColums;
}

const DataFilters & ResultListPageDelegate::getRuntimeFilters()
{
	return runtimeFilters;
}

void ResultListPageDelegate::setRuntimeFilters(DataFilters & filters)
{
	runtimeFilters = filters;
}

void ResultListPageDelegate::clearRuntimeFilters()
{
	runtimeFilters.clear();
}

bool ResultListPageDelegate::isRuntimeFiltersEmpty()
{
	return std::empty(runtimeFilters);
}

void ResultListPageDelegate::copyAllRowsToClipboard()
{
	int n = static_cast<int>(runtimeColumns.size());
	bool hasRowId = runtimeColumns.at(0) == "_ct_sqlite_rowid";

	std::ostringstream oss;
	// 1.write the columns to stringstream
	for (int i = 0; i < n; i++) {
		auto & column = runtimeColumns.at(i);
		if (column == "_ct_sqlite_rowid") {
			continue;
		}

		if (hasRowId && i > 1) {
			oss << ",";
		}else if (!hasRowId && i > 0) {
			oss << ",";
		}
		oss << L'"' << column << L'"';
	}
	oss << std::endl;

	// 2.write the datas to stringstream
	n = 0;	
	for (auto & vals : runtimeDatas) {
		if (vals.empty()) {
			continue;
		}
		
		int i = 0;
		for (auto & val : vals) {
			if (hasRowId && i == 0) {
				i++;
				continue;
			}

			if (hasRowId && i > 1) {
				oss << ",";
			} else if (!hasRowId && i > 0) {
				oss << ",";
			}

			std::string rval = StringUtil::escape(val);
			oss << L'"' << rval << L'"';
			i++;
		}
		oss << std::endl;
	}
	oss.flush();

	// 3. copy the stringstream to clipboard
	ClipboardUtil::copyToClipboard(oss.str());
}

void ResultListPageDelegate::copySelRowsToClipboard()
{
	int n = static_cast<int>(runtimeColumns.size());
	bool hasRowId = runtimeColumns.at(0) == "_ct_sqlite_rowid";

	std::ostringstream oss;
	// 1.write the columns to stringstream
	for (int i = 0; i < n; i++) {
		auto & column = runtimeColumns.at(i);
		if (column == "_ct_sqlite_rowid") {
			continue;
		}
		if (hasRowId && i > 1) {
			oss << ",";
		}else if (!hasRowId && i > 0) {
			oss << ",";
		}
		oss << L'"' << column << L'"';
	}
	oss << std::endl;

	// 2.write the selected datas to stringstream
	n = 0;
	DataList selDatas = getSelectedDatas();
	
	for (auto & vals : selDatas) {
		if (vals.empty()) {
			continue;
		}
		
		int i = 0;
		for (auto & val : vals) {
			if (hasRowId && i == 0) {
				i++;
				continue;
			}
			if (hasRowId && i > 1) {
				oss << ",";
			} else if (!hasRowId && i > 0) {
				oss << ",";
			}

			std::string rval = StringUtil::escape(val);
			oss << L'"' << rval << L'"';
			i++;
		}
		oss << std::endl;
		n++;
	}
	oss.flush();

	// 3. copy the stringstream to clipboard
	ClipboardUtil::copyToClipboard(oss.str());
}

void ResultListPageDelegate::copyAllRowsAsSql()
{
	if (runtimeTables.empty() || runtimeTables.size() > 1) {
		return ;
	}
	
	std::string & tbl = runtimeTables.at(0);
	int n = static_cast<int>(runtimeColumns.size());
	bool hasRowId = runtimeColumns.at(0) == "_ct_sqlite_rowid";

	std::ostringstream oss;
	// 1.write the data to stringstream
	n = 0;
	for (auto & vals : runtimeDatas) {
		if (vals.empty()) {
			continue;
		}
		int i = 0;
		std::ostringstream dataSql, columnStmt, valuesStmt;
		dataSql << "INSERT INTO " << tbl << L' ';

		columnStmt << "(";
		valuesStmt << " VALUES (";
		// write the selected column data value
		for (auto & column : runtimeColumns) {
			if (column == "_ct_sqlite_rowid") {
				i++;
				continue;
			}
			std::string val = StringUtil::escapeSql(vals.at(i));

			if (hasRowId && i > 1) {
				columnStmt << ", ";
				valuesStmt <<  ", ";
			} else if (!hasRowId && i > 0) {
				columnStmt << ", ";
				valuesStmt <<  ", ";
			}
			columnStmt << "\"" << column << "\"";
			if (val == "< AUTO >" || val == "< NULL >") {
				valuesStmt << "NULL";
			} else {
				valuesStmt << "'" << val <<  "'";
			}
			
			i++;
		}
		columnStmt <<  L')';
		valuesStmt << L')';
		dataSql << columnStmt.str() << valuesStmt.str() << L';';
		oss << dataSql.str() << std::endl;
		n++;
	}
	
	oss.flush();

	// 2. copy the stringstream to clipboard
	ClipboardUtil::copyToClipboard(oss.str());
}

void ResultListPageDelegate::copySelRowsAsSql()
{
	if (runtimeTables.empty() || runtimeTables.size() > 1) {
		return ;
	}
	std::string & tbl = runtimeTables.at(0);
	int n = static_cast<int>(runtimeColumns.size());
	bool hasRowId = runtimeColumns.at(0) == "_ct_sqlite_rowid";

	std::ostringstream oss;
	// 1.write the data to stringstream
	n = 0;
	DataList selDatas = getSelectedDatas();
	for (auto & vals : selDatas) {
		if (vals.empty()) {
			continue;
		}
		int i = 0;
		std::ostringstream dataSql, columnStmt, valuesStmt;
		dataSql << "INSERT INTO \"" << tbl << "\" ";

		columnStmt << "(";
		valuesStmt << " VALUES (";
		// write the selected column data value
		for (auto & column : runtimeColumns) {	
			if (column == "_ct_sqlite_rowid") {
				i++;
				continue;
			}
			std::string val = StringUtil::escapeSql(vals.at(i));

			if (hasRowId && i > 1) {
				columnStmt << ", ";
				valuesStmt <<  ", ";
			} else if (!hasRowId && i > 0) {
				columnStmt << ", ";
				valuesStmt <<  ", ";
			}

			columnStmt << "\"" << column << "\"";
			valuesStmt << "'" << val <<  "'";
			i++;
		}
		columnStmt <<  L')';
		valuesStmt << L')';
		dataSql << columnStmt.str() << valuesStmt.str() << L';';
		oss << dataSql.str() << std::endl;
		n++;
	}
	
	oss.flush();

	// 2. copy the stringstream to clipboard
	ClipboardUtil::copyToClipboard(oss.str());
}

void ResultListPageDelegate::setSettingPrefix(std::string &prefix)
{
	settingPrefix = prefix;
}

/**
 * change the runtimeDatas variable before changed the list view subitem text .
 * 
 * @param iItem
 * @param iSubItem
 * @param text
 */
void ResultListPageDelegate::changeRuntimeDatasItem(int iItem, int iSubItem, std::string & origText, std::string & newText)
{
	ATLASSERT(iItem >= 0 && iSubItem > 0);
	auto iter = runtimeDatas.begin();
	
	for (int i = 0; i < iItem; i++) {
		iter++;
	}
	RowItem & rowItem = *iter;
	// rowItem.index = listView.row.iSubItem
	rowItem[iSubItem] = newText;
}

//void ResultListPageDelegate::invalidateSubItem(int iItem, int iSubItem)
//{
//	CRect subItemRect;
//	view->GetSubItemRect(iItem, iSubItem, LVIR_BOUNDS, subItemRect);
//	view->InvalidateRect(subItemRect, false);
//}
//
//void ResultListPageDelegate::createNewRow()
//{
//	if (runtimeColumns.empty()) {
//		return;
//	}
//	// 1.create a empty row and push it to runtimeDatas list
//	RowItem row;
//	std::string primaryKey;
//	try {
//		primaryKey = tableService->getPrimaryKeyColumn(runtimeUserConnectId, runtimeTables.at(0), runtimeColumns);
//	} catch (QSqlExecuteException &ex) {
//		Q_ERROR("error{}, msg:{}", ex.getCode(), ex.getMsg());
//		QPopAnimate::report(ex);
//		return;
//	}
//	int n = static_cast<int>(runtimeColumns.size());
//	for (int i = 0; i < n; i++) {
//		auto colum = runtimeColumns.at(i);
//		if (colum == primaryKey) {
//			row.push_back("< AUTO >");
//		}else {
//			row.push_back(std::string());
//		}
//		
//	}
//	runtimeDatas.push_back(row);
//
//	// 2.update the item count and selected the new row	
//	n = static_cast<int>(runtimeDatas.size());
//	runtimeNewRows.push_back(n-1); // param - runtimeDatas index
//	view->SetItemCount(n);
//	view->SelectItem(n-1);
//
//	// 3.show the editor on first column for the new row 
//	view->createOrShowEditor(n - 1, 1);// the 1th column
//}
//
//void ResultListPageDelegate::copyNewRow()
//{
//	if (runtimeColumns.empty()) {
//		return;
//	}
//	// 1.copy selected row and push it to runtimeDatas list
//	RowItem row = getFirstSelectdRowItem();
//	if (row.empty()) {
//		return;
//	}
//	std::string primaryKey;
//	try {
//		primaryKey = tableService->getPrimaryKeyColumn(runtimeUserConnectId, runtimeTables.at(0), runtimeColumns);
//	} catch (QSqlExecuteException &ex) {
//		Q_ERROR("error{}, msg:{}", ex.getCode(), ex.getMsg());
//		QPopAnimate::report(ex);
//		return;
//	}
//	bool hasRowId = runtimeColumns.at(0) == "_ct_sqlite_rowid";
//	if (hasRowId && primaryKey == runtimeColumns.at(1)) {
//		row[1] = "< AUTO >";
//	} else if (!hasRowId && primaryKey == runtimeColumns.at(0)) {
//		row[0] = "< AUTO >";
//	}
//	runtimeDatas.push_back(row);
//	int n = static_cast<int>(runtimeDatas.size());
//	runtimeNewRows.push_back(n-1); // runtimeDatas index
//	view->SetItemCount(n);
//	view->SelectItem(n-1);
//
//	// 2.show the editor on first column for the new row 
//	std::pair<int, int> subItemPos(n - 1, 1); // the 1th column
//	view->createOrShowEditor(subItemPos);
//}

/**
 * save the change data includes:
 * 1.The changeVals from QListView::getChangeVals
 * 2.The runtimeDates index from runtimeNewRows 
 * 
 * @return 
 */
//bool ResultListPageDelegate::save()
//{
//	// 1.The changeVals from QListViewCtrl::getChangeVals function
//	bool ret = saveChangeVals();
//	if (!ret) {
//		return ret;
//	}
//	// 2. The runtimeNewRows that it save runtimeDatas index  
//	ret = saveNewRows();
//	if (!ret) {
//		return ret;
//	}
//	QPopAnimate::success(parentHwnd, S("save-success-text"));
//	return true;
//}

/**
 * The changeVals from QListViewCtrl::getChangeVals function will be save.
 * 
 */
//bool ResultListPageDelegate::saveChangeVals()
//{
//	SubItemValues changeVals = view->getChangedVals();
//	SubItemValues errorChanges;
//	if (changeVals.empty()) {
//		return true;
//	}
//	// BEGIN A TRANSACTION
//	std::string sql = "BEGIN;";
//	tableService->execBySql(runtimeUserConnectId, sql);
//	std::string sqlSuccess = sql;
//
//	// sql log
//	resetRuntimeResultInfo();
//	runtimeResultInfo.connectId = runtimeUserConnectId;
//
//	auto _begin = PerformUtil::begin();
//	int effectRows = 0;
//	for (auto subItemVal : changeVals) {
//		int iItem = subItemVal.iItem;
//		int iSubItem = subItemVal.iSubItem;
//
//		//iItem will be excludes if it's in the vector rumtimeNewRows
//		auto iterItem = std::find(runtimeNewRows.begin(), runtimeNewRows.end(), iItem);
//		if (iterItem != runtimeNewRows.end()) {
//			continue;
//		}
//
//		auto iter = runtimeDatas.begin();
//		for (int i = 0; i < iItem ; i++) {
//			iter++;
//		}
//		// this row change vals vector
//		SubItemValues rowChangeVals = view->getRowChangedVals(iItem);
//		RowItem & rowItem = *iter;
//		std::string tblName = runtimeTables.at(0);
//		
//		std::string whereClause;
//		int nSelSubItem = iSubItem;
//		std::string & origVal = subItemVal.origVal;
//		
//		whereClause = SqlUtil::makeWhereClauseByRowId(runtimeColumns, rowItem);
//		std::string newVal = StringUtil::escapeSql(subItemVal.newVal);
//		std::string sqlUpdate("UPDATE ");
//		sqlUpdate.append(quo).append(tblName).append(quo)
//			.append(" SET ").append(quo)
//			.append(runtimeColumns.at(nSelSubItem)).append(quo)
//			.append("=")
//			.append(qua).append(newVal).append(qua)
//			.append(whereClause);
//
//		try {
//			tableService->execBySql(runtimeUserConnectId, sqlUpdate);
//			effectRows++;
//			sqlSuccess.append(lbrk).append(sqlUpdate).append(edl);
//		}  catch (QSqlExecuteException &ex) {
//			// ROLLBACK
//			sql = "ROLLBACK;";
//			tableService->execBySql(runtimeUserConnectId, sql);
//
//			errorChanges.push_back(subItemVal);
//			Q_ERROR("query db has error, code:{}, msg:{}, sql:{}", ex.getCode(), ex.getMsg(), sqlUpdate);
//			view->SelectItem(subItemVal.iItem);
//			view->createOrShowEditor(subItemVal.iItem, subItemVal.iSubItem);
//
//			ex.setRollBack(true);
//			QPopAnimate::report(ex);
//
//			// SQL LOG
//			runtimeResultInfo.sql = sqlUpdate;
//			runtimeResultInfo.execTime = PerformUtil::end(_begin);
//			runtimeResultInfo.transferTime = PerformUtil::end(_begin);
//			runtimeResultInfo.totalTime = PerformUtil::end(_begin);
//			runtimeResultInfo.code = std::stoi( ex.getCode());
//			runtimeResultInfo.msg = ex.getMsg();
//			sendExecSqlMessage(runtimeResultInfo, true);
//			return false;
//		}
//	}
//
//	// commit all the executed sqls by tableService
//	sql = "COMMIT;";
//	tableService->execBySql(runtimeUserConnectId, sql);
//	sqlSuccess.append(lbrk).append(sql);
//	runtimeResultInfo.sql = sqlSuccess;
//	runtimeResultInfo.effectRows = effectRows;
//	runtimeResultInfo.execTime = PerformUtil::end(_begin);
//	runtimeResultInfo.transferTime = PerformUtil::end(_begin);
//	runtimeResultInfo.totalTime = PerformUtil::end(_begin);
//	sendExecSqlMessage(runtimeResultInfo, true);
//
//	// clear changes
//	view->clearChangeVals();
//
//	// restore the changeVals if has error
//	if (!errorChanges.empty()) {
//		view->setChangedVals(errorChanges);
//		return false;
//	} 
//	return true;
//}

/**
 * The runtimeDates index from runtimeNewRows.
 * 
 */
//bool ResultListPageDelegate::saveNewRows()
//{
//	if (runtimeNewRows.empty()) {
//		return true;
//	}
//	std::vector<int> errorNewRows;
//	std::string tblName = runtimeTables.at(0);
//
//	// BEGIN A TRASACTION	
//	std::string sql = "BEGIN;";
//	tableService->execBySql(runtimeUserConnectId, sql);
//	std::string sqlSuccess = sql;
//
//	// sql log
//	resetRuntimeResultInfo();
//	runtimeResultInfo.connectId = runtimeUserConnectId;
//
//	auto _begin = PerformUtil::begin();
//	int effectRows = 0;
//	for (auto nItem : runtimeNewRows) {
//		auto itor = runtimeDatas.begin();
//		for (int i = 0; i < nItem; i++) {
//			itor++;
//		}
//		std::string colums = SqlUtil::makeInsertColumsClause(runtimeColumns);
//		std::string values = SqlUtil::makeInsertValuesClause(*itor);
//		if (colums.empty() || values.empty()) {
//			continue;
//		}
//		
//		std::string sqlInsert("INSERT INTO ");
//		sqlInsert.append(quo).append(tblName).append(quo).append(colums).append(values);
//		
//		try {
//			tableService->execBySql(runtimeUserConnectId, sqlInsert);
//			effectRows++;
//			sqlSuccess.append(lbrk).append(sqlInsert).append(edl);
//		}  catch (QSqlExecuteException &ex) {
//			// ROLLBACK
//			sql = "ROLLBACK;";
//			tableService->execBySql(runtimeUserConnectId, sql);
//
//			// select the error row
//			errorNewRows.push_back(nItem);
//			Q_ERROR("query db has error, code:{}, msg:{}, sql:{}", ex.getCode(), ex.getMsg(), sqlInsert);
//			view->SelectItem(nItem);
//			view->createOrShowEditor(nItem, 1);
//
//			ex.setRollBack(true);
//			QPopAnimate::report(ex);
//
//			// SQL LOG
//			runtimeResultInfo.sql = sqlInsert;
//			runtimeResultInfo.execTime = PerformUtil::end(_begin);
//			runtimeResultInfo.transferTime = PerformUtil::end(_begin);
//			runtimeResultInfo.totalTime = PerformUtil::end(_begin);
//			runtimeResultInfo.code = std::stoi( ex.getCode());
//			runtimeResultInfo.msg = ex.getMsg();
//			sendExecSqlMessage(runtimeResultInfo, true);
//			return false;
//		}
//	}
//	// commit all the executed sqls by tableService
//	sql = "COMMIT;";
//	tableService->execBySql(runtimeUserConnectId, sql);
//	sqlSuccess.append(lbrk).append(sql);
//	runtimeResultInfo.sql = sqlSuccess;
//	runtimeResultInfo.effectRows = effectRows;
//	runtimeResultInfo.execTime = PerformUtil::end(_begin);
//	runtimeResultInfo.transferTime = PerformUtil::end(_begin);
//	runtimeResultInfo.totalTime = PerformUtil::end(_begin);
//	sendExecSqlMessage(runtimeResultInfo, true);
//
//	// clear new rows
//	runtimeNewRows.clear();
//
//	// reset the runtimeNewRows if has error
//	if (!errorNewRows.empty()) {
//		runtimeNewRows = errorNewRows;
//		return false;
//	}
//
//	try {
//		loadFilterListView();
//	} catch (QSqlExecuteException &ex) {
//		QPopAnimate::report(ex);
//	}
//
//	sendExecSqlMessage(runtimeResultInfo);
//	return true;
//}
//
//bool ResultListPageDelegate::remove(bool confirm)
//{
//	if (!view->GetSelectedCount()) {
//		return false;
//	}
//	if (confirm && QMessageBox::confirm(parentHwnd, S("delete-confirm-text"), S("yes"), S("no")) == Config::CUSTOMER_FORM_NO_BUTTON_ID) {
//		return false;
//	}
//		
//	// 1. delete the changeVals from listView
//	std::vector<int> nSelItems;
//	int nSelItem = -1;
//	while ((nSelItem = view->GetNextItem(nSelItem, LVNI_SELECTED)) != -1) {
//		nSelItems.push_back(nSelItem);
//	}
//
//	if (nSelItems.empty()) {
//		return false;
//	}
//
//	// 2.delete from runtimeDatas and database and listView that the item begin from the last selected item
//	int n = static_cast<int>(nSelItems.size());
//	for (int i = n - 1; i >= 0; i--) {
//		nSelItem = nSelItems.at(i);
//		auto iter = runtimeDatas.begin();
//		for (int j = 0; j < nSelItem; j++) {
//			iter++;
//		}
//		RowItem & rowItem = (*iter);
//		// 2.1 delete row from database 
//		if (!removeRowFromDb(nSelItem, rowItem)) {
//			return false;
//		}
//
//		// 2.2 delete row from runtimeDatas vector 
//		runtimeDatas.erase(iter);
//
//		// 2.3 delete row from listView
//		view->RemoveItem(nSelItem);
//	}
//
//	// 3.delete or subtract item index in runtimeNewRows and changeVals the item begin from the last selected item
//	for (int i = n - 1; i >= 0; i--) {
//		nSelItem = nSelItems.at(i);
//		auto itor = runtimeNewRows.begin();
//		while (itor != runtimeNewRows.end()) {
//			if ((*itor) == nSelItem) {
//				if (itor != runtimeNewRows.begin()) {
//					runtimeNewRows.erase(itor--);
//				} else {
//					runtimeNewRows.erase(itor);
//					itor = runtimeNewRows.begin();
//					continue;
//				}
//				if (runtimeNewRows.empty()) {
//					break;
//				}
//			} else if ((*itor) > nSelItem) {
//				(*itor)--;
//			}
//			itor++;
//		}
//	}
//
//	return true;
//}
//
//int ResultListPageDelegate::removeRowFromDb(int nSelItem, RowItem & rowItem)
//{
//	std::string & tblName = runtimeTables.at(0);
//
//	// if the nSelItem row 
//	auto iter = std::find(runtimeNewRows.begin(), runtimeNewRows.end(), nSelItem);
//	if (iter != runtimeNewRows.end()) {
//		return 1;
//	}
//
//	// this row change vals vector
//	SubItemValues rowChangedVals = view->getRowChangedVals(nSelItem);
//
//	// delete row from database 
//	std::string sqlDelete = "DELETE FROM ";
//	std::string primaryKey;
//	try {
//		primaryKey = tableService->getPrimaryKeyColumn(runtimeUserConnectId, runtimeTables.at(0), runtimeColumns);
//	} catch (QSqlExecuteException &ex) {
//		Q_ERROR("error{}, msg:{}", ex.getCode(), ex.getMsg());
//		QPopAnimate::report(ex);
//		return 1;
//	}
//	std::string whereClause;
//	if (primaryKey.empty()) {
//		whereClause = SqlUtil::makeWhereClause(runtimeColumns, rowItem, rowChangedVals);
//	}else {
//		// check the primary key value is < AUTO > / < NULL >
//		size_t n = runtimeColumns.size();
//		for (size_t i = 0; i < n; i++) {
//			auto & column = runtimeColumns.at(i);
//			auto & val = rowItem.at(i);
//			// this row data must be a new data ,database no record before save the row data.
//			if (primaryKey == column && (val == "< AUTO >" || val == "< NULL >")) {
//				return 1;
//			}
//		}
//		
//		whereClause = SqlUtil::makeWhereClauseByPrimaryKey(primaryKey, runtimeColumns, rowItem, rowChangedVals);
//	}
//	sqlDelete.append(quo).append(tblName).append(quo).append(whereClause);
//
//	// For record sql log
//	resetRuntimeResultInfo();
//	runtimeResultInfo.connectId = runtimeUserConnectId;
//	auto bt = PerformUtil::begin();
//
//	int effectRows = 0;
//	try {
//		auto stmt = sqlService->tryExecuteSql(runtimeUserConnectId, sqlDelete);
//		effectRows = stmt.exec();		
//			
//	} catch (SQLite::QSqlException &ex) {
//		std::string _err("Error:\r\n");
//		_err.append(ex.getErrorStr()).append("\r\nSQL:\r\n").append(sqlDelete);
//		Q_ERROR("query db has error, code:{}, msg:{}", ex.getErrorCode(), _err);
//		QMessageBox::confirm(parentHwnd, _err, S("yes"), S("no"));
//
//		// For record sql log
//		runtimeResultInfo.code = ex.getErrorCode();
//		runtimeResultInfo.msg = ex.getErrorStr();
//	}
//
//	// For record sql log
//	runtimeResultInfo.sql = sqlDelete;
//	runtimeResultInfo.effectRows = effectRows;	
//	runtimeResultInfo.execTime = PerformUtil::end(bt);
//	runtimeResultInfo.transferTime = PerformUtil::end(bt);
//	runtimeResultInfo.totalTime = PerformUtil::end(bt);
//	sendExecSqlMessage(runtimeResultInfo, true);
//	return effectRows;
//}
//
//bool ResultListPageDelegate::isDirty()
//{
//	bool hasEditSubItem = listView && view->IsWindow() ? view->getChangedCount() : false;
//	bool hasNewRow = !runtimeNewRows.empty(); 
//
//	return hasEditSubItem || hasNewRow;
//}
//
ResultInfo & ResultListPageDelegate::getRuntimeResultInfo()
{
	return runtimeResultInfo;
}
//
//bool ResultListPageDelegate::cancel()
//{
//	// 1. delete the new item from runtimeNewRows
//	if (!runtimeNewRows.empty()) {
//		int n = static_cast<int>(runtimeNewRows.size());
//		for (int i = n - 1; i >= 0; i--) {
//			int nSelItem = runtimeNewRows.at(i);
//			view->SelectItem(nSelItem);
//			if (!remove(false)) {
//				return false;
//			}
//		}		
//	}	
//
//	// 2. restore the text of subitem in the listview
//	if (!restoreChangeVals()) {
//		return false;
//	}
//	return true;
//}

/**
 * restore the change values from list view.
 * 
 */
//bool ResultListPageDelegate::restoreChangeVals()
//{
//	// 1.restore the runtimeDatas and refresh the text of the changed subitem 
//	auto changeVals = view->getChangedVals();
//	int n = static_cast<int>(changeVals.size());
//	for (int i = n - 1; i >= 0; i--) {
//		auto item = changeVals.at(i);
//		auto iter = runtimeDatas.begin();
//		for (int j = 0; j < item.iItem; j++) {
//			iter++;
//		}
//		(*iter).at(item.iSubItem - 1) = item.origVal;
//
//		// invalidate the subitem
//		invalidateSubItem(item.iItem, item.iSubItem);
//	}
//
//	// 2. clear all change vals
//	view->clearChangeVals();
//	return true;
//}

void ResultListPageDelegate::sendExecSqlMessage(ResultInfo & resultInfo, bool isWait)
{
	if (isWait) { // wait for response
		AppContext::getInstance()->dispatchForResponse(Config::MSG_EXEC_SQL_RESULT_MESSAGE_ID, WPARAM(NULL), LPARAM(&resultInfo));
	} else {
		AppContext::getInstance()->dispatch(Config::MSG_EXEC_SQL_RESULT_MESSAGE_ID, WPARAM(NULL), LPARAM(&resultInfo));
	}
	
}



void ResultListPageDelegate::resetRuntimeResultInfo()
{
	runtimeResultInfo.sql.clear();
	runtimeResultInfo.effectRows = 0;
	runtimeResultInfo.execTime.clear();
	runtimeResultInfo.totalTime.clear();
	runtimeResultInfo.code = 0;
	runtimeResultInfo.msg.clear();
}

void ResultListPageDelegate::sortRuntimeDatas(int index, bool isDown)
{	
	runtimeDatas.sort([&index, &isDown](const RowItem & item1, const RowItem & item2) {
		auto val1 = item1.at(index);
		auto val2 = item2.at(index);
		val1 = (val1 == "< AUTO >" || val1 == "< NULL >") ? "" : val1;
		val2 = (val2 == "< AUTO >" || val2 == "< NULL >") ? "" : val2;
		//Notice : don't verify if val1 and val2 is empty.
		if (StringUtil::isDecimal(val1) && StringUtil::isDecimal(val2)) {
			auto v1 = val1.empty() ? 0 : std::stold(val1);
			auto v2 = val2.empty() ? 0 : std::stold(val2);
			return isDown ? v1 > v2 : v1 < v2;
		}
		return isDown ? val1 > val2 : val1 < val2;
	});
}



