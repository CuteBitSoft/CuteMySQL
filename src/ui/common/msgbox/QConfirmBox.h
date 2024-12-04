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
 * @file   QConfirmBox.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-01
 *********************************************************************/
#pragma once
#include <wx/dialog.h>
#include <wx/timer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include "core/common/exception/QRuntimeException.h"

class QConfirmBox :  public wxDialog
{
	DECLARE_EVENT_TABLE()
public:
	typedef enum {
		MOVE_TIMER_ID2 = 2, // MOVE
	} ConfirmTimerId;


	static int confirm(const std::string & text, const std::string & otherBtnLabel = "");

	
private:
	QConfirmBox(const std::string &text = "", const std::string & otherBtnLabel = "");
	~QConfirmBox();

	std::string text;
	std::string otherBtnLabel;
	
	bool hasStartedMove;
	wxTimer moveTimer;

	wxColour bkgColor;
	wxColour textColor;

	wxBoxSizer* topSizer;
	wxBoxSizer* hSizer;
	wxBoxSizer* leftSizer;
	wxBoxSizer* rightSizer;
	wxBoxSizer* bottomSizer;

	wxStaticBitmap* image;
	wxStaticText* label;

	QConfirmBox(wxWindow* parent,
		wxWindowID id = 0,
		const wxString& caption = _(""),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION | wxCLOSE_BOX);

	/// Creation
	bool Create(wxWindow* parent,
		wxWindowID id = 0,
		const wxString& caption = _("QDialog"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION | wxCLOSE_BOX);

	void init();
	void createControls();
	void createImage();
	void createLabel();
	void createButtons();
	void startMove();

	void OnMoveTimeOut(wxTimerEvent & event);
	void OnShow(wxShowEvent & event);
	
};

