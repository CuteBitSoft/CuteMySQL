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

 * @file   TableStructureSupplier.cpp
 * @brief  This class supply runtime data for TableStructurePage/TableTabView/TableColumnsPage/TableIndexesPage objects
 * 
 * @author Xuehan Qin
 * @date   2023-10-21
 *********************************************************************/
#include "TableStructureSupplier.h"
#include "core/common/Lang.h"
#include "utils/StringUtil.h"
#include "utils/EntityUtil.h"

Columns TableStructureSupplier::colsHeadColumns;
const std::vector<int> TableStructureSupplier::colsHeadSizes = { 150, 100, 70, 70, 70, 70, 150, 100 };
const std::vector<int> TableStructureSupplier::colsHeadFormats = { wxALIGN_LEFT, wxALIGN_LEFT, wxALIGN_CENTER, wxALIGN_CENTER, wxALIGN_CENTER, wxALIGN_CENTER, wxALIGN_LEFT, wxALIGN_LEFT };
const std::vector<std::string> TableStructureSupplier::colsDataTypeList = { "INTEGER", "TEXT", "BLOB", "REAL", "NUMERIC"};

Columns TableStructureSupplier::idxHeadColumns; 
const std::vector<int> TableStructureSupplier::idxHeadSizes = { 150, 200, 100, 80, 200};
const std::vector<int> TableStructureSupplier::idxHeadFormats = { wxALIGN_LEFT, wxALIGN_LEFT, wxALIGN_LEFT, wxALIGN_CENTER, wxALIGN_LEFT};
const std::vector<std::string> TableStructureSupplier::idxTypeList = {"Unique", "Primary Key",  "Check", "Index"};

Columns TableStructureSupplier::frkHeadColumns;
const std::vector<int> TableStructureSupplier::frkHeadSizes = { 150, 150, 150, 150, 150, 150, 150};
const std::vector<int> TableStructureSupplier::frkHeadFormats = { wxALIGN_LEFT, wxALIGN_LEFT, wxALIGN_LEFT, wxALIGN_LEFT, wxALIGN_LEFT, wxALIGN_LEFT, wxALIGN_LEFT};
const std::vector<std::string> TableStructureSupplier::frkOnUpdateTypeList = {"SET NULL", "SET DEFAULT",  "CASCADE", "RESTRICT"};
const std::vector<std::string> TableStructureSupplier::frkOnDeleteTypeList = {"SET NULL", "SET DEFAULT",  "CASCADE", "RESTRICT"}; 

TableStructureSupplier::TableStructureSupplier()
{
	if (colsHeadColumns.empty()) {
		colsHeadColumns = { S("column-name"), S("data-type"), "Not Null", "PK", "Auto Insc", "Unique", S("default"), S("check")};
	}
	
	if (idxHeadColumns.empty()) {
		idxHeadColumns = { S("index-name"), S("columns"), S("index-type"), S("index-unique"), S("partial-clause")}; 
	}
	if (frkHeadColumns.empty()) {
		frkHeadColumns = { S("foreignkey-name"), S("referencing-columns"), S("referenced-table"), S("referenced-columns"), S("on-update"), S("on-delete"), S("partial-clause")};
	}	
}

ColumnInfo & TableStructureSupplier::getColsRuntimeData(int nSelItem)
{
	assert(nSelItem < static_cast<int>(colsRuntimeDatas.size()));
	return colsRuntimeDatas.at(nSelItem);
}

void TableStructureSupplier::eraseColsRuntimeData(int nSelItem)
{
	if (nSelItem >= static_cast<int>(colsRuntimeDatas.size())) {
		return;
	}
	auto iter = colsRuntimeDatas.begin();
	for (int j = 0; j < nSelItem; j++) {
		iter++;
	}
	colsRuntimeDatas.erase(iter);
}


void TableStructureSupplier::eraseColsOrigData(int nSelItem)
{
	if (nSelItem >= static_cast<int>(colsOrigDatas.size())) {
		return;
	}
	auto iter = colsOrigDatas.begin();
	for (int j = 0; j < nSelItem; j++) {
		iter++;
	}
	colsOrigDatas.erase(iter);
}


