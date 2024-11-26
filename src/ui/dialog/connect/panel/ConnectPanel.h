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
 * @file   ConnectPanel.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-13
 *********************************************************************/
#pragma once
#include <wx/notebook.h>
#include "ui/common/panel/QPanel.h"
#include "ui/dialog/connect/supplier/ConnectDialogSupplier.h"
#include "ui/dialog/connect/panel/page/MysqlParamsPage.h"
#include "ui/dialog/connect/panel/page/SshParamsPage.h"
#include "ui/dialog/connect/panel/page/SslParamsPage.h"

class ConnectPanel : public QPanel<ConnectDialogSupplier>
{
	DECLARE_EVENT_TABLE()
public:
	ConnectPanel();
	void loadControls(); // virtual function
	void clearControls(); // virtual function
	void exchangeData(); // virtual function	
	void disableControls();
	void enableControls();
private:
	wxStaticText* connectNameLabel;
	wxTextCtrl* connectNameEdit;

	wxNotebook* paramsPages;

	MysqlParamsPage* mysqlParamsPage;
	SshParamsPage* sshParamsPage;
	SslParamsPage* sslParamsPage;

	virtual void init();
    virtual void createControls();
	void createConnectNameCtrls();
	void createParamsPages();

	// event handle
	void OnConnectNameEditTextChange(wxCommandEvent& event);
};

