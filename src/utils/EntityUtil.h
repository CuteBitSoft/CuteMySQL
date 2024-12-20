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

 * @file   EntityUtil.h
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-10-28
 *********************************************************************/
#pragma once
#include "core/entity/Entity.h"

class EntityUtil {
public:
	static IndexInfo copy(const IndexInfo & item);
	static ResultInfo copy(const ResultInfo & item);
	static UserConnect copy(const UserConnect & item);
	static UserDb copy(const UserDb & item);
	static ParamElemData copy(const ParamElemData & item);

	static bool compare(const ColumnInfo & item1, const ColumnInfo & item2);
	static bool compare(const IndexInfo & item1, const IndexInfo & item2);
	static bool compare(const ForeignKey & item1, const ForeignKey & item2);
	static bool compare(const ParamElemData & item1, const ParamElemData & item2);
};
