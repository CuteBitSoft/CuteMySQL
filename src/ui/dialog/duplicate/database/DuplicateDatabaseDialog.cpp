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
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <spdlog/fmt/fmt.h>
#include "common/Config.h"
#include "common/AppContext.h"
#include "utils/ResourceUtil.h"
#include "utils/StringUtil.h"

BEGIN_EVENT_TABLE(DuplicateDatabaseDialog, wxDialog)
	EVT_BUTTON(wxID_OK, OnClickOkButton)
	EVT_BUTTON(Config::SELECT_ALL_BUTTON_ID, OnClickSelectAllButton)
	EVT_BUTTON(Config::UN_SELECT_ALL_BUTTON_ID, OnClickUnSelectAllButton)
	EVT_COMBOBOX(Config::DUPLICATE_SOURCE_CONNECT_COMBOBOX_ID, OnSelChangeConnectCombobox)
	EVT_CHECKBOX(Config::STRUCTURE_ONLY_CHECKBOX_ID, OnStructAndDataCheckBoxChecked)
	EVT_CHECKBOX(Config::STRUCTURE_DATA_CHECKBOX_ID, OnStructAndDataCheckBoxChecked)
	EVT_TREELIST_ITEM_CHECKED(Config::DUPLICATE_DATABASE_TREELISTCTRL_ID, OnTreeListItemChecked)
END_EVENT_TABLE()

DuplicateDatabaseDialog::DuplicateDatabaseDialog() : QFormDialog()
{
	databaseSupplier = DatabaseSupplier::getInstance();
	if (databaseSupplier->runtimeUserConnect) {
		userConnect = *databaseSupplier->runtimeUserConnect;
	}
	
	if (databaseSupplier->runtimeUserDb) {
		userDb = *databaseSupplier->runtimeUserDb;
	}	

	exportProcess = nullptr;
	importProcess = nullptr;
}

