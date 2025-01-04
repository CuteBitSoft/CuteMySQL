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
 * @file   ObjectsPageViewDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2025-01-03
 *********************************************************************/
#include "ObjectsPageViewDelegate.h"
#include <cassert>
#include "utils/StringUtil.h"

/**
 * Only load objects for the ListView.
 * 
 */
void ObjectsPageViewDelegate::loadObjects()
{
	// copy runtime data
	refreshSupplier();
	view->DeleteAllItems();
	UserViewList objects;
	try {
		objects = supplier->getUserViews();
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
	}
	int i = 0;
	for (auto& objectItem : objects) {
		wxListItem item;
		item.SetId(i);
		item.SetText(objectItem.name);
		item.SetBackgroundColour(i % 2 ? rowBkgColor1 : rowBkgColor2);
		item.SetTextColour(textColor);
		item.SetImage(1);
		item.SetAlign(wxLIST_FORMAT_LEFT);
		long row = view->InsertItem(item);
		view->SetItem(row, 1, objectItem.rowFormat == "Fixed" ? S("orig-yes") : S("orig-no")); // allow-alter
		view->SetItem(row, 2, objectItem.createTime);
		view->SetItem(row, 3, objectItem.updateTime);
		i++;
	}
}

void ObjectsPageViewDelegate::createMenu()
{

}
