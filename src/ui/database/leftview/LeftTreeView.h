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
 * @file   LeftTreeView.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-21
 *********************************************************************/
#pragma once
#include <wx/treectrl.h>
#include <wx/bmpcbox.h>

#include "common/event/MsgDispatcherEvent.h"
#include "ui/common/panel/QPanel.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/database/leftview/delegate/LeftTreeDelegate.h"
#include "ui/database/leftview/delegate/LeftTopbarDelegate.h"

class LeftTreeView : public QPanel<DatabaseSupplier>
{
	DECLARE_EVENT_TABLE()
public:
	LeftTreeView();
	~LeftTreeView();
private:

	wxBitmapButton* connectButton;
	wxBitmapButton* newButton;
	wxBitmapButton* delButton;
	wxBitmapButton* dupButton;
	wxBitmapButton* refreshButton;
	wxBitmapComboBox* selectedDbComboBox;

	wxBoxSizer* hLayout; // top box sizer
	wxBoxSizer* tLayout; // top box sizer

	wxTreeCtrl * treeView;

	LeftTreeDelegate* leftTreeDelegate;
	LeftTopbarDelegate* leftTopbarDelegate;
	virtual void init();
	virtual void createControls();
	void createButtons();
	void createComboBox();
	void createTreeView();

	void resizeComboBox();
	void resizeTreeView();

	void loadTreeView(wxTreeCtrl* treeView);

	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnTreeItemExpended(wxTreeEvent& event);
	void OnTreeItemSelChanged(wxTreeEvent& event);
	// handle message event
	void OnHandleConnectionConnected(MsgDispatcherEvent& event);
	void OnHandleAddDatabase(MsgDispatcherEvent& event);
	void OnHandleNewTable(MsgDispatcherEvent& event);

	// button id
	void OnClickConnectButton(wxCommandEvent & event);
	void OnClickCreateButton(wxCommandEvent & event);
	void OnClickDeleteButton(wxCommandEvent & event);
	void OnClickDuplicateButton(wxCommandEvent & event);

	// combobox
	void OnSelectedDbCombobox(wxCommandEvent & event);
};

