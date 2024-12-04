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
 * @file   DatabaseDialog.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-29
 *********************************************************************/
#include "DatabaseDialog.h"
#include "core/common/Lang.h"
#include "common/Config.h"
#include "ui/common/data/QClientData.h"
#include "ui/common/msgbox/QAnimateBox.h"
#include "common/AppContext.h"

BEGIN_EVENT_TABLE(DatabaseDialog, wxDialog)
	EVT_BUTTON(wxID_OK, OnClickOkButton)
	EVT_COMBOBOX(Config::DATABASE_DIALOG_CHARSET_COMBOBOX_ID, OnSelChangeCharsetCombobox)
END_EVENT_TABLE()

DatabaseDialog::DatabaseDialog(DatabaseType type): QDialog(), databaseType(type)
{

}

DatabaseDialog::~DatabaseDialog()
{

}

void DatabaseDialog::init()
{
	QDialog::init();
}

void DatabaseDialog::createControls()
{
	SetSize(600, 400);
	QDialog::createControls();

	vLayout = new wxBoxSizer(wxVERTICAL);
	tLayout = new wxBoxSizer(wxVERTICAL);
	bLayout = new wxBoxSizer(wxHORIZONTAL);
	vLayout->Add(tLayout, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	vLayout->AddSpacer(5);
	vLayout->Add(bLayout, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 10);

	createInputs();
	createButtons();

	this->SetSizer(vLayout);

	
}

void DatabaseDialog::createInputs()
{
	tLayout->AddSpacer(10);
	// CONNECTION
	auto connectLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(connectLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 10);
	connectLayout->AddSpacer(20);

	wxStaticText * conectLabel = new wxStaticText(this, wxID_STATIC, S("select-connection"), wxDefaultPosition, { 130, -1 }, wxALIGN_LEFT);
	connectLayout->Add(conectLabel, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	connectLayout->AddSpacer(20);

	connectComboBox = new wxBitmapComboBox(this, Config::DATABASE_DIALOG_CONNECT_COMBOBOX_ID, wxEmptyString, 
		wxDefaultPosition, { 200, -1 }, wxArrayString(), wxNO_BORDER | wxCLIP_CHILDREN| wxCB_READONLY);
	connectLayout->Add(connectComboBox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	connectLayout->AddSpacer(20);

	tLayout->AddSpacer(10);

	// DATABASE NAME
	auto dbNameLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(dbNameLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 10);
	dbNameLayout->AddSpacer(20);
	wxStaticText * databaseNameLabel = new wxStaticText(this, wxID_STATIC, S("database-name"), wxDefaultPosition, { 130, -1 }, wxALIGN_LEFT);
	dbNameLayout->Add(databaseNameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	dbNameLayout->AddSpacer(20);

	databaseNameEdit = new wxTextCtrl(this, Config::DATABASE_NAME_EDIT_ID, wxEmptyString, wxDefaultPosition, { 200, -1 }, wxNO_BORDER | wxCLIP_CHILDREN );
	databaseNameEdit->SetFocus();
	dbNameLayout->Add(databaseNameEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	dbNameLayout->AddSpacer(20);

	tLayout->AddSpacer(10);

	// DATABASE CHARSET
	auto charsetLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(charsetLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 10);
	charsetLayout->AddSpacer(20);

	wxStaticText * charsetLabel = new wxStaticText(this, wxID_STATIC, S("database-charset"), wxDefaultPosition, { 130, -1 }, wxALIGN_LEFT);
	charsetLayout->Add(charsetLabel, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	charsetLayout->AddSpacer(20);

	charsetComboBox = new wxBitmapComboBox(this, Config::DATABASE_DIALOG_CHARSET_COMBOBOX_ID, wxEmptyString, 
		wxDefaultPosition, { 200, -1 }, wxArrayString(), wxNO_BORDER | wxCLIP_CHILDREN | wxCB_READONLY | wxCB_READONLY);
	charsetLayout->Add(charsetComboBox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	charsetLayout->AddSpacer(20);

	tLayout->AddSpacer(10);

	// DATABASE Collation
	auto collationLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(collationLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 10);
	collationLayout->AddSpacer(20);

	wxStaticText * collationLabel = new wxStaticText(this, wxID_STATIC, S("database-collation"),  wxDefaultPosition, { 130, -1 }, wxALIGN_LEFT);
	collationLayout->Add(collationLabel, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	collationLayout->AddSpacer(20);

	collationComboBox = new wxBitmapComboBox(this, Config::DATABASE_DIALOG_COLLATION_COMBOBOX_ID, wxEmptyString,
		wxDefaultPosition, { 200, -1 },wxArrayString(),  wxNO_BORDER | wxCLIP_CHILDREN | wxCB_READONLY | wxCB_READONLY);
	collationLayout->Add(collationComboBox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	collationLayout->AddSpacer(20);
}

void DatabaseDialog::createButtons()
{
	bLayout->AddSpacer(170);
	okButton = new wxButton(this, wxID_OK, S("create"), wxDefaultPosition, { 70, -1 });
	bLayout->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	bLayout->AddSpacer(20);

	cancelButton = new wxButton(this, wxID_CANCEL, S("cancel"), wxDefaultPosition, { 70, -1 });
	bLayout->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
}

void DatabaseDialog::OnSelChangeCharsetCombobox(wxCommandEvent& event)
{
	auto connectData = (QClientData<UserConnect> *)connectComboBox->GetClientObject(connectComboBox->GetSelection());
	auto charsetData = (QClientData<CharsetInfo> *)charsetComboBox->GetClientObject(charsetComboBox->GetSelection());
	if (!connectData || !charsetData) {
		return;
	}
	delegate->loadForCollationComboBox(collationComboBox, connectData->getDataPtr()->id, 
		charsetData->getDataPtr()->name, charsetData->getDataPtr()->defaultCollation);
}

void DatabaseDialog::OnClickOkButton(wxCommandEvent& event)
{
	auto connectData = (QClientData<UserConnect>*)connectComboBox->GetClientObject(connectComboBox->GetSelection());
	if (connectData == nullptr || connectData->getDataPtr() == nullptr) {
		QAnimateBox::error(S("not-choose-connection"));
		connectComboBox->SetFocus();
		return;
	}

	UserDb & userDb = databaseSupplier->handleUserDb;
	userDb.connectId = connectData->getDataPtr()->id;
	userDb.name = databaseNameEdit->GetValue();

	if (userDb.name.empty()) {
		QAnimateBox::error(S("no-database-name"));
		databaseNameEdit->SetFocus();
		return;
	}

	QClientData<CharsetInfo>* charsetData = nullptr;
	if (charsetComboBox->HasClientObjectData()) {
		charsetData = (QClientData<CharsetInfo>*)charsetComboBox->GetClientObject(charsetComboBox->GetSelection());
	}
	
	if (charsetData == nullptr || charsetData->getDataPtr() == nullptr) {
		userDb.charset = "utf8";
	} else {
		userDb.charset = charsetData->getDataPtr()->name;
	}
	QClientData<CollationInfo>* collationData = nullptr;
	if (collationComboBox->HasClientObjectData()) {
		collationData = (QClientData<CollationInfo>*)collationComboBox->GetClientObject(collationComboBox->GetSelection());
	}
	
	if (collationData == nullptr || collationData->getDataPtr() == nullptr) {
		userDb.collation = charsetData ? charsetData->getDataPtr()->defaultCollation : "utf8_general_ci";
	} else {
		userDb.collation = collationData->getDataPtr()->name; 
	}

	try {
		if (databaseService->hasUserDb(userDb.connectId, userDb.name)) {
			auto error = StringUtil::replace(S("exists-database-name"), "{dbName}", userDb.name);
			QAnimateBox::error(error);
			databaseNameEdit->SelectAll();
			databaseNameEdit->SetFocus();
			return;
		}

		databaseService->createUserDb(userDb);

		
		QAnimateBox::success(S("create-db-success-text"));
		AppContext::getInstance()->dispatch(Config::MSG_ADD_DATABASE_ID, userDb.connectId);

		EndModal(wxID_OK);
		
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
		return;
	}	
}


void DatabaseDialog::loadControls()
{
	delegate->loadForConnectComboBox(connectComboBox);

	auto nSelItem = connectComboBox->GetSelection();
	if (nSelItem == -1) {
		return;
	}
	auto data = reinterpret_cast<QClientData<UserConnect>*>(connectComboBox->GetClientObject(nSelItem));

	std::string defaultCharset;
	if (databaseType == DatabaseType::DATABASE_ALTER) {
		databaseNameEdit->SetValue(databaseSupplier->runtimeUserDb->name);
		auto charsetData = (QClientData<CharsetInfo> *)(charsetComboBox->GetClientObject(charsetComboBox->GetSelection()));
		if (charsetData) {
			defaultCharset = charsetData->getDataPtr()->name;
		}		
	}

	delegate->loadForCharsetComboBox(charsetComboBox, data->getDataPtr()->id);
	delegate->loadForCollationComboBox(collationComboBox, data->getDataPtr()->id, defaultCharset);

}
