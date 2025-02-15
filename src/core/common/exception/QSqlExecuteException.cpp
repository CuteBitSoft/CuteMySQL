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

 * @file   QSqlExecuteException.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-10-21
 *********************************************************************/
#include "QSqlExecuteException.h"

QSqlExecuteException::QSqlExecuteException(const std::string errCode, const std::string errMsg, const std::string &sql)
	: QRuntimeException(errCode, errMsg)
{
	this->sql = sql;
}

const std::string & QSqlExecuteException::getSql() const
{
	return sql;
}
