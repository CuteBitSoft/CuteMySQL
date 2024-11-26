#pragma once

#include <string>
#include <unordered_map>
#include <atldef.h>
#include <memory>
#include <fstream>
#include <vector>
#include <utility>
#include <cassert>
#include "utils/Log.h"
#include "utils/ResourceUtil.h"
#include "utils/FileUtil.h"
#include "utils/ThreadUtil.h"
#include "utils/ResourceUtil.h"
#include "common/Config.h"

#include "core/common/repository/QConnect.h"
#include "core/common/driver/sqlite/QSqlDatabase.h"
#include "core/common/driver/sqlite/QSqlStatement.h"
#include "core/common/driver/sqlite/QSqlColumn.h"


//sql where条件使用的类型
typedef std::unordered_map<std::string, std::string>  QCondition;
//sql where 条件语句生成like keyword包含的字段
typedef std::vector<std::string> QKeywordIncludes;

// 排序字段
typedef struct QSortField {
	std::string field;
	std::string ascDesc;
} QSortField ;
// 排序字段列表
typedef std::vector<QSortField> QSortFieldList;

//sql limit 分页语句<curpage, perpage>
typedef std::pair<int, int> QPagePair;

/**
 * @brief 所有存储类的基类
 * @author Qinxuehan
 * @since 2022-03-21
 */
template <typename T>
class BaseRepository
{
public:
	static T * getInstance();//singleton
	static void destroyInstance();

    // 配置本地DB存放目录
    void setLocalDir(const std::string & path);

    std::string getErrorMsg();
    std::string getErrorCode();

	void setError(std::string code, std::string msg);
    void setErrorCode(std::string code);
    void setErrorMsg(std::string msg);

	SQLite::QSqlDatabase * getSysConnect();
	void closeConnect();
	void colseSysConnect();
	void colseUserConnect();
protected:
	static T * theInstance;
	bool isInitConnect = false;
	
    std::unordered_map<uint32_t, std::string> errorCode;
    std::unordered_map<uint32_t, std::string> errorMsg;
	std::string localDir;

	uint64_t getLastId(std::string tbl);
	std::string initSysDbFile();


	
	//生成WHERE语句
	std::string wherePrepareClause(QCondition & condition, 
		QKeywordIncludes & keywordIncludes = std::vector<std::string>(), bool caseSensitive = true);


	//生成ORDER BY 语句
	std::string orderClause(const QSortFieldList & sortFieldList, const std::string &defaultOrderClause=std::string(" ORDER BY created_at DESC "));

	std::string limitClause(const QPagePair & pagePair);

	//生成IN语句
	std::string whereInClause(std::string field, std::vector<uint64_t> & vec);

	void queryBind(SQLite::QSqlStatement & query, QCondition & condition) const;
};

template <typename T>
uint64_t BaseRepository<T>::getLastId(std::string tbl)
{
	std::string sql = "SELECT max(id) FROM :tbl ";

	try {
		SQLite::QSqlStatement query(getSysConnect(), sql.c_str());
	
		//执行语句
		if (!query.executeStep()) {
			return 0;			
		}
		
		uint64_t maxId = query.getColumn(0).getUInt64();
		return maxId;
	} catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("exec sql has error, code:{}, msg:{}, sql:{}", e.getErrorCode(), _err, sql);
		throw QRuntimeException("10018", "sorry, system has error.");
	}	
}

template <typename T>
T * BaseRepository<T>::theInstance = nullptr;

template <typename T>
T * BaseRepository<T>::getInstance()
{
	if (BaseRepository<T>::theInstance == nullptr) {
		BaseRepository<T>::theInstance = new T();
	}
	return BaseRepository<T>::theInstance;
}

template <typename T>
void BaseRepository<T>::destroyInstance()
{
	if (BaseRepository<T>::theInstance) {
		delete BaseRepository<T>::theInstance;
		BaseRepository<T>::theInstance = nullptr;
	}
}
// 配置本地工程存放目录
template <typename T>
void  BaseRepository<T>::setLocalDir(const std::string & dir)
{
    localDir = dir;
}

