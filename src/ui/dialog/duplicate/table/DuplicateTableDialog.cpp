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
 * @file   DuplicateTableDialog.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-05
 *********************************************************************/
#include "DuplicateTableDialog.h"
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <spdlog/fmt/fmt.h>
#include "common/Config.h"
#include "common/AppContext.h"
#include "utils/ResourceUtil.h"
#include "utils/StringUtil.h"

BEGIN_EVENT_TABLE(DuplicateTableDialog, wxDialog)
	EVT_BUTTON(wxID_OK, OnClickOkButton)
	EVT_COMBOBOX(Config::DUPLICATE_SOURCE_CONNECT_COMBOBOX_ID, OnSelChangeConnectCombobox)
	EVT_CHECKBOX(Config::STRUCTURE_ONLY_CHECKBOX_ID, OnStructAndDataCheckBoxChecked)
	EVT_CHECKBOX(Config::STRUCTURE_DATA_CHECKBOX_ID, OnStructAndDataCheckBoxChecked)
END_EVENT_TABLE()

DuplicateTableDialog::DuplicateTableDialog() : QFormDialog()
{
	databaseSupplier = DatabaseSupplier::getInstance();
	if (databaseSupplier->runtimeUserConnect) {
		userConnect = *databaseSupplier->runtimeUserConnect;
	}
	if (databaseSupplier->runtimeUserDb) {
		userDb = *databaseSupplier->runtimeUserDb;
	}
	if (databaseSupplier->runtimeUserTable) {
		userTable = *databaseSupplier->runtimeUserTable;
	}
	
	exportProcess = nullptr;
	importProcess = nullptr;
}

