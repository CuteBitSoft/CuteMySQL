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

 * @file   CuteMySQL.cpp
 * @brief  
 * 
 * @author Xuehan Qin
 * @date   2024-10-29
 *********************************************************************/

#include "CuteMySQL.h"
#include <wx/image.h>

IMPLEMENT_APP(CuteMySQL);

bool CuteMySQL::OnInit()
{
    if (mainFrame != nullptr) {
        return true;
    }
    // Supported the PNG image in this application
    wxImage::AddHandler(new wxICOHandler);
    wxImage::AddHandler(new wxPNGHandler);

    context = AppContext::getInstance();
    mainFrame = new MainFrame();
    context->setMainFrmWindow((wxWindow*)mainFrame);
    mainFrame->Show(true);

    initCoreServices();
    return true;
}

int CuteMySQL::OnExit()
{
    destroyCoreServices();

    // if don't do this, will be causing "Detected memory leaks!" in the end.
    AppContext::destroyInstance();
        
    return 0;
}

void CuteMySQL::initCoreServices()
{
    
}

void CuteMySQL::destroyCoreServices()
{
    
}
