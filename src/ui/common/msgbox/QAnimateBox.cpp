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
#include <wx/shadow.h>
#include "utils/ResourceUtil.h"
#include "common/AppContext.h"

BEGIN_EVENT_TABLE(QAnimateBox, wxDialog)
	EVT_TIMER(STAY_TIMER_ID, OnStayTimeOut)
END_EVENT_TABLE()


QAnimateBox::~QAnimateBox()
{
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


	// use shadow
	// 创建阴影
    wxShadow shadow;
    shadow.SetShadowColour(*wxBLACK); // 设置阴影颜色
    shadow.SetOffset(5, 5);           // 设置阴影偏移量
    shadow.SetBlurRadius(5);           // 设置阴影模糊半径
    shadow.SetQuality(wxSHADOW_QUALITY_3); // 设置阴影质量
    shadow.DrawShadow(this);          // 绘制阴影

	return true;
}

void QAnimateBox::init()
{
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
	if (type == MSG_WARNING) {
		imgpath.Append("/dialog/msgbox/warning.bmp");
	} else if (type == MSG_ERROR) {
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
	stayTimer.StartOnce(3000);
}

void QAnimateBox::OnStayTimeOut(wxTimerEvent& event)
{
	stayTimer.Stop();
	this->Destroy();
}

void QAnimateBox::message(const std::string& text, NotifyType type)
{
	auto msgbox = new QAnimateBox(text, type);
	wxRect clientRect = AppContext::getInstance()->getMainFrmWindow()->GetClientRect();
	int x = clientRect.GetWidth() - 200,
		y = clientRect.GetHeight() - 100;
		
	msgbox->Create(AppContext::getInstance()->getMainFrmWindow(), wxID_ANY, wxEmptyString, { x, y }, { 200, 100 }, wxNO_BORDER);
	msgbox->Show();
	msgbox->Move({ x, y });
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


