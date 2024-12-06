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
 * @file   DuplicateDatabaseDialog.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-05
 *********************************************************************/
#include "DuplicateDatabaseDialog.h"
#include "common/Config.h"
#include "common/AppContext.h"

BEGIN_EVENT_TABLE(DuplicateDatabaseDialog, wxDialog)
	EVT_BUTTON(wxID_OK, OnClickOkButton)
	EVT_COMBOBOX(Config::DUPLICATE_SOURCE_CONNECT_COMBOBOX_ID, OnSelChangeConnectCombobox)
END_EVENT_TABLE()

DuplicateDatabaseDialog::DuplicateDatabaseDialog() : QFormDialog()
{
}

void DuplicateDatabaseDialog::createInputs()
{
	// top layout
	topHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(topHoriLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	// center1 layout
	center1HoriLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(center1HoriLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1HoriLayout->AddSpacer(20);
	center1LeftVertLayout = new wxBoxSizer(wxVERTICAL);
	center1HoriLayout->Add(center1LeftVertLayout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	center1HoriLayout->AddSpacer(40);
	center1RightVertLayout = new wxBoxSizer(wxVERTICAL);
	center1HoriLayout->Add(center1RightVertLayout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	center1HoriLayout->AddSpacer(20);

	// center2 layout
	center2HoriLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(center2HoriLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center2HoriLayout->AddSpacer(20);
	center2LeftVertLayout = new wxBoxSizer(wxVERTICAL);
	center2HoriLayout->Add(center2LeftVertLayout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	center2HoriLayout->AddSpacer(20);
	center2RightVertLayout = new wxBoxSizer(wxVERTICAL);
	center2HoriLayout->Add(center2RightVertLayout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	center2HoriLayout->AddSpacer(20);

	// bottom
	bottomHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(bottomHoriLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	createTopControls();
	createCenter1Inputs();
	createBottomInputs();
}

void DuplicateDatabaseDialog::createTopControls()
{
	topHoriLayout->AddSpacer(20);

	auto imgdir = ResourceUtil::getProductImagesDir();
	wxString imgpath(imgdir);
	imgpath.Append("/dialog/duplicate/duplicate.bmp");
	wxBitmap bitmap(imgpath, wxBITMAP_TYPE_BMP);

	image = new wxStaticBitmap(this, wxID_ANY, wxBitmapBundle(bitmap), wxDefaultPosition, { 32, 32 }, wxNO_BORDER);
	topHoriLayout->Add(image, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	topHoriLayout->AddSpacer(10);
	label = new wxStaticText(this, wxID_ANY, S("duplicate-database-description"), wxDefaultPosition, {400, -1}, wxALIGN_LEFT);
	topHoriLayout->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
}

void DuplicateDatabaseDialog::createCenter1Inputs()
{
	// Center left
	auto sourceConnectLabel = new wxStaticText(this, wxID_ANY, S("source-connection"), wxDefaultPosition, {180, -1}, wxALIGN_LEFT);
	center1LeftVertLayout->Add(sourceConnectLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1LeftVertLayout->AddSpacer(10);
	sourceConnectEdit = new wxTextCtrl(this, Config::DUPLICATE_SOURCE_CONNECT_EDIT_ID,  S("source-connection"), wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT | wxTE_READONLY);
	center1LeftVertLayout->Add(sourceConnectEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1LeftVertLayout->AddSpacer(10);
	auto sourceDatabaseLabel = new wxStaticText(this, wxID_ANY, S("source-database"), wxDefaultPosition, {180, -1}, wxALIGN_LEFT);
	center1LeftVertLayout->Add(sourceDatabaseLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1LeftVertLayout->AddSpacer(10);
	sourceDatabaseEdit = new wxTextCtrl(this, Config::DUPLICATE_SOURCE_DATABASE_EDIT_ID,  S("source-database"), wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT | wxTE_READONLY);
	center1LeftVertLayout->Add(sourceDatabaseEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	// center right
	auto targetConnectLabel = new wxStaticText(this, wxID_ANY, S("target-connection"), wxDefaultPosition, {180, -1}, wxALIGN_LEFT);
	center1RightVertLayout->Add(targetConnectLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1RightVertLayout->AddSpacer(10);
	targetConnectComboBox = new wxBitmapComboBox(this, Config::DUPLICATE_TARGET_CONNECT_COMBOBOX_ID, wxEmptyString, wxDefaultPosition,
		{180, -1}, wxArrayString(), wxNO_BORDER | wxCLIP_CHILDREN| wxCB_READONLY);
	center1RightVertLayout->Add(targetConnectComboBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1RightVertLayout->AddSpacer(10);
	auto targetDatabaseLabel = new wxStaticText(this, wxID_ANY, S("target-database"), wxDefaultPosition, {180, -1}, wxALIGN_LEFT);
	center1RightVertLayout->Add(targetDatabaseLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1RightVertLayout->AddSpacer(10);
	targetDatabaseEdit = new wxTextCtrl(this, Config::DUPLICATE_TARGET_DATABASE_EDIT_ID,  S("target-database"), wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT | wxTE_READONLY);
	center1RightVertLayout->Add(targetDatabaseEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
}

void DuplicateDatabaseDialog::createCenter2Inputs()
{

}

void DuplicateDatabaseDialog::createBottomInputs()
{
}

void DuplicateDatabaseDialog::loadControls()
{
	/*
	delegate->loadForConnectComboBox(connectComboBox);

	auto nSelItem = connectComboBox->GetSelection();
	if (nSelItem == -1) {
		return;
	}
	auto data = reinterpret_cast<QClientData<UserConnect>*>(connectComboBox->GetClientObject(nSelItem));

	wxString newName = databaseSupplier->runtimeUserConnect->name;
	newName.Append(" (Copy)");
	duplcateNameEdit->SetValue(newName);
	duplcateNameEdit->SelectAll();
	duplcateNameEdit->SetFocus();*/
}

void DuplicateDatabaseDialog::OnSelChangeConnectCombobox(wxCommandEvent& event)
{
	if (targetConnectComboBox->GetSelection() ==  wxNOT_FOUND) {
		return;
	}
	auto connectData = (QClientData<UserConnect> *)targetConnectComboBox->GetClientObject(targetConnectComboBox->GetSelection());
	if (!connectData) {
		return;
	}

	
}

void DuplicateDatabaseDialog::OnClickOkButton(wxCommandEvent& event)
{
	
	EndModal(wxID_OK);
}
