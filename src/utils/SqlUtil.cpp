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

 * @file   SqlUtil.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-05-28
 *********************************************************************/
#include "SqlUtil.h"
#include <chrono>
#include "StringUtil.h"

std::regex SqlUtil::selectPat("^select\\s+(.*)\\s+from\\s+(.*)\\s*(where .*)?", std::regex::icase);
std::regex SqlUtil::selectPat2("^with\\s(.*)?\\s?select\\s+(.*)\\s+from\\s+(.*)\\s*(where .*)?", std::regex::icase);
std::regex SqlUtil::selectPat3("select\\s+(.*)\\s+from\\s+(.*)\\s*(order .*)+", std::regex::icase);
std::regex SqlUtil::selectPat4("(select\\s+(.*))\\s+from(.*)", std::regex::icase);
std::regex SqlUtil::explainPat("^explain(query|\\s|plan)+(.*)+", std::regex::icase); 

std::regex SqlUtil::whereClausePat1("((where)\\s+.*)\\s+(order|group|limit|having|window)+(.*)?", std::regex::icase);
std::regex SqlUtil::whereClausePat2("(where .*\\)?)+", std::regex::icase);

//std::regex SqlUtil::orderClausePat("(select.*((order\\s+by)\\s+.*)(limit|offset)+(.*)?)|(\\(select.*(order\\s+by.*)\\))|(select.*(order\\s+by\\s+.*)+)", std::regex::icase);

std::regex SqlUtil::limitClausePat("(limit .*)+", std::regex::icase);

std::regex SqlUtil::fourthClausePat("((order|group|limit|having|window)\\s+.*)+", std::regex::icase);

std::regex SqlUtil::primaryKeyPat("\\s+primary\\s+key\\s?\\(?(\\\"?([^(\\s|\\\"|\\s)]+)\\\"?\\s?)", std::regex::icase);

std::regex SqlUtil::columnPat("(.*)\\s+\\[(.*)\\]+");

std::regex SqlUtil::tblNumSuffixPat("([0-9]+)$");



std::vector<std::string> SqlUtil::tableTags{ "as", "left", "right", "inner", "cross", "full", "outer", "natural", "join" };


bool SqlUtil::isSelectSql(const std::string & sql)
{
	if (sql.empty()) {
		return false;
	}

	if (std::regex_search(sql, SqlUtil::selectPat) 
		|| std::regex_search(sql, SqlUtil::explainPat)
		|| std::regex_search(sql, SqlUtil::selectPat2)) {
		return true;
	}
	return false;
}

bool SqlUtil::isPragmaStmt(const std::string & sql, bool excludeEqual)
{
	if (sql.empty()) {
		return false;
	}
	std::string upsql = StringUtil::toupper(sql);
	if (excludeEqual) {
		return upsql.find("PRAGMA") == 0 && upsql.find_last_of('=') == std::string::npos;
	}
	return upsql.find("PRAGMA") == 0;
}

bool SqlUtil::hasLimitClause(const std::string & sql)
{
	if (sql.empty()) {
		return false;
	}

	if (std::regex_search(sql, SqlUtil::limitClausePat)) {
		return true;
	}
	return false;
}

/**
 * Fetch the column name from str such as "name [hello world]".
 * 
 * @param str
 * @return 
 */
std::string SqlUtil::getColumnName(const std::string & str)
{
	if (str.empty()) {
		return "";
	}

	std::smatch results;
	if (std::regex_search(str, results,  SqlUtil::columnPat)) {
		return results[1];
	}
	return "";
}

/**
 * Fetch the table(s) name from select query sql statement.
 * 
 * @param selectSql - The select statement
 * @param tables - All tables of databases 
 * @return the vector of table(s)
 */
std::vector<std::string> SqlUtil::getTablesFromSelectSql(const std::string & selectSql, const std::vector<std::string> & allTables)
{
	std::vector<std::string> tbls;
	if (selectSql.empty()) {
		return tbls;
	}
	
	std::smatch results;
	if (!std::regex_search(selectSql, results, SqlUtil::selectPat)) {
		return tbls;
	}
	// The second param from match results is the tables statement,
	//   such as : table1 as tbl1, table2 as tbl2 or table1 t1 left join table2 t2 
	std::string tblClause = results[2];
	if (tblClause.empty()) {
		return tbls;
	}
	
	auto vec = parseTablesFromTableClause(tblClause);
	for (auto tbl : allTables) {
		auto iterator = std::find_if(vec.begin(), vec.end(), [&tbl](std::string & str) {
			std::string lowtbl = StringUtil::tolower(tbl);
			std::string lowstr = StringUtil::tolower(str);
			return lowtbl == lowstr;
		});

		if (iterator != vec.end()) {
			tbls.push_back(tbl);
		}
	}

	return tbls;
}

/**
 * Fetch the table name(s) from table clause such as "select * from [tblClause]"..
 * 
 * @param tblStmt
 * @return The vector of table name(s) such as {tbl1,tbl2}
 */
std::vector<std::string> SqlUtil::parseTablesFromTableClause(const std::string & tblClause)
{
	std::vector<std::string> tbls;
	if (tblClause.empty()) {
		return tbls;
	}
	std::string str(tblClause);
	str = StringUtil::replace(str, std::string(","), std::string(" "));
	auto vec = StringUtil::split(str, " ");
	for (auto tag : vec) {
		if (tag.empty()) {
			continue;
		}

		std::string lowtag = StringUtil::tolower(tag);
		auto iterator = std::find_if(tableTags.begin(), tableTags.end(), [&lowtag](std::string & str) {
			return lowtag == str;
		});

		if (iterator != tableTags.end()) {
			continue;
		}
		tag = StringUtil::cutParensAndQuotes(tag);
		tbls.push_back(tag);
	}
	return tbls;
}

/**
 * Fetch the primary key clause such as 'PRIMARY KEY("id" AUTOINCREMENT)'.
 * 
 * @param createTblSql
 * @return std::string such as id
 */
std::string SqlUtil::parsePrimaryKey(const std::string & createTblSql)
{
	std::smatch results;
	if (!std::regex_search(createTblSql, results, SqlUtil::primaryKeyPat)) {
		return "";
	}
	std::string primaryKey = results[2];
	return primaryKey;
}

/**
 * Fetch the WHERE clause such as "select * from [tbl clause] [where clause] [fourth clause : order|having|window|group...]"...
 * 
 * @param sql
 * @return The string of whereClause
 */
std::string SqlUtil::getWhereClause(const std::string & sql)
{
	if (sql.empty()) {
		return "";
	}
	std::smatch results;
	if (!std::regex_search(sql, results, SqlUtil::selectPat)) {
		return "";
	}
	if (results.size() < 3 || results[3].matched == false) {
		return "";
	}
	std::smatch whereResults;
	std::string whereStmt = results[3];
	std::string whereClause;

	while (!whereStmt.empty() && std::regex_search(whereStmt, whereResults, SqlUtil::whereClausePat1)) {
		if (whereResults[1].matched == false) {
			return "";
		}
		whereClause = whereResults[1];
		if (std::regex_search(whereClause, SqlUtil::fourthClausePat)) {
			whereStmt = whereClause;
			continue;
		}
		break;
	}

	if (whereClause.empty() && std::regex_search(whereStmt, whereResults, SqlUtil::whereClausePat2)) {
		if (whereResults[1].matched == false) {
			return "";
		}
		whereClause = whereResults[1];
	}

	if (whereClause.empty() || StringUtil::toupper(whereClause.substr(0, 5)) != "WHERE") {
		return "";
	}
	return whereClause;
}

