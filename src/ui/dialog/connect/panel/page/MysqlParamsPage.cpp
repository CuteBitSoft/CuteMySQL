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
 * @file   MysqlParamsPanel.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-11
 *********************************************************************/
#include "MysqlParamsPage.h"
#include "core/common/Lang.h"
#include "common/Config.h"

BEGIN_EVENT_TABLE(MysqlParamsPage, wxPanel)
	EVT_CHECKBOX(Config::CONNECT_DEFAULT_IDLE_TIMEOUT_CHECKBOX_ID, OnClickIdleTimeoutCheckbox)
	EVT_CHECKBOX(Config::CONNECT_SPEC_IDLE_TIMEOUT_CHECKBOX_ID, OnClickIdleTimeoutCheckbox)

	// runtime data has change in the controls, OnRuntimeDataHasChanged will be called
	EVT_TEXT(Config::CONNECT_HOST_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_USERNAME_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_PASSWORD_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_PORT_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_DATABASES_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_CHECKBOX(Config::CONNECT_USE_COMPRESSED_CHECKBOX_ID, OnRuntimeDataHasChanged)
	EVT_CHECKBOX(Config::CONNECT_READONLY_CHECKBOX_ID, OnRuntimeDataHasChanged)
	EVT_CHECKBOX(Config::CONNECT_READONLY_CHECKBOX_ID, OnRuntimeDataHasChanged)
	EVT_CHECKBOX(Config::CONNECT_SPEC_IDLE_TIMEOUT_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_CHECKBOX(Config::CONNECT_KEEP_ALIVE_EDIT_ID, OnRuntimeDataHasChanged)
END_EVENT_TABLE()

MysqlParamsPage::MysqlParamsPage():QPanel()
{
}

void MysqlParamsPage::init()
{
}

void MysqlParamsPage::createControls()
{
	QPanel::createControls();

	// host
	hostEdit = createInputControl(Config::CONNECT_HOST_EDIT_ID, S("host"));
	// username
	userNameEdit = createInputControl(Config::CONNECT_USERNAME_EDIT_ID, S("username"));
	// password
	createPasswordCtrls();
	// port
	portEdit = createInputControl(Config::CONNECT_PORT_EDIT_ID, S("port"), wxID_STATIC, wxSize(150, -1));
	// Databases
	databasesEdit = createInputControl(Config::CONNECT_DATABASES_EDIT_ID, S("databases"));
	createDatbasesDiscription();

	//use compressed protocol and read only
	createCompressedAndReadonly();

	// connection idle timeout and keep alive interval
	createIdelTimeoutAndKeepAliveInterval();
}

void MysqlParamsPage::loadControls()
{
	if (supplier->runtimeUserConnect == nullptr) {
		return;
	}
	wxString idleTimeOutStr = supplier->runtimeUserConnect->idleTimeout ?
		wxString::Format("%d", supplier->runtimeUserConnect->idleTimeout) :
		_("");

	hostEdit->SetValue(supplier->runtimeUserConnect->host);
	portEdit->SetValue(wxString::Format("%d", supplier->runtimeUserConnect->port));
	userNameEdit->SetValue(supplier->runtimeUserConnect->userName);
	passwordEdit->SetValue(supplier->runtimeUserConnect->password);
	savePasswordCheckbox->SetValue(supplier->runtimeUserConnect->isPasswordSaved);
	databasesEdit->SetValue(supplier->runtimeUserConnect->databases);
	useCompressedCheckbox->SetValue(supplier->runtimeUserConnect->isUseCompressed);
	readOnlyCheckbox->SetValue(supplier->runtimeUserConnect->isReadOnly);
	defaultIdleTimeoutCheckbox->SetValue(supplier->runtimeUserConnect->idleTimeout == 0);
	specIdleTimeoutCheckbox->SetValue(supplier->runtimeUserConnect->idleTimeout > 0);	
	idleTimeoutEdit->SetValue(idleTimeOutStr);
	keepAliveEdit->SetValue(wxString::Format("%d", supplier->runtimeUserConnect->keepAlive));

	//idleTimeoutEdit->SetEditable(supplier->runtimeUserConnect->idleTimeout);
	supplier->runtimeUserConnect->idleTimeout ? idleTimeoutEdit->Enable() : idleTimeoutEdit->Disable();
}

void MysqlParamsPage::exchangeData()
{
	supplier->runtimeUserConnect->host = hostEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->port = std::atoi(portEdit->GetValue().c_str());
	supplier->runtimeUserConnect->userName = userNameEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->password = passwordEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->isPasswordSaved = savePasswordCheckbox->IsChecked();
	supplier->runtimeUserConnect->databases = databasesEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->isUseCompressed = useCompressedCheckbox->IsChecked();
	supplier->runtimeUserConnect->isReadOnly = readOnlyCheckbox->IsChecked();
	supplier->runtimeUserConnect->idleTimeout = defaultIdleTimeoutCheckbox->IsChecked() ? 0 : 
		std::atoi(idleTimeoutEdit->GetValue().c_str());
	supplier->runtimeUserConnect->keepAlive = std::atoi(keepAliveEdit->GetValue().c_str());
}

void MysqlParamsPage::createPasswordCtrls()
{
	topSizer->Add(3, 6, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 3);

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	horizontalSizer->AddSpacer(10);
	topSizer->Add(horizontalSizer, 0, wxALIGN_TOP, 3);

	wxStaticText* label = new wxStaticText(this, wxID_STATIC, S("password"), wxDefaultPosition, {130, -1}, wxALIGN_LEFT);
	horizontalSizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
	horizontalSizer->AddSpacer(5);


	wxBoxSizer* rightSizer = new wxBoxSizer(wxHORIZONTAL);
	horizontalSizer->Add(rightSizer, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	passwordEdit = new wxTextCtrl(this, Config::CONNECT_PASSWORD_EDIT_ID, wxEmptyString, wxDefaultPosition, {150, -1}, wxALIGN_LEFT | wxTE_PASSWORD);
	rightSizer->Add(passwordEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 0);
	
	rightSizer->AddSpacer(10);

	savePasswordCheckbox = new wxCheckBox(this, Config::CONNECT_SAVE_PASSWORD_CHECKBOX_ID, S("save-password"), wxDefaultPosition, { 140, -1 });
	rightSizer->Add(savePasswordCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
}

void MysqlParamsPage::createDatbasesDiscription()
{
	topSizer->Add(3, 6, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 3);
	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	horizontalSizer->AddSpacer(10);
	topSizer->Add(horizontalSizer, 0, wxALIGN_TOP, 3);

	wxStaticText* label = new wxStaticText(this, wxID_STATIC, S("databases-desription"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	horizontalSizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
}

void MysqlParamsPage::createCompressedAndReadonly()
{
	topSizer->Add(3, 6, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 3);
	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	horizontalSizer->AddSpacer(10);
	topSizer->Add(horizontalSizer, 0, wxALIGN_TOP, 3);

	useCompressedCheckbox = new wxCheckBox(this, Config::CONNECT_USE_COMPRESSED_CHECKBOX_ID, S("use-compressed"), wxDefaultPosition, { 200, -1 });
	horizontalSizer->Add(useCompressedCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	horizontalSizer->AddSpacer(10);

	readOnlyCheckbox = new wxCheckBox(this, Config::CONNECT_READONLY_CHECKBOX_ID, S("read-only-connection"), wxDefaultPosition, { 200, -1 });
	horizontalSizer->Add(readOnlyCheckbox, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
}

void MysqlParamsPage::createIdelTimeoutAndKeepAliveInterval()
{
	topSizer->Add(3, 6, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 3);
	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	horizontalSizer->AddSpacer(10);
	topSizer->Add(horizontalSizer, 0, wxALIGN_TOP, 3);

	// create session idle timeout group-box as wxStaticBoxSizer
	idleTimeoutGroup = new wxStaticBoxSizer(wxHORIZONTAL, this, S("session-idel-timeout"));
	horizontalSizer->Add(idleTimeoutGroup, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	horizontalSizer->AddSpacer(10);

	// create keep alive interval group-box as wxStaticBoxSizer
	keepAliveGroup = new wxStaticBoxSizer(wxHORIZONTAL, this, S("keep-alive-interval"));
	horizontalSizer->Add(keepAliveGroup, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);


	// session idle timeout group-box
	idleTimeoutGroup->AddSpacer(10);

	defaultIdleTimeoutCheckbox = new wxCheckBox(this, Config::CONNECT_DEFAULT_IDLE_TIMEOUT_CHECKBOX_ID, S("default"), wxDefaultPosition, {120, -1});
	idleTimeoutGroup->Add(defaultIdleTimeoutCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	specIdleTimeoutCheckbox = new wxCheckBox(this, Config::CONNECT_SPEC_IDLE_TIMEOUT_CHECKBOX_ID, _(""), wxDefaultPosition, {20, -1});
	idleTimeoutGroup->Add(specIdleTimeoutCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	idleTimeoutEdit = new wxTextCtrl(this, Config::CONNECT_SPEC_IDLE_TIMEOUT_EDIT_ID, wxEmptyString, wxDefaultPosition, {50, 20}, wxALIGN_LEFT);
	idleTimeoutGroup->Add(idleTimeoutEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	idleTimeoutGroup->AddSpacer(10);

	wxString secondTxt = _("(") + S("seconds") + _(")");
	wxStaticText* label = new wxStaticText(this, wxID_STATIC, secondTxt, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	idleTimeoutGroup->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	// keep alive interval group-box as wxStaticBoxSizer
	keepAliveGroup->AddSpacer(10);

	keepAliveEdit = new wxTextCtrl(this, Config::CONNECT_KEEP_ALIVE_EDIT_ID, wxEmptyString, wxDefaultPosition, { 80, 20 }, wxALIGN_LEFT);
	keepAliveGroup->Add(keepAliveEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	keepAliveGroup->AddSpacer(10);
	wxStaticText* label2 = new wxStaticText(this, wxID_STATIC, secondTxt, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	keepAliveGroup->Add(label2, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
}

void MysqlParamsPage::OnClickIdleTimeoutCheckbox(wxCommandEvent& event)
{
	if (event.GetId() == Config::CONNECT_DEFAULT_IDLE_TIMEOUT_CHECKBOX_ID) {
		bool check = defaultIdleTimeoutCheckbox->GetValue();
		specIdleTimeoutCheckbox->SetValue(!check);
		if (check) {
			idleTimeoutEdit->Disable();
		} else {
			idleTimeoutEdit->Enable();
			idleTimeoutEdit->SetFocus();
		}
	} else {
		bool check = specIdleTimeoutCheckbox->GetValue();
		defaultIdleTimeoutCheckbox->SetValue(!check);
		if (check) {
			idleTimeoutEdit->Enable(); 
			idleTimeoutEdit->SetFocus();
		} else {
			idleTimeoutEdit->Disable();
		}
	}
	setRuntimeDataDirty(true); // the data has dirty
}
