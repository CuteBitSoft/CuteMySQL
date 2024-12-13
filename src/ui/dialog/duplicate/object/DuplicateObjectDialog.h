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
 * @file   DuplicateObjectDialog.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-05
 *********************************************************************/

#pragma once
#include <wx/treelist.h>
#include <wx/stc/stc.h>
#include "ui/common/dialog/QFormDialog.h"
#include "core/entity/Enum.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "core/service/db/ConnectService.h"
#include "core/service/db/MetadataService.h"
#include "ui/dialog/duplicate/object/delegate/DuplicateObjectDialogDelegate.h"
#include "ui/common/progress/QProgressBar.h"
#include "ui/common/process/QProcess.h"

class DuplicateObjectDialog :  public QFormDialog<DuplicateObjectDialogDelegate>
{
	DECLARE_EVENT_TABLE()
public:
	DuplicateObjectDialog(DuplicateObjectType _dupObjectType);
	~DuplicateObjectDialog();

	// handle export process and import process
	void OnAsyncProcessTermination(QProcess<DuplicateObjectDialog>* process);
private:
	DuplicateObjectType dupObjectType;
	std::string caption;
	std::string description;
	std::string sourceLabelText;
	std::string targetLabelText;
	std::string sourceObjectName;
	std::string sourceObjectType;
	std::string ddl;

	UserConnect userConnect;
	UserDb userDb;
	UserView userView;
	UserRoutine userRoutine;
	UserTrigger userTrigger;
	UserEvent userEvent;

	// top
	wxBoxSizer* topHoriLayout;
	// center1
	wxBoxSizer* center1HoriLayout;
	wxBoxSizer* center1LeftVertLayout;
	wxBoxSizer* center1RightVertLayout;
	// center2
	wxBoxSizer* center2VertLayout;

	// bottom
	wxBoxSizer* bottomHoriLayout;

	// description for dialog - in top layout
	wxStaticBitmap* image;
	wxStaticText* label;
	
	//source controls - in center1 left layout
	wxTextCtrl* sourceConnectEdit;
	wxTextCtrl* sourceDatabaseEdit;
	wxTextCtrl* sourceObjectEdit;

	//target controls - in center1 right layout
	wxBitmapComboBox*	targetConnectComboBox;
	wxBitmapComboBox*	targetDatabaseComboBox;
	wxTextCtrl*			targetObjectEdit;

	wxStyledTextCtrl*	ddlPreviewEdit;

	// lock settings
	wxCheckBox* lockTablesCheckBox;

	std::string tmpSqlPath;

	// process bar 
	QProgressBar* progressbar;
	QProcess<DuplicateObjectDialog>* importProcess;

	DatabaseSupplier* databaseSupplier;
	DatabaseService* databaseService;
	ConnectService* connectService;
	MetadataService* metadataService;

	virtual void init();
	virtual void createInputs();
	virtual void createTopControls();
	virtual void createCenter1Inputs();
	virtual void createCenter2Inputs();
	virtual void createBottomInputs();
	virtual void loadControls();

	// combobox
	void OnSelChangeConnectCombobox(wxCommandEvent& event);
	void OnChangeTargetObjectEditText(wxCommandEvent& event);
	void OnClickOkButton(wxCommandEvent& event);

	std::string generateNewDDL(const std::string & orignalDdl);

	// export
	bool exportObjectToTmp(const std::string & tmpSqlPath);
	// replace database name
	bool replaceDatabaseAndTableInTmp();
	// import
	bool importObjectFromTmp(const std::string & tmpSqlPath);
	void afterDuplicated();
};

