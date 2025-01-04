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

 * @file   PerformUtil.h
 * @brief  Performance utils 
 * 
 * @author Xuehan Qin
 * @date   2023-07-06
 *********************************************************************/
#pragma once
#include <chrono>
#include <string>
#include "core/entity/Entity.h"

class PerformUtil {
public:
	static std::chrono::steady_clock::time_point begin();
	static std::string end(std::chrono::steady_clock::time_point _begin);

	// elapsed
	static std::string elapsedMs(const PerfTime& perf);
	static std::string elapsedUs(const PerfTime& perf);
};
