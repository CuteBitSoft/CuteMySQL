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

 * @file   QueryPageSupplier.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-10-30
 *********************************************************************/
#include "QueryPageSupplier.h"
#include "utils/StringUtil.h"
#include "utils/ResourceUtil.h"


const std::vector<std::string> QueryPageSupplier::sqlTags = {
	"SELECT", "SELECT *", "SELECT * FROM", "CREATE", "CREATE TABLE", "CREATE VIRTUAL TABLE", "CREATE INDEX", "CREATE INDEX", "CREATE TRIGGER", "CREATE VIEW", "CREATE VIRTUAL TABLE","CREATE INDEX","CREATE UNIQUE"
	"ALTER", "ALTER TABLE", "DELETE", "DELETE FROM", "BEGIN", "BEGIN TRANSATION", "COMMIT", "COMMIT TRANSACTION", "END TRANSATION",
	"DROP", "DROP INDEX", "DROP TABLE", "DROP TRIGGER", "DROP VIEW", "DROP COLUMN","INSERT", "INSERT INTO", "REPLACE", "REPLACE INTO", "UPDATE", "PRAGMA",
	"SAVEPOINT", "RELEASE", "RELEASE SAVEPOINT", "ROLLBACK", "ROLLBACK TO", "ROLLBACK TRANSACTION", "ANALYZE", "ATTACH", "ATTACH DATABASE", "DETACH", "DETACH DATABASE",
	"EXPLAIN", "REINDEX", "IF ", "NOT", "NOT NULL", "EXISTS", "IF NOT EXISTS", "IF EXISTS", "RETURNING", "VACUUM", "WITH","WITH RECURSIVE"
	"AUTOINCREMENT", "PRIMARY", "PRIMARY KEY", "DEFAULT", "CONSTRAINT", "FOREIGN", "FOREIGN KEY", "UNIQUE", "CHECK", "FROM", "WHERE", 
	"OR", "AND", "ADD COLUMN", "VALUES", "RECURSIVE", "DISTINCT", "AS", "GROUP", "GROUP BY", "ORDER", "ORDER BY", "BY", "HAVING", "LIMIT", "OFFSET",
	"JOIN", "LEFT", "LEFT JOIN", "RIGHT", "RIGHT JOIN", "INNER", "INNER JOIN", "FULL", "OUTER", "CROSS","WITHOUT","ROWID","STRICT",
	"IS", "IS NULL", "IS NOT NULL", "ON", "INTEGER", "TEXT", "NUMERIC", "REAL", "BLOB", "VARCHAR", "DATETIME", "VIRTUAL","USING","INDEXED", "USING INDEXED","NOT INDEXED",
	"LIKE", "BETWEEN", "CASE", "WHEN", "ELSE", "IN", "COLLATE", "MATCH","ESCAPE","REGEXP", "ISNULL", "NOTNULL","WINDOW", "QUERY", "PLAN",
	"RENAME","RENAME COLUMN","RENAME TO","COLUMN","DEFERRED", "IMMEDIATE", "EXCLUSIVE","TEMP","TEMPORARY","BEFORE","AFTER", "INSTEAD", "INSTEAD OF","FOR FETCH ROW",
	"ABORT", "FAIL","IGNORE","MATERIALIZED","UNION","UNION ALL","INTERSECT","EXCEPT", "CONFLICT", "REFERENCES"
};


