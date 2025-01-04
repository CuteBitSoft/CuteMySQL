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
 * @file   ObjectsPage.cpp
 * @brief  
 * 
 * @author Xuehan Qin (qinxuehan2018@gmail.com) 
 * @date   2025-01-02
 *********************************************************************/

#include "ObjectsPage.h"
#include "common/Config.h"
#include "core/common/Lang.h"
#include "ui/database/rightview/page/delegate/objects/ObjectsPageTableDelegate.h"
#include "ui/database/rightview/page/delegate/objects/ObjectsPageViewDelegate.h"
#include "ui/database/rightview/page/delegate/objects/ObjectsPageProcedureDelegate.h"
#include "ui/database/rightview/page/delegate/objects/ObjectsPageFunctionDelegate.h"
#include "ui/database/rightview/page/delegate/objects/ObjectsPageTriggerDelegate.h"
#include "ui/database/rightview/page/delegate/objects/ObjectsPageEventDelegate.h"

IMPLEMENT_DYNAMIC_CLASS(ObjectsPage, wxPanel)

BEGIN_EVENT_TABLE(ObjectsPage, wxPanel)
	EVT_TOGGLEBUTTON(Config::OBJECTS_TABLE_BUTTON_ID, OnClickObjectButton)
	EVT_TOGGLEBUTTON(Config::OBJECTS_VIEW_BUTTON_ID, OnClickObjectButton)
	EVT_TOGGLEBUTTON(Config::OBJECTS_PROCEDURE_BUTTON_ID, OnClickObjectButton)
	EVT_TOGGLEBUTTON(Config::OBJECTS_FUNCTION_BUTTON_ID, OnClickObjectButton)
	EVT_TOGGLEBUTTON(Config::OBJECTS_TRIGGER_BUTTON_ID, OnClickObjectButton)
	EVT_TOGGLEBUTTON(Config::OBJECTS_EVENT_BUTTON_ID, OnClickObjectButton)
END_EVENT_TABLE()

// page and button relationship
const std::map<ObjectsPageType, Config::ButtonId>  ObjectsPage::pageButtonMap{
	{ ObjectsPageType::TABLE_OBJECTS, Config::OBJECTS_TABLE_BUTTON_ID },
	{ ObjectsPageType::VIEW_OBJECTS, Config::OBJECTS_VIEW_BUTTON_ID },
	{ ObjectsPageType::PROCEDURE_OBJECTS, Config::OBJECTS_PROCEDURE_BUTTON_ID },
	{ ObjectsPageType::FUNCTION_OBJECTS, Config::OBJECTS_FUNCTION_BUTTON_ID },
	{ ObjectsPageType::TRIGGER_OBJECTS, Config::OBJECTS_TRIGGER_BUTTON_ID },
	{ ObjectsPageType::EVENT_OBJECTS, Config::OBJECTS_EVENT_BUTTON_ID }
};

ObjectsPage::ObjectsPage(ObjectsPageType type /*= TABLE_OBJECTS*/)
	: QTabPage<ObjectsPageSupplier>()
{
	supplier->runtimeObjectType = type;
	init();
}

ObjectsPage::~ObjectsPage()
{
	if (objectsImageList) {
		delete objectsImageList;
		objectsImageList = nullptr;
	}

	ObjectsPageTableDelegate::destroyInstance();
	ObjectsPageViewDelegate::destroyInstance();
	ObjectsPageProcedureDelegate::destroyInstance();
	ObjectsPageFunctionDelegate::destroyInstance();
	ObjectsPageTriggerDelegate::destroyInstance();
	ObjectsPageEventDelegate::destroyInstance();
}

/**
 * Refresh the datas of list view.
 * 
 * @param pageType
 */
void ObjectsPage::refreshListView(ObjectsPageType pageType)
{
	if (supplier->runtimeObjectType == pageType) {
		refreshObjects();
		return;
	}

	// change the selected button 
	size_t n = objectButtonPtrs.size();
	auto selBtnId = pageButtonMap.at(pageType);
	for (size_t i = 0; i < n; ++i) {
		auto ptr = objectButtonPtrs.at(i);
		if (ptr->GetId() == selBtnId) {
			ptr->SetValue(true);
		} else {
			ptr->SetValue(false);
		}
	}

	supplier->runtimeObjectType = pageType;
	loadControls();
}

void ObjectsPage::init()
{
	bkgColor = wxColour(30, 31, 34, 30);
}

void ObjectsPage::createControls()
{
	createLayouts(); 
	createToolbarButtons();
	createListView();
	createStatusBar();
}