template <typename T>
SQLite::QSqlDatabase * BaseRepository<T>::getSysConnect()
{
	if (QConnect::sysConnect == nullptr) {
		//auto conn = std::make_shared<QSqlDatabase>("HairAnalyzer");
		//connect = conn.get();
		QConnect::sysConnect = new SQLite::QSqlDatabase("CuteMySQL");
	}
	
    Q_INFO("BaseRepository::getConnect, local db connect...");
    Q_INFO("BaseRepository::getConnect, connect.isValid():{}, connect.isOpen():{}" , QConnect::sysConnect->isValid() , QConnect::sysConnect->isOpen());

	if (!QConnect::sysConnect->isValid() || !QConnect::sysConnect->isOpen()) {
		std::string dbPath = initSysDbFile();
		Q_INFO("db path:{}",dbPath.c_str());
		QConnect::sysConnect->setDatabaseName(dbPath);

		if (!QConnect::sysConnect->open()) {
			Q_INFO("db connect.open Error:{}", QConnect::sysConnect->lastError());
			setErrorMsg(QConnect::sysConnect->lastError());
			assert(QConnect::sysConnect->isValid() && QConnect::sysConnect->isOpen());
		}
	}

    return QConnect::sysConnect;
}

template <typename T>
void BaseRepository<T>::closeConnect()
{
	colseSysConnect();
	colseUserConnect();
}

template<typename T>
inline void BaseRepository<T>::colseSysConnect()
{
	if (QConnect::sysConnect == nullptr) {
		return;
	}

	// 1) close the connect
	if (QConnect::sysConnect->isValid() && QConnect::sysConnect->isOpen()) {
		QConnect::sysConnect->close();
	}
	// 2) delete the ptr
	delete QConnect::sysConnect;
	QConnect::sysConnect = nullptr;
}

template<typename T>
inline void BaseRepository<T>::colseUserConnect()
{
	if (QConnect::userConnectPool.empty()) {
		return;
	}
	for (auto& pair : QConnect::userConnectPool) {
		auto ptr = pair.second;
		if (ptr->isValid() || !ptr->isClosed()) {
			ptr->close();
		}
		delete ptr;
		ptr = nullptr;
		pair.second = nullptr;
	}
	QConnect::userConnectPool.clear();
}

template <typename T>
std::string BaseRepository<T>::getErrorMsg()
{
	uint32_t threadId = ThreadUtil::currentThreadId();
    return errorMsg[threadId];
}

template <typename T>
std::string BaseRepository<T>::getErrorCode()
{
	uint32_t threadId = ThreadUtil::currentThreadId();
    return errorCode[threadId];
}


template <typename T>
void BaseRepository<T>::setErrorMsg(std::string msg)
{
	uint32_t threadId = ThreadUtil::currentThreadId();
    errorMsg[threadId] = msg;
}

template <typename T>
void BaseRepository<T>::setErrorCode(std::string code)
{
	uint32_t threadId = ThreadUtil::currentThreadId();
    errorCode[threadId] = code;
}

template <typename T>
void BaseRepository<T>::setError(std::string code, std::string msg)
{
	uint32_t threadId = ThreadUtil::currentThreadId();
    errorCode[threadId] = code;
    errorMsg[threadId] = msg;
}


template <typename T>
std::string BaseRepository<T>::initSysDbFile()
{
	if (localDir.empty()) {
		localDir = ResourceUtil::getProductBinDir();
	}
    assert(!localDir.empty());

    std::string dbDir = localDir + "/.magic/data/" ;
    std::string destPath = dbDir + "CuteMySQL.s3db";
    if (_access(destPath.c_str(), 0) != 0) { //文件不存在
		std::string origPath = localDir + "/res/db/CuteMySQL.s3db" ;
		assert(_access(origPath.c_str(), 0) == 0);

		FileUtil::copy(origPath, destPath);
    }
    
    return destPath;
}

