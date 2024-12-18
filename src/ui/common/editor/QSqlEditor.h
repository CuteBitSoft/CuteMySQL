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
 * @file   QSqlEditor.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-17
 *********************************************************************/
#pragma once
#include <wx/stc/stc.h>

class QSqlEditor : public wxStyledTextCtrl
{
public:
	QSqlEditor();
    QSqlEditor(wxWindow *parent, wxWindowID id = wxID_ANY,
          const wxPoint &pos = wxDefaultPosition,
          const wxSize &size = wxDefaultSize,
          long style =
#ifndef __WXMAC__
          wxSUNKEN_BORDER|
#endif
          wxVSCROLL
         );
	void setup(int nSize, const char* face);
    bool SetBackgroundColour(const wxColour & color);
    void setDefaultColorFont(int nSize, const char* face);
private:
    wxColour textColor;
    wxColour bkgColor;
    wxColour bkgColor2;

    
    void setupSqlSyntax(int nSize, const char* face);
   
    void updateLineNumberWidth();
    void SetStyleBitsEx(int bits);
    void UsePopUpEx(int popUpMode);
};

