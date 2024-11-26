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

 * @file   StringUtil.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-11-04
 *********************************************************************/
#include "StringUtil.h"


/**
* 合并名字.
* 
* @param name1
* @param name2
* @return 
*/
std::string StringUtil::mergeName(std::string &name1, std::string name2)
{
	std::string name = name1;
	if (name1.empty() == false && name2.empty() == false) {
		name.append(" ");
	}
	name.append(name2);
	return name;
}

/**
* 去掉最后一个字符.
* 
* @param name1
* @param name2
* @return 
*/
std::string StringUtil::cutLastChar(std::string &str)
{
	if (str.empty()) {
		return str;
	}

	return str.substr(0, str.size() - 1);
}

/**
* 字符串替换函数
* #function name   : replace_str()
* #param str       : 操作之前的字符串
* #param before    : 将要被替换的字符串
* #param after     : 替换目标字符串
* #param ignoreCase : 忽略大小写
* #return          : void
*/
std::string StringUtil::replace(const std::string & str, const std::string& before, const std::string& after, bool ignoreCase /*= false*/)
{		
	std::string text = str;
	if (!ignoreCase) {			
		for (std::string::size_type pos(0); pos != std::string::npos; pos += after.length()) {
			pos = text.find(before, pos);
			if (pos != std::string::npos)
				text.replace(pos, before.length(), after);
			else
				break;
		}
	} else {
		std::regex pattern(before, std::regex::icase);
		text = std::regex_replace(text, pattern, after, std::regex_constants::match_any);
	}
		
	return text;
}

/**
* Replace the the line break : \r\n.
* 
* @param str
* @return 
*/
std::string StringUtil::replaceBreak(const std::string & str)
{
	std::string res = replace(str, "\\r", "\r");
	res = replace(res, "\\n", "\n");
	return res;
}

/**
 *  Format the the line break : \n  -> \r\n..
 * 
 * @param str
 * @return 
 */
std::string StringUtil::formatBreak(const std::string & str)
{
	std::string res = replace(str, "\r\n", "\n");
	res = replace(res, "\n", "\r\n");
	return res;
}

/**
* search if the "search" string is exists in the "str" string.
* 
* @param str - original text
* @param search - search text
* @param ignoreCase - ignore case 
* @return 
*/
bool StringUtil::search(const std::string & str, const std::string & search, bool ignoreCase /*= false*/)
{
	if (!ignoreCase) {
		std::string text = str;
		size_t pos = text.find(search);
		return pos != std::string::npos;
	} else {
		std::regex pattern(search, std::regex::icase);
		return std::regex_search(str, pattern);
	}
}

/**
 * search if the "search" string is start of the "str" string..
 * 
 * @param str - original text
 * @param search - search text
 * @param ignoreCase - ignore case 
 * @return 
 */
bool StringUtil::startWith(const std::string & str, const std::string & search, bool ignoreCase /*= false*/)
{
	if (!ignoreCase) {
		std::string text = str;
		size_t pos = text.find(search);
		return pos == 0;
	} else {
		std::string patSearch = "^" + search;
		std::regex pattern(patSearch, std::regex::icase);
		return std::regex_search(str, pattern);
	}
}


bool StringUtil::endWith(const std::string & str, const std::string & search, bool ignoreCase /*= false*/)
{
	if (!ignoreCase) {
		std::string text = str;
		size_t pos = text.rfind(search);
		return pos!= std::string::npos && pos == (text.size() - search.size() - 1);
	} else {
		std::string patSearch = search + "$" ;
		std::regex pattern(patSearch, std::regex::icase);
		return std::regex_search(str, pattern);
	}
}

/**
* 取小数点后几位，并转成字符串
* 
* @param val
* @param fixed
* @return 
*/
std::string StringUtil::doubleToString(double val, int fixed)
{
	std::ostringstream oss;
	oss << std::setprecision(fixed) << val;
	return oss.str();
}