IndexInfo & TableStructureSupplier::getIdxRuntimeData(int nSelItem)
{
	assert(nSelItem < static_cast<int>(idxRuntimeDatas.size()));
	return idxRuntimeDatas.at(nSelItem);
}

void TableStructureSupplier::eraseIdxRuntimeData(int nSelItem)
{
	if (nSelItem >= static_cast<int>(idxRuntimeDatas.size())) {
		return;
	}
	auto iter = idxRuntimeDatas.begin();
	for (int j = 0; j < nSelItem; j++) {
		iter++;
	}
	idxRuntimeDatas.erase(iter);
}

ForeignKey & TableStructureSupplier::getFrkRuntimeData(int nSelItem)
{
	assert(nSelItem < static_cast<int>(frkRuntimeDatas.size()));
	return frkRuntimeDatas.at(nSelItem);
}

void TableStructureSupplier::eraseFrkRuntimeData(int nSelItem)
{
	if (nSelItem >= static_cast<int>(frkRuntimeDatas.size())) {
		return;
	}
	auto iter = frkRuntimeDatas.begin();
	for (int j = 0; j < nSelItem; j++) {
		iter++;
	}
	frkRuntimeDatas.erase(iter);
}

/**
 * if delete index, update related column runtime data through should be deleted index.
 * 
 * @param indexInfo
 */
void TableStructureSupplier::updateRelatedColumnsIfDeleteIndex(const IndexInfo &delIndexInfo)
{
	if (delIndexInfo.type.empty() || delIndexInfo.columns.empty()) {
		return;
	}
	auto columns = StringUtil::split(delIndexInfo.columns, ",");

	int n = static_cast<int>(colsRuntimeDatas.size());
	for (int i = 0; i < n; i++) {
		ColumnInfo &columnInfo = colsRuntimeDatas.at(i);
		
		auto iter = std::find_if(columns.begin(), columns.end(), [&columnInfo](std::string &column) {
			return columnInfo.name == column;
		});

		if (iter == columns.end()) {
			continue;
		}

		// change primary key column
		if (delIndexInfo.type == idxTypeList.at(1)) { // 1- Primary Key

			columnInfo.pk = 0;
			columnInfo.ai = 0;
		}		
	}
}

/**
 * if delete index, update related column runtime data through should be deleted index.
 * 
 * @param indexInfo
 */
void TableStructureSupplier::updateRelatedColumnsIfChangeIndex(const IndexInfo & changeIndexInfo)
{
	if (changeIndexInfo.type.empty() || changeIndexInfo.columns.empty()) {
		return;
	}
	auto columns = StringUtil::split(changeIndexInfo.columns, ",");

	size_t n = colsRuntimeDatas.size();
	for (size_t i = 0; i < n; i++) {
		ColumnInfo &columnInfo = colsRuntimeDatas[i];
		if (colsOrigDatas.size() < i + 1) {
			return;
		}
		ColumnInfo &origInfo = colsOrigDatas[i];		
		
		auto iter = std::find_if(columns.begin(), columns.end(), [&columnInfo](std::string &column) {
			return columnInfo.name == column;
		});

		if (iter == columns.end()) {
			continue;
		}

		// change primary key column
		if (changeIndexInfo.type == idxTypeList.at(1)) { // 1- Primary Key
			origInfo.pk = columnInfo.pk;
			origInfo.ai = columnInfo.ai;

			columnInfo.pk = 1;
			columnInfo.ai = changeIndexInfo.ai;
		} 		
	}
}

/**
 * if update column name, update related index column and foreign key column
 * 
 * @param origName
 * @param newName
 */
void TableStructureSupplier::updateRelatedColumnsIfChangeColumnName(std::string& origName, const std::string& newName)
{
	// update column name of indexes
	updateColumnNameInIdxRuntimeDatas(origName, newName);
	// update column name of foreign keys
	updateColumnNameInFrkRuntimeDatas(origName, newName);
}


