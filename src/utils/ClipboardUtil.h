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

 * @file   ClipboardUtil.h
 * @brief  Copy string to clipboard and read string from clipboard
 * 
 * @author Xuehan Qin
  * @date  2024-12-22
 *********************************************************************/
#pragma once

#include <wx/string.h>

class ClipboardUtil {
public:
	static bool copyToClipboard(const wxString & str);
};
