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
 * @file   SshParamsPage.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-12
 *********************************************************************/
#include "SshParamsPage.h"
#include "core/common/Lang.h"
#include "common/Config.h"

BEGIN_EVENT_TABLE(SshParamsPage, wxPanel)
	EVT_CHECKBOX(Config::CONNECT_USE_SSH_TUNNEL_CHECKBOX_ID, OnClickSshTunnelCheckbox)
	EVT_CHECKBOX(Config::CONNECT_SSH_PASSWORD_CHECKBOX_ID, OnClickSshPasswordCheckbox)
	EVT_CHECKBOX(Config::CONNECT_SSH_PUBLIC_KEY_CHECKBOX_ID, OnClickSshPublicKeyCheckbox)
	EVT_BUTTON(Config::CONNECT_SSH_PRIVATE_KEY_BUTTON_ID, OnClickSshPrivateKeyButton)

	// runtime data has change in the controls, OnRuntimeDataHasChanged will be called
	EVT_TEXT(Config::CONNECT_SSH_HOST_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_SSH_PORT_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_SSH_USERNAME_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_SSH_PASSWORD_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_SSH_SAVE_PASSPHRASE_CHECKBOX_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_SSH_PRIVATE_KEY_EDIT_ID, OnRuntimeDataHasChanged)
END_EVENT_TABLE()

SshParamsPage::SshParamsPage():QPanel()
{
}

void SshParamsPage::init()
{
}

void SshParamsPage::createControls()
{
	QPanel::createControls();

	// Use SSH Tunneling
	createUseSshTunnelCtrls();
	sshHostEdit = createInputControl(Config::CONNECT_SSH_HOST_EDIT_ID, S("ssh-host"), Config::CONNECT_SSH_HOST_LABEL_ID);
	sshPortEdit = createInputControl(Config::CONNECT_SSH_PORT_EDIT_ID, S("ssh-port"), Config::CONNECT_SSH_PORT_LABEL_ID, {140, -1});
	sshUsernameEdit = createInputControl(Config::CONNECT_SSH_USERNAME_EDIT_ID, S("username"), Config::CONNECT_SSH_USERNAME_LABEL_ID, {140, -1});

	// Use password to ssh tunneling
	createUsePasswordGroup();
}

/**
 * load data to ui. It must be called after createControls 
 * 
 */
void SshParamsPage::loadControls()
{
	if (supplier->runtimeUserConnect == nullptr) {
		return;
	}
	useSshTunnelCheckbox->SetValue(supplier->runtimeUserConnect->isSshTunnel);
	sshHostEdit->SetValue(supplier->runtimeUserConnect->sshHost);
	sshPortEdit->SetValue(wxString::Format("%d", supplier->runtimeUserConnect->sshPort));
	sshUsernameEdit->SetValue(supplier->runtimeUserConnect->sshUserName);
	useSshPasswordCheckbox->SetValue(supplier->runtimeUserConnect->isSshPassword);
	sshPasswordEdit->SetValue(supplier->runtimeUserConnect->sshPassword);
	saveSshPasswordCheckbox->SetValue(supplier->runtimeUserConnect->isSshPasswordSaved);
	useSshPublicKeyCheckbox->SetValue(supplier->runtimeUserConnect->isSshPublickey);
	sshPassphraseEdit->SetValue(supplier->runtimeUserConnect->sshPullickeyPassphrase);
	saveSshPassphraseCheckbox->SetValue(supplier->runtimeUserConnect->isSshPullickeyPassphraseSaved);
	sshPrivateKeyEdit->SetValue(supplier->runtimeUserConnect->sshPrivatekeyFilepath);

	setEnableState();
}


void SshParamsPage::exchangeData()
{
	supplier->runtimeUserConnect->isSshTunnel = useSshTunnelCheckbox->IsChecked();
	supplier->runtimeUserConnect->sshHost = sshHostEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->sshPort = std::atoi(sshPortEdit->GetValue().c_str());
	supplier->runtimeUserConnect->sshUserName = sshUsernameEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->isSshPassword = useSshPasswordCheckbox->IsChecked();
	supplier->runtimeUserConnect->sshPassword = sshPasswordEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->isSshPasswordSaved = saveSshPasswordCheckbox->IsChecked();
	supplier->runtimeUserConnect->isSshPublickey = useSshPublicKeyCheckbox->IsChecked();
	supplier->runtimeUserConnect->sshPullickeyPassphrase = sshPassphraseEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->isSshPullickeyPassphraseSaved = saveSshPassphraseCheckbox->IsChecked();
	supplier->runtimeUserConnect->sshPrivatekeyFilepath = sshPrivateKeyEdit->GetValue().ToStdString();

}

