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
 * @file   QProcess.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-08
 *********************************************************************/
#pragma once

#include <wx/process.h>
#include <wx/window.h>
#include "utils/Log.h"

// template<T - subclass of wxWindow>
template <typename T = wxWindow>
class QProcess : public wxProcess {
public:
    QProcess(T *_parent, const std::string& _cmd)
        : wxProcess(_parent), cmd(_cmd), parent(_parent)
    {
        Redirect();
    }

    // instead of overriding this virtual function we might as well process the
    // event from it in the frame class - this might be more convenient in some
    // cases
    virtual void OnTerminate(int pid, int status) wxOVERRIDE;

    bool open();

    std::string getOutputText();
    std::string getOutputError();
protected:
    T *parent;
    std::string cmd;
};

template <typename T /*= wxWindow*/>
std::string QProcess<T>::getOutputError()
{
	wxString msg;
	while (IsInputAvailable()) {
		wxTextInputStream tis(*GetInputStream());

		// this assumes that the output is always line buffered		
		msg << tis.ReadLine();
	}
	return msg.ToStdString();
}

template <typename T /*= wxWindow*/>
std::string QProcess<T>::getOutputText()
{
    wxString msg;
	while (IsErrorAvailable()) {
		wxTextInputStream tis(*GetErrorStream());

		// this assumes that the output is always line buffered		
		msg << tis.ReadLine();
	}
    return msg.ToStdString();
}

template <typename T>
void QProcess<T>::OnTerminate(int pid, int status)
{
    Q_INFO("Process {} ('{}') terminated with exit code {}.",
                pid, cmd, status);

    parent->OnAsyncProcessTermination(this);
}

template<typename T>
bool QProcess<T>::open()
{
    long pidLast = wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE, this);
    if ( !pidLast ) {
        Q_ERROR("Execution of '{}' failed.", cmd);
        delete this;
        return false;
    } else {
        Q_INFO("Process {} ({}) launched.", pidLast, cmd);
        return true;
    }
}