std::regex SqlUtil::orderClausePat("(((order\\s+by)\\s+.+)(limit|offset)+(.*)?)|(\\(.*(order\\s+by.*)\\))|(order\\s+by\\s+.*)", std::regex::icase);

/**
 * Fetch the ORDER clause such as "select * from [tbl clause] [where clause] [order clause] [fourth clause : limit...]"...
 * 
 * @param sql
 * @return The string of whereClause
 */
std::vector<std::string> SqlUtil::getOrderClauses(const std::string & sql)
{
	std::vector<std::string> result;
	if (sql.empty()) {
		return result;
	}
	std::string upsql = StringUtil::toupper(sql);
	if (upsql.find("ORDER") == std::string::npos) {
		return result;
	}

	
	std::smatch orderResults;
	std::string orderClause;
	std::string orderInSql;

	auto iter_begin = std::sregex_iterator(sql.begin(), sql.end(), SqlUtil::orderClausePat);
    auto iter_end= std::sregex_iterator();

	for (std::sregex_iterator iter = iter_begin; iter != iter_end; ++iter) {
		orderResults = *iter;
		bool foundFirst = orderResults[2].matched;
		bool foundSecond = orderResults[7].matched;
		bool foundThird = orderResults[8].matched;
		if (foundFirst == false && foundSecond == false && foundThird == false) {
			break;
		}
		orderClause.clear();
		// Fetch valid order by clause
		if (foundFirst) {
			orderClause = orderResults[2];
		} else if (foundSecond) {
			orderClause = orderResults[7];			
		} else if (foundThird) {
			orderClause = orderResults[8];
		}
		if (orderClause.empty() || orderClause == sql 
			|| StringUtil::toupper(orderClause.substr(0, 5)) != "ORDER") {
			continue;
		}
		
		auto iter2 = std::find_if(result.begin(), result.end(), [&orderClause](auto & express) {
			return orderClause == express;
		});

		if (iter2 == result.end()) {
			result.push_back(orderClause);
		}
	}

	return result;
}

/**
 * Fetch the ORDER clause such as "select * from [tbl clause] [where clause] [order clause] [fourth clause : limit...]"...
 * 
 * @param sql
 * @return The string of whereClause
 */
std::vector<std::string> SqlUtil::getSelectColumnsClause(const std::string & upsql)
{
	std::vector<std::string> result;
	if (upsql.empty()) {
		return result;
	}

	size_t startPos = 0;
	for (;;) {
		auto selectPos = upsql.find("SELECT",startPos);
		auto fromPos = upsql.find("FROM",startPos);
		auto selectSize = 6;
		auto fromSize = 4;
		if (selectPos == std::string::npos || fromPos == std::string::npos) {
			return result;
		}
		std::string selectColumnClause = upsql.substr(selectPos + selectSize, fromPos - selectPos - selectSize);
		if (!selectColumnClause.empty()) {
			result.push_back(selectColumnClause);
		}
		startPos = fromPos + fromSize;
	}
	return result;
	/*
	auto iter_begin = std::sregex_iterator(upsql.begin(), upsql.end(), SqlUtil::selectPat4);
    auto iter_end= std::sregex_iterator();

	for (std::sregex_iterator iter = iter_begin; iter != iter_end; ++iter) {
		std::smatch columnResults = *iter;

		if (columnResults[1].matched == false) {
			break;
		}
		selectColumnClause = columnResults[1];
		while (!selectColumnClause.empty() && std::regex_search(selectColumnClause, columnResults, SqlUtil::selectPat4)) {
			if (columnResults[1].matched == false) {
				break;
			}
			selectColumnClause = columnResults[1];
		}

		if (!selectColumnClause.empty() && selectColumnClause == sql) {
			continue;
		}

		if (selectColumnClause.substr(0, 6) != "SELECT") {
			return result;
		}
		std::string cols = selectColumnClause.substr(7);
		result.push_back(cols);
	}
	
	return result;*/
}

/**
 * .
 * 
 * @param sql
 * @return 
 */
std::vector<std::string> SqlUtil::getOrderExpresses(const std::string & sql)
{
	std::vector<std::string> orderClauses = getOrderClauses(sql);
	std::vector<std::string> result;
	for (auto & orderClause : orderClauses) {
		if (orderClause.empty()) {
			continue;
		}
		std::string upOrderClause = StringUtil::toupper(orderClause);
		size_t byPos = upOrderClause.find("BY");
		if (byPos == std::string::npos) {
			continue;
		}
		std::string express = orderClause.substr(byPos + 2);
		result.push_back(express);
	}
	
	return result;
}

/**
 * Fetch the fourth clause such as "select * from [tbl clause] [where clause] [fourth clause : order|group|limit|having|window...]"...
 * 
 * @param sql
 * @return The string of fourthClause
 */
std::string SqlUtil::getFourthClause(const std::string & sql)
{
	std::smatch results;
	if (!std::regex_search(sql, results, SqlUtil::fourthClausePat)) {
		return "";
	}
	if (results.size() == 0 || results[1].matched == false) {
		return "";
	}

	std::string fouthClause = results[1];
	if (fouthClause.empty()  || (StringUtil::toupper(fouthClause.substr(0, 5)) != "ORDER"
		&& StringUtil::toupper(fouthClause.substr(0, 5)) != "GROUP"
		&& StringUtil::toupper(fouthClause.substr(0, 5)) != "LIMIT"
		&& StringUtil::toupper(fouthClause.substr(0, 6)) != "HAVING"
		&& StringUtil::toupper(fouthClause.substr(0, 6)) != "WINDOW")) {
		return "";
	}
	return fouthClause;
}

/**
 * Make sql where clause from columns and values.
 * @condition:
 *  1.columns and rowItem are not empty.
 *  2.columns.size must equals rowItem.size
 * 
 * @param runtimeColumns columns
 * @param rowItem row 
 * @param rowChangeVals The change subitem index in only this selected row
 * @return 
 */
std::string SqlUtil::makeWhereClause(const Columns & columns, const RowItem &rowItem, SubItemValues &rowChangeVals)
{
	if (columns.empty() || rowItem.empty() 
		|| columns.size() != rowItem.size()) {
		return "";
	}

	std::string whereClause(" WHERE ");
	int n = static_cast<int>(columns.size());
	for (int i = 0; i < n; i++) {
		if (i > 0) {
			whereClause.append(" AND ");
		}

		std::string val;
		if (rowChangeVals.empty()) {
			val = StringUtil::escapeSql(rowItem.at(i));
		} else {
			auto iter = std::find_if(rowChangeVals.begin(), rowChangeVals.end(), [&i](SubItemValue & subItem) {
				auto nSubItem = subItem.iSubItem ;
				return nSubItem == i;
			});

			val = iter == rowChangeVals.end() ? 
				StringUtil::escapeSql(rowItem.at(i)) 
				: StringUtil::escapeSql((*iter).origVal);
		}
		
			
		whereClause.append(blk).append(quo).append(columns.at(i)).append(quo)
			.append("=").append(qua).append(val).append(qua);
	}

	return whereClause;
}

