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
 * @file   QConfirmBox.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-01
 *********************************************************************/
#include "QConfirmBox.h"
#include <wx/sizer.h>
#include <wx/bmpbndl.h>
#include <wx/button.h>
#include "utils/ResourceUtil.h"
#include "common/AppContext.h"
#include "common/Config.h"
#include "core/common/Lang.h"

BEGIN_EVENT_TABLE(QConfirmBox, wxDialog)
	EVT_TIMER(MOVE_TIMER_ID2, QConfirmBox::OnMoveTimeOut)
	EVT_SHOW(OnShow)
END_EVENT_TABLE()


QConfirmBox::~QConfirmBox()
{
}

QConfirmBox::QConfirmBox(const std::string &text, const std::string &otherBtnLabel):wxDialog(), moveTimer(this, MOVE_TIMER_ID2), hasStartedMove(false)
{
	this->text = text;
	this->otherBtnLabel = otherBtnLabel;
	init();
}

QConfirmBox::QConfirmBox(wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style)
{
	init();
	Create(parent, id, caption, pos, size, style);
}
bool QConfirmBox::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
	// dialog
	// SetExtraStyle(wxWS_EX_BLOCK_EVENTS|wxDIALOG_EX_CONTEXTHELP);

	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	createControls(); 

	// This fits the dialog to the minimum size dictated by
	// the sizers

	GetSizer()->Fit(this);

	// This ensures that the dialog cannot be sized smaller
	// than the minimum size

	GetSizer()->SetSizeHints(this);

	// Centre the dialog on the parent or (if none) screen

	SetBackgroundColour(bkgColor);
	SetForegroundColour(textColor);

	return true;
}

void QConfirmBox::init()
{
	bkgColor = {43, 45, 48, 43};
	textColor = { 223, 225, 229, 213 };
}

void QConfirmBox::createControls()
{
	topSizer = new wxBoxSizer(wxVERTICAL);
	hSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->AddSpacer(20);
	topSizer->Add(hSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 10);
	topSizer->AddSpacer(20);

	leftSizer = new wxBoxSizer(wxVERTICAL);
	rightSizer = new wxBoxSizer(wxVERTICAL);
	hSizer->Add(leftSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);
	hSizer->AddSpacer(10);
	hSizer->Add(rightSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);

	bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(bottomSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 10);

	createImage();
	createLabel();
	createButtons();

	SetSizer(topSizer);
}

void QConfirmBox::createImage()
{
	wxString imgdir = ResourceUtil::getProductImagesDir();
	wxString imgpath(imgdir);
	imgpath.Append("/dialog/msgbox/warning.bmp");

	wxBitmap bitmap(imgpath, wxBITMAP_TYPE_BMP);
	image = new wxStaticBitmap(this, wxID_ANY, wxBitmapBundle(bitmap), wxDefaultPosition, { 20, 20 }, wxNO_BORDER);
	leftSizer->Add(image, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
}

void QConfirmBox::createLabel()
{
	label = new wxStaticText(this, wxID_ANY, text, wxDefaultPosition, wxDefaultSize, 0);
	label->SetForegroundColour(textColor);
	rightSizer->Add(label, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
}

void QConfirmBox::createButtons()
{
	bottomSizer->AddSpacer(120);
	if (!otherBtnLabel.empty()) {
		auto btnOther = new wxButton(this, Config::CONFIRM_OTHER_BUTTON_ID, otherBtnLabel, wxDefaultPosition, { 60, 20 });
		bottomSizer->Add(btnOther, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
		bottomSizer->AddSpacer(10);
	}
	auto btnOk = new wxButton(this, wxID_OK, S("yes"), wxDefaultPosition, {60, 20});
	bottomSizer->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	bottomSizer->AddSpacer(10);

	auto btnCancel = new wxButton(this, wxID_CANCEL, S("cancel"), wxDefaultPosition, {60, 20});
	bottomSizer->Add(btnCancel, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	bottomSizer->AddSpacer(10);
}


void QConfirmBox::startMove()
{
	moveTimer.Start(80);
	hasStartedMove = true;
}

void QConfirmBox::OnMoveTimeOut(wxTimerEvent& event)
{
	wxRect rect = this->GetScreenRect();
	wxRect frmRect = AppContext::getInstance()->getMainFrmWindow()->GetScreenRect();
	if (!IsShown() && rect.GetTop() <= frmRect.GetBottom()) {
		Show(true);
	}

	int x = frmRect.GetX() + (frmRect.GetWidth() - rect.GetWidth() - 20) / 2,
		y = rect.GetTop() + 5,
		maxY = frmRect.GetY() + 50;
	Move({x, y});
	if (y >= maxY) {
		moveTimer.Stop();
	}
}

void QConfirmBox::OnShow(wxShowEvent& event)
{
	if (moveTimer.IsRunning() || hasStartedMove) {
		return;
	}
	startMove();
}

int QConfirmBox::confirm(const std::string& text, const std::string& otherBtnLabel)
{
	auto msgbox = new QConfirmBox(text, otherBtnLabel);
		
	wxRect frmRect = AppContext::getInstance()->getMainFrmWindow()->GetScreenRect();
	wxSize size = msgbox->GetSize();
	int x = frmRect.GetX() + (frmRect.GetWidth() - size.GetWidth() - 20) / 2,
		y = frmRect.GetY() ;
	msgbox->Create(AppContext::getInstance()->getMainFrmWindow(), wxID_ANY, wxEmptyString, wxDefaultPosition, { 200, 100 }, wxNO_BORDER);
	msgbox->Move(x, y);
	return msgbox->ShowModal();
}
