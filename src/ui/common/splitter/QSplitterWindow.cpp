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
 * @file   QSplitterWindow.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-18
 *********************************************************************/
#include "QSplitterWindow.h"
#include <wx/dcclient.h>

BEGIN_EVENT_TABLE(QSplitterWindow, wxSplitterWindow)
	EVT_PAINT(QSplitterWindow::OnPaint)
END_EVENT_TABLE()

void QSplitterWindow::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	dc.SetPen(*wxRED); // 设置分割条颜色为红色
	dc.SetBrush(*wxTRANSPARENT_BRUSH); // 设置背景透明
	auto size = GetSashSize();
	int x = 0, y = 0, width = size, height = GetSize().GetHeight();
	int sashSize = GetSashSize();
	int sashPosition = GetSashPosition();
	if (GetSplitMode() == wxSPLIT_VERTICAL) {
		x = sashPosition - sashSize / 2;
		dc.DrawRectangle(x, y, sashSize, height);
	}
	else {
		y = sashPosition - sashSize / 2;
		dc.DrawRectangle(x, y, width, sashSize);
	}
}
