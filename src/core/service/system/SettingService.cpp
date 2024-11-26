/*****************************************************************//**
 * @file   SettingService.cpp
 * @brief  系统服务
 * @detail    $DETAIL
 * 
 * @author 覃学涵
 * @date   2023-03-02
 *********************************************************************/
#include <atlfile.h>
#include "SettingService.h"
#include "core/entity/Entity.h"
#include "core/common/exception/QRuntimeException.h"
#include "utils/Log.h"
#include "utils/DateUtil.h"
#include "utils/ResourceUtil.h"
#include "utils/FileUtil.h"
#include "utils/ProfileUtil.h"

SettingService::SettingService()
{

}

/**
 * 系统配置表sys_init的数据，KEY匹配到.
 * 
 * @param key
 * @return 
 */
std::string SettingService::getSysInit(const std::string & key)
{
	auto settings = getAllSysSetting();
	for (auto pair : settings) {
		if (pair.first == key) {
			return pair.second;
		}
	}
	return "";
}

/**
 * key,val保存到数据表sys_init中.
 * 
 * @param key
 * @param val
 */
void SettingService::setSysInit(const std::string & key, const std::string & val)
{
	getRepository()->set(key, val);
	getAllSysSetting(true);//重新加载数据库sys_init表所有的选项
}


/**
 * 加载数据库sys_init表所有的选项
 * 
 * @param isReload 是否重新加载
 * @return 
 */
Setting SettingService::getAllSysSetting(bool isReload)
{
	if (!isReload && !sysSetting.empty()) {
		return sysSetting;
	}
	sysSetting.clear();
	SysInitList allList = getRepository()->getAll();
	for (auto item : allList) {
		sysSetting.push_back({item.name, item.val});
	}

	return sysSetting;
}

/**
 * 获取ini所有配置项<group, Setting>.
 * 
 * @param isReload 是否重新加载
 * @return 所有分组配置项<group, Setting>
 */
IniSetting SettingService::getAllIniSetting(bool isReload)
{
	if (!isReload && iniSetting.empty() == false) {
		return iniSetting;
	}

	iniSetting.clear();
	std::vector<std::string> setions;
	std::string binDir = ResourceUtil::getStdProductBinDir();
	std::string iniFile = getSysInit("lang-file");
	std::string iniPath = binDir + "/" + iniFile;

	if (_access(iniPath.c_str(), 0) != 0) {
		Q_ERROR("the ini setting file is not exists, path:{}", iniPath);
		throw QRuntimeException("000002", "sorry, the setting file is not exists!");
	}

	std::vector<std::string> sections;	
	ProfileUtil::parseSections(iniPath, sections);

	for (auto item : sections) {
		Setting setting;
		ProfileUtil::parseSectionSettings(iniPath, item, setting);
		if (!setting.empty()) {
			iniSetting[item] = setting;
		}
		
	}
	return iniSetting;

}

/**
 * 从[language].ini配置文件中读取所有性别.
 * 
 * @return 性别所有项
 */
Setting SettingService::getAllGenderSetting()
{
	Setting genders;
	IniSetting settings = getAllIniSetting();
	if (settings.empty()) {
		return genders;
	}
	genders = settings["GENDER"];
	return genders;
}

Setting SettingService::getSettingBySection(const std::string & section)
{
	Setting sectionSetting;
	IniSetting settings = getAllIniSetting();
	if (settings.empty()) {
		return sectionSetting;
	}
	sectionSetting = settings[section];
	return sectionSetting;
}

/**
 * 找节点指定的key值.
 * 
 * @param section
 * @param key
 * @return 
 */
std::string SettingService::getValBySectionAndKey(const std::string & section, const std::string & key)
{
	Setting setting = getSettingBySection(section);
	auto iterator = std::find_if(setting.begin(), setting.end(), [&key](std::pair<std::string, std::string> &pair) {
		return pair.first == key;
	});

	if (iterator == setting.end()) {
		return "";
	}
	return (*iterator).second;
}

/**
 * 保存语言文件.
 * 
 * @param langIniPath 语言文件路径(相对路径)
 */
void SettingService::saveLanguagePath(std::string & langIniPath)
{
	setSysInit(std::string("lang-file"), langIniPath);
}

std::string SettingService::getGenderIniVal(const std::string & key)
{
	Setting settings = getSettingBySection("GENDER");
	for (auto item : settings) {
		if (item.first == key) {
			return item.second;
		}
	}
	return "";
}

