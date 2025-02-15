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

 * @file   TableStructureSupplier.h
 * @brief  This class supply runtime data for TableStructurePage,TableTabView,
 * ,TableColumnsPage,TableIndexesPage objects and their's Adapter objects
 * 
 * @author Xuehan Qin
 * @date   2023-10-21
 *********************************************************************/
#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include "core/entity/Entity.h"
#include "ui/database/rightview/common/QPageSupplier.h"

class TableStructureSupplier : public QPageSupplier<TableStructureSupplier> {
public:
	TableStructureSupplier();
	// TableColumnsPage list view header columns settings
	static Columns colsHeadColumns;
	const static std::vector<int> colsHeadSizes;
	const static std::vector<int> colsHeadFormats;
	const static std::vector<std::string> colsDataTypeList;

	// TableIndexesPage list view header columns settings
	static Columns idxHeadColumns;
	const static std::vector<int> idxHeadSizes;
	const static std::vector<int> idxHeadFormats;
	const static std::vector<std::string> idxTypeList;

	// TableForeignkeysPage list view header columns settings
	static Columns frkHeadColumns;
	const static std::vector<int> frkHeadSizes;
	const static std::vector<int> frkHeadFormats;
	const static std::vector<std::string> frkOnUpdateTypeList;
	const static std::vector<std::string> frkOnDeleteTypeList;

	std::string getOrigTblName() const { return origTblName; }
	void setOrigTblName(std::string val) { origTblName = val; }

	ColumnInfoList & getColsRuntimeDatas() { return colsRuntimeDatas; }
	void setColsRuntimeDatas(ColumnInfoList & val) { colsRuntimeDatas = val; }
	ColumnInfo & getColsRuntimeData(int nSelItem);
	void eraseColsRuntimeData(int nSelItem);

	IndexInfoList & getIdxRuntimeDatas() { return idxRuntimeDatas; }
	void setIdxRuntimeDatas(IndexInfoList & val) { idxRuntimeDatas = val; }
	IndexInfo & getIdxRuntimeData(int nSelItem);
	void eraseIdxRuntimeData(int nSelItem);

	ForeignKeyList & getFrkRuntimeDatas() { return frkRuntimeDatas; }
	void setFrkRuntimeDatas(const ForeignKeyList & val) { frkRuntimeDatas = val; }
	ForeignKey & getFrkRuntimeData(int nSelItem);
	void eraseFrkRuntimeData(int nSelItem);

	ColumnInfoList & getColsOrigDatas() { return colsOrigDatas; }
	void setColsOrigDatas(const ColumnInfoList val) { colsOrigDatas = val; }
	void eraseColsOrigData(int nSelItem);

	const IndexInfoList & getIdxOrigDatas() const { return idxOrigDatas; }
	void setIdxOrigDatas(const IndexInfoList & val) { idxOrigDatas = val; }
	const ForeignKeyList & getFrkOrigDatas() const { return frkOrigDatas; }
	void setFrkOrigDatas(const ForeignKeyList & val) { frkOrigDatas = val; }

	// update related columns
	void updateRelatedColumnsIfDeleteIndex(const IndexInfo &indexInfo);	
	void updateRelatedColumnsIfChangeIndex(const IndexInfo & changeIndexInfo);	
	void updateRelatedColumnsIfChangeColumnName(std::string& origName, const std::string& newName);
	
	TableStructurePageType getActivePageType() const { return activePageType; }
	void setActivePageType(TableStructurePageType val) { activePageType = val; }

	// compare data(s)
	bool compareDatas();
	bool compareColsDatas();
	bool compareIdxDatas();
	bool compareFrkDatas();

	
private:
	TableStructurePageType activePageType = TABLE_COLUMNS_PAGE;
	// store the runtime data of the table column info(s)
	ColumnInfoList colsRuntimeDatas;
	// store the runtime data of the index(s) settings
	IndexInfoList idxRuntimeDatas;
	// store the runtime data of the foreign key(s) settings
	ForeignKeyList frkRuntimeDatas;
	
	// Original variables, for alter table
	// store the original table name, for alter table comparing with runtimeTblName
	std::string origTblName;
	// store the original table column infos, for alter table comparing with colsRuntimeDatas
	ColumnInfoList colsOrigDatas; 
	// store the original table index infos, for alter table comparing with idxRuntimeDatas
	IndexInfoList idxOrigDatas; 
	// store the original table foreign-key infos, for alter table comparing with frkRuntimeDatas
	ForeignKeyList frkOrigDatas; 

	void updateColumnNameInIdxRuntimeDatas(const std::string & oldColumnName, const std::string & newColumnName);
	void updateColumnNameInFrkRuntimeDatas(const std::string & oldColumnName, const std::string & newColumnName);
};
