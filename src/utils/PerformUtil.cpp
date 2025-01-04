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

 * @file   PerformUtil.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-07-06
 *********************************************************************/
#include "PerformUtil.h"
#include <spdlog/fmt/fmt.h>
#include <atlstr.h>
#include "utils/Log.h"

std::chrono::steady_clock::time_point PerformUtil::begin()
{
	return std::chrono::high_resolution_clock::now();
}

std::string PerformUtil::end(std::chrono::steady_clock::time_point _begin)
{
	auto _end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(_end - _begin);
	
	auto result = fmt::format("{:.{}f} ms", static_cast<double>(elapsed.count() * 1.0 / 1000.0), 3);
	return result;
}

/**
 * Calculate the performance time and return the millisecond, ms.
 * 
 * @param perf 
 * @return string of millisecond
 */
std::string PerformUtil::elapsedMs(const PerfTime& perf)
{
	double val = static_cast<double>(perf.elapsedMicroSeconds * 1.0 / 1000.0);
	// auto begin_t = std::chrono::duration_cast<std::chrono::microseconds>(perf.begin.time_since_epoch());
	// auto end_t = std::chrono::duration_cast<std::chrono::microseconds>(perf.end.time_since_epoch());
	// Q_INFO("perf.elapsedNanoSeconds={},val={},begin={},end={}", perf.elapsedMicroSeconds, val, begin_t.count(), end_t.count());
	auto result = fmt::format("{:.{}f} ms", val, 3);
	return result;
}

/**
 * Calculate the performance time and return the microsecond, ¦Ìs.
 * 
 * @param perf 
 * @return string of microsecond
 */
std::string PerformUtil::elapsedUs(const PerfTime& perf)
{
	auto result = fmt::format("{:.{}f} ¦Ìs", static_cast<double>(perf.elapsedMicroSeconds * 1.0), 3);
	return result;
}


