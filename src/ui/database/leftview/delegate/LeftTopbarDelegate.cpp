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
 * @file   LeftTopbarDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-28
 *********************************************************************/
#include "LeftTopbarDelegate.h"
#include <cassert>
#include <wx/msgdlg.h>
#include "ui/common/data/QClientData.h"
#include "ui/common/msgbox/QAnimateBox.h"
#include "core/common/Lang.h"

LeftTopbarDelegate::~LeftTopbarDelegate()
{
	DatabaseService::destroyInstance();
	databaseService = nullptr;
}

/**
 * clear all database items at the combobox, then append new database item.
 * 
 * @param comboBox
 */
void LeftTopbarDelegate::loadDbsForComboBox(wxBitmapComboBox* comboBox)
{
	assert(comboBox != nullptr && supplier != nullptr && supplier->runtimeUserConnect != nullptr);

	auto connectId = supplier->runtimeUserConnect->id;
	comboBox->Clear();
	
	try {
		auto userDbList = databaseService->getAllUserDbs(connectId);
		int n = -1, nSel = -1;
		auto imgdir = ResourceUtil::getProductImagesDir();
		wxBitmap bitmap(imgdir + _("/database/combo/database.bmp"), wxBITMAP_TYPE_BMP);
		for (auto& item : userDbList) {
			auto data = new QClientData<UserDb>(connectId, new UserDb(item));
			n = comboBox->Append(wxString(item.name),bitmap, data);
			if (supplier->runtimeUserDb && supplier->runtimeUserDb->name == item.name) {
				nSel = n;
			}
		}
		if (nSel != -1) {
			comboBox->SetSelection(nSel);
		}
	} catch (QRuntimeException& ex) {
		Q_WARN(S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"));
		QAnimateBox::error(ex);
		return;
	}
	
	
}

/**
 * Only choose the database in the combobox, because all database items in the same connection.
 * 
 * @param comboBox
 */
bool LeftTopbarDelegate::selectDbsForComboBox(wxBitmapComboBox* comboBox)
{
	assert(comboBox != nullptr && supplier != nullptr && supplier->runtimeUserConnect != nullptr);

	auto n = comboBox->GetCount();
	for (unsigned int i = 0; i < n; ++i) {
		// Note: comboBox->GetClientObject(i) will be called in here
		auto data = reinterpret_cast<QClientData<UserDb> *>(comboBox->GetClientObject(i));
		if (supplier->runtimeUserDb && data
			&& supplier->runtimeUserDb->connectId == data->getDataPtr()->connectId
			&& supplier->runtimeUserDb->name == data->getDataPtr()->name) {
			comboBox->SetSelection(i);
			return true;
		}
	}
	return false;
}
