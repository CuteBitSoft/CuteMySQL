#pragma once
#include <string>
#include <sstream>
#include <atltime.h>
#include <chrono>
#include "utils/StringUtil.h"

class DateUtil
{
public:
	static std::string getCurrentDateTime(const char * format = "%Y-%m-%d %H:%M:%S") 
	{
		auto now = std::chrono::system_clock::now();
		std::time_t tt = std::chrono::system_clock::to_time_t(now);
		std::string str(format);
		auto tm = localtime(&tt);

		str = StringUtil::replace(str, "%Y", std::to_string(tm->tm_year + 1900));
		str = StringUtil::replace(str, "%m", std::to_string(tm->tm_mon + 1));
		str = StringUtil::replace(str, "%d", std::to_string(tm->tm_mday));
		str = StringUtil::replace(str, "%H", std::to_string(tm->tm_hour));
		str = StringUtil::replace(str, "%M", std::to_string(tm->tm_min));
		str = StringUtil::replace(str, "%S", std::to_string(tm->tm_sec));
		return str; 
	}

	static std::string getCurrentDate(const char * format = "%Y-%m-%d") 
	{
		auto now = std::chrono::system_clock::now();
		std::time_t tt = std::chrono::system_clock::to_time_t(now);		
		auto tm = localtime(&tt);

		std::string str(format);
		str = StringUtil::replace(str, "%Y", std::to_string(tm->tm_year + 1900));
		str = StringUtil::replace(str, "%m", std::to_string(tm->tm_mon + 1));
		str = StringUtil::replace(str, "%d", std::to_string(tm->tm_mday)); 

		return str; 
	}

	static std::string getYestoday(const char * format = "%Y-%m-%d") 
	{
		auto now = std::chrono::system_clock::now();
		auto yestoday = now - std::chrono::hours(24);
		std::time_t tt = std::chrono::system_clock::to_time_t(yestoday);
		auto tm = localtime(&tt);

		std::string str(format);
		str = StringUtil::replace(str, "%Y", std::to_string(tm->tm_year + 1900));
		str = StringUtil::replace(str, "%m", std::to_string(tm->tm_mon + 1));
		str = StringUtil::replace(str, "%d", std::to_string(tm->tm_mday)); 

		return str; 
	};

	static CTime getYestodayTime() 
	{
		CTime curTime = CTime::GetCurrentTime();
		// CTimeSpan params : 0 - LONG lDays, 1 - int nHours, 2 - int nMins, 3 - int nSecs
		curTime -= CTimeSpan(1, 0, 0, 0); 
		return curTime;
	};

	/**
	 * Get the date from datetime,such as "2023-11-16 17:41:22" -> "2023-11-16" .
	 * 
	 * @param datetime - Format must be "%Y-%m-%d %H:%M:%S"
	 * @return 
	 */
	static std::string getDateFromDateTime(const std::string & datetime)
	{
		if (datetime.empty()) {
			return "";
		}
		int nYear, nMonth, nDate, nHour, nMin, nSec;
	    scanf_s(datetime.c_str(), "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec); 
		
		char str[12] = { 0 };
		sprintf_s(str, "%d-%02d-%02d", nYear, nMonth, nDate);
		return str;
	}

};