/**
 * make where clause only through primary key.
 * 
 * @param primaryKey 
 * @param columns
 * @param rowItem
 * @param nChangeSubItem
 * @param subItemOrigVal
 * @return 
 */
std::string SqlUtil::makeWhereClauseByPrimaryKey(const std::string & primaryKey, const Columns & columns, const RowItem &rowItem, SubItemValues &rowChangeVals)
{
	if (columns.empty() || rowItem.empty() 
		|| columns.size() != rowItem.size()) {
		return "";
	}

	std::string whereClause(" WHERE ");
	int n = static_cast<int>(columns.size());
	for (int i = 0; i < n; i++) {
		std::string column = columns.at(i);
		if (column != primaryKey) {
			continue;
		}
		
		std::string val;
		if (rowChangeVals.empty()) {
			val = StringUtil::escapeSql(rowItem.at(i));
		} else {
			auto iter = std::find_if(rowChangeVals.begin(), rowChangeVals.end(), [&i](SubItemValue & subItem) {
				auto nSubItem = subItem.iSubItem - 1;
				return nSubItem == i;
			});

			val = iter == rowChangeVals.end() ? 
				StringUtil::escapeSql(rowItem.at(i)) 
				: StringUtil::escapeSql((*iter).origVal);
		}
		whereClause.append(blk).append(quo).append(columns.at(i)).append(quo)
			.append("=").append(qua).append(val).append(qua);
		break;
	}

	return whereClause;
}

std::string SqlUtil::makeWhereClauseByRowId(const Columns & columns, const RowItem &rowItem)
{
	if (columns.empty() || rowItem.empty() 
		|| columns.size() != rowItem.size()) {
		return "";
	}
	std::string rowIdColumnName = columns.at(0);
	std::string rowIdVal = rowItem.at(0);
	assert(rowIdColumnName == "_ct_sqlite_rowid" && !rowIdVal.empty());
	std::string whereClause(" WHERE ");
	whereClause.append(quo).append("ROWID").append(quo) // such as "ct_sqlite_row_id = 11"
		.append(equ).append(qua).append(rowIdVal).append(qua);
	return whereClause;
}

/**
 * make insert sql columns clause such as id, name1, name2, email...
 * 
 * @param columns The columns vector
 * @return 
 */
std::string SqlUtil::makeInsertColumsClause(const Columns & columns)
{
	if (columns.empty()) {
		return "";
	}

	std::string columnsClause(" (");
	int n = static_cast<int>(columns.size());
	for (int i = 1; i < n; i++) {
		std::string column = columns.at(i);
		if (i > 1) {
			columnsClause.append(", ");
		}
		columnsClause.append(quo).append(column).append(quo);
	}
	columnsClause.append(") ");
	return columnsClause;
}

/**
 * make insert sql columns clause such as id, name1, name2, email...
 * 
 * @param rowItem The row vector
 * @return 
 */
std::string SqlUtil::makeInsertValuesClause(const RowItem & rowItem)
{
	if (rowItem.empty()) {
		return "";
	}

	std::string valuesClause(" VALUES (");
	int n = static_cast<int>(rowItem.size());
	for (int i = 1; i < n; i++) {
		std::string val = StringUtil::escapeSql(rowItem.at(i));
		if (i > 1) {
			valuesClause.append(", ");
		}
		if (val == "< AUTO >" || val == "< NULL >") {
			valuesClause.append("NULL");
		} else {
			valuesClause.append(qua).append(val).append(qua);
		}
		
	}
	valuesClause.append(")");

	return valuesClause;
}

std::string SqlUtil::makeTmpTableName(const std::string & tblName, int number,  const std::string & prefix /*= std::string("ctsqlite_tmp_")*/)
{
	std::string result = prefix;
	result.append(tblName).append("_").append(std::to_string(number));
	return result;
}

/**
 * Parse all columns from create table DDL.
 * 
 * @param createTblSql
 * @return IndexInfoList
 */
ColumnInfoList SqlUtil::parseColumnsByTableDDL(const std::string & createTblSql)
{
	if (createTblSql.empty()) {
		return ColumnInfoList();
	}

	size_t paren_first = createTblSql.find_first_of('(');
	size_t paren_end = createTblSql.find_last_of(')');
	if (paren_first == std::string::npos || paren_end == std::string::npos) {
		return ColumnInfoList();
	}

	std::string columnsAndConstrains = createTblSql.substr(paren_first + 1, paren_end - paren_first - 1);
	std::vector<std::string> lineList = splitTableDDLColumnClausesToLines(columnsAndConstrains,true);

	ColumnInfoList result;
	for (auto & line : lineList) {		
		ColumnInfo columnInfo = SqlUtil::parseColumnFromLine(line);
		if (columnInfo.name.empty()) {
			continue;
		}
		result.push_back(columnInfo);
	}
	return result;
}

/**
 * Parse all constraints from create table DDL.
 * 
 * @param createTblSql
 * @return IndexInfoList
 */
IndexInfoList SqlUtil::parseConstraints(const std::string & createTblSql)
{
	if (createTblSql.empty()) {
		return IndexInfoList();
	}

	size_t paren_first = createTblSql.find_first_of('(');
	size_t paren_end = createTblSql.find_last_of(')');
	if (paren_first == std::string::npos || paren_end == std::string::npos) {
		return IndexInfoList();
	}

	std::string columnsAndConstrains = createTblSql.substr(paren_first + 1, paren_end - paren_first - 1);
	std::vector<std::string> lineList = splitTableDDLColumnClausesToLines(columnsAndConstrains,true);

	IndexInfoList result;
	for (auto & line : lineList) {		
		IndexInfo indexInfo = SqlUtil::parseConstraintFromLine(line);
		if (indexInfo.columns.empty()) {
			continue;
		}
		// Get rid of the duplicated indexes
		auto iter = std::find_if(result.begin(), result.end(), [&indexInfo](IndexInfo & item) {
			return item.type == indexInfo.type && item.columns == indexInfo.columns;
		});
		if (iter == result.end()) {
			result.push_back(indexInfo);
		}		
	}
	return result;
}

/**
 * Parse primary key constraints only from create table sql.
 * 
 * @param createTblSql
 * @return IndexInfoList
 */
