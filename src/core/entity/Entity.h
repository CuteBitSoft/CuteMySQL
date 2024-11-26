/*****************************************************************//**
 * Copyright 2024 Xuehan Qin 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and

 * limitations under the License.

 * @file   Entity.h
 * @brief  The entities struct of the system database tables or ui used
 * 
 * @author Xuehan Qin
 * @date   2023-06-10
 *********************************************************************/
#ifndef _ENTIRY_H_
#define _ENTIRY_H_
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <chrono>
#include <wx/colour.h>

typedef struct {
	std::string name;
	std::string val;
} SysInit;
/**
 * 系统加载配置项列表
 */
typedef std::vector<SysInit> SysInitList;

// 配置项(<key,val>)，存放于数据库表sys_init以及系统的ini文件中，同事具备排序功能
typedef std::vector<std::pair<std::string, std::string>> Setting;

// INI的配置项，有分组功能<group, Setting> 
typedef std::unordered_map<std::string, Setting> IniSetting;

// connects
typedef struct _UserConnect {
	uint64_t id = 0;
	std::string name;
	std::string host;
	std::string userName = "root";
	std::string password;
	int isPasswordSaved = 1;
	int port = 3306;
	std::string databases;
	int isUseCompressed = 0;
	int isReadOnly = 0;
	int idleTimeout = 0;
	int keepAlive = 0;
	int isSshTunnel = 0;
	std::string sshHost;
	int sshPort = 22;
	std::string sshUserName;
	int isSshPassword = 1;
	int isSshPublickey = 0;
	std::string sshPassword;
	int isSshPasswordSaved = 1;
	std::string sshPullickeyPassphrase;
	int isSshPullickeyPassphraseSaved = 1;
	std::string sshPrivatekeyFilepath;
	int isUseSsl = 0;
	std::string sslCaCertificate;
	std::string sslCipher;
	int isSslAuth = 0;
	std::string sslClientKey;
	std::string sslClientCertificate;
	int isUseGlobalValue = 0;
	std::string sqlMode;
	std::string initCommand;
	uint64_t sort = 0;
	std::string createdAt;
	std::string updatedAt;
} UserConnect;
typedef std::vector<UserConnect> UserConnectList;

// databases
typedef struct _UserDb{
	uint64_t connectId = 0;
	std::string name;
	std::string ref;
} UserDb;

typedef std::vector<UserDb> UserDbList;

// tables,views,triggers,index
typedef struct _UserTable {
	std::string type;
	std::string name;
	std::string tblName;
	std::string catalog;
	std::string schema;
	std::string engine;
	uint64_t version;
	std::string rowFormat;
	std::string collaction;
	std::string options;
	std::string comment;
	std::string createTime;
	std::string updateTime;
	std::string checkTime;
} UserTable, UserView, UserIndex;

typedef std::vector<UserTable> UserTableList;
typedef std::vector<UserView> UserViewList;
typedef std::vector<UserIndex> UserIndexList;

typedef enum {
	ROUTINE_PROCEDURE = 7,
	ROUTINE_FUNCTION = 11,
	ROUTINE_EVENT = 12,
} RoutineType;

typedef struct _UserRoutine {
	std::string objectType;
	std::string catalog;
	std::string schema;
	std::string name;
	int type;
	std::string ddl;
	std::string remarks;
	/*
	std::string specificName;
	std::string language;
	std::string sqlDataAccess;
	std::string isDeterministic;
	std::string securityType;
	std::string paramList;
	std::string returns;
	std::string body;
	std::string definer;
	std::string created;
	std::string modified;
	std::string sqlMode;
	std::string comment;
	std::string characterSetClient;
	std::string collationConnection;
	std::string dbCollation;
	std::string bodyUtf8;*/

} UserRoutine, UserProcedure, UserFunction, UserTrigger;
typedef std::list<UserRoutine> UserRoutineList, UserProcedureList, UserFunctionList, UserTriggerList;

// Event
typedef struct {
	std::string schema;
	std::string name;
	std::string definer;
	std::string timeZone;
	std::string type;
	std::string excuteAt;
	std::string intervalValue;
	std::string intervalField;
	std::string starts;
	std::string ends;
	std::string status;
	std::string originator;
	std::string characterSetClient;
	std::string collationConnection;
	std::string dbCollation;

} UserEvent;
typedef std::list<UserEvent> UserEventList;

typedef struct _PragmaIndexColumn {
	int seqno = 0;
	int cid = 0;
	std::string name;
} PragmaIndexColumn;
typedef std::vector<PragmaIndexColumn> PragmaIndexColumns;

