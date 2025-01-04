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
 * @file   QueryPageEditor.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-17
 *********************************************************************/
#include "QueryPageEditor.h"
#include "common/Config.h"
#include "core/common/Lang.h"

BEGIN_EVENT_TABLE(QueryPageEditor, wxPanel)
	EVT_STC_ZOOM(Config::DATABASE_QUERY_EDITOR_ID, OnStcZoom) //放大，缩小
	EVT_STC_MODIFIED(Config::DATABASE_QUERY_EDITOR_ID, OnStcModified)  //修改了文件
	EVT_STC_MARGINCLICK(Config::DATABASE_QUERY_EDITOR_ID, OnStcMarginClicked) //确定是Fold页边点击事件
	EVT_STC_UPDATEUI(Config::DATABASE_QUERY_EDITOR_ID, OnStcUpdateUI)  //界面更新(单击鼠标，按下箭头等)
	EVT_STC_CHARADDED(Config::DATABASE_QUERY_EDITOR_ID, OnStcCharAdded) // 字符输入 
	// EVT_STC_FOCUSIN(Config::DATABASE_QUERY_EDITOR_ID, OnStcFocusIn) // 获取焦点
	EVT_STC_DOUBLECLICK(Config::DATABASE_QUERY_EDITOR_ID, OnStcDoubleClick) // double click
	// EVT_STC_KEY(Config::DATABASE_QUERY_EDITOR_ID, OnStcKey) // 按键
	EVT_STC_AUTOCOMP_SELECTION(Config::DATABASE_QUERY_EDITOR_ID, OnAutoCSelection) // 提示选择
	EVT_COMBOBOX(Config::QUERY_PAGE_CONNECT_COMBOBOX_ID, OnSelChangeConnectCombobox)
	EVT_COMBOBOX(Config::QUERY_PAGE_DATABASE_COMBOBOX_ID, OnSelChangeDatabaseCombobox)
END_EVENT_TABLE()
QueryPageEditor::QueryPageEditor(QueryPageSupplier* queryPageSupplier) : QPanel<DatabaseSupplier>()
{
	this->mysupplier = queryPageSupplier;
	init();
}

QueryPageEditor::~QueryPageEditor()
{
	delete delegate;
	delegate = nullptr;

}

wxString QueryPageEditor::getSelText()
{
	return editor->getSelText();
}

wxString QueryPageEditor::getText()
{
	return editor->GetText();
}

void QueryPageEditor::focus()
{
	editor->SetFocus();
}

void QueryPageEditor::init()
{
	bkgColor = wxColour(30, 31, 34, 30);
	//my supplier init data
	this->mysupplier->setRuntimeUserConnectId(supplier->getRuntimeUserConnectId());
	this->mysupplier->setRuntimeSchema(supplier->getRuntimeSchema());
	this->mysupplier->setRuntimeTblName(supplier->getRuntimeTblName());

	delegate = new QueryPageEditorDelegate(this, mysupplier);
}

void QueryPageEditor::createControls()
{
	
	//topSizer->Add(title, 0, wxALIGN_LEFT | wxALIGN_TOP);
	createToolbarInputs();
	createEditor();
}

