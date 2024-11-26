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
 * @file   CreateConnectDialog.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-07
 *********************************************************************/
#pragma once
#include <wx/treectrl.h>

#include "common/Config.h"
#include "core/entity/Entity.h"
#include "ui/common/dialog/QDialog.h"
#include "ui/ConnectSupplier.h"
#include "ui/dialog/connect/panel/ConnectPanel.h"
#include "ui/dialog/connect/delegate/ConnectDialogDelegate.h"
#include "ui/dialog/connect/supplier/ConnectDialogSupplier.h"
#include "common/event/MsgDispatcherEvent.h"

class ConnectDialog : public QDialog<ConnectDialogDelegate, ConnectDialogSupplier>
{
    DECLARE_EVENT_TABLE()
public:
    ConnectDialog(Config::ConnectType type);
    ~ConnectDialog();
private:    
    Config::ConnectType connectType;

    wxBoxSizer* vLayout;
    wxBoxSizer* hLayout;
    wxBoxSizer* bLayout;

    wxTreeCtrl * leftTree;

    ConnectPanel* rightPanel;

    wxBitmapButton * newButton;
    wxBitmapButton * delButton;
    wxBitmapButton * dupButton;
    wxBitmapButton * upButton;
    wxBitmapButton * downButton;
    wxButton * connectButton;
    wxButton * cancelButton;
    wxButton * testButton;
    

    virtual void init();
    virtual void createControls();

    void createLeftTree();
    void createRightPanel();
    void createBottomBottons();
    void createBottomLeftBottons();
    void createBottomRightBottons();

    void OnLeftTreeSelChange(wxTreeEvent & event);

    void OnTestButtonClick(wxCommandEvent & event);
    void OnConnectButtonClick(wxCommandEvent & event);
    void OnCancelButtonClick(wxCommandEvent & event);

    void OnNewButtonClick(wxCommandEvent & event);
    void OnDelButtonClick(wxCommandEvent & event);
    void OnDupButtonClick(wxCommandEvent & event);
    void OnUpButtonClick(wxCommandEvent & event);
    void OnDownButtonClick(wxCommandEvent & event);

    void OnHandleConnectionCreate(MsgDispatcherEvent &event);
    void OnHandleConnectionChangeName(MsgDispatcherEvent &event);
};