/**
 * Wheres clause for QSqlStatement::prepare() function
 * 
 * @param condition - The condition params map of where clause.
 * @param keywordIncludes - The <key> vector for generating <like clause> in <where clause>, such as "where <key> like :keyword"
 * @return 
 */
template <typename T>
std::string BaseRepository<T>::wherePrepareClause(QCondition & condition, QKeywordIncludes & keywordIncludes, bool caseSensitive)
{
	//where-语句
	std::string whereClause;
	if (condition.empty()) {
		return whereClause;
	}
	whereClause.append(" WHERE 1 ");
	for (auto item : condition) {
		if (item.first == "keyword") {
			continue;;
		}
		if (item.second.empty()) {//不支持传空的字符串
			continue;;
		}
		whereClause.append(" AND ")
			.append(item.first)
			.append("=")
			.append(":")
			.append(item.first)
			.append(" ");
	}

	// keyword语句,根据字段拼接
	if (condition.find("keyword") != condition.end()
		&& !condition["keyword"].empty() && !keywordIncludes.empty()) {
		whereClause.append(" AND (0 ");
		for (auto key : keywordIncludes) {
			if (caseSensitive) {
				whereClause.append(" OR ")
					.append(key)
					.append(" like :keyword");
			} else {
				whereClause.append(" OR ")
					.append(key)
					.append(" like :low_keyword")
					.append(" OR ")
					.append(key)
					.append(" like :up_keyword");
			}
			
		}
		whereClause.append(") ");
	}

	return whereClause;
}
template <typename T>
std::string BaseRepository<T>::whereInClause(std::string field, std::vector<uint64_t> & vec)
{
	std::string whereClause;
	if (vec.empty()) {
		return whereClause;
	}

	whereClause.append(" WHERE ").append(field).append(" in (");
	std::string insql = "";
	for (auto item : vec) {
		insql.append(std::to_string(item)).append(",");
	}
	//去掉最后一个逗号
	insql = !insql.empty() ? insql.substr(0,  insql.size()-1) : "";
	whereClause.append(insql).append(")");

	return whereClause;
}

//生成ORDER语句，如果不需要则指定默认的defaultOrderClause
template <typename T>
std::string BaseRepository<T>::orderClause(const QSortFieldList & sortFieldList, const std::string &defaultOrderClause)
{
	std::string orderSql = " ORDER BY ";
	if (sortFieldList.empty()) {
		return defaultOrderClause;
	}

	for (auto item : sortFieldList) {
		orderSql.append(item.field).append(" ");
		orderSql.append(item.ascDesc).append(",");
	}
	if (!sortFieldList.empty()) {
		//去掉最后一个逗号
		orderSql = orderSql.substr(0, orderSql.size() - 1);

	}
	return orderSql;
}

template <typename T>
std::string BaseRepository<T>::limitClause(const QPagePair & pagePair)
{
	if (pagePair.first <= 0 && pagePair.second <= 0) {
		return "";
	}
	int pos = (pagePair.first-1) * pagePair.second;
	std::string limitSql = " LIMIT ";
	std::string perpage = std::to_string(pagePair.second);
	std::string strpos = std::to_string(pos);
	limitSql.append(perpage).append(" OFFSET ").append(strpos);

	return limitSql;
}

template <typename T>
void BaseRepository<T>::queryBind(SQLite::QSqlStatement & query, QCondition & condition) const
{
	if (condition.empty()) {
		return ;
	}
	for (auto item : condition) {
		if (item.first == "keyword" && condition["keyword"].empty()) {
			continue;
		}
		if (item.second.empty()) {//不支持传空的字符串
			continue;
		}
		std::string apname = ":" + item.first;
		std::string val;
		if (item.first == "keyword") {
			val = "%";
			val.append(item.second).append("%");
		}else {
			val = item.second;
		}
		
		query.bind(apname, val);
	}
}