void ObjectsPage::loadControls()
{
	if (supplier->runtimeObjectType == TABLE_OBJECTS) {
		auto delegate = ObjectsPageTableDelegate::getInstance(listView, supplier);
		delegate->loadListView();
		delegate->loadStatusBar(statusBar);
	} else if (supplier->runtimeObjectType == VIEW_OBJECTS) {
		auto delegate = ObjectsPageViewDelegate::getInstance(listView, supplier);
		delegate->loadListView();
		delegate->loadStatusBar(statusBar);
	} else if (supplier->runtimeObjectType == PROCEDURE_OBJECTS) {
		auto delegate = ObjectsPageProcedureDelegate::getInstance(listView, supplier);
		delegate->loadListView();
		delegate->loadStatusBar(statusBar);
	} else if (supplier->runtimeObjectType == FUNCTION_OBJECTS) {
		auto delegate = ObjectsPageFunctionDelegate::getInstance(listView, supplier);
		delegate->loadListView();
		delegate->loadStatusBar(statusBar);
	} else if (supplier->runtimeObjectType == TRIGGER_OBJECTS) {
		auto delegate = ObjectsPageTriggerDelegate::getInstance(listView, supplier);
		delegate->loadListView();
		delegate->loadStatusBar(statusBar);
	} else if (supplier->runtimeObjectType == EVENT_OBJECTS) {
		auto delegate = ObjectsPageEventDelegate::getInstance(listView, supplier);
		delegate->loadListView();
		delegate->loadStatusBar(statusBar);
	}
}

void ObjectsPage::refreshObjects()
{
	if (supplier->runtimeObjectType == TABLE_OBJECTS) {
		auto delegate = ObjectsPageTableDelegate::getInstance(listView, supplier);
		delegate->loadObjects();
		delegate->loadStatusBar(statusBar);
	} else if (supplier->runtimeObjectType == VIEW_OBJECTS) {
		auto delegate = ObjectsPageViewDelegate::getInstance(listView, supplier);
		delegate->loadObjects();
		delegate->loadStatusBar(statusBar);
	} else if (supplier->runtimeObjectType == PROCEDURE_OBJECTS) {
		auto delegate = ObjectsPageProcedureDelegate::getInstance(listView, supplier);
		delegate->loadObjects();
		delegate->loadStatusBar(statusBar);
	} else if (supplier->runtimeObjectType == FUNCTION_OBJECTS) {
		auto delegate = ObjectsPageFunctionDelegate::getInstance(listView, supplier);
		delegate->loadObjects();
		delegate->loadStatusBar(statusBar);
	} else if (supplier->runtimeObjectType == TRIGGER_OBJECTS) {
		auto delegate = ObjectsPageTriggerDelegate::getInstance(listView, supplier);
		delegate->loadObjects();
		delegate->loadStatusBar(statusBar);
	} else if (supplier->runtimeObjectType == EVENT_OBJECTS) {
		auto delegate = ObjectsPageEventDelegate::getInstance(listView, supplier);
		delegate->loadObjects();
		delegate->loadStatusBar(statusBar);
	}

}

void ObjectsPage::OnClickObjectButton(wxCommandEvent& event)
{
	auto btnId = event.GetId();
	auto winptr = FindWindow(btnId);	

	size_t n = objectButtonPtrs.size();
	for (size_t i = 0; i < n; ++i) {
		auto ptr = objectButtonPtrs.at(i);
		if (ptr == winptr) {
			ptr->SetValue(true);
		} else {
			ptr->SetValue(false);
		}
	}
	auto oldObjectType = supplier->runtimeObjectType;
	auto iter = std::find_if(pageButtonMap.begin(), pageButtonMap.end(), [&winptr](auto& pair) {
		return pair.second == winptr->GetId();
	});
	if (iter == pageButtonMap.end()) {
		return;
	}
	supplier->runtimeObjectType = iter->first;
	/*switch (winptr->GetId())
	{
	case Config::OBJECTS_VIEW_BUTTON_ID:
		supplier->runtimeObjectType = VIEW_OBJECTS;
		break;
	case Config::OBJECTS_PROCEDURE_BUTTON_ID:
		supplier->runtimeObjectType = PROCEDURE_OBJECTS;
		break;
	case Config::OBJECTS_FUNCTION_BUTTON_ID:
		supplier->runtimeObjectType = FUNCTION_OBJECTS;
		break;
	case Config::OBJECTS_TRIGGER_BUTTON_ID:
		supplier->runtimeObjectType = TRIGGER_OBJECTS;
		break;
	case Config::OBJECTS_EVENT_BUTTON_ID:
		supplier->runtimeObjectType = EVENT_OBJECTS;
		break;
	case Config::OBJECTS_TABLE_BUTTON_ID:
	default:
		supplier->runtimeObjectType = TABLE_OBJECTS;
		break;
	}*/

	if (oldObjectType != supplier->runtimeObjectType) {
		loadControls();
	}	
}

void ObjectsPage::createLayouts()
{
	topSizer->AddSpacer(5);

	toolbarHoriLayout = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(toolbarHoriLayout, 0, wxALL | wxEXPAND);
}

