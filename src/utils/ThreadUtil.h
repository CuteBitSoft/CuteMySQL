#ifndef _THREAD_UTIL_H
#define _THREAD_UTIL_H

#include <thread>

class ThreadUtil
{
public:
	static uint32_t currentThreadId() {
		std::thread::id id = std::this_thread::get_id();
		return *(unsigned int*)&id;
	}

};


#endif