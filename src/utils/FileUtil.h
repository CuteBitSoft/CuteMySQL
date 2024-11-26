#pragma once

#include <io.h>
#include <direct.h>
#include <string>
#include <fstream>
#include "StringUtil.h"
#include "DateUtil.h"
#include "ResourceUtil.h"
#include "Log.h"
#include "core/common/exception/QRuntimeException.h"

#define MAX_PATH_LEN 256
#define ACCESS(fileName, accessMode) _access(fileName, accessMode)
#define MKDIR(path) _mkdir(path)

class FileUtil
{
public:
	static unsigned int createDirectory(const std::string &directoryPath);

	/**
	 * Get the file extension.
	 * 
	 * @param filePath
	 * @return 
	 */
	static std::string getFileExt(const std::string &filePath);

	/**
	 * ����ļ���.
	 * 
	 * @param filePath �ļ�·��
	 * @param includeExt �Ƿ������׺��
	 * @return 
	 */
	static std::string getFileName(const std::string &filePath, bool includeExt = true);

	// ����ļ�·��,·��������Ŀ¼�ָ��'\\' '/'��֧�������ļ�Ŀ¼�ָ����� '\\' '/'
	static std::string getFileDir(const std::string &filePath);
	

	static bool copy(const std::string & fromPath, const std::string & toPath);

	static void saveFile(std::string path, size_t size, void * buffer);


	static std::string readFile(const std::string & path);

	static std::string readFirstLine(const std::string & path, const std::string & encoding = "UTF-8");
};





