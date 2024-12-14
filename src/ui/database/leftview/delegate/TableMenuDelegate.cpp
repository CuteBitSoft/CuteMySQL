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
 * @file   TableMenuDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-28
 *********************************************************************/
#include "TableMenuDelegate.h"
#include <cassert>
#include <wx/msgdlg.h>
#include "ui/common/data/QClientData.h"
#include "ui/common/msgbox/QAnimateBox.h"
#include "core/common/Lang.h"

TableMenuDelegate::~TableMenuDelegate()
{
	DatabaseService::destroyInstance();
	databaseService = nullptr;

	delete menu;
	menu = nullptr;
}

void TableMenuDelegate::setTreeView(wxTreeCtrl* treeView)
{
	this->treeView = treeView;
}

void TableMenuDelegate::createMenu()
{
	if (menu) {
		return;
	}
	// main menu
	menu = new QMenu();

	auto imgdir = ResourceUtil::getProductImagesDir();

	wxMenuItem* menuItem = new wxMenuItem(0, Config::TABLE_OPEN_MENU_ID, S("table-open"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap1(imgdir + "/database/menu/open-table.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap1));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::TABLE_CREATE_MENU_ID, S("table-create"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap2(imgdir + "/database/menu/create-table.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap2));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::TABLE_ALTER_MENU_ID, S("table-alter"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap3(imgdir + "/database/menu/alter-table.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap3));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::TABLE_RENAME_MENU_ID, S("table-rename"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap4(imgdir + "/database/menu/rename-table.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap4));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::TABLE_TRUNCATE_MENU_ID, S("table-truncate"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap5(imgdir + "/database/menu/truncate-table.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap5));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::TABLE_TRUNCATE_MENU_ID, S("table-drop"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap6(imgdir + "/database/menu/drop-table.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap6));
	menu->Append(menuItem);

	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menuItem = new wxMenuItem(0, Config::TABLE_COPY_MENU_ID, S("table-copy-as"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap7(imgdir + "/database/menu/copy.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap7));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::TABLE_SHARDING_MENU_ID, S("table-sharding-as"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap8(imgdir + "/database/menu/sharding.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap8));
	menu->Append(menuItem);

	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menuItem = new wxMenuItem(0, Config::TABLE_EXPORT_MENU_ID, S("table-export"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap9(imgdir + "/database/menu/export-as-sql.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap9));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::TABLE_IMPORT_SQL_MENU_ID, S("table-import-sql"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap10(imgdir + "/database/menu/import-from-sql.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap10));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::TABLE_IMPORT_CSV_MENU_ID, S("table-import-csv"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap11(imgdir + "/database/menu/import-from-csv.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap11));
	menu->Append(menuItem);

	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menuItem = new wxMenuItem(0, Config::TABLE_MANAGE_INDEX_MENU_ID, S("table-manage-index"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap12(imgdir + "/database/menu/manage-index.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap12));
	menu->Append(menuItem);

	menuItem = new wxMenuItem(0, Config::TABLE_MANAGE_FOREIGNKEY_MENU_ID, S("table-manage-foreign-key"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap13(imgdir + "/database/menu/manage-index.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap13));
	menu->Append(menuItem);

	menu->AppendSeparator()->SetBackgroundColour(bkgColor);

	menuItem = new wxMenuItem(0, Config::TABLE_PROPERTIES_MENU_ID, S("properties"));
	menuItem->SetBackgroundColour(bkgColor);
	menuItem->SetTextColour(textColor);
	wxBitmap bitmap14(imgdir + "/database/menu/properties.ico", wxBITMAP_TYPE_ICO);
	menuItem->SetBitmap(wxBitmapBundle(bitmap14));
	menu->Append(menuItem);

}
	

void TableMenuDelegate::popMenu(int x, int y)
{
	treeView->PopupMenu(menu, x, y);
}
