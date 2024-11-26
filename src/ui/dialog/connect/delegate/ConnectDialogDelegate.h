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
 * @file   ConnectDialogDelegate.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-13
 *********************************************************************/
#pragma once
#include <wx/treectrl.h>

#include "ui/common/delegate/QDelegate.h"
#include "core/service/db/ConnectService.h"
#include "ui/dialog/connect/supplier/ConnectDialogSupplier.h"
#include "ui/dialog/connect/panel/ConnectPanel.h"

class ConnectDialogDelegate : public QDelegate<ConnectDialogDelegate, ConnectDialogSupplier>
{
public:
	
	void loadTreeCtrl(wxTreeCtrl * tree);
	void createForTreeCtrl(wxTreeCtrl * leftTree, const std::string & defaultName = std::string(""));
	void removeForTreeCtrl(wxTreeCtrl * leftTree);
	void duplicateForTreeCtrl(wxTreeCtrl * leftTree);
	void moveUpForTreeCtrl(wxTreeCtrl * leftTree);
	void moveDownForTreeCtrl(wxTreeCtrl * leftTree);
	void exchangeDataFromRightPanel(ConnectPanel* rightPanel);
	void changeSelItemForTreeCtrl(wxTreeCtrl* leftTree, wxTreeItemId selItemId);
	void changeDataForRightPanel(ConnectPanel* rightPanel);
	void clearDataForRightPanel(ConnectPanel* rightPanel);
	void disableForRightPanel(ConnectPanel* rightPanel);
	void enableForRightPanel(ConnectPanel* rightPanel);

	void testConnect();
	bool connect();
private:
	ConnectService * connectService = ConnectService::getInstance();

	uint64_t saveUserConnect();

	void swapTreeCtrlItem(wxTreeCtrl* leftTree, wxTreeItemId & selItemId, wxTreeItemId & swapItemId);
	uint64_t getMaxSortFromTreeCtrl(wxTreeCtrl* leftTree);
	void saveTreeCtrlItemData(wxTreeCtrl* leftTree, wxTreeItemId& selItemId);
};

