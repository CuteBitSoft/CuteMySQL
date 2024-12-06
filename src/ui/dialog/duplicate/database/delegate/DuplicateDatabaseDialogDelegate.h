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
 * @file   DuplicateDatabaseDialogDelegate.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-30
 *********************************************************************/
#pragma once
#include <wx/treelist.h>
#include "ui/dialog/delegate/QDialogDelegate.h"
class DuplicateDatabaseDialogDelegate :  public QDialogDelegate<DuplicateDatabaseDialogDelegate>
{
public:
	void loadForTreeListCtrl(wxTreeListCtrl * treeListCtrl);
private:
	// For Database
	void loadTablesForDatabase(wxTreeListCtrl * treeListCtrl, const  wxTreeListItem & folderItem, uint64_t connectId, const std::string & schema);
	void loadViewsForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem & folderItem, uint64_t connectId, const std::string & schema);
	void loadProceduresForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem & folderItem, uint64_t connectId, const std::string & schema);
	void loadFunctionsForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem & folderItemId, uint64_t connectId, const std::string & schema);
	void loadTriggersForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem & folderItemId, uint64_t connectId, const std::string & schema);
	void loadEventsForDatabase(wxTreeListCtrl * treeListCtrl, const wxTreeListItem & folderItemId, uint64_t connectId, const std::string & schema);
};

