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
 * @file   DuplicateConnectionDialog.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-05
 *********************************************************************/
#include "DuplicateConnectionDialog.h"
#include "common/Config.h"
#include "common/AppContext.h"

BEGIN_EVENT_TABLE(DuplicateConnectionDialog, wxDialog)
	EVT_BUTTON(wxID_OK, OnClickOkButton)
	EVT_COMBOBOX(Config::DUPLICATE_SOURCE_CONNECT_COMBOBOX_ID, OnSelChangeConnectCombobox)
END_EVENT_TABLE()

DuplicateConnectionDialog::DuplicateConnectionDialog() : QFormDialog()
{
	init();
}

void DuplicateConnectionDialog::init()
{
	databaseSupplier = DatabaseSupplier::getInstance();
	connectService = ConnectService::getInstance();
}

void DuplicateConnectionDialog::createInputs()
{
	tLayout->AddSpacer(10);
	// CONNECTION
	auto connectLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(connectLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 10);
	connectLayout->AddSpacer(20);

	wxStaticText * conectLabel = new wxStaticText(this, wxID_STATIC, S("select-connection"), wxDefaultPosition, { 150, -1 }, wxALIGN_LEFT);
	connectLayout->Add(conectLabel, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	connectLayout->AddSpacer(20);

	connectComboBox = new wxBitmapComboBox(this, Config::DUPLICATE_SOURCE_CONNECT_COMBOBOX_ID, wxEmptyString, 
		wxDefaultPosition, { 200, -1 }, wxArrayString(), wxNO_BORDER | wxCLIP_CHILDREN| wxCB_READONLY);
	connectLayout->Add(connectComboBox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	connectLayout->AddSpacer(20);

	tLayout->AddSpacer(10);

	// DATABASE NAME
	auto dupNameLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(dupNameLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 10);
	dupNameLayout->AddSpacer(20);
	wxStaticText * databaseNameLabel = new wxStaticText(this, wxID_STATIC, S("duplicate-connection"), wxDefaultPosition, { 150, -1 }, wxALIGN_LEFT);
	dupNameLayout->Add(databaseNameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	dupNameLayout->AddSpacer(20);

	duplcateNameEdit = new wxTextCtrl(this, Config::DUPLICATE_NAME_EDIT_ID, wxEmptyString, wxDefaultPosition, { 200, -1 }, wxCLIP_CHILDREN );
	duplcateNameEdit->SetFocus();
	dupNameLayout->Add(duplcateNameEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	dupNameLayout->AddSpacer(20);

	tLayout->AddSpacer(10);
}

void DuplicateConnectionDialog::loadControls()
{
	delegate->loadForConnectComboBox(connectComboBox, databaseSupplier->runtimeUserConnect->id);

	auto nSelItem = connectComboBox->GetSelection();
	if (nSelItem == -1) {
		return;
	}
	auto data = reinterpret_cast<QClientData<UserConnect>*>(connectComboBox->GetClientObject(nSelItem));

	wxString newName = databaseSupplier->runtimeUserConnect->name;
	newName.Append(" (Copy)");
	duplcateNameEdit->SetValue(newName);
	duplcateNameEdit->SelectAll();
	duplcateNameEdit->SetFocus();
}

void DuplicateConnectionDialog::OnSelChangeConnectCombobox(wxCommandEvent& event)
{
	if (connectComboBox->GetSelection() ==  wxNOT_FOUND) {
		return;
	}
	auto connectData = (QClientData<UserConnect> *)connectComboBox->GetClientObject(connectComboBox->GetSelection());
	if (!connectData) {
		return;
	}

	wxString newName = connectData->getDataPtr()->name;
	newName.Append(" (Copy)");
	duplcateNameEdit->SetValue(newName);
	duplcateNameEdit->SelectAll();
	duplcateNameEdit->SetFocus();
}

void DuplicateConnectionDialog::OnClickOkButton(wxCommandEvent& event)
{
	auto connectData = (QClientData<UserConnect> *)connectComboBox->GetClientObject(connectComboBox->GetSelection());
	if (!connectData) {
		return;
	}
	UserConnect userConnect(*connectData->getDataPtr());
	userConnect.id = 0;
	userConnect.name = duplcateNameEdit->GetValue();
	if (userConnect.name.empty()) {
		QAnimateBox::error(S("no-duplicate-connection-name"));
		duplcateNameEdit->SetFocus();
		return;
	}

	if (connectComboBox->GetSelection() == wxNOT_FOUND) {
		return;
	}
	
	try {
		auto connectId = connectService->createUserConnect(userConnect);
		AppContext::getInstance()->dispatch(Config::MSG_CONNECTION_CONNECTED_ID, connectId);
		QAnimateBox::success(S("duplicate-success"));
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
		return;
	}

	EndModal(wxID_OK);
}