// table fields
typedef struct _ColumnInfo {
	uint32_t cid = 0;
	uint8_t notnull = 0; //not null
	uint8_t pk = 0; // primary key
	uint8_t ai = 0; // auto increment
	uint8_t un = 0; // unsigned

	std::string name;
	std::string type;
	std::string defVal;
	std::string checks;
	
	std::chrono::system_clock::time_point seq; //system assign a sequence, for alter table
} ColumnInfo;
typedef std::vector<ColumnInfo> ColumnInfoList;

typedef struct _IndexInfo {
	std::string name; // constrain name
	std::string type; // Primary Key,Unique,Foreign Key,Checks,Index
	uint8_t pk = 0;  // primary key
	uint8_t ai = 0; // Auto increment
	uint8_t un = 0; // unique
	uint8_t fk = 0; // foreign key
	uint8_t ck = 0; // checks
	std::string columns; // columns
	std::string partialClause; // The partial index clause
	std::string sql;
	std::chrono::system_clock::time_point seq; //system assign a sequence, for alter table
} IndexInfo;
typedef std::vector<IndexInfo> IndexInfoList;

typedef struct _ForeignKey {
	std::string name; // constrain name
	std::string type = "Foreign Key"; // must be Foreign Key
	std::string columns; // columns
	std::string referencedTable;
	std::string referencedColumns;
	std::string onUpdate;
	std::string onDelete;
	std::string partialClause; // The partial index clause
	std::chrono::system_clock::time_point seq; //system assign a sequence, for alter table
} ForeignKey;
typedef std::vector<ForeignKey> ForeignKeyList;

//Export to CSV params
typedef struct _ExportCsvParams {
	std::string csvFieldTerminatedBy;
	std::string csvFieldEnclosedBy;
	std::string csvFieldEscapedBy;
	std::string csvLineTerminatedBy;
	bool hasColumnOnTop = false;
} ExportCsvParams;

//Export to Excel XML params
typedef struct _ExportExcelParams {
	int excelComlumnMaxSize = 0;
	int excelDecimalPlaces = 0;
} ExportExcelParams;

//Export to sql params
typedef struct {
	std::string sqlSetting;
} ExportSqlParams, StructAndDataParams;

typedef std::vector<std::string> ExportSelectedColumns;

// the data structure for show in list view or export
typedef std::vector<std::string> RowItem, Columns, Functions, UserTableStrings, WhereExpresses;
// data items list
typedef std::list<RowItem> DataList;

//select sql - limit clause params
typedef struct _LimitParams {
	bool checked = false;
	int offset = 0;
	int rows = 0;
} LimitParams;

typedef struct {
	int iItem;
	int iSubItem;
	std::string origVal;
	std::string newVal;
} SubItemValue;
typedef std::vector<SubItemValue> SubItemValues;

//Insert statement params for export as sql 
typedef struct _InsertStatementParams {
	bool retainColumn = false;
	bool multiRows = false;
} InsertStatementParams;

//Insert statement params for export as sql 
typedef struct {
	std::string param;
} TblStatementParams;

// Execute sql result
typedef struct _ResultInfo {	
	int effectRows = 0;
	std::string execTime;
	std::string transferTime;
	std::string totalTime;
	std::string sql;
	int code = 0;
	std::string msg;

	// Extend for sql log
	uint64_t id = 0;
	uint64_t userDbId = 0;
	int top = 0;
	std::string createdAt;
	int64_t data = 0;
} ResultInfo, SqlLog;
// Store the sql log list for execute result
typedef std::list<SqlLog> SqlLogList;

// performance analysis report
typedef struct _PerfAnalysisReport {
	uint64_t id = 0;
	uint64_t userDbId = 0;
	uint64_t sqlLogId = 0;

	std::string createdAt;
	std::string updatedAt;
} PerfAnalysisReport;
// Store the perf analysis report list for execute result
typedef std::list<PerfAnalysisReport> PerfAnalysisReportList;

// Explain query plan
typedef struct _ExplainQueryPlan {
	int id = 0;
	int parent = 0;
	int notused = 0;
	std::string detail;
} ExplainQueryPlan;
typedef std::vector<ExplainQueryPlan> ExplainQueryPlans;

