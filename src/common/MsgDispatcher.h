#pragma once
#include <list>
#include <unordered_map>
#include <wx/window.h>

typedef std::list<wxWindow *> WindowList;

class MsgDispatcher
{
public:
	MsgDispatcher();
	~MsgDispatcher();

	void dispatch(uint64_t msgId, uint64_t wParam = NULL, uint32_t lParam = NULL);
	uint64_t dispatchForResponse(uint64_t msgId, uint64_t wParam = NULL, uint32_t lParam = NULL);

	void subscribe(wxWindow * hwnd, uint64_t msgId);
	void unsuscribe(wxWindow * hwnd, uint64_t msgId);
	void unsuscribeAll(wxWindow * hwnd);
private:
	std::unordered_map<uint64_t, WindowList> msgMap; 
};
