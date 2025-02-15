﻿#include "QSqlStatement.h"
#include "QSqlDatabase.h"
#include <sqlite3/sqlite3.h>
#include "QSqlException.h"
#include "QSqlColumn.h"
#include "utils/Log.h"

namespace SQLite
{


QSqlStatement::QSqlStatement(const QSqlDatabase* aDatabase, const char* apQuery) :
    mQuery(apQuery),
    mpSQLite(aDatabase->getHandle()),
    mpPreparedStatement(prepareStatement()) // prepare the SQL query (needs Database friendship)
{
    mColumnCount = sqlite3_column_count(mpPreparedStatement.get());
}

QSqlStatement::QSqlStatement(QSqlStatement&& aStatement) noexcept :
    mQuery(std::move(aStatement.mQuery)),
    mpSQLite(aStatement.mpSQLite),
    mpPreparedStatement(std::move(aStatement.mpPreparedStatement)),
    mColumnCount(aStatement.mColumnCount),
    mbHasRow(aStatement.mbHasRow),
    mbDone(aStatement.mbDone),
    mColumnNames(std::move(aStatement.mColumnNames))
{
    aStatement.mpSQLite = nullptr;
    aStatement.mColumnCount = 0;
    aStatement.mbHasRow = false;
    aStatement.mbDone = false;
}

// Reset the statement to make it ready for a new execution (see also #clearBindings() below)
void QSqlStatement::reset()
{
    const int ret = tryReset();
    check(ret);
}

int QSqlStatement::tryReset() noexcept
{
    mbHasRow = false;
    mbDone = false;
    return sqlite3_reset(mpPreparedStatement.get());
}

// Clears away all the bindings of a prepared statement (can be associated with #reset() above).
void QSqlStatement::clearBindings()
{
    const int ret = sqlite3_clear_bindings(getPreparedStatement());
    check(ret);
}

int QSqlStatement::getIndex(const char * apName) const
{
    auto ret = sqlite3_bind_parameter_index(getPreparedStatement(), apName);
	return ret;
}

// Bind an 32bits int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bind(const int aIndex, const int32_t aValue)
{
    const int ret = sqlite3_bind_int(getPreparedStatement(), aIndex, aValue);
    check(ret);
}

// Bind a 32bits unsigned int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bind(const int aIndex, const uint32_t aValue)
{
    const int ret = sqlite3_bind_int64(getPreparedStatement(), aIndex, aValue);
    check(ret);
}

// Bind a 64bits int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bind(const int aIndex, const int64_t aValue)
{
    const int ret = sqlite3_bind_int64(getPreparedStatement(), aIndex, aValue);
    check(ret);
}

// Bind a 64bits int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bind(const int aIndex, const uint64_t aValue)
{
    const int ret = sqlite3_bind_int64(getPreparedStatement(), aIndex, aValue);
    check(ret);
}

// Bind a double (64bits float) value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bind(const int aIndex, const double aValue)
{
    const int ret = sqlite3_bind_double(getPreparedStatement(), aIndex, aValue);
    check(ret);
}

// Bind a string value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bind(const int aIndex, const std::string& aValue)
{	
    const int ret = sqlite3_bind_text(getPreparedStatement(), aIndex, aValue.c_str(),
                                      static_cast<int>(aValue.size()), SQLITE_TRANSIENT);
    check(ret);
}

// Bind a text value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bind(const int aIndex, const char* apValue)
{
    const int ret = sqlite3_bind_text(getPreparedStatement(), aIndex, apValue, -1, SQLITE_TRANSIENT);
    check(ret);
	
}

// Bind a binary blob value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bind(const int aIndex, const void* apValue, const int aSize)
{
    const int ret = sqlite3_bind_blob(getPreparedStatement(), aIndex, apValue, aSize, SQLITE_TRANSIENT);
    check(ret);
}

// Bind a string value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bindNoCopy(const int aIndex, const std::string& aValue)
{
    const int ret = sqlite3_bind_text(getPreparedStatement(), aIndex, aValue.c_str(),
                                      static_cast<int>(aValue.size()), SQLITE_STATIC);
    check(ret);
}

// Bind a text value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bindNoCopy(const int aIndex, const char* apValue)
{
    const int ret = sqlite3_bind_text(getPreparedStatement(), aIndex, apValue, -1, SQLITE_STATIC);
    check(ret);
}

// Bind a binary blob value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bindNoCopy(const int aIndex, const void* apValue, const int aSize)
{
    const int ret = sqlite3_bind_blob(getPreparedStatement(), aIndex, apValue, aSize, SQLITE_STATIC);
    check(ret);
}

// Bind a NULL value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
void QSqlStatement::bind(const int aIndex)
{
    const int ret = sqlite3_bind_null(getPreparedStatement(), aIndex);
    check(ret);
}


// Execute a step of the query to fetch one row of results
bool QSqlStatement::executeStep()
{
    const int ret = tryExecuteStep();
    if ((SQLITE_ROW != ret) && (SQLITE_DONE != ret)) // on row or no (more) row ready, else it's a problem
    {
        if (ret == sqlite3_errcode(mpSQLite))
        {
            throw SQLite::QSqlException(mpSQLite, ret);
        }
        else
        {
            throw SQLite::QSqlException("Statement needs to be reseted", ret);
        }
    }

    return mbHasRow; // true only if one row is accessible by getColumn(N)
}

// Execute a one-step query with no expected result, and return the number of changes.
int QSqlStatement::exec()
{
    const int ret = tryExecuteStep();
    if (SQLITE_DONE != ret) // the statement has finished executing successfully
    {
        if (SQLITE_ROW == ret)
        {
            throw SQLite::QSqlException("exec() does not expect results. Use executeStep.");
        }
        else if (ret == sqlite3_errcode(mpSQLite))
        {
            throw SQLite::QSqlException(mpSQLite, ret);
        }
        else
        {
            throw SQLite::QSqlException("Statement needs to be reseted", ret);
        }
    }

    // Return the number of rows modified by those SQL statements (INSERT, UPDATE or DELETE)
    return sqlite3_changes(mpSQLite);
}

/// <summary>
/// 执行SQL语句
/// </summary>
/// <returns></returns>
int QSqlStatement::tryExecuteStep() noexcept
{
    if (mbDone)
    {
        return SQLITE_MISUSE; // Statement needs to be reseted !
    }

    const int ret = sqlite3_step(mpPreparedStatement.get());
    if (SQLITE_ROW == ret) // one row is ready : call getColumn(N) to access it
    {
        mbHasRow = true;
    }
    else
    {
        mbHasRow = false;
        mbDone = SQLITE_DONE == ret; // check if the query has finished executing
    }
    return ret;
}


// Return a copy of the column data specified by its index starting at 0
// (use the Column copy-constructor)

QSqlColumn QSqlStatement::getColumn(const int aIndex) const
{
    checkRow();
    checkIndex(aIndex);

    // Share the Statement Object handle with the new Column created
    return QSqlColumn(mpPreparedStatement, aIndex);
}

// Return a copy of the column data specified by its column name starting at 0
// (use the Column copy-constructor)

QSqlColumn QSqlStatement::getColumn(const char* apName) const
{
    checkRow();
    const int index = getColumnIndex(apName);

    // Share the Statement Object handle with the new Column created
    return QSqlColumn(mpPreparedStatement, index);
}

// Test if the column is NULL
bool QSqlStatement::isColumnNull(const int aIndex) const
{
    checkRow();
    checkIndex(aIndex);
    return (SQLITE_NULL == sqlite3_column_type(getPreparedStatement(), aIndex));
}

bool QSqlStatement::isColumnNull(const char* apName) const
{
    checkRow();
    const int index = getColumnIndex(apName);
    return (SQLITE_NULL == sqlite3_column_type(getPreparedStatement(), index));
}

// Return the named assigned to the specified result column (potentially aliased)
const char* QSqlStatement::getColumnName(const int aIndex) const
{
    checkIndex(aIndex);
    const char * column_name = sqlite3_column_name(getPreparedStatement(), aIndex);
	return column_name;
}

#ifdef SQLITE_ENABLE_COLUMN_METADATA
// Return the named assigned to the specified result column (potentially aliased)
const char* QSqlStatement::getColumnOriginName(const int aIndex) const
{
    checkIndex(aIndex);
    return sqlite3_column_origin_name(getPreparedStatement(), aIndex);
}
#endif

// Return the index of the specified (potentially aliased) column name
int QSqlStatement::getColumnIndex(const char* apName) const
{
    // Build the map of column index by name on first call
    if (mColumnNames.empty())
    {
        for (int i = 0; i < mColumnCount; ++i)
        {
            const char * pName = sqlite3_column_name(getPreparedStatement(), i);
            mColumnNames[pName] = i;
        }
    }

    const auto iIndex = mColumnNames.find(apName);
    if (iIndex == mColumnNames.end())
    {
        throw SQLite::QSqlException("Unknown column name.");
    }

    return iIndex->second;
}

const char * QSqlStatement::getColumnDeclaredType(const int aIndex) const
{
    checkIndex(aIndex);
    const char * result = sqlite3_column_decltype(getPreparedStatement(), aIndex);
    if (!result)
    {
        throw SQLite::QSqlException("Could not determine declared column type.");
    }
    else
    {
        return result;
    }
}

// Get number of rows modified by last INSERT, UPDATE or DELETE statement (not DROP table).
int QSqlStatement::getChanges() const noexcept
{
    return sqlite3_changes(mpSQLite);
}

int QSqlStatement::getBindParameterCount() const noexcept
{
    return sqlite3_bind_parameter_count(mpPreparedStatement.get());
}

// Return the numeric result code for the most recent failed API call (if any).
int QSqlStatement::getErrorCode() const noexcept
{
    return sqlite3_errcode(mpSQLite);
}

// Return the extended numeric result code for the most recent failed API call (if any).
int QSqlStatement::getExtendedErrorCode() const noexcept
{
    return sqlite3_extended_errcode(mpSQLite);
}

// Return UTF-8 encoded English language explanation of the most recent failed API call (if any).
const char* QSqlStatement::getErrorMsg() const noexcept
{
	return sqlite3_errmsg(mpSQLite);
}

// Return a UTF-8 string containing the SQL text of prepared statement with bound parameters expanded.
std::string QSqlStatement::getExpandedSQL() const {
    char * expanded = sqlite3_expanded_sql(getPreparedStatement());
    return expanded;
}

// Prepare SQLite statement object and return shared pointer to this object
QSqlStatement::TStatementPtr QSqlStatement::prepareStatement()
{
    sqlite3_stmt* statement;
    const int ret = sqlite3_prepare_v2(mpSQLite, mQuery.c_str(), static_cast<int>(mQuery.size()), &statement, nullptr);
	
    if (SQLITE_OK != ret)
    {
		const char * errorMsg = sqlite3_errmsg(mpSQLite);
		Q_ERROR("call sqlite3_prepare_v2 has error:{}", errorMsg); 
		
        throw SQLite::QSqlException(mpSQLite, ret);
    }
	//free(ssql);
    return QSqlStatement::TStatementPtr(statement, [](sqlite3_stmt* stmt)
        {
            sqlite3_finalize(stmt);
        });
}

// Return prepered statement object or throw
sqlite3_stmt* QSqlStatement::getPreparedStatement() const
{
    sqlite3_stmt* ret = mpPreparedStatement.get();
    if (ret)
    {
        return ret;
    }
    throw SQLite::QSqlException("Statement was not prepared.");
}
#if __cplusplus >= 201402L || (defined(_MSC_VER) && _MSC_VER >= 1900) // c++14: Visual Studio 2015

// Create an instance of T from the first N columns, see declaration in Statement.h for full details
template<typename T, int N>
T QSqlStatement::getColumns()
{
    checkRow();
    checkIndex(N - 1);
    return getColumns<T>(std::make_integer_sequence<int, N>{});
}

// Helper function called by getColums<typename T, int N>
template<typename T, const int... Is>
T QSqlStatement::getColumns(const std::integer_sequence<int, Is...>)
{
    return T{QSqlColumn(mpPreparedStatement, Is)...};
}


} // namespace SQLite
#endif
