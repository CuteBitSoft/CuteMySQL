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
 * @file   QProcessBar.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-06
 *********************************************************************/
#include "QProgressBar.h"
#include <wx/dcbuffer.h>
#include "core/common/Lang.h"

IMPLEMENT_DYNAMIC_CLASS(QProgressBar, wxControl)
BEGIN_EVENT_TABLE(QProgressBar, wxControl)
	EVT_PAINT(QProgressBar::OnPaint)
END_EVENT_TABLE()
QProgressBar::QProgressBar() : 
	wxControl(), 
	bkgColor(192, 192, 192, 192), 
	processColor(49, 139, 202, 49), 
	errorColor(255, 127, 39),
	textColor(255, 255, 255),
	bkgBrush(bkgColor),
	processBrush(processColor),
	errorBrush(errorColor)
{
	init();
}

QProgressBar::QProgressBar(wxWindow* parent, wxWindowID id, 
	const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, 
	long style /*= wxSUNKEN_BORDER*/, 
	const wxValidator& validator /*= wxDefaultValidator */) : 
	bkgColor(192, 192, 192, 192),
	processColor(49, 139, 202, 49),
	errorColor(255, 127, 39),
	textColor(255, 255, 255),
	bkgBrush(bkgColor),
	processBrush(processColor),
	errorBrush(errorColor)
{
	init();
	Create(parent, id, pos, size, style, validator);
}

bool QProgressBar::Create(wxWindow* parent, wxWindowID id, 
	const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, 
	long style /*= wxSUNKEN_BORDER*/, 
	const wxValidator& validator /*= wxDefaultValidator*/)
{
	if (!wxControl::Create(parent, id, pos, size, style, validator)) {
		return false;
	}
	SetBackgroundColour(wxTransparentColour);
	return true;
}

void QProgressBar::init()
{
	percent = 0;

	textFont = FT("process-text-size");
	textPen = wxPen(textColor, 1);
}

void QProgressBar::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	auto clientRect = GetClientRect();

	double radius = clientRect.GetHeight() > 20 ? 20.0 : clientRect.GetHeight()/ 2.0;
	
	// draw background
	auto oldBrush = dc.GetBrush();
	auto oldPen = dc.GetPen();
	auto oldFont = dc.GetFont();

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(bkgBrush);
	dc.DrawRoundedRectangle(clientRect, radius);

	// draw foreground
	double perPixel = clientRect.GetWidth() * 1.0 / 100.0;
	int pixel = int(round(perPixel * percent));
	int x = clientRect.x, y = clientRect.y, w = pixel, h = clientRect.GetHeight();
	wxRect rect(x, y, w, h);
	if (err.empty()) {
		dc.SetBrush(processBrush);
		dc.DrawRoundedRectangle(rect, radius);
	} else {
		dc.SetBrush(errorBrush);
		dc.DrawRoundedRectangle(rect, radius);
	}

	// draw percent text
	if (percent == 0) {
		return ;
	}
	wxString perText = wxString::Format("%d", percent).Append("%");

	x = (clientRect.GetWidth() - 80) / 2, y = rect.y + 2, w = 80, h = 20;
	wxRect textRect(x, y, w, h);

	auto oldTextColor = dc.GetTextForeground();
	auto oldBkgColor = dc.GetTextBackground();

	wxColour textBkColor =  err.empty() ? processColor : errorColor;

	textBkColor = percent < 60 ? bkgColor : textBkColor;

	dc.SetTextForeground(textColor);
	dc.SetTextBackground(textBkColor);
	dc.SetFont(textFont);
	dc.SetPen(textPen);
	dc.DrawText(perText, x, y);

	// draw status text
	std::string statusText;
	if (err.empty()) {
		statusText = percent < 100 ? "Waiting..." : "Done";
	} else {
		statusText = err;
		dc.SetTextForeground(errorColor);
	}
	textBkColor = percent < 90 ? bkgColor : processColor;
	auto size = dc.GetTextExtent(statusText.c_str());
	x = clientRect.GetWidth() - size.GetWidth() - 10;
	dc.SetTextBackground(textBkColor);
	dc.DrawText(statusText, x, y);

	//release 
	dc.SetTextBackground(oldBkgColor);
	dc.SetTextForeground(oldTextColor);
	
	dc.SetBrush(oldBrush);
	dc.SetFont(oldFont);
	dc.SetPen(oldPen);
	
}

void QProgressBar::run(int percent /*= 0*/)
{
	this->percent = percent <= 100 ? percent : 100;
	if (percent <= 5) {
		err.clear();
	}
	
	Refresh();
}

void QProgressBar::error(const wxString& err)
{
	this->err = err;
	Refresh();
}

void QProgressBar::reset()
{
	this->percent = 0;
	this->err.clear();
	Refresh();
}

