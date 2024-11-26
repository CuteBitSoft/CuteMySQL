#include "UserProcedureRepository.h"
#include <list>

UserProcedureList UserProcedureRepository::getAll(uint64_t connectId, const std::string& schema)
{
    UserProcedureList result;

    if (connectId <= 0 || schema.empty()) {
		return result;
	}
	
	try {
		std::list<sql::SQLString> types = {"VIEW"};
		auto connect = getUserConnect(connectId);
		auto catalog = connect->getCatalog();
		sql::ResultSet* resultSet = connect->getMetaData()->getTables(catalog, schema, "%", types);
		bool isOk = resultSet->first();
		while (isOk) {
			UserProcedure item = toUserProcedure(resultSet);
			result.push_back(item);
			isOk = resultSet->next();
		}
		resultSet->close();
		return result;
	} catch (sql::SQLException& ex) {
		auto code = std::to_string(ex.getErrorCode());
		BaseRepository::setError(code, ex.what());
		Q_ERROR("Fail to getAll(),code:{}, error:{}", code, ex.what());
		throw QRuntimeException(code, ex.what());
	}

    return result;
}

UserProcedure UserProcedureRepository::toUserProcedure(sql::ResultSet* rs)
{
	UserProcedure result;
	result.catalog = rs->getString(1).asStdString();
	result.schema = rs->getString(2).asStdString();
	result.name = rs->getString(3).asStdString();
	result.type = rs->getString(4).asStdString();
	result.comment = rs->getString(5).asStdString();
	
	return result;
}
