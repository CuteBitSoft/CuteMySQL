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
 * @file   SslParamsPage.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-13
 *********************************************************************/
#include "SslParamsPage.h"

#include "core/common/Lang.h"
#include "common/Config.h"

BEGIN_EVENT_TABLE(SslParamsPage, wxPanel)
	EVT_CHECKBOX(Config::CONNECT_USE_SSL_ENCRYPT_CHECKBOX_ID, OnClickUseSslEncryptCheckbox)
	EVT_CHECKBOX(Config::CONNECT_SSL_USE_AUTHENTICATION_CHECKBOX_ID, OnClickUseAuthenticationCheckbox)
	EVT_BUTTON(Config::CONNECT_SSL_CA_CERTIFICATE_BUTTON_ID, OnClickSslCaCertificateButton)
	EVT_BUTTON(Config::CONNECT_SSL_CLIENT_KEY_BUTTON_ID, OnClickClientKeyButton)
	EVT_BUTTON(Config::CONNECT_SSL_CLIENT_CERTIFICATE_BUTTON_ID, OnClickClientCertificateButton)

	// runtime data has change in the controls, OnRuntimeDataHasChanged will be called
	EVT_TEXT(Config::CONNECT_SSL_CA_CERTIFICATE_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_SSL_CIPHER_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_SSL_CLIENT_KEY_EDIT_ID, OnRuntimeDataHasChanged)
	EVT_TEXT(Config::CONNECT_SSL_CLIENT_CERTIFICATE_EDIT_ID, OnRuntimeDataHasChanged)
END_EVENT_TABLE()

SslParamsPage::SslParamsPage():QPanel()
{
	 
}


void SslParamsPage::init()
{
}

void SslParamsPage::createControls()
{
	QPanel::createControls();

	createUseSslEncryptCtrls();
	
	caCertificateEdit = createInputFileControl(Config::CONNECT_SSL_CA_CERTIFICATE_EDIT_ID, Config::CONNECT_SSL_CA_CERTIFICATE_BUTTON_ID,
		S("ca-certificate"), Config::CONNECT_SSL_CA_CERTIFICATE_LABEL_ID, {260, -1});
	
	cipherEdit = createInputControl(Config::CONNECT_SSL_CIPHER_EDIT_ID, S("cipher"),Config::CONNECT_SSL_CIPHER_LABEL_ID,  {260, -1});
	
	createUseAuthenticationGroup();
}

/**
 * load data to UI.It must be called after createControls
 * 
 */
void SslParamsPage::loadControls()
{
	if (supplier->runtimeUserConnect == nullptr) {
		return;
	}
	useSslEncryptCheckbox->SetValue(supplier->runtimeUserConnect->isUseSsl);
	caCertificateEdit->SetValue(supplier->runtimeUserConnect->sslCaCertificate);
	cipherEdit->SetValue(supplier->runtimeUserConnect->sslCipher);
	useAuthenticationCheckbox->SetValue(supplier->runtimeUserConnect->isSslAuth);
	clientKeyEdit->SetValue(supplier->runtimeUserConnect->sslClientKey);
	clientCertificateEdit->SetValue(supplier->runtimeUserConnect->sslClientCertificate);

	setEnableState();
}

void SslParamsPage::exchangeData()
{
	supplier->runtimeUserConnect->isUseSsl = useSslEncryptCheckbox->IsChecked();
	supplier->runtimeUserConnect->sslCaCertificate = caCertificateEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->sslCipher = cipherEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->isSslAuth = useAuthenticationCheckbox->IsChecked();
	supplier->runtimeUserConnect->sslClientKey = clientKeyEdit->GetValue().ToStdString();
	supplier->runtimeUserConnect->sslClientCertificate = clientCertificateEdit->GetValue().ToStdString();
}

