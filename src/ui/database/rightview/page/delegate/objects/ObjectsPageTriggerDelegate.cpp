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
 * @file   ObjectsPageTriggerDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2025-01-03
 *********************************************************************/
#include "ObjectsPageTriggerDelegate.h"
#include <cassert>
#include "utils/StringUtil.h"

/**
 * Only load object for the ListView.
 * 
 */
void ObjectsPageTriggerDelegate::loadObjects()
{
	// copy runtime data
	refreshSupplier();
	view->DeleteAllItems();
	UserTriggerList objects;
	try {
		objects = supplier->getUserTriggers();
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	int i = 0;
	for (auto& object : objects) {
		wxListItem item;
		item.SetId(i);
		item.SetText(object.name);
		item.SetBackgroundColour(i % 2 ? rowBkgColor1 : rowBkgColor2);
		item.SetTextColour(textColor);
		item.SetImage(4);
		item.SetAlign(wxLIST_FORMAT_LEFT); 
		long row = view->InsertItem(item);
		view->SetItem(row, 1, object.actionSchema); 
		view->SetItem(row, 2, object.actionTable); 
		view->SetItem(row, 3, object.actionTiming); 
		view->SetItem(row, 4, object.createTime); 
		view->SetItem(row, 5, object.updateTime); 
		i++;
	}
}

void ObjectsPageTriggerDelegate::createMenu()
{

}