IndexInfo SqlUtil::parseConstraintsForPrimaryKey(const std::string & createTblSql)
{
	if (createTblSql.empty()) {
		return IndexInfo();
	}

	size_t paren_first = createTblSql.find_first_of('(');
	size_t paren_end = createTblSql.find_last_of(')');
	if (paren_first == std::string::npos || paren_end == std::string::npos) {
		return IndexInfo();
	}

	std::string columnsAndConstrains = createTblSql.substr(paren_first + 1, paren_end - paren_first - 1);
	// std::vector<std::string> lineList = StringUtil::split(columnsAndConstrains, ",", true);
	std::vector<std::string> lineList = splitTableDDLColumnClausesToLines(columnsAndConstrains,true);
		
	for (auto & line : lineList) {
		std::string upline = StringUtil::toupper(line);
		if (upline.find("PRIMARY") != std::string::npos) {
			return SqlUtil::parseLineToPrimaryKey(line, true);
		}		
	}
	return IndexInfo();
}

/**
 * Split columns and index to text lines in the create table DDL.
 * Such as :
*      "is_delete" INTEGER NOT NULL DEFAULT (0), // will be a line
*      "created_at" DATETIME NOT NULL DEFAULT (''), // will be a line
*      "updated_at" DATETIME NOT NULL DEFAULT (''), // will be a line
*       PRIMARY KEY(id,analysis_type,sample_lib_id) // will be a line
 * @param str
 * @param bTrim
 * @return string vector 
 */
std::vector<std::string> SqlUtil::splitTableDDLColumnClausesToLines(const std::string & str, bool bTrim /*= true*/)
{
	std::string pattern = ","; // column or index clause line split character
	
	std::string::size_type pos;
	std::vector<std::string> result;
	std::string strv = str;
	strv.append(pattern); // 扩展字符串以方便操作

	// not found the pattern character between ignore_begin and ignore_end
	wchar_t ignore_begin = '('; 
	wchar_t ignore_end = ')'; 
	
	size_t size = static_cast<int>(strv.size());
	size_t from = 0;
	for (size_t i = 0; i < size; i++) {
		size_t ignore_begin_pos = strv.find_first_of(ignore_begin, i);
		size_t ignore_end_pos = strv.find_first_of(ignore_end, i);
		pos = strv.find(pattern, i);
		if (pos < size && (pos < ignore_begin_pos || pos > ignore_end_pos)) {
			std::string s = strv.substr(from, pos - from);
			if (s.empty()) {
				i = pos + pattern.size() - 1;
				continue;
			}
			if (bTrim && !s.empty()) {
				StringUtil::trim(s);
			}
			result.push_back(s);
			i = pos + pattern.size() - 1;
			from = pos + pattern.size();
		} else {
			i = ignore_end_pos ;
		}
	}
	return result;
}

ForeignKeyList SqlUtil::parseForeignKeysByTableDDL(const std::string & createTblSql)
{
	if (createTblSql.empty()) {
		return ForeignKeyList();
	}

	size_t paren_first = createTblSql.find_first_of('(');
	size_t paren_end = createTblSql.find_last_of(')');
	if (paren_first == std::string::npos || paren_end == std::string::npos) {
		return ForeignKeyList();
	}

	std::string columnsAndConstrains = createTblSql.substr(paren_first + 1, paren_end - paren_first - 1);
	std::vector<std::string> lineList = splitTableDDLColumnClausesToLines(columnsAndConstrains, true);

	ForeignKeyList result;
	for (auto & line : lineList) {
		ForeignKey foreignKey = SqlUtil::parseForeignKeyFromLine(line);
		if (foreignKey.columns.empty()) {
			continue;
		}
		result.push_back(foreignKey);
	}
	return result;
}

/**
 * Parse one line to IndexInfo object, line text such as "PRIMARY KEY ("uid")"
 * 
 * @param line
 * @return 
 */
IndexInfo SqlUtil::parseConstraintFromLine(const std::string& line)
{
	IndexInfo result;
	std::string upline = StringUtil::toupper(line);
	if (upline.find("PRIMARY") != std::string::npos) {
		IndexInfo indexInfo = SqlUtil::parseLineToPrimaryKey(line, true); // from constrain only
		if (indexInfo.name.empty() && indexInfo.type.empty()) {
			indexInfo = SqlUtil::parseLineToPrimaryKey(line, false); // from column only
		}
		return indexInfo;
	}
	if (upline.find("UNIQUE") != std::string::npos) {
		return SqlUtil::parseLineToUnique(line, true);
	}

	if (upline.find("CHECK") != std::string::npos) {
		return SqlUtil::parseLineToCheck(line, true);
	}
	return result;
}

/**
 * Parse the primary key line, you can parse the column line or the constraint line through specified isConstainLine.
 * 
 * @param line
 * @param isConstaintLine - true : parse the constraint line only, false : parse the column line only
 * @return 
 */
IndexInfo SqlUtil::parseLineToPrimaryKey(const std::string& line, bool isConstaintLine)
{
	if (line.empty()) {
		return IndexInfo();
	}
	std::vector<std::string> words = StringUtil::splitByBlank(line, true);
	if (words.empty()) {
		return IndexInfo();
	}

	IndexInfo result;
	std::string first = StringUtil::toupper(words.at(0));
	if (isConstaintLine && first == "CONSTRAINT") {
		// such as : CONSTRAINT "idx_name1" Primary Key("id" AUTOINCREMENT)
		result.name = StringUtil::cutParensAndQuotes(words.at(1));
	} 
	
	bool isFound = false;
	if (isConstaintLine) { // parse the constraint line only
		if ((first == "PRIMARY" || first.find("PRIMARY") == 0) // such as : Primary Key("id" AUTOINCREMENT)
			|| (words.size() > 2 && StringUtil::toupper(words.at(2)) == "PRIMARY")){ // such as : CONSTRAINT "idx_name1" Primary Key("id" AUTOINCREMENT)
			result.type = "Primary Key";
			result.pk = 1;
			auto pair = getColumnAndAiFromPrimaryKeyLine(line);
			if (pair.first.empty()) {
				return IndexInfo();
			}
			result.columns = pair.first;
			result.ai = pair.second;
			// return conflict-clause
			result.partialClause = getConflictClauseFromConstraintLine(line);
			isFound = true;
		} else {
			return IndexInfo();
		}
	} else { // parse the column line only
		std::string upline = StringUtil::toupper(line);		
		// the column line contains PRIMARY KEY, such as : "id" INTEGER NOT NULL PRIMARY KEY
		result.columns = StringUtil::cutParensAndQuotes(words.at(0));
		result.type = "Primary Key";
		result.pk = 1;
		if (upline.find("AUTOINCREMENT") != std::string::npos) {
			result.ai = 1;
		}
	}
	

	result.sql = line;
	result.seq = std::chrono::system_clock::now();
	return result;

}

/**
 * Parse the Unique line, you can parse the column line or the constraint line with specified isConstainLine=true/false.
 * 
 * @param line
 * @param isConstaintLine - true : parse the constraint line only, false : parse the column line only
 * @return 
 */
