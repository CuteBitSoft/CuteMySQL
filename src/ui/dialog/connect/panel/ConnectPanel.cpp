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
 * @file   ConnectPanel.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-13
 *********************************************************************/
#include "ConnectPanel.h"
#include "core/common/Lang.h"
#include "common/Config.h"
#include "common/AppContext.h"

BEGIN_EVENT_TABLE(ConnectPanel, wxPanel)
	EVT_TEXT(Config::CONNECT_NAME_EDIT_ID, OnConnectNameEditTextChange)
END_EVENT_TABLE()

ConnectPanel::ConnectPanel(): QPanel()
{
	
}

void ConnectPanel::init()
{
}

/**
 * load data to UI.It must be called after createControls
 * 
 */
void ConnectPanel::loadControls()
{
	connectNameEdit->SetValue(supplier->runtimeUserConnect->name);
	mysqlParamsPage->loadControls();
	sshParamsPage->loadControls();
	sslParamsPage->loadControls();

	connectNameEdit->SelectAll();
	connectNameEdit->SetFocus();
}

void ConnectPanel::clearControls()
{
	connectNameEdit->Clear();
	mysqlParamsPage->clearControls();
	sshParamsPage->clearControls();
	sslParamsPage->clearControls();
}

/**
 * exchange data from the controls to supplier->runtimeUserConnect.
 * 
 */
void ConnectPanel::exchangeData()
{
	supplier->runtimeUserConnect->name = connectNameEdit->GetValue().ToStdString();
	mysqlParamsPage->exchangeData();
	sshParamsPage->exchangeData();
	sslParamsPage->exchangeData();
}

void ConnectPanel::disableControls()
{
	connectNameLabel->Disable();
	connectNameEdit->Disable();
}

void ConnectPanel::enableControls()
{
	connectNameLabel->Enable();
	connectNameEdit->Enable();
}

void ConnectPanel::createControls()
{

	QPanel::createControls();

	// connection name
	createConnectNameCtrls();

	// MySQL notebook(tab ctrl)
	createParamsPages();
}

void ConnectPanel::createConnectNameCtrls()
{
	wxBoxSizer * connectNameSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(connectNameSizer, 0, wxALIGN_TOP, 5);
	
	connectNameLabel = new wxStaticText(this, wxID_STATIC, S("connect-name"), wxDefaultPosition, { 130, -1 }, wxALIGN_RIGHT);
	connectNameSizer->Add(connectNameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	connectNameSizer->AddSpacer(20);

	connectNameEdit = new wxTextCtrl(this, Config::CONNECT_NAME_EDIT_ID, wxEmptyString, wxDefaultPosition, { 300, -1 }, 0);
	connectNameSizer->Add(connectNameEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
}

void ConnectPanel::createParamsPages()
{
	// spacer
	topSizer->AddSpacer(20);

	paramsPages = new wxNotebook(this, wxID_ANY, wxDefaultPosition, { 500, -1 }, 0);
	paramsPages->SetBackgroundColour(bkgColor);
	paramsPages->SetForegroundColour(textColor);

	// MySQL params page
	mysqlParamsPage = new MysqlParamsPage();
	mysqlParamsPage->Create(paramsPages, Config::CONNECT_MYSQL_PARAMS_PAGE_ID, wxDefaultPosition, {480, -1});
	paramsPages->AddPage(mysqlParamsPage, "MySQL", true);

	// ssh params page
	sshParamsPage = new SshParamsPage();
	sshParamsPage->Create(paramsPages, Config::CONNECT_SSH_PARAMS_PAGE_ID, wxDefaultPosition, {480, -1});
	paramsPages->AddPage(sshParamsPage, "SSH");

	// SSL params page
	sslParamsPage = new SslParamsPage();
	sslParamsPage->Create(paramsPages, Config::CONNECT_SSL_PARAMS_PAGE_ID, wxDefaultPosition, { 480, -1 });
	paramsPages->AddPage(sslParamsPage, "SSL");

	topSizer->Add(paramsPages, 1, wxALIGN_TOP| wxEXPAND, 5);
}

void ConnectPanel::OnConnectNameEditTextChange(wxCommandEvent& event)
{
	if (!supplier->runtimeUserConnect) {
		//auto wParam = (uint64_t)connectNameEdit->GetValue().ToStdString().c_str();
		//AppContext::getInstance()->dispatch(Config::MSG_CONNECTION_CREATE_ID, wParam);
		return;
	}
	supplier->runtimeUserConnect->name = connectNameEdit->GetValue().ToStdString();
	setRuntimeDataDirty(true);
	AppContext::getInstance()->dispatch(Config::MSG_CONNECTION_CHANGE_NAME_ID, (uint64_t)supplier->runtimeUserConnect);
}
