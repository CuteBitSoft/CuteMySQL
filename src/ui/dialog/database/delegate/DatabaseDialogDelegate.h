/*****************************************************************//**
 * Copyright 2024 Xuehan Qin (qinxuehan2018@gmail.com) 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * @file   DatabaseDialogDelegate.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-30
 *********************************************************************/
#pragma once
#include <wx/bmpcbox.h>
#include "core/service/db/ConnectService.h"
#include "core/service/db/MetadataService.h"
#include "ui/common/delegate/QDelegate.h"
#include "ui/database/supplier/DatabaseSupplier.h"
class DatabaseDialogDelegate :  public QDelegate<DatabaseDialogDelegate>
{
public:
	void loadForConnectComboBox(wxBitmapComboBox * connectComboBox);
	void loadForCharsetComboBox(wxComboBox * charsetComboBox, uint64_t connectId, const std::string & defval = "");
	void loadForCollationComboBox(wxComboBox * collationComboBox, uint64_t connectId, const std::string& charset, const std::string & defval = "");
private:
	DatabaseSupplier * databaseSupplier = DatabaseSupplier::getInstance();
	ConnectService* connectService = ConnectService::getInstance();
	MetadataService* metadataService = MetadataService::getInstance();
};

