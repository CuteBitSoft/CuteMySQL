#pragma once
#include "core/common/service/BaseService.h"
#include "core/repository/db/UserSqlExecutorRepository.h"
#include "core/entity/Entity.h"

class ExecutorService :   public BaseService<ExecutorService, UserSqlExecutorRepository>
{
public:
	sql::ResultSet * executeQuerySql(uint64_t connectId, const std::string & schema, const std::string &sql);

	int executeSql(uint64_t connectId, const std::string & schema,  const std::string &sql);

	const PerfTime & getPerfTime() const;
};

