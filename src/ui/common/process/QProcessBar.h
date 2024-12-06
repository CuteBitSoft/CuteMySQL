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
 * @file   QProcessBar.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-06
 *********************************************************************/
#pragma once
#include <wx/control.h>
#include <wx/brush.h>
#include <wx/pen.h>
#include <wx/font.h>
#include <wx/colour.h>

class QProcessBar : public wxControl
{
	DECLARE_DYNAMIC_CLASS(QProcessBar)
	DECLARE_EVENT_TABLE()
public:
	QProcessBar();

	QProcessBar(wxWindow * parent, 
		wxWindowID id, 
		const wxPoint & pos = wxDefaultPosition,
		const wxSize & size = wxDefaultSize,
		long style = wxNO_BORDER,
		const wxValidator & validator = wxDefaultValidator );

	bool Create(wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxNO_BORDER,
		const wxValidator& validator = wxDefaultValidator);

	void run(int percent = 0);
	void error(const wxString& err);
	void reset();
private:
	int percent;
	wxString err;
	
	const wxColour bkgColor;
	const wxColour processColor;
	const wxColour errorColor;
	const wxColour textColor;

	const wxBrush bkgBrush;
	const wxBrush processBrush;
	const wxBrush errorBrush;

	wxFont textFont;
	wxPen textPen;

	void init();
	void OnPaint(wxPaintEvent& event);
	
};

