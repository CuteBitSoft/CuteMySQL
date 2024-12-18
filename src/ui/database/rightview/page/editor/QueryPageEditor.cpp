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

QueryPageEditor::QueryPageEditor(const wxString & titleLabel) : QPanel<QueryPageSupplier>()
{
	init();
	this->titleLabel = titleLabel;
}

void QueryPageEditor::init()
{
	bkgColor = wxColour(30, 31, 34, 30);
}

void QueryPageEditor::createControls()
{
	title = new wxStaticText(this, wxID_ANY, titleLabel, wxDefaultPosition, FromDIP(wxSize(- 1, 20)), wxNO_BORDER | wxCLIP_CHILDREN);
	title->SetBackgroundColour(bkgColor);
	title->SetForegroundColour(textColor);
	topSizer->Add(title, 0, wxALIGN_LEFT | wxALIGN_TOP);

	editor = new QSqlEditor();
	editor->Create(this, Config::DATABASE_QUERY_EDITOR_ID, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxCLIP_CHILDREN);
	editor->setup(12, FN("Courier New").c_str());
	editor->SetFocus();
	editor->SetForegroundColour(textColor);
	topSizer->Add(editor, 1, wxEXPAND);
}
