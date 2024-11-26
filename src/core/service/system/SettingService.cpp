/*****************************************************************//**
 * @file   SettingService.cpp
 * @brief  ϵͳ����
 * @detail    $DETAIL
 * 
 * @author ��ѧ��
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
 * ϵͳ���ñ�sys_init�����ݣ�KEYƥ�䵽.
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
 * key,val���浽���ݱ�sys_init��.
 * 
 * @param key
 * @param val
 */
void SettingService::setSysInit(const std::string & key, const std::string & val)
{
	getRepository()->set(key, val);
	getAllSysSetting(true);//���¼������ݿ�sys_init�����е�ѡ��
}


/**
 * �������ݿ�sys_init�����е�ѡ��
 * 
 * @param isReload �Ƿ����¼���
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
 * ��ȡini����������<group, Setting>.
 * 
 * @param isReload �Ƿ����¼���
 * @return ���з���������<group, Setting>
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
 * ��[language].ini�����ļ��ж�ȡ�����Ա�.
 * 
 * @return �Ա�������
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
 * �ҽڵ�ָ����keyֵ.
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
 * ���������ļ�.
 * 
 * @param langIniPath �����ļ�·��(���·��)
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

