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
 * @file   DuplicateDatabaseDialog.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-05
 *********************************************************************/

#pragma once
#include <wx/treelist.h>
#include "ui/common/dialog/QFormDialog.h"
#include "core/entity/Enum.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "core/service/db/ConnectService.h"
#include "ui/dialog/duplicate/database/delegate/DuplicateDatabaseDialogDelegate.h"
#include "ui/common/progress/QProgressBar.h"
#include "ui/common/process/QProcess.h"

class DuplicateDatabaseDialog :  public QFormDialog<DuplicateDatabaseDialogDelegate>
{
	DECLARE_EVENT_TABLE()
public:
	DuplicateDatabaseDialog();
	~DuplicateDatabaseDialog();

	// handle export process and import process
	void OnAsyncProcessTermination(QProcess<DuplicateDatabaseDialog>* process);
private:
	UserConnect userConnect ;
	UserDb userDb;

	// top
	wxBoxSizer* topHoriLayout;
	// center1
	wxBoxSizer* center1HoriLayout;
	wxBoxSizer* center1LeftVertLayout;
	wxBoxSizer* center1RightVertLayout;
	// center2
	wxBoxSizer* center2HoriLayout;
	wxStaticBoxSizer* center2LeftVertLayout;
	wxBoxSizer* center2RightVertLayout;
	wxStaticBoxSizer* dulicateSettingsVertLayout;
	wxStaticBoxSizer* lockSettingsVertLayout;

	// bottom
	wxBoxSizer* bottomHoriLayout;

	// description for dialog - in top layout
	wxStaticBitmap* image;
	wxStaticText* label;
	
	//source controls - in center1 left layout
	wxTextCtrl* sourceConnectEdit;
	wxTextCtrl* sourceDatabaseEdit;

	//target controls - in center1 right layout
	wxBitmapComboBox* targetConnectComboBox;
	wxTextCtrl* targetDatabaseEdit;

	// source objects
	wxTreeListCtrl * treeListCtrl;
	wxButton* selectAllButton;
	wxButton* unSelectAllButton;

	// duplicate settings
	wxCheckBox* structOnlyCheckBox;
	wxCheckBox* structAndDataCheckBox;

	// lock settings
	wxCheckBox* lockTablesCheckBox;

	std::string tmpSqlPath;

	// process bar 
	QProgressBar* progressbar;
	QProcess<DuplicateDatabaseDialog>* exportProcess;
	QProcess<DuplicateDatabaseDialog>* importProcess;

	DatabaseSupplier* databaseSupplier;
	DatabaseService* databaseService = DatabaseService::getInstance();
	ConnectService* connectService = ConnectService::getInstance();

	virtual void createInputs();
	virtual void createTopControls();
	virtual void createCenter1Inputs();
	virtual void createCenter2Inputs();
	virtual void createBottomInputs();
	virtual void loadControls();

	// combobox
	void OnSelChangeConnectCombobox(wxCommandEvent& event);
	void OnClickOkButton(wxCommandEvent& event);
	void OnClickSelectAllButton(wxCommandEvent& event);
	void OnClickUnSelectAllButton(wxCommandEvent& event);
	void OnStructAndDataCheckBoxChecked(wxCommandEvent& event);
	void OnTreeListItemChecked(wxTreeListEvent& event);

	// export
	bool exportDatabaseToTmp(const std::string & tmpSqlPath);
	// replace database name
	bool replaceDatabaseNameInTmp();
	// import
	bool importDatabaseFromTmp(const std::string & tmpSqlPath);

	void afterDuplicated();
};

