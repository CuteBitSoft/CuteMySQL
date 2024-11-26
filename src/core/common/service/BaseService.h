#pragma once

#include <unordered_map>
#include <string>
#include "utils/ThreadUtil.h"

/**
 * 所有service的模板基类
 * 定义模板：
 * typename T - BaseService的子类
 * typename R - BaseRepository的子类
 */
template <typename T, typename R>
class BaseService
{
public:
	static T * getInstance();
	static R * getRepository();
	static void destroyInstance();

	std::string & getErrorCode();
	std::string & getErrorMsg();

	void setErrorCode(std::string code);
	void setErrorMsg(std::string msg);
	void setError(std::string code, std::string  msg);

protected:
	static T * theInstance;
	static R * theRepository;

	std::unordered_map<unsigned long, std::string> errorCode;
	std::unordered_map<unsigned long, std::string> errorMsg;

	~BaseService();
};


template <typename T, typename R> 
T * BaseService<T, R>::theInstance = nullptr;

template <typename T, typename R> 
R * BaseService<T, R>::theRepository = nullptr;


template <typename T, typename R>
T * BaseService<T, R>::getInstance()
{
	if (BaseService<T, R>::theInstance == nullptr) {
		BaseService<T, R>::theInstance = new T();
	}

	return BaseService<T, R>::theInstance;
}

template <typename T, typename R>
R * BaseService<T, R>::getRepository()
{
	if (BaseService<T, R>::theRepository == nullptr) {
		BaseService<T, R>::theRepository = R::getInstance();
	}
	return BaseService<T, R>::theRepository;
}

template<typename T, typename R>
inline BaseService<T, R>::~BaseService()
{
	if (BaseService<T, R>::theRepository != nullptr) {
		BaseService<T, R>::theRepository->closeConnect();
	}
	R::destroyInstance();

	BaseService<T, R>::theRepository = nullptr;
}

template<typename T, typename R>
void BaseService<T, R>::destroyInstance()
{
	if (BaseService<T, R>::theInstance != nullptr) {
		delete BaseService<T, R>::theInstance;
		BaseService<T, R>::theInstance = nullptr;
	}

}

template <typename T, typename R>
std::string & BaseService<T, R>::getErrorCode()
{
	unsigned long theadId = ThreadUtil::currentThreadId();
	return errorCode[theadId];
}

template <typename T, typename R>
std::string & BaseService<T, R>::getErrorMsg()
{
	unsigned long theadId = ThreadUtil::currentThreadId();
	return errorMsg[theadId];
}

template <typename T, typename R>
void BaseService<T, R>::setErrorCode(std::string code)
{
	unsigned long theadId = ThreadUtil::currentThreadId();
	errorCode[theadId] = code;
}

template <typename T, typename R>
void BaseService<T, R>::setErrorMsg(std::string msg)
{
	unsigned long theadId = ThreadUtil::currentThreadId();
	errorMsg[theadId] = msg;
}

template <typename T, typename R>
void BaseService<T, R>::setError(std::string code, std::string  msg)
{
	unsigned long theadId = ThreadUtil::currentThreadId();
	errorCode[theadId] = code;
	errorMsg[theadId] = msg;
}



