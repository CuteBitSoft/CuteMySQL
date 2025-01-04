#include "ExecutorService.h"

sql::ResultSet * ExecutorService::executeQuerySql(uint64_t connectId, const std::string& schema, const std::string& sql)
{
     return getRepository()->executeQuery(connectId, schema, sql);
}

int ExecutorService::executeSql(uint64_t connectId, const std::string& schema, const std::string& sql)
{
    return getRepository()->execute(connectId, schema, sql);
}

const PerfTime& ExecutorService::getPerfTime() const
{
     return getRepository()->getPerfTime();
}