void SshParamsPage::createUseSshTunnelCtrls()
{
	topSizer->Add(3, 6, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 5);

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(horizontalSizer, 0, wxALIGN_TOP, 3);

	horizontalSizer->AddSpacer(10);	
	useSshTunnelCheckbox = new wxCheckBox(this, Config::CONNECT_USE_SSH_TUNNEL_CHECKBOX_ID, S("use-ssh-tunnel"));
	horizontalSizer->Add(useSshTunnelCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
}

void SshParamsPage::createUsePasswordGroup()
{
	topSizer->Add(3, 6, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 3);

	//auto 
	//auto box = new wxStaticBox(this, wxID_ANY, useSshPasswordCheckbox, wxDefaultPosition, {450, 50});
	auto box = new wxStaticBox(this, wxID_ANY, S("choose-password-publickey"), wxDefaultPosition, {450, 200});
	
	
	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(horizontalSizer, 0, wxALIGN_TOP, 3);
	
	horizontalSizer->AddSpacer(10);
	wxStaticBoxSizer* groupBoxSizer = new wxStaticBoxSizer(box, wxVERTICAL);
	horizontalSizer->Add(groupBoxSizer, 0, wxALIGN_LEFT,  3);
	
	// choose password to authonizion
	wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
	groupBoxSizer->Add(passwordSizer, 0, wxALIGN_TOP, 3);

	passwordSizer->AddSpacer(10);
	useSshPasswordCheckbox = new wxCheckBox(this, Config::CONNECT_SSH_PASSWORD_CHECKBOX_ID, S("password"), wxDefaultPosition, {115, -1}, wxALIGN_LEFT);
	passwordSizer->Add(useSshPasswordCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
	passwordSizer->AddSpacer(5);

	
	wxBoxSizer* rightSizer = new wxBoxSizer(wxHORIZONTAL);
	passwordSizer->Add(rightSizer, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	sshPasswordEdit = new wxTextCtrl(this, Config::CONNECT_SSH_PASSWORD_EDIT_ID, wxEmptyString, wxDefaultPosition, {150, -1}, wxALIGN_LEFT | wxTE_PASSWORD);
	rightSizer->Add(sshPasswordEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 0);
	
	rightSizer->AddSpacer(10);

	saveSshPasswordCheckbox = new wxCheckBox(this, Config::CONNECT_SSH_SAVE_PASSWORD_CHECKBOX_ID, S("save-password"), wxDefaultPosition, { 140, -1 });
	rightSizer->Add(saveSshPasswordCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
	
	groupBoxSizer->AddSpacer(10);

	// choose public key to auth
	wxBoxSizer* publicKeySizer = new wxBoxSizer(wxHORIZONTAL);
	groupBoxSizer->Add(publicKeySizer, 0, wxALIGN_TOP, 3);
	publicKeySizer->AddSpacer(10);

	useSshPublicKeyCheckbox = new wxCheckBox(this, Config::CONNECT_SSH_PUBLIC_KEY_CHECKBOX_ID, S("public-key"), wxDefaultPosition, {115, -1}, wxALIGN_LEFT);
	publicKeySizer->Add(useSshPublicKeyCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	groupBoxSizer->AddSpacer(10);

	// private key passphrase
	wxBoxSizer* passhpraseSizer = new wxBoxSizer(wxHORIZONTAL);
	groupBoxSizer->Add(passhpraseSizer, 0, wxALIGN_TOP, 3);

	passhpraseSizer->AddSpacer(30);
	wxStaticText* label = new wxStaticText(this, Config::CONNECT_SSH_PASSPHRASE_LABEL_ID, S("passphrase"), wxDefaultPosition, { 100, -1 }, wxALIGN_LEFT);
	passhpraseSizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	wxBoxSizer* rightSizer2 = new wxBoxSizer(wxHORIZONTAL);
	passhpraseSizer->Add(rightSizer2, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	sshPassphraseEdit = new wxTextCtrl(this, Config::CONNECT_SSH_PASSPHRASE_EDIT_ID, wxEmptyString, wxDefaultPosition, {150, -1}, wxALIGN_LEFT | wxTE_PASSWORD);
	rightSizer2->Add(sshPassphraseEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 0);
	
	rightSizer2->AddSpacer(10);

	saveSshPassphraseCheckbox = new wxCheckBox(this, Config::CONNECT_SSH_SAVE_PASSPHRASE_CHECKBOX_ID, S("save-passphrase"), wxDefaultPosition, { 140, -1 });
	rightSizer2->Add(saveSshPassphraseCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3); 

	// private key path 
	groupBoxSizer->AddSpacer(10);
	wxBoxSizer* privateKeySizer = new wxBoxSizer(wxHORIZONTAL);
	groupBoxSizer->Add(privateKeySizer, 0, wxALIGN_TOP, 3);

	privateKeySizer->AddSpacer(30);
	wxStaticText* label2 = new wxStaticText(this, Config::CONNECT_SSH_PRIVATE_KEY_LABEL_ID, S("private-key"), wxDefaultPosition, { 100, -1 }, wxALIGN_LEFT);
	privateKeySizer->Add(label2, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	wxBoxSizer* rightSizer3 = new wxBoxSizer(wxHORIZONTAL);
	privateKeySizer->Add(rightSizer3, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	sshPrivateKeyEdit = new wxTextCtrl(this, Config::CONNECT_SSH_PRIVATE_KEY_EDIT_ID, wxEmptyString, wxDefaultPosition, {260, -1}, wxALIGN_LEFT );
	rightSizer3->Add(sshPrivateKeyEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 0);
	
	rightSizer3->AddSpacer(10);
	sshPrivateKeyButton = new wxButton(this, Config::CONNECT_SSH_PRIVATE_KEY_BUTTON_ID, _("..."), wxDefaultPosition, { 30, 20 });
	rightSizer3->Add(sshPrivateKeyButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 0);

	groupBoxSizer->AddSpacer(5);
}

void SshParamsPage::setEnableState()
{
	if (useSshTunnelCheckbox->GetValue()) {
		FindWindow(Config::CONNECT_SSH_HOST_LABEL_ID)->Enable();
		FindWindow(Config::CONNECT_SSH_PORT_LABEL_ID)->Enable();
		FindWindow(Config::CONNECT_SSH_USERNAME_LABEL_ID)->Enable();

		sshHostEdit->Enable();
		sshPortEdit->Enable();
		sshUsernameEdit->Enable();
		useSshPasswordCheckbox->Enable();
		useSshPublicKeyCheckbox->Enable();
	} else {
		FindWindow(Config::CONNECT_SSH_HOST_LABEL_ID)->Disable();
		FindWindow(Config::CONNECT_SSH_PORT_LABEL_ID)->Disable();
		FindWindow(Config::CONNECT_SSH_USERNAME_LABEL_ID)->Disable();

		sshHostEdit->Disable();
		sshPortEdit->Disable();
		sshUsernameEdit->Disable();
		useSshPasswordCheckbox->Disable();
		useSshPublicKeyCheckbox->Disable();
	}

	if (useSshPasswordCheckbox->GetValue() && useSshTunnelCheckbox->GetValue()) {
		//FindWindow(Config::CONNECT_SSH_PASSWORD_LABEL_ID)->Enable();

		sshPasswordEdit->Enable();
		saveSshPasswordCheckbox->Enable();
	} else {
		//FindWindow(Config::CONNECT_SSH_PASSWORD_LABEL_ID)->Disable();

		sshPasswordEdit->Disable();
		saveSshPasswordCheckbox->Disable();
	}

	if (useSshPublicKeyCheckbox->GetValue() && useSshTunnelCheckbox->GetValue()) {
		FindWindow(Config::CONNECT_SSH_PASSPHRASE_LABEL_ID)->Enable();
		FindWindow(Config::CONNECT_SSH_PRIVATE_KEY_LABEL_ID)->Enable();

		sshPassphraseEdit->Enable();
		saveSshPassphraseCheckbox->Enable();
		sshPrivateKeyEdit->Enable();
		sshPrivateKeyButton->Enable();
	} else {
		FindWindow(Config::CONNECT_SSH_PASSPHRASE_LABEL_ID)->Disable();
		FindWindow(Config::CONNECT_SSH_PRIVATE_KEY_LABEL_ID)->Disable();

		sshPassphraseEdit->Disable();
		saveSshPassphraseCheckbox->Disable();
		sshPrivateKeyEdit->Disable();
		sshPrivateKeyButton->Disable();
	}
}

void SshParamsPage::OnClickSshTunnelCheckbox(wxCommandEvent& event)
{
	setEnableState();
	setRuntimeDataDirty(true);
}

void SshParamsPage::OnClickSshPasswordCheckbox(wxCommandEvent& event)
{
	bool check = useSshPasswordCheckbox->GetValue();
	useSshPublicKeyCheckbox->SetValue(!check);
	setEnableState();
	setRuntimeDataDirty(true);
}

void SshParamsPage::OnClickSshPublicKeyCheckbox(wxCommandEvent& event)
{
	bool check = useSshPublicKeyCheckbox->GetValue();
	useSshPasswordCheckbox->SetValue(!check);
	setEnableState();
	setRuntimeDataDirty(true);
}

void SshParamsPage::OnClickSshPrivateKeyButton(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, S("open-file"), "", "",
		"Putty Private Key Files (*.ppk)|*.ppk|All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	wxString path = openFileDialog.GetPath();
	sshPrivateKeyEdit->SetValue(path);
	setRuntimeDataDirty(true);
}