void QueryPageEditor::createToolbarInputs()
{
	topSizer->AddSpacer(5);
	wxBoxSizer* toolbarHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(toolbarHoriLayout, 0, wxALIGN_LEFT | wxALIGN_TOP);

	toolbarHoriLayout->AddSpacer(10);
	connectComboBox = new wxBitmapComboBox(this, Config::QUERY_PAGE_CONNECT_COMBOBOX_ID, wxEmptyString, wxDefaultPosition,
		{ 180, -1 }, wxArrayString(), wxNO_BORDER | wxCLIP_CHILDREN | wxCB_READONLY);
	
	toolbarHoriLayout->Add(connectComboBox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	toolbarHoriLayout->AddSpacer(10);
	databaseComboBox = new wxBitmapComboBox(this, Config::QUERY_PAGE_DATABASE_COMBOBOX_ID, wxEmptyString, wxDefaultPosition,
		{ 180, -1 }, wxArrayString(), wxNO_BORDER | wxCLIP_CHILDREN | wxCB_READONLY);
	toolbarHoriLayout->Add(databaseComboBox, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
}

void QueryPageEditor::createEditor()
{
	topSizer->AddSpacer(10);
	editor = new QSqlEditor();
	editor->Create(this, Config::DATABASE_QUERY_EDITOR_ID, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxCLIP_CHILDREN);
	editor->setup(12, FN("Courier New").c_str());
	editor->SetFocus();
	topSizer->Add(editor, 1, wxEXPAND);
}

void QueryPageEditor::loadControls()
{
	delegate->loadForConnectComboBox(connectComboBox, mysupplier->getRuntimeUserConnectId());
	delegate->loadForSchemaComboBox(databaseComboBox, mysupplier->getRuntimeUserConnectId());
}

void QueryPageEditor::OnStcZoom(wxStyledTextEvent& event)
{
	if (editor->GetMarginWidth(0) != 0) {
		editor->updateLineNumberWidth();
	}
}

void QueryPageEditor::OnStcModified(wxStyledTextEvent& event)
{
	if (editor->GetMarginWidth(0) != 0) {
		editor->updateLineNumberWidth();
	}
	
}

void QueryPageEditor::OnStcMarginClicked(wxStyledTextEvent& event)
{
	long n = editor->LineFromPosition(event.GetPosition());
	editor->ToggleFold(n);
}

void QueryPageEditor::OnStcUpdateUI(wxStyledTextEvent& event)
{
}

void QueryPageEditor::OnStcCharAdded(wxStyledTextEvent& event)
{
	wxString line, preline, word;
	line = editor->GetCurLine();
	if (line.empty()) {
		return ;
	}
	preline = editor->getPrePositionTextOfCurLine();
	word = editor->getCurWord();
	size_t curPosInLine = editor->getCurPosInLine();
	std::vector<std::string> tags = delegate->getTags(line.ToStdString(), preline.ToStdString(), word.ToStdString(), curPosInLine);
	editor->autoShow(tags);
}

void QueryPageEditor::OnStcFocusIn(wxStyledTextEvent& event)
{
}

void QueryPageEditor::OnStcDoubleClick(wxStyledTextEvent& event)
{
}

void QueryPageEditor::OnStcKey(wxStyledTextEvent& event)
{
}

void QueryPageEditor::OnAutoCSelection(wxStyledTextEvent& event)
{
	wxString selText = editor->GetSelectedText();
	if (selText.empty()) {
		editor->autoReplaceWord();
		return;
	}
	if ((char)selText.at(0) == '<' && (char)selText.Last() == '>') {
		editor->autoReplaceSelectTag();
		return;
	}
	editor->autoReplaceWord();
}

void QueryPageEditor::OnSelChangeConnectCombobox(wxCommandEvent& event)
{
	if (connectComboBox->GetSelection() ==  wxNOT_FOUND) {
		mysupplier->setRuntimeUserConnectId(0);
		return;
	}
	auto connectData = (QClientData<UserConnect> *)connectComboBox->GetClientObject(connectComboBox->GetSelection());
	if (!connectData) {
		mysupplier->setRuntimeUserConnectId(0);
		return;
	}
	// load runtime connection 
	mysupplier->setRuntimeUserConnectId(connectData->getDataPtr()->id);
	delegate->loadForSchemaComboBox(databaseComboBox, connectData->getDataPtr()->id);

	// load runtime database and table name in the mysupplier(QueryPageSupplier)
	doLoadRuntimeDbAndTblName();
}

void QueryPageEditor::OnSelChangeDatabaseCombobox(wxCommandEvent& event)
{
	doLoadRuntimeDbAndTblName();
}

void QueryPageEditor::doLoadRuntimeDbAndTblName()
{
	if (databaseComboBox->GetSelection() ==  wxNOT_FOUND) {
		mysupplier->setRuntimeSchema("");
		return;
	}
	auto dbData = (QClientData<UserDb> *)databaseComboBox->GetClientObject(databaseComboBox->GetSelection());
	if (!dbData) {
		mysupplier->setRuntimeSchema("");
		return;
	}
	mysupplier->setRuntimeSchema(dbData->getDataPtr()->name);
	mysupplier->setRuntimeTblName("");
}