void SslParamsPage::createUseSslEncryptCtrls()
{
	topSizer->Add(3, 6, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 5);

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(horizontalSizer, 0, wxALIGN_TOP, 3);

	horizontalSizer->AddSpacer(10);
	useSslEncryptCheckbox = new wxCheckBox(this, Config::CONNECT_USE_SSL_ENCRYPT_CHECKBOX_ID, S("use-ssl-encryption"));
	horizontalSizer->Add(useSslEncryptCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

}

void SslParamsPage::createUseAuthenticationGroup()
{
	topSizer->Add(3, 6, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 3);

	//auto 
	//auto box = new wxStaticBox(this, wxID_ANY, useSshPasswordCheckbox, wxDefaultPosition, {450, 50});
	auto box = new wxStaticBox(this, wxID_ANY, S("ssl-authentication"), wxDefaultPosition, { 450, 200 });


	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(horizontalSizer, 0, wxALIGN_TOP, 3);

	horizontalSizer->AddSpacer(10);
	wxStaticBoxSizer* groupBoxSizer = new wxStaticBoxSizer(box, wxVERTICAL);
	horizontalSizer->Add(groupBoxSizer, 0, wxALIGN_LEFT, 3);

	
	wxBoxSizer* useAuthenticationSizer = new wxBoxSizer(wxHORIZONTAL);
	groupBoxSizer->Add(useAuthenticationSizer, 0, wxALIGN_TOP, 3);

	// create use authentication check-box
	useAuthenticationSizer->AddSpacer(10);
	useAuthenticationCheckbox = new wxCheckBox(this, Config::CONNECT_SSL_USE_AUTHENTICATION_CHECKBOX_ID, S("use-authentication"), wxDefaultPosition, { 115, -1 }, wxALIGN_LEFT);
	useAuthenticationSizer->Add(useAuthenticationCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);
	useAuthenticationSizer->AddSpacer(5);
	
	// create client key file controls
	groupBoxSizer->AddSpacer(10);
	wxBoxSizer* clientKeySizer = new wxBoxSizer(wxHORIZONTAL);
	groupBoxSizer->Add(clientKeySizer, 0, wxALIGN_TOP, 3);

	clientKeySizer->AddSpacer(30);
	wxStaticText* label1 = new wxStaticText(this, Config::CONNECT_SSL_CLIENT_KEY_LABEL_ID, S("ca-certificate"), wxDefaultPosition, { 100, -1 }, wxALIGN_LEFT);
	clientKeySizer->Add(label1, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	wxBoxSizer* rightSizer1 = new wxBoxSizer(wxHORIZONTAL);
	clientKeySizer->Add(rightSizer1, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	clientKeyEdit = new wxTextCtrl(this, Config::CONNECT_SSL_CLIENT_KEY_EDIT_ID, wxEmptyString, wxDefaultPosition, { 260, -1 }, wxALIGN_LEFT);
	rightSizer1->Add(clientKeyEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 0);

	rightSizer1->AddSpacer(10);
	clientKeyButton = new wxButton(this, Config::CONNECT_SSL_CLIENT_KEY_BUTTON_ID, _("..."), wxDefaultPosition, { 30, 20 });
	rightSizer1->Add(clientKeyButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 0);
	
	// create client certificate file controls
	groupBoxSizer->AddSpacer(10);
	wxBoxSizer* clientCertificateSizer = new wxBoxSizer(wxHORIZONTAL);
	groupBoxSizer->Add(clientCertificateSizer, 0, wxALIGN_TOP, 3);

	clientCertificateSizer->AddSpacer(30);
	wxStaticText* label2 = new wxStaticText(this, Config::CONNECT_SSL_CLIENT_CERTIFICATE_LABEL_ID , S("client-certificate"), wxDefaultPosition, { 100, -1 }, wxALIGN_LEFT);
	clientCertificateSizer->Add(label2, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 3);

	wxBoxSizer* rightSizer2 = new wxBoxSizer(wxHORIZONTAL);
	clientCertificateSizer->Add(rightSizer2, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	clientCertificateEdit = new wxTextCtrl(this, Config::CONNECT_SSL_CLIENT_CERTIFICATE_EDIT_ID, wxEmptyString, wxDefaultPosition, { 260, -1 }, wxALIGN_LEFT);
	rightSizer2->Add(clientCertificateEdit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 0);

	rightSizer2->AddSpacer(10);
	clientCertificateButton = new wxButton(this, Config::CONNECT_SSL_CLIENT_CERTIFICATE_BUTTON_ID, _("..."), wxDefaultPosition, { 30, 20 });
	rightSizer2->Add(clientCertificateButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 0);

	groupBoxSizer->AddSpacer(5);
}

void SslParamsPage::setEnableState()
{
	if (useSslEncryptCheckbox->GetValue()) {
		FindWindow(Config::CONNECT_SSL_CA_CERTIFICATE_LABEL_ID)->Enable();
		FindWindow(Config::CONNECT_SSL_CA_CERTIFICATE_BUTTON_ID)->Enable();
		FindWindow(Config::CONNECT_SSL_CIPHER_LABEL_ID)->Enable();

		caCertificateEdit->Enable();
		cipherEdit->Enable();
	} else {
		FindWindow(Config::CONNECT_SSL_CA_CERTIFICATE_LABEL_ID)->Disable();
		FindWindow(Config::CONNECT_SSL_CA_CERTIFICATE_BUTTON_ID)->Disable();
		FindWindow(Config::CONNECT_SSL_CIPHER_LABEL_ID)->Disable();

		caCertificateEdit->Disable();
		cipherEdit->Disable();
	}

	if (useAuthenticationCheckbox->GetValue() && useSslEncryptCheckbox->GetValue()) {
		FindWindow(Config::CONNECT_SSL_CLIENT_KEY_LABEL_ID)->Enable();
		FindWindow(Config::CONNECT_SSL_CLIENT_CERTIFICATE_LABEL_ID)->Enable();

		clientKeyEdit->Enable();
		clientKeyButton->Enable();
		clientCertificateEdit->Enable();
		clientCertificateButton->Enable();
	} else {
		FindWindow(Config::CONNECT_SSL_CLIENT_KEY_LABEL_ID)->Disable();
		FindWindow(Config::CONNECT_SSL_CLIENT_CERTIFICATE_LABEL_ID)->Disable();

		clientKeyEdit->Disable();
		clientKeyButton->Disable();
		clientCertificateEdit->Disable();
		clientCertificateButton->Disable();
	}
}

void SslParamsPage::OnClickUseSslEncryptCheckbox(wxCommandEvent& event)
{
	setEnableState();
	setRuntimeDataDirty(true);
}

void SslParamsPage::OnClickUseAuthenticationCheckbox(wxCommandEvent& event)
{
	setEnableState();
	setRuntimeDataDirty(true);
}

void SslParamsPage::OnClickSslCaCertificateButton(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, S("open-file"), "", "",
		"All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	wxString path = openFileDialog.GetPath();
	caCertificateEdit->SetValue(path);
}

void SslParamsPage::OnClickClientKeyButton(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, S("open-file"), "", "",
		"All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	wxString path = openFileDialog.GetPath();
	clientKeyEdit->SetValue(path);
}

void SslParamsPage::OnClickClientCertificateButton(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, S("open-file"), "", "",
		"All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	wxString path = openFileDialog.GetPath();
	clientCertificateEdit->SetValue(path);
}
