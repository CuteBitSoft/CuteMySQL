﻿/**
 * @file    Column.cpp
 * @ingroup SQLiteCpp
 * @brief   Encapsulation of a Column in a row of the result pointed by the prepared SQLite::Statement.
 *
 * Copyright (c) 2012-2022 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include "QSqlColumn.h"
#include <sqlite3/sqlite3.h>
#include <iostream>
#include <string>

namespace SQLite
{

const int INTEGER = SQLITE_INTEGER;
const int FLOAT = SQLITE_FLOAT;
const int TEXT = SQLITE_TEXT;
const int BLOB = SQLITE_BLOB;
const int Null = SQLITE_NULL;


// Encapsulation of a Column in a row of the result pointed by the prepared Statement.
QSqlColumn::QSqlColumn(const QSqlStatement::TStatementPtr& aStmtPtr, int aIndex) :
    mStmtPtr(aStmtPtr),
    mIndex(aIndex)
{
    if (!aStmtPtr)
    {
        throw SQLite::QSqlException("Statement was destroyed");
    }
}

// Return the named assigned to this result QSqlColumn (potentially aliased)
const char* QSqlColumn::getName() const noexcept
{
    return (char*)sqlite3_column_name16(mStmtPtr.get(), mIndex);
}

#ifdef SQLITE_ENABLE_COLUMN_METADATA
// Return the name of the table column that is the origin of this result column
const char* QSqlColumn::getOriginName() const noexcept
{
    return (char*)sqlite3_column_origin_name16(mStmtPtr.get(), mIndex);
}
#endif

// Return the integer value of the column specified by its index starting at 0
int32_t QSqlColumn::getInt() const noexcept
{
    return sqlite3_column_int(mStmtPtr.get(), mIndex);
}

// Return the unsigned integer value of the column specified by its index starting at 0
uint32_t QSqlColumn::getUInt() const noexcept
{
    return static_cast<unsigned>(getInt64());
}



// Return the 64bits integer value of the column specified by its index starting at 0
int64_t QSqlColumn::getInt64() const noexcept
{
    return sqlite3_column_int64(mStmtPtr.get(), mIndex);
}

// Return the unsigned integer value of the column specified by its index starting at 0
uint64_t QSqlColumn::getUInt64() const noexcept
{
	uint64_t res = static_cast<uint64_t>(getInt64());
    return res;
}

// Return the double value of the column specified by its index starting at 0
double QSqlColumn::getDouble() const noexcept
{
    return sqlite3_column_double(mStmtPtr.get(), mIndex);
}

// Return a pointer to the text value (NULL terminated string) of the column specified by its index starting at 0
const std::string QSqlColumn::getText(const char* apDefaultValue /* = "" */) const noexcept
{
    auto unicode = reinterpret_cast<const char*>(sqlite3_column_text(mStmtPtr.get(), mIndex));
    return (sizeof(unicode) ? unicode : apDefaultValue);
}

// Return a pointer to the blob value (*not* NULL terminated) of the column specified by its index starting at 0
const void* QSqlColumn::getBlob() const noexcept
{
    return sqlite3_column_blob(mStmtPtr.get(), mIndex);
}

// Return a std::string to a TEXT or BLOB column
std::string QSqlColumn::getString() const
{
    // Note: using sqlite3_column_blob and not sqlite3_column_text
    // - no need for sqlite3_column_text to add a \0 on the end, as we're getting the bytes length directly
    //   however, we need to call sqlite3_column_bytes() to ensure correct format. It's a noop on a BLOB
    //   or a TEXT value with the correct encoding (UTF-8). Otherwise it'll do a conversion to TEXT (UTF-8).
    (void)sqlite3_column_bytes(mStmtPtr.get(), mIndex);
    auto data = static_cast<const char *>(sqlite3_column_blob(mStmtPtr.get(), mIndex));

    // SQLite docs: "The safest policy is to invoke… sqlite3_column_blob() followed by sqlite3_column_bytes()"
    // Note: std::string is ok to pass nullptr as first arg, if length is 0
	std::string unicode(data, sqlite3_column_bytes(mStmtPtr.get(), mIndex)); 
    return unicode;
}

// Return the type of the value of the column
int QSqlColumn::getType() const noexcept
{
    return sqlite3_column_type(mStmtPtr.get(), mIndex);
}

// Return the number of bytes used by the text value of the column
int QSqlColumn::getBytes() const noexcept
{
    return sqlite3_column_bytes(mStmtPtr.get(), mIndex);
}

// Standard std::ostream inserter
std::ostream& operator<<(std::ostream& aStream, const QSqlColumn& aColumn)
{
	std::string str = aColumn.getText();
    aStream.write(str.c_str(), aColumn.getBytes());
    return aStream;
}

}  // namespace SQLite
