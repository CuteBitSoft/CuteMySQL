/**
 * @file    QSqlException.cpp
 * @ingroup SQLiteCpp
 * @brief   Encapsulation of the error message from SQLite3 on a std::runtime_error.
 *
 * Copyright (c) 2012-2022 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <sqlite3/sqlite3.h>
#include "QSqlException.h"

namespace SQLite
{

QSqlException::QSqlException(const char* aErrorMessage, int ret) :
    std::runtime_error(aErrorMessage),
    mErrcode(ret),
    mExtendedErrcode(-1)
{
}

QSqlException::QSqlException(sqlite3* apSQLite) :
    std::runtime_error(sqlite3_errmsg(apSQLite)),
    mErrcode(sqlite3_errcode(apSQLite)),
    mExtendedErrcode(sqlite3_extended_errcode(apSQLite))
{
}

QSqlException::QSqlException(sqlite3* apSQLite, int ret) :
    std::runtime_error(sqlite3_errmsg(apSQLite)),
    mErrcode(ret),
    mExtendedErrcode(sqlite3_extended_errcode(apSQLite))
{
}

// Return a string, solely based on the error code

const std::string QSqlException::getErrorStr() const noexcept
{
	return what();
}


}  // namespace SQLite
