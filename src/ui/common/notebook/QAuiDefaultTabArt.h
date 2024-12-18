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
 * @file   QAuiDefaultTabArt.h
 * @brief  Provide default tab art to QNotebook
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-16
 *********************************************************************/
#pragma once
#include <wx/aui/auibook.h>
#include <wx/dc.h>
class QAuiDefaultTabArt :  public wxAuiGenericTabArt
{
public:
	QAuiDefaultTabArt();
	wxAuiTabArt* Clone()
	{
		return new QAuiDefaultTabArt(*this);
	}
	virtual void DrawBackground(wxDC& dc, wxWindow* wnd, const wxRect& rect) wxOVERRIDE;
	virtual int GetBorderWidth(wxWindow* wnd) wxOVERRIDE;
};

