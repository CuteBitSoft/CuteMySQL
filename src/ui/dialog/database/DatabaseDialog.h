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
 * @file   DatabaseDialog.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-29
 *********************************************************************/
#pragma once
#include <wx/bmpcbox.h>
#include "ui/common/dialog/QDialog.h"
#include "core/entity/Enum.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/dialog/database//delegate/DatabaseDialogDelegate.h"
#include "core/service/db/DatabaseService.h"

class DatabaseDialog : public QDialog<DatabaseDialogDelegate>
{
	DECLARE_EVENT_TABLE()
public:
	DatabaseDialog(DatabaseType type);
	~DatabaseDialog();
private:
	DatabaseType databaseType;

	wxBoxSizer* vLayout;
	wxBoxSizer* tLayout;
	wxBoxSizer* bLayout;

	wxBitmapComboBox* connectComboBox;
	wxTextCtrl* databaseNameEdit;
	wxComboBox* charsetComboBox;
	wxComboBox* collationComboBox;

	wxButton* okButton;
	wxButton* cancelButton;

	DatabaseSupplier * databaseSupplier = DatabaseSupplier::getInstance();
	DatabaseService * databaseService = DatabaseService::getInstance();


	virtual void init();
	virtual void createControls();

	void loadControls();

	void createInputs();
	void createButtons();

	// combobox
	void OnSelChangeCharsetCombobox(wxCommandEvent & event);
	void OnClickOkButton(wxCommandEvent & event);
};