IndexInfo SqlUtil::parseLineToUnique(const std::string& line, bool isConstaintLine)
{
	if (line.empty()) {
		return IndexInfo();
	}
	std::vector<std::string> words = StringUtil::splitByBlank(line, true);
	if (words.empty()) {
		return IndexInfo();
	}

	IndexInfo result;
	std::string first = StringUtil::toupper(words.at(0));
	if (isConstaintLine && first == "CONSTRAINT") {
		// such as : CONSTRAINT "idx_name2"  UNIQUE("id","name"...)
		result.name = StringUtil::cutParensAndQuotes(words.at(1));	}
	
	if (isConstaintLine) { // parse the constraint line only
		if ( (first == "UNIQUE" || first.find("UNIQUE") == 0) // such as : UNIQUE("id","name"...)
			|| (words.size() > 2  && StringUtil::toupper(words.at(2)).find("UNIQUE") == 0)) { // such as : CONSTRAINT "idx_name2"  UNIQUE("id","name"...)
			result.type = "Unique";
			result.un = 1;
			auto columns = getColumnsFromUniqueLine(line);
			if (columns.empty()) {
				return IndexInfo();
			}
			result.columns = columns;
			// return conflict-clause
			result.partialClause = getConflictClauseFromConstraintLine(line);
		} else {
			return IndexInfo();
		}
	} else { // parse the column line only
		// the column line contains UNIQUE, such as : "id" INTEGER NOT NULL PRIMARY KEY
		result.columns = StringUtil::cutParensAndQuotes(words.at(0));
		result.type = "Unique";
		result.un = 1;
		std::string upline = StringUtil::toupper(line);
		if (upline.find("AUTOINCREMENT") != std::string::npos) {
			return IndexInfo();
		} else if (upline.find("PRIMARY") != std::string::npos) {
			return IndexInfo();
		}			
	}

	result.sql = line;
	result.seq = std::chrono::system_clock::now();
	return result;

}

/**
 * Parse the Check line, you can parse the column line or the constraint line with specified isConstainLine=true/false.
 * 
 * @param line
 * @param isConstaintLine - true : parse the constraint line only, false : parse the column line only
 * @return 
 */
IndexInfo SqlUtil::parseLineToCheck(const std::string& line, bool isConstaintLine)
{
	if (line.empty()) {
		return IndexInfo();
	}
	std::vector<std::string> words = StringUtil::splitByBlank(line, true);
	if (words.empty()) {
		return IndexInfo();
	}

	IndexInfo result;
	std::string first = StringUtil::toupper(words.at(0));
	if (isConstaintLine && first == "CONSTRAINT") {
		// such as : CONSTRAINT "idx_chk"  CHECK("id" > 8)
		result.name = StringUtil::cutParensAndQuotes(words.at(1));
	}
	
	if (isConstaintLine) { // parse the constraint line only
		if ( (first == "CHECK") // such as : UNIQUE("id","name"...)
			|| (words.size() > 2 && StringUtil::toupper(words.at(2)) == "CHECK") ) { // such as : CONSTRAINT "idx_check"  CHECK("id" > 8)
			result.type = "Check";
			result.ck = 1;
			auto expression = getExpressionByCheckLine(line);
			if (expression.empty()) {
				return IndexInfo();
			}
			result.columns = expression;
		} else {
			return IndexInfo();
		}
	} else { // parse the column line only
		std::string upline = StringUtil::toupper(line);		
		// the column line contains PRIMARY KEY, such as : "id" INTEGER NOT NULL PRIMARY KEY
		result.columns = StringUtil::cutParensAndQuotes(words.at(0));
		result.type = "Check";
		result.ck = 1;
		if (upline.find("AUTOINCREMENT") != std::string::npos) {
			return IndexInfo();
		} else if (upline.find("PRIMARY") != std::string::npos) {
			return IndexInfo();
		} else if (upline.find("UNIQUE") != std::string::npos) {
			return IndexInfo();
		}			
	}

	result.sql = line;
	result.seq = std::chrono::system_clock::now();
	return result;
}

/**
 * Fetch the column name and auto increment in the Primary key line. 
 * such as "Primary Key("id" AUTOINCREMENT)"
 * 
 * @param line
 * @return pair<std::string : column, uint_8 : ai> 
 *              - first:primary key column name, 
 *              - second: 1 if AUTOINCREMENT, 0 if not AUTOINCREMENT
 */
std::pair<std::string, uint8_t> SqlUtil::getColumnAndAiFromPrimaryKeyLine(const std::string &line)
{
	if (line.empty()) {
		return std::pair<std::string, int>();
	}

	size_t paren_first = line.find_first_of('(');
	size_t paren_end = line.find_last_of(')');
	if (paren_first == std::string::npos || paren_end == std::string::npos) {
		return std::pair<std::string, int>();
	}

	std::string parenClause = line.substr(paren_first + 1, paren_end - paren_first - 1);
	std::vector<std::string> words = StringUtil::splitByBlank(parenClause, true);
	if (words.empty()) {
		return std::pair<std::string, int>();
	}
	std::string columns;
	if (words.at(0).find_first_of(',') == std::string::npos) {
		 // not found ',', single column ,such as "Primary key(id AUTOINCREMENT)"
		columns = StringUtil::cutParensAndQuotes(words.at(0));
	} else { 
		// found ',', multiple columns, such as "Primary key(id,name,...)"
		std::vector<std::string> columnVect = StringUtil::split(words.at(0), ",", true);

		// Get gid of the symbol 
		int i = 0;
		for (auto & colItem : columnVect) {
			colItem = StringUtil::cutParensAndQuotes(colItem);
			StringUtil::trim(colItem);
			if (colItem.empty()) {
				continue;
			}
			if (i++ > 0) {
				columns.append(",");
			}
			columns.append(colItem);
		}
	}
	uint8_t ai = 0;
	if (words.size() > 1 && StringUtil::toupper(words.at(1)) == "AUTOINCREMENT") {
		ai = 1;
	}
	return { columns, ai };
}

/**
 * return conflict-clause from the primary-key line or a unique-line
 *  conflict-clause : such as "ON CONFLICT ROLLBACK/ABORT/FAIL/IGNORE/REPLACE"
 * 
 * @param line
 * @return 
 */
std::string SqlUtil::getConflictClauseFromConstraintLine(const std::string& line)
{
	std::string partialClause;
	if (line.empty()) {
		return partialClause;
	}
	std::string upline = StringUtil::toupper(line);
	if (upline.find("PRIMARY") == std::string::npos 
		&& upline.find("UNIQUE") == std::string::npos) {
		return partialClause;
	}

	size_t onPos = line.find("ON");
	if (onPos == std::string::npos) {
		return partialClause;
	}
	partialClause = line.substr(onPos, -1);
	return partialClause;
}

/**
 * Fetch the column names in the unique line. 
 * such as : CONSTRAINT "idx_name2"  UNIQUE("id","name"...)
 * 
 * @param line
 * @return columns
 */
std::string SqlUtil::getColumnsFromUniqueLine(const std::string &line)
{
	if (line.empty()) {
		return "";
	}

	size_t paren_first = line.find_first_of('(');
	size_t paren_end = line.find_last_of(')');
	if (paren_first == std::string::npos || paren_end == std::string::npos) {
		return "";
	}

	std::string result = line.substr(paren_first + 1, paren_end - paren_first - 1);
	result = StringUtil::cutParensAndQuotes(result);
	return result;
}

