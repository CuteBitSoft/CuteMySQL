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

 * @file   DbRepository.cpp
 * @brief  The table db repository
 * 
 * @author Xuehan Qin
 * @date   2023-05-19
 *********************************************************************/
#include "UserConnectRepository.h"
#include "core/common/exception/QRuntimeException.h"
#include "core/common/driver/sqlite/QSqlColumn.h"

uint64_t UserConnectRepository::create(const UserConnect & item)
{
	//sql
	std::string sql = "INSERT INTO user_connect (name, host, user_name, password, is_password_saved, port, databases, is_use_compressed, is_read_only, idle_timeout, keep_alive, is_ssh_tunnel, ssh_host, \
ssh_port, ssh_user_name, is_ssh_password, is_ssh_publickey, ssh_password, is_ssh_password_saved, ssh_pullickey_passphrase, is_ssh_pullickey_passphrase_saved,\
ssh_privatekey_filepath, is_use_ssl, ssl_ca_certificate, ssl_cipher, is_ssl_auth, ssl_client_key, ssl_client_certificate, is_use_global_value, sql_mode, init_command, \
sort, created_at, updated_at) \
VALUES (:name, :host, :user_name, :password, :is_password_saved, :port, :databases, :is_use_compressed, :is_read_only, :idle_timeout, :keep_alive, :is_ssh_tunnel, :ssh_host, :ssh_port, \
:ssh_user_name, :is_ssh_password, :is_ssh_publickey, :ssh_password, :is_ssh_password_saved, :ssh_pullickey_passphrase, :is_ssh_pullickey_passphrase_saved, \
:ssh_privatekey_filepath, :is_use_ssl, :ssl_ca_certificate, :ssl_cipher, :is_ssl_auth, :ssl_client_key, :ssl_client_certificate, :is_use_global_value, :sql_mode, \
:init_command, :sort, datetime(), datetime())";

	try {
		SQLite::QSqlStatement query(getSysConnect(), sql.c_str());
		queryBind(query, item);

		query.exec();
		Q_INFO("create user_connect success.");
		return getSysConnect()->getLastInsertRowid();
	}
	catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("exec sql has error, code:{}, msg:{}, sql:{}", e.getErrorCode(), _err, sql);
		throw QRuntimeException("10020", "sorry, system has error.");
	}
}

void UserConnectRepository::update(const UserConnect& item)
{
	std::string sql = "UPDATE user_connect SET name=:name, host=:host, user_name=:user_name, password=:password, is_password_saved=:is_password_saved, \
port=:port, databases=:databases, is_use_compressed=:is_use_compressed, is_read_only=:is_read_only, idle_timeout=:idle_timeout,keep_alive=:keep_alive, \
is_ssh_tunnel=:is_ssh_tunnel, ssh_host=:ssh_host, ssh_port=:ssh_port, ssh_user_name=:ssh_user_name, is_ssh_password=:is_ssh_password, \
is_ssh_publickey=:is_ssh_publickey, ssh_password=:ssh_password, is_ssh_password_saved=:is_ssh_password_saved, ssh_pullickey_passphrase=:ssh_pullickey_passphrase, \
is_ssh_pullickey_passphrase_saved=:is_ssh_pullickey_passphrase_saved, ssh_privatekey_filepath=:ssh_privatekey_filepath, is_use_ssl=:is_use_ssl, \
ssl_ca_certificate=:ssl_ca_certificate, ssl_cipher=:ssl_cipher, is_ssl_auth=:is_ssl_auth, ssl_client_key=:ssl_client_key, ssl_client_certificate=:ssl_client_certificate, \
is_use_global_value=:is_use_global_value, sql_mode=:sql_mode, init_command=:init_command, sort=:sort, updated_at=datetime() \
WHERE id=:id ";

	try {
		SQLite::QSqlStatement query(getSysConnect(), sql.c_str());
		queryBind(query, item, true);

		query.exec();
		Q_INFO("update user_connect success.");
	}
	catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("exec sql has error, code:{}, msg:{}, sql:{}", e.getErrorCode(), _err, sql);
		throw QRuntimeException("10020", "sorry, system has error.");
	}
}