/**
* 字符串分割函数.
* 
* @param str 待分离字符串
* @param pattern 分离字符串,如逗号
* @return 
*/
std::vector<std::string> StringUtil::split(std::string str, const std::string & pattern, bool bTrim /*= true*/)
{
	if (str.empty() || pattern.empty()) {
		return std::vector<std::string>();
	}
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern; // 扩展字符串以方便操作
	size_t size = str.size();
	for (size_t i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			if (s.empty()) {
				i = pos + pattern.size() - 1;
				continue;
			}
			if (bTrim && !s.empty()) {
				StringUtil::trim(s);
			}
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

/**
 * 字符串分割函数.
 * 
 * @param str 待分离字符串
 * @param pattern 分离字符串,如逗号
 * @param cutEnclose 返回的字符串数组，字符串去掉指定的前后关闭字符
 * @return 
 */
std::vector<std::string> StringUtil::splitCutEnclose(std::string str, const std::string & pattern, std::string & cutEnclose, bool bTrim /*= true*/)
{
	if (str.empty() || pattern.empty()) {
		return std::vector<std::string>();
	}

	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern; // 扩展字符串以方便操作
	size_t size = str.size();
	for (size_t i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			if (s.empty()) {
				i = pos + pattern.size() - 1;
				continue;
			}
			
			// 返回的字符串数组中的字符串，不包含在关闭字符串中
			if (!cutEnclose.empty()) {
				size_t _begin = s.find(cutEnclose);
				size_t _end = s.rfind(cutEnclose);
				if (_begin != std::string::npos && _end != std::string::npos) {
					s = s.substr(_begin + 1, _end - _begin - 1);
				}
			}
			if (bTrim && !s.empty()) {
				StringUtil::trim(s);
			}
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

std::vector<std::string> StringUtil::splitByBlank(std::string str, bool bTrim /*= true*/)
{
	if (str.empty()) {
		return std::vector<std::string>();
	}
	std::string::size_type pos;
	std::vector<std::string> result;
	str.append(" ");
	size_t size = str.size();
	for (size_t i = 0; i < size; i++) {
		pos = str.find_first_of(" \n\t\v\r\f", i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			if (s.empty()) {
				i = pos ;
				continue;
			}
			if (bTrim && !s.empty()) {
				StringUtil::trim(s);
			}
			result.push_back(s);
			i = pos ;
		}
	}
	return result;
}

/**
* 字符串分割函数，pattern不包含在notBegin, notEnd之间
* 
* @param str 待分离字符串
* @param pattern 分离字符串,如逗号
* @param notBegin 分离字符串不包含在此字符串开始（开始和结束前遇到pattern字符串，则忽略）
* @param notEnd 分离字符串不包含在此字符串结束（开始和结束前遇到pattern字符串，则忽略）
* @param bTrim
* @return 
*/
std::vector<std::string> StringUtil::splitNotIn(std::string str, const std::string & pattern, 
	const std::string & notBegin, const std::string & notEnd, 
	const std::string & notContain, bool bTrim /*= true*/)
{
	if (str.empty() || pattern.empty()) {
		return std::vector<std::string>();
	}

	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern; // 扩展字符串以方便操作
	std::string upstr = StringUtil::toupper(str); // 大写
	std::string upNotBegin = StringUtil::toupper(notBegin); // 大写
	std::string upNotEnd = StringUtil::toupper(notEnd); // 大写
	std::string upNotContain = StringUtil::toupper(notContain); // 大写
	size_t size = str.size();
	size_t subPos = 0; // 用来记录截取字符串的开始位置
	for (size_t i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(subPos, pos - subPos);
			if (s.empty()) {
				i = pos + pattern.size() - 1;
				subPos = pos + pattern.size();
				continue;
			}
			
			// 返回的字符串数组中的字符串，不包含在关闭字符串中
			if (!notBegin.empty() && !notEnd.empty()) {
				size_t _begin = upstr.find(upNotBegin, i);
				size_t _end = upstr.find(upNotEnd,i);
				size_t _contain = upstr.find(upNotContain,i); //notContain 要保证不在notBegin到notEnd 字符串中
				if (_begin != std::string::npos && _end != std::string::npos && 
					(_contain == std::string::npos || _contain < _begin || _contain >_end)
					) {
					if (pos >= _begin && pos <= _end) {
						i = _end + notEnd.size() - 1;
						// 此时，subPos保持不变，查找的字符串继续往下找
						continue;
					}
				}
			}

			if (bTrim && !s.empty()) {
				StringUtil::trim(s);
			}

			if (!s.empty()) {
				s = convertQuotes(s);
				result.push_back(s);
			}			
			i = pos + pattern.size() - 1;
			subPos = pos + pattern.size(); // 下次截取字符串的位置
		}
	}
	return result;
}

/**
* implode strings with symbol character.
* 
* @param strVec
* @param symbol - split character
* @param bTrim - whether trim the 
* @return 
*/
std::string StringUtil::implode(const std::vector<std::string> & strVec, const std::string & symbol, bool bTrim /*= true*/)
{
	if (strVec.empty()) {
		return "";
	}
	std::string result;
	int i = 0;
	for (auto str : strVec) {
		if (i > 0 && !result.empty()) {
			result.append(symbol);
		}
		if (bTrim && !str.empty()) {
			StringUtil::trim(str);
		}
		if (!str.empty()) {
			result.append(str);
		}
		i++;
	}
	return result;
}

/**
 *  add a character in front of every word and back of every word. the words is split by splitSymbol
 *  such as "id,name,gender" => "'id','name','word'"
 * 
 * @param str - The words string ,such as "id,name,gender"
 * @param splitSymbol - The split symbol , such as ","
 * @param addSymbol - The symbol to add, such as "\"" , "'"
 * @return string "'id','name','word'"
 */
std::string StringUtil::addSymbolToWords(const std::string & str, const std::string & splitSymbol, const std::string & addSymbol)
{
	if (str.empty() || splitSymbol.empty() || addSymbol.empty()) {
		return str;
	}
	std::string newStr = str;
	newStr.append(splitSymbol);
	auto words = StringUtil::split(newStr, splitSymbol, true);
	std::string result;
	size_t n = words.size();
	for (size_t i = 0; i < n; i++) {
		auto word = words.at(i);
		word = replace(word, addSymbol, "");
		if (word.empty()) {
			continue;
		}
		if (!result.empty()) {
			result.append(splitSymbol);
		}

		std::string newWorld = addSymbol;
		newWorld.append(word).append(addSymbol);
		result.append(newWorld);
	}

	return result;
}

/**
 * Remove leading and trailing whitespace.
 * 
 * @param inout_str
 */
void StringUtil::trim(std::string &inout_str)
{
	static const char whitespace[] = " \n\t\v\r\f";
	inout_str.erase(0, inout_str.find_first_not_of(whitespace));
	inout_str.erase(inout_str.find_last_not_of(whitespace) + 1U);
}

std::string StringUtil::escape(std::string &source)
{
	if (source.empty()) {
		return source;
	}

	// replace the escapre sequance
	std::string result = replace(source, "\\", "\\\\");
	result = replace(result, "\"", "\\\""); 
	result = replace(result, "\r", "\\r");
	result = replace(result, "\n", "\\n");
	return result;
}

std::string StringUtil::escapeXml(std::string & source)
{
	if (source.empty()) {
		return source;
	}

	std::ostringstream wis;
	// replace the escapre sequance
	if (source.find_first_of(L'<') != -1 
		|| source.find_first_of(L'>') != -1
		|| source.find_first_of(L'\r') != -1
		|| source.find_first_of(L'\n') != -1
		|| source.find_first_of(L'[') != -1
		|| source.find_first_of(L']') != -1) {
		wis << "<![CDATA[" << source << "]]>";
		return wis.str();
	}
	return source;		
}

std::string StringUtil::escapeSql(std::string &source)
{
	if (source.empty()) {
		return source;
	}

	// replace the escape character
	std::string result = replace(source, "\'", "\'\'");
	return result;
}

std::string StringUtil::toupper(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](char & c){ 
		return std::toupper(c); 
	});
	return s;
}

std::string StringUtil::tolower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](char & c){ 
		return std::tolower(c); 
	});
	return s;
}

