/*****************************************************************//**
 * Copyright 2023 Xuehan Qin 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and

 * limitations under the License.

 * @file   BaseUserRepository.h
 * @brief  BaseUserRepository is a repository class of user database table handle 
 * 
 * @author Xuehan Qin
 * @date   2023-05-20
 *********************************************************************/
#pragma once
#include "core/common/repository/BaseRepository.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <cassert>

#include <mysql/jdbc.h>
#include <boost/scoped_ptr.hpp>

#include "core/entity/Entity.h"
#include "utils/FileUtil.h"
#include "core/common/repository/QConnect.h"

template <typename T>
class BaseUserRepository : public BaseRepository<T>
{
public:
	~BaseUserRepository();

	sql::Connection * getUserConnect(uint64_t userConnectId);
	void testUserConnect(uint64_t userConnectId);	
	void closeUserConnect(uint64_t userConnectId);	
	void closeAllUserConnect();	
protected:
	
	UserConnect getUserConnectEntity(uint64_t userConnectId);
	UserConnect toUserConnect(SQLite::QSqlStatement& query);

	RowItem toRowItem(sql::Statement* query);
};

template <typename T>
RowItem BaseUserRepository<T>::toRowItem(sql::Statement * query)
{
	RowItem rowItem;
	
	return rowItem;
}

template<typename T>
BaseUserRepository<T>::~BaseUserRepository()
{
	closeAllUserConnect();
}

/**
* Read dbName and dbPath from the table CuteSqlite.user_db
*
* @param userDbId The param of CuteSqlite.user_db.id
* @return QSqlDatabase pointer
*/
template <typename T>
sql::Connection * BaseUserRepository<T>::getUserConnect(uint64_t userConnectId)
{
	if (QConnect::userConnectPool.empty() && QConnect::driver == nullptr) {
		QConnect::driver = sql::mysql::get_mysql_driver_instance();
	}

	if (QConnect::userConnectPool.find(userConnectId) == QConnect::userConnectPool.end()) {
		UserConnect userConnEntity = getUserConnectEntity(userConnectId);

		sql::ConnectOptionsMap options;
		options["hostName"] = userConnEntity.host;
		options["userName"] = userConnEntity.userName;
		options["password"] = userConnEntity.password;
		if (!userConnEntity.databases.empty()) {
			options["schema"] = userConnEntity.databases;
		}

		options["port"] = userConnEntity.port;
		options["OPT_RECONNECT"] = true;

		try {
			//QConnect::userConnectPool[userConnectId] = QConnect::driver->connect("tcp://127.0.0.1:3306", "root", "qinziyi");
			QConnect::userConnectPool[userConnectId] = QConnect::driver->connect(options);
		} catch (sql::SQLException& ex) {
			BaseRepository<T>::setError(std::to_string(ex.getErrorCode()), ex.what());
			Q_ERROR("Fail to connect the mysql. hostName:{}, port:{},error:{}", userConnEntity.host, ex.what());
			throw QRuntimeException(std::to_string(ex.getErrorCode()), ex.what());
		}
		
	}

	Q_INFO(L"BaseUserRepository::getConnect(connectId), connectId:{} connect...", userConnectId);
	Q_INFO(L"BaseUserRepository::getConnect(connectId), connectId:{}, connect.isValid():{}",
		userConnectId, QConnect::userConnectPool[userConnectId]->isValid());

	if (!QConnect::userConnectPool[userConnectId]->isValid()) {
		try {
			if (!QConnect::userConnectPool[userConnectId]->reconnect()) {
				throw(sql::SQLException("Fail to reconnect the mysql."));
			}
		} catch (sql::SQLException& ex) {
			BaseRepository<T>::setError(std::to_string(ex.getErrorCode()), ex.what());
			Q_ERROR("Open mysql db raise error. connectId:{}, error:{}", userConnectId, ex.what());
			throw QRuntimeException(std::to_string(ex.getErrorCode()), ex.what());
		}
		
	}

	return QConnect::userConnectPool[userConnectId];
}

template <typename T>
void BaseUserRepository<T>::testUserConnect(uint64_t userConnectId)
{
	if (QConnect::userConnectPool.empty() && QConnect::driver == nullptr) {
		QConnect::driver = sql::mysql::get_mysql_driver_instance();
	}

	
	UserConnect userConnEntity = getUserConnectEntity(userConnectId);

	sql::ConnectOptionsMap options;
	options["hostName"] = userConnEntity.host;
	options["userName"] = userConnEntity.userName;
	options["password"] = userConnEntity.password;
	options["port"] = userConnEntity.port;
	options["OPT_RECONNECT"] = true;
	if (!userConnEntity.databases.empty()) {
		options["schema"] = userConnEntity.databases;
	}

	try {
		boost::scoped_ptr<sql::Connection> conn(QConnect::driver->connect(options));
		
		if (conn.get()->isValid()) {
			conn.get()->close();
		}
		/*
		if (conn) {
			delete conn;
		}*/
	} catch (sql::SQLException& ex) {
		BaseRepository<T>::setError(std::to_string(ex.getErrorCode()), ex.what());
		Q_ERROR("Fail to connect the mysql. hostName:{}, port:{},error:{}", userConnEntity.host, ex.what());
		throw QRuntimeException(std::to_string(ex.getErrorCode()), ex.what());
	}
}

