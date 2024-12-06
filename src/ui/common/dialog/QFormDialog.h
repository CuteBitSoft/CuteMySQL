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
 * @file   QFormDialog.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-29
 *********************************************************************/
#pragma once
#include <wx/bmpcbox.h>
#include "ui/common/dialog/QDialog.h"
#include "core/entity/Enum.h"
#include "core/common/Lang.h"

// template <T - subclass of QDelegate>
template<typename T>
class QFormDialog : public QDialog<T>
{
public:
	QFormDialog();
protected:
	wxBoxSizer* vLayout;
	wxBoxSizer* tLayout;
	wxBoxSizer* bLayout;

	wxButton* okButton;
	wxButton* cancelButton;

	virtual void init();
	virtual void createControls();
	virtual void createInputs() = 0;
	virtual void createButtons();
};
template<typename T>
QFormDialog<T>::QFormDialog() : QDialog<T>()
{
	
}

template<typename T>
void QFormDialog<T>::init()
{
	QDialog<T>::init();
}

template<typename T>
void QFormDialog<T>::createControls()
{
	QDialog<T>::createControls();

	vLayout = new wxBoxSizer(wxVERTICAL);
	tLayout = new wxBoxSizer(wxVERTICAL);
	bLayout = new wxBoxSizer(wxHORIZONTAL);
	vLayout->Add(tLayout, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	vLayout->AddSpacer(5);
	vLayout->Add(bLayout, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 10);

	createInputs();
	createButtons();

	this->SetSizer(vLayout);
}

template<typename T>
void QFormDialog<T>::createButtons()
{
	bLayout->AddSpacer(170);
	okButton = new wxButton(this, wxID_OK, S("ok"), wxDefaultPosition, { 70, -1 });
	bLayout->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
	bLayout->AddSpacer(20);

	cancelButton = new wxButton(this, wxID_CANCEL, S("cancel"), wxDefaultPosition, { 70, -1 });
	bLayout->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);
}
