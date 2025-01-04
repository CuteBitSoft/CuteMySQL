/*****************************************************************//**
 * Copyright 2025 Xuehan Qin (qinxuehan2018@gmail.com) 
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
 * @file   ObjectsPageTableDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2025-01-03
 *********************************************************************/
#include "ObjectsPageTableDelegate.h"
#include <cassert>
#include "utils/StringUtil.h"

/**
 * Only load object for the ListView.
 * 
 */
void ObjectsPageTableDelegate::loadObjects()
{
	// copy runtime data
	refreshSupplier();
	view->DeleteAllItems();
	UserTableList tables;
	try {
		tables = supplier->getUserTables();
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	int i = 0;
	for (auto& table : tables) {
		wxListItem item;
		item.SetId(i);
		item.SetText(table.name);
		item.SetBackgroundColour(i % 2 ? rowBkgColor1 : rowBkgColor2);
		item.SetTextColour(textColor);
		item.SetImage(0);
		item.SetAlign(wxLIST_FORMAT_LEFT); 
		long row = view->InsertItem(item);
		view->SetItem(row, 1, std::to_string(table.autoIncVal)); // auto-inc-val
		view->SetItem(row, 2, table.createTime); // create-time
		view->SetItem(row, 3, std::to_string(table.dataLength)); // data-length
		view->SetItem(row, 4, table.engine); // engine
		view->SetItem(row, 5, std::to_string(table.rows)); // rows
		view->SetItem(row, 6, table.comment); // comment
		i++;
	}
}

void ObjectsPageTableDelegate::createMenu()
{

}