const std::list<std::tuple<int, std::string, std::string>> QueryPageSupplier::pragmas = {
	{1713, "analysis_limit", "PRAGMA analysis_limit;"},
	{1714, "application_id", "PRAGMA application_id;"},
	{1715, "auto_vacuum", "PRAGMA auto_vacuum;"},
	{1716, "automatic_index", "PRAGMA automatic_index;"},
	{1717, "busy_timeout", "PRAGMA busy_timeout;"},
	{1718, "cache_size", "PRAGMA cache_size;"},
	{1719, "cache_spill", "PRAGMA cache_spill;"},
	{1720, "case_sensitive_like", "PRAGMA case_sensitive_like=<true/false>;"},
	{1721, "cell_size_check", "PRAGMA cell_size_check;"},
	{1722, "checkpoint_fullfsync", "PRAGMA checkpoint_fullfsync;"},
	{1723, "collation_list", "PRAGMA collation_list;"},
	{1724, "compile_options", "PRAGMA compile_options;"},
	{1725, "count_changes", "PRAGMA count_changes;"},
	{1726, "data_store_directory", "PRAGMA data_store_directory;"},
	{1727, "data_version", "PRAGMA data_version;"},
	{1728, "database_list", "PRAGMA database_list;"},
	{1729, "default_cache_size", "PRAGMA default_cache_size;"},
	{1730, "defer_foreign_keys", "PRAGMA defer_foreign_keys;"},
	{1731, "empty_result_callbacks", "PRAGMA empty_result_callbacks;"},
	{1732, "encoding", "PRAGMA encoding;"},
	{1733, "foreign_key_check", "PRAGMA foreign_key_check;"},
	{1734, "foreign_key_check(table-name)", "PRAGMA foreign_key_check(<table>);"},
	{1735, "foreign_key_list(table-name)", "PRAGMA foreign_key_list(<table>);"},
	{1736, "foreign_keys", "PRAGMA foreign_keys;"},
	{1737, "freelist_count", "PRAGMA freelist_count;"},
	{1738, "fullfsync", "PRAGMA fullfsync;"},
	{1739, "function_list", "PRAGMA function_list;"},
	{1740, "ignore_check_constraints", "PRAGMA ignore_check_constraints=<true/false>;"},
	{1741, "incremental_vacuum", "PRAGMA incremental_vacuum;"},
	{1742, "index_info(index-name)", "PRAGMA index_info(<index_name>);"},
	{1743, "index_list(table-name)", "PRAGMA index_list(<table>);"},
	{1744, "index_xinfo(index-name)", "PRAGMA index_xinfo(<index_name>);"},
	{1745, "integrity_check", "PRAGMA integrity_check;"},
	{1746, "integrity_check(N)", "PRAGMA integrity_check(<N>);"},
	{1747, "integrity_check(table-name)", "PRAGMA integrity_check(<table-name>);"},
	{1748, "journal_mode", "PRAGMA journal_mode;"},
	{1749, "journal_size_limit", "PRAGMA journal_size_limit;"},
	{1750, "legacy_alter_table", "PRAGMA legacy_alter_table;"},
	{1751, "legacy_file_format", "PRAGMA legacy_file_format;"},
	{1752, "locking_mode", "PRAGMA locking_mode;"},
	{1753, "max_page_count", "PRAGMA max_page_count;"},
	{1754, "mmap_size", "PRAGMA mmap_size;"},
	{1755, "module_list", "PRAGMA module_list;"},
	{1756, "optimize", "PRAGMA optimize;"},
	{1757, "optimize(MASK)", "PRAGMA optimize(<mask>);"},
	{1758, "page_count", "PRAGMA page_count;"},
	{1759, "page_size", "PRAGMA page_size;"},
	{1760, "parser_trace", "PRAGMA parser_trace=<true/false>;"},
	{1761, "pragma_list", "PRAGMA pragma_list;"},
	{1762, "query_only", "PRAGMA query_only;"},
	{1763, "quick_check", "PRAGMA quick_check;"},
	{1764, "quick_check(N)", "PRAGMA quick_check(<N>);"},
	{1765, "quick_check(table-name)", "PRAGMA quick_check(<table>);"},
	{1766, "read_uncommitted", "PRAGMA read_uncommitted;"},
	{1767, "recursive_triggers", "PRAGMA recursive_triggers;"},
	{1768, "reverse_unordered_selects", "PRAGMA reverse_unordered_selects;"},
	{1769, "schema_version", "PRAGMA schema_version;"},
	{1770, "secure_delete", "PRAGMA secure_delete;"},
	{1771, "short_column_names", "PRAGMA short_column_names;"},
	{1772, "shrink_memory", "PRAGMA shrink_memory;"},
	{1773, "soft_heap_limit", "PRAGMA soft_heap_limit;"},
	{1774, "stats", "PRAGMA stats;"},
	{1775, "synchronous", "PRAGMA synchronous;"},
	{1776, "table_info(table-name)", "PRAGMA table_info(<table>);"},
	{1777, "table_list", "PRAGMA table_list;"},
	{1778, "table_list(table-name)", "PRAGMA table_list(<table>);"},
	{1779, "table_xinfo(table-name)", "PRAGMA table_xinfo(<table>);"},
	{1780, "temp_store", "PRAGMA temp_store;"},
	{1781, "temp_store_directory", "PRAGMA temp_store_directory;"},
	{1782, "threads", "PRAGMA threads;"},
	{1783, "trusted_schema", "PRAGMA trusted_schema;"},
	{1784, "user_version", "PRAGMA user_version;"},
	{1785, "vdbe_addoptrace", "PRAGMA vdbe_addoptrace;"},
	{1786, "vdbe_debug", "PRAGMA vdbe_debug;"},
	{1787, "vdbe_listing", "PRAGMA vdbe_listing;"},
	{1788, "vdbe_trace", "PRAGMA vdbe_trace;"},
	{1789, "wal_autocheckpoint", "PRAGMA wal_autocheckpoint;"},
	{1790, "wal_checkpoint", "PRAGMA wal_checkpoint;"},
	{1791, "wal_checkpoint(PASSIVE)", "PRAGMA wal_checkpoint(PASSIVE);"},
	{1792, "wal_checkpoint(FULL)", "PRAGMA wal_checkpoint(FULL);"},
	{1793, "wal_checkpoint(RESTART)", "PRAGMA wal_checkpoint(RESTART);"},
	{1794, "wal_checkpoint(TRUNCATE)", "PRAGMA wal_checkpoint(TRUNCATE);"},
	{1795, "writable_schema", "PRAGMA writable_schema;"},
};

