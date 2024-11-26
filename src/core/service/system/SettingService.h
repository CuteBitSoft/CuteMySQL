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

 * @file   SettingService.h
 * @brief  the settings service class
 * 
 * @author Xuehan Qin
 * @date   2023-05-19
 *********************************************************************/
#pragma once
#include <unordered_map>
#include "core/entity/Entity.h"
#include "core/common/service/BaseService.h"
#include "core/repository/system/SysInitRepository.h"

class SettingService: public BaseService<SettingService, SysInitRepository>
{
public:
	SettingService();
	//--------------数据库---------------------------------------------------
	std::string getSysInit(const std::string & key);
	void setSysInit(const std::string & key, const std::string & val);
	// 获取数据库所有的配置项
	Setting getAllSysSetting(bool isReload = false);
	
	//--------------ini---------------------------------------------------
	// 获取ini所有分组以及配置项<group, Setting>
	IniSetting getAllIniSetting(bool isReload = false);
	// 获得ini配置文件的值 key=val
	std::string getGenderIniVal(const std::string & key);

	// 获取ini的配置
	Setting getAllGenderSetting();
	Setting getSettingBySection(const std::string & section);
	std::string getValBySectionAndKey(const std::string & section, const std::string & key);
	void saveLanguagePath(std::string & langIniPath);
private:
	
	//--------------数据库---------------------------------------------------
	// 所有sys_init表中的配置
	Setting sysSetting;

	//--------------ini---------------------------------------------------
	// ini所有分组以及配置项<group, Setting>
	IniSetting iniSetting;
};
