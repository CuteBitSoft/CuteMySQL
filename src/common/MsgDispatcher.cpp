#include "MsgDispatcher.h"
#include <algorithm>
#include <wx/event.h>
#include <boost/scoped_ptr.hpp>
#include "MsgClientData.h"
#include "common/event/MsgDispatcherEvent.h"
//#include "utils/Log.h"

MsgDispatcher::MsgDispatcher()
{
	msgMap.clear();
}

MsgDispatcher::~MsgDispatcher()
{

}

void MsgDispatcher::dispatch(uint64_t msgId, uint64_t wParam /*= NULL*/, uint32_t lParam /*= NULL*/)
{
	if (msgMap.empty() || msgMap.find(msgId) == msgMap.end()) {
		return ;
	}

	WindowList & winList = msgMap.at(msgId);
	std::for_each(winList.begin(), winList.end(), [&msgId, &wParam, &lParam](wxWindow * win) {
		//::PostMessage(hwnd, msgId, wParam, lParam);
		boost::scoped_ptr<MsgClientData> clientData(new MsgClientData(msgId, wParam));
		MsgDispatcherEvent event(wxEVT_NOTITY_MESSAGE_HANDLE, msgId);
		event.SetClientData(clientData.get());

		event.SetEventObject(win);
		win->GetEventHandler()->ProcessEvent(event);
	});
	
}

uint64_t MsgDispatcher::dispatchForResponse(uint64_t msgId, uint64_t wParam /*= NULL*/, uint32_t lParam /*= NULL*/)
{
	if (msgMap.empty() || msgMap.find(msgId) == msgMap.end()) {
		return 0;
	}

	WindowList & winList = msgMap.at(msgId);
	uint64_t result = 1;
	std::for_each(winList.begin(), winList.end(), [&msgId, &wParam, &lParam, &result](wxWindow * hwnd) {
		uint64_t response = 0; // = ::SendMessage(hwnd, msgId, wParam, lParam);
		// correct result must be 1, other wise be failed, and return
		if (response != 1) {
			result = 0;
		}
	});
	return result;
}

void MsgDispatcher::subscribe(wxWindow * hwnd, uint64_t msgId)
{
	if (msgMap.empty() || msgMap.find(msgId) == msgMap.end()) {
		WindowList winList = { hwnd };
		msgMap.insert(std::pair<uint64_t, WindowList>(msgId, winList));
		return ;
	}
	
	WindowList & winList = msgMap.at(msgId);
	if (winList.empty()) {
		winList.push_back(hwnd);
		return ;
	}

	
	auto hwndIterator = std::find(winList.begin(), winList.end(), hwnd);
	if (hwndIterator == winList.end()) {
		winList.push_back(hwnd);
	}
}

void MsgDispatcher::unsuscribe(wxWindow * hwnd, uint64_t msgId)
{
	if (msgMap.empty()) {
		return ;
	}

	auto iterator = msgMap.find(msgId);
	if (iterator == msgMap.end()) {
		return ;
	}

	WindowList & winList = msgMap.at(msgId);
	if (winList.empty()) {
		return ;
	}

	
	winList.erase(std::remove_if(winList.begin(), winList.end(), [&hwnd](wxWindow * _hwnd) -> bool {
		return hwnd == _hwnd;
	}), winList.end());
}

void MsgDispatcher::unsuscribeAll(wxWindow * hwnd)
{
	if (msgMap.empty()) {
		return ;
	}

	
	for (auto iterator = msgMap.begin(); iterator != msgMap.end(); iterator++) {
		WindowList & winList = (*iterator).second;
		if (winList.empty()) {
			return;
		}

		
		winList.erase(std::remove_if(winList.begin(), winList.end(), [&hwnd](wxWindow * _hwnd) -> bool {
			return hwnd == _hwnd;
		}), winList.end());
	}
}