void UserConnectRepository::updateSort(uint64_t id, uint64_t sort)
{
	std::string sql = "UPDATE user_connect SET sort=:sort WHERE id=:id";
	try {
		SQLite::QSqlStatement query(getSysConnect(), sql.c_str());
		query.bind(":sort", sort);
		query.bind(":id", id);

		query.exec();
		Q_INFO("update user_connect success.");
	}
	catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("exec sql has error, code:{}, msg:{}, sql:{}", e.getErrorCode(), _err, sql);
		throw QRuntimeException("10020", "sorry, system has error.");
	}
}


bool UserConnectRepository::remove(uint64_t id)
{
	if (id <= 0) {
		return false;
	}
	//sql
	std::string sql = "DELETE FROM user_connect WHERE id=:id ";
	try {
		SQLite::QSqlStatement query(getSysConnect(), sql.c_str());
		query.bind(":id", id);

		query.exec();
		Q_INFO("delete user_connect success, id:{}.", id);
		return true;
	}
	catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("exec sql has error, code:{}, msg:{}, sql:{}", e.getErrorCode(), _err, sql);
		throw QRuntimeException("10021", "sorry, system has error.");
	}
}

UserConnect UserConnectRepository::getById(uint64_t id)
{
	return getUserConnectEntity(id);
}

UserConnectList UserConnectRepository::getAll()
{
	UserConnectList result;
	std::string sql = "SELECT * FROM user_connect ORDER BY sort ASC";
	try {
		SQLite::QSqlStatement query(getSysConnect(), sql.c_str());

		while (query.executeStep()) {
			UserConnect item = toUserConnect(query);
			result.push_back(item);
		}
		return result;
	}
	catch (SQLite::QSqlException &e) {
		std::string _err = e.getErrorStr();
		Q_ERROR("query db has error:{}, msg:{}", e.getErrorCode(), _err);
		throw QRuntimeException("000024", "sorry, system has error when loading databases.");
	}
}

/**
* bind the fields
*
* @param query
* @param item
*/
void UserConnectRepository::queryBind(SQLite::QSqlStatement &query, const UserConnect &item, bool isUpdate)
{
	if (isUpdate) {
		query.bind(":id", item.id);
	}
	query.bind(":name", item.name);
	query.bind(":host", item.host);
	query.bind(":user_name", item.userName);
	query.bind(":password", item.password);
	query.bind(":is_password_saved", item.isPasswordSaved);
	query.bind(":port", item.port);
	query.bind(":databases", item.databases);
	query.bind(":is_use_compressed", item.isUseCompressed);
	query.bind(":is_read_only", item.isReadOnly);
	query.bind(":idle_timeout", item.idleTimeout);
	query.bind(":keep_alive", item.keepAlive);
	query.bind(":is_ssh_tunnel", item.isSshTunnel);
	query.bind(":ssh_host", item.sshHost);
	query.bind(":ssh_port", item.sshPort);
	query.bind(":ssh_user_name", item.sshUserName);
	query.bind(":is_ssh_password", item.isSshPassword);
	query.bind(":is_ssh_publickey", item.isSshPublickey);
	query.bind(":ssh_password", item.sshPassword);
	query.bind(":is_ssh_password_saved", item.isSshPasswordSaved);
	query.bind(":ssh_pullickey_passphrase", item.sshPullickeyPassphrase);
	query.bind(":is_ssh_pullickey_passphrase_saved", item.isSshPullickeyPassphraseSaved);
	query.bind(":ssh_privatekey_filepath", item.sshPrivatekeyFilepath);
	query.bind(":is_use_ssl", item.isUseSsl);
	query.bind(":ssl_ca_certificate", item.sslCaCertificate);
	query.bind(":ssl_cipher", item.sslCipher);
	query.bind(":is_ssl_auth", item.isSslAuth);
	query.bind(":ssl_client_key", item.sslClientKey);
	query.bind(":ssl_client_certificate", item.sslClientCertificate);
	query.bind(":is_use_global_value", item.isUseGlobalValue);
	query.bind(":sql_mode", item.sqlMode);
	query.bind(":init_command", item.initCommand);
	query.bind(":sort", item.sort);
}