void DuplicateTableDialog::createInputs()
{
	// top layout
	topHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(topHoriLayout, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	tLayout->AddSpacer(20);
	// center1 layout
	center1HoriLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(center1HoriLayout, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1HoriLayout->AddSpacer(20);
	center1LeftVertLayout = new wxBoxSizer(wxVERTICAL);
	center1HoriLayout->Add(center1LeftVertLayout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	center1HoriLayout->AddSpacer(40);
	center1RightVertLayout = new wxBoxSizer(wxVERTICAL);
	center1HoriLayout->Add(center1RightVertLayout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	center1HoriLayout->AddSpacer(20);

	tLayout->AddSpacer(20);
	// center2 layout
	center2HoriLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(center2HoriLayout, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center2HoriLayout->AddSpacer(20);
	center2LeftVertLayout = new wxBoxSizer(wxVERTICAL);
	center2HoriLayout->Add(center2LeftVertLayout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	center2HoriLayout->AddSpacer(20);	
	center2RightVertLayout = new wxBoxSizer(wxVERTICAL);
	center2HoriLayout->Add(center2RightVertLayout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	auto dulicateSettingsBox = new wxStaticBox(this, wxID_ANY, S("duplicate-settings"), wxDefaultPosition, {200, -1});
	dulicateSettingsVertLayout = new wxStaticBoxSizer(dulicateSettingsBox, wxVERTICAL);
	center2LeftVertLayout->Add(dulicateSettingsVertLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP);
	center2LeftVertLayout->AddSpacer(10);

	auto lockTablesBox = new wxStaticBox(this, wxID_ANY, S("lock-settings"), wxDefaultPosition, { 200, -1 });
	lockSettingsVertLayout = new wxStaticBoxSizer(lockTablesBox, wxVERTICAL);
	center2RightVertLayout->Add(lockSettingsVertLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP);

	center2HoriLayout->AddSpacer(20); 

	tLayout->AddSpacer(20);
	// bottom
	bottomHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(bottomHoriLayout, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	createTopControls();
	createCenter1Inputs();
	createCenter2Inputs();
	createBottomInputs();
}

void DuplicateTableDialog::createTopControls()
{
	topHoriLayout->AddSpacer(20);

	auto imgdir = ResourceUtil::getProductImagesDir();
	wxString imgpath(imgdir);
	imgpath.Append("/dialog/duplicate/duplicate.bmp");
	wxBitmap bitmap(imgpath, wxBITMAP_TYPE_BMP);

	image = new wxStaticBitmap(this, wxID_ANY, wxBitmapBundle(bitmap), wxDefaultPosition, { 32, 32 }, wxNO_BORDER);
	topHoriLayout->Add(image, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	topHoriLayout->AddSpacer(10);
	label = new wxStaticText(this, wxID_ANY, S("duplicate-table-description"), wxDefaultPosition, {400, -1}, wxALIGN_LEFT);
	topHoriLayout->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
}

void DuplicateTableDialog::createCenter1Inputs()
{
	// Center left
	auto sourceConnectLabel = new wxStaticText(this, wxID_ANY, S("source-connection"), wxDefaultPosition, {180, -1}, wxALIGN_LEFT);
	center1LeftVertLayout->Add(sourceConnectLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1LeftVertLayout->AddSpacer(10);
	sourceConnectEdit = new wxTextCtrl(this, Config::DUPLICATE_SOURCE_CONNECT_EDIT_ID,  wxEmptyString, wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT | wxTE_READONLY);
	center1LeftVertLayout->Add(sourceConnectEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
	sourceConnectEdit->SetBackgroundColour(disabledColor);
	sourceConnectEdit->SetForegroundColour(textColor);

	center1LeftVertLayout->AddSpacer(10);
	auto sourceDatabaseLabel = new wxStaticText(this, wxID_ANY, S("source-database"), wxDefaultPosition, {180, -1}, wxALIGN_LEFT);
	center1LeftVertLayout->Add(sourceDatabaseLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1LeftVertLayout->AddSpacer(10);
	sourceDatabaseEdit = new wxTextCtrl(this, Config::DUPLICATE_SOURCE_DATABASE_EDIT_ID,  wxEmptyString, wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT | wxTE_READONLY);
	center1LeftVertLayout->Add(sourceDatabaseEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
	sourceDatabaseEdit->SetBackgroundColour(disabledColor);
	sourceDatabaseEdit->SetForegroundColour(textColor);

	center1LeftVertLayout->AddSpacer(10);
	auto sourceTableLabel = new wxStaticText(this, wxID_ANY, S("source-table"), wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT);
	center1LeftVertLayout->Add(sourceTableLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1LeftVertLayout->AddSpacer(10);
	sourceTableEdit = new wxTextCtrl(this, Config::DUPLICATE_SOURCE_TABLE_EDIT_ID, wxEmptyString, wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT | wxTE_READONLY);
	center1LeftVertLayout->Add(sourceTableEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
	sourceTableEdit->SetBackgroundColour(disabledColor);
	sourceTableEdit->SetForegroundColour(textColor);

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
	targetDatabaseComboBox = new wxBitmapComboBox(this, Config::DUPLICATE_TARGET_DATABASE_COMBOBOX_ID, wxEmptyString, wxDefaultPosition,
		{ 180, -1 }, wxArrayString(), wxNO_BORDER | wxCLIP_CHILDREN | wxCB_READONLY);
	center1RightVertLayout->Add(targetDatabaseComboBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1RightVertLayout->AddSpacer(10);
	auto targetTableLabel = new wxStaticText(this, wxID_ANY, S("target-table"), wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT);
	center1RightVertLayout->Add(targetTableLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1RightVertLayout->AddSpacer(10);
	targetTableEdit = new wxTextCtrl(this, Config::DUPLICATE_TARGET_TABLE_EDIT_ID, wxEmptyString, wxDefaultPosition,
		{ 180, -1 }, wxCLIP_CHILDREN | wxALIGN_LEFT);
	center1RightVertLayout->Add(targetTableEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
}

void DuplicateTableDialog::createCenter2Inputs()
{
	// center2 left  - duplicate settings
	structOnlyCheckBox = new wxCheckBox(this, Config::STRUCTURE_ONLY_CHECKBOX_ID, S("structure-only"), wxDefaultPosition, wxDefaultSize);
	dulicateSettingsVertLayout->Add(structOnlyCheckBox, 0, wxALIGN_LEFT | wxALIGN_TOP, 5);
	dulicateSettingsVertLayout->AddSpacer(10);
	structAndDataCheckBox = new wxCheckBox(this, Config::STRUCTURE_DATA_CHECKBOX_ID, S("structure-and-data"), wxDefaultPosition, wxDefaultSize);
	dulicateSettingsVertLayout->Add(structAndDataCheckBox, 0, wxALIGN_LEFT | wxALIGN_TOP, 5);

	// center2 left  - duplicate settings
	lockTablesCheckBox = new wxCheckBox(this, Config::LOCK_TABLES_CHECKBOX_ID, S("lock-tables"), wxDefaultPosition, wxDefaultSize);
	lockSettingsVertLayout->AddSpacer(10);
	lockSettingsVertLayout->Add(lockTablesCheckBox, 0, wxALIGN_LEFT | wxALIGN_TOP, 5);
	lockSettingsVertLayout->AddSpacer(20);
}

void DuplicateTableDialog::createBottomInputs()
{
	progressbar = new QProgressBar(this, wxID_ANY, wxDefaultPosition, { 400, 20 });
	bottomHoriLayout->Add(progressbar, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTRE);
}

void DuplicateTableDialog::loadControls()
{
	assert(userConnect.id && !userDb.name.empty() && !userTable.name.empty());
	
	okButton->SetLabel(S("start"));
	cancelButton->SetLabel(S("close"));

	wxString connnectName = userConnect.name;
	connnectName.Append(" [").Append(userConnect.host)
		.Append(":").Append(std::to_string(userConnect.port)).Append("]");
	sourceConnectEdit->SetValue(connnectName);
	sourceDatabaseEdit->SetValue(userDb.name);
	sourceTableEdit->SetValue(userTable.name);

	delegate->loadForConnectComboBox(targetConnectComboBox);
	delegate->loadForSchemaComboBox(targetDatabaseComboBox,
		userConnect.id,
		userDb.name);
	wxString targetTableName = userTable.name;
	targetTableName.Append(" (copy)");
	targetTableEdit->SetValue(targetTableName);
	targetTableEdit->SelectAll();
	targetTableEdit->SetFocus();


	structOnlyCheckBox->SetValue(true);
	lockTablesCheckBox->SetValue(true);
}

void DuplicateTableDialog::OnSelChangeConnectCombobox(wxCommandEvent& event)
{
	if (targetConnectComboBox->GetSelection() ==  wxNOT_FOUND) {
		return;
	}
	auto connectData = (QClientData<UserConnect> *)targetConnectComboBox->GetClientObject(targetConnectComboBox->GetSelection());
	if (!connectData) {
		return;
	}

	
}

void DuplicateTableDialog::OnClickOkButton(wxCommandEvent& event)
{
	progressbar->reset();
	auto tmpdir = ResourceUtil::getStdProductTmpDir();
	tmpSqlPath = tmpdir + "/duplicate_table_" + DateUtil::getCurrentDateTime("%Y%m%d_%H%M%S") + ".sql";
	bool isStructOnly = structOnlyCheckBox->GetValue();
	auto targetSchema = targetDatabaseComboBox->GetValue();
	auto targetTable = targetTableEdit->GetValue();
	if (targetTable.empty()) {
		QAnimateBox::error(S("no-target-table"));
		targetTableEdit->SelectAll();
		targetTableEdit->SetFocus();
		return;
	}
	try {
		auto nSelItem = targetConnectComboBox->GetSelection();
		auto data = reinterpret_cast<QClientData<UserConnect> *>(targetConnectComboBox->GetClientObject(nSelItem));
		if (metadataService->hasUserTable(data->getDataPtr()->id, targetSchema.ToStdString(), targetTable.ToStdString())) {
			QAnimateBox::error(S("exists-target-table"));
			targetTableEdit->SelectAll();
			targetTableEdit->SetFocus();
			return;
		}
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
		return;
	}
	okButton->Disable();
	exportTableToTmp(tmpSqlPath);
}

void DuplicateTableDialog::OnStructAndDataCheckBoxChecked(wxCommandEvent& event)
{
	auto id = event.GetId();
	auto item = reinterpret_cast<wxCheckBox *>(FindItem(id));
	bool state = item->GetValue();
	if (id == Config::STRUCTURE_ONLY_CHECKBOX_ID) {
		structAndDataCheckBox->SetValue(!state);
	} else if (id == Config::STRUCTURE_DATA_CHECKBOX_ID) {
		structOnlyCheckBox->SetValue(!state);
	}
}


/**
 * Export sql statements to specified template file.
 * 
 * @param tmpSqlPath - expert sql statements to template file path
 * @return 
 */
bool DuplicateTableDialog::exportTableToTmp(const std::string& tmpSqlPath)
{
	std::string productDir = ResourceUtil::getStdProductBinDir();
	std::string executePath = productDir + "/mysqldump.exe";
	if (_access(executePath.c_str(), 0) != 0) {
		Q_ERROR("file not found. Path:{}", executePath);
		okButton->Enable();
		return false;
	}

	std::string cmd(executePath);
	cmd.append(" -h").append(userConnect.host)
		.append(" -P").append(std::to_string(userConnect.port))
		.append(" -u").append(userConnect.userName)
		.append(" -p").append(userConnect.password)
		.append(" --no-create-db ")
		.append(" --default-character-set=utf8")
		.append(" --databases \"").append(userDb.name).append("\"")
		.append(" --tables \"").append(userTable.name).append("\"");
	
	

	bool isStuctOnly = structOnlyCheckBox->GetValue();
	bool isLockTables = lockTablesCheckBox->GetValue();
	if (isStuctOnly) {
		cmd.append(" --no-data");
	}
	if (isLockTables) {
		cmd.append(" --lock-tables");
	}
	cmd.append(" --result-file=\"").append(tmpSqlPath).append("\"");
	exportProcess = new QProcess<DuplicateTableDialog>(this, cmd);
	progressbar->run(5);
	if (!exportProcess->open()) {
		Q_ERROR("Failed to launch the command.");
		okButton->Enable();
		return false;
	}
	
	if (!exportProcess) {
		Q_ERROR("Failed to launch the command.");
		okButton->Enable();
		return false;
	}
	Q_INFO("PID of the new process: {}", exportProcess->GetPid());
	progressbar->run(10);
	return true;
}

bool DuplicateTableDialog::replaceDatabaseAndTableInTmp()
{
	if (_access(tmpSqlPath.c_str(), 0) != 0) {
		auto err = fmt::format("file not found. Path:{}", tmpSqlPath);
		Q_ERROR(err);
		QAnimateBox::error(err);
		return false;
	}
	std::string newTmpPath = tmpSqlPath + ".tmp";
	std::ifstream fin;
	std::ofstream fout;
	fin.open(tmpSqlPath.c_str(), std::ios_base::in);
	fout.open(newTmpPath.c_str(), std::ios_base::out);

	std::string sourceDbName("`" + userDb.name + "`");
	std::string targetDbName("`" + targetDatabaseComboBox->GetValue().ToStdString() + "`");

	std::string sourceTableName("`" + sourceTableEdit->GetValue().ToStdString() + "`");
	std::string targetTableName("`" + targetTableEdit->GetValue().ToStdString() + "`");

	std::string line;
	while (std::getline(fin, line)) {
		line = StringUtil::replace(line, sourceDbName, targetDbName);
		line = StringUtil::replace(line, sourceTableName, targetTableName);
		fout << line << std::endl;
	}
	fout.close();
	fin.close();

	// delete original file
	_unlink(tmpSqlPath.c_str());

	// rename new temp file
	rename(newTmpPath.c_str(), tmpSqlPath.c_str());
	Q_INFO("rename file. original:{}, target:{}", newTmpPath, tmpSqlPath);

	return true;
}

bool DuplicateTableDialog::importTableFromTmp(const std::string& tmpSqlPath)
{
	std::string productDir = ResourceUtil::getStdProductBinDir();
	std::string executePath = productDir + "/mysql.exe";
	if (_access(executePath.c_str(), 0) != 0) {
		Q_ERROR("file not found. Path:{}", executePath);
		okButton->Enable();
		return false;
	}

	auto nSelItem = targetConnectComboBox->GetSelection();
	auto data = reinterpret_cast<QClientData<UserConnect> *>(targetConnectComboBox->GetClientObject(nSelItem));
	auto userConnectPtr = data->getDataPtr();
	auto schema = targetDatabaseComboBox->GetValue();
	auto table = targetTableEdit->GetValue();

	std::string cmd(executePath);
	cmd.append(" -h").append(userConnectPtr->host)
		.append(" -P").append(std::to_string(userConnectPtr->port))
		.append(" -u").append(userConnectPtr->userName)
		.append(" -p").append(userConnectPtr->password)
		.append(" --default-character-set=utf8")
		.append(" --database ").append(schema)
		.append(" -e \"source ").append(tmpSqlPath).append("\"");
	importProcess = new QProcess<DuplicateTableDialog>(this, cmd);
	progressbar->run(65);
	if (!importProcess->open()) {
		Q_ERROR("Failed to launch the command.");
		okButton->Enable();
		return false;
	}
	
	if (!importProcess) {
		Q_ERROR("Failed to launch the command.");
		okButton->Enable();
		return false;
	}
	return true;
}

DuplicateTableDialog::~DuplicateTableDialog()
{
	if (exportProcess) {
		wxProcess::Kill(exportProcess->GetPid());
		delete exportProcess;
		exportProcess = nullptr;
	}

	if (importProcess) {
		wxProcess::Kill(importProcess->GetPid());
		delete importProcess;
		importProcess = nullptr;
	}

	// delete the temp sql file
	if (!tmpSqlPath.empty() && _access(tmpSqlPath.c_str(), 0) == 0) {
		_unlink(tmpSqlPath.c_str());
	}
}

void DuplicateTableDialog::OnAsyncProcessTermination(QProcess<DuplicateTableDialog>* process)
{
	if (exportProcess  && process->GetPid() == exportProcess->GetPid()) {
		// 1.ending export process
		progressbar->run(50);

		auto msg = process->getOutputText();
		if (!msg.empty()) {
			QAnimateBox::notice(msg);
		}
			
		auto error = process->getOutputError();
		if (error.empty() == false) {
			QAnimateBox::error(error);
			progressbar->error("Dumping failed.");
			okButton->Enable();
			delete exportProcess;
			exportProcess = nullptr;
			return;
		}

		QAnimateBox::success("Export success! Pid:" + std::to_string(process->GetPid()));		
		
		delete exportProcess;
		exportProcess = nullptr;
		// 2.replace database name in the templace sql file;
		if (!replaceDatabaseAndTableInTmp()) {
			okButton->Enable();
			return;
		}
		progressbar->run(60);

		// 3.import sql file to new table when ending export process
		importTableFromTmp(tmpSqlPath);

	} else if (importProcess && process->GetPid() == importProcess->GetPid()) {
		progressbar->run(80);

		auto msg = process->getOutputText();
		if (!msg.empty()) {
			QAnimateBox::notice(msg);
		}
			
		auto error = process->getOutputError();
		if (error.empty() == false) {
			QAnimateBox::error(error);
			progressbar->error("Import Failed.");
		} else {
			QAnimateBox::success("Import success! Pid:" + std::to_string(process->GetPid()));
			progressbar->run(100);
		}
		
		okButton->Enable();
		delete importProcess;
		importProcess = nullptr;

		// delete the temp sql file
		if (_access(tmpSqlPath.c_str(), 0) == 0) {
			_unlink(tmpSqlPath.c_str());
		}

		afterDuplicated();
	}
}

void DuplicateTableDialog::afterDuplicated()
{
	auto connectData = (QClientData<UserConnect> *)targetConnectComboBox->GetClientObject(targetConnectComboBox->GetSelection());
	if (!connectData) {
		return;
	}

	databaseSupplier->handleUserDb = userDb;
	databaseSupplier->handleUserDb.connectId = connectData->getDataPtr()->id;
	databaseSupplier->handleUserDb.name = targetDatabaseComboBox->GetValue().ToStdString();

	databaseSupplier->handleUserObject = userTable;
	databaseSupplier->handleUserObject.schema = databaseSupplier->handleUserDb.name;
	databaseSupplier->handleUserObject.name = targetTableEdit->GetValue().ToStdString();
	databaseSupplier->handleUserObject.type = "TABLE";

	AppContext::getInstance()->dispatch(Config::MSG_NEW_OBJECT_ID); 
}
