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
#include <wx/window.h>
#include "ui/common/supplier/EmptySupplier.h"

/**
 * template T - subclass, S - subclass of QSupplier ,V - dataView class such as 
 */
template <typename T,typename S = EmptySupplier, typename V = wxWindow>
class QDelegate
{
public:
	static T * getInstance(V * view = nullptr, S * supplier = nullptr); // singleton
	static void destroyInstance();
protected:	
	static T * theInstance;
	int parentId;
	V* view;
	S* supplier = nullptr;
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

	QDelegate::theInstance->view = view;
		
	if (supplier && QDelegate::theInstance->supplier) {
		S::destroyInstance();
		QDelegate::theInstance->supplier = supplier;
	}
	
	if (view) {
		QDelegate::theInstance->parentId = view->GetId();
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

