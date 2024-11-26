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
 * @file   MsgDispatcherEvent.h
 * @brief  Custom events for the MsgDispatcher class, 
 *         and for window subscriptions and unsubscriptions to custom messages.
 *         
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-15
 *********************************************************************/
#pragma once
#include <wx/event.h>

class MsgDispatcherEvent : public wxNotifyEvent
{
public:
	MsgDispatcherEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
                 wxNotifyEvent(commandType, id)
    {}

    MsgDispatcherEvent(const MsgDispatcherEvent& event): wxNotifyEvent(event)
    {}

    virtual wxEvent *Clone() const { return new MsgDispatcherEvent(*this); }

    DECLARE_DYNAMIC_CLASS(wxFontSelectorCtrlEvent);
};

typedef void (wxEvtHandler::*MsgDispatcherEventFunction)(MsgDispatcherEvent&);

/*!
 * message dispatcher events and macros for handling them
 */

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_NOTITY_MESSAGE_HANDLE, 802)
END_DECLARE_EVENT_TYPES()

#define EVT_NOTITY_MESSAGE_HANDLE(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_NOTITY_MESSAGE_HANDLE, id, -1, (wxObjectEventFunction) (wxEventFunction) (MsgDispatcherEventFunction) & fn, (wxObject *) NULL ),
