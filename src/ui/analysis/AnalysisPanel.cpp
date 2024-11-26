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
 * @file   AnalysisPanel.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-10-31
 *********************************************************************/

#include "AnalysisPanel.h"
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/colour.h>

BEGIN_EVENT_TABLE(AnalysisPanel, wxPanel)
	//EVT_SHOW(HomeView::OnShow)
	//EVT_SIZE(HomeView::OnSize)
	EVT_PAINT(AnalysisPanel::OnPaint)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(AnalysisPanel, wxPanel)

void AnalysisPanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	// wxMemoryDC mdc(&dc);
	const wxRegion clientRegion = GetUpdateRegion();
	if (clientRegion.IsEmpty()) {
		return;
	}
	const wxRect clientRect = clientRegion.GetBox();
	const wxBrush oldBrush = dc.GetBrush();
	const wxPen oldPen = dc.GetPen();

	wxColour color;
	color.SetRGB(0x2b2d30);
	dc.SetTextForeground(wxColour(0xffffff));

	dc.DrawText(_("·ÖÎö"), { 0, 0 });
}
