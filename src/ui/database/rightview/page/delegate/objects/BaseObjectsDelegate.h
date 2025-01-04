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
 * @file   BaseObjectsDelegate.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2025-01-03
 *********************************************************************/
#pragma once
#include <wx/statusbr.h>
#include "ui/common/delegate/QDelegate.h"
#include "ui/database/rightview/page/supplier/ObjectsPageSupplier.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/common/listview/QListView.h"
#include "utils/ResourceUtil.h"

template <typename T>
class BaseObjectsDelegate : public QDelegate<T, ObjectsPageSupplier, QListView>
{
public:
	BaseObjectsDelegate();
	~BaseObjectsDelegate();

	void loadListView();
	void loadStatusBar(wxStatusBar * statusBar);
	virtual void loadObjects() = 0;
	void popupMenu();
	void refreshSupplier();
protected:
	wxString imgdir;
	wxColour rowBkgColor1, rowBkgColor2, textColor;
	DatabaseSupplier* databaseSupplier = DatabaseSupplier::getInstance();

	// list view
	void createListHeader();
	

	// right click menu
	virtual void createMenu() = 0;
	
};

template <typename T>
BaseObjectsDelegate<T>::BaseObjectsDelegate()
	: rowBkgColor1(30, 31, 34, 30), rowBkgColor2(46, 46, 46, 43), textColor(223, 225, 229, 213)
{
	
}

template<typename T>
inline BaseObjectsDelegate<T>::~BaseObjectsDelegate()
{
	
}

template <typename T>
void BaseObjectsDelegate<T>::loadListView()
{
	createListHeader();
	loadObjects();
}

template<typename T>
void BaseObjectsDelegate<T>::loadStatusBar(wxStatusBar * statusBar)
{
	UserConnect userConnect = this->supplier->getUserConnect();
	wxString connectName = wxString::Format("%s[%s@%s:%d]", 
		userConnect.name, userConnect.userName, userConnect.host, userConnect.port);
	
	wxString strRow, unit;
	switch (this->supplier->runtimeObjectType)
	{
	case TABLE_OBJECTS:
		unit = S("tables");
		break;
	case VIEW_OBJECTS:
		unit = S("views");
		break;
	case PROCEDURE_OBJECTS:
		unit = S("store-procedures");
		break;
	case FUNCTION_OBJECTS:
		unit = S("functions");
		break;
	case TRIGGER_OBJECTS:
		unit = S("triggers");
		break;
	case EVENT_OBJECTS:
		unit = S("events");
		break;
	default:
		break;
	}
	strRow = wxString::Format("%d %s", this->view->GetItemCount(), unit);
	statusBar->SetStatusText(strRow, 0);
	statusBar->SetStatusText(connectName, 1);
	statusBar->SetStatusText(this->supplier->getRuntimeSchema(), 2);
}

template <typename T>
void BaseObjectsDelegate<T>::popupMenu()
{
	createMenu();
}

template<typename T>
void BaseObjectsDelegate<T>::createListHeader()
{
	assert(this->view);
	this->view->DeleteAllColumns();

	const auto & columns = this->supplier->getColumns();
	int colIdx = 0;
	for (const auto& pair : columns) {
		wxListItem item;
		item.SetId(colIdx);
		item.SetText(pair.first);
		item.SetWidth(pair.second);
		item.SetBackgroundColour(rowBkgColor1);
		item.SetTextColour(textColor);
		this->view->InsertColumn(colIdx, item);
		colIdx++;
	}
}

/**
 * copy the runtime data from databaseSupplier to this supplier.
 * 
 */
template <typename T>
void BaseObjectsDelegate<T>::refreshSupplier()
{
	this->supplier->setRuntimeUserConnectId(databaseSupplier->getRuntimeUserConnectId());
	this->supplier->setRuntimeSchema(databaseSupplier->getRuntimeSchema());
	this->supplier->setRuntimeTblName(databaseSupplier->getRuntimeTblName());
}

