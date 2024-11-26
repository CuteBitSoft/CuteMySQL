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

 * @file   DatabaseListItem.h
 * @brief  Show database list in HomePanel 
 * 
 * @author Xuehan Qin
 * @date   2023-11-25
 *********************************************************************/
#pragma once
#include <wx/panel.h>
#include <wx/brush.h>
#include <wx/colour.h>
#include <wx/font.h>
#include <wx/bmpbuttn.h>
#include <wx/stattext.h>
#include "core/entity/Entity.h"

#define DATABASE_LIST_ITEM_WIDTH 240
#define DATABASE_LIST_ITEM_HEIGHT 100
class ConnectListItem : public wxPanel
{
	DECLARE_EVENT_TABLE()
public:
	
	ConnectListItem(UserConnect & _UserConnect);
	uint64_t getUserConnectId();
	bool getChecked() const { return checked; }
	void setChecked(bool val);
private:
	UserConnect userConnect;
	bool checked = false;

	bool isHover = false;
	bool isTracking = false;
	wxColour bkgColor = { 213, 213, 213 , 213};
	wxColour bkgHoverColor = { 180, 180, 180 };
	wxColour textColor = { 80, 80, 80, 80 };
	wxBrush bkgBrush;
	wxBrush bkgHoverBrush;
	wxFont nameFont;
	wxFont textFont;

	wxBitmapButton * userImage = nullptr;
	wxStaticText * userLabel = nullptr;

	wxBitmapButton * hostImage = nullptr;
	wxStaticText * hostLabel = nullptr;

	void createOrShowUI();
	void createOrShowUserImage(const wxRect &clientRect);
	void createOrShowUserLabel(const wxRect &clientRect);

	void createOrShowHostImage(const wxRect &clientRect);
	void createOrShowHostLabel(const wxRect &clientRect);

	wxStaticText * createOrShowLabel(wxStaticText * pathEdit, uint32_t id, std::string text, const wxRect & rect, const wxRect &clientRect, uint32_t exStyle);
	void OnSize(wxSizeEvent& event);
	void OnShow(wxShowEvent& event);
	void OnPaint(wxPaintEvent& event);
	
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnLButtonDown(wxMouseEvent& event);
	void OnLeftDoubleClick(wxMouseEvent& event);
};
