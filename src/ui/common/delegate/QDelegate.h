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
 * @file   QDelegate.h
 * @brief  USE view/supplier design pattern
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2024-11-13
 *********************************************************************/
#pragma once
#include <cassert>
#include <wx/window.h>
#include <wx/bmpcbox.h>
#include <wx/msgdlg.h>
#include "core/entity/Entity.h"
#include "core/common/Lang.h"
#include "utils/ResourceUtil.h"
#include "ui/common/data/QClientData.h"
#include "ui/common/supplier/EmptySupplier.h"
#include "core/service/db/DatabaseService.h"
#include "core/service/db/ConnectService.h"
#include "core/service/db/MetadataService.h"
#include "ui/common/msgbox/QAnimateBox.h"

/**
 * template T - subclass, S - subclass of QSupplier ,V - dataView class such as 
 */
template <typename T, typename S = EmptySupplier, typename V = wxWindow>
class QDelegate
{
public:
	static T * getInstance(V * view = nullptr, S * supplier = nullptr); // singleton
	static void destroyInstance();

	// load common comboBox
	void loadForConnectComboBox(wxBitmapComboBox * connectComboBox, uint64_t defVal = 0);
	void loadForSchemaComboBox(wxBitmapComboBox * schemaComboBox, uint64_t connectId, const std::string & defval = "");
	void loadForCharsetComboBox(wxComboBox * charsetComboBox, uint64_t connectId, const std::string & defval = "");
	void loadForCollationComboBox(wxComboBox * collationComboBox, uint64_t connectId, const std::string& charset, const std::string & defval = "");
protected:	
	static T * theInstance;
	wxWindow * parentWin;
	V* view;
	S* supplier = nullptr;

	ConnectService* connectService = ConnectService::getInstance();
	DatabaseService* databaseService = DatabaseService::getInstance();
	MetadataService* metadataService = MetadataService::getInstance();
};


template <typename T,typename S, typename V>
T * QDelegate<T, S, V>::theInstance = nullptr; // singleton

// singleton
template <typename T,typename S, typename V>
T * QDelegate<T, S, V>::getInstance(V * view, S * supplier)
{
	if (QDelegate::theInstance == nullptr) {
		QDelegate::theInstance = new T();
	}

	QDelegate::theInstance->parentWin = nullptr;
	QDelegate::theInstance->view = view;
		
	if (supplier) {
		if (QDelegate::theInstance->supplier && QDelegate::theInstance->supplier != supplier) {
			S::destroyInstance();
		}
		QDelegate::theInstance->supplier = supplier;
	}
	
	if (!QDelegate::theInstance->supplier) {
		QDelegate::theInstance->supplier = S::getInstance();
	}
		
	return QDelegate::theInstance;
}


template <typename T, typename S /*= EmptySupplier*/, typename V /*= wxWindow*/>
void QDelegate<T, S, V>::destroyInstance()
{
	S::destroyInstance();

	if (QDelegate::theInstance != nullptr) {
		QDelegate::theInstance->supplier = nullptr;

		delete QDelegate::theInstance;
		QDelegate::theInstance = nullptr;
	}

	
}


template <typename T, typename S, typename V>
void QDelegate<T, S, V>::loadForConnectComboBox(wxBitmapComboBox* connectComboBox, uint64_t defVal)
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
			if (defVal == item.id) {
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


template <typename T, typename S, typename V>
void QDelegate<T, S, V>::loadForSchemaComboBox(wxBitmapComboBox* schemaComboBox, uint64_t connectId, const std::string& defval /*= ""*/)
{
	assert(schemaComboBox != nullptr);

	try {
		auto schemaList = databaseService->getAllUserDbs(connectId);

		schemaComboBox->Clear();
		
		auto imgdir = ResourceUtil::getProductImagesDir();
		wxString path = imgdir + "/dialog/database/database.bmp";
		wxBitmap bitmap(path, wxBITMAP_TYPE_BMP);

		int n = 0, nSelItem = 0;
		for (auto& item : schemaList) {
			auto data = new QClientData<UserDb>(connectId, new UserDb(item));
			n = schemaComboBox->Append(item.name, bitmap, data);
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
void QDelegate<T, S, V>::loadForCharsetComboBox(wxComboBox* charsetComboBox, uint64_t connectId, const std::string & defval)
{
	assert(charsetComboBox != nullptr);

	try {
		auto charsetList = metadataService->getCharsets(connectId);

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
void QDelegate<T, S, V>::loadForCollationComboBox(wxComboBox* collationComboBox, uint64_t connectId, const std::string& charset, const std::string & defval)
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
		auto list = metadataService->getCollations(connectId, charset);

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