// Explain sql to this struct
typedef struct _ByteCodeResult {
	//BASIC INFORMATION
	int no; // OpenRead/OpenWrite p1
	uint64_t userDbId;
	std::string addr;
	std::string name;
	std::string type; // index, table, view, trigger
	uint64_t rootPage;

	// FOR INDEX ANALYSIS
	std::vector<std::pair<int, std::string>> useIndexes; // useing index in this table, pair item params: first(int) - index no, second(std::string) - index name
	Columns whereColumns; // where clause used columns in this table
	Columns orderColumns; // order clause used columns in this table
	std::vector<std::pair<int, std::string>> whereIndexColumns; // using index columns of where clause in this table, pair item params: first(int) - index no, second(std::string) - column name
	std::vector<std::pair<int, std::string>> orderIndexColumns; // using index columns of order clause in this table, pair item params: first(int) - index no, second(std::string) - column name
	
	Columns mergeColumns; // merge whereColumns  and orderColumns
	std::string coveringIndexName; // covering index name
	std::vector<std::pair<int, std::string>> coveringIndexColumns; // covering index columns of wherer and orde clause in this table, pair item params: first(int) - index no, second(std::string) - column name

	// FOR JOIN ANALYSIS
	WhereExpresses whereExpresses;
	uint64_t effectRows;

} ByteCodeResult;
typedef std::vector<ByteCodeResult> ByteCodeResults;

// Select column for select sql statement
typedef struct _SelectColumn {
	int regNo = 0; // register no
	std::string fullName;
	std::string name;
	std::string alias;
	std::string tblName;
	std::string tblAlias;	
} SelectColumn;
typedef std::vector<SelectColumn> SelectColumns;

typedef struct _ByteCodeUseColumn {
	int tblNo; // OpenRead/OpenWrite p1
	std::string tblName;
	int idxNo;
	std::string idxName;
	std::string columnName;
} ByteCodeUseColumn;
typedef std::vector<ByteCodeUseColumn> ByteCodeUseColumns;
#endif

// Support create/modify table
typedef enum {
	QUERY_DATA,
	TABLE_DATA,
	CREATE_VIEW,
	CREATE_TRIGGER,
	MODIFY_VIEW,
	MODIFY_TRIGGER,
	NEW_TABLE,
	MOD_TABLE,
} PageOperateType;

typedef enum _StructAndDataSetting {
	UNKOWN = 0,
	STRUCT_ONLY = 1,
	DATA_ONLY = 2,
	STRUCTURE_AND_DATA = 3
} StructAndDataSetting;

typedef enum {
	TABLE_COLUMNS_PAGE = 0,
	TABLE_INDEXS_PAGE = 1,
	TABLE_FOREIGN_KEYS_PAGE = 2
} TableStructurePageType;

typedef enum _AnalysisPageType {
	SQL_LOG_PAGE,
	ANALYSIS_REPORT_PAGE,
	STORE_ANALYSIS_PAGE,
	DATABASE_PARAMS_PAGE
} AnalysisPageType;

typedef enum _SqlClauseType {
	FROM_CLAUSE,
	WHERE_CLAUSE,
	ORDER_CLAUSE,
	INSERT_COLUMNS_CLAUSE,
	INSERT_VALUES_CLAUSE,
	GROUP_CLAUSE,
	HAVING_CLAUSE,
	LIMIT_CLAUSE,

	COVERING_INDEXES
} SqlClauseType;

// Explain sql statement column idx
typedef enum _ExplainColumn {
	EXP_ADDR = 0, // addr
	EXP_OPCODE, // opcode
	EXP_P1,	// p1
	EXP_P2,	// p2
	EXP_P3,	// p3
	EXP_P4,	// p4
	EXP_P5,	// p5
	EXP_COMMENT, //comment
} ExplainColumn;

// use for select statement such as "tbl1 as m1 left join tbl2 as m2"
typedef struct _TableAlias {
	std::string tbl;
	std::string alias;
} TableAlias;
typedef std::vector<TableAlias> TableAliasVector;

typedef struct _TblIdxSpaceUsed {
	std::string name;				// Name of a table or index in the database file
	std::string tblName;			// Name of associated table
	uint8_t isIndex = 0;			// TRUE if it is an index, false for a table
	uint8_t isWithoutRowid = 0;		// TRUE if WITHOUT ROWID table
	uint64_t nentry = 0;			// Number of entries in the BTree
	uint64_t leafEntries = 0;		// Number of leaf entries
	uint8_t depth = 0;				// Depth of the b-tree
	uint64_t payload = 0;			// Total amount of data stored in this table or index
	uint64_t ovflPayload = 0;		// Total amount of data stored on overflow pages
	uint64_t ovflCnt = 0;			// Number of entries that use overflow
	uint64_t mxPayload = 0;			// Maximum payload size
	uint64_t intPages = 0;			// Number of interior pages used
	uint64_t leafPages = 0;			// Number of leaf pages used
	uint64_t ovflPages = 0;			// Number of overflow pages used
	uint64_t intUnused = 0;			// Number of unused bytes on interior pages
	uint64_t leafUnused = 0;		// Number of unused bytes on primary pages
	uint64_t ovflUnused = 0;		// Number of unused bytes on overflow pages
	uint64_t gapCnt = 0;			// Number of gaps in the page layout
	uint64_t compressedSize = 0;	// Total bytes stored on disk
	uint64_t cnt = 0;				// Total record count : count(*) as cnt
} TblIdxSpaceUsed;
typedef std::list<TblIdxSpaceUsed> TblIdxSpaceUsedList;


