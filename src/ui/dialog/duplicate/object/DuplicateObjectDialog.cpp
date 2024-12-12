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
 * @file   DuplicateObjectDialog.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-05
 *********************************************************************/
#include "DuplicateObjectDialog.h"
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <spdlog/fmt/fmt.h>
#include "common/Config.h"
#include "common/AppContext.h"
#include "utils/ResourceUtil.h"
#include "utils/StringUtil.h"
#include "core/entity/Enum.h"

BEGIN_EVENT_TABLE(DuplicateObjectDialog, wxDialog)
	EVT_BUTTON(wxID_OK, OnClickOkButton)
	EVT_COMBOBOX(Config::DUPLICATE_SOURCE_CONNECT_COMBOBOX_ID, OnSelChangeConnectCombobox)
END_EVENT_TABLE()

DuplicateObjectDialog::DuplicateObjectDialog(DuplicateObjectType _dupObjectType) : QFormDialog(),dupObjectType(_dupObjectType)
{
	init();	
}

void DuplicateObjectDialog::init()
{
	exportProcess = nullptr;
	importProcess = nullptr;

	databaseSupplier = DatabaseSupplier::getInstance();
	databaseService = DatabaseService::getInstance();
	connectService = ConnectService::getInstance();
	metadataService = MetadataService::getInstance();

	if (databaseSupplier->runtimeUserConnect) {
		userConnect = *databaseSupplier->runtimeUserConnect;
	}
	if (databaseSupplier->runtimeUserDb) {
		userDb = *databaseSupplier->runtimeUserDb;
	}
	if (dupObjectType == DuplicateObjectType::DUPLICATE_VIEW) {
		userView = *databaseSupplier->runtimeUserView;
		caption = S("duplicate-view");
		description = S("duplicate-view-description");
		sourceLabelText = S("source-view");
		targetLabelText = S("target-view");
		sourceObjectName = userView.name;
		try {
			userView.ddl = metadataService->getUserViewDDL(userConnect.id, userDb.name, userView.name);
		} catch (QRuntimeException& ex) {
			QAnimateBox::error(ex);
			return;
		}		
	} else if (dupObjectType == DuplicateObjectType::DUPLICATE_STORE_PROCEDURE) {
		userRoutine = *databaseSupplier->runtimeUserRoutine;
		caption = S("duplicate-store-procedure");
		description = S("duplicate-store-procedure-description");
		sourceLabelText = S("source-store-procedure");
		targetLabelText = S("target-store-procedure");
		sourceObjectName = userRoutine.name;
	} else if (dupObjectType == DuplicateObjectType::DUPLICATE_FUNCTION) {
		userRoutine = *databaseSupplier->runtimeUserRoutine;
		caption = S("duplicate-function");
		description = S("duplicate-function-description");
		sourceLabelText = S("source-function");
		targetLabelText = S("target-function");
		sourceObjectName = userRoutine.name;
	} else if (dupObjectType == DuplicateObjectType::DUPLICATE_TRIGGER) {
		userTrigger = *databaseSupplier->runtimeUserTrigger;
		caption = S("duplicate-trigger");
		description = S("duplicate-trigger-description");
		sourceLabelText = S("source-trigger");
		targetLabelText = S("target-trigger");
		sourceObjectName = userTrigger.name;
	} else if (dupObjectType == DuplicateObjectType::DUPLICATE_EVENT) {
		userEvent = *databaseSupplier->runtimeUserEvent;
		caption = S("duplicate-event");
		description = S("duplicate-event-description");
		sourceLabelText = S("source-event");
		targetLabelText = S("target-event");
		sourceObjectName = userEvent.name;
	}
	
	
}