void ObjectsPage::createToolbarButtons()
{
	imgdir = ResourceUtil::getProductImagesDir() +"/database/objects/button/";

	ObjectsPageType type = supplier->runtimeObjectType;

	refreshButton = createBitmapButton(Config::LISTVIEW_REFRESH_BUTTON_ID, imgdir, "refresh", S("refresh"), { 22, 22 });
	toolbarHoriLayout->Add(refreshButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	auto splitter = createToolSplit();
	toolbarHoriLayout->Add(splitter, 0, wxALIGN_TOP | wxALIGN_LEFT);
	
	tableButton = createBitmapToggleButton(Config::OBJECTS_TABLE_BUTTON_ID, S("tables"), S("tables"), 
		imgdir, "table", {-1, 22}, type == TABLE_OBJECTS);
	toolbarHoriLayout->Add(tableButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	toolbarHoriLayout->AddSpacer(5);
	viewButton = createBitmapToggleButton(Config::OBJECTS_VIEW_BUTTON_ID, S("views"), S("views"), 
		imgdir, "view", { -1, 22 }, type == VIEW_OBJECTS);
	toolbarHoriLayout->Add(viewButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	toolbarHoriLayout->AddSpacer(5);
	procedureButton = createBitmapToggleButton(Config::OBJECTS_PROCEDURE_BUTTON_ID, S("store-procedures"), S("store-procedures"), 
		imgdir, "procedure", { -1, 22 }, type == PROCEDURE_OBJECTS);
	toolbarHoriLayout->Add(procedureButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	toolbarHoriLayout->AddSpacer(5);
	functionButton = createBitmapToggleButton(Config::OBJECTS_FUNCTION_BUTTON_ID, S("functions"), S("functions"), 
		imgdir, "function", { -1, 22 }, type == FUNCTION_OBJECTS);
	toolbarHoriLayout->Add(functionButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	toolbarHoriLayout->AddSpacer(5);
	triggerButton = createBitmapToggleButton(Config::OBJECTS_TRIGGER_BUTTON_ID, S("triggers"), S("triggers"), 
		imgdir, "trigger", { -1, 22 }, type == TRIGGER_OBJECTS);
	toolbarHoriLayout->Add(triggerButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	toolbarHoriLayout->AddSpacer(5);
	eventButton = createBitmapToggleButton(Config::OBJECTS_EVENT_BUTTON_ID, S("events"), S("events"), 
		imgdir, "event", { -1, 22 }, type == EVENT_OBJECTS);
	toolbarHoriLayout->Add(eventButton, 0, wxALIGN_TOP | wxALIGN_LEFT);

	objectButtonPtrs.push_back(tableButton);
	objectButtonPtrs.push_back(viewButton);
	objectButtonPtrs.push_back(procedureButton);
	objectButtonPtrs.push_back(functionButton);
	objectButtonPtrs.push_back(triggerButton);
	objectButtonPtrs.push_back(eventButton);
}

void ObjectsPage::createImageList()
{
	if (objectsImageList && objectsImageList->GetImageCount()) {
		return;
	}
	imgdir = ResourceUtil::getProductImagesDir() +"/database/objects/list/";
	objectsImageList = new wxImageList(16, 16, true, 6);
	objectsImageList->Add(wxBitmap(imgdir + "table.png", wxBITMAP_TYPE_PNG)); // 0 - table
	objectsImageList->Add(wxBitmap(imgdir + "view.png", wxBITMAP_TYPE_PNG)); // 1 - view
	objectsImageList->Add(wxBitmap(imgdir + "procedure.png", wxBITMAP_TYPE_PNG)); // 2 - procedure
	objectsImageList->Add(wxBitmap(imgdir + "function.png", wxBITMAP_TYPE_PNG)); // 3 - function
	objectsImageList->Add(wxBitmap(imgdir + "trigger.png", wxBITMAP_TYPE_PNG)); // 4 - trigger
	objectsImageList->Add(wxBitmap(imgdir + "event.png", wxBITMAP_TYPE_PNG)); // 5 - event
}

void ObjectsPage::createListView()
{
	listView = new QListView();	

	// virtual style: wxLC_VIRTUAL
	listView->Create(this, Config::DATABASE_OBJECTS_LISTVIEW_ID, wxDefaultPosition, wxDefaultSize, 
		wxCLIP_CHILDREN | wxNO_BORDER | wxLC_REPORT | wxLC_ALIGN_LEFT );
	listView->SetBackgroundColour(bkgColor);
	listView->SetTextColour(textColor);
	
	topSizer->Add(listView, 1, wxEXPAND);

	createImageList();
	listView->SetImageList(objectsImageList, wxIMAGE_LIST_SMALL);
}

void ObjectsPage::createStatusBar()
{
	statusBar = new wxStatusBar();
	statusBar->Create(this, Config::DATABASE_OBJECTS_STATUSBAR_ID, 
		wxCLIP_CHILDREN | wxNO_BORDER | wxSTB_SIZEGRIP | wxSTB_SHOW_TIPS | wxSTB_ELLIPSIZE_END | wxFULL_REPAINT_ON_RESIZE);
	const int widths[4]{ -3, -1, -1, 350 };
	statusBar->SetFieldsCount(4, widths);
	statusBar->SetStatusWidths(4, widths);
	//statusBar->SetBackgroundColour(bkgColor);
	//statusBar->SetForegroundColour(textColor);
	topSizer->Add(statusBar, 0, wxALIGN_TOP | wxEXPAND);
}


