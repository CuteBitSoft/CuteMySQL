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
 * @file   DatabaseDialogDelegate.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-30
 *********************************************************************/
#include "DatabaseDialogDelegate.h"
#include <cassert>
#include <wx/msgdlg.h>
#include "core/common/Lang.h"
#include "ui/common/data/QClientData.h"

void DatabaseDialogDelegate::loadForConnectComboBox(wxBitmapComboBox* connectComboBox)
{
	assert(connectComboBox != nullptr);
	try {
		auto connectList = connectService->getAllUserConnects();

		auto imgdir = ResourceUtil::getProductImagesDir();
		wxString path = imgdir + "/dialog/database/connection.bmp";
		wxBitmap bitmap(path, wxBITMAP_TYPE_BMP);
		int nSelItem = -1;
		for (auto& item : connectList) {
			auto data = new QClientData<UserConnect>(item.id, new UserConnect(item));
			wxString connnectName = item.name;
			connnectName.Append(" [").Append(item.host).Append(":").Append(std::to_string(item.port)).Append("]");
			int n = connectComboBox->Append(connnectName, bitmap, data);
			if (databaseSupplier->runtimeUserConnect && databaseSupplier->runtimeUserConnect->id == item.id) {
				nSelItem = n;
			}
		}
		if (nSelItem == -1) {
			return;
		}

		connectComboBox->SetSelection(nSelItem);
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK | wxCENTRE | wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}
}

void DatabaseDialogDelegate::loadForCharsetComboBox(wxComboBox* charsetComboBox, uint64_t connectId, const std::string & defval)
{
	assert(charsetComboBox != nullptr);

	try {
		auto charsetList = metadataService->getCharsets(databaseSupplier->runtimeUserConnect->id);

		charsetComboBox->Clear();
		charsetComboBox->AppendString(S("default"));
		int n = 0, nSelItem = 0;
		for (auto& item : charsetList) {
			auto data = new QClientData<CharsetInfo>(connectId, new CharsetInfo(item));
			n = charsetComboBox->Append(item.name, data);
			if (item.name == defval) {
				nSelItem = n;
			}
		}
		charsetComboBox->SetSelection(nSelItem);
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK | wxCENTRE | wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}

	
}

void DatabaseDialogDelegate::loadForCollationComboBox(wxComboBox* collationComboBox, uint64_t connectId, const std::string& charset, const std::string & defval)
{
	assert(collationComboBox != nullptr);

	collationComboBox->Clear();
	collationComboBox->AppendString(S("default"));

	int n = 0, nSelItem = 0;
	if (charset.empty()) {
		collationComboBox->SetSelection(0);
		return;
	}

	try {
		auto list = metadataService->getCollations(databaseSupplier->runtimeUserConnect->id, charset);

		int n = 0, nSelItem = 0;
		for (auto& item : list) {
			auto data = new QClientData<CollationInfo>(connectId, new CollationInfo(item));
			n = collationComboBox->Append(item.name, data);
			if (item.name == defval) {
				nSelItem = n;
			}
		}
		collationComboBox->SetSelection(nSelItem);
	} catch (QRuntimeException& ex) {
		wxMessageDialog msgbox(view, S("connect-fail").append(",Error:").append(ex.getMsg()), S("error-notice"), wxOK | wxCENTRE | wxICON_ERROR);
		msgbox.ShowModal();
		return;
	}	
}
