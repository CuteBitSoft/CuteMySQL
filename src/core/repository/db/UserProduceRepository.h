#pragma once
#include "core/common/repository/BaseUserRepository.h"
#include "core/entity/Entity.h"
class UserProcedureRepository : public BaseUserRepository<UserProcedureRepository>
{
public:
	UserProcedureList getAll(uint64_t connectId, const std::string& schema);
private:
	UserProcedure toUserProcedure(sql::ResultSet* rs);
};

