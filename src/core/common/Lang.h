/*****************************************************************//**
 * @file   Lang.h
 * @brief  读取语言配置文件的选项
 * @detail    $DETAIL
 * 
 * @author 覃学涵
 * @date   2023-04-05
 *********************************************************************/
#pragma once
#include <string>
#include <wx/font.h>

// 取指定语言的字符串
#define S(key) Lang::lang(key)
#define ERR(key) Lang::error(key)
#define SNT(key) Lang::langNoTab(key)

// 取指定语言的字体
#define FT(key) Lang::font(key)
#define FN(key) Lang::fontName(key)
#define FTB(key, bold) Lang::font(key, bold)
#define FTBP(key, bold, pixed) Lang::font(key, bold, pixed)
#define FTBPF(key, bold, pixed, fontName) Lang::font(key, bold, pixed, fontName)


class Lang {
public:
	/**
	 * 获得指定语言的文本.
	 */
	static std::string lang(const char * key);

	/**
	 * 获得指定语言的文本.
	 */
	static std::string error(const std::string & key);

	/**
	 * 获得指定语言的文本.
	 */
	static std::string langNoTab(const std::string & key);

	/**
	 * 获得指定语言的字体.
	 * 
	 * @param key
	 * @return 
	 */
	static wxFont font(const char * key, bool bold = false, int defPixed = 20, const char * fontName  = "Microsoft Yahei UI");

	/**
	 * 获得字体名.
	 * 
	 * @return 
	 */
	static std::string fontName(const char * defFontName = "Microsoft Yahei UI");

	/**
	 * 获得指定的字体大小.
	 * 
	 * @param key
	 * @param defPixed
	 * @return 
	 */
	static int fontSize(const char * key, int defPixed = 20);
};





