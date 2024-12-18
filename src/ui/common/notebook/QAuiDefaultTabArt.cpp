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
 * @file   QAuiDefaultTabArt.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-16
 *********************************************************************/
#include "QAuiDefaultTabArt.h"

QAuiDefaultTabArt::QAuiDefaultTabArt() : wxAuiGenericTabArt()
{
}

void QAuiDefaultTabArt::DrawBackground(wxDC& dc, wxWindow* WXUNUSED(wnd), const wxRect& rect)
{
	// draw background using arbitrary hard-coded, but at least adapted to dark
	// mode, gradient
	int topLightness, bottomLightness;
	if (wxSystemSettings::GetAppearance().IsUsingDarkBackground())
	{
		topLightness = 110;
		bottomLightness = 90;
	}
	else
	{
		topLightness = 90;
		bottomLightness = 170;
	}

	wxColor top_color = m_baseColour.ChangeLightness(topLightness);
	wxColor bottom_color = m_baseColour.ChangeLightness(bottomLightness);
	wxRect r;

	if (m_flags & wxAUI_NB_BOTTOM)
		r = wxRect(rect.x, rect.y, rect.width + 2, rect.height);
	// TODO: else if (m_flags &wxAUI_NB_LEFT) {}
	// TODO: else if (m_flags &wxAUI_NB_RIGHT) {}
	else //for wxAUI_NB_TOP
		r = wxRect(rect.x, rect.y, rect.width + 2, rect.height - 3);

	dc.GradientFillLinear(r, top_color, bottom_color, wxSOUTH);


	// draw base lines

	dc.SetPen(m_borderPen);
	int y = rect.GetHeight();
	int w = rect.GetWidth();

	if (m_flags & wxAUI_NB_BOTTOM)
	{
		dc.SetBrush(wxBrush(bottom_color));
		dc.DrawRectangle(-1, 0, w + 2, 4);
	}
	// TODO: else if (m_flags &wxAUI_NB_LEFT) {}
	// TODO: else if (m_flags &wxAUI_NB_RIGHT) {}
	else //for wxAUI_NB_TOP
	{
		dc.SetBrush(m_baseColourBrush);
		dc.DrawRectangle(-1, y - 4, w + 2, 4);
	}
}

int QAuiDefaultTabArt::GetBorderWidth(wxWindow* wnd)
{
	return 1;
}
