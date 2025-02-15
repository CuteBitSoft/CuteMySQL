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

 * @file   SavePointUtil.h
 * @brief  The utils class for sqlite3 SavePoints
 * 
 * @author Xuehan Qin
 * @date   2023-10-23
 *********************************************************************/
#pragma once
#include <string>
class SavePointUtil {
public:
	static std::string create(const std::string & type, const std::string & prefix = std::string("cute_mysql"));
};
