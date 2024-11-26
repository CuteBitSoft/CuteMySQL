/*****************************************************************//**
 * @file   SysInitRepository.h
 * @brief  系统配置加载的类
 * @detail    $DETAIL
 * 
 * @author 覃学涵
 * @date   2023-03-02
 *********************************************************************/
#pragma once
#include "core/entity/Entity.h"
#include "core/common/repository/BaseRepository.h"

class SysInitRepository : public BaseRepository<SysInitRepository>
{
public:
	SysInitRepository() {};

	std::string get(const std::string & name);
	void set(const std::string & name, const std::string & val);
	bool has(const std::string & name);
	
	SysInitList getAll();



};