void DuplicateDatabaseDialog::createInputs()
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
	auto center2LeftBox = new wxStaticBox(this, wxID_ANY, S("source-objects"), wxDefaultPosition, {200, -1});
	center2LeftVertLayout = new wxStaticBoxSizer(center2LeftBox, wxVERTICAL);
	center2HoriLayout->Add(center2LeftVertLayout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	center2HoriLayout->AddSpacer(20);	
	center2RightVertLayout = new wxBoxSizer(wxVERTICAL);
	center2HoriLayout->Add(center2RightVertLayout, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	auto dulicateSettingsBox = new wxStaticBox(this, wxID_ANY, S("duplicate-settings"), wxDefaultPosition, {200, -1});
	dulicateSettingsVertLayout = new wxStaticBoxSizer(dulicateSettingsBox, wxVERTICAL);
	center2RightVertLayout->Add(dulicateSettingsVertLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP);
	center2RightVertLayout->AddSpacer(10);

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
	targetDatabaseEdit = new wxTextCtrl(this, Config::DUPLICATE_TARGET_DATABASE_EDIT_ID,  S("target-database"), wxDefaultPosition, { 180, -1 }, wxALIGN_LEFT );
	center1RightVertLayout->Add(targetDatabaseEdit, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
}

void DuplicateDatabaseDialog::createCenter2Inputs()
{
	// center2 left  - source object treeListCtrl / select all /un select all 
	treeListCtrl = new wxTreeListCtrl(this, Config::DUPLICATE_DATABASE_TREELISTCTRL_ID, wxDefaultPosition, { 180, 250 }, wxTL_MULTIPLE | wxTL_CHECKBOX | wxTL_NO_HEADER);
	center2LeftVertLayout->Add(treeListCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
	//treeListCtrl->SetBackgroundColour(bkgColor);
	//treeListCtrl->SetForegroundColour(textColor);

	center2LeftVertLayout->AddSpacer(10);
	auto center2BottomHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	center2LeftVertLayout->Add(center2BottomHoriLayout, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP);

	center2BottomHoriLayout->AddSpacer(10);
	selectAllButton = new wxButton(this, Config::SELECT_ALL_BUTTON_ID, S("select-all"), wxDefaultPosition, { -1, 20 });
	center2BottomHoriLayout->Add(selectAllButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	center2BottomHoriLayout->AddSpacer(10);
	unSelectAllButton = new wxButton(this, Config::UN_SELECT_ALL_BUTTON_ID, S("un-select-all"), wxDefaultPosition, { -1, 20 });
	center2BottomHoriLayout->Add(unSelectAllButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

	// center2 RIGHT  - duplicate settings/lock settings
	structOnlyCheckBox = new wxCheckBox(this, Config::STRUCTURE_ONLY_CHECKBOX_ID, S("structure-only"), wxDefaultPosition, wxDefaultSize);
	dulicateSettingsVertLayout->Add(structOnlyCheckBox, 0, wxALIGN_LEFT | wxALIGN_TOP, 5);
	dulicateSettingsVertLayout->AddSpacer(10);
	structAndDataCheckBox = new wxCheckBox(this, Config::STRUCTURE_DATA_CHECKBOX_ID, S("structure-and-data"), wxDefaultPosition, wxDefaultSize);
	dulicateSettingsVertLayout->Add(structAndDataCheckBox, 0, wxALIGN_LEFT | wxALIGN_TOP, 5);

	lockTablesCheckBox = new wxCheckBox(this, Config::LOCK_TABLES_CHECKBOX_ID, S("lock-tables"), wxDefaultPosition, wxDefaultSize);
	lockSettingsVertLayout->AddSpacer(10);
	lockSettingsVertLayout->Add(lockTablesCheckBox, 0, wxALIGN_LEFT | wxALIGN_TOP, 5);
	lockSettingsVertLayout->AddSpacer(95);
}

void DuplicateDatabaseDialog::createBottomInputs()
{
	progressbar = new QProgressBar(this, wxID_ANY, wxDefaultPosition, { 400, 20 });
	bottomHoriLayout->Add(progressbar, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTRE);
	//progressbar->run(50);
}

void DuplicateDatabaseDialog::loadControls()
{
	assert(userConnect.id && !userDb.name.empty());
	okButton->SetLabel(S("start"));
	cancelButton->SetLabel(S("close"));

	wxString connnectName = userConnect.name;
	connnectName.Append(" [").Append(userConnect.host)
		.Append(":").Append(std::to_string(userConnect.port)).Append("]");
	sourceConnectEdit->SetValue(connnectName);
	sourceDatabaseEdit->SetValue(userDb.name);

	delegate->loadForConnectComboBox(targetConnectComboBox, userConnect.id);
	wxString targetDbName = userDb.name;
	targetDbName.Append(" (copy)");
	targetDatabaseEdit->SetValue(targetDbName);
	targetDatabaseEdit->SelectAll();
	targetDatabaseEdit->SetFocus();

	delegate->loadForTreeListCtrl(treeListCtrl);

	structOnlyCheckBox->SetValue(true);
	lockTablesCheckBox->SetValue(true);
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
	progressbar->reset();
	auto tmpdir = ResourceUtil::getStdProductTmpDir();
	tmpSqlPath = tmpdir + "/duplicate_database_" + DateUtil::getCurrentDateTime("%Y%m%d_%H%M%S") + ".sql";
	bool isStructOnly = structOnlyCheckBox->GetValue();
	auto targetSchema = targetDatabaseEdit->GetValue();
	if (targetSchema.empty()) {
		QAnimateBox::error(S("no-target-database"));
		return;
	}
	try {
		auto nSelItem = targetConnectComboBox->GetSelection();
		auto data = reinterpret_cast<QClientData<UserConnect> *>(targetConnectComboBox->GetClientObject(nSelItem));
		if (databaseService->hasUserDb(data->getDataPtr()->id, targetSchema.ToStdString())) {
			QAnimateBox::error(S("exists-target-database"));
			return;
		}
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
		return;
	}
	okButton->Disable();
	exportDatabaseToTmp(tmpSqlPath);
}

void DuplicateDatabaseDialog::OnClickSelectAllButton(wxCommandEvent& event)
{
	auto rootItem = treeListCtrl->GetRootItem();
	if (!rootItem.IsOk()) {
		return;
	}
	treeListCtrl->CheckItem(rootItem, wxCHK_CHECKED);
	treeListCtrl->CheckItemRecursively(rootItem, wxCHK_CHECKED);
}

void DuplicateDatabaseDialog::OnClickUnSelectAllButton(wxCommandEvent& event)
{
	auto rootItem = treeListCtrl->GetRootItem();
	if (!rootItem.IsOk()) {
		return;
	}
	treeListCtrl->CheckItem(rootItem, wxCHK_UNCHECKED);
	treeListCtrl->CheckItemRecursively(rootItem, wxCHK_UNCHECKED);
}

void DuplicateDatabaseDialog::OnStructAndDataCheckBoxChecked(wxCommandEvent& event)
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

void DuplicateDatabaseDialog::OnTreeListItemChecked(wxTreeListEvent& event)
{
	auto item = event.GetItem();
	if (!item.IsOk()) {
		return;
	}

	auto state = treeListCtrl->GetCheckedState(item);
	auto firstChildItem = treeListCtrl->GetFirstChild(item);
	
	if (firstChildItem.IsOk()) {
		treeListCtrl->CheckItemRecursively(item, state);
	}
	if (!treeListCtrl->GetNextSibling(item).IsOk()) {
		auto parentItem = treeListCtrl->GetItemParent(item);
		if (parentItem.IsOk() && treeListCtrl->GetFirstChild(parentItem) == item) {
			treeListCtrl->CheckItem(parentItem, state);
		}
	}
}


/**
 * Export sql statements to specified template file.
 * 
 * @param tmpSqlPath - expert sql statements to template file path
 * @return 
 */
bool DuplicateDatabaseDialog::exportDatabaseToTmp(const std::string& tmpSqlPath)
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
		.append(" --default-character-set=utf8")
		.append(" --databases \"").append(userDb.name).append("\"");
	
	std::string ignoreTables = delegate->getIgnoreTablesStringFromTreeListCtrl(treeListCtrl, userDb.name);
	if (!ignoreTables.empty()) {
		cmd.append(ignoreTables);
	}
	std::string objects = delegate->getObjectsStringFromTreeListCtrl(treeListCtrl);
	if (!objects.empty()) {
		cmd.append(objects);
	}

	bool isStuctOnly = structOnlyCheckBox->GetValue();
	bool isLockTables = lockTablesCheckBox->GetValue();
	if (isStuctOnly) {
		cmd.append(" --no-data");
	}
	if (isLockTables) {
		cmd.append(" --lock-tables");
	}
	cmd.append(" --result-file=\"").append(tmpSqlPath).append("\"");
	exportProcess = new QProcess<DuplicateDatabaseDialog>(this, cmd);
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

bool DuplicateDatabaseDialog::replaceDatabaseNameInTmp()
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
	std::string targetDbName("`" + targetDatabaseEdit->GetValue().ToStdString() + "`");

	std::string line;
	while (std::getline(fin, line)) {
		line = StringUtil::replace(line, sourceDbName, targetDbName);
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

bool DuplicateDatabaseDialog::importDatabaseFromTmp(const std::string& tmpSqlPath)
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
	auto userConnect = data->getDataPtr();
	auto schema = targetDatabaseEdit->GetValue();

	std::string cmd(executePath);
	cmd.append(" -h").append(userConnect->host)
		.append(" -P").append(std::to_string(userConnect->port))
		.append(" -u").append(userConnect->userName)
		.append(" -p").append(userConnect->password)
		.append(" --default-character-set=utf8")
		.append(" -e \"source ").append(tmpSqlPath).append("\"");
	importProcess = new QProcess<DuplicateDatabaseDialog>(this, cmd);
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

DuplicateDatabaseDialog::~DuplicateDatabaseDialog()
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

void DuplicateDatabaseDialog::OnAsyncProcessTermination(QProcess<DuplicateDatabaseDialog>* process)
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
		if (!replaceDatabaseNameInTmp()) {
			okButton->Enable();
			return;
		}
		progressbar->run(60);

		// 3.import sql file to new database when ending export process
		importDatabaseFromTmp(tmpSqlPath);

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


void DuplicateDatabaseDialog::afterDuplicated()
{
	auto connectData = (QClientData<UserConnect> *)targetConnectComboBox->GetClientObject(targetConnectComboBox->GetSelection());
	if (!connectData) {
		return;
	}

	databaseSupplier->handleUserDb.connectId = connectData->getDataPtr()->id;
	databaseSupplier->handleUserDb.name = targetDatabaseEdit->GetValue().ToStdString();
	AppContext::getInstance()->dispatch(Config::MSG_ADD_DATABASE_ID, databaseSupplier->handleUserDb.connectId); 
}