/**
 * Fetch the column names in the unique line. 
 * such as : CONSTRAINT "idx_check"  CHECK("id" > 8)
 * 
 * @param line
 * @return columns
 */
std::string SqlUtil::getExpressionByCheckLine(const std::string &line)
{
	if (line.empty()) {
		return "";
	}

	size_t paren_first = line.find_first_of('(');
	size_t paren_end = line.find_last_of(')');
	if (paren_first == std::string::npos || paren_end == std::string::npos) {
		return "";
	}

	std::string parenClause = line.substr(paren_first + 1, paren_end - paren_first - 1);
	std::vector<std::string> words = StringUtil::splitByBlank(parenClause, true);
	if (words.empty()) {
		return "";
	}
	std::string result;
	int i = 0;
	for (auto & word : words) {
		auto column = StringUtil::cutParensAndQuotes(word);
		StringUtil::trim(column);
		if (column.empty()) {
			continue;
		}
		if (i++ > 0) {
			result.append(",");
		}
		result.append(column);
	}
	return result;
}


ColumnInfo SqlUtil::parseColumnFromLine(const std::string& line)
{
	ColumnInfo result;
	std::string upline = StringUtil::toupper(line);
	auto words = StringUtil::splitByBlank(line, true);
	if (words.empty() || words.at(0).empty()) {
		return result;
	}

	// exclude the constrains
	std::string & first = words.at(0);
	std::string upfirst = StringUtil::toupper(first);
	if (upfirst == "CONSTRAINT"
		|| upfirst == "PRIMARY"
		|| upfirst == "UNIQUE" 
		|| upfirst == "CHECK"
		|| upfirst == "FOREIGN") {
		return result;
	}

	// first word must be column name
	std::string columnName = StringUtil::cutParensAndQuotes(first);
	if (columnName.empty()) {
		return result;
	}
	result.name = columnName;

	if (words.size() > 1 && !words.at(1).empty()) {
		result.type = words.at(1);
	}
	if (upline.find("NOT NULL") != std::string::npos) {
		result.isNullable = 0;
	}

	size_t defPos = -1;
	if ((defPos = upline.find("DEFAULT")) != std::string::npos) {
		char def[] = "DEFAULT";
		wchar_t ch = StringUtil::nextNotBlankChar(line, def, defPos);
		if (ch == '(') {
			result.defVal = StringUtil::inSymbolString(line, '(', ')', defPos);
		} else {
			auto iter = std::find_if(words.begin(), words.end(), [](std::string &str) {
				return StringUtil::toupper(str) == "DEFAULT";
			});
			while (iter != words.end() && ++iter != words.end()) {
				if ((*iter).empty()) {
					continue;
				}
				result.defVal = (*iter);
				break;
			}
		}		
	}

	if (upline.find("PRIMARY") != std::string::npos) {
		result.pk = 1;
	}

	if (upline.find("UNIQUE") != std::string::npos) {
		result.un = 1;
	}

	if (upline.find("AUTOINCREMENT") != std::string::npos) {
		result.ai = 1;
	}

	if ((defPos = upline.find("CHECK")) != std::string::npos) {
		size_t paren_first = line.find_first_of('(', defPos);
		size_t paren_end = line.find_first_of(')', defPos);
		if (paren_first != std::string::npos && paren_end != std::string::npos) {
			result.checks = StringUtil::inSymbolString(line, '(', ')', defPos);
		} else {
			auto iter = std::find_if(words.begin(), words.end(), [](std::string &str) {
				return StringUtil::toupper(str) == "DEFAULT";
			});
			while (iter != words.end()) {
				if ((*iter).empty()) {
					iter++;
				}
				result.checks = (*iter);
				break;
			}
		}
	}

	return result;
}

/**
 * parse ddl line to a ForeignKey object
 * foreig key supported such as :
 *  // CONSTRAINT `name` FOREIGN KEY (`dog_id`) REFERENCES `asia_dog` (`id`) ON DELETE SET NULL ON UPDATE RESTRICT
 * 
 * @param line
 * @return 
 */
ForeignKey SqlUtil::parseForeignKeyFromLine(const std::string& line)
{
	ForeignKey result;
	std::string upline = StringUtil::toupper(line);
	auto words = StringUtil::splitByBlank(line, true);
	if (words.empty() || words.at(0).empty()) {
		return result;
	}

	// exclude the constrains
	std::string first = StringUtil::toupper(words.at(0));
	if (first == "PRIMARY"
		|| first == "UNIQUE" 
		|| first == "CHECK") {
		return result;
	}

	// 1. fetch name
	if (first == "CONSTRAINT") {
		// such as : CONSTRAINT "foreign_key_idx1"  FOREIGN KEY("uid")
		result.name = StringUtil::cutParensAndQuotes(words.at(1));
	}
	
	size_t defPos = -1;
	if (words.size() <= 2 || 
		((first != "FOREIGN") // such as : FOREIGN KEY("id","name"...)
		&& (words.size() > 2 && StringUtil::toupper(words.at(2)) != "FOREIGN"))) { // such as : CONSTRAINT "idx_check"  FOREIGN KEY("uid")...
		return result;
	}
		
	// 2. fetch columns
	defPos = upline.find("FOREIGN");
	char word[]{ "FOREIGN KEY" };
	wchar_t ch = StringUtil::nextNotBlankChar(upline, word, defPos);
	if (ch == '(') {
		result.columns = StringUtil::inSymbolString(line, '(', ')', defPos);
		StringUtil::trim(result.columns);
		result.columns = StringUtil::cutParensAndQuotes(result.columns);
	}

	// 2. fetch referenced table and columns
	auto iter = std::find_if(words.begin(), words.end(), [](const std::string & word) {
		return StringUtil::toupper(word) == "REFERENCES";
	});
	if (iter != words.end() && ++iter != words.end()) {
		result.referencedTable = StringUtil::cutParensAndQuotes((*iter));

		defPos = upline.find("REFERENCES");
		result.referencedColumns = StringUtil::inSymbolString(line, '(', ')', defPos);
		StringUtil::trim(result.referencedColumns);
		result.referencedColumns = StringUtil::cutParensAndQuotes(result.referencedColumns);
	}
		
	// 3. fetch on update / on delete
	size_t n = words.size();
	for (size_t i = 0; i < n - 2; i++) {
		auto word = StringUtil::toupper(words.at(i));
		std::string next1 = (i + 1) < n ? StringUtil::toupper(words.at(i + 1)) : "";
		std::string next2 = (i + 2) < n ? StringUtil::toupper(words.at(i + 2)) : "";
		std::string next3 = (i + 3) < n ? StringUtil::toupper(words.at(i + 3)) : "";
		if (word == "ON" && next1 == "UPDATE") {				
			if (next2 == "SET" || next2 == "ON") {
				result.onUpdate = next2 + " " + next3;
			} else {
				result.onUpdate = next2;
			}
		} else if (word == "ON" && next1 == "DELETE") {
			if (next2 == "SET" || next2 == "ON") {
				result.onDelete = next2 + " " + next3;
			} else {
				result.onDelete = next2;
			}
		}
	}
	result.partialClause = getPartialClauseFromForeignKeyLine(line);
	result.seq = std::chrono::system_clock::now();
	return result;
}