bool TableStructureSupplier::compareDatas()
{
	return compareColsDatas() && compareIdxDatas() && compareFrkDatas();
}

/**
 * compare colsRuntimeDatas and colsOrigDatas.
 * 
 * @return 
 */
bool TableStructureSupplier::compareColsDatas()
{
	size_t rtSize = colsRuntimeDatas.size();
	size_t origSize = colsOrigDatas.size();
	if (rtSize != origSize) {
		return false;
	}
	if (colsRuntimeDatas.empty() && colsOrigDatas.empty()) {
		return true;
	}

	size_t n = colsOrigDatas.size();
	for (size_t i = 0; i < n; i++) {
		auto & rtItem = colsRuntimeDatas.at(i);
		auto & origItem = colsOrigDatas.at(i);
		if (!EntityUtil::compare(rtItem, origItem)) {
			return false;
		}
	}
	return true;
}


bool TableStructureSupplier::compareIdxDatas()
{
	size_t rtSize = idxRuntimeDatas.size();
	size_t origSize = idxOrigDatas.size();
	if (rtSize != origSize) {
		return false;
	}
	if (idxRuntimeDatas.empty() && idxOrigDatas.empty()) {
		return true;
	}

	size_t n = getIdxOrigDatas().size();
	for (size_t i = 0; i < n; i++) {
		auto & rtItem = idxRuntimeDatas.at(i);
		auto & origItem = idxOrigDatas.at(i);
		if (!EntityUtil::compare(rtItem, origItem)) {
			return false;
		}
	}
	return true;
}


bool TableStructureSupplier::compareFrkDatas()
{
	size_t rtSize = frkRuntimeDatas.size();
	size_t origSize = frkOrigDatas.size();
	if (rtSize != origSize) {
		return false;
	}
	if (frkRuntimeDatas.empty() && frkOrigDatas.empty()) {
		return true;
	}

	size_t n = frkOrigDatas.size();
	for (size_t i = 0; i < n; i++) {
		auto & rtItem = frkRuntimeDatas.at(i);
		auto & origItem = frkOrigDatas.at(i);
		if (!EntityUtil::compare(rtItem, origItem)) {
			return false;
		}
	}
	return true;
}

/**
 * change columns value in listView of TableIndexesPage when TableColumnsPage changing column name.
 * 
 * @param oldColumnName
 * @param newColumnName
 */
void TableStructureSupplier::updateColumnNameInIdxRuntimeDatas(const std::string & oldColumnName, const std::string & newColumnName)
{
	if (oldColumnName.empty() || newColumnName.empty() || oldColumnName == newColumnName) {
		return;
	}
	IndexInfoList & indexes = getIdxRuntimeDatas();
	int n = static_cast<int>(indexes.size());
	for (int i = 0; i < n; i++) {
		auto & item = indexes.at(i);
		auto columns = StringUtil::split(item.columns, ",");
		auto iter = std::find(columns.begin(), columns.end(), oldColumnName);
		if (iter == columns.end()) {
			continue;
		}
		(*iter) = newColumnName;
		item.columns = StringUtil::implode(columns, ",");
	}
}

/**
 * change columns value in listView of TableForeinkeysPage when TableColumnsPage changing column name.
 * 
 * @param oldColumnName
 * @param newColumnName
 */
void TableStructureSupplier::updateColumnNameInFrkRuntimeDatas(const std::string & oldColumnName, const std::string & newColumnName)
{
	if (oldColumnName.empty() || newColumnName.empty() || oldColumnName == newColumnName) {
		return;
	}
	ForeignKeyList & foreignKeys = getFrkRuntimeDatas();
	int n = static_cast<int>(foreignKeys.size());
	for (int i = 0; i < n; i++) {
		auto & item = foreignKeys.at(i);
		auto columns = StringUtil::split(item.columns, ",");
		auto iter = std::find(columns.begin(), columns.end(), oldColumnName);
		if (iter == columns.end()) {
			continue;
		}
		(*iter) = newColumnName;
		item.columns = StringUtil::implode(columns, ",");
	}
}
