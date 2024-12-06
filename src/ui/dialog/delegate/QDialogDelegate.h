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
 * @file   QDialogDelegate.h
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-12-05
 *********************************************************************/
#pragma once
#include <wx/bmpcbox.h>
#include <cassert>
#include <wx/msgdlg.h>
#include "core/common/Lang.h"
#include "ui/common/data/QClientData.h"
#include "ui/common/delegate/QDelegate.h"
#include "core/service/db/ConnectService.h"
#include "core/service/db/DatabaseService.h"
#include "core/service/db/MetadataService.h"
#include "ui/database/supplier/DatabaseSupplier.h"
#include "ui/common/msgbox/QAnimateBox.h"

template <typename T, typename S = EmptySupplier, typename V = wxWindow>
class QDialogDelegate : public QDelegate<T, S, V>
{
public:
	void loadForConnectComboBox(wxBitmapComboBox * connectComboBox);
	void loadForSchemaComboBox(wxBitmapComboBox * schemaComboBox, uint64_t connectId, const std::string & defval = "");
	void loadForCharsetComboBox(wxComboBox * charsetComboBox, uint64_t connectId, const std::string & defval = "");
	void loadForCollationComboBox(wxComboBox * collationComboBox, uint64_t connectId, const std::string& charset, const std::string & defval = "");
protected:
	DatabaseSupplier * databaseSupplier = DatabaseSupplier::getInstance();
	ConnectService* connectService = ConnectService::getInstance();
	DatabaseService* databaseService = DatabaseService::getInstance();
	MetadataService* metadataService = MetadataService::getInstance();
};


template <typename T, typename S, typename V>
void QDialogDelegate<T, S, V>::loadForConnectComboBox(wxBitmapComboBox* connectComboBox)
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
		QAnimateBox::error(ex);
		return;
	}
}


template <typename T, typename S /*= EmptySupplier*/, typename V /*= wxWindow*/>
void QDialogDelegate<T, S, V>::loadForSchemaComboBox(wxBitmapComboBox* schemaComboBox, uint64_t connectId, const std::string& defval /*= ""*/)
{
	assert(schemaComboBox != nullptr);

	try {
		auto schemaList = databaseService->getAllUserDbs(databaseSupplier->runtimeUserConnect->id);

		schemaComboBox->Clear();
		schemaComboBox->AppendString(S("default"));
		int n = 0, nSelItem = 0;
		for (auto& item : schemaList) {
			auto data = new QClientData<CharsetInfo>(connectId, new UserDb(item));
			n = schemaComboBox->Append(item.name, data);
			if (item.name == defval) {
				nSelItem = n;
			}
		}
		schemaComboBox->SetSelection(nSelItem);
	} catch (QRuntimeException& ex) {
		QAnimateBox::error(ex);
		return;
	}
}

template <typename T, typename S, typename V>
void QDialogDelegate<T, S, V>::loadForCharsetComboBox(wxComboBox* charsetComboBox, uint64_t connectId, const std::string & defval)
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
		QAnimateBox::error(ex);
		return;
	}
}

template <typename T, typename S, typename V>
void QDialogDelegate<T, S, V>::loadForCollationComboBox(wxComboBox* collationComboBox, uint64_t connectId, const std::string& charset, const std::string & defval)
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
		QAnimateBox::error(ex);
		return;
	}	
}