std::string SqlUtil::getPartialClauseFromForeignKeyLine(const std::string& line)
{
	std::string partialClause;
	if (line.empty()) {
		return partialClause;
	}
	std::string upline = StringUtil::toupper(line);
	if (upline.find("FOREIGN") == std::string::npos ) {
		return partialClause;
	}

	size_t notDefferPos = upline.find("NOT DEFERRABLE");
	size_t fromPos = -1;
	if (notDefferPos == std::string::npos) {
		size_t defferPos = upline.find("DEFERRABLE");
		if (defferPos == std::string::npos) {
			return partialClause;
		}
		fromPos = defferPos;
	} else {
		fromPos = notDefferPos;
	}
	
	partialClause = line.substr(fromPos, -1);
	return partialClause;
}

/**
 * Replace the number of suffix in the table name. use for table sharding
 * such as : "analysis_23" => "analysis_{n}"; 
 * 
 * @param tblName
 * @param after
 * @return 
 */
std::string SqlUtil::replaceNumOfSuffixInTblName(const std::string & tblName, const std::string & after)
{
	if (tblName.empty() || after.empty()) {
		return tblName;
	}
	if (tblName.find(after) != std::string::npos) {
		return tblName;
	}

	std::regex_constants::match_flag_type fonly =
        std::regex_constants::format_first_only;
	
	std::string str = std::regex_replace(tblName, SqlUtil::tblNumSuffixPat, after, fonly);
	if (str.find(after) != std::string::npos) {
		return str;
	}
	str.append("_").append(after);
	return str;
}

/**
 * parse table clause from select sql, such as "SELECT * FROM [tbl_1 as alias_1, tbl_2 as alias_2] WHERE ..."
 * 
 * @param selectSql
 * @param endWord - substring begin FROM to [endWord]
 * @return  - up case vector ,such as : [{tbl:"ANALYSIS", alis:"A"},{...}]
 */

TableAliasVector SqlUtil::parseTableClauseFromSelectSql(const std::string & upSql)
{
	TableAliasVector result;
	if (upSql.empty()) {
		return result;
	}
	size_t fromPos = upSql.find(" FROM ");
	size_t onPos, wherePos, groupPos, orderPos, limitPos;
	if (fromPos == std::string::npos) {
		return result;
	}

	std::string tblClause;
	size_t endPos = upSql.size() -1;
	if ((onPos = upSql.rfind(" ON ")) != std::string::npos && onPos > fromPos) {
		endPos = onPos;
	}else if ((wherePos = upSql.rfind(" WHERE ")) != std::string::npos && wherePos > fromPos) {
		endPos = wherePos;
	} else if ((groupPos = upSql.rfind(" GROUP ")) != std::string::npos && groupPos > fromPos) {
		endPos = groupPos;
	} else if ((orderPos = upSql.rfind(" ORDER ")) != std::string::npos && orderPos > fromPos) {
		endPos = orderPos;
	} else if ((limitPos = upSql.rfind(" LIMIT ")) != std::string::npos && limitPos > fromPos) {
		endPos = limitPos;
	}

	tblClause = upSql.substr(fromPos + 6, endPos - fromPos - 6);  // 4 - size of " FROM "
	StringUtil::trim(tblClause);

	if (tblClause.empty()) {
		return result;
	}

	std::string pattern;
	if (tblClause.find(",") != std::string::npos ) {
		pattern = ",";
	} else if (tblClause.find(" JOIN ") != std::string::npos ) {
		pattern = " JOIN ";
	}

	std::vector<std::string> tbls;
	if (!pattern.empty()) {
		// [tbl_1 as alias_1, tbl_2 as alias_2]; [tbl_1 as alias_1 left join tbl_2 as alias_2]
		tbls = StringUtil::split(tblClause, pattern); 
	} else {
		tbls.push_back(tblClause);
	}
	
	for (auto & tbl : tbls) {
		TableAlias item;
		auto tblWords = StringUtil::splitByBlank(tbl); // tbl_1
		item.tbl = StringUtil::cutParensAndQuotes(tblWords.at(0));
		if (tblWords.size() > 2 && tblWords.at(1) == "AS") { // tbl_1 as alias_1
			item.alias = StringUtil::cutParensAndQuotes(tblWords.at(2));
		} else if (tblWords.size() > 1) { // tbl_1 alias_1
			item.alias = StringUtil::cutParensAndQuotes(tblWords.at(1));
		}
		result.push_back(item);
	}
	return result;
}

/**
 * parse table clause from update sql, such as "SELECT * FROM [tbl_1 as alias_1, tbl_2 as alias_2] WHERE ..."
 * 
 * @param selectSql
 * @param endWord - substring begin FROM to [endWord]
 * @return 
 */
TableAliasVector SqlUtil::parseTableClauseFromUpdateSql(const std::string & upSql)
{
	TableAliasVector result;
	if (upSql.empty()) {
		return result;
	}
	size_t npos = std::string::npos;
	size_t updatePos = upSql.find("UPDATE");
	if (updatePos == npos) {
		return result;
	}

	size_t onPos, setPos;
	std::string tblClause;
	size_t endPos = upSql.size() -1;
	if ((onPos = upSql.rfind(" ON ")) != npos && onPos > updatePos) {
		endPos = onPos;
	}else if ((setPos = upSql.rfind(" SET ")) != npos && setPos > updatePos) {
		endPos = setPos;
	}

	tblClause = upSql.substr(updatePos + 6, endPos - updatePos - 6); // 6 - size of "UPDATE"
	StringUtil::trim(tblClause);

	if (tblClause.empty()) {
		return result;
	}

	std::string pattern;
	if (tblClause.find(",") != std::string::npos ) {
		pattern = ",";
	} 

	std::vector<std::string> tbls;
	if (!pattern.empty()) {
		// [tbl_1 as alias_1, tbl_2 as alias_2];
		tbls = StringUtil::split(tblClause, pattern); 
	} else {
		tbls.push_back(tblClause);
	}
	for (auto & tbl : tbls) {
		TableAlias item;
		auto tblWords = StringUtil::splitByBlank(tbl); // tbl_1
		item.tbl = StringUtil::cutParensAndQuotes(tblWords.at(0));
		if (tblWords.size() > 2 && tblWords.at(1) == "AS") { // tbl_1 as alias_1
			item.alias = StringUtil::cutParensAndQuotes(tblWords.at(2));
		} else if (tblWords.size() > 1) { // tbl_1 alias_1
			item.alias = StringUtil::cutParensAndQuotes(tblWords.at(1));
		} 
		result.push_back(item);
	}
	return result;
}


/**
 * Pars the create index DDL statement to IndexInfo.
 * For example : CREATE UNIQUE INDEX "name" ON "analysis_sample_class_16" ("sample_lib_id","inspection_id");
 * 
 * @param createIndexSql
 * @return 
 */
