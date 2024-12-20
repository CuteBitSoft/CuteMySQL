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
#include <vector>

class QSqlEditor : public wxStyledTextCtrl
{
    DECLARE_EVENT_TABLE()
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
    // update the line number width
    void updateLineNumberWidth();
    wxString getPrePositionTextOfCurLine();
    wxString getCurWord();
    wxString getCurMaxWord();
    size_t getCurPosInLine();

    wxString getSelText();
    wxString getText();
    wxString getCurLineText();

    void selectCurMaxWord();
    void replaceSelText(const wxString& text);

    void autoShow(const std::vector<std::string> & tags);
	void autoComplete();
	void autoReplaceWord();
	void autoReplaceSelectTag();
private:
    wxColour textColor;
    wxColour bkgColor;
    wxColour bkgColor2;

    
    void setupSqlSyntax(int nSize, const char* face);
   
    
    void SetStyleBitsEx(int bits);
    void UsePopUpEx(int popUpMode);

    void OnKeydown(wxKeyEvent& evt);
};

