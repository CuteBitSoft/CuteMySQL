#include "AppContext.h"
#include <sstream>
AppContext * AppContext::theInstance = nullptr;

AppContext * AppContext::getInstance()
{
	if (AppContext::theInstance == nullptr) {
		AppContext::theInstance = new AppContext();
	}

	return AppContext::theInstance;
}

void AppContext::destroyInstance()
{
	if (AppContext::theInstance) {
		delete AppContext::theInstance;
		AppContext::theInstance = nullptr;
	}
}


void AppContext::setMainFrmWindow(wxWindow * hwnd)
{
	this->mainFrmWindow = hwnd;
}

wxWindow * AppContext::getMainFrmWindow()
{
	return mainFrmWindow;
}

std::string AppContext::getMainFrmCaption()
{
	return std::string();
}

void AppContext::appendMainFrmCaption(const std::string & text)
{
	
}

std::string AppContext::get(const std::string k)
{
	return settings.at(k);
}


int AppContext::getInt(std::string k)
{
	std::string str = settings.at(k);
	if (str.empty()) {
		return 0;
	}
	int v = std::stoi(str.c_str());
	return v;
}

void AppContext::set(const std::string k, const std::string v)
{
	settings[k] = v;
}


void AppContext::set(std::string k, int v)
{
	std::stringstream ss; 
	ss << v;
	settings[k] = ss.str();
}

AppContext::AppContext()
{

}

AppContext::~AppContext()
{
	clearPopAnimatePtrs();
	clearDialogPtrs();
}

/**
 * 消息分发（无返回）.
 * 
 * @param msgId 消息ID
 * @param wParam 参数1
 * @param lParam 参数2
 */
void AppContext::dispatch(uint64_t msgId, uint64_t wParam, uint32_t lParam)
{
	msgDispatcher.dispatch(msgId, wParam, lParam);
}

/**
 * 消息分发（有返回，等待完成）.
 * 
 * @param msgId 消息ID
 * @param wParam 参数1
 * @param lParam 参数2
 */
uint64_t AppContext::dispatchForResponse(uint64_t msgId, uint64_t wParam /*= NULL*/, uint32_t lParam /*= NULL*/)
{
	return msgDispatcher.dispatchForResponse(msgId, wParam, lParam);
}

/**
 * 消息订阅者.
 * 
 * @param msgId 消息ID
 */
void AppContext::subscribe(wxWindow * hwnd, uint64_t msgId)
{
	msgDispatcher.subscribe(hwnd, msgId);
}

/**
 * 取消订阅.
 * 
 * @param hwnd 窗口句柄
 * @param msgId 消息ID
 */
void AppContext::unsubscribe(wxWindow * hwnd, uint64_t msgId)
{
	msgDispatcher.unsuscribe(hwnd, msgId);
}

/**
 * 取消窗口的所有订阅.
 * 
 * @param hwnd 窗口句柄
 */
void AppContext::unsuscribeAll(wxWindow * hwnd)
{
	msgDispatcher.unsuscribeAll(hwnd);
}

std::vector<wxWindow *> & AppContext::getPopAnimatePtrs()
{
	return popAnimatePtrs;
}

void AppContext::addPopAnimatePtr(wxWindow * ptr)
{
	popAnimatePtrs.push_back(ptr);
}

void AppContext::erasePopAnimatePtr(wxWindow * ptr)
{
	auto iter = popAnimatePtrs.begin();
	for (; iter != popAnimatePtrs.end(); iter++) {
		if ((*iter)->GetId() != ptr->GetId()) {
			continue;
		}
		popAnimatePtrs.erase(iter);
		break;
	}
}

void AppContext::clearPopAnimatePtrs()
{
	for (auto & ptr : popAnimatePtrs) {
		if (ptr && ptr->IsShown()) {
			ptr->Close(true);
			ptr->Destroy();
		}
	}
	popAnimatePtrs.clear();
}

std::vector<wxWindow *> & AppContext::getDialogPtrs()
{
	return dialogPtrs;
}

void AppContext::addDialogPtr(wxWindow * ptr)
{
	dialogPtrs.push_back(ptr);
}

void AppContext::eraseDialogPtr(wxWindow * ptr)
{
	auto iter = dialogPtrs.begin();
	for (; iter != dialogPtrs.end(); iter++) {
		if ((*iter)->GetId() != ptr->GetId()) {
			continue;
		}
		dialogPtrs.erase(iter);
		break;
	}
}

void AppContext::clearDialogPtrs()
{
	dialogPtrs.clear();
}