QueryPageSupplier::QueryPageSupplier()
{
	std::string imgDir = ResourceUtil::getStdProductImagesDir();
	std::string imagePath = imgDir + "/database/list/top.bmp";
	
}


QueryPageSupplier::~QueryPageSupplier()
{
}

UserTableStrings & QueryPageSupplier::getCacheUserTableStrings(uint64_t userDbId)
{
	assert(userDbId);
	return cacheUserTableMap[userDbId];
}


void QueryPageSupplier::setCacheUserTableStrings(uint64_t userDbId, UserTableStrings & tblStrs)
{
	assert(userDbId);
	cacheUserTableMap[userDbId] = tblStrs;
}


Columns & QueryPageSupplier::getCacheTableColumns(uint64_t userDbId, const std::string & tblName)
{
	assert(userDbId && !tblName.empty());
	std::pair<uint64_t, std::string> pair({ userDbId, tblName });
	return cacheTableColumnsMap[pair];
}


void QueryPageSupplier::setCacheTableColumns(uint64_t userDbId, const std::string & tblName, const Columns & columns)
{
	assert(userDbId && !tblName.empty());
	std::pair<uint64_t, std::string> pair({ userDbId, tblName });
	cacheTableColumnsMap[pair] = columns;
}

void QueryPageSupplier::splitToSqlVector(std::string sql)
{
	sqlVector.clear();
	if (sql.empty()) {
		return;
	} 
	
	if (sql.find(L';') == std::string::npos) {
		sqlVector.push_back(sql);
		return ;
	}

	sqlVector = StringUtil::splitNotIn(sql, ";", "BEGIN", "END", "TRANSACTION", true);
}
