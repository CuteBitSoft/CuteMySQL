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

 * @file   SavePointUtil.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-10-23
 *********************************************************************/
#include "SavePointUtil.h"
#include <chrono>
#include <ctime>
#include <stdexcept>

/**
 * Create a save point name for Sqlite3 transaction.
 * 
 * @param type - DDL type,such as create_table,alter_table,rename_table
 * @param prefix
 * @return 
 */
std::string SavePointUtil::create(const std::string & type, const std::string & prefix /*= std::string("cute_mysql")*/)
{
	std::string str = prefix;
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	std::time_t tt = std::chrono::system_clock::to_time_t(tp);
	
	str.append("_").append(type).append("_").append(std::to_string(tt));
	return str;
}
