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
 * @file   DatabasePanel.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-10-31
 *********************************************************************/

#include "DatabasePanel.h"
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/colour.h>

BEGIN_EVENT_TABLE(DatabasePanel, wxPanel)
	EVT_SIZE(DatabasePanel::OnSize)
END_EVENT_TABLE()
	
IMPLEMENT_DYNAMIC_CLASS(DatabasePanel, wxPanel)

DatabasePanel::DatabasePanel() :QPanel()
{
	
}

DatabasePanel::~DatabasePanel()
{
}

void DatabasePanel::init()
{

}

void DatabasePanel::createControls()
{
	QPanel::createControls();
	createSplitter();
	createLeftTreeView();
	createRightWorkView();
	splitter->SplitVertically(leftTreeView, rightWorkView, 250);
}

void DatabasePanel::loadControls()
{
}

void DatabasePanel::createSplitter()
{
	wxSize clientSize = GetClientSize();
	splitter = new wxSplitterWindow();
	splitter->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN | wxNO_BORDER);
	//splitter->SetSplitMode(wxSPLIT_VERTICAL);
	splitter->SetSize(clientSize);
	splitter->SetMinimumPaneSize(200);
}

void DatabasePanel::createLeftTreeView()
{
	leftTreeView = new LeftTreeView();
	leftTreeView->Create(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);
}

void DatabasePanel::createRightWorkView()
{
	rightWorkView = new RightWorkView();
	rightWorkView->Create(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);
}

void DatabasePanel::resizeSplitter()
{
	if (!splitter) {
		return;
	}
	wxSize clientSize = GetClientSize();
	splitter->SetSize(clientSize);
}

void DatabasePanel::OnSize(wxSizeEvent& event)
{
	resizeSplitter();
}
