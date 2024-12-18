/*****************************************************************//**
 * Copyright 2023 Xuehan Qin 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and

 * limitations under the License.

 * @file   QTabPage.h
 * @brief  The base TabPage class for CTabView.
 *		   It's child class supplier to store runtime data and handle functions.
 * 
 * @author Xuehan Qin
 * @date   2023-10-30
 *********************************************************************/
#pragma once
#include <wx/aui/framemanager.h>
#include "QPageSupplier.h"
#include "ui/common/panel/QPanel.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/common/supplier/EmptySupplier.h"

template <typename T = EmptySupplier>
class QTabPage : public QPanel<T> {
public:
	QTabPage();
	DatabaseSupplier * getDataPageSupplier() { return databaseSupplier; }
protected:
	wxAuiManager* uiMgr;
	DatabaseSupplier * databaseSupplier;

	virtual void init();
	virtual void createControls();

	void createUiMgr();
};

template<typename T>
QTabPage<T>::QTabPage():QPanel<T>()
{
	init();
}

template<typename T>
void QTabPage<T>::init()
{
	databaseSupplier = DatabaseSupplier::getInstance();
	QPanel<T>::bkgColor = wxColour(30, 31, 34, 30);
}


template <typename T /*= EmptySupplier*/>
void QTabPage<T>::createControls()
{
	//createUiMgr();
}


template <typename T /*= EmptySupplier*/>
void QTabPage<T>::createUiMgr()
{
	uiMgr = new wxAuiManager(this, wxAUI_MGR_DEFAULT);
}
