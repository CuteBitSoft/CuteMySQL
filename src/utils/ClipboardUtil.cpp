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

 * @file   ClipboardUtil.cpp
 * @brief  Copy string to clipboard and read string from clipboard
 * 
 * @author Xuehan Qin
 * @date   2024-12-22
 *********************************************************************/

#include "ClipboardUtil.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <utils/Log.h>

bool ClipboardUtil::copyToClipboard(const wxString & str)
{
    wxClipboardLocker locker;
    if (!locker) {
        Q_ERROR("Failed to lock clipboard.");
        return false;
    }
	if (!wxTheClipboard->Open()) {
        Q_ERROR("Failed to open clipboard.");
        return false;
    }
 
    wxTheClipboard->Clear();
    wxTextDataObject data;
    data.SetText(str);
    wxTheClipboard->AddData(&data);
    wxTheClipboard->Close();
    return true;
}

