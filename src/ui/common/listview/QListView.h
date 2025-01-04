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
 * @file   QListView.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-24
 *********************************************************************/
#pragma once
#include <wx/listctrl.h>
#include <wx/itemattr.h>
#include <utility>
#include <unordered_map>

#include "core/entity/Entity.h"

struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

class QListView : public wxListView
{
public:
    QListView();
    void SetDataList(const DataList* dataList);
private:
    
    const DataList* dataList;
    std::unordered_map<std::pair<uint32_t, uint32_t>, const char *, pair_hash> cachData;
    wxColour rowBkgColor1, rowBkgColor2;
    wxColour textColor;
    wxColour colBkgColor;

    virtual wxItemAttr* OnGetItemAttr(long item) const;
    virtual wxItemAttr* OnGetItemColumnAttr(long item, long column) const;
    virtual wxString OnGetItemText(long item, long column) const;
    virtual bool OnGetItemIsChecked(long item) const;
    virtual int OnGetItemImage(long item) const;
    virtual int  OnGetItemColumnImage(long item, long column) const;

    void OnListCacheHint(wxListEvent& event);
    void OnListItemChecked(wxListEvent& event);
    void OnListItemUnChecked(wxListEvent& event);
};