bool StringUtil::isDigit(std::string & s)
{
	if (s.empty()) {
		return false;
	}

	int n = static_cast<int>(s.length());
	for (int i = 0; i < n; i++) {
		char ch = s.at(i);
		if (!std::isdigit(ch)) {
			return false;
		}
	}
	return true;
}

bool StringUtil::isDecimal(std::string & s)
{
	if (s.empty()) {
		return false;
	}

	int n = static_cast<int>(s.length());
	for (int i = 0; i < n; i++) {
		char ch = s.at(i);
		if (!std::isdigit(ch) && ch != L'.') {
			return false;
		}
	}
	return true;
}

/**
 * append blank to string..
 * 
 * @param str
 * @param nBlk - the blank character length of appending
 * @return 
 */
std::string & StringUtil::blkToTail(std::string & str, int nBlk /*= 3*/)
{
	const char * blk = " "; // blank 
	for (int i = 0; i < nBlk; i++) {
		str.append(blk);
	}
	return str;
}

/**
* Get gid of the parens and quotes.
* 
* @param str
* @return 
*/
std::string & StringUtil::cutParensAndQuotes(std::string & str)
{
	if (str.empty()) {
		return str;
	}
		
	str = StringUtil::replace(str, ")", "");
	str = StringUtil::replace(str, "`", "");
	str = StringUtil::replace(str, "'", "");
	str = StringUtil::replace(str, "\"", "");
	StringUtil::trim(str);

	return str;
}

/**
* Get gid of the quotes.
* 
* @param str
* @return 
*/
std::string & StringUtil::cutQuotes(std::string & str)
{
	if (str.empty()) {
		return str;
	}

	str = StringUtil::replace(str, "`", "");
	str = StringUtil::replace(str, "'", "");
	str = StringUtil::replace(str, "\"", "");
	StringUtil::trim(str);

	return str;
}

/**
 * Fetch a substring between begin char and end char that start find from offset position.
 * e.g., SQL statement:    "SELECT a.*, b.uid, FROM analysis a, (select name1,created_at from customer where name1='jack') b WHERE a.uid=b.uid and a.name2='ma'"
 *       return statement: select name1,created_at from customer where name1='jack'
 * 
 * @param str - string
 * @param begin - begin symbol
 * @param end - end symbol
 * @param offset - offset to start finding
 * @param rescure - rescure search begin symbol and end symbol
 * @return 
 */