template <typename T>
void BaseUserRepository<T>::closeUserConnect(uint64_t userConnectId)
{
	if (QConnect::userConnectPool.empty() || QConnect::userConnectPool.find(userConnectId) == QConnect::userConnectPool.end()) {
		return;
	}
	
	auto tmpConnect = QConnect::userConnectPool.at(userConnectId);	
	if (tmpConnect) {
		// 1) close the connect
		if (tmpConnect->isValid()) {
			tmpConnect->close();
		}
		// 2) delete the ptr
		delete tmpConnect;
		tmpConnect = nullptr;
	}
	// 3) erase from userConnectPool (map)
	QConnect::userConnectPool.erase(userConnectId);
}

template <typename T>
void BaseUserRepository<T>::closeAllUserConnect()
{
	if (QConnect::userConnectPool.empty()) {
		return;
	}

	for (auto pair : QConnect::userConnectPool) {
		auto tmpConnect = pair.second;
		// 1) close the connect
		if (tmpConnect->isValid()) {
			tmpConnect->close();
		}

		// 2) delete the ptr
		delete tmpConnect;
		tmpConnect = nullptr;
	}

	// 3) erase all item from userConnectPool (map)
	QConnect::userConnectPool.clear();
}

template <typename T>
UserConnect BaseUserRepository<T>::getUserConnectEntity(uint64_t userConnectId)
{
	if (userConnectId <= 0) {
		return UserConnect();
	}

	std::string sql = "SELECT * FROM user_connect WHERE id=:id";

	try {
		SQLite::QSqlStatement query(BaseRepository<T>::getSysConnect(), sql.c_str());
		query.bind(":id", userConnectId);

		//Ö´ÐÐÓï¾ä
		if (!query.executeStep()) {
			return UserConnect();
		}

		Q_INFO("Get user_connect detail success");
		UserConnect item = toUserConnect(query);
		return item;
	}
	catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("exec sql has error, code:{}, msg:{}, sql:{}", e.getErrorCode(), _err, sql);
		throw QRuntimeException("10022", "sorry, system has error.");
	}
}

template <typename T>
UserConnect BaseUserRepository<T>::toUserConnect(SQLite::QSqlStatement & query)
{
	UserConnect item;
	item.id = query.getColumn("id").getInt64();
	item.name = query.getColumn("name").getText();
	item.host = query.getColumn("host").getText();
	item.userName = query.getColumn("user_name").getText();
	item.password = query.getColumn("password").getText();
	item.isPasswordSaved = query.getColumn("is_password_saved").getInt();
	item.port = query.getColumn("port").getInt();
	item.databases = query.getColumn("databases").getText();
	item.isUseCompressed = query.getColumn("is_use_compressed").getInt();
	item.isReadOnly = query.getColumn("is_read_only").getInt();
	item.idleTimeout = query.getColumn("idle_timeout").getInt();
	item.keepAlive = query.getColumn("keep_alive").getInt();
	item.isSshTunnel = query.getColumn("is_ssh_tunnel").getInt();
	item.sshHost = query.getColumn("ssh_host").getText();
	item.sshPort = query.getColumn("ssh_port").getInt();
	item.sshUserName = query.getColumn("ssh_user_name").getText();
	item.isSshPassword = query.getColumn("is_ssh_password").getInt();
	item.isSshPublickey = query.getColumn("is_ssh_publickey").getInt();
	item.sshPassword = query.getColumn("ssh_password").getText();
	item.isSshPasswordSaved = query.getColumn("is_ssh_password_saved").getInt();
	item.sshPullickeyPassphrase = query.getColumn("ssh_pullickey_passphrase").getText();
	item.isSshPullickeyPassphraseSaved = query.getColumn("is_ssh_pullickey_passphrase_saved").getInt();
	item.sshPrivatekeyFilepath = query.getColumn("ssh_privatekey_filepath").getText();
	item.isUseSsl = query.getColumn("is_use_ssl").getInt();
	item.sslCaCertificate = query.getColumn("ssl_ca_certificate").getText();
	item.sslCipher = query.getColumn("ssl_cipher").getText();
	item.isSslAuth = query.getColumn("is_ssl_auth").getInt();
	item.sslClientKey = query.getColumn("ssl_client_key").getText();
	item.sslClientCertificate = query.getColumn("ssl_client_certificate").getText();
	item.isUseGlobalValue = query.getColumn("is_use_global_value").getInt();
	item.sqlMode = query.getColumn("sql_mode").getText();
	item.initCommand = query.getColumn("init_command").getText();
	item.sort = query.getColumn("sort").getInt64();
	item.createdAt = query.getColumn("created_at").getText();
	item.updatedAt = query.getColumn("updated_at").getText();
	return item;
}