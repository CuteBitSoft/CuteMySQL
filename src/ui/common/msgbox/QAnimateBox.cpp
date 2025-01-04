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
 * @file   QAnimateBox.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-01
 *********************************************************************/
#include "QAnimateBox.h"
#include <wx/sizer.h>
#include <wx/bmpbndl.h>
#include "utils/ResourceUtil.h"
#include "common/AppContext.h"
#include "core/common/Lang.h"

BEGIN_EVENT_TABLE(QAnimateBox, wxDialog)
	EVT_TIMER(STAY_TIMER_ID, OnStayTimeOut)
	EVT_TIMER(MOVE_TIMER_ID, OnMoveTimeOut)
END_EVENT_TABLE()

unsigned int QAnimateBox::boxCount = 0;

QAnimateBox::~QAnimateBox()
{
	boxCount--;
}

QAnimateBox::QAnimateBox(const std::string &text, NotifyType type):wxDialog(), moveTimer(this, MOVE_TIMER_ID), stayTimer(this, STAY_TIMER_ID)
{
	this->text = text;
	this->type = type;
	init();
}

QAnimateBox::QAnimateBox(wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style)
{
	init();
	Create(parent, id, caption, pos, size, style);
}
bool QAnimateBox::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
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

void QAnimateBox::init()
{
	boxCount++;
	bkgColor = {43, 45, 48, 43};
	textColor = { 223, 225, 229, 213 };
}

void QAnimateBox::createControls()
{
	topSizer = new wxBoxSizer(wxHORIZONTAL);
	leftSizer = new wxBoxSizer(wxVERTICAL);
	rightSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(leftSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);
	topSizer->AddSpacer(10);
	topSizer->Add(rightSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);

	createImage();
	createLabel();
	createTimers();

	SetSizer(topSizer);
}

void QAnimateBox::createImage()
{
	wxString imgdir = ResourceUtil::getProductImagesDir();
	wxString imgpath(imgdir) ;
	if (type == MSG_SUCCESS) {
		imgpath.Append("/dialog/msgbox/success.bmp");
	} else if (type == MSG_WARNING) {
		imgpath.Append("/dialog/msgbox/warning.bmp");
	}
	else if (type == MSG_ERROR) {
		imgpath.Append("/dialog/msgbox/error.bmp");
	} else {
		imgpath.Append("/dialog/msgbox/notice.bmp");
	}
	wxBitmap bitmap(imgpath, wxBITMAP_TYPE_BMP);
	image = new wxStaticBitmap(this, wxID_ANY, wxBitmapBundle(bitmap), wxDefaultPosition, { 20, 20 }, wxNO_BORDER);
	leftSizer->Add(image, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
}

void QAnimateBox::createLabel()
{
	label = new wxStaticText(this, wxID_ANY, text, wxDefaultPosition, wxDefaultSize, 0);
	label->SetForegroundColour(textColor);
	rightSizer->Add(label, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP, 5);
}

void QAnimateBox::createTimers()
{
	// stayTimer.StartOnce(3000);
}


void QAnimateBox::startMove()
{
	wxRect rect = this->GetScreenRect();
	wxRect frmRect = AppContext::getInstance()->getMainFrmWindow()->GetScreenRect();
	minY = frmRect.GetY() + frmRect.GetHeight() - (rect.GetHeight() + 20) * boxCount;

	moveTimer.Start(80);	
}

void QAnimateBox::OnStayTimeOut(wxTimerEvent& event)
{
	stayTimer.Stop();
	this->Destroy();
}

void QAnimateBox::OnMoveTimeOut(wxTimerEvent& event)
{
	wxRect rect = this->GetScreenRect();
	wxRect frmRect = AppContext::getInstance()->getMainFrmWindow()->GetScreenRect();
	if (!IsShown() && rect.GetTop() <= frmRect.GetBottom()) {
		Show(true);
	}

	int x = frmRect.GetX() + frmRect.GetWidth() - rect.GetWidth() - 20,
		y = rect.GetTop() - 5;
		
	Move({x, y});
	if (y <= minY) {
		moveTimer.Stop();
		stayTimer.StartOnce(3000);
	}
}

void QAnimateBox::message(const std::string& text, NotifyType type)
{
	auto msgbox = new QAnimateBox(text, type);
	
	msgbox->Create(AppContext::getInstance()->getMainFrmWindow(), wxID_ANY, wxEmptyString, wxDefaultPosition, { 200, 100 }, wxNO_BORDER);
	msgbox->Show(false);

	wxRect frmRect = AppContext::getInstance()->getMainFrmWindow()->GetScreenRect();
	wxSize size = msgbox->GetSize();
	int x = frmRect.GetX() + frmRect.GetWidth() - size.GetWidth() - 20,
		y = frmRect.GetY() + frmRect.GetHeight() + 20;
	msgbox->Move({ x, y });

	msgbox->startMove();
}

void QAnimateBox::success(const std::string& text)
{
	QAnimateBox::message(text, NotifyType::MSG_SUCCESS);
}

void QAnimateBox::notice(const std::string& text)
{
	QAnimateBox::message(text, NotifyType::MSG_NOTICE);
}

void QAnimateBox::warning(const std::string& text)
{
	QAnimateBox::message(text, NotifyType::MSG_WARNING);
}

void QAnimateBox::error(const std::string& text)
{
	QAnimateBox::message(text, NotifyType::MSG_ERROR);
}

void QAnimateBox::error(const QSqlExecuteException& ex)
{
	std::string text = S("error-text");
	text.append(ex.getMsg()).append(" [code:").append(ex.getCode()).append("]");
	QAnimateBox::message(text, NotifyType::MSG_ERROR);
}

void QAnimateBox::error(const QRuntimeException& ex)
{
	std::string text = S("error-execute-sql");
	text.append(" [code:").append(ex.getCode()).append("] ").append(ex.getMsg());
	QAnimateBox::message(text, NotifyType::MSG_ERROR);
}
