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
 * @file   QDialogDelegate.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-05
 *********************************************************************/
#pragma once
#include <wx/bmpcbox.h>
#include <cassert>
#include "ui/common/delegate/QDelegate.h"
#include "ui/database/supplier/DatabaseSupplier.h"

/**
 * template T - subclass, S - subclass of QSupplier ,V - dataView class such as 
 */
template <typename TT, typename SS = EmptySupplier, typename VV = wxWindow>
class QDialogDelegate : public QDelegate<TT, SS, VV>
{
public:
	QDialogDelegate();
	
protected:
	UserConnect userConnect;
	UserDb userDb;
	UserTable userTable;

	DatabaseSupplier * databaseSupplier;
};

template <typename TT, typename SS, typename VV>
QDialogDelegate<TT, SS, VV>::QDialogDelegate()
{
	databaseSupplier = DatabaseSupplier::getInstance();
	if (databaseSupplier->runtimeUserConnect) {
		userConnect = *databaseSupplier->runtimeUserConnect;
	}

	if (databaseSupplier->runtimeUserDb) {
		userDb = *databaseSupplier->runtimeUserDb;
	}

	if (databaseSupplier->runtimeUserTable) {
		userTable = *databaseSupplier->runtimeUserTable;
	}
}