std::string StringUtil::inSymbolString(const std::string & str, char begin, char end, size_t offset, bool rescure /*= true*/)
{
	if (str.empty()) {
		return str;
	}

	size_t _begin_pos = str.find_first_of(begin, offset);
	size_t _end_pos = str.find_first_of(end, offset);
	size_t _str_len = str.size();

	if (_begin_pos == std::string::npos || _end_pos == std::string::npos) {
		return str;
	}
	if (!rescure) { // not rescure search, then let end position = the last end symbol position 
		size_t last = str.find_last_of(end, offset);
		if (last == std::string::npos || last >= _str_len) {
			return str;
		}
		return str.substr(_begin_pos + 1, last - _begin_pos - 1);
	}

	std::stack<size_t> stack;
	size_t _find_pos = _begin_pos;
	while (_find_pos < _end_pos) {			
		stack.push(_find_pos);
		_find_pos = str.find_first_of(begin, _find_pos+1);
	}

	_find_pos = _end_pos;
	while (_find_pos < _str_len) {
		_end_pos = _find_pos;
		stack.pop();
		if (stack.empty()) {
			break;
		}			
		_find_pos = str.find_first_of(end, _find_pos+1);
	}

	if (_end_pos == std::string::npos || _end_pos >= _str_len) {
		return str;
	}
	return str.substr(_begin_pos + 1, _end_pos - _begin_pos - 1);
}


std::vector<std::string> StringUtil::inSymbolStrings(const std::string & str, char begin, char end, size_t offset, bool rescure /*= true*/)
{
	std::vector<std::string> result;
	if (str.empty()) {
		return result;
	}

	size_t pos = offset;
	size_t nMax = str.size();
	for (;;) {
		std::string instr = inSymbolString(str, begin, end, pos, rescure);
		if (instr == str || instr.empty()) {
			break;
		}
		result.push_back(instr);
		pos = str.find(instr) + instr.size();
		if (pos == std::string::npos || pos >= nMax) {
			break;
		}		
	}

	return result;
}

/**
 * Fetch a substring not between begin char and end char that start find from offset position.
 * e.g., SQL statement:    "SELECT a.*, b.uid, FROM analysis a, (select name1,created_at from customer where name1='jack') b WHERE a.uid=b.uid and a.name2='ma'"
 *       return statement: "SELECT a.*, b.uid, FROM analysis a, b WHERE a.uid=b.uid and a.name2='ma'"
 * 
 * @param str - string
 * @param begin - begin symbol
 * @param end - end symbol
 * @param offset - offset to start finding
 * @param rescure - rescure search begin symbol and end symbol
 * @return 
 */
std::string StringUtil::notInSymbolString(const std::string & str, char begin, char end, size_t offset, bool rescure /*= true*/)
{
	if (str.empty()) {
		return str;
	}

	size_t pos = offset;
	std::string newstr = str;
	for (;;) {
		std::string instr = inSymbolString(newstr, begin, end, pos, rescure);
		if (instr == newstr || instr.empty()) {
			break;
		}
		newstr = replace(newstr, instr, "");
		pos = 0;
	}

	return newstr;
}

/**
 * Find the next character that it is not blank character including CR/LF/CRLF char and the space char.
 * 
 * @param line
 * @param word
 * @param offset
 * @return 
 */
char StringUtil::nextNotBlankChar(const std::string & line, char * word, size_t offset /*= 0*/)
{	
	if (line.empty() || word == nullptr || !strlen(word)) {
		return 0;
	}
	size_t _line_len = line.size();
	size_t _word_pos = line.find(word, offset);
	if (_word_pos == std::string::npos) {
		return 0;
	}
	size_t pos = _word_pos + strlen(word);
	if (pos >= _line_len) {
		return 0;
	}
	char nextChar = line.at(pos);
	// ignore the blank char
	while (nextChar == L' ' || nextChar == L'\t' 
		|| nextChar == L'\r' || nextChar == L'\n' ) {
		if (++pos > _line_len - 1) {
			break;
		}
		nextChar = line.at(pos);
	}
		
	return nextChar;
}


bool StringUtil::existsIn(const std::string & str, const std::vector<std::string> & strVector)
{
	auto iter = std::find(strVector.begin(), strVector.end(), str);
	if (iter != strVector.end()) {
		return true;
	}
	return false;
}

/**
 * Convert the quotes(\'->'', \" -> "")
 */
std::string& StringUtil::convertQuotes(std::string& str)
{
	if (str.find("\\'") != std::string::npos) {
		str = replace(str, "\\'", "''");
	}

	if (str.find("\\\"") != std::string::npos) {
		str = replace(str, "\\\"", "\"\"");
	}
	return str;
}
