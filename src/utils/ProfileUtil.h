/*****************************************************************//**
 * @file   ProfileUtil.h
 * @brief  ����ini�����ļ��Ĺ�����
 * @detail    $DETAIL
 * 
 * @author ��ѧ��
 * @date   2023-03-03
 *********************************************************************/
#pragma once
#include <stdio.h> 
#include <vector>
#include <string>
#include <utility>
#include "utils/Log.h"
#include "core/entity/Entity.h"

class ProfileUtil {
public:
	/**
	 * ���ͳ�ini�ļ������е�section����.
	 * 
	 * @param [in]iniProfilePath  ini�ļ�·��
	 * @param [out]sections ���ص�sections����
	 */
	static void parseSections(std::string & iniProfilePath, std::vector<std::string> &sections)
	{
		// �ο��ֲ᣺https://learn.microsoft.com/zh-cn/windows/win32/api/winbase/nf-winbase-getprivateprofilesectionnames
		char buff[1024] = {0}; // ���ص������У���'\0'����һ��section; ���е�section�ַ�������������ټ�һ��\0;
		DWORD buff_len = GetPrivateProfileSectionNamesA(buff,  1024, iniProfilePath.c_str());
		if (buff_len == 0) {
			return ;
		}

		ProfileUtil::parseReturnBuffer(buff, buff_len, sections);

	}

	/**
	 * ���ͳ�ini�ļ���section�µ�������<key,val>.
	 * 
	 * @param [in]iniProfilePath - ini�ļ�
	 * @param [in]section - ��
	 * @param [out]settings - <key,val>��map
	 */
	static void parseSectionSettings(std::string & iniProfilePath, std::string &section, Setting &settings)
	{
		// �ο��ֲ᣺https://learn.microsoft.com/zh-cn/windows/win32/api/winbase/nf-winbase-getprivateprofilesectionw
		char buff[30720] = {0}; // ���ص������У���'\0'����һ��section; ���е�section�ַ�������������ټ�һ��\0;
		memset(buff, 0, sizeof(buff));
		DWORD buff_len = GetPrivateProfileSectionA(section.c_str(), buff,  30720, iniProfilePath.c_str());
		if (buff_len == 0) {
			return ;
		}

		std::vector<std::string> itemList ; //key=val���ַ�������
		ProfileUtil::parseReturnBuffer(buff, buff_len, itemList); //�Ƚ��ͳ�ÿһ����
		for (std::string item : itemList) {
			std::pair<std::string, std::string> pair;
			if (ProfileUtil::explode(item.c_str(),  L'=',  pair)) {
				settings.push_back(pair);
			}
			
		}
	}

	/**
	 * ����ָ��Ŀ¼�����е��ļ�.
	 * 
	 * @param [in]iniProfileDir  ָ��Ŀ¼�²���ini�ļ�
	 * @param [out]languages ���Ե����飬pair<std::string, std::string> ����<language, filepath>
	 */
	static void parseLanguagesFromDir(std::string & iniProfileDir, std::vector<std::pair<std::string,std::string>> &languages)
	{
		WIN32_FIND_DATAA wfd;
		HANDLE hFind = INVALID_HANDLE_VALUE;

		std::string findPath = iniProfileDir;
		findPath.append("*.ini");
		hFind = ::FindFirstFileA(findPath.c_str(), &wfd);

		languages.clear();
		if (hFind == INVALID_HANDLE_VALUE) {
			Q_ERROR("Has error :INVALID_HANDLE_VALUE, path:{}", iniProfileDir.c_str());
			return ;
		}

		do {
			std::string iniFindPath = iniProfileDir;
			iniFindPath.append(wfd.cFileName);
			if (_access(iniFindPath.c_str(), 0) != 0) {
				Q_ERROR("file not exisits, path:{}", iniFindPath.c_str());
				continue;
			}
			Setting langSetting;
			std::string lang = "LANG";
			parseSectionSettings(iniFindPath, lang, langSetting);
			if (langSetting.empty())  continue;
			
			std::pair<std::string, std::string> pair;
			std::string language = langSetting.at(0).second;
			if (language.empty()) continue;
			pair.first = language;
			pair.second = iniFindPath;
			languages.push_back(pair);
		} while (FindNextFileA(hFind, &wfd) != 0);

		::FindClose(hFind);

	}
private:
	/**
	 * �����ݽ��ͳ���.
	 * 
	 * @param [in]buff
	 * @param [in]buff_len
	 * @param [out]vec
	 */
	static void parseReturnBuffer(char * buff, DWORD buff_len, std::vector<std::string> &vec)
	{
		if (buff == nullptr || buff_len <=0 ) {
			return ;
		}

		size_t cp_size = 0;
		char * _start = buff; //��ʼ���Ƶ��ַ���
		char * p = buff;
		for (DWORD n = 0; n < buff_len; p++, n++) {
			if (*p == L'\0') { //�����ַ�\0�ͱ�ʾ�ַ������� 
				char * _dest = new char[cp_size + 1];
				memset(_dest, 0, (cp_size + 1) * sizeof(char));
				memcpy(_dest, _start, cp_size * sizeof(char));
				vec.push_back(_dest);
				delete[] _dest;
				cp_size = 0;
				if (n < buff_len) {					
					_start = p + 1;
				}

				if (*(p + 1) == L'\0') { //pָ��֮��һ���ַ�Ϊ\0���ͱ�ʾ����section�ַ���ȫ������
					break;
				}
			}
			cp_size++;
		}
	}

	/**
	 * �����ַ�����KV�ķ�ʽ.
	 * 
	 * @param [in]buff �ַ���
	 * @param [in]ch �ָ���
	 * @param [out]pair ��ֵ�� 
	 * @return ����ɹ�/ʧ��
	 */
	static bool explode(const char * buff, char ch, std::pair<std::string, std::string> & pair)
	{
		if (buff == nullptr || sizeof(buff) == 0) {
			return false;
		}
		std::vector<std::string> vec;
		size_t cp_size = 0;
		size_t buff_len = strlen(buff) ;

		char * tmpbuf = new char[buff_len+1];
		memset(tmpbuf, 0, (buff_len +1) * sizeof(char));
		memcpy(tmpbuf, buff, buff_len * sizeof(char));

		char * p = tmpbuf;
		char * _start = tmpbuf; //��ʼ���Ƶ��ַ���
		for (DWORD n = 0; n < buff_len+1; p++, n++) {
			if (*p == ch || *p == L'\0') { //�����ַ�=0�ͱ�ʾ�ַ������� 
				char * _dest = new char[cp_size + 1];
				memset(_dest, 0, (cp_size + 1) * sizeof(char));
				memcpy(_dest, _start, cp_size * sizeof(char));
				vec.push_back(_dest);
				delete[] _dest;
				cp_size = 0;
				if (n < buff_len+1) {					
					_start = p + 1;
				}				
			}
			cp_size++;
		}
		delete[] tmpbuf;
		if (vec.empty() || vec.size() < 1) {
			return false;
		}
		pair.first = vec[0];
		pair.second = vec[1];
		return true;
	}

};
