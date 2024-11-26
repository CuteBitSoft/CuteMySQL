#include "QSqlDatabase.h"
#include <cassert>
#include "QSqlStatement.h"
#include "QSqlColumn.h"
#include "QSqlAssert.h"
#include "utils/Log.h"
#include "QSQLDatabase.h"

namespace SQLite
{

QSqlDatabase::QSqlDatabase(std::string name)
{
	assert(!name.empty());
	
	activeName = name; //当前激活的name
}

QSqlDatabase::QSqlDatabase()
{
	
}

QSqlDatabase::QSqlDatabase(QSqlDatabase & database)
{
	this->handle = database.getHandle();
	this->activeName = database.getActiveName();
	this->databaseName = database.getDatabaseName();
	this->hostName = database.getHostName();
	this->userName = database.getUserName();
	this->password = database.getPassword();
}

QSqlDatabase::~QSqlDatabase()
{
	if (handle != nullptr) {
		// delete by Xuehan Qin 2023/10/25, because handle should be released by passing it to sqlite3_close() when it is no longer required.
		// free(handle);
	}
}

QSqlDatabase & QSqlDatabase::operator=(QSqlDatabase & database)
{
	this->handle = database.getHandle();
	this->activeName = database.getActiveName();
	this->databaseName = database.getDatabaseName();
	this->hostName = database.getHostName();
	this->userName = database.getUserName();
	this->password = database.getPassword();

	return *this;
}

void QSqlDatabase::setDatabaseName(const std::string & dbName)
{
	this->databaseName = dbName;
}

void QSqlDatabase::setHostName(const std::string &hostName)
{
	this->hostName = hostName;
}

void QSqlDatabase::setUserName(const std::string & userName)
{
	this->userName = userName;
}

void QSqlDatabase::setPassword(const std::string & password)
{
	this->password = password;
}


sqlite3 * QSqlDatabase::getHandle() const noexcept
{
	return handle;
}

bool QSqlDatabase::isValid()
{
	if (handle == nullptr) {
		isOpenFlag = false;
		return false;
	}
	return isOpenFlag;
}

bool QSqlDatabase::isOpen()
{
	if (handle == nullptr) {
		isOpenFlag = false;
		return false;
	}
	return isOpenFlag;
}

bool QSqlDatabase::isClose()
{
	if (handle == nullptr || !isOpenFlag) {
		return true;
	}
	return false;
}

bool QSqlDatabase::open()
{
	assert(!databaseName.empty());
	
	int ret =  sqlite3_open_v2(databaseName.c_str(), &handle, SQLITE_OPEN_READWRITE, nullptr);
	isOpenFlag = (SQLITE_OK == ret);
	if (SQLITE_OK != ret) {
		setErrorMsg("Open sqlite db raise error. path:" + databaseName);
		Q_ERROR("Open sqlite db raise error. path:{}", databaseName);
		return false;
	}
	Q_INFO("Open sqlite db success. path:{}", databaseName);
	return isOpenFlag;
}

bool QSqlDatabase::close()
{
	Q_LOG("Close sqlite : ", databaseName);
	if (handle == nullptr) {
		return true;
	}

	int ret = sqlite3_close_v2(handle);
	if (SQLITE_OK != ret) {
		Q_ERROR("Close the sqlite3 raise error");
		return false;
	}

	return true;
}

// Deleter functor to use with smart pointers to close the SQLite database connection in an RAII fashion.
void QSqlDatabase::Deleter::operator()(sqlite3* apSQLite)
{
    const int ret = sqlite3_close_v2(apSQLite); // Calling sqlite3_close() with a nullptr argument is a harmless no-op.

    // Avoid unreferenced variable warning when build in release mode
    (void) ret;

    // Only case of error is SQLITE_BUSY: "database is locked" (some statements are not finalized)
    // Never throw an exception in a destructor :
	SQLITECPP_ASSERT(SQLITE_OK == ret, "database is locked");
	apSQLite = nullptr;
}

std::string QSqlDatabase::lastErrorCode()
{
	unsigned long threadId = ThreadUtil::currentThreadId();
	return errorCodeMap[threadId];
}

std::string QSqlDatabase::lastError()
{
	unsigned long threadId = ThreadUtil::currentThreadId();
	return errorMsgMap[threadId];
}

void QSqlDatabase::setError(const std::string & code, const std::string & msg)
{
	unsigned long threadId = ThreadUtil::currentThreadId();
	errorCodeMap[threadId] = code;
	errorMsgMap[threadId] = msg;
}

void QSqlDatabase::setErrorCode(const std::string & code)
{
	unsigned long threadId = ThreadUtil::currentThreadId();
	errorCodeMap[threadId] = code;
}

void QSqlDatabase::setErrorMsg(const std::string & msg)
{
	unsigned long threadId = ThreadUtil::currentThreadId();
	errorMsgMap[threadId] = msg;
}

//Set a busy handler that sleeps for a specified amount of time when a table is locked.
void QSqlDatabase::setBusyTimeout(const int aBusyTimeoutMs)
{
    const int ret = sqlite3_busy_timeout(getHandle(), aBusyTimeoutMs);
    check(ret);
}

// Shortcut to execute one or multiple SQL statements without results (UPDATE, INSERT, ALTER, COMMIT, CREATE...).
// Return the number of changes.
int QSqlDatabase::exec(const char * apQueries)
{
    const int ret = tryExec(apQueries);
    check(ret);

    // Return the number of rows modified by those SQL statements (INSERT, UPDATE or DELETE only)
    return sqlite3_changes(getHandle());
}

int QSqlDatabase::tryExec(const char* wpQueries) noexcept
{
    auto ret = sqlite3_exec(getHandle(), wpQueries, nullptr, nullptr, nullptr);
	return ret;
}

// Shortcut to execute a one step query and fetch the first column of the result.
// WARNING: Be very careful with this dangerous method: you have to
// make a COPY OF THE result, else it will be destroy before the next line
// (when the underlying temporary Statement and Column objects are destroyed)
// this is an issue only for pointer type result (ie. char* and blob)
// (use the Column copy-constructor)

QSqlColumn QSqlDatabase::execAndGet(const char* apQuery)
{
    QSqlStatement query(this, apQuery);
    (void)query.executeStep(); // Can return false if no result, which will throw next line in getColumn()
    return query.getColumn(0);
}

// Shortcut to test if a table exists.
bool QSqlDatabase::tableExists(const char* apTableName) const
{
    QSqlStatement query(this, "SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?");
	//char * aapTableName = StringUtil::utf8ToUnicode(apTableName);
    query.bind(1, apTableName);
    (void)query.executeStep(); // Cannot return false, as the above query always return a result
    return (1 == query.getColumn(0).getInt());
}


// Get the rowid of the most recent successful INSERT into the database from the current connection.
int64_t QSqlDatabase::getLastInsertRowid() const noexcept
{
    return sqlite3_last_insert_rowid(getHandle());
}

// Get number of rows modified by last INSERT, UPDATE or DELETE statement (not DROP table).
int QSqlDatabase::getChanges() const noexcept
{
    return sqlite3_changes(getHandle());
}

// Get total number of rows modified by all INSERT, UPDATE or DELETE statement since connection.
int QSqlDatabase::getTotalChanges() const noexcept
{
    return sqlite3_total_changes(getHandle());
}

// Return the numeric result code for the most recent failed API call (if any).
int QSqlDatabase::getErrorCode() const noexcept
{
    return sqlite3_errcode(getHandle());
}

// Return UTF-8 encoded English language explanation of the most recent failed API call (if any).
const char* QSqlDatabase::getErrorMsg() const noexcept
{
    return (char*)sqlite3_errmsg16(getHandle());
}

// Attach a custom function to your sqlite database. Assumes UTF8 text representation.
// Parameter details can be found here: http://www.sqlite.org/c3ref/create_function.html
void QSqlDatabase::createFunction(const char*   apFuncName,
                              int           aNbArg,
                              bool          abDeterministic,
                              void*         apApp,
                              void        (*apFunc)(sqlite3_context *, int, sqlite3_value **),
                              void        (*apStep)(sqlite3_context *, int, sqlite3_value **) /* = nullptr */,
                              void        (*apFinal)(sqlite3_context *) /* = nullptr */, // NOLINT(readability/casting)
                              void        (*apDestroy)(void *) /* = nullptr */)
{
    int textRep = SQLITE_UTF8;
    // optimization if deterministic function (e.g. of nondeterministic function random())
    if (abDeterministic)
    {
        textRep = textRep | SQLITE_DETERMINISTIC;
    }
    const int ret = sqlite3_create_function16(getHandle(), apFuncName, aNbArg, textRep,
                                               apApp, apFunc, apStep, apFinal);
    check(ret);
}

} // namespace SQLite

