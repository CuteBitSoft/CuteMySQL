#include "SysInitRepository.h"
#include "core/common/driver/sqlite/QSqlException.h"
#include "core/common/driver/sqlite/QSqlDatabase.h"
#include "core/common/driver/sqlite/QSqlStatement.h"
#include "core/common/driver/sqlite/QSqlColumn.h"
#include "core/common/exception/QRuntimeException.h"
#include "utils/Log.h"
#include "core/entity/Entity.h"

std::string SysInitRepository::get(const std::string & name)
{
	std::string sql = "SELECT * FROM sys_init name=:name;";

	try {
		SQLite::QSqlStatement query(getSysConnect(), sql.c_str());
		query.bind("name", name);
		if (query.executeStep()) {
			return query.getColumn("val").getText();;
		}
		throw QRuntimeException("000012", "sorry, has no setting,name:"  + name );
	} catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("query sys_init has error:{}, msg:{}", e.getErrorCode(), _err);
		throw QRuntimeException("000011", "sorry, system has error when we are loading settings.");
	}
}

bool SysInitRepository::has(const std::string & name)
{
	std::string sql = "SELECT * FROM sys_init WHERE name=:name";

	try {
		SQLite::QSqlStatement query(getSysConnect(), sql.c_str());
		query.bind(":name", name);
		if (query.executeStep()) {
			return true;
		}
		return false;
	} catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("query sys_init has error:{}, msg:{}", e.getErrorCode(), _err);
		throw QRuntimeException("000013", "sorry, system has error when we are loading settings.");
	}
}

void SysInitRepository::set(const std::string & name, const std::string & val)
{
	bool hasName = has(name);
	std::string sql = hasName ? "UPDATE sys_init SET val=:val WHERE name=:name;" :
		"INSERT INTO sys_init (name, val) VALUES(:name, :val);";
	
	try {
		SQLite::QSqlStatement query(getSysConnect(), sql.c_str());
		query.bind(":name", name);
		query.bind(":val", val);
		query.executeStep();
	} catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("query sys_init has error:{}, msg:{}", e.getErrorCode(), _err);
		throw QRuntimeException("000014", "sorry, system has error when we are loading settings.");
	}
}

SysInitList SysInitRepository::getAll()
{
	SysInitList result;
	std::string sql = "SELECT * FROM sys_init";
	try {
		SQLite::QSqlStatement query(getSysConnect(), sql.c_str());
		
		while (query.executeStep()) {
			SysInit item;
			item.name = query.getColumn("name").getText();
			item.val = query.getColumn("val").getText();
			result.push_back(item);
		}
		return result;
	} catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("query sys_init has error:{}, msg:{}", e.getErrorCode(), _err); 
		throw QRuntimeException("000015", "sorry, system has error when we are loading settings.");
	}
}
