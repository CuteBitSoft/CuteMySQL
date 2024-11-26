/*****************************************************************//**
 * @file   AppContext.h
 * @brief  全局上下文
 * @detail    $DETAIL
 * 
 * @author 覃学涵
 * @date   2023-03-14
 *********************************************************************/
#pragma once
#include <wx/window.h>
#include <string>
#include <unordered_map>
#include "MsgDispatcher.h"

class AppContext
{
public:
	~AppContext();

	static AppContext * getInstance();
	static void destroyInstance();

	// mainFrm的窗口句柄
	void setMainFrmWindow(wxWindow * hwnd);
	wxWindow * getMainFrmWindow();

	std::string getMainFrmCaption();
	void appendMainFrmCaption(const std::string & text);

	// HomeView的窗口句柄
	void setHomeViewWindow(wxWindow * hwnd) { homeViewWindow = hwnd; };
	wxWindow * getHomeViewWindow() { return homeViewWindow; };

	std::string get(std::string k);
	int getInt(std::string k);
	void set(std::string k, std::string v);
	void set(std::string k, int v);

	// 消息分发(无返回)
	void dispatch(uint64_t msgId, uint64_t wParam = NULL, uint32_t lParam = NULL);
	// 消息分发(有返回，等待完成)
	uint64_t dispatchForResponse(uint64_t msgId, uint64_t wParam = NULL, uint32_t lParam = NULL);

	// 消息订阅
	void subscribe(wxWindow * hwnd, uint64_t msgId);
	void unsubscribe(wxWindow * hwnd, uint64_t msgId);
	void unsuscribeAll(wxWindow * hwnd);

	std::vector<wxWindow *> & getPopAnimatePtrs();
	void addPopAnimatePtr(wxWindow * ptr);
	void erasePopAnimatePtr(wxWindow * ptr);
	void clearPopAnimatePtrs();

	std::vector<wxWindow *> & getDialogPtrs();
	void addDialogPtr(wxWindow * ptr);
	void eraseDialogPtr(wxWindow * ptr);
	void clearDialogPtrs();
private:
	AppContext();

	
	//singleton
	static AppContext * theInstance; 

	// mainFrm的窗口句柄
	wxWindow * mainFrmWindow = nullptr;
	// HomeView的窗口句柄
	wxWindow * homeViewWindow = nullptr;

	// MessageBoxes and PopAnimates
	std::vector<wxWindow *> popAnimatePtrs;
	std::vector<wxWindow *> messageBoxPtrs;
	std::vector<wxWindow *> dialogPtrs;
	
	// 全局的配置
	std::unordered_map<std::string, std::string> settings;

	// 全局的消息调度器
	MsgDispatcher msgDispatcher;// 掌管全局消息的分发和订阅
};

