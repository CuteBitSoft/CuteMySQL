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

 * @file   FileUtil.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-11-04
 *********************************************************************/
#include <cassert>
#include "FileUtil.h"


unsigned int FileUtil::createDirectory(const std::string &directoryPath)
{
	size_t dirPathLen = directoryPath.length();
	if (dirPathLen > MAX_PATH_LEN) {
		return 1;
	}
	char tmpDirPath[MAX_PATH_LEN] = {0};
	for (uint32_t i=0; i< dirPathLen; ++i) {
		tmpDirPath[i] = directoryPath[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/') {
			if (ACCESS(tmpDirPath, 0) != 0) {
				int32_t ret = MKDIR(tmpDirPath);
				if (ret != 0) {
					return ret;
				}
			}
		}
	}
	
	return 0;
}


/**
* Get the file extension.
* 
* @param filePath
* @return 
*/
std::string FileUtil::getFileExt(const std::string &filePath)
{
	size_t p = filePath.find_last_of('.');
	std::string ext = p != std::string::npos ? filePath.substr(p+1, -1) : "";
	return ext;
}

/**
* 获得文件名.
* 
* @param filePath 文件路径
* @param includeExt 是否包含后缀名
* @return 
*/
std::string FileUtil::getFileName(const std::string &filePath, bool includeExt /*= true*/)
{
	size_t p = filePath.find_last_of('\\');
	if (p == std::string::npos) {
		p = filePath.find_last_of('/');
	}
	if (p != std::string::npos && includeExt) {
		std::string fileName = filePath.substr(p+1, -1);
		return fileName;
	}else if (p != std::string::npos && !includeExt) {
		size_t p2 = filePath.find_last_of('.');
		if (p2 > p) {
			return filePath.substr(p+1, p2 - p - 1);
		}
	}
		
	return "";
}

/**
 * 获得文件路径,路径最后包含目录分割符'\\' '/'，支持两种文件目录分隔符符 '\\' '/'.
 * 
 * @param filePath
 * @return 
 */
std::string FileUtil::getFileDir(const std::string &filePath)
{
	size_t p1 = filePath.find_last_of('\\');
	size_t p2 = filePath.find_last_of('/');
	size_t p = 0;
	if (p1 == std::string::npos && p2 == std::string::npos) {
		return "";
	}else if (p1 != std::string::npos && p2 != std::string::npos) {
		p = max(p1, p2);
	}else if (p1 == std::string::npos) {
		p = p2;
	}else if (p2 == std::string::npos) {
		p = p1;
	}
		
	std::string dir = p != std::string::npos ? filePath.substr(0, p+1) : "";
	return dir;
}

bool FileUtil::copy(const std::string & fromPath, const std::string & toPath)
{
	std::string toUserDbDir = FileUtil::getFileDir(toPath);
	assert(!toUserDbDir.empty());
	
	if (_access(toUserDbDir.c_str(), 0) != 0) { //文件目录不存在
		Q_INFO("mkpath:{}", toUserDbDir);
		//创建DB目录，子目录不存在，则创建
		FileUtil::createDirectory(toUserDbDir);
	}

	if (_access(toPath.c_str(), 0) == 0) { // 文件存在,删除后覆盖
		_unlink(toPath.c_str());
	}
	
	assert(_access(fromPath.c_str(), 0) == 0);

	errno_t _err;
	char _err_buf[256] = { 0 };
	FILE * origFile, *destFile;
	_err = fopen_s(&origFile, fromPath.c_str(), "rb"); //原文件
	if (_err != 0 || origFile == NULL) { 
		strerror_s(_err_buf, 256, _err);
		Q_ERROR("orig db file open error,error:{},path:{}", _err_buf, fromPath);
		QRuntimeException ex(std::to_string(_err), _err_buf);
		throw ex;
	}
	_err = fopen_s(&destFile, toPath.c_str(), "wb"); // 目标文件
	if (_err != 0 || destFile == NULL) {
		strerror_s(_err_buf, 256, _err);
		Q_ERROR("dest db file open error,error:{},path:{}", _err_buf, toPath);
		QRuntimeException ex(std::to_string(_err), _err_buf);
		throw ex;
	}
	char ch = fgetc(origFile);
	while (!feof(origFile)) {
		_err = fputc(ch, destFile);
		ch = fgetc(origFile);
	}

	fclose(destFile);
	fclose(origFile);
	return true;
}

void FileUtil::saveFile(std::string path, size_t size, void * buffer)
{
	errno_t _err;
	char _err_buf[80] = { 0 };
	FILE * file;
	
    _err = fopen_s(&file, path.c_str(),  "wb"); //目标文件
	if (_err != 0 || file == NULL) {
		strerror_s(_err_buf, 80, _err);
		assert(_err_buf);
	}
		
	fwrite(buffer, size, 1, file);
	fclose(file);
}


std::string FileUtil::readFile(const std::string & path)
{
	if (_access(path.c_str(), 0) != 0) {
		assert(false);
		return std::string();
	}
	std::ifstream ifs;
	auto codeccvt = new std::codecvt_utf8<char, 0x10ffff, std::codecvt_mode(std::generate_header | std::little_endian)>();
	std::locale utf8(std::locale("C"), codeccvt);
	ifs.imbue(utf8);
	ifs.open(path, std::ios::in);
	if (ifs.bad()) {
		return "";
	}
	std::string result;
	char buff[257];
	while (!ifs.eof()) {
		memset(buff, 0, 257);
		ifs.read(buff, 256);
		result.append(buff); 
	}

	ifs.close();
	return result;
}


std::string FileUtil::readFirstLine(const std::string & path, const std::string & encoding)
{
	std::ifstream ifs;
	if (encoding == "UTF-16") {
		auto codeccvt = new std::codecvt_utf16<char, 0x10ffff, std::codecvt_mode(std::generate_header | std::little_endian)>();
		std::locale utf16(std::locale("C"), codeccvt);
		ifs.imbue(utf16);
	} else {
		auto codeccvt = new std::codecvt_utf8<char, 0x10ffff, std::codecvt_mode(std::generate_header | std::little_endian)>();
		std::locale utf8(std::locale("C"), codeccvt);
		ifs.imbue(utf8);
	}
	
	ifs.open(path, std::ios::in);
	if (ifs.bad()) {
		return "";
	}
	std::string line;
	if (!ifs.eof()) {
		char buff[4096] = {0};
		ifs.getline(buff, 4096);
		line.append(buff);
	}
	ifs.close();

	return line;
}
