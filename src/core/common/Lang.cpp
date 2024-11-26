#include "Lang.h"
#include "core/service/system/SettingService.h"
#include "utils/StringUtil.h"

/**
 * 获得ini语言配置文件[STRING]节点中的文本.
 * 
 * @param key 文本的key
 * @return val 
 */
std::string Lang::lang(const char * key)
{
	Setting langSetting = SettingService::getInstance()->getSettingBySection(std::string("STRING"));

	auto iterator = std::find_if(langSetting.begin(), langSetting.end(), [&key](std::pair<std::string, std::string> & pair) {
		return pair.first == std::string(key);
	});
	if (iterator == langSetting.end()) {
		return "";
	}

	return (*iterator).second;
}

/**
 * 获得ini语言配置文件[ERROR]节点中的错误文本.
 * 
 * @param key 文本的key
 * @return val 
 */
std::string Lang::error(const std::string & key)
{
	Setting langSetting = SettingService::getInstance()->getSettingBySection(std::string("ERROR"));

	auto iterator = std::find_if(langSetting.begin(), langSetting.end(), [&key](std::pair<std::string, std::string> & pair) {
		return pair.first == key;
	});
	if (iterator == langSetting.end()) {
		return "";
	}

	return (*iterator).second;
}

std::string Lang::langNoTab(const std::string & key)
{
	auto str = lang(key.c_str());
	if (str.empty()) {
		return str;
	}
	size_t tabPos = str.find_last_of(L'\t');
	if (tabPos != std::string::npos) {
		std::string pre = str.substr(0, tabPos);
		std::string tail = str.substr(tabPos+1, -1);
		str = pre;
		str.append("\t(").append(tail).append(")");
	}
	str = StringUtil::replace(str, "\t", " ");
	return str;
}

/**
 * 获得ini语言配置文件[FONT]节点中的文本字体和大小.HFONT返回
 * 
 * @param key ini文件指定的字体的key，比如：left-panel-size
 * @param bold 是否加粗
 * @param defPixed 默认的字体大小
 * @param defFontName 默认的字体 
 * @return HFONT返回
 */
wxFont Lang::font(const char * key, bool bold /*= false*/, int defPixed, const char * fontName)
{
	Setting fontSetting = SettingService::getInstance()->getSettingBySection(std::string("FONT"));
	int pixel = defPixed; // 默认值
	
	std::string fontNameStr(fontName);
	for (auto iterator = fontSetting.begin(); iterator != fontSetting.end(); iterator++) {
		if ((*iterator).first == key) {
			pixel = std::stoi((*iterator).second);
		}
		if ((*iterator).first == "font-name") {
			fontNameStr = (*iterator).second; 
		}
	}	

	wxFont font({0, pixel}, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,  wxFONTWEIGHT_NORMAL, false, fontNameStr);
	// lf.lfHeight = pixel; // request a 8-pixel-height font
	if(bold) {
		font.SetWeight(wxFONTWEIGHT_BOLD);  
	}
	
	return font;
}


/**
 * 获得ini语言配置文件[FONT]节点中的文本字体.
 * 
 * @param defFontName
 * @return 
 */
std::string Lang::fontName(const char * defFontName /*= "Microsoft Yahei UI"*/)
{
	std::string fn = SettingService::getInstance()->getValBySectionAndKey(std::string("FONT"), std::string("font-name"));
	if (fn.empty()) {
		return std::string(defFontName);
	}
	return fn;
}

/**
 * 获得ini语言配置文件[FONT]节点中的文本大小.
 * 
 * @param key
 * @param defPixed
 * @return 
 */
int Lang::fontSize(const char * key, int defPixed /*= 20*/)
{
	Setting fontSetting = SettingService::getInstance()->getSettingBySection(std::string("FONT"));
	int pixel = defPixed; // 默认值
	
	
	for (auto iterator = fontSetting.begin(); iterator != fontSetting.end(); iterator++) {
		if ((*iterator).first == key) {
			pixel = std::stoi((*iterator).second);
			break;
		}
	}	

	return pixel;
}
