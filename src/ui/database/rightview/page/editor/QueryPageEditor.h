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
 * @file   QueryPageEditor.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-17
 *********************************************************************/
#pragma once
#include <wx/bmpcbox.h>
#include "ui/common/panel/QPanel.h"
#include "ui/common/editor/QSqlEditor.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/database/rightview/page/supplier/QueryPageSupplier.h"
#include "ui/database/rightview/page/supplier/QueryPageSupplier.h"
#include "ui/database/rightview/page/editor/delegate/QueryPageEditorDelegate.h"

class QueryPageEditor :  public QPanel<DatabaseSupplier>
{
	DECLARE_EVENT_TABLE()
public:
	QueryPageEditor(QueryPageSupplier * queryPageSupplier);
	~QueryPageEditor();
private:

	//toolbar controls
	wxBitmapComboBox*	connectComboBox;
	wxBitmapComboBox*	databaseComboBox;

	QSqlEditor* editor;

	QueryPageSupplier* mysupplier;
	QueryPageEditorDelegate* delegate;

	virtual void init();
	virtual void createControls();
	void createToolbarInputs();
	void createEditor();
	virtual void loadControls();

	// wxStyledTextCtrl Events
	void OnStcZoom(wxStyledTextEvent& event);
	void OnStcModified(wxStyledTextEvent& event);
	void OnStcMarginClicked(wxStyledTextEvent& event);
	void OnStcUpdateUI(wxStyledTextEvent& event);
	void OnStcCharAdded(wxStyledTextEvent& event);
	void OnStcFocusIn(wxStyledTextEvent& event);
	void OnStcDoubleClick(wxStyledTextEvent& event);
	void OnStcKey(wxStyledTextEvent& event);
	void OnAutoCSelection(wxStyledTextEvent& event);

	// combobox changed events
	void OnSelChangeConnectCombobox(wxCommandEvent& event);
	void OnSelChangeDatabaseCombobox(wxCommandEvent& event);

	void doLoadRuntimeDbAndTblName();
};