void DuplicateObjectDialog::createInputs()
{
	SetLabel(caption);

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
	center2VertLayout = new wxBoxSizer(wxVERTICAL);
	tLayout->Add(center2VertLayout, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	
	tLayout->AddSpacer(20);
	// bottom
	bottomHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	tLayout->Add(bottomHoriLayout, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	createTopControls();
	createCenter1Inputs();
	createCenter2Inputs();
	createBottomInputs();
}

void DuplicateObjectDialog::createTopControls()
{
	topHoriLayout->AddSpacer(20);

	auto imgdir = ResourceUtil::getProductImagesDir();
	wxString imgpath(imgdir);
	imgpath.Append("/dialog/duplicate/duplicate.bmp");
	wxBitmap bitmap(imgpath, wxBITMAP_TYPE_BMP);

	image = new wxStaticBitmap(this, wxID_ANY, wxBitmapBundle(bitmap), wxDefaultPosition, { 32, 32 }, wxNO_BORDER);
	topHoriLayout->Add(image, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	topHoriLayout->AddSpacer(10);
	label = new wxStaticText(this, wxID_ANY, description, wxDefaultPosition, {400, -1}, wxALIGN_LEFT);
	topHoriLayout->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
}

void DuplicateObjectDialog::createCenter1Inputs()
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
	auto sourceObjectLabel = new wxStaticText(this, wxID_ANY, sourceLabelText, wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT);
	center1LeftVertLayout->Add(sourceObjectLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1LeftVertLayout->AddSpacer(10);
	sourceObjectEdit = new wxTextCtrl(this, Config::DUPLICATE_SOURCE_OBJECT_EDIT_ID, wxEmptyString, wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT | wxTE_READONLY);
	center1LeftVertLayout->Add(sourceObjectEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
	sourceObjectEdit->SetBackgroundColour(disabledColor);
	sourceObjectEdit->SetForegroundColour(textColor);

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
	auto targetObjectLabel = new wxStaticText(this, wxID_ANY, targetLabelText, wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT);
	center1RightVertLayout->Add(targetObjectLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);

	center1RightVertLayout->AddSpacer(10);
	targetObjectEdit = new wxTextCtrl(this, Config::DUPLICATE_TARGET_OBJECT_EDIT_ID, wxEmptyString, wxDefaultPosition,
		{ 180, -1 }, wxCLIP_CHILDREN | wxALIGN_LEFT);
	center1RightVertLayout->Add(targetObjectEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
}

void DuplicateObjectDialog::createCenter2Inputs()
{
	// center2 left  - ddl preview
	auto ddlPreviewLabel = new wxStaticText(this, wxID_ANY, S("ddl-preview"), wxDefaultPosition, {180, -1}, wxALIGN_LEFT);
	center2VertLayout->Add(ddlPreviewLabel, 0, wxALIGN_LEFT | wxALIGN_TOP, 5);

	center2VertLayout->AddSpacer(10);

	ddlPreviewEdit = new wxStyledTextCtrl(this, Config::DUPLICATE_DDL_PREVIEW_EDIT_ID, wxDefaultPosition,
		{ 400, 200 }, wxCLIP_CHILDREN | wxALIGN_LEFT);
	center2VertLayout->Add(ddlPreviewEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
}

void DuplicateObjectDialog::createBottomInputs()
{
	progressbar = new QProgressBar(this, wxID_ANY, wxDefaultPosition, { 400, 20 });
	bottomHoriLayout->Add(progressbar, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTRE);
}

void DuplicateObjectDialog::loadControls()
{
	assert(userConnect.id && !userDb.name.empty() && !sourceObjectName.empty());
	
	okButton->SetLabel(S("start"));
	cancelButton->SetLabel(S("close"));

	wxString connnectName = userConnect.name;
	connnectName.Append(" [").Append(userConnect.host)
		.Append(":").Append(std::to_string(userConnect.port)).Append("]");
	sourceConnectEdit->SetValue(connnectName);
	sourceDatabaseEdit->SetValue(userDb.name);
	sourceObjectEdit->SetValue(sourceObjectName);

	delegate->loadForConnectComboBox(targetConnectComboBox);
	delegate->loadForSchemaComboBox(targetDatabaseComboBox,
		userConnect.id,
		userDb.name);
	wxString targetObjectName = sourceObjectName;
	targetObjectName.Append(" (copy)");
	targetObjectEdit->SetValue(targetObjectName);
	targetObjectEdit->SelectAll();
	targetObjectEdit->SetFocus();

	// preview ddl
	ddlPreviewEdit->SetText(userView.ddl);
}

void DuplicateObjectDialog::OnSelChangeConnectCombobox(wxCommandEvent& event)
{
	if (targetConnectComboBox->GetSelection() ==  wxNOT_FOUND) {
		return;
	}
	auto connectData = (QClientData<UserConnect> *)targetConnectComboBox->GetClientObject(targetConnectComboBox->GetSelection());
	if (!connectData) {
		return;
	}
}

void DuplicateObjectDialog::OnClickOkButton(wxCommandEvent& event)
{
	progressbar->reset();
	auto tmpdir = ResourceUtil::getStdProductTmpDir();
	tmpSqlPath = tmpdir + "/duplicate_table_" + DateUtil::getCurrentDateTime("%Y%m%d_%H%M%S") + ".sql";
	
	auto targetSchema = targetDatabaseComboBox->GetValue();
	auto targetObject = targetObjectEdit->GetValue();
	if (targetObject.empty()) {
		QAnimateBox::error(S("no-target-object"));
		targetObjectEdit->SelectAll();
		targetObjectEdit->SetFocus();
		return;
	}
	try {
		auto nSelItem = targetConnectComboBox->GetSelection();
		auto data = reinterpret_cast<QClientData<UserConnect> *>(targetConnectComboBox->GetClientObject(nSelItem));
		if (metadataService->hasUserObject(data->getDataPtr()->id, targetSchema.ToStdString(), dupObjectType , targetObject.ToStdString())) {
			QAnimateBox::error(S("exists-target-object"));
			return;
		}
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
		return;
	}
	okButton->Disable();
	exportTableToTmp(tmpSqlPath);
}


/**
 * Export sql statements to specified template file.
 * 
 * @param tmpSqlPath - expert sql statements to template file path
 * @return 
 */
bool DuplicateObjectDialog::exportTableToTmp(const std::string& tmpSqlPath)
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
		.append(" --tables \"").append(userView.name).append("\"");
	
	
	cmd.append(" --result-file=\"").append(tmpSqlPath).append("\"");
	exportProcess = new QProcess<DuplicateObjectDialog>(this, cmd);
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

bool DuplicateObjectDialog::replaceDatabaseAndTableInTmp()
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

	std::string sourceTableName("`" + sourceObjectEdit->GetValue().ToStdString() + "`");
	std::string targetTableName("`" + targetObjectEdit->GetValue().ToStdString() + "`");

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

bool DuplicateObjectDialog::importTableFromTmp(const std::string& tmpSqlPath)
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
	auto objectName = targetObjectEdit->GetValue();

	std::string cmd(executePath);
	cmd.append(" -h").append(userConnectPtr->host)
		.append(" -P").append(std::to_string(userConnectPtr->port))
		.append(" -u").append(userConnectPtr->userName)
		.append(" -p").append(userConnectPtr->password)
		.append(" --default-character-set=utf8")
		.append(" --database ").append(schema)
		.append(" -e \"source ").append(tmpSqlPath).append("\"");
	importProcess = new QProcess<DuplicateObjectDialog>(this, cmd);
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

DuplicateObjectDialog::~DuplicateObjectDialog()
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

void DuplicateObjectDialog::OnAsyncProcessTermination(QProcess<DuplicateObjectDialog>* process)
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

void DuplicateObjectDialog::afterDuplicated()
{
	auto connectData = (QClientData<UserConnect> *)targetConnectComboBox->GetClientObject(targetConnectComboBox->GetSelection());
	if (!connectData) {
		return;
	}

	databaseSupplier->handleUserDb = userDb;
	databaseSupplier->handleUserDb.connectId = connectData->getDataPtr()->id;
	databaseSupplier->handleUserDb.name = targetDatabaseComboBox->GetValue().ToStdString();

	databaseSupplier->handleUserTable.schema = databaseSupplier->handleUserDb.name;
	databaseSupplier->handleUserTable.name = targetObjectEdit->GetValue().ToStdString();
	AppContext::getInstance()->dispatch(Config::MSG_NEW_TABLE_ID); 
}
