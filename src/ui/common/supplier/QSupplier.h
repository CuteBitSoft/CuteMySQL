/*****************************************************************//**
 * Copyright 2024 Xuehan Qin (qinxuehan2018@gmail.com) 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * @file   QSupplier.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-13
 *********************************************************************/
#pragma once
#include <stdint.h>
#include <string>

// template param: T - subclass of QSupplier
template <typename T>
class QSupplier {
public:
	// CSV Options
	static std::string csvFieldSeperators[5];
	static std::string csvLineSeperators[3];
	static std::string csvNullAsKeywords[2];
	static std::string csvEncodings[2];

	static T* getInstance();
	static void destroyInstance();

	uint64_t getRuntimeUserConnectId() const { return runtimeUserConnectId; }
	void setRuntimeUserConnectId(uint64_t val) { runtimeUserConnectId = val; }

	uint64_t getRuntimeUserDbId() const { return runtimeUserDbId; }
	void setRuntimeUserDbId(uint64_t val) { runtimeUserDbId = val; }

	std::string & getRuntimeTblName() { return runtimeTblName; }
	void setRuntimeTblName(const std::string & val) { runtimeTblName = val; }

	std::string & getRuntimeSchema() { return runtimeSchema; }
	void setRuntimeSchema(const std::string & val) { runtimeSchema = val; }
	
	bool getIsDirty() { return isDirty; }
	void setIsDirty(bool dirty = false) { isDirty = dirty; }
protected:
	// Runtime variables
	uint64_t runtimeUserConnectId = 0;
	uint64_t runtimeUserDbId = 0;
	std::string runtimeSchema;
	std::string runtimeTblName;
	bool isDirty = false;
private:
	static T* theInstance;
};


template <typename T>
std::string QSupplier<T>::csvFieldSeperators[5] = { ",", "TAB", "|", ";",":" };

template <typename T>
std::string QSupplier<T>::csvLineSeperators[3] = { "CR", "CRLF", "LF" };

template <typename T>
std::string QSupplier<T>::csvNullAsKeywords[2] = { "YES", "NO" };

template <typename T>
std::string QSupplier<T>::csvEncodings[2] = { "UTF-8", "UTF-16" };

template <typename T>
T* QSupplier<T>::theInstance = nullptr;

template <typename T>
T* QSupplier<T>::getInstance()
{
	if (QSupplier::theInstance == nullptr) {
		QSupplier::theInstance = new T();
	}
	return QSupplier::theInstance;
}

template <typename T>
void QSupplier<T>::destroyInstance()
{
	if (QSupplier::theInstance != nullptr) {
		delete QSupplier::theInstance;
		QSupplier::theInstance = nullptr;
	}
}