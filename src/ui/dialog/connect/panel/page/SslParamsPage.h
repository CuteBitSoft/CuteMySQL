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
 * @file   SslParamsPage.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-13
 *********************************************************************/
#pragma once
#include "ui/common/panel/QPanel.h"
#include "ui/dialog/connect/supplier/ConnectDialogSupplier.h"
class SslParamsPage :  public QPanel<ConnectDialogSupplier>
{
	DECLARE_EVENT_TABLE()
public:	
	SslParamsPage();

	void loadControls(); // virtual function
	void exchangeData(); // virtual function
private:
	wxCheckBox* useSslEncryptCheckbox;
	wxTextCtrl* caCertificateEdit;
	wxTextCtrl* cipherEdit;

	wxCheckBox* useAuthenticationCheckbox;
	wxTextCtrl* clientKeyEdit;
	wxButton* clientKeyButton;
	wxTextCtrl* clientCertificateEdit;
	wxButton* clientCertificateButton;

	virtual void init();
	virtual void createControls();

	// Use SSL Encryption Controls
	void createUseSslEncryptCtrls();
	void createUseAuthenticationGroup();

	void setEnableState();
	void OnClickUseSslEncryptCheckbox(wxCommandEvent& event);
	void OnClickUseAuthenticationCheckbox(wxCommandEvent& event);
	void OnClickSslCaCertificateButton(wxCommandEvent& event);
	void OnClickClientKeyButton(wxCommandEvent& event);
	void OnClickClientCertificateButton(wxCommandEvent& event);
};