typedef struct _DbSpaceUsed {
	int pageSize = 0;				// Size of each page in bytes.
	uint64_t fileBytes = 0;			// File size in bytes.
	uint64_t filePgcnt = 0;			// Number of pages in the file.
	uint64_t filePgcnt2 = 0;		// Number of pages in the file (calculated).
	uint64_t avPgcnt = 0;			// Pages consumed by the auto-vacuum pointer-map.
	double avPercent = 0.0;			// Percentage of the file consumed by auto-vacuum pointer-map.
	uint64_t inusePgcnt = 0;		// Data pages in the file.
	double inusePercent = 0.0;		// Percentage of pages used to store data.
	uint64_t freePgcnt = 0;			// Free pages calculated as (<total pages> - <in-use pages>)
	uint64_t freePgcnt2 = 0;		// Free pages in the file according to the file header.
	double freePercent = 0.0;		// Percentage of file consumed by free pages (calculated).
	double freePercent2 = 0.0;		// Percentage of file consumed by free pages (header).
	int ntable = 0;					// Number of tables in the db.
	int nindex = 0;					// Number of indices in the db.
	int nautoindex = 0;				// Number of indices created automatically.
	int nmanindex = 0;				// Number of indices created manually.
	uint64_t userPayload = 0;		// Number of bytes of payload in table btrees ((not including sqlite_schema))
	double userPercent = 0;		// $userPayload as a percentage of total file size.
} DbSpaceUsed;

typedef struct _StoreAnalysisItem {
	wxColour color;
	std::string name;			// item name
	std::string description;	// item description
	std::string val;			// item value str
	double percent;				// max value
} StoreAnalysisItem;
typedef std::vector<StoreAnalysisItem> StoreAnalysisItems;

typedef struct _DbStat {
	std::string name;
	std::string path;
	uint64_t pageno = 0;
	std::string pagetype;
	uint64_t ncell = 0;
	uint64_t payload = 0;
	uint64_t unused = 0;
	uint64_t mxPayload = 0;
	uint64_t pgoffset = 0;
	uint16_t pgsize = 0;
	std::string schema;
	uint8_t aggregate = 0;
} DbStat;
typedef std::list<DbStat> DbStatList;

typedef struct _SqliteSchema {
	std::string type;
	std::string name;
	std::string tblName;
	uint64_t rootpage = 0;
	std::string sql;
} SqliteSchema;
typedef std::list<SqliteSchema> SqliteSchemaList;

typedef struct _TblIdxPageCnt {
	std::string name;
	uint64_t pageCnt;
	double percent;
}TblIdxPageCnt;
typedef std::vector<TblIdxPageCnt> TblIdxPageCntVector;


typedef struct _TblIdxEntryCnt {
	std::string name;
	uint64_t entryCnt;
	double percent;
}TblIdxEntryCnt;
typedef std::vector<TblIdxEntryCnt> TblIdxEntryCntVector;

typedef enum {
	READ_ELEM,
	EDIT_ELEM,
	COMBO_READ_ELEM,
	COMBO_EDIT_ELEM
} ParamElemType;

typedef enum {
	READ_ONLY,
	WRITE_ONLY,
	READ_WRITE
} ParamValReadWriteType;

typedef enum {
	IGNORE_VAL, // ignore the value
	ASSIGN_VAL, // VALUE for assign express, such as PRAGMA wal_autocheckpoint=N;
	ASSIGN_STR, // STRING for assign express, such as PRAGMA give_me_five='5';
	PARAM_VAL, // VALUE for function params, such as PRAGMA optimize(VAL);
	PARAM_STR, // VALUE for function params, such as PRAGMA table_list('table-name');
} ParamValBackType;

typedef struct _ParamElemData {
	ParamElemType type;
	std::string labelText;
	std::string val;
	std::vector<std::string> options;
	std::string description;
	ParamValReadWriteType valRwType;
	ParamValBackType valBackType;
} ParamElemData;

typedef std::list<ParamElemData> ParamElemDataList;
