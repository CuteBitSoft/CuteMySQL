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

 * @file   EntityUtil.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2023-10-28
 *********************************************************************/
#include "EntityUtil.h"


IndexInfo EntityUtil::copy(const IndexInfo & item)
{
	IndexInfo result;
	result.name = item.name;
	result.type = item.type;
	result.pk = item.pk;
	result.ai = item.ai;
	result.un = item.un;
	result.fk = item.fk;
	result.ck = item.ck;
	result.columns = item.columns;
	result.sql = item.sql;
	result.seq = item.seq;

	return result;
}

ResultInfo EntityUtil::copy(const ResultInfo & item)
{
	ResultInfo result;	
	result.id = item.id;
	result.createdAt = item.createdAt;
	result.effectRows = item.effectRows;
	result.code = item.code;
	result.msg = item.msg;
	result.sql = item.sql;
	result.top = item.top;
	result.execTime = item.execTime;
	result.transferTime = item.transferTime;
	result.totalTime = item.totalTime;
	return result;
}

UserDb EntityUtil::copy(const UserDb & item)
{
	UserDb result;
	result.connectId = item.connectId;
	result.name = item.name;
	result.catalog = item.catalog;
	
	return result;
}

UserConnect EntityUtil::copy(const UserConnect& item)
{
	UserConnect result;
	result.id = item.id;
	result.name = item.name;
	result.host = item.host;
	result.userName = item.userName;
	result.password = item.password;
	result.port = item.port;
	result.databases = item.databases;
	result.isUseCompressed = item.isUseCompressed;
	result.isReadOnly = item.isReadOnly;
	result.idleTimeout = item.idleTimeout;
	result.keepAlive = item.keepAlive;
	result.isSshTunnel = item.isSshTunnel;
	result.sshHost = item.sshHost;
	result.sshPort = item.sshPort;
	result.sshUserName = item.sshUserName;
	result.isSshPassword = item.isSshPassword;
	result.isSshPublickey = item.isSshPublickey;
	result.sshPassword = item.sshPassword;
	result.isSshPasswordSaved = item.isSshPasswordSaved;
	result.sshPullickeyPassphrase = item.sshPullickeyPassphrase;
	result.isSshPullickeyPassphraseSaved = item.isSshPullickeyPassphraseSaved;
	result.sshPrivatekeyFilepath = item.sshPrivatekeyFilepath;
	result.isUseSsl = item.isUseSsl;
	result.sslCaCertificate = item.sslCaCertificate;
	result.sslCipher = item.sslCipher;
	result.isSslAuth = item.isSslAuth;
	result.sslClientKey = item.sslClientKey;
	result.sslClientCertificate = item.sslClientCertificate;
	result.isUseGlobalValue = item.isUseGlobalValue;
	result.sqlMode = item.sqlMode;
	result.initCommand = item.initCommand;
	result.createdAt = item.createdAt;
	result.updatedAt = item.updatedAt;

	return result;
}


ParamElemData EntityUtil::copy(const ParamElemData & item)
{
	ParamElemData result;
	result.type = item.type;
	result.labelText = item.labelText;
	result.val = item.val;
	result.options = item.options;
	result.description = item.description;
	result.valRwType = item.valRwType;
	result.valBackType = item.valBackType;
	return result;
}

bool EntityUtil::compare(const ParamElemData & item1, const ParamElemData & item2)
{
	if (item1.val != item2.val
		|| item1.type != item2.type
		|| item1.labelText != item2.labelText
		|| item1.description != item2.description
		|| item1.valRwType != item2.valRwType
		|| item1.valBackType != item2.valBackType) {
		return false;
	}
	return true;
}

bool EntityUtil::compare(const ColumnInfo & item1, const ColumnInfo & item2)
{
	if (item1.ai != item2.ai
		|| item1.isNullable != item2.isNullable
		|| item1.ai != item2.ai
		|| item1.un != item2.un
		|| item1.size != item2.size
		|| item1.catalog != item2.catalog
		|| item1.schema != item2.schema
		|| item1.table != item2.table
		|| item1.name != item2.name
		|| item1.type != item2.type
		|| item1.defVal != item2.defVal) {
		return false;
	}
	return true;
}

bool EntityUtil::compare(const IndexInfo & item1, const IndexInfo & item2)
{
	if (item1.ai != item2.ai
		|| item1.fk != item2.fk
		|| item1.pk != item2.pk
		|| item1.un != item2.un
		|| item1.name != item2.name
		|| item1.type != item2.type
		|| item1.columns != item2.columns
		|| item1.partialClause != item2.partialClause) {
		return false;
	}
	return true;
}

bool EntityUtil::compare(const ForeignKey & item1, const ForeignKey & item2)
{
	if (item1.name != item2.name
		|| item1.type != item2.type
		|| item1.columns != item2.columns
		|| item1.referencedTable != item2.referencedTable
		|| item1.referencedColumns != item2.referencedColumns
		|| item1.onUpdate != item2.onUpdate
		|| item1.onDelete != item2.onDelete
		|| item1.partialClause != item2.partialClause) {
		return false;
	}
	return true;
}