IndexInfo SqlUtil::parseCreateIndex(const std::string & createIndexSql)
{	
	if (createIndexSql.empty()) {
		return IndexInfo();
	}

	std::string upline = StringUtil::toupper(createIndexSql);
	std::vector<std::string> upwords = StringUtil::splitByBlank(upline);
	if (upwords.empty() || upwords.at(0) != "CREATE" || (upwords.at(1) != "INDEX" && upwords.at(2) != "INDEX")) {
		return IndexInfo();
	}
	IndexInfo result;
	result.type = "Index";
	// Find the index name and table name and unique
	size_t n = upwords.size();
	for (size_t i = 0; i < n; i++) {
		auto & word = upwords.at(i);
		if (word == "ON") {
			if (upwords.at(i - 1) != "INDEX" && upwords.at(i - 1) != "EXISTS") {
				std::vector<std::string> words = StringUtil::splitByBlank(createIndexSql);
				result.name = StringUtil::cutParensAndQuotes(words.at(i - 1));
			}
		}
		if (word == "UNIQUE") {
			result.un = 1;
		}
	}
	// Find the columns, it must be between in the first ()
	size_t paren_first = upline.find_first_of('(');
	size_t paren_end = upline.find_first_of(')');
	if (paren_first == std::string::npos || paren_end == std::string::npos) {
		return IndexInfo();
	}	
	auto columns = createIndexSql.substr(paren_first + 1, paren_end - paren_first - 1);
	columns = StringUtil::cutParensAndQuotes(columns);
	auto columnVector = StringUtil::split(columns, ",");
	columns = StringUtil::implode(columnVector, ",");
	result.columns = StringUtil::cutParensAndQuotes(columns);

	// Find the partial clause, it must be start with "WHERE"
	auto wherePos = upline.find("WHERE");
	if (wherePos != std::string::npos) {
		result.partialClause = createIndexSql.substr(wherePos);
	}

	return result;
}

/**
 * Parse the sql, get the alias of specified table.
 * 
 * @param sql - sql statement
 * @param table - specified table
 * @param tables - all table names of sql
 * @return UPCASE alias/table
 */
std::string SqlUtil::parseTableAliasFromSelectSql(const std::string & sql, const std::string & table, const std::vector<std::string> & tables)
{
	std::string uptable = StringUtil::toupper(table);
	size_t tblLen = tables.size();
	if (tables.empty()) {
		return uptable;
	}
	std::string alias;	
	std::string upsql = StringUtil::toupper(sql);
	upsql = StringUtil::replace(upsql, ",", " ");

	std::vector<std::string> words = StringUtil::splitByBlank(upsql);
	return parseTableAliasFromSelectSqlUpWords(words, uptable, tables);
}

/**
 * Parse the sql up case words vector, get the alias of specified table..
 * 
 * @param upWords - Up case word vector of sql statement split by blank
 * @param upTable - Table name of up case
 * @param tables - All of table names for sql
 * @return table alias such as : "customer as c" return "c", "customer c" return "c"
 */
std::string SqlUtil::parseTableAliasFromSelectSqlUpWords(const std::vector<std::string>& upWords, const std::string & upTable, const UserTableStrings & tables)
{
	if (upWords.empty() || upTable.empty() || tables.empty()) {
		return upTable;
	}
	size_t n = upWords.size();
	for (size_t i = 0; i < n - 1; i++) {
		auto & word = upWords.at(i);
		if (word != upTable) {
			continue;
		}
		
		if (i < n - 2 && upWords.at(i + 1) == "AS" && !(upWords.at(i + 2).empty())) {
			return upWords.at(i + 2);
		}
		auto & nextWord = upWords.at(i + 1);
		if (nextWord == "NATURAL" || nextWord == "JOIN"
			|| nextWord == "LEFT" || nextWord == "RIGHT"
			|| nextWord == "FULL" || nextWord == "INNER"
			|| nextWord == "CROSS" || nextWord == "OUTER"
			|| nextWord == "WHERE" || nextWord == "ORDER" 
			|| nextWord == "ON" || nextWord == "GROUP"
			|| nextWord == "HAVING" || nextWord == "LIMIT"
			|| nextWord == "OFFSET" || nextWord == "WINDOW"
			|| nextWord == "(" || nextWord == "(SELECT") {
			return upTable;
		}

		auto iter = std::find_if(tables.begin(), tables.end(), [&nextWord] (auto & tblName) {
			return nextWord == StringUtil::toupper(tblName);
		});
		if (iter == tables.end()) {
			return nextWord;
		}
		
	}
	return "";
}

/**
 * Parse columns of "is null" or "is not null" in the sql statement
 * 
 * @param sqlWords - word vector of be splitted sql statement with blank
 * @param allAliases - pair vector, params: pair.first - table, pair.second - table alias
 * @return columns tuple, tuple params: pair.first - table name, pair.second - columns or is not null columns
 */
std::vector<std::pair<std::string, std::string>> SqlUtil::parseWhereNullColumnsFromSelectSqlUpWords(
	const std::vector<std::string>& upSqlWords, 
	const std::vector<std::pair<std::string, std::string>> & allAliases)
{
	std::vector<std::pair<std::string, std::string>> results;
	int n = static_cast<int>(upSqlWords.size());
	for (int i = 0; i < n - 1; i++) {
		auto & word = upSqlWords.at(i);
		auto & nextWord = upSqlWords.at(i + 1);
		if ((word == "IS" && (nextWord == "NOT" || nextWord == "NULL"))
			|| word == "ISNULL" || word == "NOTNULL") {
			auto & columnWord = upSqlWords.at(i - 1);
			auto pos = columnWord.find_first_of('.');
			if (pos != std::string::npos) {
				// columnWord, such as "t.name" or "tbl.name"
				std::string tblAlias = columnWord.substr(0, pos); // table alias or table name, "t.name"--get->"t", "tbl.name"--get->"tbl"
				std::string columnName = columnWord.substr(pos + 1); // column name, "tbl.name"--get->"name"
				for (auto & pair : allAliases) {
					// pair.first - alias, pair.second - table 
					if (StringUtil::toupper(pair.first) == tblAlias || StringUtil::toupper(pair.second) == tblAlias) {
						 // found, add to results, {tableName, columnName}
						results.push_back({ pair.second, columnName });
					}
				}
			} else {
				const std::string & columnName = columnWord;
				if (i == 0) {
					continue;
				}
				// Find backward from i-1
				for (int j = i - 1; j >= 0; j--) {
					auto & prevWord = upSqlWords.at(j);
					for (auto & pair : allAliases) {
						// pair.first - alias, pair.second - table
						if (StringUtil::toupper(pair.first) == prevWord || StringUtil::toupper(pair.second) == prevWord) {	
							 // found, add to results, {tableName, columnName}
							results.push_back({ pair.second, columnName });
						}
					}// for allAliases
				} // for upSqlWords, Find backward from i-1
			}  // if pos != std::string::npos
		} // if ((word == "IS"..
	} // upSqlWords

	return results;
}

