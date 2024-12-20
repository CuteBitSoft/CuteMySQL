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

 * @file   SqlUtil.h
 * @brief  sql statement utils
 * 
 * @author Xuehan Qin
 * @date   2023-05-28
 *********************************************************************/
#pragma once
#include <regex>
#include <string>
#include <vector>
#include "core/entity/Entity.h"

// special charactor for SQL
// special characters for sql statement
#define quo "\""
#define lbrk "\r\n"
#define qua "'"
#define blk " "
#define cma ","

#define cmb "("
#define cme ")"
#define equ "="
#define edl ";"
#define brk "\n"
#define nil "NULL"
#define dot "."

class SqlUtil {
public:
	// select sql regex pattern
	
	static std::regex selectPat;
	static std::regex selectPat2;
	static std::regex selectPat3;
	static std::regex selectPat4;
	static std::regex explainPat;

	// where clause regex pattern
	static std::regex whereClausePat1;
	static std::regex whereClausePat2;

	// order clause regex pattern
	static std::regex orderClausePat;

	// limit clause regex patter
	static std::regex limitClausePat;

	// fourth clause regex pattern 
	static std::regex fourthClausePat;

	// table statement of select sql regex pattern
	static std::regex selectTablesPat;

	//primary key pattern of create table statement
	static std::regex primaryKeyPat;

	// field regex pattern
	static std::regex columnPat;

	// number suffix pattern of table name, use for table sharding
	static std::regex tblNumSuffixPat;

	static std::vector<std::string> tableTags;
	
	// parse sql 
	static bool isSelectSql(std::string & sql);
	static bool isPragmaStmt(std::string & sql, bool excludeEqual);
	static bool hasLimitClause(std::string & sql);
	static std::string getColumnName(std::string & str);
	static std::vector<std::string> getTablesFromSelectSql(const std::string & sql, std::vector<std::string> allTables);
	static std::vector<std::string> parseTablesFromTableClause(std::string & tblStmt);
	static std::string parseTableAliasFromSelectSql(const std::string & sql, const std::string & table, const std::vector<std::string> & tables);
	static std::string parseTableAliasFromSelectSqlUpWords(const std::vector<std::string>& upSqlWords, std::string & upTable, const UserTableStrings & tables);

	static std::string parsePrimaryKey(std::string & createTblSql);
	static std::string getWhereClause(const std::string & sql);
	static std::vector<std::string> getOrderClauses(const std::string & upsql);
	static std::vector<std::string> getSelectColumnsClause(const std::string & upsql);
	static std::vector<std::string> getOrderExpresses(const std::string & sql);
	static std::string getFourthClause(const std::string & sql);

	// make sql
	static std::string makeWhereClause(Columns & columns, RowItem &rowItem , SubItemValues &rowChangeVals);
	static std::string makeWhereClauseByPrimaryKey(std::string & primaryKey, Columns & columns, RowItem &rowItem, SubItemValues &rowChangeVals);
	static std::string makeWhereClauseByRowId(Columns & columns, RowItem &rowItem);
	static std::string makeInsertColumsClause(Columns & columns);
	static std::string makeInsertValuesClause(RowItem & rowItem);

	// make table name
	static std::string makeTmpTableName(const std::string & tblName, int number = 1, const std::string & prefix = std::string("ctsqlite_tmp_"));
	
	// parse columns by create table DDL
	static ColumnInfoList parseColumnsByTableDDL(const std::string & createTblSql);

	// parse constraints by create table DDL exclude foreign key
	static IndexInfoList parseConstraints(const std::string & createTblSql);
	static IndexInfo parseConstraintsForPrimaryKey(const std::string & createTblSql);
	static std::vector<std::string> splitTableDDLColumnClausesToLines(std::string str, bool bTrim = true);

	// parse foreign keys by create table DDL
	static ForeignKeyList parseForeignKeysByTableDDL(const std::string & createTblSql);

	static std::string replaceNumOfSuffixInTblName(const std::string & tblName, const std::string & after);

	// parse table clause from sql, such as "SELECT * FROM [tbl_1 as name_1, tbl_2 as name_2] WHERE ..."
	static TableAliasVector parseTableClauseFromSelectSql(const std::string & upSelectSql);
	static TableAliasVector parseTableClauseFromUpdateSql(const std::string & upSelectSql);

	static IndexInfo parseCreateIndex(const std::string & createIndexSql);
	
	
	static std::vector<std::pair<std::string, std::string>> parseWhereNullColumnsFromSelectSqlUpWords(
		const std::vector<std::string>& upSqlWords, 
		const std::vector<std::pair<std::string, std::string>> & allAliases);
private:
	static IndexInfo parseConstraintFromLine(const std::string& line);
	static IndexInfo parseLineToPrimaryKey(const std::string& line, bool isConstaintLine = true);
	static IndexInfo parseLineToUnique(const std::string& line, bool isConstaintLine = true);
	static IndexInfo parseLineToCheck(const std::string& line, bool isConstaintLine = true);
	static std::pair<std::string, uint8_t> getColumnAndAiFromPrimaryKeyLine(const std::string &line);
	static std::string getConflictClauseFromConstraintLine(const std::string& line);
	static std::string getColumnsFromUniqueLine(const std::string &line);
	static std::string getExpressionByCheckLine(const std::string &line);

	static ColumnInfo parseColumnFromLine(const std::string& line);
	static ForeignKey parseForeignKeyFromLine(const std::string& line);
	static std::string getPartialClauseFromForeignKeyLine(const std::string& line);
};
