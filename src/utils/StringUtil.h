#ifndef _STRINGUTIL_H
#define _STRINGUTIL_H

#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cwctype>
#include <algorithm>
#include <regex>
#include <stack>
#include <wx/string.h>

/// <summary>
/// 字符串转换工具类，
/// </summary>
class StringUtil
{
public:

	/**
	 * 合并名字.
	 * 
	 * @param name1
	 * @param name2
	 * @return 
	 */
	static std::string mergeName(std::string &name1, std::string name2);

	/**
	 * 去掉最后一个字符.
	 * 
	 * @param name1
	 * @param name2
	 * @return 
	 */
	static std::string cutLastChar(std::string &str);

	/**
	 * 字符串替换函数
	 * #function name   : replace_str()
	 * #param str       : 操作之前的字符串
	 * #param before    : 将要被替换的字符串
	 * #param after     : 替换目标字符串
	 * #param ignoreCase : 忽略大小写
	 * #return          : void
	 */
	static std::string replace(const std::string & str, const std::string& before, const std::string& after, bool ignoreCase = false);

	/**
	 * Replace the the line break : \r\n.
	 * 
	 * @param str
	 * @return 
	 */
	static std::string replaceBreak(const std::string & str);

	/**
	 * Format the the line break : \n  -> \r\n.
	 * 
	 * @param str
	 * @return 
	 */
	static std::string formatBreak(const std::string & str);

	/**
	 * search if the "search" string is exists in the "str" string..
	 * 
	 * @param str - original text
	 * @param search - search text
	 * @param ignoreCase - ignore case 
	 * @return 
	 */
	static bool search(const std::string & str, const std::string & search, bool ignoreCase = false);

	/**
	 * search if the "search" string is start of the "str" string..
	 * 
	 * @param str - original text
	 * @param search - search text
	 * @param ignoreCase - ignore case 
	 * @return 
	 */
	static bool startWith(const std::string & str, const std::string & search, bool ignoreCase = false);

	/**
	 * search if the "search" string is end of the "str" string..
	 * 
	 * @param str - original text
	 * @param search - search text
	 * @param ignoreCase - ignore case 
	 * @return 
	 */
	static bool endWith(const std::string & str, const std::string & search, bool ignoreCase = false);


	/**
	 * 取小数点后几位，并转成字符串
	 * 
	 * @param val
	 * @param fixed
	 * @return 
	 */
	static std::string doubleToString(double val, int fixed);


	/**
	 * 字符串分割函数.
	 * 
	 * @param str 待分离字符串
	 * @param pattern 分离字符串,如逗号
	 * @return 
	 */
	static std::vector<std::string> split(std::string str, const std::string & pattern, bool bTrim = true);

	/**
	 * 字符串分割函数.
	 * 
	 * @param str 待分离字符串
	 * @param pattern 分离字符串,如逗号
	 * @param cutEnclose 返回的字符串数组中的字符串，不包含在关闭字符串中
	 * @return 
	 */
	static std::vector<std::string> splitCutEnclose(std::string str, const std::string & pattern, std::string & cutEnclose, bool bTrim = true);

	/**
	 * 使用空字符串分割函数.
	 * 
	 * @param str 待分离字符串
	 * @return 
	 */
	static std::vector<std::string> splitByBlank(std::string str, bool bTrim = true);

	/**
	 * 字符串分割函数，pattern不包含在notBegin, notEnd.
	 * 
	 * @param str 待分离字符串
	 * @param pattern 分离字符串,如逗号
	 * @param notBegin 分离字符串不包含在此字符串开始（开始和结束前遇到pattern字符串，则忽略）
	 * @param notEnd 分离字符串不包含在此字符串结束（开始和结束前遇到pattern字符串，则忽略）
	 * @param notContain notBegin和notEnd之间如果存在notContain字符串，则忽略本次的notBegin和notEnd
	 * @param bTrim
	 * @return 
	 */
	static std::vector<std::string> splitNotIn(std::string str, const std::string & pattern, 
		const std::string & notBegin, const std::string & notEnd, 
		const std::string & notContainIn, bool bTrim = true);

	/**
	 * implode strings with symbol character.
	 * 
	 * @param strVec
	 * @param symbol - split character
	 * @param bTrim - whether trim the 
	 * @return 
	 */
	static std::string implode(const std::vector<std::string> & strVec, const std::string & symbol, bool bTrim = true);

	/**
	 * add a character in front of every word and back of every word. the words is split by splitSymbol
	 * such as "id,name,gender" => "'id','name','word'"
	 */
	static std::string addSymbolToWords(const std::string & str, const std::string & splitSymbol, const std::string & addSymbol);

	// Remove leading and trailing whitespace
	static void trim(std::string &inout_str);

	static std::string escape(const std::string &source);
	static std::string escapeXml(const std::string & source);	
	static std::string escapeSql(const std::string &source);

	static std::string toupper(std::string s);
	static std::string tolower(std::string s);

	static bool isDigit(std::string & s);
	static bool isDecimal(std::string & s);

	/**
	 * append blank to string.
	 */
	static std::string & blkToTail(std::string & str, int nBlk = 3);

	/**
	 * Get gid of the parens and quetes.
	 * 
	 * @param str
	 * @return 
	 */
	static std::string & cutParensAndQuotes(std::string & str);

	/**
	 * Get gid of the parens and quotes.
	 * 
	 * @param str
	 * @return 
	 */
	static std::string & cutQuotes(std::string & str);

	/**
	 * Fetch a substring between begin char and end char that start find from offset position.
	 * 
	 * @param str - string
	 * @param begin - begin symbol
	 * @param end - end symbol
	 * @param offset - offset to start finding
	 * @param rescure - 
	 * @return 
	 */
	static std::string inSymbolString(const std::string & str, char begin, char end, size_t offset, bool rescure = true);

	//  Fetch a substring between begin char and end char that start find from offset position.
	static std::vector<std::string> inSymbolStrings(const std::string & str, char begin, char end, size_t offset, bool rescure = true);

	// Fetch a substring between begin char and end char that start find from offset position.
	static std::string notInSymbolString(const std::string & str, char begin, char end, size_t offset, bool rescure = true);

	/**
	 * Find the next character that it is not blank character.
	 * 
	 * @param line
	 * @param word
	 * @param offset
	 * @return 
	 */
	static char nextNotBlankChar(const std::string & line, char * word, size_t offset = 0);

	
	static bool existsIn(const std::string & str, const std::vector<std::string> & strVector);

	/**
	 * Convert the quotes(\'->'', ")
	 */
	static std::string& convertQuotes(std::string& str);

	//convert utf8 to UI charset of local used
	static std::string converFromUtf8(const std::string &str);
	static wxString converFromUtf8(const wxString &str);
};



#endif