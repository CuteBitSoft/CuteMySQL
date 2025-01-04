/*****************************************************************//**
 * Copyright 2024 Xuehan Qin 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and

 * limitations under the License.

 * @file   MainFrame.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2024-10-29
 *********************************************************************/

#include "MainFrame.h"
#include <wx/colour.h>
#include <common/AppContext.h>
#include "utils/ResourceUtil.h"

MainFrame::MainFrame(): wxFrame(NULL, wxID_ANY, "CuteMySQL", wxDefaultPosition, wxSize(1024, 760), wxDEFAULT_FRAME_STYLE)
{
	AppContext::getInstance()->setMainFrmWindow(this);

	wxString iconpath = ResourceUtil::getResourceDir() + "/CuteMySQL.ico";
	SetIcon(wxIcon(iconpath, wxBITMAP_TYPE_ICO)); 
	Centre();
	const wxSize clientSize = GetClientSize();
	wxSize homeSize(80, clientSize.GetHeight());
	homeView.Create(this, wxID_ANY, wxPoint(0, 0), homeSize);
	AppContext::getInstance()->setHomeViewWindow((wxWindow *) & homeView);

	wxColour colour(43, 45, 48, 43);
	homeView.SetBackgroundColour(colour);
}

void MainFrame::OnShow(wxShowEvent& event)
{
	
}

void MainFrame::OnClose(wxCloseEvent& event)
{
	Destroy();
}


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_SHOW(MainFrame::OnShow)
	//EVT_WINDOW_CREATE(MainFrame::OnWindowCreate)
	EVT_CLOSE(MainFrame::OnClose)
END_EVENT_TABLE()

